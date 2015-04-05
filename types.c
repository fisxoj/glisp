#include <stdlib.h>
#include "types.h"

#define TYPE_PREDICATE(t, name)			\
  char t##_p (object *o) {			\
    return (o->type == TYPE_##name);		\
  }

TYPE_PREDICATE (t, T);
TYPE_PREDICATE (nil, NIL);
TYPE_PREDICATE (singleton, SINGLETON);
TYPE_PREDICATE (character, CHARACTER);
TYPE_PREDICATE (string, STRING);
TYPE_PREDICATE (symbol, SYMBOL);
TYPE_PREDICATE (fixnum, FIXNUM);
TYPE_PREDICATE (rational, RATIONAL);
TYPE_PREDICATE (class, CLASS);
TYPE_PREDICATE (cons, CONS);
TYPE_PREDICATE (lambda, LAMBDA);
TYPE_PREDICATE (primitive, PRIMITIVE);

object *new_object (int type) {
  object *o;

  o = malloc (sizeof (object));

  if (o == NULL) {
    exit (1);
  }

  o->type = type;

  return o;
}
