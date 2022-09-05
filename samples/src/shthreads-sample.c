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
		printf("thread %" PRIi64 ": power of %i: %i\n",
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

	return NULL;
}



int main(void) {

	ShThreadsHandle handle = shAllocateThreads(2);

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

	sample_args args_0 = { 2 };
	sample_args args_1 = { 5 };
	ShThreadParameters parameters[2] = { &args_0, &args_1 };

	status = shLaunchThreads(0, 2, parameters, &handle);
	assert(status == SH_THREADS_SUCCESS);

	status = shWaitForThreads(0, 2, UINT64_MAX, &handle);
	assert(status == SH_THREADS_SUCCESS);

	shThreadsRelease(&handle);

	puts("End of program\n");
#ifdef _WIN32
	system("pause");
#endif//_WIN32

	return 0;
}



#ifdef __cplusplus
}
#endif//_cplusplus

