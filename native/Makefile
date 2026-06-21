# Build the loader. Mods are NOT built here - they are plain .cpp files that the
# loader compiles at runtime when you drop them in <game>/mods/.
#   make                  -> build/libeetsmod.so
#   make install GAME=..   -> install loader + headers + sample mods into <GAME>
.PHONY: all loader install release clean

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
	@for f in mods/*.cpp mods/*.cfg; do [ -f "$$f" ] && install -m644 "$$f" "$(GAME)/mods/$$(basename $$f)"; done; true
	@echo "installed loader + headers + sample mods to $(GAME)"
	@echo "mods are compiled on launch; drop .cpp files in $(GAME)/mods/"

release: all
	@echo "packaging eetsmod-$(VERSION)"
	rm -rf dist/eetsmod-$(VERSION)
	mkdir -p dist/eetsmod-$(VERSION)/eetsmod-include dist/eetsmod-$(VERSION)/mods
	cp build/libeetsmod.so dist/eetsmod-$(VERSION)/
	cp include/*.h          dist/eetsmod-$(VERSION)/eetsmod-include/
	@for f in mods/*.cpp mods/*.cfg; do [ -f "$$f" ] && cp "$$f" dist/eetsmod-$(VERSION)/mods/; done; true
	cp ../run-eets.sh README.md dist/eetsmod-$(VERSION)/ 2>/dev/null || true
	printf '#!/usr/bin/env bash\nset -e\nG="$${1:-$$HOME/.local/share/Steam/steamapps/common/Eets}"\n[ -x "$$G/Eets" ] || { echo "no Eets at $$G"; exit 1; }\ninstall -m644 libeetsmod.so "$$G/"\nmkdir -p "$$G/eetsmod-include" "$$G/mods"\ninstall -m644 eetsmod-include/*.h "$$G/eetsmod-include/"\nfor f in mods/*; do [ -f "$$f" ] && install -m644 "$$f" "$$G/mods/"; done\necho "installed. Steam launch option: LD_PRELOAD=$$G/libeetsmod.so %%command%%"\n' > dist/eetsmod-$(VERSION)/install.sh
	chmod +x dist/eetsmod-$(VERSION)/install.sh
	cd dist && tar czf eetsmod-$(VERSION).tar.gz eetsmod-$(VERSION)
	@echo "-> native/dist/eetsmod-$(VERSION).tar.gz"

clean:
	$(MAKE) -C loader clean
	rm -rf build dist
