#******************************************************************************
# 
#
#------------------------------------------------------------------------------

#Common Compiler Flag
CFLAGS  = 	-g        \
			-std=c99  \
			-Wall     \
			-Werror   \
			-O0
			
CPPFLAGs = -E
SOURCES := writer.c
TARGET = writer
LDFLAGS += -Wl,-Map=$(TARGET).map				
DEPFLAGS = -MD 			

ifeq ($(CROSS_COMPILE),aarch64-none-linux-gnu-)
CC = aarch64-none-linux-gnu-gcc
# Architectures Specific Flags
LD = aarch64-none-linux-gnu-ld

else	

# Compiler Flags and Defines
CC = gcc
LD = ld
CFLAGS += -DHOST

endif


#Generating Dependency Files For Each Source file

%.d : %.c
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@
DEP = $(SOURCES:.c=.d)	


# Generating Preprocessed Output Of Each Source File

%.i : %.c
	$(CC) $(CFLAGS) $(CPPFLAGs) $< -o $@ 

PREP = $(SOURCES:.c=.i)


# Generating Required Assembly Files For Each Source File

%.asm : %.c
	$(CC) $(CFLAGS) -S $<  

ASM = $(SOURCES:.c=.asm)
        
# Generating Required Object Files For Each Source File

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@ 

OBJS = $(SOURCES:.c=.o)


# Compile All Source Files Into Object Files But Do Not Link
.PHONY : compile-all
compile-all : $(OBJS)

# Compile all object files and link into a final executable , Produce map file of final build and Provide Information Of The Data Section Sizes.
.PHONY : build
build : $(TARGET)
         

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $@
	size $@

$(TARGET).asm : $(TARGET)
	objdump -s $(TARGET)

print:
	@echo CC=$(CC)

# Cleaning All Generated Files
.PHONY : clean
clean : 
	rm -f $(OBJS) $(ASM) $(PREP) $(DEP) $(TARGET).out $(TARGET).map
			



