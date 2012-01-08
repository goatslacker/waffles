#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "waffle.h"
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

// Stringy stuff.
void string_new(String *string) {
  string->string = malloc(0);
  string->size = 0;
}

void string_add(char chr, String *string) {
  string->size += 1;
  string->string = realloc(string->string, string->size);

  string->string[string->size - 1] = chr;
}

char *string_get(String *string) {
  return string->string;
}

void string_clear(String *string) {
  int i = 0;

  free(string->string);

  for (i; i < string->size; i += 1) {
    string->string[i] = 0;
  }

  string->string = NULL;
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
      return false;
    }
  }

  return true;
}

void lexer_number(lexer *this) {
  String str;
  string_new(&str);

  do {
    string_add(this->character, &str);
  } while (isdigit(lexer_next(this)));

  lexer_prev(this);

  printf("NUMBER, %s, %d\n", string_get(&str), this->line);

  string_clear(&str);
}

void lexer_identifier(lexer *this) {
  char *type_of;

  String str;
  string_new(&str);

  do {
    string_add(this->character, &str);
  } while (isalpha(lexer_next(this)));

  lexer_prev(this);

  int is_ident = lexer_isIdentifier(this, string_get(&str));

  if (is_ident == false) {
    type_of = string_get(&str);
  } else {
    type_of = "IDENTIFIER";
  }

  printf("%s, %s, %d\n", type_of, string_get(&str), this->line);

  string_clear(&str);
}

void lexer_string(lexer *this) {
  String str;
  string_new(&str);

  while ((lexer_next(this)) != '"') {
    string_add(this->character, &str);
  }

  printf("STRING, %s, %d\n", string_get(&str), this->line);

  string_clear(&str);
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
    // increment line counter
    case '\n':
      this->line++;
      break;

    // ignore whitespace
    case ' ':
      break;

    case '>':
    case '<':
    case '!':
    case '=':
      comparison = lexer_isComparison(this);
      if (comparison == true) {
        printf("COMPARE, %c%c, %d\n", this->character, lexer_next(this), this->line);
      }
      break;

    case '|':
    case '&':
      logic = lexer_isLogic(this);
      if (logic == true) {
        printf("LOGIC, %c%c, %d\n", this->character, lexer_next(this), this->line);
      }
      break;

    case '%':
      lexer_ignore_comments(this);
      break;

    case '"':
      lexer_string(this);
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
    return error;
  }

  lexer lex;
  lexer_init(&lex, fp, args[1]);

  lexer_tokenize(&lex);

  if (fclose(fp) != ok) {
    return error;
  }

  return ok;
}
