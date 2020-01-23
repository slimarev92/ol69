/********************************************
*                 _....._                   *
*              _.:`.--|--.`:._              *
*            .: .'\o  | o /'. '.            *
*           // '.  \ o|  /  o '.\           *
*          //'._o'. \ |o/ o_.-'o\\          *
*          || o '-.'.\|/.-' o   ||          *
*          ||--o--o-->|<o-----o-||          *
*          \\  o _.-'/|\'-._o  o//          *
*           \\.-'  o/ |o\ o '-.//           *
*            '.'.o / o|  \ o.'.'            *
*              `-:/.__|__o\:-'              *
*                 `"--=--"`                 *
*                                           *
*   Author: TEAM PIZZOLOGY                  *
*   Date: 12/08/19                          *
*   Description: File Histogram             *
*                                           *
********************************************/
#include <stdio.h>     /* printf */
#include <pthread.h>   /* pthread_create */
#include <stdlib.h>    /* malloc */
#include <assert.h>    /* assert    */
#include <sys/mman.h>  /* mmap      */
#include <sys/stat.h>  /* fstat     */
#include <fcntl.h>     /* O_RDONLY  */

#define ASCII_SIZE (128)
#define CASE_OFFSET 32

#ifndef N_THREADS
#define N_THREADS (4) /* DEFAULT */
#endif /* N_THREADS */

#define FILE_PERM O_RDONLY
#define MAP_PERM PROT_READ
#define HANDLE_ERROR(str) { perror(str); exit(EXIT_FAILURE); }

typedef struct
{
	int from;
	int to;
} count_range_t;

static char *g_mapped_file_addr;
static size_t g_file_size;
static int g_histogram[ASCII_SIZE];
static count_range_t g_ranges[N_THREADS];
static pthread_t g_threads[N_THREADS];

static void *ReaderThreadFunc(void *args);
static int Init(const char *filename);
static int Run();
static void *MapFile(const char *filename);

int main(int argc, char *argv[])
{
    int c = 0;
	char *filename = NULL;
    
	if (argc < 2)
	{
		puts("need filename as parameter");

		return -1;
	}
	else
	{
		filename = argv[1];
	}

    if (0 != Init(filename))
	{
		puts("Init failed!");

		return -1;
	}

	if (0 != Run())
	{
		puts("Run failed!");

		return -1;
	}
    
	#ifdef _PRINT_RES
	for(c = 'A'; 'Z' >= c; ++c)
	{
		printf("%c: %d\n", c, g_histogram[c]);
	}
	#endif
    
    return 0;
}

static void *ReaderThreadFunc(void *args)
{
	count_range_t *range = (count_range_t *)args;
    int histogram[ASCII_SIZE] = {0};
    int i = range->from;
	
	/* count chars */
    for(; i < range->to; ++i)
	{
		++histogram[(int)*(g_mapped_file_addr + i)];
	}
	
	/* sum upper case and lower case letters */
	for(i = 'A'; 'Z' >= i; ++i)
	{
		histogram[i] += histogram[i + CASE_OFFSET];
	}

	/* update global histogram atomically */
    for(i = 'A'; 'Z' >= i; ++i)
    {
        __sync_fetch_and_add(g_histogram + i, histogram[i]);
    }

    return NULL;
}

static int Init(const char *filename)
{
    int i = 0;
	int from = 0;
	int block_size = 0;

	g_mapped_file_addr = MapFile(filename);
	if (MAP_FAILED == g_mapped_file_addr)
	{
		puts("mapping file failed");

		return -1;
	}

	block_size = g_file_size / N_THREADS;

	for(i = 0; N_THREADS > i; ++i)
	{
		g_ranges[i].from = from;
		g_ranges[i].to = from + block_size;
		from = g_ranges[i].to;
	}
	g_ranges[N_THREADS -1].to += (g_file_size % N_THREADS);
	
    return 0;
}

static int Run()
{
	int i = 0, status = 0;

    for(i = 0; N_THREADS > i; ++i)
    {
        status = pthread_create((g_threads + i),
                                NULL,
                                ReaderThreadFunc,
                                g_ranges + i);
        if(0 != status)
        {
            puts("thread creation error");

            return -1;
        }
    }

    for(i = 0; N_THREADS > i; ++i)
    {
        status = pthread_join(g_threads[i], NULL);
        if(0 != status)
        {
            puts("thread join error");

            return -1;
        }
    }

	return 0;
}

static void *MapFile(const char *filename)
{
	int res = 0, fd = 0;
	struct stat file_stats;

	fd = open(filename, FILE_PERM);
	if (-1 == fd)
	{
		HANDLE_ERROR("open");
	}

	res = fstat(fd, &file_stats);
	if (0 != res)
	{
		HANDLE_ERROR("fstat");
	}
	g_file_size = file_stats.st_size;

	g_mapped_file_addr = mmap(NULL, g_file_size, MAP_PERM, MAP_PRIVATE, fd, 0);

	return g_mapped_file_addr;
}
