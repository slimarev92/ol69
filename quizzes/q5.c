#include <sys/types.h> /* ssize_t */
#include <stdio.h> /* printf */

#include "barr.h" /* bit array */


typedef struct delta
{
    ssize_t row;
    ssize_t col;
} delta_t;

static size_t g_board[64];
static const delta_t g_deltas[8] =
{
    {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}
};

static void InitBoard()
{
    size_t i = 0;


    for(; 64 > i; ++i)
    {
       g_board[i] = (size_t)0x1;
    }
}

static void DestroyIslandRec(ssize_t row, ssize_t col)
{
    ssize_t curr_row = 0;
    ssize_t curr_col = 0;
    size_t i = 0;

    for(; 8 > i; ++i)
    {
        curr_row = row + g_deltas[i].row;
        curr_col = col + g_deltas[i].col;

        if(0 <= curr_row && 63 >= curr_row && 0 <= curr_col && 63 >= curr_col)
        {
            if(BARRIsOn(g_board[curr_row], curr_col))
            {
                BARRSetOff(g_board[curr_row], curr_col);
                DestroyIslandRec(curr_row, curr_col);
            }
        }
    }
}

size_t CountIslands()
{
    size_t row = 0, col = 0;
    size_t res = 0;

    for(; 64 > row; ++row)
    {
        for(; 64 > col; ++col)
        {
            if(BARRIsOn(g_board[row], col))
            {
                ++res;
                DestroyIslandRec(row, col);
            }
        }
    }
    
    return res;
}

int main()
{
    InitBoard();

    printf("%lu\n", CountIslands());

    return 0;
}
