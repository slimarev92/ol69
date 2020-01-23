/* regular functions */


char *Strtok(char *str, const char *delim);
int IsWs(char c);
void MoveBy(char *str, int from, int by, int times);
int RemoveWsBlock(char *str, int leave_ws_chars);
void CleanWs(char *str);
char *Strdup(const char *s);
size_t Strlen(const char *str);
int Strcasecmp(const char *s1, const char *s2);
int Strcmp(const char *s1, const char *s2);
char *Strcpy(char *dest, const char *src);
char *Strncpy(char *dest, const char *src, size_t n);
char *Strchr(const char *s, int c);
char *Strcat(char *dest, const char *src);
char *Strncat(char *dest, const char *src, size_t n);
char *Strstr(char *haystack, const char *needle); 
size_t Strspn(const char *str, const char *accept);
int IsPalindrome(char *str);
void SevenBoom(int from, int to);
void SwapPtr(int **ptr1, int **ptr2);
void AddBigNums(char *num1, char *num2, char *result);
int CharToDigit(char c);



