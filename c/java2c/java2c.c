/****************************************************
*    8888888888 .d8888b.   .d8888b.   .d8888b.      *
*    888       d88P  Y88b d88P  Y88b d88P  Y88b     *
*    888       Y88b.      888        888    888     *
*    8888888    "Y888b.   888d888b.  Y88b. d888     *
*    888           "Y88b. 888P "Y88b  "Y888P888     *
*    888             "888 888    888        888     *
*    888       Y88b  d88P Y88b  d88P Y88b  d88P     *
*    888        "Y8888P"   "Y8888P"   "Y8888P"      *
*                                                   *
*    FULL STACK: JAVA TO C (PROGRAM)                *
*    CODE BY:            SASHA LIMAREV              *
*    REVIEWED BY:        ANDREI PLAKHOTA            *
*****************************************************/
#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */
#include <string.h> /* strcmp */

#include "java2c.h" 

#define SIZE_STRING (50)
#define ARR_SIZE (5)
#define TO_FUNCT(NAME) ((void (*)(void *))NAME)

enum method_index {TO_STRING, FINALIZE, SAY_HELLO, GET_N_MASTERS};

struct meta 
{
    const char *name;
    const metadata *superclass;
    const size_t size;
    const funct_t *vtable;
};

/******************************** CLASSES *************************************/

struct object 
{
    metadata *meta;
};

struct Animal 
{
    Object object;
    int num_legs; /* PRIVATE */
    int num_masters; /* PRIVATE */
    int ID; /* PACKAGE PRIVATE */
};

struct Dog
{
    Animal animal;
    int num_legs;
};

struct Cat
{
    Animal animal;
    char *colors; /* PRIVATE */
    int num_masters; /* PRIVATE */
};

struct LegendaryAnimal
{
    Cat cat;
};

/******************************* STATIC FIELDS ********************************/

int Animal_counter = 0;

/********************************* VTABLES ************************************/

const funct_t Object_vtable[] = {TO_FUNCT(Object_toString), 
                                 TO_FUNCT(Object_finalize)};

const funct_t Animal_vtable[] = {TO_FUNCT(Animal_toString), 
                                 TO_FUNCT(Animal_finalize),
                                 TO_FUNCT(Animal_sayHello),
                                 TO_FUNCT(Animal_getNumMasters)};

const funct_t Dog_vtable[] =   {TO_FUNCT(Dog_toString), 
                                TO_FUNCT(Dog_finalize),
                                TO_FUNCT(Dog_sayHello),
                                TO_FUNCT(Animal_getNumMasters)};

const funct_t Cat_vtable[] =   {TO_FUNCT(Cat_toString), 
                                TO_FUNCT(Cat_finalize),
                                TO_FUNCT(Animal_sayHello),
                                TO_FUNCT(Animal_getNumMasters)};


const funct_t LegendaryAnimal_vtable[] =  {TO_FUNCT(LegendaryAnimal_toString), 
                                           TO_FUNCT(LegendaryAnimal_finalize),
                                           TO_FUNCT(LegendaryAnimal_sayHello),
                                           TO_FUNCT(Animal_getNumMasters)};

/********************************* METADATA ***********************************/
metadata Object_metadata = {"Object",
                            NULL,
                            sizeof(Object),
                            Object_vtable };

metadata Animal_metadata = {"Animal",
                            &Object_metadata,
                            sizeof(Object),
                            Animal_vtable };

metadata Dog_metadata = {"Dog",
                        &Animal_metadata,
                        sizeof(Dog),
                        Dog_vtable };

metadata Cat_metadata = {"Cat",
                        &Animal_metadata,
                        sizeof(Cat),
                        Cat_vtable };

metadata LegendaryAnimal_metadata = {"LegendaryAnimal",
                                     &Cat_metadata,
                                     sizeof(LegendaryAnimal),
                                     LegendaryAnimal_vtable };

/************************************** SIB + IIB *****************************/
void AnimalStaticInitizlier()
{
    printf("Static block Animal 1\n");
    printf("Static block Animal 2\n");
}

void AnimalInstanceInitializer(Animal *animal)
{
    printf("Instance initialization block Animal\n");
    animal->num_legs = 5;
    animal->num_masters = 1;
    animal->ID = 0;
}

void DogStaticInitializer()
{
    printf("Static block Dog\n");
}

void DogInstanceInitializer(Dog *dog)
{
    printf("Instance initialization block Dog\n");
    dog->num_legs = 4;
}

void CatStaticInitializer()
{
    printf("Static block Cat\n");
}

void CatInstanceInitizlier(Cat *cat)
{
    cat->num_masters = 5;
}

void LegendaryAnimalStaticInitializer()
{
    printf("Static block Legendary Animal\n");
}

/********************************** VIRTUAL METHODS ***************************/

char *Object_toString(const Object *obj)
{
    char *retval = (char *)malloc(sizeof(char) * SIZE_STRING);

    sprintf(retval, "%s@%lu", obj->meta->name, (size_t)obj); 

    return retval;
}

void Animal_sayHello(const Animal *animal)
{
    printf("Animal Hello!\n");
    printf("I have %d legs\n", animal->num_legs);
}

void Dog_sayHello(const Animal *dog)
{
    printf("Dog Hello!\n");
    printf("I have %d legs\n", ((Dog *)dog)->num_legs);
}

void LegendaryAnimal_sayHello(const Animal *legend)
{
    (void)legend;
    printf("Legendary Hello!\n");
}

char *LegendaryAnimal_toString(const Object *legend)
{
    char *result = (char *)malloc(sizeof(char) * SIZE_STRING);

    sprintf(result, "LegendaryAnimal with ID: %d", ((Animal *)legend)->ID);

    return result;
}
    
char *Cat_toString(const Object *cat)
{
    char *result = (char *)malloc(sizeof(char) * SIZE_STRING);

    sprintf(result, "Cat with ID: %d", ((Animal *)cat)->ID);

    return result;
}

char *Dog_toString(const Object *dog)
{
    char *result = (char *)malloc(sizeof(char) * SIZE_STRING);

    sprintf(result, "Dog with ID: %d", ((Animal *)dog)->ID);

    return result;
}

int Animal_getNumMasters(const Animal *animal)
{
    return animal->num_masters;
}

char *Animal_toString(const Object *animal)
{
    char *result = (char *)malloc(sizeof(char) * SIZE_STRING);

    sprintf(result, "Animal with ID: %d", ((Animal *)animal)->ID);

    return result;
}

void Cat_finalize(Cat *cat)
{
    funct_t super = GetSuperMethod((Object *)cat, "Animal", FINALIZE);

    printf("finalize cat with ID: %d\n", ((Animal *)cat)->ID);    

    super((Animal *)cat);
}

void LegendaryAnimal_finalize(LegendaryAnimal *legend)
{
    funct_t super = GetSuperMethod((Object *)legend, "Cat", FINALIZE);

    printf("finalize LegendaryAnimal with ID: %d\n", ((Animal *)legend)->ID);    

    super((Animal *)legend);
}

void Dog_finalize(Dog *dog)
{
    funct_t super = GetSuperMethod((Object *)dog, "Animal", FINALIZE);

    printf("finalize dog with ID: %d\n", ((Animal *)dog)->ID);    

    super((Animal *)dog);
}

void Animal_finalize(Animal *animal)
{
    funct_t super = NULL;

    printf("finalize Animal with ID: %d\n", animal->ID);

    super = GetSuperMethod((Object *)animal, "Object", FINALIZE);

    super((Object *)animal);
}

void Object_finalize(Object *obj)
{
    printf("finalizing object\n");
    free(obj);
}

/**************************** STATIC METHODS **********************************/

void Animal_showCounter()
{
    printf("%d\n", Animal_counter);
}

void Java2C_foo(const Animal *animal)
{
    funct_t func = GetVirtualMethod((Object *)animal, TO_STRING);

    printf("%s\n", ((char *(*)(Object *))func)((Object *)animal));
}

/************************ UTILITY METHODS *************************************/

Object *new(metadata *meta) 
{
    Object *new_obj = (Object *)malloc(meta->size); 
    new_obj->meta = meta;

    return new_obj;
}

funct_t GetVirtualMethod(const Object *obj, size_t index)
{
    return *((funct_t *)obj->meta->vtable + index);
}

funct_t GetSuperMethod(const Object *obj, const char *superclass, size_t index)
{
    metadata *curr_meta = (metadata *)obj->meta->superclass;

    while(0 != strcmp(curr_meta->name, superclass))
    {
        curr_meta = (metadata *)curr_meta->superclass;
    }

    return *(curr_meta->vtable + index);
}

/*********************** CONSTRUTORS ******************************************/

void Object_constructor(Object *obj) 
{
    (void)obj;
}

void Animal_constructor_no_arg(Animal *animal) 
{
    char *str = NULL;
    funct_t func = NULL;
    Object_constructor((Object *)animal);
    AnimalInstanceInitializer(animal);
    printf("Animal Ctor\n");
    ++Animal_counter;
    animal->ID = Animal_counter;

    func = GetVirtualMethod((Object *)animal, SAY_HELLO);
    ((void (*)(Animal *))func)(animal);

    func = GetVirtualMethod((Object *)animal, TO_STRING);
    str = ((char *(*)(Object *))func)((Object *)animal);

    Animal_showCounter();
    printf("%s\n", str);

    func = GetSuperMethod((Object *)animal, "Object", TO_STRING);
    str = ((char *(*)(Object *))func)((Object *)animal);
    printf("%s\n", str);  
}

void Animal_constructor_int(Animal *animal, int num_masters)
{
    AnimalInstanceInitializer(animal);
    printf("Animal Ctor int\n");
    ++Animal_counter;
    animal->ID = Animal_counter;
    animal->num_masters = num_masters;
}

void Dog_constructor_no_arg(Dog *dog)
{
    Animal_constructor_int((Animal *)dog, 2);
    DogInstanceInitializer(dog);
    printf("Dog Ctor\n");
}

void Cat_constructor_no_arg(Cat *cat)
{
    Cat_constructor_str(cat, "black");
    printf("Cat Ctor\n");
    cat->num_masters = 2;
}

void Cat_constructor_str(Cat *cat, char *colors)
{
    Animal_constructor_no_arg((Animal *)cat);
    CatInstanceInitizlier(cat);
    cat->colors = colors;
    printf("Cat Ctor with color: %s\n", cat->colors);
}

void LegendaryAnimal_constructor_no_arg(LegendaryAnimal *legend)
{
    Cat_constructor_no_arg((Cat *)legend);
    printf("Legendary Ctor\n");
}

/************************************** MAIN **********************************/

int main()
{
    size_t i = 0;
    funct_t func = NULL;

    Animal *animal = (Animal *)new(&Animal_metadata);
    Dog *dog = (Dog *)new(&Dog_metadata);
    Cat *cat = (Cat *)new(&Cat_metadata);
    LegendaryAnimal *la = (LegendaryAnimal *)new(&LegendaryAnimal_metadata); 
    Animal *array[5] = {0};

    AnimalStaticInitizlier();
    Animal_constructor_no_arg(animal); 

    DogStaticInitializer();
    Dog_constructor_no_arg(dog); 

    CatStaticInitializer();
    Cat_constructor_no_arg(cat);

    LegendaryAnimalStaticInitializer();
    LegendaryAnimal_constructor_no_arg(la);

    Animal_showCounter();

    printf("%d\n", animal->ID);
    printf("%d\n", ((Animal *)dog)->ID);
    printf("%d\n", ((Animal *)cat)->ID);
    printf("%d\n", ((Animal *)la)->ID);
    
    array[0] = (Animal *)new(&Dog_metadata);
    Dog_constructor_no_arg((Dog *)array[0]);

    array[1] = (Animal *)new(&Cat_metadata);
    Cat_constructor_no_arg((Cat *)array[1]);

    array[2] = (Animal *)new(&Cat_metadata);
    Cat_constructor_str((Cat *)array[2], "white");

    array[3] = (Animal *)new(&LegendaryAnimal_metadata);
    LegendaryAnimal_constructor_no_arg((LegendaryAnimal *)array[3]);

    array[4] = (Animal *)new(&Animal_metadata);
    Animal_constructor_no_arg((Animal *)array[4]); 

    for(i = 0; i < ARR_SIZE; ++i)
    {
        func = GetVirtualMethod((Object *)array[i], SAY_HELLO);
        ((void (*)(Animal *))func)((Animal *)array[i]);

        func = GetVirtualMethod((Object *)array[i], GET_N_MASTERS);
        printf("%d\n", ((int (*)(Animal *))func)(array[i]));

    }   

    for(i = 0; i < ARR_SIZE; ++i)
    {
        Java2C_foo(array[i]);
    } 

    return 0;
}