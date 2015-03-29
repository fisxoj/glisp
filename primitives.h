#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include "types.h"

object *atom (object *object);

object *quote (object *object);

object *eq (object *arguments);

object *car (object *object);

object *cdr (object *object);

object *cons (object *object);

object *cond (object *forms);

object *apply (object *arguments);

object *setq (object *arguments);

object *equal_p (object *a, object *b);

void register_primitive_functions ();
#endif
