all:
	mkdir -p build
	cmake -G Ninja -B ./build -DCMAKE_BUILD_TYPE=Release
	ninja -C build

debug:
	mkdir -p build
	cmake -G Ninja -B ./build -DCMAKE_BUILD_TYPE=Debug
	ninja -C build

run: 
	./build/cubo

clean:
	rm -rf build

crun: all run


