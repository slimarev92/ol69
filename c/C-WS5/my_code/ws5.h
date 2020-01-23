enum action_status {PROG_EXIT, SUCCESS, ERROR};

enum action_status ExitProg(char *str);
enum action_status Remove(char *str);
enum action_status Count(char *str);
enum action_status AppendEnd(char *str);
enum action_status Prepend(char *str);

int PrependCmp(const char *input, const char *str);
int AppendCmp(const char *input, const char *str);
int StrCmpBool(const char *s1, const char *s2);

void ChainMechanism();
void Start(char *argv[], int argc);
void ErrorCheck(enum action_status curr_status);

int DoesFileExist();
char *ReadLine(FILE *stream, char *str);
void ErrorCheck(enum action_status curr_status);
int CountLines(FILE *stream);
enum action_status CopyFileContents(char *path_src, char *path_dst);


