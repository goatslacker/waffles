#include <stdio.h>
#include <string.h>
#include <ctype.h>
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

char lexer_prev(lexer *this) {
  this->character = ungetc(this->character, this->file);
  return this->character;
}

void lexer_identifier(lexer *this) {
  char buffer[128];
  int i = 0;

  do {
    buffer[i++] = this->character;
  } while (isalpha(lexer_next(this)));

  printf("IDENTIFIER, %s\n", buffer);
}

void lexer_string(lexer *this) {
  char buffer[128];
  int i = 0;

  while ((lexer_next(this)) != '"') {
    buffer[i++] = this->character;
  }

  printf("STRING, %s\n", buffer);
}

void lexer_tokenize(lexer *this) {

  while (lexer_next(this) != EOF) {
    switch (this->character) {
    // ignore whitespace
    case ' ':
      break;

    case '"':
      lexer_string(this);
      break;

    // don't care about newlines
    case '\n':
      break;

    // handle token
    default:
      if (isalpha(this->character)) {
        lexer_identifier(this);
      } else {
        printf("CHARACTER, %c\n", this->character);
      }
    }
  }

}

int main(int argc, char **args) {
  FILE *fp;

  if ((fp = fopen(args[1], "r")) == NULL) {
    return 1;
  }

  lexer lex;
  lexer_init(&lex, fp, args[1]);

  lexer_tokenize(&lex);

  if (fclose(fp) != 0) {
    return 1;
  }

  return 0;
}
