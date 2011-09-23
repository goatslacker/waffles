#include <stdio.h>
#include <string.h>
#include <regex.h>

#include "lexer.h"

//void lexer(Lexer *this, FILE file, const char filename) {
//  FILE *fp;
//
//}

int isString(char string) {
  regex_t regex;
  int statusCode;

  regcomp(&regex, TOKEN, 0);

  statusCode = regexec(&regex, &string, 0, NULL, 0);

  regfree(&regex);

  return statusCode;
}

int tokenize(char args[]) {
  FILE *fp;
  char character;

  if ((fp = fopen(args, "r")) == NULL) {
    return 1;
  }

  next:
    character = fgetc(fp);

    switch (character) {
    // ignore whitespace
    case ' ':
      break;

    // don't care about newlines
    case '\n':
      break;

    // return on EOF
    case EOF:
      if (fclose(fp) != 0) {
        return 1;
      }
      return 0;

    // handle token
    default:
      printf("%c", character);
    }

    goto next;
}

int main(int argc, char **args) {
  return tokenize(args[1]);
}
