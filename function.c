#include <stdio.h>

#include "function.h"
#include "environment.h"
#include "primitives.h"
#include "eval.h"
#include "types.h"
#include "global.h"
#include "util.h"
#include "print.h"

void define_function (char *name, object *(*function) (object *arguments)) {
  object *symbol = new_object (TYPE_SYMBOL);
  object *primitive = new_object (TYPE_PRIMITIVE);

  symbol->data.symbol = name;

  primitive->data.primitive_function = function;

  env_bind_function (global_scope, symbol, primitive);
}

object *call_function (object *symbol, object *arguments) {
  object *function = env_find_function_value (current_env, symbol);

  /* printf("%s: Found function %s, primitive: %d\n", */
  /* 	 __func__, symbol->data.symbol, primitive_p (function)); */

  if (primitive_p (function)) {
    return (function->data.primitive_function)(arguments);
  } else {
    /* FIXME: Implement calling non-builtin functions */
    // Use function's environment
    return apply (c_cons (symbol, arguments));
  }

}
