main: main.c
	cc main.c -o main

ppm: main
	./main

png: ppm
	./to_png.sh

targetDir:
	mkdir -p target

run: png targetDir
	mv *.png target/
	rm *.ppm

