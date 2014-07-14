
CXX = g++
CXXFLAGS = 

# Main target
all: capture_timelapse

# Main object files
main.o: main.cpp
	$(CXX) -c -o main.o main.cpp $(CXXFLAGS) `pkg-config opencv --cflags`

# Main executable
capture_timelapse: main.o
	$(CXX) -o capture_timelapse main.o $(CXXFLAGS) `pkg-config opencv --libs`


# Cleanup
clean:
	rm -f *.o

realclean: clean
	rm -f capture_timelapse
