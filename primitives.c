#include <string.h>

#include "types.h"
#include "function.h"
#include "environment.h"
#include "util.h"
#include "eval.h"
#include "global.h"
#include "print.h"

#define PRIMITIVE_FUNCTION(name)\
  define_function(#name, name);

object *atom (object *object) {
  if (cons_p (object)) {
    return nil;
  } else {
    return t;
  }
}

object *quote (object *object) {
  return object;
}

object *eq (object *arguments) {

  return t;
};

object *car (object *object) {

  return object->data.cons.car;
}

object *cdr (object *object) {
  if (nil_p (object)) return nil;

  return object->data.cons.cdr;
}

/* Helper definitions */

/* object *first = &car; */

object *cadr (object *argument) {
  return car (cdr (argument));
}

/* END Helper definitions */

object *cons (object *arguments) {
  object *se1 = eval (car (arguments));
  object *se2 = eval (cadr (arguments));
  object *o = new_object(TYPE_CONS);

  o->data.cons.car = se1;
  o->data.cons.cdr = se2;

  return o;
}

object *cond (object *forms) {
  while (!nil_p (car (forms)) && !is_truthy (eval (car (car (forms))))) {
    forms = cdr (forms);
  }

  object *expression = cdr (forms);

  if (nil_p (expression)) {
      return nil;
  } else {
    return eval (expression);
  }
}

object *apply (object *arguments) {
  /* return function_call (function, cons (arg, arguments)); */
  return t;
}

object *setq (object *arguments) {
  object *symbol = car (arguments);
  object *value = eval (cadr (arguments));

  add_symbol_to_env (current_env, symbol, value);

  return value;
}


object *equal_p (object *a, object* b) {
  if (strcmp (a->data.symbol, b->data.symbol) == 0) {
    return t;
  } else {
    return nil;
  }
}

void register_primitive_functions () {
  PRIMITIVE_FUNCTION(atom);
  PRIMITIVE_FUNCTION(quote);
  PRIMITIVE_FUNCTION(eq);
  PRIMITIVE_FUNCTION(car);
  PRIMITIVE_FUNCTION(cdr);
  PRIMITIVE_FUNCTION(cons);

  PRIMITIVE_FUNCTION(cond);
  PRIMITIVE_FUNCTION(apply);
  /* PRIMITIVE_FUNCTION(first); */
  PRIMITIVE_FUNCTION(cadr);
  PRIMITIVE_FUNCTION(setq);

  /* PRIMITIVE_FUNCTION(eval); */
}
