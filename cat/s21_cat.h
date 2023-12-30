struct flag{
    int E;
    int v;
    int e;
    int n;
    int T;
    int b;
    int t;
    int s;
};

void nach(struct flag *cat);
void flagN(struct flag *flg, int pust, int edit, int *fl);
void flagE(struct flag *flg, int *pust, int *edit, int *numberpust);
void flagB(struct flag *flg, int pust, int edit, int *fl);
void flagT(struct flag *flg, char ch);
int parsing(struct flag *flg, char *str);
int fpr(struct flag *flg, char *file, int fl, int *edit);
int sch(struct flag *flg);
void lastproc(struct flag *flg, char ch, int *edit, int countflg);