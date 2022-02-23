
build-directory ?= build/
jobs ?= 8

all: _vim_run

# Commands for Vim to compile and run the project
.PHONY: _vim_run _vim_build _vim_clean _vim_test
_vim_run: _vim_build
	cd $(build-directory)
	./torusview/torusview

_vim_build:
	mkdir -p $(build-directory)
	cd $(build-directory) 
	cmake .. 
	make -j$(jobs) torusview

_vim_test:


_vim_clean:
	rm -rf $(build-directory)

.PHONY: .ONESHELL
.ONESHELL:

