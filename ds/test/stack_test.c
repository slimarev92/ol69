#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */
#include <string.h> /* strcmp */

#include "../include/stack.h"

#define GREEN "\x1b[32m"
#define RED "\x1b[31m"
#define RESET_COLOR "\x1b[0m"

#define WORD (8)
#define SIZE_SIMPLE (7)
#define SIZE_COMPLEX (5)

#define RUN_TEST(test_name) \
    if(0 == test_name()) \
    { \
        printf("%s[" #test_name " was completed successfully]\n", GREEN); \
    } \
    else \
    { \
        printf("%s[" #test_name " was completed unsuccessfully]\n", RED); \
    } \
    printf("%s", RESET_COLOR);

typedef struct
{
    char *name;
    char *color;
    char *fave_food;
    int age;
} dog_t;

int TestSimple()
{
    static int test_array[] = {2, 2, 3, 4, 5, 6, 7};
    static int push_status = 0;
    stack_t *test_stack = STACKCreate(SIZE_SIMPLE, sizeof(int));

    int i = 0;
    int *last = NULL;

    for(; i < SIZE_SIMPLE; ++i)
    {
        push_status = STACKPush(test_stack, (void *)&test_array[i]);

        if(0 != push_status)
        {
            return 1;
        }

        last = (int *) STACKPeek(test_stack); 
    }

    for(i = 0; i < 1000; ++i)
    {
        push_status = STACKPush(test_stack, (void *)&push_status);

        if(0 == push_status)
        {
            printf("PUSHED TO FULL STACK SOMEHOW\n");
            return 1;
        }
    }

    for(i = 0; i < SIZE_SIMPLE; ++i)
    {
        last = (int *) STACKPeek(test_stack); 

        if(*last != test_array[6 - i])
        {
            printf("!!! %d != %d\n", *last, test_array[6 - i]);
            return 1;
        }

        STACKPop(test_stack);
    }

    for(i = 0; i < 1000; ++i)
    {
        STACKPop(test_stack);
    }

    STACKDestroy(test_stack);

    return 0;
}

void InitTestArr(dog_t *test_arr)
{
    test_arr[0].name = "Olive";
    test_arr[0].color = "Black";
    test_arr[0].fave_food = "Cabbage";
    test_arr[0].age = 1;

    test_arr[1].name = "Nehama";
    test_arr[1].color = "Yellow-ish";
    test_arr[1].fave_food = "Everything";
    test_arr[1].age = 5;

    test_arr[2].name = "Becky";
    test_arr[2].color = "Orange";
    test_arr[2].fave_food = "Oatmeal";
    test_arr[2].age = 9;

    test_arr[3].name = "Dvora";
    test_arr[3].color = "White and Brown";
    test_arr[3].fave_food = "Chicken";
    test_arr[3].age = 6;

    test_arr[4].name = "Billy";
    test_arr[4].color = "Light Brown";
    test_arr[4].fave_food = "Any human food";
    test_arr[4].age = 11;
}

void DisplayDogInfo(dog_t *dog)
{
    printf("Name: %s\n", dog->name);
    printf("Color: %s\n", dog->color);
    printf("Favorite Food: %s\n", dog->fave_food);
    printf("Age: %d\n", dog->age);
    printf("------------------------\n");
}

int AreDogsSame(dog_t *dog1, dog_t *dog2)
{
    int t1 = strcmp(dog1->name, dog2->name); /* exp: 0 */
    int t2 = strcmp(dog1->color, dog2->color); /* exp: 0 */
    int t3 = strcmp(dog1->fave_food, dog2->fave_food); /* exp: 0 */
    int t4 = (dog1->age == dog2->age); /* exp: !0 */

    if(0 != t1 || 0 != t2 || 0 != t3 || 0 == t4)
    {
        return 0;
    } 
    
    return 1;
}

int TestComplex()
{
    dog_t *test_arr = NULL;
    static int push_status = 0;
    stack_t *test_stack = STACKCreate(SIZE_COMPLEX, sizeof(dog_t));

    int i = 0;

    test_arr = (dog_t *) malloc(sizeof(dog_t) * SIZE_COMPLEX);
    if(NULL == test_arr)
    {
        return 1;
    }

    InitTestArr(test_arr);

    for(; i < SIZE_COMPLEX; ++i)
    {
        push_status = STACKPush(test_stack, (void *)&test_arr[i]);

        if(0 != push_status)
        {
            return 1;
        }
    }

    for(i = 0; i < 1000; ++i)
    {
        push_status = STACKPush(test_stack, (void *)&push_status);

        if(0 == push_status)
        {
            printf("PUSHED TO FULL STACK SOMEHOW\n");
            return 1;
        }
    }

    for(i = 0; i < SIZE_COMPLEX; ++i)
    {
        if(!AreDogsSame(&test_arr[4 - i], (dog_t *) STACKPeek(test_stack)))
        {
            return 1;
        }

        STACKPop(test_stack);
    }

    for(i = 0; i < 1000; ++i)
    {
        STACKPop(test_stack);
    }

    free(test_arr);
    STACKDestroy(test_stack);

    return 0;
}

int main()
{
    RUN_TEST(TestSimple);
    RUN_TEST(TestComplex);

    return 0;
}
