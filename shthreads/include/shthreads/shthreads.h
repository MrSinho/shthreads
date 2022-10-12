#ifndef SH_THREADS_H
#define SH_THREADS_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#include <pthread.h>
#endif//_WIN32

#include <stdio.h>
#include <stdint.h>
#include <assert.h>

typedef void* ShThreadParameters;
#ifdef _WIN32
typedef unsigned long ShThreadFunc(void*);
typedef HANDLE ShMutex;
#else
typedef void* ShThreadFunc(void*);
//to fix
typedef pthread_mutex_t ShMutex;
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


typedef enum ShThreadState {
    SH_THREAD_RUNNING       =  2,
    SH_THREAD_RETURNED      =  1,
    SH_THREAD_INVALID_STATE = -1,
} ShThreadState;


typedef enum ShThreadsStatus {
    SH_THREADS_SUCCESS              =  1,
    SH_THREADS_FAILURE              =  0,
    SH_INVALID_HANDLE_MEMORY        = -1,
    SH_INVALID_THREAD_MEMORY        = -2,
    SH_INVALID_THREAD_HANDLE_MEMORY = -3,
    SH_INVALID_FUNCTION_MEMORY      = -4,
    SH_INVALID_THREAD_IDX           = -5,
    SH_INVALID_MUTEX_MEMORY         = -6,
    SH_MUTEX_UNLOCK_FAILURE         = -7,
    SH_INVALID_EXIT_CODE_MEMORY     = -8,
    SH_INVALID_THREAD_STATE_MEMORY  = -9,
    SH_THREAD_STATE_FAILURE         = -10,
} ShThreadsStatus;



typedef struct ShThread {
    ShThreadFunc* p_func;
    uint32_t      stack_size;
    uint64_t      id;
} ShThread;



typedef struct ShThreadsHandle {
    ShThread*   p_threads;
    uint32_t    thread_count;

#ifdef _WIN32
    HANDLE*     p_handles;
#else 
	pthread_t*  p_handles;
#endif//_WIN32

    uint32_t   mutex_count;
    ShMutex*   p_mutexes;

} ShThreadsHandle;

#ifdef _WIN32
#define shGetCurrentThreadId()\
    (uint64_t)GetCurrentThreadId()
#else
//TO FIX
#define shGetCurrentThreadId()\
	(uint64_t)0
#endif//_WIN32



extern ShThreadsHandle shAllocateThreads(uint32_t thread_count);



extern ShThreadsStatus shCreateThread(uint32_t idx, void* p_func, uint32_t stack_size, ShThreadsHandle* p_handle);

extern ShThreadsStatus shLaunchThreads(uint32_t first_thread, uint32_t thread_count, ShThreadParameters* p_parameters, ShThreadsHandle* p_handle);

extern ShThreadsStatus shGetThreadState(uint32_t thread_idx, ShThreadState* p_state, ShThreadsHandle* p_handle);

extern ShThreadsStatus shExitCurrentThread(uint32_t exit_code);

extern ShThreadsStatus shWaitForThreads(uint32_t first_thread, uint32_t thread_count, uint64_t timeout, uint64_t* p_exit_codes, ShThreadsHandle* p_handle);

extern ShThreadsStatus shThreadsRelease(ShThreadsHandle* p_handle);



extern ShMutex* shCreateMutexes(uint32_t mutex_count, ShThreadsHandle* p_handle);



extern ShThreadsStatus shWaitForMutexes(uint32_t first_mutex, uint32_t mutex_count, uint64_t timeout, ShMutex* p_mutexes);

extern ShThreadsStatus shUnlockMutexes(uint32_t first_mutex, uint32_t mutex_count, ShMutex* p_mutexes);



#ifdef _WIN32
#define shThreadsSleep(i_milliseconds) Sleep((DWORD)(i_milliseconds))
#else
#define shThreadsSleep(i_milliseconds) usleep((i_milliseconds) * 1000)
#endif//_WIN32


#ifdef __cplusplus
}
#endif//_cplusplus

#endif//SH_THREADS_H
