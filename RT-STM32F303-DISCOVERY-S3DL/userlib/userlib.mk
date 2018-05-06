#Userlib directories
USERLIB = ./userlib

# List of all the Userlib files
USERSRC = $(USERLIB)/src/alphabet.c \
          $(USERLIB)/src/s3dl.c \
          $(USERLIB)/src/opt_math.c \
          $(USERLIB)/src/draw.c  \
          $(USERLIB)/src/games.c \
          $(USERLIB)/src/games_handler.c \
          $(USERLIB)/ports/STM32F3/draw_lld.c \
          $(USERLIB)/ports/STM32F3/video3d_lld.c
          
# Required include directories
USERINC = $(USERLIB) \
          $(USERLIB)/include \
          $(USERLIB)/ports/STM32F3
          
# Shared variables
ALLCSRC += $(USERSRC)
ALLINC  += $(USERINC)