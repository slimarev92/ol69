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
*     PROJECTS: WATCH DOG (PROGRAM)                    *
*     CODE BY:            SASHA LIMAREV                *
*     REVIEWED BY:        ANDREI PLAKHOTA              *
*******************************************************/
#include <sys/types.h> /* pid_t */
#include <unistd.h> /* getppid */
#include <stdlib.h> /* atoi */
#include <signal.h> /* sigaction */
#include <sys/sem.h> /* sempost */
#include <assert.h> /* assert */
#include <sys/wait.h> /* waitpid */

#include "scheduler.h" /* ScdRun */
#include "uid.h" /* UIDIsBad */

#define FAIL (-1)
#define N_SEMS (2)
#define WD_READY (0)
#define APP_READY (1)
#define REMOVE_TASK (-1)
#define FREQUENCY (2)
#define RESET_REVIVE_TIME (2)

static int g_semid = 0;
static pid_t g_target_pid = 0;
static char **g_wd_argv = NULL;
static volatile sig_atomic_t g_seconds_til_revive = RESET_REVIVE_TIME;
static volatile sig_atomic_t g_should_stop = 0;

static const struct sembuf g_post = {WD_READY, 2, 0};
static const struct sembuf g_wait = {APP_READY, -1, 0};

static int Init(char **argv, scd_t **scheduler);
static void PingHandler(int signal);
static void StopHandler(int signal);
static long SendPing(void *args);
static long CheckPong(void *args);
static int ReviveApp();
static scd_t *InitScd();
static int SetSigHandlers();

static void PingHandler(int signal)
{
	(void)signal; 

	g_seconds_til_revive = RESET_REVIVE_TIME; 
}

static void StopHandler(int signal)
{
	(void)signal; 

	kill(g_target_pid, SIGUSR2);
	g_should_stop = 1;
}

static long SendPing(void *args)
{
	(void)args;

	if(g_should_stop)
	{
		return REMOVE_TASK;
	}

	kill(g_target_pid, SIGUSR1);

	return 0;
}

static int ReviveApp()
{
	waitpid(g_target_pid, NULL, 0);

	g_target_pid = fork();
	if(0 == g_target_pid)
	{
		if(FAIL == execvp(g_wd_argv[2], g_wd_argv + 2))
		{
			exit(1);
		}
	}
	else if(FAIL == g_target_pid)
	{
		return FAIL;
	}

	if(FAIL == semop(g_semid, (struct sembuf *)&g_wait, 1))
	{
		return FAIL;
	}
	
	g_seconds_til_revive = RESET_REVIVE_TIME;

	return 0;
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
        if(0 != ReviveApp())
        {
            exit(1);
        }
	}

	return 0;
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

    return 0;
}

static int Init(char **argv, scd_t **scheduler)
{
	g_semid = atoi(argv[1]);
	g_wd_argv = argv;

	g_target_pid = getppid();

	if(FAIL == SetSigHandlers())
	{
		return FAIL;
	}

	if(FAIL == semop(g_semid, (struct sembuf *)&g_post, 1))
	{
		return FAIL;
	}

	*scheduler = InitScd();
    if(NULL == *scheduler)
    {
        return FAIL;
    }

	return 0;
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

static int RunWD(char **argv)
{
	scd_t *scheduler = NULL;
	int status = Init(argv, &scheduler);

	if(FAIL != status)
	{
		ScdRun(scheduler);
	}

	return status;
}

int main(int argc, char **argv)
{
	(void)argc;

	RunWD(argv);

	return 0;
}
