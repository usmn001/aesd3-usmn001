#******************************************************************************
# 
#
#------------------------------------------------------------------------------

TARGET = aesdsocket	

all : $(TARGET)
#Common Compiler Flag
CFLAGS  = 	-g        \
			-std=c99  \
			-Wall     \
			-O0

SOURCES := aesdsocket.c
	
CROSS_COMPILE ?=
CC := $(CROSS_COMPILE)gcc

ifeq ($(CROSS_COMPILE),)
CFLAGS += -DHOST
endif
      
# Generating Required Object Files For Each Source File

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@ 

OBJS = $(SOURCES:.c=.o)


# Compile All Source Files Into Object Files But Do Not Link
.PHONY : compile-all
compile-all : $(OBJS)

# Compile all object files and link into a final executable , Produce map file of final build and Provide Information Of The Data Section Sizes.
#.PHONY : build
#build : $(TARGET)
         

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@
	size $@

#print:
#	@echo CC=$(CC)

# Cleaning All Generated Files
.PHONY : clean
clean : 
	rm -f $(OBJS) $(TARGET) 
			



