# Build the native loader and all sample mods.
#   make                 -> build/libeetsmod.so + build/mods/*.so
#   make install GAME=..  -> copy loader to <GAME>/ and mods to <GAME>/mods/
.PHONY: all loader mods install clean

all: loader mods

loader:
	$(MAKE) -C loader

mods:
	$(MAKE) -C mods/lowgrav

install: all
	@test -n "$(GAME)" || { echo "usage: make install GAME=/path/to/Eets"; exit 1; }
	@test -x "$(GAME)/Eets" || { echo "no Eets binary at $(GAME)"; exit 1; }
	install -m644 build/libeetsmod.so "$(GAME)/libeetsmod.so"
	mkdir -p "$(GAME)/mods"
	install -m644 build/mods/*.so "$(GAME)/mods/"
	@echo "installed loader + $$(ls build/mods | wc -l) mod(s) to $(GAME)"

clean:
	$(MAKE) -C loader clean
	$(MAKE) -C mods/lowgrav clean
	rm -rf build
