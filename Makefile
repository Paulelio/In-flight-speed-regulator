OBJ_dir = object
SRC_dir = source
BIN_dir = binary
INC_dir = include

skin = native
CC = gcc
SOURCES = main.c fmc.c fdr.c ctrl.c 
OBJETOS = $(SOURCES:.c=.o)

CFLAGS := $(shell /usr/xenomai/bin/xeno-config --skin=$(skin) --cflags)
LDFLAGS := $(shell /usr/xenomai/bin/xeno-config --skin=$(skin) --ldflags)

out:
	make fmc.o fdr.o ctrl.o main.o

%.o: $(SRC_dir)/%.c $($@)
	$(CC) -o $(OBJ_dir)/$@ $< $(CFLAGS) $(LDFLAGS) 

clean:
	@rm -f $addprefix $(OBJ_dir)/,$(OBJETOS)
	@rm -f $addprefix $(BIN_dir)/aircraft
