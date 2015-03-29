#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "types.h"
#include "global.h"
#include "environment.h"
#include "primitives.h"
#include "eval.h"
#include "print.h"
#include "util.h"
//#include "environment.

object *read (FILE *in);
void print (object *o);

/* Flat out stolen from

https://github.com/petermichaux/bootstrap-scheme/blob/b24bc9dfe11b9caf20805c854382232ad57c46e8/scheme.c
*/

void eat_whitespace (FILE *in) {
    int c;

    while ((c = getc(in)) != EOF) {
        if (isspace(c)) {
            continue;
        }
        else if (c == ';') { /* comments are whitespace also */
            while (((c = getc(in)) != EOF) && (c != '\n'));
            continue;
        }
        ungetc(c, in);
        break;
    }
}

char is_delimiter(int c) {
    return isspace(c) || c == EOF ||
           c == '(' || c == ')' ||
           c == '"' || c == ';';
}

int peek (FILE *in)
{
  int c;

  c = getc (in);
  ungetc (c, in);

  return c;
}

int matches_string (FILE *in, char *string)
{
  int c;

  while (string != '\0') {
    c = getc(in);
    if (c != *string) {
      return false;
    } else {
      string++;
    }
  }

  return true;
}

object *read_character (FILE *in)
{
  object *o = new_object (TYPE_CHARACTER);
  int c = getc (in);

  if (is_delimiter (peek (in))) {
    o->data.character = c;
  } else if (c == 's' && matches_string (in, "pace")) {
    o->data.character = ' ';
  } else if (c == 'n' && matches_string (in, "ewline")) {
    o->data.character = '\n';
  } else {
    fprintf (stderr, "What kind of character is that?");
    return nil;
  }
}

object *read_fixnum (FILE *in)
{
  object *o = new_object (TYPE_FIXNUM);
  int c;
  short sign = 1;
  long numberwang = 0;

  if (peek(in) == '-') {
    getc (in);
    sign = -1;
  }

  while (isdigit (c = getc (in))) {
    numberwang = (numberwang * 10) + (c - '0');
  }

  numberwang *= sign;

  ungetc (c, in);

  o->data.fixnum = numberwang;

  return o;
}

object *read_string (FILE *in)
{
  object *o = new_object (TYPE_STRING);
  int c, i;
#define BUFFER_MAX 1000
  char buffer[BUFFER_MAX];

  i = 0;

  while ((c = getc (in)) != '"') {
    if (c == EOF) {
      fprintf (stderr, "This is the string that never ends, it just tail recurses, my friends...");
      return nil;
    }

    if (i < BUFFER_MAX - 1) {
      buffer[i++] = c;
    }
  }

    buffer[i] = '\0';

    o->data.string = malloc (strlen (buffer));
    strcpy (o->data.string, buffer);

    return o;
}

object *read_cons (FILE *in)
{
  object *o = new_object (TYPE_CONS);

  object *car, *cdr;
  int c;

  c = getc (in);

  if (c == ')')
    return nil;

  ungetc (c, in);

  car = read (in);

  eat_whitespace (in);

  c = getc (in);

  if (c == '.') { // Improper list
    c = peek (in);
    if (!is_delimiter (c)) {
      fprintf (stderr, "Not putting anything after your dots, eh?\n");
      return nil;
    }
    cdr = read (in);
    eat_whitespace (in);

    c = getc (in);

    if (c != ')') {
      fprintf (stderr, "Missing final paren! All is lost! All is lost!\n");
      return nil;
    }
  } else { // Add more items to the list
    ungetc (c, in);
    cdr = read_cons (in);
  }

  o->data.cons.car = car;
  o->data.cons.cdr = cdr;
  return o;
}

object *read_symbol (FILE *in)
{
  int c, i;
#define BUFFER_SIZE 100
  char buffer[BUFFER_SIZE];

  i = 0;
  while (!is_delimiter(c = getc (in))) {
    buffer[i++] = c;
  }

  buffer[i] = '\0';

  ungetc (c, in);

  if (strcmp (buffer, "nil") == 0) {
      return nil;
      //  } IS IT A SYMBOL? {
  } else {
    object *o = new_object(TYPE_SYMBOL);

    o->data.symbol = malloc (i);
    strcpy(o->data.symbol, buffer);
    return o;
  }
}

object *read_quote (FILE *in) {
  /* object *o = new_object(TYPE_QUOTE) */

}

object *read (FILE *in)
{
  int c;

  eat_whitespace (in);

  c = getc (in);

  if (c == 't' && is_delimiter(peek(in))) {
    return t;
  } else if (isalpha(c)) {
    ungetc (c, in);
    return read_symbol (in);
  } else if (c == '#' && peek(in) == '\\') {
    getc (in); // Start at the character
    return read_character (in);
  }  else if (isdigit(c) || (c == '-' && isdigit (peek (in)))) {
    ungetc (c, in);
    return read_fixnum (in);
  } else if (c == '"') {
    return read_string (in);
  /* } else if (c == '\'') { */
  /*   return read_quote (in); */
  } else if (c == '(') {
    return read_cons (in);
  } else {
    fprintf (stderr, "A what not? What does '%c' mean?\n", c);
    return nil;
  }
}


int main (int argc, char **argv)
{
  printf ("Welcome to GLisp, a really crappy lisp!\nPress ^C to exit\n");

  /* Init the singletons! */
  nil = new_object (TYPE_NIL);
  t = new_object (TYPE_T);

  global_scope = env_new (NULL);
  current_env = global_scope;

  register_primitive_functions();

  while (1)
    {
      printf ("> ");

      print (eval (read (stdin)));

      printf ("\n");
    }

  printf ("Thanks for playing!");
}
