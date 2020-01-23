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
*    FULL STACK: JAVA TO C (HEADER)                 *
*    CODE BY:            SASHA LIMAREV              *
*    REVIEWED BY:        ANDREI PLAKHOTA            *
*****************************************************/
typedef void (*funct_t)(void *);
typedef struct meta metadata;
typedef struct object Object;
typedef struct Animal Animal;
typedef struct Dog Dog;
typedef struct Cat Cat;
typedef struct LegendaryAnimal LegendaryAnimal;

void AnimalStaticInitizlier();
void AnimalInstanceInitializer(Animal *animal);
void DogStaticInitializer();
void DogInstanceInitializer(Dog *dog);
void CatStaticInitializer();
void CatInstanceInitizlier(Cat *cat);
void LegendaryAnimalStaticInitializer();
char *Object_toString(const Object *obj);
void Animal_sayHello(const Animal *animal);
void Dog_sayHello(const Animal *dog);
void LegendaryAnimal_sayHello(const Animal *legend);
char *LegendaryAnimal_toString(const Object *legend);
char *Cat_toString(const Object *cat);
char *Dog_toString(const Object *dog);
int Animal_getNumMasters(const Animal *animal);
char *Animal_toString(const Object *animal);
void Cat_finalize(Cat *cat);
void LegendaryAnimal_finalize(LegendaryAnimal *legend);
void Dog_finalize(Dog *dog);
void Animal_finalize(Animal *animal);
void Object_finalize(Object *obj);
void Animal_showCounter();
void Java2C_foo(const Animal *animal);
Object *new(metadata *meta);
funct_t GetVirtualMethod(const Object *obj, size_t index);
funct_t GetSuperMethod(const Object *obj, const char *superclass, size_t index);
void Object_constructor(Object *obj);
void Animal_constructor_no_arg(Animal *animal);
void Animal_constructor_int(Animal *animal, int num_masters);
void Dog_constructor_no_arg(Dog *dog);
void Cat_constructor_no_arg(Cat *cat);
void Cat_constructor_str(Cat *cat, char *colors);
void LegendaryAnimal_constroctur_no_arg(LegendaryAnimal *legend);