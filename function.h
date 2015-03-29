#ifndef FUNCTION_H
#define FUNCTION_H

#include "types.h"


void define_function (char *name, object *(*function) (object *arguments));
object *call_function (object *symbol, object *arguments);
#endif
