CPP_FILES := $(shell find src/GeoGen src/Console -type f -iname '*.cpp')
C_FILES := $(shell find src/GeoGen src/antlr3 src/lpng1612 -type f -iname '*.c')
CPPOBJ_FILES := $(patsubst %.cpp, %.o, $(CPP_FILES:.cpp=.o))
COBJ_FILES := $(patsubst %.c, %.o, $(C_FILES:.c=.o))
CPP_FLAGS := -std=c++98 -Isrc/antlr3 -Iinclude -Isrc/png++ -Isrc/lpng1612 -Iinclude -v  -DHAVE_STDINT_H -DSTDC_HEADERS -DHAVE_STRING_H -DHAVE_STRINGS_H  -g
C_FLAGS := -std=c++98 -Isrc/antlr3 -fpermissive -w -DANTLR3_NODEBUGGER -DHAVE_STDINT_H -DSTDC_HEADERS -DHAVE_STRING_H -DHAVE_STRINGS_H -g

all:headers geogen 

geogen: $(COBJ_FILES) $(CPPOBJ_FILES)
	g++ -o $@ $^ -lz -g
	
headers:
	rm -rf include/GeoGen
	mkdir -p include/GeoGen	
	find src/GeoGen -name \*.hpp | sed 's/^...........//' | sed /AntlrRaiiWrappers/d | sed "1s;^;#pragma once\n\n/// @file GeoGen.hpp Header file used for including GeoGen into other projects.\n// Machine generated file, do not edit.\n\n;" > include/GeoGen/GeoGen.hpp
	find src/GeoGen -name \*.hpp -exec cp {} include/GeoGen \;

%.o: %.cpp
	g++ $(CPP_FLAGS) -c -o $@ $<
	
%.o: %.c
	g++ $(C_FLAGS) -c -o $@ $<

clean:
	@rm $(COBJ_FILES) $(CPPOBJ_FILES)