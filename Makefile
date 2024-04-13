all:
	g++ -Iinclude -Llib -o build/main main.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

run: all
	./build/main.exe

clean:
	rm -f build/main
