function BUILD {
	mkdir linux
    cd linux
    cmake -DSH_THREADS_BUILD_SAMPLES=ON ..
    cmake --build .
}

BUILD > .shci/linux/build-output.txt
