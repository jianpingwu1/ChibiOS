/*
    ChibiOS - Copyright (C) 2006-2026 Giovanni Di Sirio.

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/**
 * @file    RTCv2/hal_rtc_lld.c
 * @brief   GD32 RTC low level driver.
 *
 * @addtogroup RTC
 * @{
 */

#include "hal.h"

#if HAL_USE_RTC || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

#define RTC_TIME_HRT_OFFSET                 20
#define RTC_TIME_HRU_OFFSET                 16
#define RTC_TIME_MNT_OFFSET                 12
#define RTC_TIME_MNU_OFFSET                 8
#define RTC_TIME_SCT_OFFSET                 4
#define RTC_TIME_SCU_OFFSET                 0

#define RTC_DATE_YRT_OFFSET                 20
#define RTC_DATE_YRU_OFFSET                 16
#define RTC_DATE_DOW_OFFSET                 13
#define RTC_DATE_MONT_OFFSET                12
#define RTC_DATE_MONU_OFFSET                8
#define RTC_DATE_DAYT_OFFSET                4
#define RTC_DATE_DAYU_OFFSET                0

#define RTC_CTL_DSM_OFFSET                  18

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   RTC driver identifier.
 */
RTCDriver RTCD1;

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Waits for a status flag to be set.
 *
 * @param[in] mask      mask of the flag to be waited for
 * @return              The operation status.
 * @retval false        if the flag has been set.
 * @retval true         if a timeout occurred.
 *
 * @notapi
 */
static bool gd32_rtc_wait_flag(uint32_t mask) {
  uint32_t timeout = GD32_RTC_FLAG_TIMEOUT;

  while ((RTC_STAT & mask) == 0U) {
    if (--timeout == 0U) {
      return true;
    }
  }

  return false;
}

/**
 * @brief   Beginning of configuration procedure.
 *
 * @return              The operation status.
 * @retval false        if the initialization mode has been entered.
 * @retval true         if a timeout occurred.
 *
 * @notapi
 */
static bool gd32_rtc_enter_init(void) {

  if ((RTC_STAT & RTC_STAT_INITF) != 0U) {
    return false;
  }

  RTC_STAT |= RTC_STAT_INITM;

  return gd32_rtc_wait_flag(RTC_STAT_INITF);
}

/**
 * @brief   Finalizing of configuration procedure.
 *
 * @notapi
 */
static inline void gd32_rtc_exit_init(void) {

  RTC_STAT &= ~RTC_STAT_INITM;
}

/**
 * @brief   Converts time from RTC_TIME register encoding to timespec.
 *
 * @param[in] time      RTC_TIME register value
 * @param[out] timespec pointer to a @p RTCDateTime structure
 *
 * @notapi
 */
static void gd32_rtc_decode_time(uint32_t time, RTCDateTime *timespec) {
  uint32_t n;

  n  = ((time >> RTC_TIME_HRT_OFFSET) & 3)   * 36000000;
  n += ((time >> RTC_TIME_HRU_OFFSET) & 15)  * 3600000;
  n += ((time >> RTC_TIME_MNT_OFFSET) & 7)   * 600000;
  n += ((time >> RTC_TIME_MNU_OFFSET) & 15)  * 60000;
  n += ((time >> RTC_TIME_SCT_OFFSET) & 7)   * 10000;
  n += ((time >> RTC_TIME_SCU_OFFSET) & 15)  * 1000;
  timespec->millisecond = n;
}

/**
 * @brief   Converts date from RTC_DATE register encoding to timespec.
 *
 * @param[in] date      RTC_DATE register value
 * @param[out] timespec pointer to a @p RTCDateTime structure
 *
 * @notapi
 */
static void gd32_rtc_decode_date(uint32_t date, RTCDateTime *timespec) {

  timespec->year  = (((date >> RTC_DATE_YRT_OFFSET) & 15) * 10) +
                     ((date >> RTC_DATE_YRU_OFFSET) & 15);
  timespec->month = (((date >> RTC_DATE_MONT_OFFSET) & 1) * 10) +
                     ((date >> RTC_DATE_MONU_OFFSET) & 15);
  timespec->day   = (((date >> RTC_DATE_DAYT_OFFSET) & 3) * 10) +
                     ((date >> RTC_DATE_DAYU_OFFSET) & 15);
  timespec->dayofweek = (date >> RTC_DATE_DOW_OFFSET) & 7;
}

/**
 * @brief   Converts time from timespec to RTC_TIME register encoding.
 *
 * @param[in] timespec  pointer to a @p RTCDateTime structure
 * @return              The RTC_TIME register encoding.
 *
 * @notapi
 */
static uint32_t gd32_rtc_encode_time(const RTCDateTime *timespec) {
  uint32_t n, time = 0;

  /* Subseconds cannot be set.*/
  n = timespec->millisecond / 1000;

  /* Seconds conversion.*/
  time = time | ((n % 10) << RTC_TIME_SCU_OFFSET);
  n /= 10;
  time = time | ((n % 6) << RTC_TIME_SCT_OFFSET);
  n /= 6;

  /* Minutes conversion.*/
  time = time | ((n % 10) << RTC_TIME_MNU_OFFSET);
  n /= 10;
  time = time | ((n % 6) << RTC_TIME_MNT_OFFSET);
  n /= 6;

  /* Hours conversion.*/
  time = time | ((n % 10) << RTC_TIME_HRU_OFFSET);
  n /= 10;
  time = time | (n << RTC_TIME_HRT_OFFSET);

  return time;
}

/**
 * @brief   Converts a date from timespec to RTC_DATE register encoding.
 *
 * @param[in] timespec  pointer to a @p RTCDateTime structure
 * @return              The RTC_DATE register encoding.
 *
 * @notapi
 */
static uint32_t gd32_rtc_encode_date(const RTCDateTime *timespec) {
  uint32_t n, date = 0;

  /* Year conversion, only the last two digits are considered.*/
  n = timespec->year;
  date = date | ((n % 10) << RTC_DATE_YRU_OFFSET);
  n /= 10;
  date = date | ((n % 10) << RTC_DATE_YRT_OFFSET);

  /* Months conversion.*/
  n = timespec->month;
  date = date | ((n % 10) << RTC_DATE_MONU_OFFSET);
  n /= 10;
  date = date | ((n % 10) << RTC_DATE_MONT_OFFSET);

  /* Days conversion.*/
  n = timespec->day;
  date = date | ((n % 10) << RTC_DATE_DAYU_OFFSET);
  n /= 10;
  date = date | ((n % 10) << RTC_DATE_DAYT_OFFSET);

  /* Days of week conversion.*/
  date = date | (timespec->dayofweek << RTC_DATE_DOW_OFFSET);

  return date;
}

#if RTC_HAS_STORAGE == TRUE
static size_t _getsize(void *instance) {

  (void)instance;

  return (size_t)GD32_RTC_STORAGE_SIZE;
}

static ps_error_t _read(void *instance, ps_offset_t offset,
                        size_t n, uint8_t *rp) {
  volatile uint32_t *bkp = &RTC_BKP0;
  unsigned i;

  osalDbgCheck((instance != NULL) && (rp != NULL));
  osalDbgCheck((n > 0U) && (n <= GD32_RTC_STORAGE_SIZE));
  osalDbgCheck((offset < GD32_RTC_STORAGE_SIZE) &&
               (offset + n <= GD32_RTC_STORAGE_SIZE));

  for (i = 0; i < (unsigned)n; i++) {
    unsigned index = ((unsigned)offset + i) / sizeof (uint32_t);
    unsigned shift = ((unsigned)offset + i) % sizeof (uint32_t);
    *rp++ = (uint8_t)(bkp[index] >> (shift * 8U));
  }

  return PS_NO_ERROR;
}

static ps_error_t _write(void *instance, ps_offset_t offset,
                         size_t n, const uint8_t *wp) {
  volatile uint32_t *bkp = &RTC_BKP0;
  unsigned i;

  osalDbgCheck((instance != NULL) && (wp != NULL));
  osalDbgCheck((n > 0U) && (n <= GD32_RTC_STORAGE_SIZE));
  osalDbgCheck((offset < GD32_RTC_STORAGE_SIZE) &&
               (offset + n <= GD32_RTC_STORAGE_SIZE));

  for (i = 0; i < (unsigned)n; i++) {
    unsigned index = ((unsigned)offset + i) / sizeof (uint32_t);
    unsigned shift = ((unsigned)offset + i) % sizeof (uint32_t);
    uint32_t regval = bkp[index];
    regval &= ~(0xFFU << (shift * 8U));
    regval |= (uint32_t)*wp++ << (shift * 8U);
    bkp[index] = regval;
  }

  return PS_NO_ERROR;
}

/**
 * @brief   VMT for the RTC storage file interface.
 */
struct RTCDriverVMT _rtc_lld_vmt = {
  (size_t)0,
  _getsize, _read, _write
};
#endif /* RTC_HAS_STORAGE == TRUE */

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/**
 * @brief   RTC tamper and time stamp interrupt handler.
 * @note    This vector is shared with the LXTAL clock monitor, only the RTC
 *          flags are handled here.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_TAMPER_STAMP_LXTAL_HANDLER) {
  uint32_t stat, clear;

  OSAL_IRQ_PROLOGUE();

  clear = RTC_STAT_TSF | RTC_STAT_TSOVRF | RTC_STAT_TP0F | RTC_STAT_TP1F;

  stat = RTC_STAT;
  RTC_STAT = stat & ~clear;

  extiClearGroup1(EXTI_MASK1(GD32_RTC_TAMP_STAMP_EXTI));

  if (RTCD1.callback != NULL) {
    uint32_t ctl = RTC_CTL;
    uint32_t tamp = RTC_TAMP;

    if ((ctl & RTC_CTL_TSIE) != 0U) {
      if ((stat & RTC_STAT_TSF) != 0U) {
        RTCD1.callback(&RTCD1, RTC_EVENT_TS);
      }
      if ((stat & RTC_STAT_TSOVRF) != 0U) {
        RTCD1.callback(&RTCD1, RTC_EVENT_TS_OVF);
      }
    }

    /* A single interrupt enable bit is shared by both tamper channels.*/
    if ((tamp & RTC_TAMP_TPIE) != 0U) {
      if ((stat & RTC_STAT_TP0F) != 0U) {
        RTCD1.callback(&RTCD1, RTC_EVENT_TAMP1);
      }
      if ((stat & RTC_STAT_TP1F) != 0U) {
        RTCD1.callback(&RTCD1, RTC_EVENT_TAMP2);
      }
    }
  }

  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   RTC wakeup interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_RTC_WKUP_HANDLER) {
  uint32_t stat;

  OSAL_IRQ_PROLOGUE();

  stat = RTC_STAT;
  RTC_STAT = stat & ~RTC_STAT_WTF;

  extiClearGroup1(EXTI_MASK1(GD32_RTC_WKUP_EXTI));

  if (RTCD1.callback != NULL) {
    uint32_t ctl = RTC_CTL;

    if (((ctl & RTC_CTL_WTIE) != 0U) && ((stat & RTC_STAT_WTF) != 0U)) {
      RTCD1.callback(&RTCD1, RTC_EVENT_WAKEUP);
    }
  }

  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   RTC alarm interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_RTC_ALARM_HANDLER) {
  uint32_t stat, clear;

  OSAL_IRQ_PROLOGUE();

  clear = RTC_STAT_ALRM0F | RTC_STAT_ALRM1F;

  stat = RTC_STAT;
  RTC_STAT = stat & ~clear;

  extiClearGroup1(EXTI_MASK1(GD32_RTC_ALARM_EXTI));

  if (RTCD1.callback != NULL) {
    uint32_t ctl = RTC_CTL;

    if (((ctl & RTC_CTL_ALRM0IE) != 0U) && ((stat & RTC_STAT_ALRM0F) != 0U)) {
      RTCD1.callback(&RTCD1, RTC_EVENT_ALARM_A);
    }
    if (((ctl & RTC_CTL_ALRM1IE) != 0U) && ((stat & RTC_STAT_ALRM1F) != 0U)) {
      RTCD1.callback(&RTCD1, RTC_EVENT_ALARM_B);
    }
  }

  OSAL_IRQ_EPILOGUE();
}

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Enable access to registers.
 *
 * @notapi
 */
void rtc_lld_init(void) {

  /* RTC object initialization.*/
  rtcObjectInit(&RTCD1);

  /* Disabling write protection, it is kept disabled while the driver is
     in use.*/
  RTC_WPK = RTC_UNLOCK_KEY1;
  RTC_WPK = RTC_UNLOCK_KEY2;

  /* If the calendar has not been initialized yet then proceed with the
     initial setup.*/
  if ((RTC_STAT & RTC_STAT_YCM) == 0U) {

    if (!gd32_rtc_enter_init()) {
      RTC_CTL  = GD32_RTC_CTL_INIT | RTC_CTL_BPSHAD;
      RTC_TAMP = GD32_RTC_TAMP_INIT;
      RTC_CFG  = GD32_RTC_CFG_INIT;

      /* Clearing all status flags but the initialization request.*/
      RTC_STAT = RTC_STAT_INITM;
      RTC_PSC  = GD32_RTC_PSC_BITS;

      gd32_rtc_exit_init();
    }
    else {
      osalDbgAssert(false, "RTC initialization mode timeout");
    }
  }
  else {
    RTC_STAT &= ~RTC_STAT_RSYNF;
  }

  /* Callback initially disabled.*/
  RTCD1.callback = NULL;

  /* Enabling the RTC-related EXTI lines, they are rising edge sensitive.*/
  extiEnableGroup1(EXTI_MASK1(GD32_RTC_ALARM_EXTI) |
                   EXTI_MASK1(GD32_RTC_TAMP_STAMP_EXTI) |
                   EXTI_MASK1(GD32_RTC_WKUP_EXTI),
                   EXTI_MODE_RISING_EDGE | EXTI_MODE_ACTION_INTERRUPT);

  /* IRQ vectors permanently assigned to this driver.*/
  GD32_RTC_IRQ_ENABLE();
}

/**
 * @brief   Set current time.
 * @note    Fractional part will be silently ignored, there is no possibility
 *          to set it.
 * @note    The function can be called from any context.
 *
 * @param[in] rtcp      pointer to RTC driver structure
 * @param[in] timespec  pointer to a @p RTCDateTime structure
 *
 * @notapi
 */
void rtc_lld_set_time(RTCDriver *rtcp, const RTCDateTime *timespec) {
  uint32_t date, time;
  syssts_t sts;

  (void)rtcp;

  time = gd32_rtc_encode_time(timespec);
  date = gd32_rtc_encode_date(timespec);

  /* Entering a reentrant critical zone.*/
  sts = osalSysGetStatusAndLockX();

  /* Writing the registers.*/
  if (!gd32_rtc_enter_init()) {
    RTC_TIME = time;
    RTC_DATE = date;
    RTC_CTL  = (RTC_CTL & ~(1U << RTC_CTL_DSM_OFFSET)) |
               ((uint32_t)timespec->dstflag << RTC_CTL_DSM_OFFSET);
    gd32_rtc_exit_init();
  }
  else {
    osalDbgAssert(false, "RTC initialization mode timeout");
  }

  /* Leaving a reentrant critical zone.*/
  osalSysRestoreStatusX(sts);
}

/**
 * @brief   Get current time.
 * @note    The function can be called from any context.
 *
 * @param[in] rtcp      pointer to RTC driver structure
 * @param[out] timespec pointer to a @p RTCDateTime structure
 *
 * @notapi
 */
void rtc_lld_get_time(RTCDriver *rtcp, RTCDateTime *timespec) {
  uint32_t ctl, date, time, prev_date, prev_time;
  uint32_t subs;
#if GD32_RTC_HAS_SUBSECONDS
  uint32_t ss, prev_ss;
#endif
  syssts_t sts;

  (void)rtcp;

  /* Entering a reentrant critical zone.*/
  sts = osalSysGetStatusAndLockX();

  /* Repeated registers read until 2 matching sets are found.*/
#if GD32_RTC_HAS_SUBSECONDS
  ss   = 0U;
  time = 0U;
  date = 0U;
  do {
    prev_ss   = ss;
    prev_time = time;
    prev_date = date;
    ss   = RTC_SS;
    time = RTC_TIME;
    date = RTC_DATE;
  } while ((ss != prev_ss) || (time != prev_time) || (date != prev_date));
#else
  time = 0U;
  date = 0U;
  do {
    prev_time = time;
    prev_date = date;
    time = RTC_TIME;
    date = RTC_DATE;
  } while ((time != prev_time) || (date != prev_date));
#endif

  /* The DST bit is in RTC_CTL, no need to poll on this one.*/
  ctl = RTC_CTL;

  /* Leaving a reentrant critical zone.*/
  osalSysRestoreStatusX(sts);

  /* Decoding day time.*/
  gd32_rtc_decode_time(time, timespec);

  /* The sub-second counter is normalized in milliseconds and added to the
     time.*/
#if GD32_RTC_HAS_SUBSECONDS
  subs = (((GD32_RTC_PRESS_VALUE - 1U) - ss) * 1000U) / GD32_RTC_PRESS_VALUE;
#else
  subs = 0;
#endif
  timespec->millisecond += subs;

  /* Decoding date.*/
  gd32_rtc_decode_date(date, timespec);

  /* Retrieving the DST bit.*/
  timespec->dstflag = (ctl >> RTC_CTL_DSM_OFFSET) & 1;
}

#if (RTC_ALARMS > 0) || defined(__DOXYGEN__)
/**
 * @brief   Set alarm time.
 * @note    Default value after backup domain reset is 0 for both comparators.
 * @note    Function does not perform any check of alarm time validity.
 * @note    The function can be called from any context.
 *
 * @param[in] rtcp      pointer to RTC driver structure
 * @param[in] alarm     alarm identifier, can be 0 or 1
 * @param[in] alarmspec pointer to a @p RTCAlarm structure
 *
 * @notapi
 */
void rtc_lld_set_alarm(RTCDriver *rtcp,
                       rtcalarm_t alarm,
                       const RTCAlarm *alarmspec) {
  syssts_t sts;

  (void)rtcp;

  /* Entering a reentrant critical zone.*/
  sts = osalSysGetStatusAndLockX();

  if (alarm == 0) {
    if (alarmspec != NULL) {
      RTC_CTL &= ~RTC_CTL_ALRM0EN;
      if (!gd32_rtc_wait_flag(RTC_STAT_ALRM0WF)) {
        RTC_ALRM0TD = alarmspec->alrmtd;
        RTC_CTL |= RTC_CTL_ALRM0EN;
        RTC_CTL |= RTC_CTL_ALRM0IE;
      }
      else {
        osalDbgAssert(false, "alarm 0 not writable, alarm left disabled");
      }
    }
    else {
      RTC_CTL &= ~RTC_CTL_ALRM0IE;
      RTC_CTL &= ~RTC_CTL_ALRM0EN;
    }
  }
#if RTC_ALARMS > 1
  else {
    if (alarmspec != NULL) {
      RTC_CTL &= ~RTC_CTL_ALRM1EN;
      if (!gd32_rtc_wait_flag(RTC_STAT_ALRM1WF)) {
        RTC_ALRM1TD = alarmspec->alrmtd;
        RTC_CTL |= RTC_CTL_ALRM1EN;
        RTC_CTL |= RTC_CTL_ALRM1IE;
      }
      else {
        osalDbgAssert(false, "alarm 1 not writable, alarm left disabled");
      }
    }
    else {
      RTC_CTL &= ~RTC_CTL_ALRM1IE;
      RTC_CTL &= ~RTC_CTL_ALRM1EN;
    }
  }
#endif /* RTC_ALARMS > 1 */

  /* Leaving a reentrant critical zone.*/
  osalSysRestoreStatusX(sts);
}

/**
 * @brief   Get alarm time.
 * @note    The function can be called from any context.
 *
 * @param[in] rtcp       pointer to RTC driver structure
 * @param[in] alarm      alarm identifier, can be 0 or 1
 * @param[out] alarmspec pointer to a @p RTCAlarm structure
 *
 * @notapi
 */
void rtc_lld_get_alarm(RTCDriver *rtcp,
                       rtcalarm_t alarm,
                       RTCAlarm *alarmspec) {

  (void)rtcp;

  if (alarm == 0) {
    alarmspec->alrmtd = RTC_ALRM0TD;
  }
#if RTC_ALARMS > 1
  else {
    alarmspec->alrmtd = RTC_ALRM1TD;
  }
#endif /* RTC_ALARMS > 1 */
}
#endif /* RTC_ALARMS > 0 */

/**
 * @brief   Enables or disables RTC callbacks.
 * @details This function enables or disables callbacks, use a @p NULL pointer
 *          in order to disable a callback.
 * @note    The function can be called from any context.
 *
 * @param[in] rtcp      pointer to RTC driver structure
 * @param[in] callback  callback function pointer or @p NULL
 *
 * @notapi
 */
void rtc_lld_set_callback(RTCDriver *rtcp, rtccb_t callback) {

  rtcp->callback = callback;
}

#if GD32_RTC_HAS_PERIODIC_WAKEUPS || defined(__DOXYGEN__)
/**
 * @brief   Sets time of periodic wakeup.
 * @note    Default value after backup domain reset is 0x0000FFFF.
 * @note    The function can be called from any context.
 *
 * @param[in] rtcp       pointer to RTC driver structure
 * @param[in] wakeupspec pointer to a @p RTCWakeup structure
 *
 * @api
 */
void rtcGD32SetPeriodicWakeup(RTCDriver *rtcp, const RTCWakeup *wakeupspec) {
  syssts_t sts;

  (void)rtcp;

  /* Entering a reentrant critical zone.*/
  sts = osalSysGetStatusAndLockX();

  if (wakeupspec != NULL) {
    osalDbgCheck(wakeupspec->wut != 0x30000);

    RTC_CTL &= ~RTC_CTL_WTEN;
    RTC_CTL &= ~RTC_CTL_WTIE;
    if (!gd32_rtc_wait_flag(RTC_STAT_WTWF)) {
      RTC_WUT = wakeupspec->wut & RTC_WUT_WTRV;
      RTC_CTL &= ~RTC_CTL_WTCS;
      RTC_CTL |= (wakeupspec->wut >> 16) & RTC_CTL_WTCS;
      RTC_CTL |= RTC_CTL_WTIE;
      RTC_CTL |= RTC_CTL_WTEN;
    }
    else {
      osalDbgAssert(false, "wakeup timer not writable, wakeup left disabled");
    }
  }
  else {
    RTC_CTL &= ~RTC_CTL_WTEN;
    RTC_CTL &= ~RTC_CTL_WTIE;
  }

  /* Leaving a reentrant critical zone.*/
  osalSysRestoreStatusX(sts);
}

/**
 * @brief   Gets time of periodic wakeup.
 * @note    Default value after backup domain reset is 0x0000FFFF.
 * @note    The function can be called from any context.
 *
 * @param[in] rtcp        pointer to RTC driver structure
 * @param[out] wakeupspec pointer to a @p RTCWakeup structure
 *
 * @api
 */
void rtcGD32GetPeriodicWakeup(RTCDriver *rtcp, RTCWakeup *wakeupspec) {
  syssts_t sts;

  (void)rtcp;

  /* Entering a reentrant critical zone.*/
  sts = osalSysGetStatusAndLockX();

  wakeupspec->wut  = 0;
  wakeupspec->wut |= RTC_WUT & RTC_WUT_WTRV;
  wakeupspec->wut |= (RTC_CTL & RTC_CTL_WTCS) << 16;

  /* Leaving a reentrant critical zone.*/
  osalSysRestoreStatusX(sts);
}
#endif /* GD32_RTC_HAS_PERIODIC_WAKEUPS */

#endif /* HAL_USE_RTC */

/** @} */
