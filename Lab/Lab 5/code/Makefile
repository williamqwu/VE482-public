CC = gcc
CFLAGS = -std=c11 -O2 -Wextra -Werror -pedantic -Wno-unused-result
SRC = list.c sort.c ui.c
HEADER = list.h sort.h ui.h
CMD_SRC = driver_cmd.c
MENU_SRC = driver_menu.c
CMD = ui_cmd
MENU = ui_menu
.PHONY: clean

all: $(CMD) $(MENU)
	@echo driver program successfully constructed

$(CMD): $(SRC) $(CMD_SRC)
	$(CC) $(CFLAGS) -o $(CMD) $(SRC) $(CMD_SRC)

$(MENU): $(SRC) $(MENU_SRC)
	$(CC) $(CFLAGS) -o $(MENU) $(SRC) $(MENU_SRC)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean: 
	$(RM) *.o *.a *~ $(CMD) $(MENU)

cmd:
	$(CC) $(CFLAGS) -o $(CMD) $(SRC) $(CMD_SRC)
	@echo ui_cmd successfully constructed

menu:
	$(CC) $(CFLAGS) -o $(MENU) $(SRC) $(MENU_SRC)
	@echo ui_menu successfully constructed
