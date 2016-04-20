#ifndef _SSTAT_TYPE_DEF_H_
#define _SSTAT_TYPE_DEF_H_

typedef struct point{
	double x;
	double y;
} point;

typedef struct curve{
	struct point* point_arr;
	int size;
} curve;

#endif