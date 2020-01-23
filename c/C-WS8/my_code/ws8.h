typedef struct var_holder
{
    void *data;
    void (*Print)(void *);
    void (*Add)(struct var_holder *, int);
    void (*Free)(void *);
} var_holder_t;

typedef struct date_of_birth
{
    size_t day;
    size_t month;
    size_t year;
} date_of_birth_t;

typedef struct contact 
{
    char first_name[50];
    char last_name[50];
    date_of_birth_t dob;
    size_t ID;
} contact_t;

void StructOfFuncs();
void PrintInt(void *data);
void PrintFloat(void *data);
void PrintString(void *data);
void AddToInt(var_holder_t *var, int num);
void AddToFloat(var_holder_t *var, int num);
void AddToString(var_holder_t *var, int num);
void DoNothing(void *data);
void InitStructArr();
void InitContactBook();
void DisplayContact(contact_t person);
void DisplayAllContacs();
void WriteContactsToFile();
void ReadContactsFromFile();
void ReadWriteContacts();
