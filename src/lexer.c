#include <stdio.h>
#include <string.h>
#include <regex.h>

#include "lexer.h"

void lexer_init(lexer *this, FILE *file, const char *filename) {
  this->file = file;
  this->filename = filename;
}

char lexer_next(lexer *this) {
  this->character = fgetc(this->file);
  return this->character;
}

//char lexer_prev(lexer *this) {
//  return ungetc(this->character, this->file);
//}

int isString(char string) {
  regex_t regex;
  int statusCode;

  regcomp(&regex, TOKEN, 0);

  statusCode = regexec(&regex, &string, 0, NULL, 0);

  regfree(&regex);

  return statusCode;
}

int lexer_tokenize(lexer *this) {
  char character;

  next:
    character = lexer_next(this);

    switch (character) {
    // ignore whitespace
    case ' ':
      break;

    // don't care about newlines
    case '\n':
      break;

    // return on EOF
    case EOF:
      return 0;

    // handle token
    default:
      printf("%c", character);
    }

    goto next;
}

int main(int argc, char **args) {
  FILE *fp;

  if ((fp = fopen(args[1], "r")) == NULL) {
    return 1;
  }

  lexer lex;
  lexer_init(&lex, fp, args[1]);

  int statusCode = lexer_tokenize(&lex);

  if (fclose(fp) != 0) {
    return 1;
  }

  return statusCode;
}
