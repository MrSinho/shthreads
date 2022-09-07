#ifdef __cplusplus
extern "C" {
#endif//_cplusplus



#include <shthreads/shthreads.h>

#include <assert.h>
#include <inttypes.h>



SH_BUILD_THREAD_ARGS(
	sample_args, uint64_t value; ShMutex mutex
);
void* sample(sample_args* p_args) {
	assert(p_args != NULL);

	printf("thread %" PRIi64 " starting...\n",
		shGetCurrentThreadId()
	);

	for (;p_args->value < 1000000;) {

		ShThreadsStatus status = shWaitForMutexes(0, 1, UINT64_MAX, &p_args->mutex);
		assert(status == SH_THREADS_SUCCESS);

		printf("\tthread %" PRIi64 ": %" PRIi64 " + 1: %" PRIi64 "\n",
			shGetCurrentThreadId(),
			p_args->value,
			p_args->value
		);
		p_args->value++;

		status = shUnlockMutexes(0, 1, &p_args->mutex);
		assert(status == SH_THREADS_SUCCESS);
	}

	return NULL;
}



int main(void) {
	
	//             //
	//CREATE HANDLE//
	//             //
	ShThreadsHandle handle = shAllocateThreads(2);


	//                  //
	//INITIALIZE THREADS//   
	//                  //
	ShThreadsStatus status = shCreateThread(
		0,
		sample,
		1024,
		&handle
	);
	assert(status == SH_THREADS_SUCCESS);
	status = shCreateThread(
		1,
		sample,
		1024,
		&handle
	);
	assert(status == SH_THREADS_SUCCESS);

	//            //
	//CREATE MUTEX//
	//            //
	ShMutex* p_mutexes = shCreateMutexes(1, &handle);

	//                       //
	//SETUP THREDS PARAMETERS//
	//                       //
	sample_args        args_0        = { 2, p_mutexes[0] };  //shared memory block { uint64_t value, ShMutex mutex }
	ShThreadParameters parameters[2] = { &args_0, &args_0 }; //use same memory for both threads

	//           //
	//RUN THREADS//
	//           //
	status = shLaunchThreads(0, 2, parameters, &handle);
	assert(status == SH_THREADS_SUCCESS);

	//                   //
	//WAIT END OF THREADS//
	//                   //
	uint64_t return_values[2] = { 0 };
	status                    = shWaitForThreads(0, 2, UINT64_MAX, return_values, &handle);
	assert(status            == SH_THREADS_SUCCESS);

	//                                //
	//CLOSE THREADS AND RELEASE MEMORY//
	//                                //
	shThreadsRelease(&handle);



#ifdef _WIN32
	system("pause");
#endif//_WIN32



	return 0;
}



#ifdef __cplusplus
}
#endif//_cplusplus

