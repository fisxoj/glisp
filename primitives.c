#include <string.h>
#include <stdio.h>

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
  object *symbol_or_function = car (arguments);
  object *function;

  if (lambda_p (symbol_or_function)) {
    /* printf ("%s: Already a lamba\n", __func__); */
    function = symbol_or_function;
  } else {
    /* printf ("%s: Looking up function %s\n", */
    /* 	    __func__, */
    /* 	    symbol_or_function->data.symbol); */

    function = env_find_function_value (current_env, symbol_or_function);
  }

  object *args = function->data.lambda.arguments;
  arguments = cdr (arguments);
  object *result;
  current_env = env_new (function->data.lambda.env);

  if (!lambda_p (function)) {
    /* printf ("Didn't find a function\n"); */
    return nil;
  }
  /* printf ("%s: args:\n", __func__); */
  /* print (args); */
  /* printf ("\n"); */

  while (!nil_p (args)) {

      // Bind arguments to value or nil, no required arguments
    if (!nil_p (args)) {
      env_bind_symbol (current_env, car (args), car (arguments));
    } else {
      env_bind_symbol (current_env, car (args), nil);
    }

    args = cdr (args);
    arguments = cdr (arguments);
  }

  result = eval (function->data.lambda.body);

  current_env = current_env->parent;

  return result;
}

object *progn (object *body) {

  object *result;

  while (!nil_p (body)) {
    result = eval (car (body));

    body = cdr (body);
  }

  return result;
}

object *progn_wrap (object *body) {
  object *progn = new_object (TYPE_SYMBOL);
  progn->data.symbol = "progn";

  return c_cons (progn, body);
}

object *lambda (object *arguments) {
  object *lambda = new_object (TYPE_LAMBDA);

  env *env = env_new (current_env);
  object *args = car (arguments);
  object *body = cdr (arguments);

  lambda->data.lambda.env = env;
  lambda->data.lambda.arguments = args;
  lambda->data.lambda.body = progn_wrap (body);

  /* printf ("Making new lambda\n"); */
  /* print (args); */
  /* printf ("\n"); */

  return lambda;
}

object *function (object *symbol) {
  object *lambda = env_find_function_value (current_env, symbol);

  return lambda;
}

object *defun (object *arguments) {

  object *lambda_args = cdr (arguments);
  object *lambda_function = lambda (lambda_args);
  object *function_symbol = car (arguments);

  env_bind_function (current_env, function_symbol, lambda_function);

  return lambda_function;
}

object *setq (object *arguments) {
  object *symbol = car (arguments);
  object *value = eval (cadr (arguments));

  env_bind_symbol (current_env, symbol, value);

  return value;
}


object *equal_p (object *a, object* b) {
  /* printf ("%s: comparing %s and %s\n", */
  /* 	  __func__, */
  /* 	  a->data.symbol, */
  /* 	  b->data.symbol); */
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

  PRIMITIVE_FUNCTION(lambda);
  PRIMITIVE_FUNCTION(function);
  PRIMITIVE_FUNCTION(defun);

  PRIMITIVE_FUNCTION(progn);

  /* PRIMITIVE_FUNCTION(eval); */
}
