#include <stdio.h>
#include "types.h"

int is_truthy (object *object) {
  /* printf("%s: Got type: %d, truthy: %d\n", */
  /* 	 __func__, object->type, !nil_p(object)); */
  return !nil_p(object);
}

object *c_cons (object *se1, object *se2) {
  object *cons = new_object (TYPE_CONS);

  cons->data.cons.car = se1;
  cons->data.cons.cdr = se2;

  return cons;
}
