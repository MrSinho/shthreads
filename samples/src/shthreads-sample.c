#ifdef __cplusplus
extern "C" {
#endif//_cplusplus

#include <shthreads/shthreads.h>

#include <assert.h>
#include <inttypes.h>



SH_BUILD_THREAD_ARGS(
	sample_args, uint32_t value
);
void* sample(sample_args* p_args) {
	assert(p_args != NULL);

	for (uint8_t i = 0; i < 3; i++) {
		printf("thread %" PRIu64 ": power of %i: %i\n",
			shGetCurrentThreadId(),
			p_args->value,
			p_args->value * p_args->value
		);
		p_args->value *= p_args->value;
#ifdef _WIN32
		Sleep(1000);
#else
		sleep(1.0f);
#endif//_WIN32
	}

	return (void*)p_args->value;
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

	//                       //
	//SETUP THREDS PARAMETERS//
	//                       //
	sample_args        args_0        = { 2 };
	sample_args        args_1        = { 5 };
	ShThreadParameters parameters[2] = { &args_0, &args_1 };

	//           //
	//RUN THREADS//
	//           //
	status         = shLaunchThreads(0, 2, parameters, &handle);
	assert(status == SH_THREADS_SUCCESS);

	//                   //
	//WAIT END OF THREADS//
	//                   //
	uint64_t return_values[2] = {0};
	status                    = shWaitForThreads(0, 2, UINT64_MAX, return_values, &handle);
	assert(status            == SH_THREADS_SUCCESS);

	//   //
	//LOG//
	//   //
	printf("End of program\n return values:\n\tthread %" PRIu64 ": %" PRIu64 "\n\tthread %" PRIu64 ": %" PRIu64"\n", 
		handle.p_threads[0].id,
		return_values[0],
		handle.p_threads[1].id,
		return_values[1]
	);

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

