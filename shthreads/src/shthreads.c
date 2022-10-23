#ifdef __cplusplus
extern "C" {
#endif//__cplusplus



#include "shthreads/shthreads.h"



#include <stdlib.h>



#ifdef _MSC_VER
#pragma warning (disable: 6258)
#endif//_MSC_VER



ShThreadsStatus shAllocateThreads(uint32_t thread_count, ShThreadPool* p_pool) {
    shThreadsError(thread_count == 0, "invalid thread count", return SH_INVALID_THREADS_RANGE);
    shThreadsError(p_pool == NULL, "invalid thread pool memory", return SH_INVALID_THREAD_POOL_MEMORY);
    
    ShThread* p_threads = (ShThread*)calloc(thread_count, sizeof(ShThread));

#ifdef _WIN32
    HANDLE* p_handles = calloc(thread_count, sizeof(HANDLE));

    shThreadsError(p_handles == NULL, "invalid thread handles memory", return SH_INVALID_THREAD_HANDLE_MEMORY);
#else
    pthread_t*      p_handles        = (pthread_t*)calloc(thread_count, sizeof(pthread_t));
    pthread_attr_t* p_handle_attribs = (pthread_attr_t*)calloc(thread_count, sizeof(pthread_attr_t));

    shThreadsError(p_handles         == NULL, "invalid thread handles memory",           return SH_INVALID_THREAD_HANDLE_MEMORY);
    shThreadsError(p_handle_attribs  == NULL, "invalid thread handle attributes memory", return SH_INVALID_THREAD_HANDLE_MEMORY);
#endif//_WIN32


    ShThreadPool pool = {
        p_threads,
        thread_count,
        p_handles,
#ifndef _WIN32
        p_handle_attribs
#endif//_WIN32
    };

    (*p_pool) = pool;

    return SH_THREADS_SUCCESS;
}



ShThreadsStatus shCreateThread(uint32_t idx, void* p_func, uint32_t stack_size, ShThreadPool* p_pool) {
    shThreadsError(p_func   == NULL,            "invalid function pointer",   return SH_INVALID_FUNCTION_MEMORY);
    shThreadsError(p_pool   == NULL,            "invalid thread pool memory", return SH_INVALID_THREAD_POOL_MEMORY);
    shThreadsError(idx >= p_pool->thread_count, "invalid thread count",       return SH_INVALID_THREAD_IDX);
    
    ShThread* p_thread = &p_pool->p_threads[idx];
    shThreadsError(p_thread == NULL,              "invalid thread memory", return SH_INVALID_THREAD_HANDLE_MEMORY);

    p_thread->p_func = p_func;
    p_thread->stack_size = stack_size;
    
    return SH_THREADS_SUCCESS;
}


ShThreadsStatus shLaunchThreads(uint32_t first_thread, uint32_t thread_count, ShThreadParameters* p_parameters, ShThreadPool* p_pool) {
    shThreadsError(p_pool == NULL, "invalid thread pool memory", return SH_INVALID_THREAD_POOL_MEMORY);

#ifdef _WIN32
    for (uint32_t thread_idx = first_thread; thread_idx < (first_thread + thread_count); thread_idx++) {
        ShThread* p_thread = &p_pool->p_threads[thread_idx];
        HANDLE* p_thread_handle = &p_pool->p_handles[thread_idx];
        (*p_thread_handle) = CreateThread(
            NULL,
            p_thread->stack_size,
            (LPTHREAD_START_ROUTINE)p_thread->p_func,
            p_parameters[thread_idx],
            0,
            (LPDWORD)&p_thread->id
        );
        shThreadsError((*p_thread_handle) == NULL, "invalid thread handle memory", return SH_INVALID_THREAD_HANDLE_MEMORY);
    }
#else
    for (uint32_t thread_idx = first_thread; thread_idx < (first_thread + thread_count); thread_idx++) {
        ShThread* p_thread = &p_pool->p_threads[thread_idx];

        int r = pthread_attr_init(&p_pool->p_handle_attribs[thread_idx]);
        shThreadsError(r != 0, "invalid posix thread attributes memory", return SH_THREAD_INVALID_ATTRIBS_MEMORY);
        if (p_thread->stack_size != 0) {
            pthread_attr_setstacksize(
                &p_pool->p_handle_attribs[thread_idx], 
                p_thread->stack_size
            );
        }

        r = pthread_create(
            &p_pool->p_handles[thread_idx], 
            &p_pool->p_handle_attribs[thread_idx], 
            p_thread->p_func, 
            p_parameters[thread_idx]
        );
        shThreadsError(r != 0, "invalid thread handle memory", return SH_INVALID_THREAD_HANDLE_MEMORY);

        p_pool->p_threads[thread_idx].id = (uint64_t)p_pool->p_handles[thread_idx];
    }
#endif//_WIN32

    return SH_THREADS_SUCCESS;
}

ShThreadsStatus shGetThreadState(uint32_t thread_idx, ShThreadState* p_state, ShThreadPool* p_pool) {
    shThreadsError(p_state == NULL, "invalid thread state memory", return SH_INVALID_THREAD_STATE_MEMORY);
    shThreadsError(thread_idx >= p_pool->thread_count, "invalid thread index", return SH_INVALID_THREAD_IDX);

#ifdef _WIN32
    DWORD status = WaitForSingleObject(p_pool->p_handles[thread_idx], 0);
    switch (status) {
    case WAIT_OBJECT_0:
        (*p_state) = SH_THREAD_RETURNED;
        break;
    case WAIT_TIMEOUT:
        (*p_state) = SH_THREAD_RUNNING;
        break;
    default:
        (*p_state) = SH_THREAD_INVALID_STATE;
        return SH_THREAD_STATE_FAILURE;
    }
#else
//TO FIX
#endif//_WIN32

    return SH_THREADS_SUCCESS;
}

ShThreadsStatus shWaitForThreads(uint32_t first_thread, uint32_t thread_count, uint64_t ms_timeout, uint64_t* p_exit_codes, ShThreadPool* p_pool) {
    shThreadsError(p_pool == NULL,       "invalid thread pool memory", return SH_INVALID_THREAD_POOL_MEMORY);
    shThreadsError(p_exit_codes == NULL, "invalid exit codes memory", return SH_INVALID_EXIT_CODE_MEMORY);

    shThreadsError(
        first_thread >= p_pool->thread_count || 
        thread_count > p_pool->thread_count ||
        first_thread + thread_count > p_pool->thread_count,
        "invalid threads range",
        return SH_INVALID_THREADS_RANGE
    );

#ifdef _WIN32
    DWORD status = WaitForMultipleObjects(
        thread_count,
        &p_pool->p_handles[first_thread],
        1,
        (DWORD)ms_timeout
    );
    shThreadsError(status < WAIT_OBJECT_0 || status > (DWORD)((uint32_t)WAIT_OBJECT_0 + thread_count - 1), "failed waiting for threads", return SH_THREADS_WAIT_FAILURE);
    for (uint32_t thread_idx = first_thread; thread_idx < (first_thread + thread_count); thread_idx++) {
        GetExitCodeThread(p_pool->p_handles[thread_idx], (LPDWORD)&p_exit_codes[thread_idx]);
    }
#else
    for (uint32_t thread_idx = first_thread; thread_idx < first_thread + thread_count; thread_idx++) {
        int r = pthread_join(p_pool->p_handles[thread_idx], (void**)&p_exit_codes[thread_idx]);
        shThreadsError(r != 0, "failed waiting for threads", return SH_THREADS_WAIT_FAILURE);
    }
#endif//_WIN32
    
    return SH_THREADS_SUCCESS;
}

ShThreadsStatus shExitCurrentThread(uint64_t return_value) {
#ifdef _WIN32
    ExitThread((DWORD)return_value);
#else
    pthread_exit((void*)return_value);
#endif//_WIN32

    return SH_THREADS_SUCCESS;
}

ShThreadsStatus shReleaseThreads(ShThreadPool* p_pool) {
    shThreadsError(p_pool            == NULL, "invalid thread pool memory",    return SH_INVALID_THREAD_POOL_MEMORY);
    shThreadsError(p_pool->p_threads == NULL, "invalid threads memory",        return SH_INVALID_THREAD_MEMORY);
#ifdef _WIN32
    shThreadsError(p_pool->p_handles == NULL, "invalid thread handles memory", return SH_INVALID_THREAD_HANDLE_MEMORY);
#else
	shThreadsError(p_pool->p_handles == NULL, "invalid thread handles memory", return SH_INVALID_THREAD_HANDLE_MEMORY);
#endif//_WIN32


#ifdef _WIN32
    for (uint32_t thread_idx = 0; thread_idx < p_pool->thread_count; thread_idx++) {
        CloseHandle(p_pool->p_handles[thread_idx]);
    }
    free(p_pool->p_handles);
    p_pool->p_handles = NULL;
#else
    for (uint32_t thread_idx = 0; thread_idx < p_pool->thread_count; thread_idx++) {
        
    }
    free(p_pool->p_handles);
    p_pool->p_handles = NULL;
    free(p_pool->p_handle_attribs);
    p_pool->p_handle_attribs = NULL;
#endif//_WIN32

    free(p_pool->p_threads);
    p_pool->p_threads = NULL;
    p_pool->thread_count = 0;

    return SH_THREADS_SUCCESS;
}


ShThreadsStatus shAllocateMutexes(uint32_t mutex_count, ShMutexPool* p_pool) {
    shThreadsError(mutex_count == 0, "invalid mutex count", return SH_INVALID_MUTEX_RANGE);

    ShMutex* p_mutexes = (ShMutex*)calloc(mutex_count, sizeof(ShMutex));
    shThreadsError(p_mutexes == NULL, "invalid mutexes memory", return SH_INVALID_MUTEX_MEMORY);

#ifdef _WIN32
    for (uint32_t mutex_idx = 0; mutex_idx < mutex_count; mutex_idx++) {
        p_mutexes[mutex_idx] = CreateMutex(
            NULL,
            FALSE,
            NULL
        );
        shThreadsError(p_mutexes[mutex_idx] == NULL, "invalid mutex memory", return SH_INVALID_MUTEX_MEMORY);
    }
#else
    for (uint32_t mutex_idx = 0; mutex_idx < mutex_count; mutex_idx++) {
        int r = pthread_mutex_init(&p_mutexes[mutex_idx], NULL);
        shThreadsError(r != 0, "invalid mutex memory", return SH_INVALID_MUTEX_MEMORY);
    }
#endif//_WIN32

    p_pool->p_mutexes = p_mutexes;
    p_pool->mutex_count = mutex_count;

    return SH_THREADS_SUCCESS;
}

ShThreadsStatus shWaitForMutexes(uint32_t first_mutex, uint32_t mutex_count, uint64_t ms_timeout, ShMutexPool* p_pool) {
    shThreadsError(p_pool == NULL,            "invalid mutex pool memory", return SH_INVALID_MUTEX_POOL_MEMORY);
    shThreadsError(p_pool->p_mutexes == NULL, "invalid mutexes memory",    return SH_INVALID_MUTEX_MEMORY);

    shThreadsError(first_mutex > p_pool->mutex_count || mutex_count > p_pool->mutex_count || (first_mutex + mutex_count) > p_pool->mutex_count,
        "invalid mutex range", 
        return SH_INVALID_MUTEX_RANGE
    );

#ifdef _WIN32
    DWORD lock_status = WaitForMultipleObjects(
        mutex_count,
        &p_pool->p_mutexes[first_mutex],
        TRUE,
        (DWORD)ms_timeout
    );
    shThreadsError(lock_status < WAIT_OBJECT_0 || lock_status > (DWORD)((uint32_t)WAIT_OBJECT_0 + mutex_count - 1), "failed waiting for mutexes", return SH_MUTEX_LOCK_FAILURE);
#else
    uint32_t lock_status = 0;
    for (uint32_t mutex_idx = first_mutex; mutex_idx < (first_mutex + mutex_count); mutex_idx++) {
        lock_status += pthread_mutex_lock(&p_pool->p_mutexes[mutex_idx]) == 0;
    }
    shThreadsError(lock_status != mutex_count, "failed waiting for mutexes", return SH_MUTEX_LOCK_FAILURE);
#endif//_WIN32

    return SH_THREADS_SUCCESS;
}

ShThreadsStatus shUnlockMutexes(uint32_t first_mutex, uint32_t mutex_count, ShMutexPool* p_pool) {
    shThreadsError(p_pool == NULL,            "invalid mutex pool memory", return SH_INVALID_MUTEX_POOL_MEMORY);
    shThreadsError(p_pool->p_mutexes == NULL, "invalid mutexes memory",    return SH_INVALID_MUTEX_MEMORY);

    shThreadsError(first_mutex > p_pool->mutex_count || mutex_count > p_pool->mutex_count || (first_mutex + mutex_count) > p_pool->mutex_count,
        "invalid mutex range", 
        return SH_INVALID_MUTEX_RANGE
    );

#ifdef _WIN32
    uint32_t unlock_status = 0;
    for (uint32_t mutex_idx = first_mutex; mutex_idx < (first_mutex + mutex_count); mutex_idx++) {
        unlock_status += (uint32_t)ReleaseMutex(p_pool->p_mutexes[mutex_idx]);
    }
    shThreadsError(unlock_status != mutex_count, "failed unlocking mutexes", return SH_MUTEX_UNLOCK_FAILURE);
#else
    uint32_t unlock_status = 0;
    for (uint32_t mutex_idx = first_mutex; mutex_idx < (first_mutex + mutex_count); mutex_idx++) {
        unlock_status += pthread_mutex_unlock(&p_pool->p_mutexes[mutex_idx]) == 0;
        shThreadsError(unlock_status != mutex_count, "failed unlocking mutexes", return SH_MUTEX_UNLOCK_FAILURE);
    }
#endif//_WIN32

	return SH_THREADS_SUCCESS;
}

ShThreadsStatus shReleaseMutexes(ShMutexPool* p_pool) {
    shThreadsError(p_pool == NULL, "invalid mutex pool memory", return SH_INVALID_MUTEX_POOL_MEMORY);
    shThreadsError(p_pool == NULL && p_pool->mutex_count > 0, "invalid mutexes memory", return SH_INVALID_MUTEX_MEMORY);
    
#ifdef _WIN32
    for (uint32_t mutex_idx = 0; mutex_idx < p_pool->mutex_count; mutex_idx++) {
        CloseHandle(p_pool->p_mutexes[mutex_idx]);
    }
#else
    for (uint32_t mutex_idx = 0; mutex_idx < p_pool->mutex_count; mutex_idx++) {
        pthread_mutex_destroy(&p_pool->p_mutexes[mutex_idx]);
    }
#endif//_WIN32

    free(p_pool->p_mutexes);
    p_pool->p_mutexes = NULL;
    p_pool->mutex_count = 0;

    return SH_THREADS_SUCCESS;
}



#ifdef __cplusplus
}
#endif//__cplusplus
