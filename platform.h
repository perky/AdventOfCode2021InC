/* date = December 8th 2021 11:04 am */

#ifndef PLATFORM_H
#define PLATFORM_H
#define __STDC_FORMAT_MACROS
#include <stdint.h>
#include <limits.h>

#define U64FMT  "%llu"
#define I64FMT  "%lld"
#define MAX_U64 ULLONG_MAX;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

#define THOUSAND 1000
#define MILLION 1000000
#define KILLION THOUSAND*MILLION
#define BILLION MILLION*MILLION
#define KABILLION THOUSAND*BILLION
#define TRILLION MILLION*MILLION*MILLION
#define KATRILLION THOUSAND*TRILLION
#define QUADRILLION MILLION*MILLION*MILLION*MILLION

extern inline i32 max(i32 a, i32 b);
extern inline i32 min(i32 a, i32 b);

#if !defined(WIN32)
inline i32 max(i32 a, i32 b)
{
    return a > b ? a : b;
}
inline i32 min(i32 a, i32 b)
{
    return a < b ? a : b;
}
#endif

i64 abs_64(i64 v)
{
    return (v < 0) ? (v*-1) : (v);
}

extern inline double 
time_difference(double  t1, double  t2, double k)
{
    double result = (t2 - t1) * k; // sec to ms
    return result;
}

#if defined(WIN32)
#include <windows.h>
#define TIME LARGE_INTEGER
#define GET_ACCURATE_TIME(t) QueryPerformanceCounter(t);
#define GET_ACCURATE_TIME_DIFFERENCE(t1, t2) \
({ \
TIME freq; \
QueryPerformanceFrequency(&freq); \
time_difference(t1.QuadPart, t2.QuadPart, 1000.0 / freq.QuadPart) \
})

#else

#include <sys/time.h>  // for gettimeofday()
#define TIME struct timeval

inline void get_accurate_time_unix(TIME *t)
{
    gettimeofday(t, NULL);
}

#define GET_ACCURATE_TIME(t) gettimeofday(t, NULL)
#define GET_ACCURATE_TIME_DIFFERENCE(t1, t2) \
(time_difference(t1.tv_sec, t2.tv_sec, 1000.0) + (t2.tv_usec - t1.tv_usec) / 1000.0)
#endif


#endif //PLATFORM_H
