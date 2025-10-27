blah.exe: blah.o
	cc blah.o -o blah.exe # Runs third

blah.o: blah.c
	cc -c blah.c -o blah.o # Runs second

# Typically blah.c would already exist, but I want to limit any additional required files
blah.c:
	touch blah.c
	echo "#include <stdio.h>\n\nint main() {\n\tprintf(\"Hello, World!\\\n\");\n\tfflush(stdout);\n\treturn 0;\n}" > blah.c # Runs first

clean:
	rm -f blah*