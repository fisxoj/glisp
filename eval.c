#include <stdio.h>
#include "types.h"
#include "util.h"
#include "primitives.h"
#include "function.h"
#include "global.h"

object *eval (object *o)
{
  /* printf("%s: Trying to eval object type: %d\n", __func__, o->type); */

if (is_truthy (atom (o))) {
  if (symbol_p (o)) {
    /* printf("%s: Looking for symbol\n", __func__); */
    return env_find_symbol_value (current_env, o);
  } else {
    /* printf("%s: Not eval-ing\n", __func__); */
    return o;
  }
 } else {
  /* printf("%s: Function call\n", __func__); */
    object *symbol = car (o);
    object *arguments = cdr (o);

    return call_function (symbol, arguments);
  }
}
