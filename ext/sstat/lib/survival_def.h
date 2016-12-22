#ifndef _SURVIVAL_STAT_DEF_H_
#define _SURVIVAL_STAT_DEF_H_
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "type_def.h"

/* Debug macro from http://c.learncodethehardway.org/book/ex20.html */

#include <stdio.h>
#include <errno.h>
#include <string.h>

#ifdef NDEBUG
#define debug(M, ...)
#else
#define debug(M, ...) fprintf(stderr, "DEBUG %s:%d: " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#endif

#define clean_errno() (errno == 0 ? "None" : strerror(errno))

#define log_err(M, ...) fprintf(stderr, "[ERROR] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)

#define log_warn(M, ...) fprintf(stderr, "[WARN] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)

#define log_info(M, ...) fprintf(stderr, "[INFO] (%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)

#define check(A, M, ...) if(!(A)) { log_err(M, ##__VA_ARGS__); errno=0; goto error; }
#define check_1(A, M, ...) if(!(A)) { log_err(M, ##__VA_ARGS__); errno=0; goto error_1; }

#define sentinel(M, ...)  { log_err(M, ##__VA_ARGS__); errno=0; goto error; }

#define check_mem(A) check((A), "Out of memory.")
#define check_mem_1(A) check_1((A), "Out of memory.")

#define check_debug(A, M, ...) if(!(A)) { debug(M, ##__VA_ARGS__); errno=0; goto error; }

enum ERRORS {
	OUTOF_MEMORY_ERROR = 1,
	NOT_EMPTY_ERROR = 2,
};

/**
 * @brief stores the number of censored and uncensored samples for each time range
 */
typedef struct CENS_UC_NUM
{
	int * uncensored; /* Number of ncensored samples */
	int * censored; /* Number of censored samples */
	double* time; /* Survival time series */
	int size;
} CENS_UC_NUM;

void free_CENS_UC_NUM(struct CENS_UC_NUM** instance)
{
	if ( (*instance) != NULL)
	{
		if((*instance)->uncensored != NULL)
			free((*instance)->uncensored);

		if((*instance)->censored != NULL)
			free((*instance)->censored);

		if((*instance)->time != NULL)
			free((*instance)->time);

		free((*instance));
	}
}

void print_CENS_UC_NUM(struct CENS_UC_NUM *cens_uncens_instance)
{
	int i;
	for( i = 0; i < cens_uncens_instance->size; i++ )
	{
		printf("Time : %f -- Uncensored : %i -- Censored : %i \n", 
			cens_uncens_instance-> time[i],
			cens_uncens_instance-> uncensored[i],
			cens_uncens_instance-> censored[i]);
	}
}

int alloc_CENS_UC_NUM(struct CENS_UC_NUM** cens_uncens_instance, int size)
{

	(*cens_uncens_instance) = malloc(sizeof(struct CENS_UC_NUM));
	check_mem_1(cens_uncens_instance);

	(*cens_uncens_instance)->size = size;

	(*cens_uncens_instance)->uncensored = malloc(size * sizeof(int));
	check_mem_1(cens_uncens_instance);

	(*cens_uncens_instance)->censored = malloc(size * sizeof(int));
	check_mem_1(cens_uncens_instance);

	(*cens_uncens_instance)->time = malloc(size * sizeof(double));
	check_mem_1(cens_uncens_instance);

	return 0;
	
error_1:
	if((*cens_uncens_instance) == NULL)
		return OUTOF_MEMORY_ERROR;
	else
		free_CENS_UC_NUM(cens_uncens_instance);
		free((*cens_uncens_instance));
}


#endif