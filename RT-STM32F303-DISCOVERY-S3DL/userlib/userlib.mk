#Userlib directories
USERLIB = ./userlib

# List of all the Userlib files
USERSRC = $(USERLIB)/src/alphabet.c \
          $(USERLIB)/src/s3dl.c \
          $(USERLIB)/src/opt_math.c \
          $(USERLIB)/src/draw.c  \
          $(USERLIB)/src/games.c \
          $(USERLIB)/src/games_handler.c

# Required include directories
USERINC = $(USERLIB) \
          $(USERLIB)/include

# Shared variables
ALLCSRC += $(USERSRC)
ALLINC  += $(USERINC)

#Including proper platform          
include $(USERLIB)/ports/STM32F3/platform.mk