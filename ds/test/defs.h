/*******************************************************************************
* Programmer: Eldad Carin
* Reviewer: 
* File: DEFS.H
*******************************************************************************/
/*Print Colors*****************************************************************/
#define BLUE      "\x1b[34m"
#define RED       "\x1b[31m"
#define REDL      "\x1b[91m"
#define YELLOW    "\x1b[1;33m" 
#define YELLOW2   "\x1b[33m"   
#define CYANDARK  "\x1b[36m"
#define CYAN      "\x1b[36m\x1b[1m"
#define GREEN     "\x1B[32m"
#define GREENL    "\x1b[92m"
#define MAGENTA   "\x1B[35m"
#define ORANGE    "\x1b[33m"
#define BLINKON   "\x1b[5m"
#define BLINKOFF  "\x1b[25m"
#define BOLDON    "\x1b[1m"
#define BOLDOFF   "\x1b[21m"
#define UNDERLON  "\x1b[4m"
#define UNDERLOFF "\x1b[24m"
#define WHITE     "\x1B[37m"
#define RESET     "\x1b[0m"

/*RUN TEST MACRO***************************************************************/
#define RUN_TEST(test_name) \
{ \
if (0 == test_name()) \
{ puts(GREEN"SUCCESS"RESET); } \
else \
{ puts(RED"FAILURE"RESET); } \
}

/* DEFINES ********************************************************************/
#define WORD_SIZE_DEF (sizeof(size_t)) /* size of WORD */
#define ARR_SIZE_DEF sizeof(X) / sizeof(X[0])) /* size of array */
#define UNUSED(X)  (void)(X)  /* unused parameters macro */

