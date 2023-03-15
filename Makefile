VPATH=./src/
EXEC=main
OBJDIR=./obj/
CPP=g++
OPTS=-Ofast
LDFLAGS= -lm -pthread 
COMMON=-Isrc/
CFLAGS=-Wall -Wno-unused-result -Wno-unknown-pragmas -Wfatal-errors -fPIC
CFLAGS+=$(OPTS)
OBJ=hardware.o platform.o software.o inst.o
EXECOBJA=main.o hardware.o platform.o software.o inst.o
EXECOBJ = $(addprefix $(OBJDIR), $(EXECOBJA))
OBJS = $(addprefix $(OBJDIR), $(OBJ))
all: obj backup results $(EXEC) $(OBJDIR)
$(EXEC): $(EXECOBJ)
	$(CPP) $(COMMON) $(CFLAGS) $^ -o $@ $(LDFLAGS)
$(OBJDIR)%.o: %.cpp
	$(CPP) $(COMMON) $(CFLAGS) -c $< -o $@
$(OBJDIR)%.o: %.c
	$(CPP) $(COMMON) $(CFLAGS) -c $< -o $@
obj:
	mkdir -p obj
backup:
	mkdir -p backup
results:
	mkdir -p results
.PHONY: clean
clean:
	rm -rf $(OBJS) $(EXEC) $(EXECOBJ) $(OBJDIR)*

