#include <stdio.h>
#include <string.h>
#include <regex.h>

#include "lexer.h"

int isToken() {
  regex_t regex;
  int statusCode;

  regcomp(&regex, TOKEN, 0);

  statusCode = regexec(&regex, "hello world", 0, NULL, 0);

  regfree(&regex);

  if (!statusCode) {
    return 1;
  } else {
    return 0;
  }
}

int main(int argc, char *argv[]) {

//  char str[15] = "hello world";
  int val;

  val = isToken();
  if (val == 1) {
    printf("It's a token!\n");
  } else {
    printf(":(\n");
  }

  return 0;
}
