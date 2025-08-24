LCM_FILES := $(wildcard *.lcm)
HPP_FILES := $(LCM_FILES:.lcm=.hpp)

all: $(HPP_FILES)

%.hpp: %.lcm
	lcm-gen -x $<
	mv data/$(basename $<).hpp ../data_lcm	