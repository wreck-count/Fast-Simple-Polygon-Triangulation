VERSION = 0.1
CPP     = g++
CPPFLAGS  = -Wall -O2 -std=c++11 -fpermissive
LDFLAGS = -lm 

OBJ = main.o utils.o data_structure.o

make: $(OBJ)
	$(CPP) -o dcel_triangulate $(OBJ) $(LDFLAGS)	

%.o: %.cpp
	$(CPP) $(CPPFLAGS) -c $<

clean:
	rm *.o dcel_triangulate

