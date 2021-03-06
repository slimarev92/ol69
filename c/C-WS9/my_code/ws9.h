void *MemSet(void *s, int c, size_t n);
void *MemCpy(void *dest, const void *src, size_t n);
void *MemMove(void *dest, const void *src, size_t n);
int CalculatePart1Iters(int n, size_t dest);
int IsLittleEndian();
char *Itoa(int num, char *str);
char *StrReverse(char *str, int len);
int Atoi(const char *str);
void OnlyFirstTwo(char *arr1, char *arr2, char *arr3, int s1, int s2, int s3);
char *ItoaAny(int num, char *str, int base);
int AtoiAny(const char *str, int base);
int FindLUTIndex(char c);
