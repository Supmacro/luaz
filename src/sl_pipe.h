
#define PRT_ERROR(s) fprintf(stdout, "error: %s", (s));
#define PRT_TAIL_CHR  fprintf(stdout, "\n");


void pipe_stdout(int n, ...);

void  pipe_topath(char top[], char pgname[]);

