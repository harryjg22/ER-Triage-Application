CXX = g++
CXXFLAGS = -g -Wall

proj3: 	 pqueue.o mytest.cpp
	$(CXX) $(CXXFLAGS) pqueue.o mytest.cpp -o proj3

pqueue.o: pqueue.h pqueue.cpp
	$(CXX) $(CXXFLAGS) -c pqueue.cpp

clean:
	rm *.o*
	rm *~ 

run:
	./proj3

val:
	valgrind -s --track-origins=yes --leak-check=full ./proj3

debug:
	gdb ./proj3