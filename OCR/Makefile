## Simple SDL mini code

CC=gcc

# Directories
NET_DIR= Network
IMG_DIR= Image

# Sources
COMMON_NET_SRCS= $(NET_DIR)/Core.c $(NET_DIR)/Execute.c $(NET_DIR)/DataSource.c $(NET_DIR)/Display.c $(NET_DIR)/Save.c
COMMON_IMG_SRCS= $(IMG_DIR)/Core.c $(IMG_DIR)/Execute.c

IMG_MAIN= $(IMG_DIR)/main.c
IMG_SRCS= $(COMMON_IMG_SRCS) $(IMG_MAIN)

NET_MAIN= $(NET_DIR)/main.c
NET_SRCS= $(COMMON_NET_SRCS) $(NET_MAIN)

CPPFLAGS= `pkg-config --cflags sdl`
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3
LDFLAGS= -lm
LDLIBS= `pkg-config --libs sdl` -lSDL_image

SRCS= $(COMMON_NET_SRCS) $(COMMON_IMG_SRCS)
OBJS= $(SRCS)

SRC= IMG/Core.c IMG/main.c
OBJ= ${SRC:.c=.o}

all: main

main: ${OBJS}

img: $(IMG_SRCS)
	$(CC) $(CFLAGS) $(LDLIBS) $(CPPFLAGS) $(LDFLAGS) $^ -o $@

net: $(NET_SRCS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

help:
	@echo tap \'make\' or \'make main\'for global project $@
	@echo tap \'make net\' for Network project $@
	@echo tap \'make img\' for Image project $@
	@echo tap \'make clean\' for clean global project $@
	@echo tap \'make img_clean\' for clean Image project $@
	@echo tap \'make net_clean\' for clean Network project $@

img_clean:
	rm -rf $(IMG_DIR)/Object
	rm -f $(IMG_DIR)/*.o
	rm -f img

net_clean:
	rm -rf $(NET_DIR)/Object
	rm -f $(NET_DIR)/*.o
	rm -f net

clean:
	make img_clean
	make net_clean
	rm -f main

# END
