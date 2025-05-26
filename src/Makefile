# name of the program (Minix service)
PROG=proj

VPATH = I_O:classes

# source code files to be compiled
SRCS = main.c \
       game.c \
       timer.c \
       keyboard.c \
       video_card.c \
       utils.c \
       sprite.c \
       car.c \
       player.c \
       tunnel.c \
       lane.c \

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>

