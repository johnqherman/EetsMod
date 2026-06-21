CXX      ?= g++
CXXFLAGS ?= -O2 -fPIC -std=c++17 -Wall -I../include
BUILD    ?= ../build

$(BUILD)/libeetsmod.so: loader.cpp ../include/eetsmod.h ../include/eets_engine.h
	@mkdir -p $(BUILD)
	$(CXX) $(CXXFLAGS) -shared -fvisibility=default -o $@ loader.cpp -ldl

clean:
	rm -f $(BUILD)/libeetsmod.so
