#include "survival_kaplan_meier.h"

int main(int argc, char const *argv[])
{
	struct curve KM_curve;

	double _time[14] = {1.0, 2.1, 3.2, 4.5, 5.5, 6.2, 7.4, 8.5, 9.1, 10.4, 11.3, 12.5, 13.2, 14.7};
	int _censored[14] = {1, 0 , 0, 0, 1, 1, 1, 0 , 1, 1,0,1,0,1};

	kaplan_meier_3p_extrapolation(_time, _censored, 14, &KM_curve);
	free(KM_curve.point_array);

	return 0;
}