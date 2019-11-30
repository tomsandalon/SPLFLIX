# All Targets
all: SPL

# Tool invocations
# Executable "SPL" depends on the files  bin/main.o bin/Action.o bin/Watchable.o bin/User.o bin/Session.o
SPL: bin/main.o bin/Action.o bin/Watchable.o bin/User.o bin/Session.o
    @echo 'Building target: SPL'
    @echo 'Invoking: C++ Linker'
    g++ -o bin/spl bin/main.o bin/Action.o  bin/Watchable.o  bin/User.o  bin/Session.o
    @echo 'Finished building target: SPL'
    @echo ' '

# Depends on the source and header files
bin/main.o: main.cpp
    g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/main.o main.cpp
bin/Session.o: Session.cpp
    g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Session.o Session.cpp
bin/User.o: User.cpp
    g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/User.o User.cpp
bin/Watchable.o: Watchable.cpp
    g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Watchable.o Watchable.cpp
bin/Action.o: Action.cpp
    g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Action.o Action.cpp

#Clean the build directory
clean:
    rm -f bin/*
