#pragma once

#include <samples.h>

#ifdef DEBUG
#define LOG_WITH_THREADID(text, ...) printf("[thread %d]:" text, thread_id, ##__VA_ARGS__)
#else
#define LOG_WITH_THREADID(text, ...)
#endif
