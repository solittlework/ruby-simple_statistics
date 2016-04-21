#ifndef _SSTAT_TYPE_DEF_H_
#define _SSTAT_TYPE_DEF_H_

#define EPS 1e-10
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

#endif