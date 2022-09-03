#ifdef __cplusplus
extern "C" {
#endif//_cplusplus

#include <shthreads/shthreads.h>
#include <assert.h>


SH_BUILD_THREAD_ARGS(
	sample_args, float value
);
size_t sample(sample_args* p_args) {
	assert(p_args != NULL);

	printf("power of %.1f: %.2f\n",
		p_args->value,
		p_args->value * p_args->value);

	return 0;
}


int main(void) {

	ShThreadsHandle handle = shAllocateThreads(3);

	ShThreadsStatus status = shCreateThread(
		0,
		sample,
		1024,
		&handle
	);
	assert(status == SH_THREAD_SUCCESS);
	status = shCreateThread(
		1,
		sample,
		1024,
		&handle
	);
	assert(status == SH_THREAD_SUCCESS);

	sample_args args_0 = { 1.5f };
	sample_args args_1 = { 2.5f };
	ShThreadParameters parameters[2] = { &args_0, &args_1 };
	
	status = shLaunchThreads(0, 2, parameters, &handle);
	assert(status == SH_THREAD_SUCCESS);

	status = shWaitForThreads(0, 2, UINT64_MAX, &handle);
	assert(status == SH_THREAD_SUCCESS);

	shThreadsRelease(&handle);

	return 0;
}

#ifdef __cplusplus
}
#endif//_cplusplus

