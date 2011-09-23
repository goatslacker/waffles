#define TOKEN "TOKEN"
#define IDENTIFIER "IDENTIFIER"

typedef struct {
  FILE *file;
  char character;
  const char *filename;
} lexer;
