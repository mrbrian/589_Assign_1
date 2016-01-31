EXEFILE = myprogram
LIBS = -lglfw -lGLU -lGL
LIBDIRS += -L/usr/X11R6/lib -L/usr/X11R6/lib64 -L/usr/local/lib
INCDIRS += -I/usr/include -I/usr/local/include -I/usr/include/GL


$(EXEFILE): main.cpp
	g++ -o $(EXEFILE) $(LIBDIRS) $(INCDIRS) $(LIBS) main.cpp

clean:
	rm $(EXEFILE)
