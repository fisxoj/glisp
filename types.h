#ifndef TYPES_H
#define TYPES_H

enum {
  TYPE_T, // 0
  TYPE_NIL, // 1
  TYPE_SINGLETON, //2
  TYPE_CHARACTER, // 3
  TYPE_STRING, // 4
  TYPE_SYMBOL, // 5
  TYPE_FIXNUM, // 6
  TYPE_RATIONAL, // 7
  TYPE_CLASS, // 8
  TYPE_CONS, // 9
  TYPE_FUNCTION, // 10
  TYPE_PRIMITIVE // 11
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
    char *symbol;

    struct object *(*function);

    struct object *(*primitive_function) (struct object *);
  } data;
} object;

#define PREDICATE_HEADER(t, name)\
  char t##_p (object *o);

PREDICATE_HEADER (t, T);
PREDICATE_HEADER (nil, NIL);
PREDICATE_HEADER (singleton, SINGLETON);
PREDICATE_HEADER (character, CHARACTER);
PREDICATE_HEADER (string, STRING);
PREDICATE_HEADER (symbol, SYMBOL);
PREDICATE_HEADER (fixnum, FIXNUM);
PREDICATE_HEADER (rational, RATIONAL);
PREDICATE_HEADER (class, CLASS);
PREDICATE_HEADER (cons, CONS);
PREDICATE_HEADER (function, FUNCTION);
PREDICATE_HEADER (primitive, PRIMITIVE);

#define true 0;
#define false 1;

object *nil;  // nil or t, there is no try (or f)
object *t;

object *new_object (int type);

#endif
