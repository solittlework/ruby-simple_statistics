#ifndef _SSTAT_TYPE_DEF_H_
#define _SSTAT_TYPE_DEF_H_

//EPSILON here is set based on ruby float. we need to chagne it for other project
#define EPSILON 2.2204460492503131e-16

//The following constants are from ATMEL http://www.atmel.com/webdoc/
#define M_2_SQRTPI 1.12837916709551257390
#define M_SQRT1_2 0.70710678118654752440
#define M_1_SQRT2PI (M_2_SQRTPI * M_SQRT1_2 / 2.0)
#define M_SQRT2 1.41421356237309504880
#define SQRT32 (4.0 * M_SQRT2)

//GSL constants for gaussian distribution
#define GAUSS_XUPPER (8.572)
#define GAUSS_XLOWER (-37)
#define GAUSS_SCALE (16.0)
#define GAUSS_EPSILON 1e-12

typedef struct point{
	double x;
	double y;
} point;

typedef struct curve{
	struct point* point_array;
	int size;
} curve;

typedef struct array{
	double* D_ptr;
	int size;
} array;

point* alloc_points(size_t size)
{
	return (struct point*) malloc(size * sizeof(struct point));
}

#endif