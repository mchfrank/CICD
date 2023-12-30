#include "s21_cat.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
  int err = 0;
  struct flag flg;
  nach(&flg);
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      if (parsing(&flg, argv[i])) {
        err = 1;
        break;
      }
    }
  }
  if (!err) {
    int fl = 1, edit = 1;
    for (int i = 1; i < argc; i++) {
      if (argv[i][0] != '-') fl = fpr(&flg, argv[i], fl, &edit);
    }
  }
  return 0;
}

void nach(struct flag *cat) {
  cat->e = 0;
  cat->E = 0;
  cat->v = 0;
  cat->n = 0;
  cat->t = 0;
  cat->T = 0;
  cat->s = 0;
  cat->b = 0;
}

void flagN(struct flag *flg, int pust, int edit, int *fl) {
  if (pust == 0 && flg->n == 1) {
    if (edit) {
      printf("%6d\t", *fl);
      *fl += 1;
    }
  }
}

void flagE(struct flag *flg, int *pust, int *edit, int *numberpust) {
  if (flg->e || flg->E) printf("$");
  if (*pust == 0) *numberpust += 1;
  printf("\n");
  *pust = 0;
  *edit = 1;
}

void flagB(struct flag *flg, int pust, int edit, int *fl) {
  if (pust == 0 && flg->b == 1) {
    if (edit) {
      printf("%6d\t", *fl);
      *fl += 1;
    }
  }
}

void flagT(struct flag *flg, char ch) {
  if (ch == '\t') {
    if (flg->t || flg->T) {
      printf("^I");
    } else {
      printf("%c", ch);
    }
  } else if (((ch < 32 && ch >= 0) || ch == 127) && flg->v) {
    printf("^%c", ch + 64);
  } else {
    printf("%c", ch);
  }
}

void lastproc(struct flag *flg, char ch, int *edit, int countflg) {
  if (ch != '\n') {
    *edit = 0;
    if ((flg->b || flg->n) && countflg == 1) {
      printf("\n");
      *edit = 1;
    }
  } else {
    *edit = 1;
  }
}

int fpr(struct flag *flg, char *file, int fl, int *edit) {
  if (flg->b) flg->n = 0;
  if (flg->t || flg->e) flg->v = 1;
  char ch;
  FILE *fp;
  fp = fopen(file, "r");
  if (fp == NULL) {
    printf("cat: can't open '%s': No such file or directory\n", file);
  } else {
    int pust = 0, numberpust = 0, countflg = sch(flg);
    while (fscanf(fp, "%c", &ch) != -1) {
      if (ch == '\n' && (pust == 0 && numberpust != 0 && flg->s == 1)) continue;
      flagN(flg, pust, *edit, &fl);
      if (ch == '\n') {
        flagE(flg, &pust, edit, &numberpust);
      } else {
        numberpust = 0;
        flagB(flg, pust, *edit, &fl);
        flagT(flg, ch);
        pust++;
      }
    }
    lastproc(flg, ch, edit, countflg);
    fclose(fp);
  }
  return fl;
}

int sch(struct flag *flg) {
  int count = 0;
  count = flg->e + flg->E + flg->v + flg->n + flg->t + flg->T + flg->s + flg->b;
  return count;
}

int pointer_options(struct flag *flg, char read) {
  int err = 0;
  switch (read) {
    case 'e':
      flg->e = 1;
      break;
    case 'E':
      flg->E = 1;
      break;
    case 'n':
      flg->n = 1;
      break;
    case 't':
      flg->t = 1;
      break;
    case 'T':
      flg->T = 1;
      break;
    case 's':
      flg->s = 1;
      break;
    case 'b':
      flg->b = 1;
      break;
    case 'v':
      flg->v = 1;
      break;
    default:
      printf("cat: invalid option -- '%c'\n", read);
      printf("Try 'cat --help' for more information.\n");
      err = 1;
  }
  return err;
}

int parsing(struct flag *flg, char *str) {
  int err = 0;
  str++;
  while (*str != '\0') {
    if (pointer_options(flg, *str)) {
      err = 1;
      break;
    }
    str++;
  }
  return err;
}
