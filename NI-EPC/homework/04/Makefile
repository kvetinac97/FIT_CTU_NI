include config.mk

test: test.cpp vector.h
	g++ $(CXXFLAGS) test.cpp -o test

benchmark-gcc: benchmark.cpp vector.h
	g++ $(CXXFLAGS) $(CXXFLAGSOPT) benchmark.cpp -lbenchmark `llvm-config --ldflags` -lLLVM -o benchmark-gcc

benchmark-clang: benchmark.cpp vector.h
	clang++ $(CXXFLAGS) $(CXXFLAGSOPT) benchmark.cpp -lbenchmark -lLLVM -o benchmark-clang
