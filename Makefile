COMP = g++
COMPFLAGS = -Wall
 
msh: msh.o
	$(COMP) $(COMPFLAGS) -o msh msh.o
 
msh.o: msh.cpp
	$(COMP) $(COMPFLAGS) -c msh.cpp
 
clean:
	rm -i *.o msh.o
