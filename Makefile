CC = gcc
C_FLAGS = -Wall -Wextra -pthread

program_NAME = rwp

main_program_C_SRCS = Hw5.c

main_program_C_OBJS = ${main_program_C_SRCS:.c=.o}

all: $(program_NAME)

$(program_NAME): $(main_program_C_OBJS)
	$(CC) $(main_program_C_OBJS) $(C_FLAGS) -o $(program_NAME)

clean: 
	@- rm $(program_NAME)
	@- rm $(main_program_C_OBJS)
