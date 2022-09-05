#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include "shthreads/shthreads.h"

#include <stdlib.h>


ShThreadsHandle shAllocateThreads(uint32_t thread_count) {
    shThreadsError(thread_count == 0, "invalid thread count", return (ShThreadsHandle){ 0 });
    
    return (ShThreadsHandle) {
        calloc(thread_count, sizeof(ShThread)),
        thread_count,
#ifdef _WIN32
        calloc(thread_count, sizeof(HANDLE)),
#endif//_WIN32
    };
}



ShThreadsStatus shCreateThread(uint32_t idx, void* p_func, uint32_t stack_size, ShThreadsHandle* p_handle) {
    shThreadsError(p_func   == NULL,              "invalid function pointer", return SH_INVALID_FUNCTION_MEMORY);
    shThreadsError(p_handle == NULL,              "invalid handle memory",    return SH_INVALID_HANDLE_MEMORY);
    shThreadsError(idx >= p_handle->thread_count, "invalid thread count",     return SH_INVALID_THREAD_IDX);
    
    ShThread* p_thread = &p_handle->p_threads[idx];
    shThreadsError(p_thread == NULL,             "invalid p_thread", return SH_INVALID_THREAD_HANDLE_MEMORY);

    p_thread->p_func = p_func;
    p_thread->stack_size = stack_size;
    
    return SH_THREADS_SUCCESS;
}


ShThreadsStatus shLaunchThreads(uint32_t first_thread, uint32_t thread_count, ShThreadParameters* p_parameters, ShThreadsHandle* p_handle) {
    shThreadsError(p_handle == NULL, "invalid handle memory", return SH_INVALID_HANDLE_MEMORY);

#ifdef _WIN32
    for (uint32_t thread_idx = first_thread; thread_idx < (first_thread + thread_count); thread_idx++) {
        ShThread* p_thread = &p_handle->p_threads[thread_idx];
        HANDLE* p_thread_handle = &p_handle->p_handles[thread_idx];
        (*p_thread_handle) = CreateThread(
            NULL,
            p_thread->stack_size,
            (LPTHREAD_START_ROUTINE)p_thread->p_func,
            p_parameters[thread_idx],
            0,
            (LPDWORD)&p_thread->thread_id
        );
        shThreadsError((*p_thread_handle) == NULL, "invalid thread handle memory", return SH_INVALID_THREAD_HANDLE_MEMORY);
    }
#else
#endif//_WIN32

    return SH_THREADS_SUCCESS;
}

ShThreadsStatus shWaitForThreads(uint32_t first_thread, uint32_t thread_count, uint64_t timeout, ShThreadsHandle* p_handle) {
    shThreadsError(p_handle == NULL, "invalid thread memory", return SH_INVALID_HANDLE_MEMORY);
#ifdef _WIN32
    DWORD status = WaitForMultipleObjects(
        thread_count,
        &p_handle->p_handles[first_thread],
        1,
        (DWORD)timeout
    );
    shThreadsError(status < WAIT_OBJECT_0 || status > (DWORD)((uint32_t)WAIT_OBJECT_0 + thread_count - 1), "failed waiting for threads", return SH_THREADS_FAILURE);
#else
#endif//_WIN32
    
    return SH_THREADS_SUCCESS;
}

ShThreadsStatus shThreadsRelease(ShThreadsHandle* p_handle) {
    shThreadsError(p_handle            == NULL, "invalid thread memory",         return SH_INVALID_HANDLE_MEMORY);
    shThreadsError(p_handle->p_threads == NULL, "invalid threads memory",        return SH_INVALID_THREAD_MEMORY);
    shThreadsError(p_handle->p_handles == NULL, "invalid thread handles memory", return SH_INVALID_THREAD_HANDLE_MEMORY);

#ifdef _WIN32
    for (uint32_t thread_idx = 0; thread_idx < p_handle->thread_count; thread_idx++) {
        CloseHandle(p_handle->p_handles[thread_idx]);
    }
    for (uint32_t mutex_idx = 0; mutex_idx < p_handle->mutex_count; mutex_idx++) {
        CloseHandle(p_handle->p_mutexes[mutex_idx]);
    }
#else
#endif//_WIN32

    free(p_handle->p_handles);
    free(p_handle->p_threads);
    free(p_handle->p_mutexes);

    p_handle->thread_count = 0;
    p_handle->mutex_count  = 0;

    return SH_THREADS_SUCCESS;
}


ShMutex* shCreateMutexes(uint32_t mutex_count, ShThreadsHandle* p_handle) {
    shThreadsError(p_handle == NULL, "invalid threads handle", return NULL);

    p_handle->mutex_count = mutex_count;
    p_handle->p_mutexes   = (ShMutex*)calloc(mutex_count, sizeof(ShMutex));
    shThreadsError(p_handle->p_mutexes == NULL, "invalid mutexes memory", return NULL);

#ifdef _WIN32
    for (uint32_t mutex_idx = 0; mutex_idx < mutex_count; mutex_idx++) {
        p_handle->p_mutexes[mutex_idx] = CreateMutex(
            NULL,
            FALSE,
            NULL
        );
    }
    
#else
#endif//_WIN32

    return p_handle->p_mutexes;
}

ShThreadsStatus shWaitForMutexes(uint32_t first_mutex, uint32_t mutex_count, uint32_t timeout, ShMutex* p_mutexes) {
    shThreadsError(p_mutexes == NULL, "invalid mutexes memory", return SH_INVALID_MUTEX_MEMORY);

#ifdef _WIN32
    DWORD status = WaitForMultipleObjects(
        mutex_count,
        p_mutexes,
        TRUE,
        timeout
    );
    shThreadsError(status < WAIT_OBJECT_0 || status > (DWORD)((uint32_t)WAIT_OBJECT_0 + mutex_count - 1), "failed waiting for mutexes", return SH_THREADS_FAILURE);
#else
#endif//_WIN32

    return SH_THREADS_SUCCESS;
}

ShThreadsStatus shUnlockMutexes(uint32_t first_mutex, uint32_t mutex_count, ShMutex* p_mutexes) {
    shThreadsError(p_mutexes == NULL, "invalid mutexes memory", return SH_INVALID_MUTEX_MEMORY);

#ifdef _WIN32
    uint32_t unlock_status = 0;
    for (uint32_t mutex_idx = first_mutex; mutex_idx < (first_mutex + mutex_count); mutex_idx++) {
        unlock_status += (uint32_t)ReleaseMutex(p_mutexes[mutex_idx]);
    }
#else
#endif//_WIN32

    return unlock_status == mutex_count ? SH_THREADS_SUCCESS : SH_MUTEX_UNLOCK_FAILURE;
}



#ifdef __cplusplus
}
#endif//__cplusplus