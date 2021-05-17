TARGETS = s-talk

CC_C = $(CROSS_TOOL)gcc

CFLAGS =  -g -std=gnu99 -Werror -pthread

all: clean $(TARGETS)

$(TARGETS): 
	$(CC_C) $(CFLAGS) list.c $@.c -o $@ 

clean:
	rm -rf $(TARGETS)