#include <stdio.h>
#include "types.h"

int is_truthy (object *object) {
  /* printf("%s: Got type: %d, truthy: %d\n", */
  /* 	 __func__, object->type, !nil_p(object)); */
  return !nil_p(object);
}
