# List of all the board related files.
BOARDSRC = $(CHIBIOS)/os/hal/boards/GD32H759I_EVAL/board.c

# Required include directories
BOARDINC = $(CHIBIOS)/os/hal/boards/GD32H759I_EVAL

# Shared variables
ALLCSRC += $(BOARDSRC)
ALLINC  += $(BOARDINC)
