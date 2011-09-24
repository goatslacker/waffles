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

int lexer_isIdentifier(lexer *this, char *buffer) {
  if (strcmp(buffer, "return") == 0) {
    return 0;
  } else if (strcmp(buffer, "print") == 0) {
    return 0;
  } else {
    return 1;
  }
}

void lexer_identifier(lexer *this) {
  char buffer[128];
  char *type_of;
  int i = 0;

  do {
    buffer[i++] = this->character;
  } while (isalpha(lexer_next(this)));

  int is_ident = lexer_isIdentifier(this, buffer);

  if (is_ident == 0) {
    type_of = buffer;
  } else {
    type_of = "IDENTIFIER";
  }

  printf("%s, %s\n", type_of, buffer);

  for (i = 0; i < 128; i += 1) {
    buffer[i] = 0;
  }
}

void lexer_string(lexer *this) {
  char buffer[128];
  int i = 0;

  while ((lexer_next(this)) != '"') {
    buffer[i++] = this->character;
  }

  printf("STRING, %s\n", buffer);

  for (i = 0; i < 128; i += 1) {
    buffer[i] = 0;
  }
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
        printf("%c, %c\n", this->character, this->character);
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
