# All Targets
all: spl

# Tool invocations
# Executable "hello" depends on the files hello.o and run.o.
spl: bin/Main2.o bin/User.o bin/Watchable.o  bin/Action.o bin/Session.o
	@echo 'Building target: spl'
	@echo 'Invoking: C++ Linker'
	g++ -o bin/splflix bin/Main2.o  bin/User.o bin/Watchable.o  bin/Action.o bin/Session.o
	@echo 'Finished building target: spl'
	@echo ' '


# Depends on the source and header files 
bin/Main.o: src/Main.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Main.o src/Main.cpp
bin/User.o: src/User.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/User.o src/User.cpp
bin/Action.o: src/Action.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Action.o src/Action.cpp
bin/Watchable.o: src/Watchable.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Watchable.o src/Watchable.cpp
bin/Session.o: src/Session.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Session.o src/Session.cpp
bin/Main2.o: src/Main2.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Main2.o src/Main2.cpp

#Clean the build directory
clean: 
	rm -f bin/*
