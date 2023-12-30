#include <pcre.h>
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

void flag_o(char *str, char *serch);
int flag_cve(char *str, char *serch, struct flags strflags);
void flag_v(char *str, char *serch);
int valid_flag(struct flags *strflags, char argv);
void output(FILE *txt, char *serch, struct flags strflags, int checkFile,
            char *argv, char pattern[200][2000], int n_patt);

int main(int args, char *argv[]) {
  FILE *txt;
  int chek = 1, checkFile = 0;
  char errorUse[350] =
      "usage: s21_grep [-abcDEFGHhIiJLlmnOoqRSsUVvwxZ] [-A num] [-B num] "
      "[-C[num]]\n\t"
      "[-e pattern] [-f file] [--binary-files=value] [--color=when]\n\t"
      "[--context[=num]] [--directories=action] [--label] [--line-buffered]\n\t"
      "[--null] [pattern] [file ...]\n";
  int i = 1;
  char pattern[200][2000] = {'\0'};
  int n_patt = 1;
  struct flags strflags;
  strflags.e = 0;
  strflags.i = 0;
  strflags.v = 0;
  strflags.c = 0;
  strflags.l = 0;
  strflags.n = 0;
  strflags.h = 0;
  strflags.s = 0;
  strflags.f = 0;
  strflags.o = 0;
  char serch[2000];
  char str[2000];

  if (args < 3) {
    printf("%s", errorUse);
    chek = 0;
  }

  //  чтение флагов
  for (int f = 1; f < args && chek == 1; f++) {
    if (argv[f][0] == '-') {
      if (strlen(argv[f]) == 2) {
        if (valid_flag(&strflags, argv[f][1]) == 1) {
          printf("s21_grep: illegal option -- %c\n%s", argv[i][1], errorUse);
          chek = 0;
        }
      } else {
        for (int j = 1; argv[f][j] != '\0' && chek == 1; j++) {
          if (argv[f][j] == 'f' || argv[f][j] == 'e') {
            valid_flag(&strflags, argv[f][j]);
            chek = 2;
          } else if (valid_flag(&strflags, argv[f][j]) == 1) {
            printf("s21_grep: illegal option -- %c\n%s", argv[f][j], errorUse);
            chek = 0;
          }
        }
      }
    }
  }
  if (chek == 2) chek = 1;

  //  чтение паттернов
  if (chek == 1 && strflags.e == 1) {
    n_patt = 0;
    for (int f = 1; f < args; f++) {
      if (strlen(argv[f]) == 2 && argv[f][0] == '-' && argv[f][1] == 'e' &&
          f + 1 < args) {
        strcpy(pattern[n_patt], argv[f + 1]);
        n_patt++;
      } else if (argv[f][0] == '-' && strlen(argv[f]) > 2) {
        for (int j = 1; argv[f][j] != '\0'; j++) {
          if (argv[f][j] == 'e' && strlen(argv[f]) != (size_t)j + 1) {
            strcpy(pattern[n_patt], argv[f] + j + 1);
            n_patt++;
          } else if (argv[f][j] == 'e' && strlen(argv[f]) == (size_t)j + 1) {
            strcpy(pattern[n_patt], argv[f + 1]);
            n_patt++;
          }
        }
      }
    }
  }

  // паттерны из флага f
  if (chek == 1 && strflags.f == 1) {
    for (int f = 1; f < args; f++) {
      if (strlen(argv[f]) == 2 && argv[f][0] == '-' && argv[f][1] == 'f' &&
          f + 1 < args) {
        txt = fopen(argv[f + 1], "r");
        if (txt == NULL) {
          printf("s21_grep: %s: No such file or directory\n", argv[f + 1]);
          chek = 0;
        } else {
          while (!feof(txt)) {
            if (fgets(str, 2000, txt)) {
              strcpy(pattern[n_patt], str);
              if (pattern[n_patt][strlen(pattern[n_patt]) - 1] == '\n')
                pattern[n_patt][strlen(pattern[n_patt]) - 1] = '\0';
              n_patt++;
            }
          }
        }
        if (txt != NULL) fclose(txt);
      } else if (argv[f][0] == '-' && strlen(argv[f]) > 2) {
        for (int j = 1; argv[f][j] != '\0' && chek == 1; j++) {
          if (argv[f][j] == 'f' && strlen(argv[f]) != (size_t)j + 1) {
            txt = fopen(argv[f] + j + 1, "r");
            if (txt == NULL) {
              printf("s21_grep: %s: No such file or directory\n",
                     argv[f] + j + 1);
              chek = 0;
            } else {
              while (!feof(txt)) {
                if (fgets(str, 2000, txt)) {
                  strcpy(pattern[n_patt], str);
                  if (pattern[n_patt][strlen(pattern[n_patt]) - 1] == '\n')
                    pattern[n_patt][strlen(pattern[n_patt]) - 1] = '\0';
                  n_patt++;
                }
              }
            }
            if (txt != NULL) fclose(txt);
          } else if (argv[f][j] == 'f' && strlen(argv[f]) == (size_t)j + 1 &&
                     f + 1 < args) {
            txt = fopen(argv[f + 1], "r");
            if (txt == NULL) {
              printf("s21_grep: %s: No such file or directory\n", argv[f + 1]);
              chek = 0;
            } else {
              while (!feof(txt)) {
                if (fgets(str, 2000, txt)) {
                  strcpy(pattern[n_patt], str);
                  if (pattern[n_patt][strlen(pattern[n_patt]) - 1] == '\n')
                    pattern[n_patt][strlen(pattern[n_patt]) - 1] = '\0';
                  n_patt++;
                }
              }
            }
            if (txt != NULL) fclose(txt);
          }
        }
      }
    }
  }

  // счетчик файлов
  for (int f = 1; f < args; f++) {
    if (argv[f][0] != '-' && argv[f - 1][0] != '-' && f - 1 != 0 &&
        strflags.h == 0) {
      checkFile++;
    } else if (argv[f][0] != '-' && argv[f - 1][0] == '-' && strflags.h == 0 &&
               strlen(argv[f - 1]) > 2 &&
               argv[f - 1][strlen(argv[f - 1]) - 1] != 'e' &&
               argv[f - 1][strlen(argv[f - 1]) - 1] != 'f') {
      checkFile++;
    }
  }

  if (checkFile == 0 && chek == 1 && strflags.h == 0) {
    printf("%s", errorUse);
    chek = 0;
  }

  // запуск поиска
  if (chek == 1 && strflags.e == 0) {
    for (; i < args; i++) {
      if (argv[i][0] != '-' && chek == 1) {
        strcpy(serch, argv[i]);
        chek = 0;
        continue;
      }
      if (chek == 0) {
        if (argv[i][0] != '-') {
          txt = fopen(argv[i], "r");
          if (txt == NULL) {
            printf("s21_grep: %s: No such file or directory\n", argv[i]);
          } else {
            output(txt, serch, strflags, checkFile, argv[i], pattern, n_patt);
          }
          if (txt != NULL) fclose(txt);
        }
      }
    }
  } else if (chek == 1) {
    for (; i < args; i++) {
      if ((argv[i][0] != '-' && argv[i - 1][0] != '-') ||
          (argv[i][0] != '-' && argv[i - 1][0] == '-' &&
           strlen(argv[i - 1]) > 2 &&
           argv[i - 1][strlen(argv[i - 1]) - 1] != 'e' &&
           argv[i - 1][strlen(argv[i - 1]) - 1] != 'f')) {
        txt = fopen(argv[i], "r");
        if (txt == NULL) {
          printf("s21_grep: %s: No such file or directory\n", argv[i]);
        } else {
          output(txt, serch, strflags, checkFile, argv[i], pattern, n_patt);
        }
        if (txt != NULL) fclose(txt);
      }
    }
  }
  return 0;
}

int valid_flag(struct flags *strflags, char argv) {
  int result = 1;

  switch (argv) {
    case 'e':
      strflags->e = 1;
      result = 0;
      break;
    case 'i':
      strflags->i = 1;
      result = 0;
      break;
    case 'v':
      strflags->v = 1;
      result = 0;
      break;
    case 'c':
      strflags->c = 1;
      result = 0;
      break;
    case 'l':
      strflags->l = 1;
      result = 0;
      break;
    case 'n':
      strflags->n = 1;
      result = 0;
      break;
    case 'h':
      strflags->h = 1;
      result = 0;
      break;
    case 's':
      strflags->s = 1;
      result = 0;
      break;
    case 'f':
      strflags->f = 1;
      result = 0;
      break;
    case 'o':
      strflags->o = 1;
      result = 0;
      break;
  }
  return result;
}

void output(FILE *txt, char *serch, struct flags strflags, int checkFile,
            char *argv, char pattern[200][2000], int n_patt) {
  char str[1000];
  int score = 0;
  int chek = 0;
  int strScore = 0, strScore2;

  while (!feof(txt)) {
    if (fgets(str, 1000, txt)) {
      strScore++;
      for (int i = 0; i < n_patt; i++) {
        if (strflags.e == 1) strcpy(serch, pattern[i]);
        if (strflags.v == 1) {
          if (flag_cve(str, serch, strflags) == 0 && chek != 2) {
            chek = 1;
          } else {
            chek = 2;
          }
        } else {
          if (flag_cve(str, serch, strflags) == 1) chek = 1;
        }
        if (strflags.o == 1 && chek == 1 && strflags.v == 0 &&
            strflags.l == 0 && strflags.c == 0) {
          if (checkFile > 1) {
            printf("%s:", argv);
          }
          if (strflags.n == 1 && strScore2 != strScore) {
            printf("%d:", strScore);
            strScore2 = strScore;
          }
          flag_o(str, serch);
          chek = 0;
        }
      }
      if (chek == 2) chek = 0;
      if (strflags.l == 1 && chek == 1) {
        score = 1;
        chek = 0;
      } else if (strflags.c == 1 && chek == 1) {
        score++;
        chek = 0;
      }
      if (checkFile > 1 && chek == 1) {
        printf("%s:", argv);
      }
      if (strflags.n == 1 && chek == 1) {
        printf("%d:", strScore);
      }
      if (chek == 1) {
        printf("%s", str);
        if (!feof(txt))
          chek = 0;
        else
          chek = 1;
      }
    }
  }
  if (chek == 1) printf("\n");
  if (strflags.c == 1) {
    if (checkFile > 1) printf("%s:", argv);
    printf("%d\n", score);
  }
  if (strflags.l == 1 && score == 1) {
    printf("%s\n", argv);
  }
}

int flag_cve(char *str, char *serch, struct flags strflags) {
  const char *error;
  int over[30] = {0};
  int erroffset, fl = 0, result = 0;
  int f_i = 0;
  pcre *pcre;

  if (strflags.i == 1) f_i = PCRE_CASELESS;
  pcre = pcre_compile(serch, f_i, &error, &erroffset, NULL);
  fl = pcre_exec(pcre, NULL, str, strlen(str), 0, 0, over, 30);
  if (fl == 1) result = 1;
  free(pcre);

  return result;
}

void flag_o(char *str, char *serch) {
  const char *error;
  int over[3] = {0};
  int erroffset, fl = 0;
  int f_i = 0;
  pcre *pcre;

  pcre = pcre_compile(serch, f_i, &error, &erroffset, NULL);
  while (fl != -1) {
    fl = pcre_exec(pcre, NULL, str, strlen(str), over[1], 0, over, 3);
    if (fl == 1) {
      for (int i = over[0]; i < over[1]; i++) printf("%c", str[i]);
      printf("\n");
    }
  }
  free(pcre);
}
