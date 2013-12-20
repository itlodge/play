CXX = gcc
FLAGS = -Wall -m32 -std=gnu99 -o2 -fno-builtin -nostdlib -fno-stack-protector \
	-Wl,-emy_crt_entry -g
LIBS =
 
SRC = entry.c stdio.c string.c heap.c
OBJS = $(SRC:.c=.o)
TARGET = mycrt
 
%.o: %.c
	$(CXX) $(FLAGS) -c $< -o $@
 
all: $(TARGET)
 
$(TARGET): $(OBJS)
	ar -rs mycrt.a $^
 
.PRONY: clean
clean:
	$(RM) $(OBJS) $(TARGET)
