#include <stdio.h>

#include "types.h"
#include "print.h"

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
  case TYPE_SYMBOL:
    printf ("%s", o->data.symbol);
    break;
  default:
    fprintf (stderr, "I don't want to print this");
  }
}
