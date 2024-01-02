#include <stdio.h>
#include <stdlib.h>

#include "util.h"

int gcd(int a, int b)
{
    if (b == 0)
        return a;
    else
        return gcd(b, a % b);
}