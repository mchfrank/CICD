#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct flags {
    int e;
    int i;
    int v;
    int c;
    int l;
    int n;
    int h;
    int s;
    int f;
    int o;
};
// for fro for or fro forforforforfroofroforffrosenforest
int flag_cve(char *str, char *serch);
void flag_v(char *str, char *serch);
int valid_flag(struct flags *strflags, char argv);
void output(FILE *txt, char *serch, struct flags strflags, int checkFile,
            char* argv, char pattern[200][2000], int n_patt);
