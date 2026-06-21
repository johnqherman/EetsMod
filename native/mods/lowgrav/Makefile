CXX      ?= g++
CXXFLAGS ?= -O2 -fPIC -std=c++17 -Wall -I../../include
BUILD    ?= ../../build/mods

$(BUILD)/lowgrav.so: lowgrav.cpp ../../include/eetsmod.h ../../include/eets_engine.h
	@mkdir -p $(BUILD)
	$(CXX) $(CXXFLAGS) -shared -o $@ lowgrav.cpp

clean:
	rm -f $(BUILD)/lowgrav.so
