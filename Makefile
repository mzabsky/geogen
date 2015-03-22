CPP_FILES := $(shell find src/GeoGen src/Console -type f -iname '*.cpp')
C_FILES := $(shell find src/GeoGen src/antlr3 -type f -iname '*.c')
CPPOBJ_FILES := $(patsubst %.cpp, %.o, $(CPP_FILES:.cpp=.o))
COBJ_FILES := $(patsubst %.c, %.o, $(C_FILES:.c=.o))
CPP_FLAGS := -std=c++98 -Isrc/antlr3 -Iinclude -v
C_FLAGS := -std=c++98 -Isrc/antlr3 -fpermissive -w -DANTLR3_NODEBUGGER

all:main.exe

main.exe: $(COBJ_FILES) $(CPPOBJ_FILES)
	g++ -o $@ $^

%.o: %.cpp
	g++ $(CPP_FLAGS) -c -o $@ $<
	
%.o: %.c
	g++ $(C_FLAGS) -c -o $@ $<

clean:
	@rm $(COBJ_FILES) $(CPPOBJ_FILES)