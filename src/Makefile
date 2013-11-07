CXXFLAGS = -Wall -g

OBJECTS = Instance.o Engine.o

default:	test1 example

test1:	test1.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

example:	example.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f test1 test1.o example example.o $(OBJECTS) *~

Instance.o: Instance.cpp Instance.h PtrInterface.h Ptr.h Engine.h Engine.cpp
test1.o: test1.cpp Instance.h PtrInterface.h Ptr.h
Engine.o: Engine.cpp Engine.h
