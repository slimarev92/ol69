void PrintEnv(char **envp);
void CleanEnv(char **envp);
int NumOfEnvp(char **envp);
void CopyEnv(char **envp, char **envp_dest);
int LivingRight(int soldiers[], int num_of_soldiers, int curr_soldier);
void StabSoldier(int soldiers[], int num_of_soldier);
void DisplaySoldierArray(int soldiers[], int num_of_soldiers);
int SoldierFind(int soldiers[], int num_of_soldiers);
void SoldierArrayInit(int soldiers[], int num_of_soldiers);
void PrintLivingSoldiers(int *soldiers[], int living_soldiers);
int GetNext(int soldiers[], int soldier);
int **GetNextNext(int **soldier);
int KillOne(int soldiers[], int curr_soldier);
int KillIter(int soldiers[], int num_of_kills, int curr_soldier);
int SoldierFind_N(int soldiers[], int num_of_soldiers);
void PrintTypeSizes();


