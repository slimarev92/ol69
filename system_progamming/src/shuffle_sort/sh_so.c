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
*     SYSTEM PROGRAMMING: THREADS - SHUFFLE AND SORT   *
*     CODE BY:            SASHA LIMAREV                *
*******************************************************/
#include <stdlib.h> /* rand */
#include <stdio.h> /* printf */
#include <sys/mman.h> /* mmap */
#include <sys/stat.h> /* fstat */
#include <unistd.h> /* fstat */
#include <sys/types.h> /* statbuf */
#include <fcntl.h> /* open */
#include <time.h> /* time */
#include <string.h> /* strcmp */
#include <pthread.h> /* pthread_create */

#ifndef N_THREADS   
#define N_THREADS (4)
#endif 

#define STR_SIZE (51)

typedef struct
{
    char **arr; 
    int from;
    int to;
} range_t;

static int OpenAndGetSize(const char *file_path, int *fd, size_t *file_size);
static int CopyLinesToArr(const char *file_path, char ***arr, size_t *n_lines);
static void *MapFileToMem(int fd, size_t file_size);
static size_t CountLines(const char *mapped_file, size_t file_size);
static int CreateArrays(char ***arr, size_t n_lines);
static void DestroyArrays(char **arr, size_t n_lines);
static void ShuffleStrings(char **arr, size_t n_lines);
static int CopyAndShuffle(const char *file_path, char ***arr, size_t *n_lines);
static int CompareStrings(const void *s1, const void *s2);
static void SortArray(char **arr, range_t *range);
static int MergeArrs(char **arr1, size_t len1, char **arr2, size_t len2);
static int CreateThreads(range_t **ranges, pthread_t **threads, size_t n_lines,
                                                                     char **ar);
static void FreeAll(range_t *ranges, pthread_t *threads, char **arr, size_t nl);
static int JoinThreads(pthread_t *threads);
static void SetRanges(range_t **ranges, size_t n_lines, char **arr);
static void *SortSection(void *args);
static int Run(const char *file_path);
static int MergeAll(char **arr, range_t *ranges);
static int InitThreadInfo(range_t **ranges, pthread_t **threads);

int main(int argc, char **argv)
{
    if(2 > argc)
    {
        puts("Missing filename argument");

        return 1;
    }

    printf("N_THREADS: %d\n", N_THREADS);

    Run(argv[1]);

    return 0;
}

static int Run(const char *file_path)
{
    size_t n_lines = 0, i = 0, is_fail = 0;
    pthread_t *threads = NULL;
    char **arr = NULL;
    range_t *ranges = NULL;

    if(0 != CopyAndShuffle(file_path, &arr, &n_lines))
    {
       is_fail = 1;
    }

    if(!is_fail && (0 != CreateThreads(&ranges, &threads, n_lines, arr)))
    {
        is_fail = 1;
    }

    if(!is_fail && 0 !=  JoinThreads(threads))
    {
        is_fail = 1;
    }

    if(!is_fail && 0 != MergeAll(arr, ranges))
    {
        is_fail = 1;
    }
    
    for(i = 0; !is_fail && n_lines > i; ++i)
    {
        printf("%s", arr[i]);
    }

    FreeAll(ranges, threads, arr, n_lines);

    return is_fail;
}

static int CopyAndShuffle(const char *file_path, char ***arr, size_t *n_lines)
{
    if(0 != CopyLinesToArr(file_path, arr, n_lines))
    {
        return 1;
    }

    ShuffleStrings(*arr, *n_lines);

    return 0;
}

static int CreateThreads(range_t **ranges, pthread_t **threads, size_t n_lines,
                                                                     char **arr)
{
    size_t i = 0;
    int status = 0;

    if(0 != InitThreadInfo(ranges, threads))
    {
        return 1;
    }

    SetRanges(ranges, n_lines, arr);

    for(i = 0; N_THREADS > i; ++i)
    {
        status = pthread_create((*threads + i),
                                NULL,
                                SortSection,
                                (*ranges) + i);
        if(0 != status)
        {
            perror("thread creation error");

            return -1;
        }
    }

    return 0;
}

static int JoinThreads(pthread_t *threads)
{
    size_t i = 0;
    size_t is_fail = 0;

    for(i = 0; N_THREADS > i && !is_fail; ++i)
    {
        is_fail = pthread_join(threads[i], NULL);
        if(0 != is_fail)
        {
            perror("thread join error");

            is_fail = 1;
        }
    }

    return is_fail;
}

static int MergeAll(char **arr, range_t *ranges)
{
    char **arr1 = NULL;
    char **arr2 = NULL;
    size_t len1 = 0;
    size_t len2 = 0;    
    size_t i = 0;

    for(i = 1; N_THREADS > i; ++i) 
    {
        len1 = ranges[0].to - ranges[0].from;
        len2 = ranges[i].to - ranges[i].from;
        arr1 = arr + ranges[0].from;
        arr2 = arr + ranges[i].from; 

        if(0 != MergeArrs(arr1, len1, arr2, len2))
        {
            return 1;
        }
        
        ranges[0].to = ranges[i].to;
    }

    return 0;
}

static int OpenAndGetSize(const char *file_path, int *fd, size_t *file_size)
{
    struct stat file_info;
    int curr_fd = open(file_path, O_RDONLY);

    if(-1 == curr_fd)
    {
        perror("Error while opening a file");

        return 1;
    }

    *fd = curr_fd;

    if(0 != fstat(curr_fd, &file_info))
    {
        perror("Error while getting file info");

        return 1;
    }

    *file_size = (size_t)file_info.st_size;
    
    return 0;
}

static void *MapFileToMem(int fd, size_t file_size)
{
    void *mapped_file = mmap(NULL, file_size + 1, PROT_READ | PROT_WRITE,
                                                            MAP_PRIVATE, fd ,0);

    if(MAP_FAILED == mapped_file)
    {
        perror("Error while mapping file to virtual memory");

        return NULL;
    }

    *((char *)mapped_file + file_size) = '\n';

    return mapped_file;
}

static size_t CountLines(const char *mapped_file, size_t file_size)
{
    size_t i = 0;
    size_t counter = 0;

    for(; file_size >= i; ++i)
    {
        if('\n' == mapped_file[i])
        {
            ++counter;
        }
    }

    return counter;
}

static int CreateArrays(char ***arr, size_t n_lines)
{
    size_t i = 0;
    char *strs = NULL;
    char **curr_arr = NULL;

    curr_arr = (char **)malloc((sizeof(char **) * n_lines) + 1);
    if(NULL == curr_arr)
    {
        perror("Error while allocatin arr");

        return 1;
    }

    strs = (char *)calloc(n_lines * STR_SIZE, sizeof(char));
    if(NULL == strs)
    {
        free(*arr);
        perror("Error while allocating strings");

        return 1;
    }

    curr_arr[n_lines] = strs;

    for(; n_lines > i; ++i)
    {
        curr_arr[i] = strs + (i * STR_SIZE);
    }

    *arr = curr_arr;

    return 0;
}

static int CopyLinesToArr(const char *file_path, char ***arr, size_t *n_lines)
{
    int fd = 0;
    size_t i = 0;
    size_t curr_char = 0;
    size_t file_size = 0;
    char *mapped_file = NULL;
    char *mapped_cpy = NULL;

    if(0 != OpenAndGetSize(file_path, &fd, &file_size))
    {
        return 1;
    }

    mapped_file = (char *)MapFileToMem(fd, file_size);

    if(NULL == mapped_file)
    {
        close(fd);

        return 1;
    }

    mapped_cpy = mapped_file;
    *n_lines = CountLines(mapped_file, file_size);

    if(0 != CreateArrays(arr, *n_lines))
    {
        close(fd);
        
        return 1;
    }

    for(i = 0; *n_lines > i; ++i, ++mapped_cpy)
    {
        curr_char = 0;

        while('\n' != *mapped_cpy)
        {
            (*arr)[i][curr_char] = *mapped_cpy;
            ++mapped_cpy;
            ++curr_char;
        } 

        (*arr)[i][curr_char] = '\n';
    }

    close(fd);
    return 0;
}

static int InitThreadInfo(range_t **ranges, pthread_t **threads)
{
    *ranges = (range_t *)malloc(sizeof(range_t) * N_THREADS);
    if(NULL == ranges)
    {
        perror("Error while allocating ranges array");
        
        return 1;
    }

    *threads = (pthread_t *)malloc(sizeof(pthread_t) * N_THREADS);
    if(NULL == ranges)
    {
        free(ranges);
        perror("Error while alloting thread_t array");

        return 1;
    }

    return 0;
}

static void SetRanges(range_t **ranges, size_t n_lines, char **arr)
{
    size_t i = 0;
    size_t from = 0;
    size_t block_size = 0;

    block_size = n_lines / N_THREADS;

    for(i = 0; N_THREADS > i; ++i)
    {
        (*ranges)[i].arr = arr;
        (*ranges)[i].from = from;
        (*ranges)[i].to = from + block_size;
        from = (*ranges)[i].to;
    }
    (*ranges)[N_THREADS -1].to += (n_lines % N_THREADS);
}

static void ShuffleStrings(char **arr, size_t n_lines)
{
    int curr = 0;
    int last = n_lines - 1;
    char *temp = NULL;

    srand(time(NULL));

    for(; 0 < last; --last)
    {
        curr = rand() % (last + 1);

        temp = arr[last];

        arr[last] = arr[curr];
        arr[curr] = temp;
    }    
}

static void *SortSection(void *args)
{
    range_t *range = (range_t *)args;

    SortArray(range->arr, range);

    return NULL;
}

static void SortArray(char **arr, range_t *range)
{
    size_t n_lines = range->to - range->from;

    qsort(arr + range->from, n_lines, sizeof(char *), CompareStrings);
}

static int CompareStrings(const void *s1, const void *s2)
{
    return strcmp(*(char **)s1, *(char **)s2);
}

static int MergeArrs(char **arr1, size_t len1, char **arr2, size_t len2)
{
    size_t i = 0;
    char *curr_str = NULL;
    size_t iterations = len1 + len2;
    char **runner1 = arr1;
    char **runner2 = arr2;

    char **arr_temp = (char **)malloc(sizeof(char *) * (len1 + len2));
    if(NULL == arr_temp)
    {
        perror("Error while allocating temp array");

        return 1;
    }

    for(; i < iterations; ++i)
    {
        if(0 == len1)
        {
            curr_str = *runner2;
            ++runner2;
            --len2;
        }
        else if(0 == len2)
        {
            curr_str = *runner1;
            ++runner1;
            --len1;
        }
        else if(0 > CompareStrings(runner1, runner2))
        {
            curr_str = *runner1;
            ++runner1;
            --len1;
        }
        else
        {
            curr_str = *runner2;
            ++runner2;
            --len2;
        }

        *(arr_temp + i) = curr_str;
    }

    memcpy(arr1, arr_temp, sizeof(char *) * iterations);
    free(arr_temp);

    return 0;
}

static void FreeAll(range_t *ranges, pthread_t *threads, char **arr, size_t n_l)
{
    free(ranges);
    free(threads);
    DestroyArrays(arr, n_l);
}

static void DestroyArrays(char **arr, size_t n_lines)
{
    free(arr[n_lines]);
    free(arr);
}