typedef struct stack stack_t;

void STACKPop(stack_t *stack);
int STACKPush(stack_t *stack, const void *element);
void *STACKPeek(const stack_t *stack);
stack_t *STACKCreate(size_t n_elements, size_t size_of_element);
size_t STACKSize(const stack_t *stack);
void STACKDestroy(stack_t *stack);
