# Build the loader. Mods are NOT built here - they are plain .cpp files that the
# loader compiles at runtime when you drop them in <game>/mods/.
#   make                  -> build/libeetsmod.so
#   make install GAME=..   -> install loader + headers + sample mods into <GAME>
.PHONY: all loader install clean

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

clean:
	$(MAKE) -C loader clean
	rm -rf build
