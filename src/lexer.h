#define TOKEN "TOKEN"
#define IDENTIFIER "IDENTIFIER"

const char *KEYWORDS[] = {
  "else",
  "if",
  "print",
  "return",
  "while"
};

typedef struct {
  FILE *file;
  char character;
  const char *filename;
  int line;
} lexer;
