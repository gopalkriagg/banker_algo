prog: prog.cpp fn_definitions state_class state_class.h declarations.h
	g++ -c prog.cpp -o prog.o
	g++ prog.o fn_definitions.o state_class.o -o prog
	rm *.o


fn_definitions: fn_definitions.cpp state_class.h declarations.h
	g++ -c fn_definitions.cpp -o fn_definitions.o

state_class: state_class.cpp state_class.h
	g++ -c state_class.cpp -o state_class.o