#ifndef _HISTOGRAM_ERROR_H
#define _HISTOGRAM_ERROR_H
#include <string.h>
#include <stdlib.h>

typedef enum {
    HIST_SUCCESS = 0,
    M_ALLOC_ERR = 1,
    OUT_OF_ARANGE = 2
} HIST_ERR;

char* alloc_str_err(const char* err_msg, size_t n) {
    char * err_buf = malloc(n * sizeof(char));
    if (err_buf == 0) {
        return NULL;
    } else {
        strcpy(err_buf, err_msg);
        return err_buf;
    }
}

void get_hist_err(char * err_buf, size_t buf_n, HIST_ERR hist_err) {
    if (hist_err == M_ALLOC_ERR) {
        err_buf = alloc_str_err("Histogram: Bad Memory allocation", buf_n);
        return;
    }

    if (hist_err == OUT_OF_ARANGE) {
        err_buf = alloc_str_err("Histogram: size out of range", buf_n);
        return;
    }

    return;
}
#endif
