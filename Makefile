all:
	g++ main.cpp -lncurses -o dbuild
install:
	g++ main.cpp -lncurses -o ./build/usr/bin/dbuild
	cd ./build;./usr/bin/dbuild
clean:
	rm -rf ./build/usr/bin/dbuild
	rm -rf build.deb
	rm -rf ./dbuild
	rm -rf ./build/DEBIAN
