
OB += \
Traitements.o \
Contours.o
	

%.o:
	g++  -O0 -g3 -Wall -c  Traitements.cpp Contours.cpp `pkg-config --cflags --libs opencv`

all:$(OB)
	@echo 'Building target: $@'
	g++  -o "Contours" $(OB)  `pkg-config --cflags --libs opencv`
	@echo 'Finished building target: $@'

clear:
	rm -rf *.o

mproper: clean
	rm -rf $(OB) fonction

