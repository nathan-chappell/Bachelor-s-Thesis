CPP =  g++ --std=c++14
CPPFLAGS = -Wall -g -O0 `pkg-config gtkmm-3.0 --cflags --libs` -Iinclude
COMP = $(CPP) $^ $(CPPFLAGS) -o $@

main : src/main.o src/UI.o src/common.o
	$(COMP)

parse_test : src/parse_test.o src/parse.o src/common.o
