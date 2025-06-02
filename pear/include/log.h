#ifndef LOG_H_
#define LOG_H_

#include <stdio.h>
#include <stdlib.h>

#ifdef PEAR_DEBUG
    #define PEAR_INFO(msg, ...) printf("[info] %s:%d %s(): "msg"\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)
    #define PEAR_WARN(msg, ...) printf("[warn] %s:%d %s(): "msg"\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)
    #define PEAR_ERROR(msg, ...) printf("[error] %s:%d %s(): "msg"\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)
    #define PEAR_ASSERT(x, msg, ...) do { if (!(x)) { PEAR_ERROR(msg, ##__VA_ARGS__); exit(EXIT_FAILURE); } } while (0)
#else
    #define PEAR_INFO(msg, ...)
    #define PEAR_WARN(msg, ...)
    #define PEAR_ERROR(msg, ...)
    #define PEAR_ASSERT(x, msg, ...)
#endif

#endif
