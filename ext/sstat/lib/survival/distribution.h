#ifndef _SSTAT_DISTRIBUTION_UTILITY_H_
#define _SSTAT_DISTRIBUTION_UTILITY_H_
#include "type_def.h"
static double
get_del (double x, double rational)
{
double xsq = 0.0;
double del = 0.0;
double result = 0.0;
 
xsq = floor (x * GAUSS_SCALE) / GAUSS_SCALE;
del = (x - xsq) * (x + xsq);
del *= 0.5;
 
result = exp (-0.5 * xsq * xsq) * exp (-1.0 * del) * rational;
 
return result;
}
 
/*
 * Normal cdf for fabs(x) < 0.66291
 */
static double
gauss_small (const double x)
{
unsigned int i;
double result = 0.0;
double xsq;
double xnum;
double xden;
 
const double a[5] = {
2.2352520354606839287,
161.02823106855587881,
1067.6894854603709582,
18154.981253343561249,
0.065682337918207449113
};
const double b[4] = {
47.20258190468824187,
976.09855173777669322,
10260.932208618978205,
45507.789335026729956
};
 
xsq = x * x;
xnum = a[4] * xsq;
xden = xsq;
 
for (i = 0; i < 3; i++)
{
xnum = (xnum + a[i]) * xsq;
xden = (xden + b[i]) * xsq;
}
 
result = x * (xnum + a[3]) / (xden + b[3]);
 
return result;
}
 
/*
 * Normal cdf for 0.66291 < fabs(x) < sqrt(32).
 */
static double
gauss_medium (const double x)
{
unsigned int i;
double temp = 0.0;
double result = 0.0;
double xnum;
double xden;
double absx;
 
const double c[9] = {
0.39894151208813466764,
8.8831497943883759412,
93.506656132177855979,
597.27027639480026226,
2494.5375852903726711,
6848.1904505362823326,
11602.651437647350124,
9842.7148383839780218,
1.0765576773720192317e-8
};
const double d[8] = {
22.266688044328115691,
235.38790178262499861,
1519.377599407554805,
6485.558298266760755,
18615.571640885098091,
34900.952721145977266,
38912.003286093271411,
19685.429676859990727
};
 
absx = fabs (x);
 
xnum = c[8] * absx;
xden = absx;
 
for (i = 0; i < 7; i++)
{
xnum = (xnum + c[i]) * absx;
xden = (xden + d[i]) * absx;
}
 
temp = (xnum + c[7]) / (xden + d[7]);
 
result = get_del (x, temp);
 
return result;
}
 
/*
 * Normal cdf for
 * {sqrt(32) < x < GAUSS_XUPPER} union { GAUSS_XLOWER < x < -sqrt(32) }.
 */
static double
gauss_large (const double x)
{
int i;
double result;
double xsq;
double temp;
double xnum;
double xden;
double absx;
 
const double p[6] = {
0.21589853405795699,
0.1274011611602473639,
0.022235277870649807,
0.001421619193227893466,
2.9112874951168792e-5,
0.02307344176494017303
};
const double q[5] = {
1.28426009614491121,
0.468238212480865118,
0.0659881378689285515,
0.00378239633202758244,
7.29751555083966205e-5
};
 
absx = fabs (x);
xsq = 1.0 / (x * x);
xnum = p[5] * xsq;
xden = xsq;
 
for (i = 0; i < 4; i++)
{
xnum = (xnum + p[i]) * xsq;
xden = (xden + q[i]) * xsq;
}
 
temp = xsq * (xnum + p[4]) / (xden + q[4]);
temp = (M_1_SQRT2PI - temp) / absx;
 
result = get_del (x, temp);
 
return result;
}
 
double
gsl_cdf_ugaussian_P (const double x)
{
double result;
double absx = fabs (x);
 
if (absx < GAUSS_EPSILON)
{
result = 0.5;
return result;
}
else if (absx < 0.66291)
{
result = 0.5 + gauss_small (x);
return result;
}
else if (absx < SQRT32)
{
result = gauss_medium (x);
 
if (x > 0.0)
{
result = 1.0 - result;
}
 
return result;
}
else if (x > GAUSS_XUPPER)
{
result = 1.0;
return result;
}
else if (x < GAUSS_XLOWER)
{
result = 0.0;
return result;
}
else
{
result = gauss_large (x);
 
if (x > 0.0)
{
result = 1.0 - result;
}
}
 
return result;
}

double
cdf_ugaussian_Q (const double x)
{
double result;
double absx = fabs (x);
 
if (absx < GAUSS_EPSILON)
{
result = 0.5;
return result;
}
else if (absx < 0.66291)
{
result = gauss_small (x);
 
if (x < 0.0)
{
result = fabs (result) + 0.5;
}
else
{
result = 0.5 - result;
}
 
return result;
}
else if (absx < SQRT32)
{
result = gauss_medium (x);
 
if (x < 0.0)
{
result = 1.0 - result;
}
 
return result;
}
else if (x > -(GAUSS_XLOWER))
{
result = 0.0;
return result;
}
else if (x < -(GAUSS_XUPPER))
{
result = 1.0;
return result;
}
else
{
result = gauss_large (x);
 
if (x < 0.0)
{
result = 1.0 - result;
}
 
}
 
return result;
}
#endif