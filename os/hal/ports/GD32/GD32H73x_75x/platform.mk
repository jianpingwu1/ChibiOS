# Required platform files.
PLATFORMSRC := $(CHIBIOS)/os/hal/ports/common/ARMCMx/nvic.c \
               $(CHIBIOS)/os/hal/ports/GD32/GD32H73x_75x/gd32_isr.c \
               $(CHIBIOS)/os/hal/ports/GD32/GD32H73x_75x/hal_lld.c \
               $(CHIBIOS)/os/hal/ports/GD32/GD32H73x_75x/hal_efl_lld.c

# Required include directories.
PLATFORMINC := $(CHIBIOS)/os/hal/ports/common/ARMCMx \
               $(CHIBIOS)/os/hal/ports/GD32/GD32H73x_75x \
               $(CHIBIOS)/os/hal/ports/GD32/LLD/ADC \
               $(CHIBIOS)/os/hal/ports/GD32/LLD/EXTIv1 \
               $(CHIBIOS)/os/hal/ports/GD32/LLD/I2C \
               $(CHIBIOS)/os/hal/ports/GD32/LLD/I2Cv1 \
               $(CHIBIOS)/os/hal/ports/GD32/LLD/SDIO \
               $(CHIBIOS)/os/hal/ports/GD32/LLD/SDMMCv2 \
               $(CHIBIOS)/os/hal/ports/GD32/LLD/SPI \
               $(CHIBIOS)/os/hal/ports/GD32/LLD/SPIv1 \
               $(CHIBIOS)/os/hal/ports/GD32/LLD/TIMER \
               $(CHIBIOS)/os/hal/ports/GD32/LLD/TIMERv1 \
               $(CHIBIOS)/os/hal/ports/GD32/LLD/USART

# Optional platform files.
ifeq ($(USE_SMART_BUILD),yes)

# Configuration files directory
ifeq ($(HALCONFDIR),)
  ifeq ($(CONFDIR),)
    HALCONFDIR = .
  else
    HALCONFDIR := $(CONFDIR)
  endif
endif

HALCONF := $(strip $(shell cat $(HALCONFDIR)/halconf.h | egrep -e "\#define"))

else
endif

# Drivers compatible with the platform.
include $(CHIBIOS)/os/hal/ports/GD32/LLD/ADCv1/driver.mk
# include $(CHIBIOS)/os/hal/ports/GD32/LLD/CAUv1/driver.mk
include $(CHIBIOS)/os/hal/ports/GD32/LLD/DACv1/driver.mk
include $(CHIBIOS)/os/hal/ports/GD32/LLD/DMAv2/driver.mk
include $(CHIBIOS)/os/hal/ports/GD32/LLD/EXTIv1/driver.mk
include $(CHIBIOS)/os/hal/ports/GD32/LLD/FDCANv2/driver.mk
include $(CHIBIOS)/os/hal/ports/GD32/LLD/GPIOv1/driver.mk
include $(CHIBIOS)/os/hal/ports/GD32/LLD/I2Cv1/driver.mk
# include $(CHIBIOS)/os/hal/ports/GD32/LLD/MACv1/driver.mk
# include $(CHIBIOS)/os/hal/ports/GD32/LLD/MDMAv1/driver.mk
include $(CHIBIOS)/os/hal/ports/GD32/LLD/USBv1/driver.mk
include $(CHIBIOS)/os/hal/ports/GD32/LLD/SDMMCv2/driver.mk
include $(CHIBIOS)/os/hal/ports/GD32/LLD/SPIv1/driver.mk
# include $(CHIBIOS)/os/hal/ports/GD32/LLD/TRNGv1/driver.mk
include $(CHIBIOS)/os/hal/ports/GD32/LLD/RTCv2/driver.mk
include $(CHIBIOS)/os/hal/ports/GD32/LLD/SYSTICKv1/driver.mk
include $(CHIBIOS)/os/hal/ports/GD32/LLD/TIMERv1/driver.mk
include $(CHIBIOS)/os/hal/ports/GD32/LLD/USARTv1/driver.mk
# include $(CHIBIOS)/os/hal/ports/GD32/LLD/xWDGv1/driver.mk

# Shared variables
ALLCSRC += $(PLATFORMSRC)
ALLINC  += $(PLATFORMINC)
