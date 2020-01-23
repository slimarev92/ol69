#ifndef CALCULATOR_H
#define CALCULATOR_H

enum status {OK, E_MATH, E_SYNTAX, E_MALLOC};

int Calculate(const char *str, double *result);

#endif
