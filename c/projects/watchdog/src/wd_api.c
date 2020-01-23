/*******************************************************
*                                                      *
*      .oooooo.   ooooo            .ooo    .ooooo.     *
*     d8P'  `Y8b  `888'          .88'     888' `Y88.   *
*    888      888  888          d88'      888    888   *
*    888      888  888         d888P"Ybo.  `Vbood888   *
*    888      888  888         Y88[   ]88       888'   *
*    `88b    d88'  888       o `Y88   88P     .88P'    *
*     `Y8bood8P'  o888ooooood8  `88bod8'    .oP'       *
*                                                      *
*     PROJECTS: WATCH DOG (API)                        *
*     CODE BY:            SASHA LIMAREV                *
*     REVIEWED BY:        ANDREI PLAKHOTA              *
*******************************************************/
#include <sys/types.h> /* key_t */
#include <sys/ipc.h> /* ftok */
#include <sys/sem.h> /* semget */
#include <signal.h> /* sigaction */
#include <unistd.h> /* fork */
#include <stdlib.h> /* malloc */
#include <pthread.h> /* pthread_create */
#include <sys/wait.h> /* waitpid */
#include <stdio.h> /* sprintf */

#include "scheduler.h" /* ScdRun */
#include "watchdog.h" 

#define MAX_TRIES (3)
#define N_SEMS (2)
#define READ_WRITE (0600)
#define WD_READY (0)
#define APP_READY (1)
#define WD_PATH "./watchdog.out"
#define SEMID_STRLEN (20)
#define WD_INDEX (0)
#define REMOVE_TASK (-1)
#define SEMID_INDEX (1)
#define WD_ARGV_OFFSET (2)
#define FREQUENCY (2)
#define FAIL (-1)
#define MAX_ATTEMPTS_TIL_REVIVE (2)

static void StopHandler(int signal);
static void PingHandler(int signal);
static void *ThreadFunction(void *args);
static long SendPing(void *args);
static long CheckPong(void *args);
static void DestroyWDArgv();
static int SetSigHandlers();
static int SetSemid(char **argv, int wd_id);
static scd_t *InitScd();
static int InitWDArgv(int argc, char **argv);
static int CreateWDAndSetTarget();
static int InitSemsAndHandlers(char **argv, int wd_id);

static int g_semid = 0;
static pid_t g_target_pid = 0;
static char **g_wd_argv = NULL;
static pthread_t g_scd_thread = 0;
static volatile sig_atomic_t g_seconds_til_revive = MAX_ATTEMPTS_TIL_REVIVE;
static volatile int g_should_stop = 0;
static volatile sig_atomic_t g_can_send = 0;
static volatile sig_atomic_t g_got_confirm = 0;

static const struct sembuf g_post = {APP_READY, 1, 0};
static const struct sembuf g_wait = {WD_READY, -1, 0};

static void DestroyWDArgv()
{
    if(NULL != g_wd_argv)
    {
        free(g_wd_argv[1]);
        g_wd_argv[SEMID_INDEX] = NULL;
    }

	free(g_wd_argv);
    g_wd_argv = NULL;
}

static void StopHandler(int signal)
{
	(void)signal; 

	g_got_confirm = 1;
}

static void PingHandler(int signal)
{
	(void)signal; 

	g_seconds_til_revive = MAX_ATTEMPTS_TIL_REVIVE;
	g_can_send = 1;
}

static void *ThreadFunction(void *args)
{
	scd_t *scheduler = (scd_t *)args;

	ScdRun(scheduler);
	ScdDestroy(scheduler);

    scheduler = NULL;

	return NULL;
}

static long SendPing(void *args)
{
	(void)args;
	
    if(g_should_stop)
	{
		return REMOVE_TASK;
	}

	if(g_can_send)
	{
		kill(g_target_pid, SIGUSR1);
	}

	return 0;
}

static int ReviveWD()
{
	waitpid(g_target_pid, NULL, 0);

	if(FAIL == semop(g_semid, (struct sembuf *)&g_wait, 1))
	{
		return WD_E_EXEC;
	}

	g_target_pid = fork();
	if(0 == g_target_pid)
	{
		if(FAIL == execvp(WD_PATH, g_wd_argv))
		{
			return WD_E_EXEC; 
		}
	}

	if(FAIL == semop(g_semid, (struct sembuf *)&g_wait, 1))
	{
		return WD_E_EXEC;
	}

    g_seconds_til_revive = MAX_ATTEMPTS_TIL_REVIVE;

	return WD_OK;
}

static long CheckPong(void *args)
{
	if(g_should_stop)
	{
		return REMOVE_TASK;
	}

	(void)args;

	if(0 < g_seconds_til_revive)
	{
		--g_seconds_til_revive;
	}
	else 
	{
        ReviveWD();
	}

	return 0;
}

void WDStop()
{
    int sig_attempts = MAX_TRIES;
    int to_sleep = 0;

    g_should_stop = 1;

    for(to_sleep = MAX_TRIES; !g_got_confirm && 0 < sig_attempts; --sig_attempts)
    {
        kill(g_target_pid, SIGUSR2);

        while(!g_got_confirm && 0 < to_sleep)
        {
            to_sleep = sleep(to_sleep);
        }
    }

    kill(g_target_pid, SIGKILL);

    pthread_join(g_scd_thread, NULL);
	semctl(g_semid, 0, IPC_RMID);
	DestroyWDArgv();
}

static int SetSigHandlers()
{
    struct sigaction usr1_action = {0};
    struct sigaction usr2_action = {0};

    usr1_action.sa_handler = PingHandler;
    usr2_action.sa_handler = StopHandler;

    if(FAIL == sigaction(SIGUSR1, &usr1_action, NULL) ||
       FAIL == sigaction(SIGUSR2, &usr2_action, NULL))
    {
        return FAIL;
    }

    return WD_OK;
}

static int SetSemid(char **argv, int wd_id)
{
    key_t sems_key = ftok(argv[WD_INDEX], wd_id);
    int ret = WD_OK;

    if(FAIL == sems_key)
    {
        ret = WD_E_INVARGV;
    }

    if(WD_OK == ret)
    {
        g_semid = semget(sems_key, N_SEMS, IPC_CREAT | READ_WRITE);
        if(FAIL == g_semid)
        {
            ret = WD_E_SEM;
        }
    }

    return ret;
}

static scd_t *InitScd()
{
    scd_t *scheduler = ScdCreate();
    if(NULL == scheduler)
    {
        return NULL;
    }

    if(UIDIsBad(ScdAdd(scheduler, FREQUENCY, SendPing, NULL)))
    {
        ScdDestroy(scheduler);
        return NULL;
    } 

    if(UIDIsBad(ScdAdd(scheduler, FREQUENCY, CheckPong, NULL)))
    {
        ScdDestroy(scheduler);
        return NULL;
    } 

    return scheduler;
}

static int InitWDArgv(int argc, char **argv)
{
    int i = 0;

    g_wd_argv = malloc(sizeof(*g_wd_argv) * (argc + WD_ARGV_OFFSET + 1));
    if(NULL == g_wd_argv)
    {       
        return WD_E_MEM;
    }

    g_wd_argv[SEMID_INDEX] = (char *)malloc(sizeof(*g_wd_argv[SEMID_INDEX]) * SEMID_STRLEN);
    if(NULL == g_wd_argv[1])
    {
        DestroyWDArgv();
        return WD_E_MEM;
    }

    sprintf(g_wd_argv[SEMID_INDEX], "%d", g_semid);

    g_wd_argv[WD_INDEX] = WD_PATH;

    for(; argc > i; ++i)
    {
        g_wd_argv[i + WD_ARGV_OFFSET] = argv[i];
    }

    g_wd_argv[i + WD_ARGV_OFFSET] = NULL;

    return WD_OK;
}

static int CreateWDAndSetTarget()
{
    int wd_sem_val = semctl(g_semid, WD_READY, GETVAL);

    if(-1 == wd_sem_val)
    {
        return WD_E_SEM;
    }

    g_target_pid = getppid();

    if(0 == wd_sem_val) 
    {
        g_target_pid = fork();

        if(0 == g_target_pid)
        {
            execvp(WD_PATH, g_wd_argv);
            exit(1);
        }
        else if(FAIL == g_target_pid)
        {
            return WD_E_EXEC; 
        }
        else
        {
            if(FAIL == semop(g_semid, (struct sembuf *)&g_wait, 1))
            {
                return WD_E_SEM;
            }
        }
    }

    return WD_OK;
}

static int InitSemsAndHandlers(char **argv, int wd_id)
{
    int status = SetSemid(argv, wd_id);

    if(WD_OK != status)
    {
        return status;
    }

    if(FAIL == SetSigHandlers()) 
    {
        return WD_E_SIGACT;
    }

    return WD_OK;
}

int WDStart(int argc, char **argv, int wd_id)
{
	scd_t *scheduler = NULL;
	int status = 0;

    status = InitSemsAndHandlers(argv, wd_id);

	if(NULL == g_wd_argv)
	{
		if(WD_OK != InitWDArgv(argc, argv))
        {
            return WD_E_MEM;
        }
	}

    status = CreateWDAndSetTarget();
	if(WD_OK != status)
    {
        DestroyWDArgv();
        return status;
    }

    scheduler = InitScd();
    if(NULL == scheduler)
    {
        DestroyWDArgv();
        return WD_E_MEM;
    }

	if(-1 == pthread_create(&g_scd_thread, NULL, ThreadFunction, scheduler))
	{
        ScdDestroy(scheduler);
        DestroyWDArgv();
		return WD_E_THREAD;
	}

	if(-1 == semop(g_semid, (struct sembuf *)&g_post, 1))
	{
        ScdDestroy(scheduler);
        InitWDArgv(argc, argv);
		return WD_E_SEM;
	}

	return WD_OK;
}
