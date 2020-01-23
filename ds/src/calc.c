/***************************************
* DATA STRUCTURES: CALCULATOR          *
* CODE BY:         SASHA LIMAREV       *   
* REVIEWED BY:     SANDRA HADAD        *                          
****************************************/ 
#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */
#include <string.h> /* strlen */
#include <math.h> /* pow */

#include "../include/calculator.h" /* calculator header file */
#include "../include/stack.h" /* stack header file */

enum states {WAIT_OP, WAIT_NUM, STOPPED};
enum associativity {LTR, RTL};

typedef struct operator_row operator_row_t;
typedef struct state_trans state_trans_t;
typedef struct calc calc_t;
typedef enum status (*state_handler)(char **str, calc_t *calc);
typedef enum status (*operator_handler)(double, double, double * temp);

struct calc
{
    stack_t *num_stack;
    stack_t *op_stack;
};

struct state_trans 
{
    enum states state_to;
    state_handler handler;
};

struct operator_row
{
    enum associativity asoc;
    size_t prec;
    operator_handler handler;
};

static enum status CalcPlusHadler(double first, double second, double *temp);
static enum status CalcMinusHandler(double first, double second, double *temp);
static enum status CalcMultiplicationHandler(double first, double, double *);
static enum status CalcDivisionHandler(double first, double second, double *);
static enum status CalcPowerHandler(double first, double second, double *temp);
static enum status CalcDummyHandler(double first, double second, double *temp);
static calc_t *CalcCreate(size_t len);
static void CalcDestroy(calc_t *calc);
static enum status DefaultHandler(char **str, calc_t *calculator);
static enum status SynErrorHandler(char **str, calc_t *calculator);
static enum status MathErrorHandler(char **str, calc_t *calculator);
static enum status NumHandler(char **str, calc_t *calc);
static enum status OpHandler(char **str, calc_t *calc);
static enum status EOSHandler(char **str, calc_t *calc);
static enum status ClosingBracketHandler(char **str, calc_t *calculator);
static void InitOperatorTable();
static void InitTransitionTable();
static int InitMachine(calc_t **calc, const char *str, enum states *state);
int Calculate(const char *str, double *result);
static int CalcIsNewRTL(char operator);
static int CalcIsNewPrecHigher(calc_t *calc, char operator);
static int CalcIsTopOpStackLBracket(calc_t *calc);
static int CalcIsOpStackEmpty(calc_t *calc);
static int CalcComparePrec(char first, calc_t *calc);
static enum status CalcFoldOnce(calc_t *calc);

static state_trans_t g_trans_table[256][3];
static operator_row_t g_op_table[256];
/* used to inform the parser of a math error */
static char *g_math_error = "#"; 
/* used to inform the parser of a syntax error during calc - brackets issue */
static char *g_syn_error = "$";
/* used to inform the parser of a successful calculation */
static char *g_done = "@";

static calc_t *CalcCreate(size_t len)
{
    calc_t *new_calc = (calc_t *)malloc(sizeof(calc_t));
    if(NULL == new_calc)
    {
        return NULL;
    }

    new_calc->op_stack = STACKCreate(len, sizeof(char));
    new_calc->num_stack = STACKCreate(len, sizeof(double));
    if(NULL == new_calc->op_stack || NULL == new_calc->num_stack)
    {
        free(new_calc->op_stack);
        free(new_calc->num_stack);
        free(new_calc);

        return NULL;
    }

    return new_calc;
}

static void CalcDestroy(calc_t *calc)
{
    STACKDestroy(calc->num_stack);
    STACKDestroy(calc->op_stack);
    free(calc);
}

static int CalcIsNewRTL(char operator)
{
    return RTL == g_op_table[(size_t)operator].asoc;
}

static int CalcIsNewPrecHigher(calc_t *calc, char operator)
{
    return 0 < CalcComparePrec(operator, calc);
}

static int CalcIsTopOpStackLBracket(calc_t *calc)
{
    return '(' ==   *(char *)STACKPeek(calc->op_stack);
}

static int CalcIsOpStackEmpty(calc_t *calc)
{
    return 0 == STACKSize(calc->op_stack);
}

/* returns a number larger, smaller or equal to zero */
static int CalcComparePrec(char first, calc_t *calc)
{
    char second = 0;

    if(!CalcIsOpStackEmpty(calc))
    {
        second = *(char *)STACKPeek(calc->op_stack);
    }

    return g_op_table[(int)first].prec - g_op_table[(int)second].prec;
}

static enum status CalcFoldOnce(calc_t *calc)
{
    double first = 0.0;
    double second = 0.0;
    double res = 0.0;
    enum status status = OK;
    char operator = ' ';
    operator_handler hnd = NULL;

    operator = *(char *)STACKPeek(calc->op_stack);
    STACKPop(calc->op_stack);    
    first = *(double *)STACKPeek(calc->num_stack);
    STACKPop(calc->num_stack);
    second = *(double *)STACKPeek(calc->num_stack);
    STACKPop(calc->num_stack);
    hnd = g_op_table[(size_t)operator].handler;
    
    status = hnd(first, second, &res); 

    STACKPush(calc->num_stack, (void *)(size_t *)&res);

    return status;
}

static enum status CalcPlusHadler(double first, double second, double *temp)
{
    *temp = first + second;

    return OK;
}

static enum status CalcMinusHandler(double first, double second, double *temp)
{
    *temp = second - first;

    return OK;
}

static enum status CalcMultiplicationHandler(double first, double second,
                                                                   double *temp)
{
    *temp = first * second;

    return OK;
}

static enum status CalcDivisionHandler(double first, double second, double *tmp)
{
    if(0 == first)
    {
        return E_MATH;
    }
    else
    {
        *tmp = second / first;

        return OK;
    }   
}

static enum status CalcPowerHandler(double first, double second, double *temp)
{
    *temp = pow(second, first);
                                                           
    return OK; 
}

static enum status CalcDummyHandler(double first, double second, double *temp)                                                               
{
    (void)first;
    (void)second;
    (void)temp;
    
    return OK;
}

static void InitOperatorTable()
{  
    size_t i = 0;

    for(; i < 256; ++i)
    {
        g_op_table[0].asoc = LTR;
        g_op_table[0].prec = 0;
        g_op_table[0].handler = CalcDummyHandler;
    }

    g_op_table[(size_t)'+'].asoc = LTR;
    g_op_table[(size_t)'+'].prec = 0;
    g_op_table[(size_t)'+'].handler = CalcPlusHadler;

    g_op_table[(size_t)'-'].asoc = LTR;
    g_op_table[(size_t)'-'].prec = 0;
    g_op_table[(size_t)'-'].handler = CalcMinusHandler;
    
    g_op_table[(size_t)'*'].asoc = LTR;
    g_op_table[(size_t)'*'].prec = 1;
    g_op_table[(size_t)'*'].handler = CalcMultiplicationHandler;

    g_op_table[(size_t)'/'].asoc = LTR;
    g_op_table[(size_t)'/'].prec = 1;
    g_op_table[(size_t)'/'].handler = CalcDivisionHandler;

    g_op_table[(size_t)'^'].asoc = RTL;
    g_op_table[(size_t)'^'].prec = 2;
    g_op_table[(size_t)'^'].handler = CalcPowerHandler;

    g_op_table[(size_t)'('].asoc = RTL;
    g_op_table[(size_t)'('].prec = -1;
    g_op_table[(size_t)'('].handler = CalcDummyHandler;
}

static void InitTransitionTable()
{
    size_t i = 0;
    state_trans_t syn_error_trans = {STOPPED, SynErrorHandler};
    state_trans_t stopped_trans = {STOPPED, DefaultHandler};
    state_trans_t curr_trans = syn_error_trans;
    const char operators[] = {'+', '-', '*', '/', '^', '(', ')'};
 
    /* default */
    for(; i <= 256; ++i)
    {
        g_trans_table[i][WAIT_OP] = syn_error_trans;
        g_trans_table[i][WAIT_NUM] = syn_error_trans;
        g_trans_table[i][STOPPED] = syn_error_trans;       
    }

    /* digits */
    for(i = '0'; i <= '9'; ++i)
    {
        curr_trans.state_to = WAIT_OP;
        curr_trans.handler = NumHandler;

        g_trans_table[i][WAIT_OP] = syn_error_trans;
        g_trans_table[i][WAIT_NUM] = curr_trans;
        g_trans_table[i][STOPPED] = stopped_trans;
    }

    /* operators */
    for(i = 0; i < 6; ++i)
    {
        curr_trans.state_to = WAIT_NUM;
        curr_trans.handler = OpHandler;

        g_trans_table[(size_t)operators[i]][WAIT_OP] = curr_trans;
        g_trans_table[(size_t)operators[i]][WAIT_NUM] = syn_error_trans;
        g_trans_table[(size_t)operators[i]][STOPPED] = stopped_trans;       
    }

    /* opening brackets */
    curr_trans.state_to = WAIT_NUM;
    curr_trans.handler = OpHandler;

    g_trans_table['('][WAIT_OP] = syn_error_trans;
    g_trans_table['('][WAIT_NUM] = curr_trans;
    g_trans_table['('][STOPPED] = stopped_trans;

    /* closing brackets */
    curr_trans.state_to = WAIT_OP;
    curr_trans.handler = ClosingBracketHandler;

    g_trans_table[')'][WAIT_OP] = curr_trans;
    g_trans_table[')'][WAIT_NUM] = syn_error_trans;
    g_trans_table[')'][STOPPED] = stopped_trans;
    
    /* spaces */
    curr_trans.handler = DefaultHandler;
    
    curr_trans.state_to = WAIT_OP;
    g_trans_table[' '][WAIT_OP] = curr_trans;

    curr_trans.state_to = WAIT_NUM;
    g_trans_table[' '][WAIT_NUM] = curr_trans;

    curr_trans.state_to = STOPPED;
    g_trans_table[' '][STOPPED] = curr_trans;

    /* end of string */
    curr_trans.state_to = WAIT_OP;
    curr_trans.handler = EOSHandler;

    g_trans_table['\0'][WAIT_OP] = curr_trans;
    g_trans_table['\0'][WAIT_NUM] = syn_error_trans;
    g_trans_table['\0'][STOPPED] = curr_trans;

    /* math error */
    curr_trans.state_to = STOPPED;
    curr_trans.handler = MathErrorHandler;

    g_trans_table['#'][WAIT_OP] = curr_trans;
    g_trans_table['#'][WAIT_NUM] = curr_trans;
    g_trans_table['#'][STOPPED] = curr_trans;

    /* syntax error - bad closing brackets */
    curr_trans.state_to = STOPPED;
    curr_trans.handler = SynErrorHandler;

    g_trans_table['$'][WAIT_OP] = curr_trans;
    g_trans_table['$'][WAIT_NUM] = curr_trans;
    g_trans_table['$'][STOPPED] = curr_trans;

    /* successful calculation */
    curr_trans.state_to = STOPPED;
    curr_trans.handler = DefaultHandler;

    g_trans_table['@'][WAIT_OP] = curr_trans;
    g_trans_table['@'][WAIT_NUM] = curr_trans;
    g_trans_table['@'][STOPPED] = curr_trans;
}

int Calculate(const char *str, double *result)
{
    enum states curr_state = WAIT_NUM;
    char *curr_char = (char *)str;
    calc_t *calc = NULL;
    state_trans_t trans = g_trans_table[' '][WAIT_NUM];
    enum status curr_status = OK;

    assert(NULL != str && NULL != result);

    curr_char = (char *)str;
    curr_status = InitMachine(&calc, str, &curr_state);

    while(STOPPED != curr_state)
    {
        trans = g_trans_table[(size_t)*curr_char][curr_state];
        curr_state = trans.state_to;
        curr_status = trans.handler(&curr_char, calc);
    }

    *result = *(double *)STACKPeek(calc->num_stack);
  
    CalcDestroy(calc);
    return curr_status;
}

static int InitMachine(calc_t **calc, const char *str, enum states *state)
{
    static int was_init = 0;
    static double zero = 0;
    static char plus = '+';

    *calc = CalcCreate(strlen(str));
    if(NULL == calc)
    {
        *state = STOPPED;  
        return E_MALLOC;
    }

    STACKPush((*calc)->op_stack, (void *)&plus);
    STACKPush((*calc)->num_stack, (void *)&zero);

    if(!was_init)
    {
        was_init = 1;
        InitTransitionTable();
        InitOperatorTable();
    }

    return OK;
}

static enum status DefaultHandler(char **str, calc_t *calculator)
{
    (void)calculator;

    ++(*str);

    return OK;
}

static enum status SynErrorHandler(char **str, calc_t *calc)
{
    (void)str;
    (void)calc;
   
    return E_SYNTAX;
}

static enum status MathErrorHandler(char **str, calc_t *calc)
{
    (void)str;
    (void)calc;

    return E_MATH;
}

static enum status NumHandler(char **str, calc_t *calc)
{
    double num = strtod(*str, str);
    
    STACKPush(calc->num_stack, (void *)(size_t *)&num);

    return OK;
}

static enum status OpHandler(char **str, calc_t *calc)
{
    enum status status = OK;    

    /* keep folding while new operator can't be pushed */
    while(OK == status)
    {
        if(CalcIsTopOpStackLBracket(calc)    ||
           CalcIsOpStackEmpty(calc)          || 
           CalcIsNewPrecHigher(calc, **str)  ||
           CalcIsNewRTL(**str))
        {
            break;
        }
        else
        {
            status = CalcFoldOnce(calc);
        }  
    }

    switch(status)
    {
        case E_MATH:
            *str = g_math_error;
            break;

        case E_SYNTAX:
            *str = g_syn_error;
            break;

        default:
            STACKPush(calc->op_stack, (void *)*str);
            ++(*str);
            break;
    }

    return OK;
}

static enum status EOSHandler(char **str, calc_t *calc)
{
    enum status status = OK;
  
    while(0 < STACKSize(calc->op_stack) && OK == status)
    {
        if(CalcIsTopOpStackLBracket(calc))
        {   
            status = E_SYNTAX;
        }
        else
        {
            status = CalcFoldOnce(calc);
        }      
    }

    switch(status)
    {
        case E_MATH:
            *str = g_math_error;
            break;

        case E_SYNTAX:
            *str = g_syn_error;
            break;

        default:
            *str = g_done;
            break;
    }

    return OK;
}

static enum status ClosingBracketHandler(char **str, calc_t *calc)
{  
    enum status status = OK;

    while(!CalcIsOpStackEmpty(calc) && !CalcIsTopOpStackLBracket(calc) && 
                                                                   OK == status)
    {
        status = CalcFoldOnce(calc);
    }

    if(!CalcIsOpStackEmpty(calc))
    {
        STACKPop(calc->op_stack);
    }
    else
    {
        status = E_SYNTAX;
    }

    switch(status)
    {
        case E_MATH:    
            *str = g_math_error;
            break;

        case E_SYNTAX:
            *str = g_syn_error;
            break;

        default:
            *str = *str + 1;
            break;
    }

    return status;
}