CC = gcc
C_FLAGS = -Wall -Wextra -pthread

program_NAME = rwp

helper_C_SRCS = Semaphore.c
main_program_C_SRCS = Hw5.c

helper_C_OBJS = ${helper_C_SRCS:.c=.o}
main_program_C_OBJS = ${main_program_C_SRCS:.c=.o}

all: $(program_NAME)

$(program_NAME): $(main_program_C_OBJS) $(helper_C_OBJS)
	$(CC) $(helper_C_OBJS) $(main_program_C_OBJS) $(C_FLAGS) -o $(program_NAME)

clean: 
	@- rm $(program_NAME)
	@- rm $(helper_C_OBJS) $(main_program_C_OBJS)
