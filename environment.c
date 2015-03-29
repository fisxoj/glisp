#include <stdlib.h>
#include <stdio.h>

#include "environment.h"
#include "primitives.h"
#include "util.h"

int const STARTING_EXTENT = 5;


env *env_new(env *parent) {
  env *new_env = malloc(sizeof(env));

  new_env->parent = parent;
  new_env->symbols = malloc(sizeof(object*) * STARTING_EXTENT);
  new_env->symbol_values = malloc(sizeof(object*) * STARTING_EXTENT);
  new_env->symbol_count = 0;
  new_env->symbol_extent = STARTING_EXTENT;

  new_env->functions = malloc(sizeof(object*) * STARTING_EXTENT);
  new_env->function_values = malloc(sizeof(object*) * STARTING_EXTENT);
  new_env->function_count = 0;
  new_env->function_extent = STARTING_EXTENT;

  return new_env;
}

void env_extend_symbols (env *env) {
  int new_extent = env->symbol_extent + STARTING_EXTENT;
  env->symbols = realloc(env->symbols, new_extent * sizeof(object *));
  env->symbol_values = realloc(env->symbol_values, new_extent * sizeof(object *));

  env->symbol_extent = new_extent;
};

void env_extend_functions (env *env) {
  int new_extent = env->function_extent + STARTING_EXTENT;
  env->functions = realloc(env->functions, new_extent * sizeof(object *));
  env->function_values = realloc(env->function_values, new_extent * sizeof(object *));

  env->function_extent = new_extent;
};


int add_symbol_to_env (env *env, object *symbol, object *value) {
  if (env->symbol_count == env->symbol_extent) env_extend_symbols (env);

  int count = env->symbol_count;

  env->symbols[count] = symbol;
  env->symbol_values[count] = value;
  env->symbol_count++;

  return 0;
}

void env_bind_symbol (env *env, object *symbol, object *value) {

  for (int i = env->symbol_count - 1; i >= 0; i--) {
    if (is_truthy (equal_p(symbol, env->symbols[i]))) {
      env->symbol_values[i] = value;
      return;
    }
  }

  // Value wasn't in the environment, add it

  add_symbol_to_env (env, symbol, value);

  return;
}

object *env_find_symbol_value (env *env, object *symbol) {

  for (int i = env->symbol_count - 1; i >= 0; i--) {
    if (is_truthy (equal_p(symbol, env->symbols[i]))) {
      return env->symbol_values[i];
    }
  }

  if (env->parent == NULL) return nil;

  return env_find_symbol_value (env->parent, symbol);
}

int add_function_to_env (env *env, object *function, object *value) {

  if (env->function_count == env->function_extent) env_extend_functions (env);

  int count = env->function_count;

  env->functions[count] = function;
  env->function_values[count] = value;
  env->function_count++;

  return 0;
}

void env_bind_function (env *env, object *function, object *value) {
  /* printf("%s: Count: %d, symbol: %s\n", */
  /* 	 __func__, */
  /* 	 env->function_count, */
  /* 	 function->data.symbol); */
  for (int i = env->function_count - 1; i >= 0; i--) {
    /* printf("comparing: %s with %s", */
    /* 	   function->data.symbol, */
    /* 	   env->functions[i]->data.symbol); */
    if (is_truthy (equal_p(function, env->functions[i]))) {
      env->function_values[i] = value;
      return;
    }
  }

  // Value wasn't in the environment, add it

  add_function_to_env (env, function, value);

  return;
}

object *env_find_function_value (env *env, object *function) {
  /* printf("%s: Count: %d, symbol: %s\n", */
  /* 	 __func__, */
  /* 	 env->function_count, */
  /* 	 function->data.symbol); */
  for (int i = env->function_count - 1; i >= 0; i--) {
    if (is_truthy (equal_p(function, env->functions[i]))) {
      return env->function_values[i];
    }
  }

  if (env->parent == NULL) return nil;

  return env_find_function_value (env->parent, function);
}
