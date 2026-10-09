ifeq ($(USE_SMART_BUILD),yes)
ifneq ($(findstring HAL_USE_GPT TRUE,$(HALCONF)),)
PLATFORMSRC += $(CHIBIOS)/os/hal/ports/GD32/LLD/TIMERv1/hal_gpt_lld.c
endif
else
PLATFORMSRC += $(CHIBIOS)/os/hal/ports/GD32/LLD/TIMERv1/hal_gpt_lld.c
endif

ifeq ($(USE_SMART_BUILD),yes)
ifneq ($(findstring HAL_USE_PWM TRUE,$(HALCONF)),)
PLATFORMSRC += $(CHIBIOS)/os/hal/ports/GD32/LLD/TIMERv1/hal_pwm_lld.c
endif
else
PLATFORMSRC += $(CHIBIOS)/os/hal/ports/GD32/LLD/TIMERv1/hal_pwm_lld.c
endif

ifeq ($(USE_SMART_BUILD),yes)
ifneq ($(findstring HAL_USE_ICU TRUE,$(HALCONF)),)
PLATFORMSRC += $(CHIBIOS)/os/hal/ports/GD32/LLD/TIMERv1/hal_icu_lld.c
endif
else
PLATFORMSRC += $(CHIBIOS)/os/hal/ports/GD32/LLD/TIMERv1/hal_icu_lld.c
endif

ifeq ($(USE_SMART_BUILD),yes)
ifneq ($(findstring HAL_USE_EICU TRUE,$(HALCONF)),)
PLATFORMSRC += $(CHIBIOS)/os/hal/ports/GD32/LLD/TIMERv1/hal_eicu_lld.c
endif
else
PLATFORMSRC += $(CHIBIOS)/os/hal/ports/GD32/LLD/TIMERv1/hal_eicu_lld.c
endif

PLATFORMINC += $(CHIBIOS)/os/hal/ports/GD32/LLD/TIMER \
               $(CHIBIOS)/os/hal/ports/GD32/LLD/TIMERv1
