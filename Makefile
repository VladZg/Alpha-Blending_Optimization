TARGET = main
FLAGS = -mavx2 #-mavx512vl
OPTIM_FLAGS = -O3

OBJ_DIR = ./Object
SRC_DIR = ./Source

LIBS = -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system

all: compile link run

compile:
	@g++ -c $(TARGET).cpp $(FLAGS) $(OPTIM_FLAGS) -I/usr/include/ -o $(OBJ_DIR)/$(TARGET).o
	@g++ -c $(SRC_DIR)/AppUtils.cpp $(FLAGS) $(OPTIM_FLAGS) -I/usr/include/ -o $(OBJ_DIR)/AppUtils.o
# g++ -c $(SRC_DIR)/IntrinsicsPrintf.cpp $(FLAGS) $(OPTIM_FLAGS) -I/usr/include/ -o $(OBJ_DIR)/IntrinsicsPrintf.o

link:
	g++ $(OBJ_DIR)/AppUtils.o $(OBJ_DIR)/$(TARGET).o -o $(TARGET) $(LIBS)

run:
	@./$(TARGET)

clean:
	@rm -f $(OBJ_DIR)/*.o
