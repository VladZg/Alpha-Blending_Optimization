TARGET = main
FLAGS = -O3 -mavx2 #-mavx512vl

OBJ_DIR = ./Object
SRC_DIR = ./Source

LIBS = -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system

all: compile link run

compile:
	@g++ -c $(TARGET).cpp $(FLAGS) -I/usr/include/ -o $(OBJ_DIR)/$(TARGET).o
	@g++ -c $(SRC_DIR)/AppUtils.cpp  $(FLAGS) -I/usr/include/ -o $(OBJ_DIR)/AppUtils.o

link:
	@g++ $(OBJ_DIR)/$(TARGET).o $(OBJ_DIR)/AppUtils.o -o $(TARGET) $(LIBS)

run:
	@./$(TARGET)

clean:
	@rm -f $(OBJ_DIR)/*.o
