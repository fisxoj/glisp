#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "types.h"

typedef struct env {
  struct env *parent;
  object **symbols;
  object **symbol_values;

  int symbol_count;
  int symbol_extent;

  object **functions;
  object **function_values;

  int function_count;
  int function_extent;

} env;

env* env_new (env *parent);

int add_symbol_to_env (env *env, object *symbol, object *value);
void env_bind_function (env *env, object *function, object *value);
object *env_find_function_value (env *env, object *function);
object *env_find_symbol_value (env *env, object *symbol);
#endif
