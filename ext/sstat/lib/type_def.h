#ifndef _SSTAT_TYPE_DEF_H_
#define _SSTAT_TYPE_DEF_H_

//EPSILON here is set based on ruby float. we need to chagne it for other project
#define EPSILON 2.2204460492503131e-16

/* The following constants defined based on http://www.atmel.com/webdoc/ */
#define M_2_SQRTPI 1.12837916709551257390
#define M_SQRT1_2 0.70710678118654752440
#define M_1_SQRT2PI (M_2_SQRTPI * M_SQRT1_2 / 2.0)
#define M_SQRT2 1.41421356237309504880
#define SQRT32 (4.0 * M_SQRT2)

/* Define constants for gaussian distribution */
#define GAUSS_XUPPER (8.572)
#define GAUSS_XLOWER (-37)
#define GAUSS_SCALE (16.0)
#define GAUSS_EPSILON 1e-12
#include <stdio.h>

/* Define point structure */
typedef struct point
{
	double x;
	double y;
} point;

/* Define curve structure */
typedef struct curve {
	struct point* point_array;
	int size;
} curve;

/* Define array structure */
typedef struct array {
	double* D_ptr; /* Short for data pointer */
	int size;
} array;

void print_points(struct point* points, int size)
{
	int i;
	for(i = 0; i < size; i++)
	{
		printf("point %i, x : %f, y : %f \n", i, points[i].x, points[i].y);
	}
}


point* alloc_points(int size)
{
	return (struct point*) malloc(size * sizeof(struct point));
}

void print_curve(struct curve* curve)
{
	int i;
	for(i = 0; i < curve->size; i++)
	{
		printf("x: %f  y: %f\n", curve->point_array[i].x, curve->point_array[i].y);
	}

}

#endif