#OUTPUT folder for debug compile
OUT=dist/Debug/GNU-Linux-x86
#OUTPUT folder for release compile
#OUT=dist/Release/GNU-Linux-x86
debug=-g
FLAGS=-pthread -lmongoclient -lboost_thread-mt -lboost_filesystem -lboost_program_options -lboost_system
exec="mongodb-tester"
SRC=src/

$(exec): $(SRC)mongo-tester.cpp $(SRC)stopWatch.cpp $(SRC)config.cpp
	mkdir -p $(OUT)
	g++ $^ $(debug) -O2 $(FLAGS) -o ./$(OUT)/$@

clean:
	rm -rf dist

.PHONY:	clean

