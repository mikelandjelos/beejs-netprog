NUMBERS := 1 2 3 4
EXECUTABLES := $(patsubst %,dist/file%.exe,$(NUMBERS))

all: $(EXECUTABLES) run

dist:
	mkdir -p dist

dist/%.exe: %.c | dist
	echo "#include <stdio.h>\n\nint main() {\n\tprintf(\"Hello, World from $<!\\\n\");\n\tfflush(stdout);\n\treturn 0;\n}" > $<
	gcc $< -o $@

%.c:
	# C files will be created by the dist/%.exe rule

run: $(EXECUTABLES)
	$(foreach exe,$(EXECUTABLES),./$(exe);)

clean:
	rm -rf dist file*

.PRECIOUS: %.c
.PHONY: all run clean