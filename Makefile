CC = g++ -std=c++11
CCFLAGS = -g -Wall -pthread
INCLUDES = 
LIBRARIES = 
EXECUTABLES = coarse fine


# Command for creating an executable file
.cpp:
	$(CC) $(CCFLAGS) $(INCLUDES) -o $@ $@.cpp $(LIBRARIES)

# Rule for generating .o file from .c file
%.o: %.cpp
	$(CC) $(CCFLAGS) $(INCLUDES) -c $^ 

# All files to be generated
all: $(EXECUTABLES)

# Clean the directory
clean: 
	rm -rf $(EXECUTABLES) $(RPCGENERATED) *.o *.dSYM
