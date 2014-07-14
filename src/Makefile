
CXX = g++

# Main target
all: capture_timelapse

# Main object files
main.o: main.cpp
	$(CXX) -c -o main.o main.cpp `pkg-config opencv --cflags`

# Main executable
capture_timelapse: main.o
	$(CXX) -o capture_timelapse main.o `pkg-config opencv --libs`


# Cleanup
clean:
	rm -f *.o

realclean: clean
	rm -f capture_timelapse
