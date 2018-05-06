# List of all the STM32F3 platform files.
USERSRC += $(USERLIB)/ports/STM32F3/draw_lld.c \
           $(USERLIB)/ports/STM32F3/video3d_lld.c
                               
# Required include directories
USERINC += $(USERLIB)/ports/STM32F3/

# Shared variables
ALLCSRC += $(USERSRC)
ALLINC  += $(USERINC)