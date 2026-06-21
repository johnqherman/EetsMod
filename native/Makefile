# build the loader. mods are plain .cpp the loader compiles at runtime from <game>/mods/.
#   make                       -> build/libeetsmod.so
#   make install GAME=..        -> loader + headers (no mods)
#   make install-examples GAME=.. -> also copy native/examples/* into <game>/mods/
.PHONY: all loader install install-examples check apidoc bundles release clean

VERSION := $(shell sed -n 's/.*EETSMOD_VERSION "\(.*\)".*/\1/p' loader/loader.cpp)

all: loader

loader:
	$(MAKE) -C loader

install: all
	@test -n "$(GAME)" || { echo "usage: make install GAME=/path/to/Eets"; exit 1; }
	@test -x "$(GAME)/Eets" || { echo "no Eets binary at $(GAME)"; exit 1; }
	install -m644 build/libeetsmod.so "$(GAME)/libeetsmod.so"
	mkdir -p "$(GAME)/eetsmod-include" "$(GAME)/mods"
	install -m644 include/*.h "$(GAME)/eetsmod-include/"
	@echo "installed loader + headers to $(GAME). drop .eetsmod mods in $(GAME)/mods/"

# compile every example the way the loader does, without launching the game
check:
	../tools/check-mod.sh examples/*.cpp

# regenerate docs/NATIVE_API.md from the headers
apidoc:
	../tools/gen-api-ref.sh

# pack each example into a one-file .eetsmod (source + prebuilt .so + manifest)
bundles:
	@mkdir -p examples/build
	@for f in examples/*.cpp; do n=$$(basename "$$f" .cpp); ../bin/eetsmod pack "$$f" -o "examples/build/$$n.eetsmod"; done
	@echo "packed example bundles -> native/examples/build/"

install-examples: install bundles
	@for f in examples/build/*.eetsmod; do [ -f "$$f" ] && install -m644 "$$f" "$(GAME)/mods/$$(basename $$f)"; done; true
	@echo "installed example .eetsmod mods to $(GAME)/mods/"

release: all bundles
	@echo "packaging eetsmod-$(VERSION)"
	rm -rf dist/eetsmod-$(VERSION)
	mkdir -p dist/eetsmod-$(VERSION)/eetsmod-include dist/eetsmod-$(VERSION)/examples dist/eetsmod-$(VERSION)/mods
	cp build/libeetsmod.so dist/eetsmod-$(VERSION)/
	cp include/*.h          dist/eetsmod-$(VERSION)/eetsmod-include/
	@for f in examples/*.cpp examples/*.cfg; do [ -f "$$f" ] && cp "$$f" dist/eetsmod-$(VERSION)/examples/; done; true
	@for f in examples/build/*.eetsmod; do [ -f "$$f" ] && cp "$$f" dist/eetsmod-$(VERSION)/mods/; done; true
	cp README.md dist/eetsmod-$(VERSION)/ 2>/dev/null || true
	cp ../bin/eetsmod ../tools/check-mod.sh ../tools/new-mod.sh ../tools/add-sound.sh dist/eetsmod-$(VERSION)/ 2>/dev/null || true
	cp ../docs/NATIVE_API.md dist/eetsmod-$(VERSION)/ 2>/dev/null || true
	printf '#!/usr/bin/env bash\nset -e\nG="$${1:-$$HOME/.local/share/Steam/steamapps/common/Eets}"\n[ -x "$$G/Eets" ] || { echo "no Eets at $$G"; exit 1; }\ninstall -m644 libeetsmod.so "$$G/"\nmkdir -p "$$G/eetsmod-include" "$$G/mods"\ninstall -m644 eetsmod-include/*.h "$$G/eetsmod-include/"\nfor m in mods/*.eetsmod; do [ -f "$$m" ] && cp "$$m" "$$G/mods/"; done\necho "installed. Steam launch option: LD_PRELOAD=$$G/libeetsmod.so %%command%%"\n' > dist/eetsmod-$(VERSION)/install.sh
	chmod +x dist/eetsmod-$(VERSION)/install.sh
	cd dist && tar czf eetsmod-$(VERSION).tar.gz eetsmod-$(VERSION)
	@echo "-> native/dist/eetsmod-$(VERSION).tar.gz"

clean:
	$(MAKE) -C loader clean
	rm -rf build dist
