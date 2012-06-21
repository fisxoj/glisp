#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

enum {
  TYPE_T,
  TYPE_NIL,
  TYPE_SINGLETON,
  TYPE_CHARACTER,
  TYPE_STRING,
  TYPE_QUOTE,
  TYPE_FIXNUM,
  TYPE_RATIONAL,
  TYPE_CLASS,
  TYPE_CONS
} Types;

typedef struct object {
  int type;
  union {
    long fixnum; // Fixnum
    struct rational {
      int numerator;
      int denominator;
    } rational;
    char character;
    struct {
      struct object *car;
      struct object *cdr;
    } cons;
    char *string;
  } data;
} object;

#define TYPE_PREDICATE(t, name)			\
  char t##_p (object *o) {			\
    return (o->type == TYPE_##name);		\
  }

TYPE_PREDICATE (t, T);
TYPE_PREDICATE (nil, NIL);
TYPE_PREDICATE (singleton, SINGLETON);
TYPE_PREDICATE (character, CHARACTER);
TYPE_PREDICATE (string, STRING);
TYPE_PREDICATE (quote, QUOTE);
TYPE_PREDICATE (fixnum, FIXNUM);
TYPE_PREDICATE (rational, RATIONAL);
TYPE_PREDICATE (class, CLASS);
TYPE_PREDICATE (cons, CONS);

#define true 0;
#define false 1;

object *read (FILE *in);
void print (object *o);

object *new_object (int type) {
  object *o;

  o = malloc (sizeof (object));

  if (o == NULL) {
    fprintf (stderr, "No more memories!\n");
    exit (1);
  }

  o->type = type;

  return o;
}

object *nil;  // nil or t, there is no try (or f)
object *t;

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
#define BUFFER_SIZE 1000
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
    fprintf (stderr, "What's that supposed to mean?\n");
    return nil;
  }
}

object *eval (object *o)
{
  // Nothing yet
  return o;
}

void print_cons (object *o)
{
  object *cdr;

  printf ("(");
  print (o->data.cons.car);
  printf (" ");

  cdr = o->data.cons.cdr;

  while (cons_p (cdr)) {
    print (cdr->data.cons.car);

    cdr = cdr->data.cons.cdr;

    if (cons_p (cdr))
	printf (" ");
  }

  if (!cons_p(cdr)) {
    if (nil_p (cdr)) { // Finished with the list
      printf (")");
    } else { // Improper list
      printf (" . ");
      print (cdr);
      printf (")");
    }
  }
    
}

void print (object *o)
{

  switch (o->type) {
  case TYPE_NIL:
    printf ("nil");
    break;
  case TYPE_T:
    printf ("t");
    break;
  case TYPE_CHARACTER:
    if (o->data.character == ' ')
      printf ("#\\space");
    else if (o->data.character == '\n')
      printf ("#\\newline");
    else
      printf ("#\\%c", o->data.character);
    break;
  case TYPE_FIXNUM:
    printf ("%ld", o->data.fixnum);
    break;
  case TYPE_RATIONAL:
    printf ("%d/%d", o->data.rational.numerator, o->data.rational.denominator);
    break;
  case TYPE_STRING:
    printf ("\"%s\"", o->data.string);
    break;
  case TYPE_CONS:
    print_cons (o);
    break;
  default:
    fprintf (stderr, "I don't want to print this");
  }
}	   

object *read (FILE *in)
{
  int c;

  eat_whitespace (in);

  c = getc (in);

  if (c == 't') {
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
  }  else if (c == '(') {
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

  while (1)
    {
      printf ("> ");

      print (eval (read (stdin)));

      printf ("\n");
    }

  printf ("Thanks for playing!");
}
