#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "types.h"

env* env_new (env *parent);

void env_bind_function (env *env, object *function, object *value);
void env_bind_symbol (env *env, object *symbol, object *value);
object *env_find_function_value (env *env, object *function);
object *env_find_symbol_value (env *env, object *symbol);
#endif
