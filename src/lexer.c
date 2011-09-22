#include <stdio.h>
#include <string.h>
#include <regex.h>

#include "lexer.h"

int main(int argc, char **args) {
  FILE *fp;
  char character;

  if ((fp = fopen(args[1], "r")) == NULL) {
    return 1;
  }

  character = fgetc(fp);

  printf("%c\n", character);

  if (fclose(fp) != 0) {
    return 1;
  }

  return 0;
}
