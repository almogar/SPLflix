# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
CC = g++
CFLAGS  = -g -Wall -Weffc++ -std=c++11
LFLAGS  = -L/usr/lib

# All Targets
all: splflix

# Tool invocations
# Executable "splflix" depends on the files Main.o - Action.o - Session.o - Watchable.o - User.o
splflix: bin/Main.o bin/Action.o bin/Session.o bin/Watchable.o bin/User.o
	@echo 'Building target: splflix'
	@echo 'Invoking: C++ Linker'
	$(CC) -o bin/splflix bin/Main.o bin/Action.o bin/Session.o bin/Watchable.o bin/User.o $(LFLAGS)
	@echo 'Finished building target: splflix'
	@echo ' '

# Depends on the source and header files
bin/Main.o: src/Main.cpp
	$(CC) $(CFLAGS) -c -Linclude -o bin/Main.o src/Main.cpp
bin/Action.o: src/Action.cpp
	$(CC) $(CFLAGS) -c -Linclude -o bin/Action.o src/Action.cpp
bin/Session.o: src/Session.cpp
	$(CC) $(CFLAGS) -c -Linclude -o bin/Session.o src/Session.cpp
bin/Watchable.o: src/Watchable.cpp
	$(CC) $(CFLAGS) -c -Linclude -o bin/Watchable.o src/Watchable.cpp
bin/User.o: src/User.cpp
	$(CC) $(CFLAGS) -c -Linclude -o bin/User.o src/User.cpp

#Clean the build directory
clean: 
	rm -f bin/*
