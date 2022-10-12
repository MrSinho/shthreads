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
		printf("\tthread %" PRIu64 ": power of %i: %i\n",
			shGetCurrentThreadId(),
			p_args->value,
			p_args->value * p_args->value
		);
		p_args->value *= p_args->value;
		shThreadsSleep(1000);
	}

	uint64_t return_value = (uint64_t)p_args->value;

	return (void*)return_value;
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

	//                 //
	//GET THREADS STATE//
	//                 //
	ShThreadState state_0 = SH_THREAD_INVALID_STATE;
	ShThreadState state_1 = SH_THREAD_INVALID_STATE;
	shGetThreadState(0, &state_0, &handle);
	shGetThreadState(1, &state_1, &handle);
	printf("\nThreads state: \n\tthread %" PRIu64 ": %s,\n\tthread %" PRIu64 ": %s\n\n",
		handle.p_threads[0].id,
		state_0 == SH_THREAD_RUNNING ? "RUNNING" : "RETURNED",
		handle.p_threads[1].id,
		state_1 == SH_THREAD_RUNNING ? "RUNNING" : "RETURNED"
	);

	//                   //
	//WAIT END OF THREADS//
	//                   //
	uint64_t return_values[2] = {0};
	status                    = shWaitForThreads(0, 2, UINT64_MAX, return_values, &handle);
	assert(status            == SH_THREADS_SUCCESS);

	//                 //
	//GET THREADS STATE//
	//                 //
	state_0 = SH_THREAD_INVALID_STATE;
	state_1 = SH_THREAD_INVALID_STATE;
	shGetThreadState(0, &state_0, &handle);
	shGetThreadState(1, &state_1, &handle);
	printf("\nThreads state: \n\tthread %" PRIu64 ": %s,\n\tthread %" PRIu64 ": %s\n\n",
		handle.p_threads[0].id,
		state_0 == SH_THREAD_RUNNING ? "RUNNING" : "RETURNED",
		handle.p_threads[1].id,
		state_1 == SH_THREAD_RUNNING ? "RUNNING" : "RETURNED"
	);

	//   //
	//LOG//
	//   //
	printf("End of program\n\treturn values:\n\tthread %" PRIu64 ": %" PRIu64 "\n\tthread %" PRIu64 ": %" PRIu64"\n\n", 
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

