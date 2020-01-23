/*******************************
* DS (-ISH):   KNIGHT'S TOUR   *
* CODE BY:     SASHA LIMAREV   *   
* REVIEWED BY: DANIEL MAIZEL   *                          
*******************************/
#include <assert.h> /* assert */
#include <sys/types.h> /* ssize_t */
#include <stdlib.h> /* malloc */
#include <time.h> /* time */

#include "barr.h" 
#include "stack.h" 
#include "kt.h"

#define BOARD_WIDTH (8)
#define RUN_TIME (10000)
#define BOARD_SIZE (64)
#define FULL_BOARD ((size_t)0 - 1)

static const signed char move_table[BOARD_SIZE][BOARD_WIDTH] = 
{
    {10, 17, -10, -10, -10, -10, -10, -10},  
    {11, 18, 16, -10, -10, -10, -10, -10},  
    {12, 19, 17, 8, -10, -10, -10, -10},  
    {13, 20, 18, 9, -10, -10, -10, -10},  
    {14, 21, 19, 10, -10, -10, -10, -10},  
    {15, 22, 20, 11, -10, -10, -10, -10},  
    {23, 21, 12, -10, -10, -10, -10, -10},  
    {22, 13, -10, -10, -10, -10, -10, -10},  
    {2, 18, 25, -10, -10, -10, -10, -10},  
    {3, 19, 26, 24, -10, -10, -10, -10},  
    {4, 20, 27, 25, 16, 0, -10, -10},  
    {5, 21, 28, 26, 17, 1, -10, -10},  
    {6, 22, 29, 27, 18, 2, -10, -10},  
    {7, 23, 30, 28, 19, 3, -10, -10},  
    {31, 29, 20, 4, -10, -10, -10, -10},  
    {30, 21, 5, -10, -10, -10, -10, -10},  
    {1, 10, 26, 33, -10, -10, -10, -10},  
    {2, 11, 27, 34, 32, 0, -10, -10},  
    {3, 12, 28, 35, 33, 24, 8, 1},  
    {4, 13, 29, 36, 34, 25, 9, 2},  
    {5, 14, 30, 37, 35, 26, 10, 3},  
    {6, 15, 31, 38, 36, 27, 11, 4},  
    {7, 39, 37, 28, 12, 5, -10, -10},  
    {38, 29, 13, 6, -10, -10, -10, -10},  
    {9, 18, 34, 41, -10, -10, -10, -10},  
    {10, 19, 35, 42, 40, 8, -10, -10},  
    {11, 20, 36, 43, 41, 32, 16, 9},  
    {12, 21, 37, 44, 42, 33, 17, 10},  
    {13, 22, 38, 45, 43, 34, 18, 11},  
    {14, 23, 39, 46, 44, 35, 19, 12},  
    {15, 47, 45, 36, 20, 13, -10, -10},  
    {46, 37, 21, 14, -10, -10, -10, -10},  
    {17, 26, 42, 49, -10, -10, -10, -10},  
    {18, 27, 43, 50, 48, 16, -10, -10},  
    {19, 28, 44, 51, 49, 40, 24, 17},  
    {20, 29, 45, 52, 50, 41, 25, 18},  
    {21, 30, 46, 53, 51, 42, 26, 19},  
    {22, 31, 47, 54, 52, 43, 27, 20},  
    {23, 55, 53, 44, 28, 21, -10, -10},  
    {54, 45, 29, 22, -10, -10, -10, -10},  
    {25, 34, 50, 57, -10, -10, -10, -10},  
    {26, 35, 51, 58, 56, 24, -10, -10},  
    {27, 36, 52, 59, 57, 48, 32, 25},  
    {28, 37, 53, 60, 58, 49, 33, 26},  
    {29, 38, 54, 61, 59, 50, 34, 27},  
    {30, 39, 55, 62, 60, 51, 35, 28},  
    {31, 63, 61, 52, 36, 29, -10, -10},  
    {62, 53, 37, 30, -10, -10, -10, -10},  
    {33, 42, 58, -10, -10, -10, -10, -10},  
    {34, 43, 59, 32, -10, -10, -10, -10},  
    {35, 44, 60, 56, 40, 33, -10, -10},  
    {36, 45, 61, 57, 41, 34, -10, -10},  
    {37, 46, 62, 58, 42, 35, -10, -10},  
    {38, 47, 63, 59, 43, 36, -10, -10},  
    {39, 60, 44, 37, -10, -10, -10, -10},  
    {61, 45, 38, -10, -10, -10, -10, -10},  
    {41, 50, -10, -10, -10, -10, -10, -10},  
    {42, 51, 40, -10, -10, -10, -10, -10},  
    {43, 52, 48, 41, -10, -10, -10, -10},  
    {44, 53, 49, 42, -10, -10, -10, -10},  
    {45, 54, 50, 43, -10, -10, -10, -10},  
    {46, 55, 51, 44, -10, -10, -10, -10},  
    {47, 52, 45, -10, -10, -10, -10, -10},  
    {53, 46, -10, -10, -10, -10, -10, -10}
};

/* static functions here TODO REMOVE */

static void PushToPath(stack_t *path, unsigned char cell);
static enum status KTRec(stack_t *path, size_t board, time_t end_time);
static void PopFromPath(stack_t *path);
enum status KT(int *path, int first);
static int IsBoardFull(size_t board);
static unsigned char GetCurrLocation(stack_t *path);
static int IsMoveAvailable(size_t board, unsigned char move);
static int IsRealMove(signed char move);
static size_t SetCellVisited(size_t board, unsigned char cell);

/***************************************/

static enum status KTRec(stack_t *path, size_t board, time_t end_time)
{
    size_t i = 0;
    unsigned char location = GetCurrLocation(path);
    const signed char *moves = move_table[location];

    if(time(NULL) >= end_time)
    {
        return T_FAIL;
    }
    else if(IsBoardFull(board))
    {
        return OK;
    }

    for(; i < BOARD_WIDTH; ++i)
    {   
        if(IsRealMove(moves[i]) && IsMoveAvailable(board, moves[i]))
        {
            PushToPath(path, moves[i]);

            if(OK == KTRec(path, SetCellVisited(board, moves[i]), end_time))
            {
                return OK;
            }

            PopFromPath(path);
        }
    }
   
    return T_FAIL;
}

enum status KT(int *path, int first)
{
    stack_t *path_stack = NULL;
    int i = BOARD_SIZE - 1;
    enum status status = OK;

    assert(path);
    assert(BOARD_SIZE > first);

    path_stack = STACKCreate(BOARD_SIZE, sizeof(unsigned char));
    if(NULL == path_stack)
    {
        return M_FAIL;
    }
    
    PushToPath(path_stack, first);
    status = KTRec(path_stack, SetCellVisited(0, first), time(NULL) + RUN_TIME);

    /* if path was found, copy it to the array */
    if(OK == status)
    {
        for(; 0 <= i; --i)
        {
            path[i] = *(unsigned char *)STACKPeek(path_stack);
            STACKPop(path_stack);
        }
    }

    STACKDestroy(path_stack);
    return status;
}

static int IsBoardFull(size_t board)
{
    return FULL_BOARD == board;
}

static void PushToPath(stack_t *path, unsigned char cell)
{
    STACKPush(path, &cell);
}

static void PopFromPath(stack_t *path)
{
    STACKPop(path);
}

static unsigned char GetCurrLocation(stack_t *path)
{
    return *(unsigned char *)STACKPeek(path);
}

static int IsRealMove(signed char move)
{
    return 0 <= move;
}

static int IsMoveAvailable(size_t board, unsigned char move)
{  
    return BARRIsOff(board, move);
}

static size_t SetCellVisited(size_t board, unsigned char cell)
{    
    return BARRSetOn(board, (size_t)cell); 
}