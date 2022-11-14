all: stacktask

FLAGS = -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++\
 -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations\
  -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported\
   -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal\
    -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2\
	 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual\
	  -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow\
	   -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2\
	    -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types\
		 -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef\
		  -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros\
		   -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing\
		    -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new\
			 -fsized-deallocation -fstack-protector -fstrict-overflow -fno-omit-frame-pointer\
			  -Wlarger-than=8192 -Wstack-usage=8192 -pie -fPIE\
			   -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,\
			   -fsanitize=float-divide-by-zero,integer-divide-by-zero,nonnull-attribute,null,\
			   -fsanitize=object-size,return,returns-nonnull-attribute,shift,\
			   -fsanitize=address,signed-integer-overflow,undefined,unreachable,vla-bound,vptr,

stacktask: main.o stack.o debug.o log.o hash.o
	g++ main.o stack.o debug.o log.o hash.o $(FLAGS) -o stack

main.o: main.cpp stack.h debug.h
	g++ -c main.cpp $(FLAGS)

stack.o: stack.cpp stack.h debug.h log.h
	g++ -c stack.cpp $(FLAGS)

debug.o: debug.cpp debug.h
	g++ -c debug.cpp $(FLAGS)

log.o: log.cpp log.h
	g++ -c log.cpp $(FLAGS)

hash.o: hash.cpp hash.h
	g++ -c hash.cpp $(FLAGS)

.PHONY: clean

clean:
	rm *.o stackj