#ifdef __cplusplus
extern "C" {
#endif//_cplusplus



#include <shthreads/shthreads.h>

#include <assert.h>
#include <inttypes.h>



typedef struct sample_args {
	uint32_t* p_shared_value;
	ShMutex   mutex;
} sample_args;

void* sample(sample_args* p_args) {
	assert(p_args                 != NULL);
	assert(p_args->p_shared_value != NULL);

	printf("thread %" PRIi64 " starting...\n",
		shGetCurrentThreadId()
	);

	for (;;) {

		ShThreadsStatus status = shWaitForMutexes(0, 1, UINT64_MAX, &p_args->mutex);
		assert(status == SH_THREADS_SUCCESS);

		if ((*p_args->p_shared_value) >= 100000) { 
			shUnlockMutexes(0, 1, &p_args->mutex); 
			break;
		}

		printf("\tthread %" PRIi64 "\t\t: %" PRIi32 " + 1 : \t\t%" PRIi32 "\n",
			shGetCurrentThreadId(),
			(*p_args->p_shared_value),
			(*p_args->p_shared_value) + 1
		);

		(*p_args->p_shared_value)++;

		status = shUnlockMutexes(0, 1, &p_args->mutex);

		assert(status == SH_THREADS_SUCCESS);
	}

	return NULL;
}



int main(void) {
	
	//                  //
	//CREATE THREAD POOL//
	//                  //
	ShThreadPool    pool   = { 0 };
	ShThreadsStatus status = shAllocateThreads(2, &pool);

	assert(status         == SH_THREADS_SUCCESS);
	assert(pool.p_threads != NULL);


	//                  //
	//INITIALIZE THREADS//   
	//                  //
	status = shCreateThread(
		0,
		sample,
		1024,
		&pool
	);
	assert(status == SH_THREADS_SUCCESS);
	status = shCreateThread(
		1,
		sample,
		1024,
		&pool
	);
	assert(status == SH_THREADS_SUCCESS);

	//            //
	//CREATE MUTEX//
	//            //
	ShMutex mutex = SH_THREADS_NULL_MUTEX;
	status = shCreateMutexes(1, &mutex);
	assert(status == SH_THREADS_SUCCESS);

	//                       //
	//SETUP THREDS PARAMETERS//
	//                       //
	uint32_t           shared_value  = 0;
	sample_args        args_0        = { &shared_value, mutex }; //shared memory block { uint32_t shared_value, ShMutex mutex }
	sample_args        args_1        = { &shared_value, mutex }; //shared memory block { uint32_t shared_value, ShMutex mutex }
	ShThreadParameters parameters[2] = { &args_0, &args_1 };     //use same memory for both threads

	//           //
	//RUN THREADS//
	//           //
	status = shLaunchThreads(0, 2, parameters, &pool);
	assert(status == SH_THREADS_SUCCESS);

	//                   //
	//WAIT END OF THREADS//
	//                   //
	uint64_t return_values[2] = { 0 };
	status = shWaitForThreads(0, 2, UINT64_MAX, return_values, &pool);
	assert(status == SH_THREADS_SUCCESS);

	//                                //
	//CLOSE THREADS AND RELEASE MEMORY//
	//                                //
	status = shReleaseThreads(&pool);
	status = shReleaseMutexes(0, 1, &mutex);


#ifdef _WIN32
	system("pause");
#endif//_WIN32



	return 0;
}



#ifdef __cplusplus
}
#endif//_cplusplus

