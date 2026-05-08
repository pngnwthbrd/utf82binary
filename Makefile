C_FILES = main.c
build:
		gcc -o utf82binary ${C_FILES}
clean:
		rm -f utf82binary
