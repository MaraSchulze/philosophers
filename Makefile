NAME = philosophers
FLAGS = -Wall -Wextra -pthread
SOURCES = main.c philosophers.c helper.c threads.c tools.c
OBJECTS = $(SOURCES:.c=.o)


all : $(NAME)

$(NAME) : $(OBJECTS)
	cc $(FLAGS) $(OBJECTS) -g -fsanitize=address -o philosophers  

bonus : 

%.o : %.c
	cc $(FLAGS) -g -fsanitize=address -c $< -o $@

clean :
	rm -f $(OBJECTS)

fclean : clean
	rm -f philosophers
	
re : fclean all
