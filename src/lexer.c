#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <regex.h>

#include "lexer.h"

void lexer_init(lexer *this, FILE *file, const char *filename) {
  this->file = file;
  this->filename = filename;
  this->line = 1;
}

char lexer_next(lexer *this) {
  this->character = fgetc(this->file);
  return this->character;
}

char lexer_prev(lexer *this) {
  this->character = ungetc(this->character, this->file);
  return this->character;
}

int lexer_isComparison(lexer *this) {
  int is_comparison;

  lexer_next(this);

  // FIXME: > and < will not be scanned properly
  is_comparison = (this->character == '=');

  lexer_prev(this);

  return is_comparison;
}

int lexer_isLogic(lexer *this) {
  int is_logical;

  char next = this->character;

  lexer_next(this);

  is_logical = (this->character == next);

  lexer_prev(this);

  return is_logical;
}

int lexer_isIdentifier(lexer *this, char *buffer) {
  int i = 0;

  for (i; sizeof(KEYWORDS); i += 1) {
    if (strcmp(buffer, KEYWORDS[i])) {
      return 0;
    }
  }

  return 1;
}

void lexer_number(lexer *this) {
  char buffer[128];
  char *type_of;
  int i = 0;

  do {
    buffer[i++] = this->character;
  } while (isdigit(lexer_next(this)));

  printf("NUMBER, %s, %d\n", buffer, this->line);

  for (i = 0; i < 128; i += 1) {
    buffer[i] = 0;
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

  printf("%s, %s, %d\n", type_of, buffer, this->line);

  for (i = 0; i < 128; i += 1) {
    buffer[i] = 0;
  }
}

void lexer_string(lexer *this) {
  // TODO buffer 128 :(
  char buffer[128];
  int i = 0;

  while ((lexer_next(this)) != '"') {
    buffer[i++] = this->character;
  }

  printf("STRING, %s, %d\n", buffer, this->line);

  for (i = 0; i < 128; i += 1) {
    buffer[i] = 0;
  }
}

void lexer_ignore_comments(lexer *this) {
  next:
    if (lexer_next(this) != '\n') goto next;

  this->line++;
}

void lexer_tokenize(lexer *this) {
  int comparison, logic;

  while (lexer_next(this) != EOF) {
    switch (this->character) {
    // ignore whitespace
    case ' ':
      break;

    case '>':
    case '<':
    case '!':
    case '=':
      comparison = lexer_isComparison(this);
      if (comparison == 1) {
        printf("COMPARE, %c%c, %d\n", this->character, lexer_next(this), this->line);
      }
      break;

    case '|':
    case '&':
      logic = lexer_isLogic(this);
      if (logic == 1) {
        printf("LOGIC, %c%c, %d\n", this->character, lexer_next(this), this->line);
      }
      break;

    case '#':
      lexer_ignore_comments(this);
      break;

    case '"':
      lexer_string(this);
      break;

    // increment line counter
    case '\n':
      this->line++;
      break;

    // handle token
    default:
      if (isalpha(this->character)) {
        lexer_identifier(this);
      } else if (isdigit(this->character)) {
        lexer_number(this);
      } else {
        printf("%c, %c, %d\n", this->character, this->character, this->line);
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
