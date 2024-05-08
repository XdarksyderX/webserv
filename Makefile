# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: migarci2 <migarci2@student.42malaga.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/05 16:30:49 by migarci2          #+#    #+#              #
#    Updated: 2024/05/08 17:46:12 by migarci2         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CXX = g++
CXXFLAGS = -Wall -Werror -Wextra -std=c++98

NAME = webserv

SRC_DIR = src
OBJ_DIR = obj
INC_DIR = include

SRC =	$(SRC_DIR)/CGIHandler.cpp $(SRC_DIR)/HTTPCookie.cpp $(SRC_DIR)/HTTPMultiFormData.cpp \
		$(SRC_DIR)/HTTPRequest.cpp $(SRC_DIR)/HTTPRequestParser.cpp $(SRC_DIR)/HTTPResponseBuilder.cpp \
		$(SRC_DIR)/HTTPResponse.cpp $(SRC_DIR)/HTTPServer.cpp \
		$(SRC_DIR)/LocationConfig.cpp $(SRC_DIR)/Logger.cpp $(SRC_DIR)/main.cpp \
		$(SRC_DIR)/MIMETypes.cpp $(SRC_DIR)/Parser.cpp \
		$(SRC_DIR)/ParserLocation.cpp $(SRC_DIR)/ParserServer.cpp \
		$(SRC_DIR)/ServerConfig.cpp $(SRC_DIR)/ServerManager.cpp $(SRC_DIR)/StatusCodes.cpp \
		$(SRC_DIR)/Time.cpp $(SRC_DIR)/utils.cpp

OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

all: $(NAME)

$(NAME): $(OBJ)
	@$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)
	@echo "webserv compiled"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	@$(CXX) $(CXXFLAGS) -I $(INC_DIR) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
