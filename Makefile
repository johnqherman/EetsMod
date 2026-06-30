# build the loader. mods are plain .cpp the loader compiles at runtime from <game>/mods/.
#   make                       -> build/libeetsmod.so
#   make install GAME=..        -> loader + headers (no mods)
#   make install-examples GAME=.. -> install example .eetsmod bundles into <game>/mods/
.PHONY: all loader win install install-examples check apidoc bundles release clean

VERSION  := $(shell sed -n 's/.*EETSMOD_VERSION "\(.*\)".*/\1/p' loader/loader.cpp)
CXX      ?= g++
WINCXX   ?= i686-w64-mingw32-g++
CXXFLAGS ?= -O2 -fPIC -std=c++17 -Wall -Iinclude
BUILD    ?= build
HDRS     := loader/hook.h loader/platform.h loader/compat.h include/eetsmod.h include/eets_engine.h include/eets_addr.h
WINHDRS  := loader/hook.h loader/platform.h loader/compat.h include/eetsmod.h include/eets_engine.h include/eets_addr_win.h

all: loader
loader: $(BUILD)/libeetsmod.so

$(BUILD)/libeetsmod.so: loader/loader.cpp $(HDRS)
	@mkdir -p $(BUILD)
	$(CXX) $(CXXFLAGS) -shared -fvisibility=default -o $@ loader/loader.cpp -ldl

# 32-bit Windows loader: a version.dll proxy injected into the (i386) game.
# --kill-at strips MinGW's stdcall @N decoration from our exports, so the proxy
# exports the plain names (GetFileVersionInfoA, ...) that SDL2.dll imports by name.
win: $(BUILD)/version.dll
$(BUILD)/version.dll: loader/loader.cpp $(WINHDRS)
	@mkdir -p $(BUILD)
	$(WINCXX) -O2 -std=c++17 -Wall -Iinclude -DEETSMOD_LOADER -shared -static \
		-Wl,--kill-at,--out-implib,$(BUILD)/libeetsmod.dll.a -o $@ loader/loader.cpp

install: all
	@test -n "$(GAME)" || { echo "usage: make install GAME=/path/to/Eets"; exit 1; }
	@test -x "$(GAME)/Eets" || { echo "no Eets binary at $(GAME)"; exit 1; }
	install -m644 build/libeetsmod.so "$(GAME)/libeetsmod.so"
	mkdir -p "$(GAME)/eetsmod-include" "$(GAME)/mods"
	install -m644 include/*.h "$(GAME)/eetsmod-include/"
	@echo "installed loader + headers to $(GAME). drop .eetsmod mods in $(GAME)/mods/"

# compile every example the way the loader does, without launching the game
check:
	tools/check-mod.sh examples/*.cpp

# regenerate API.md from the headers
apidoc:
	tools/gen-api-ref.sh

# pack each example into a one-file .eetsmod (source + prebuilt .so + manifest)
bundles:
	@mkdir -p examples/build
	@for f in examples/*.cpp; do n=$$(basename "$$f" .cpp); bin/eetsmod pack "$$f" -o "examples/build/$$n.eetsmod"; done
	@echo "packed example bundles -> examples/build/"

install-examples: install bundles
	@for f in examples/build/*.eetsmod; do [ -f "$$f" ] && install -m644 "$$f" "$(GAME)/mods/$$(basename $$f)"; done; true
	@echo "installed example .eetsmod mods to $(GAME)/mods/"

# ships BOTH loaders (Linux .so + Windows version.dll) so the release works on either platform
release: all win bundles
	@echo "packaging eetsmod-$(VERSION)"
	rm -rf dist/eetsmod-$(VERSION)
	mkdir -p dist/eetsmod-$(VERSION)/eetsmod-include dist/eetsmod-$(VERSION)/examples dist/eetsmod-$(VERSION)/mods
	cp build/libeetsmod.so build/version.dll dist/eetsmod-$(VERSION)/
	cp include/*.h          dist/eetsmod-$(VERSION)/eetsmod-include/
	@for f in examples/*.cpp examples/*.cfg; do [ -f "$$f" ] && cp "$$f" dist/eetsmod-$(VERSION)/examples/; done; true
	@for f in examples/build/*.eetsmod; do [ -f "$$f" ] && cp "$$f" dist/eetsmod-$(VERSION)/mods/; done; true
	cp README.md dist/eetsmod-$(VERSION)/ 2>/dev/null || true
	cp bin/eetsmod tools/check-mod.sh tools/new-mod.sh tools/add-sound.sh dist/eetsmod-$(VERSION)/ 2>/dev/null || true
	cp API.md KNOWN_ISSUES.md dist/eetsmod-$(VERSION)/ 2>/dev/null || true
	cp tools/install.sh tools/install.bat dist/eetsmod-$(VERSION)/
	chmod +x dist/eetsmod-$(VERSION)/install.sh
	cd dist && tar czf eetsmod-$(VERSION).tar.gz eetsmod-$(VERSION)
	@echo "-> dist/eetsmod-$(VERSION).tar.gz (Linux .so + Windows version.dll)"

clean:
	rm -rf build dist examples/build
