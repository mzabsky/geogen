CPP_FILES := $(shell find src/GeoGen src/Console -type f -iname '*.cpp')
C_FILES := $(shell find src/GeoGen src/antlr3 src\lpng1612 -type f -iname '*.c')
CPPOBJ_FILES := $(patsubst %.cpp, %.o, $(CPP_FILES:.cpp=.o))
COBJ_FILES := $(patsubst %.c, %.o, $(C_FILES:.c=.o))
CPP_FLAGS := -std=c++98 -Isrc/antlr3 -Iinclude -Isrc/png++ -Isrc\lpng1612 -v -g
C_FLAGS := -std=c++98 -Isrc/antlr3 -fpermissive -w -DANTLR3_NODEBUGGER -g

all:geogen

geogen: $(COBJ_FILES) $(CPPOBJ_FILES)
	g++ -o $@ $^ -lz -g

%.o: %.cpp
	g++ $(CPP_FLAGS) -c -o $@ $<
	
%.o: %.c
	g++ $(C_FLAGS) -c -o $@ $<

clean:
	@rm $(COBJ_FILES) $(CPPOBJ_FILES)