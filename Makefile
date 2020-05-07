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
	make ctrl.o fdr.o fmc.o aircraft.o aircraft

aircraft:
	$(CC) $(OBJ_dir)/ctrl.o $(OBJ_dir)/fdr.o $(OBJ_dir)/fmc.o $(OBJ_dir)/aircraft.o -o $(BIN_dir)/aircraft $(CFLAGS) $(LDFLAGS) 

%.o: $(SRC_dir)/%.c $($@)
	$(CC) -o $(OBJ_dir)/$@ $< -I include $(CFLAGS) $(LDFLAGS) 

clean:
	@rm -f $(addprefix $(OBJ_dir)/,$(OBJETOS))
	@rm -f $(addprefix $(BIN_dir)/aircraft)
