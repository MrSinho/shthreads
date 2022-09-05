#ifndef SH_THREADS_H
#define SH_THREADS_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#ifdef _WIN32
#include <Windows.h>
#endif//_WIN32

#include <stdio.h>
#include <stdint.h>
#include <assert.h>

typedef void* ShThreadParameters;
#ifdef _WIN32
typedef unsigned long ShThreadFunc(void*);
typedef HANDLE ShMutex;
#endif//_WIN32

#define SH_BUILD_THREAD_ARGS(struct_name, ...)\
    typedef struct {\
        __VA_ARGS__;\
    } struct_name

#define shThreadsError(condition, error_msg, failure_expression)\
    if ((int)(condition)) {\
        printf("shthreads error: %s \n", (error_msg));\
        failure_expression;\
    }



typedef enum ShThreadsStatus {
    SH_THREADS_SUCCESS              = 1,
    SH_THREADS_FAILURE              = 0,
    SH_INVALID_HANDLE_MEMORY        = -1,
    SH_INVALID_THREAD_MEMORY        = -2,
    SH_INVALID_THREAD_HANDLE_MEMORY = -3,
    SH_INVALID_FUNCTION_MEMORY      = -4,
    SH_INVALID_THREAD_IDX           = -5,
    SH_INVALID_MUTEX_MEMORY         = -6,
} ShThreadsStatus;



typedef struct ShThread {
    ShThreadFunc* p_func;
    uint32_t stack_size;
    uint64_t thread_id;
} ShThread;



typedef struct ShThreadsHandle {
    ShThread* p_threads;
    uint32_t  thread_count;

#ifdef _WIN32
    HANDLE* p_handles;
#else 
#endif//_WIN32

    uint32_t mutex_count;
    ShMutex* p_mutexes;

} ShThreadsHandle;

#ifdef _WIN32
#define shGetCurrentThreadId()\
    (uint64_t)GetCurrentThreadId()
#else
#endif//_WIN32



extern ShThreadsHandle shAllocateThreads(uint32_t thread_count);



extern ShThreadsStatus shCreateThread(uint32_t idx, void* p_func, uint32_t stack_size, ShThreadsHandle* p_handle);

extern ShThreadsStatus shLaunchThreads(uint32_t first_thread, uint32_t thread_count, ShThreadParameters* p_parameters, ShThreadsHandle* p_handle);

extern ShThreadsStatus shWaitForThreads(uint32_t first_thread, uint32_t thread_count, uint64_t timeout, ShThreadsHandle* p_handle);

extern ShThreadsStatus shThreadsRelease(ShThreadsHandle* p_handle);



extern ShMutex* shCreateMutexes(uint32_t mutex_count, ShThreadsHandle* p_handle);



extern ShThreadsStatus shWaitForMutexes(uint32_t first_mutex, uint32_t mutex_count, uint32_t timeout, ShMutex* p_mutexes);

extern ShThreadsStatus shUnlockMutexes(uint32_t first_mutex, uint32_t mutex_count, ShMutex* p_mutexes);



#ifdef __cplusplus
}
#endif//_cplusplus

#endif//SH_THREADS_H