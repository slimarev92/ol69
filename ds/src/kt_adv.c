/*******************************
* DS (-ISH):   KNIGHT'S TOUR   *
* CODE BY:     SASHA LIMAREV   *   
* REVIEWED BY: ALEX Z          *                          
*******************************/
#include <assert.h> /* assert */
#include <sys/types.h> /* ssize_t */
#include <stdlib.h> /* malloc */
#include <time.h> /* time */

#include "barr.h" 
#include "stack.h" 
#include "kt.h"

#define N_MOVES_COL (8)
#define BOARD_WIDTH (8)
#define RUN_TIME (160)
#define BOARD_SIZE (64)
#define FULL_BOARD ((size_t)0 - 1)

static const signed char move_table[BOARD_SIZE][BOARD_WIDTH + 1] = 
{
    { 10,  17, -10, -10, -10, -10, -10, -10,   2},
    { 16,  11,  18, -10, -10, -10, -10, -10,   3},
    {  8,  17,  12,  19, -10, -10, -10, -10,   4},
    {  9,  13,  18,  20, -10, -10, -10, -10,   4},
    { 14,  10,  19,  21, -10, -10, -10, -10,   4},
    { 15,  22,  11,  20, -10, -10, -10, -10,   4},
    { 23,  12,  21, -10, -10, -10, -10, -10,   3},
    { 22,  13, -10, -10, -10, -10, -10, -10,   2},
    {  2,  25,  18, -10, -10, -10, -10, -10,   3},
    {  3,  24,  26,  19, -10, -10, -10, -10,   4},
    {  0,  16,   4,  25,  20,  27, -10, -10,   6},
    {  1,   5,  17,  26,  28,  21, -10, -10,   6},
    {  6,   2,  22,  27,  18,  29, -10, -10,   6},
    {  7,  23,   3,  30,  19,  28, -10, -10,   6},
    { 31,   4,  20,  29, -10, -10, -10, -10,   4},
    {  5,  30,  21, -10, -10, -10, -10, -10,   3},
    {  1,  10,  33,  26, -10, -10, -10, -10,   4},
    {  0,  32,   2,  11,  27,  34, -10, -10,   6},
    {  8,   1,  24,   3,  33,  12,  35,  28,   8},
    {  4,   9,   2,  25,  13,  36,  34,  29,   8},
    {  5,  14,   3,  10,  30,  26,  37,  35,   8},
    {  6,  15,  31,   4,  11,  38,  36,  27,   8},
    {  7,  39,   5,  12,  28,  37, -10, -10,   6},
    {  6,  13,  38,  29, -10, -10, -10, -10,   4},
    {  9,  41,  34,  18, -10, -10, -10, -10,   4},
    {  8,  40,  10,  42,  19,  35, -10, -10,   6},
    { 32,  16,   9,  41,  11,  43,  20,  36,   8},
    { 17,  10,  33,  12,  42,  37,  44,  21,   8},
    { 13,  22,  38,  11,  43,  34,  18,  45,   8},
    { 14,  23,  39,  46,  12,  35,  19,  44,   8},
    { 15,  47,  13,  36,  20,  45, -10, -10,   6},
    { 14,  46,  21,  37, -10, -10, -10, -10,   4},
    { 49,  17,  42,  26, -10, -10, -10, -10,   4},
    { 48,  16,  50,  43,  18,  27, -10, -10,   6},
    { 49,  40,  24,  17,  51,  28,  44,  19,   8},
    { 52,  50,  41,  25,  29,  45,  20,  18,   8},
    { 30,  46,  53,  51,  21,  42,  26,  19,   8},
    { 31,  47,  54,  22,  52,  43,  27,  20,   8},
    { 55,  23,  53,  44,  28,  21, -10, -10,   6},
    { 54,  22,  29,  45, -10, -10, -10, -10,   4},
    { 57,  50,  25,  34, -10, -10, -10, -10,   4},
    { 56,  58,  24,  51,  26,  35, -10, -10,   6},
    { 57,  48,  59,  32,  52,  25,  27,  36,   8},
    { 60,  58,  49,  53,  33,  28,  37,  26,   8},
    { 54,  61,  59,  38,  50,  29,  34,  27,   8},
    { 55,  62,  39,  60,  30,  51,  35,  28,   8},
    { 63,  31,  61,  52,  36,  29, -10, -10,   6},
    { 62,  53,  30,  37, -10, -10, -10, -10,   4},
    { 58,  33,  42, -10, -10, -10, -10, -10,   3},
    { 59,  32,  34,  43, -10, -10, -10, -10,   4},
    { 56,  60,  40,  33,  44,  35, -10, -10,   6},
    { 57,  61,  41,  36,  45,  34, -10, -10,   6},
    { 62,  58,  46,  37,  42,  35, -10, -10,   6},
    { 63,  47,  59,  38,  43,  36, -10, -10,   6},
    { 39,  60,  44,  37, -10, -10, -10, -10,   4},
    { 61,  38,  45, -10, -10, -10, -10, -10,   3},
    { 41,  50, -10, -10, -10, -10, -10, -10,   2},
    { 40,  51,  42, -10, -10, -10, -10, -10,   3},
    { 48,  52,  41,  43, -10, -10, -10, -10,   4},
    { 49,  53,  44,  42, -10, -10, -10, -10,   4},
    { 54,  50,  45,  43, -10, -10, -10, -10,   4},
    { 55,  46,  51,  44, -10, -10, -10, -10,   4},
    { 47,  52,  45, -10, -10, -10, -10, -10,   3},
    { 53,  46, -10, -10, -10, -10, -10, -10,   2}
};

static void PushToPath(stack_t *path, unsigned char cell);
static enum status KTRec(stack_t *path, size_t board, time_t end_time);
static void PopFromPath(stack_t *path);
enum status KT(int *path, int first);
static int IsBoardFull(size_t board);
static unsigned char GetCurrLocation(stack_t *path);
static int IsMoveAvailable(size_t board, unsigned char move);
static int IsRealMove(signed char move);
static size_t SetCellVisited(size_t board, unsigned char cell);

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

enum status KTAdvanced(int *path, int first)
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