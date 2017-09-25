OS := $(shell uname)
ifeq ($(OS),Darwin)
  CXXFLAGS=-I.. -I/usr/local/include/eigen3 -g -O0 -std=c++11 
else
  CXXFLAGS=-I.. -I/usr/include/eigen3 -g -O0 -std=c++11
endif

exp:
	g++ $(CXXFLAGS) regen.cpp -o regen

clean:
	rm -f regen *.o *~ 
