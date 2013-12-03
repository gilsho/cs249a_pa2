CXXFLAGS = -Wall -g

OBJECTS = Instance.o Engine.o ActivityImpl.o NetworkSimulation.o

default:	verification experiment

example:	example.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

verification: verification.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

experiment: experiment.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f experiment experiment.o verification verification.o $(OBJECTS) *~

Instance.o: Instance.cpp Instance.h PtrInterface.h Ptr.h Engine.h Engine.cpp
Engine.o: Engine.cpp Engine.h
ActivityImpl.o: ActivityImpl.h ActivityImpl.cpp Activity.h
NetworkSimulation.o: Engine.o NetworkSimulation.cpp NetworkSimulation.h
verification.o: verification.cpp
experiment.o: experiment.cpp
