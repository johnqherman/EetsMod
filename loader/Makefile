CXX      ?= g++
CXXFLAGS ?= -O2 -fPIC -std=c++17 -Wall -I../include
BUILD    ?= ../build

HDRS = hook.h platform.h ../include/eetsmod.h ../include/eets_engine.h ../include/eets_addr.h
$(BUILD)/libeetsmod.so: loader.cpp $(HDRS)
	@mkdir -p $(BUILD)
	$(CXX) $(CXXFLAGS) -shared -fvisibility=default -o $@ loader.cpp -ldl

clean:
	rm -f $(BUILD)/libeetsmod.so
