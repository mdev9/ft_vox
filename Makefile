CC = g++
CFLAGS = -std=c++11 -g -Wextra -Werror -Wall -Iinclude -Ilib/stb -Ilib/glm
LDFLAGS = -lglfw -lGL -lGLEW -lm

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = .
INC_DIR = include

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

EXEC = $(BIN_DIR)/ft_vox

$(EXEC): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(OBJS) -o $(EXEC) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(INC_DIR)/%.hpp
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean fclean re

clean:
	rm -rf $(OBJ_DIR)

fclean:
	rm -rf $(OBJ_DIR) $(EXEC)

re: fclean $(EXEC)
