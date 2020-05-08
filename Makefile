OBJ_dir = object
SRC_dir = source
BIN_dir = binary
INC_dir = include

skin = native
CC = gcc
SOURCES = aircraft.c fmc.c fdr.c ctrl.c 
OBJETOS = $(SOURCES:.c=.o)

CFLAGS := $(shell /usr/xenomai/bin/xeno-config --skin=$(skin) --cflags)
LDFLAGS := $(shell /usr/xenomai/bin/xeno-config --skin=$(skin) --ldflags)

out:
	make $(OBJ_dir)/ctrl.o 
	make $(OBJ_dir)/fdr.o 
	make $(OBJ_dir)/fmc.o 
	make $(OBJ_dir)/aircraft.o 
	make aircraft

aircraft:
	$(CC) $(OBJ_dir)/ctrl.o $(OBJ_dir)/fdr.o $(OBJ_dir)/fmc.o $(OBJ_dir)/aircraft.o -o $(BIN_dir)/aircraft $(CFLAGS) $(LDFLAGS) 

$(OBJ_dir)/%.o: $(SRC_dir)/%.c $($@)
	$(CC) -Wall -c -I include -o $@ $< $(CFLAGS) $(LDFLAGS)

clean:
	@rm -f $(addprefix $(OBJ_dir)/,$(OBJETOS))
	@rm -f $(addprefix $(BIN_dir)/,aircraft)
