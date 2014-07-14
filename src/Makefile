
CXX = g++
CXXFLAGS = 

# Main target
all: capture_timelapse

# Main object files
capture_timelapse.o: capture_timelapse.cpp
	$(CXX) -c -o capture_timelapse.o capture_timelapse.cpp $(CXXFLAGS) `pkg-config opencv --cflags`

# Main executable
capture_timelapse: capture_timelapse.o
	$(CXX) -o capture_timelapse capture_timelapse.o $(CXXFLAGS) `pkg-config opencv --libs`


# Cleanup
clean:
	rm -f *.o

realclean: clean
	rm -f capture_timelapse
