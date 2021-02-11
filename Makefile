
all: _vim

# Command for Vim to compile and run the project
_vim:
	mkdir -p build
	cd build && cmake .. && make torusview && ./torusview/torusview

clean:
	rm -rf build
