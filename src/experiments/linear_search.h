#ifndef LINEAR_SEARCH_H
#define LINEAR_SEARCH_H

#include "defines.h"
using namespace tsunami;
void linsearch(const VectorDouble& oldX, double oldFunc, const VectorDouble& gradient,
               VectorDouble& p, VectorDouble& newX, double& newFunc,
               double stepMax, bool& check, FUNCTION func);

#endif // LINEAR_SEARCH_H
