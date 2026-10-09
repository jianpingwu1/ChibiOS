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
 * @file    RTCv2/hal_rtc_lld.h
 * @brief   GD32 RTC low level driver header.
 *
 * @addtogroup RTC
 * @{
 */

#ifndef HAL_RTC_LLD_H
#define HAL_RTC_LLD_H

#if HAL_USE_RTC || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    Implementation capabilities
 * @{
 */
/**
 * @brief   Callback support in the driver.
 */
#define RTC_SUPPORTS_CALLBACKS      TRUE

/**
 * @brief   Number of alarms available.
 */
#define RTC_ALARMS                  GD32_RTC_NUM_ALARMS

/**
 * @brief   Presence of a local persistent storage.
 */
#define RTC_HAS_STORAGE             (GD32_RTC_STORAGE_SIZE > 0)
/** @} */

/**
 * @brief   RTC_PSC register initializer.
 * @note    Prefixed in order to not collide with the RTC_PSC register
 *          definition.
 *
 * @param[in] a         asynchronous prescaler factor
 * @param[in] s         synchronous prescaler factor
 */
#define GD32_RTC_PSC(a, s)          ((((a) - 1) << 16) | ((s) - 1))

/**
 * @name    Alarm helper macros
 * @{
 */
#define RTC_ALRM_MSKD               (1U << 31)
#define RTC_ALRM_DOWS               (1U << 30)
#define RTC_ALRM_DAYT(n)            ((n) << 28)
#define RTC_ALRM_DAYU(n)            ((n) << 24)
#define RTC_ALRM_MSKH               (1U << 23)
#define RTC_ALRM_HRT(n)             ((n) << 20)
#define RTC_ALRM_HRU(n)             ((n) << 16)
#define RTC_ALRM_MSKM               (1U << 15)
#define RTC_ALRM_MNT(n)             ((n) << 12)
#define RTC_ALRM_MNU(n)             ((n) << 8)
#define RTC_ALRM_MSKS               (1U << 7)
#define RTC_ALRM_SCT(n)             ((n) << 4)
#define RTC_ALRM_SCU(n)             ((n) << 0)
/** @} */

/* Requires services from the EXTI driver.*/
#if !defined(GD32_EXTI_REQUIRED)
#define GD32_EXTI_REQUIRED
#endif

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   RTC asynchronous prescaler initialization.
 * @note    The default is calculated for a 32768Hz clock.
 */
#if !defined(GD32_RTC_PRESA_VALUE) || defined(__DOXYGEN__)
#define GD32_RTC_PRESA_VALUE                32
#endif

/**
 * @brief   RTC synchronous prescaler initialization.
 * @note    The default is calculated for a 32768Hz clock.
 */
#if !defined(GD32_RTC_PRESS_VALUE) || defined(__DOXYGEN__)
#define GD32_RTC_PRESS_VALUE                1024
#endif

/**
 * @brief   RTC_CTL register initialization value.
 * @note    Use this value to initialize features not directly handled by
 *          the RTC driver.
 */
#if !defined(GD32_RTC_CTL_INIT) || defined(__DOXYGEN__)
#define GD32_RTC_CTL_INIT                   0
#endif

/**
 * @brief   RTC_TAMP register initialization value.
 * @note    Use this value to initialize features not directly handled by
 *          the RTC driver.
 */
#if !defined(GD32_RTC_TAMP_INIT) || defined(__DOXYGEN__)
#define GD32_RTC_TAMP_INIT                  0
#endif

/**
 * @brief   RTC_CFG register initialization value.
 * @note    Selects the RTC_OUT pin and the RTC_ALARM output type.
 */
#if !defined(GD32_RTC_CFG_INIT) || defined(__DOXYGEN__)
#define GD32_RTC_CFG_INIT                   0
#endif

/**
 * @brief   Polling cycles before giving up on an RTC flag.
 */
#if !defined(GD32_RTC_FLAG_TIMEOUT) || defined(__DOXYGEN__)
#define GD32_RTC_FLAG_TIMEOUT               0x00100000U
#endif

/**
 * @brief   RTC alarm interrupt priority level setting.
 */
#if !defined(GD32_IRQ_EXTI17_PRIORITY) || defined(__DOXYGEN__)
#define GD32_IRQ_EXTI17_PRIORITY            15
#endif

/**
 * @brief   RTC tamper and time stamp interrupt priority level setting.
 */
#if !defined(GD32_IRQ_EXTI18_PRIORITY) || defined(__DOXYGEN__)
#define GD32_IRQ_EXTI18_PRIORITY            15
#endif

/**
 * @brief   RTC wakeup interrupt priority level setting.
 */
#if !defined(GD32_IRQ_EXTI19_PRIORITY) || defined(__DOXYGEN__)
#define GD32_IRQ_EXTI19_PRIORITY            15
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if HAL_USE_RTC && !GD32_HAS_RTC
#error "RTC not present in the selected device"
#endif

#if !defined(GD32_RTCCLK)
#error "RTC clock not exported by HAL layer"
#endif

#if GD32_RTCCLK == 0
#error "RTC clock not enabled, check GD32_RTCSEL in mcuconf.h"
#endif

#if GD32_PCLK1 < (GD32_RTCCLK * 7)
#error "GD32_PCLK1 frequency is too low"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(GD32_IRQ_EXTI17_PRIORITY)
#error "invalid GD32_IRQ_EXTI17_PRIORITY value specified"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(GD32_IRQ_EXTI18_PRIORITY)
#error "invalid GD32_IRQ_EXTI18_PRIORITY value specified"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(GD32_IRQ_EXTI19_PRIORITY)
#error "invalid GD32_IRQ_EXTI19_PRIORITY value specified"
#endif

/**
 * @brief   Initialization for the RTC_PSC register.
 */
#define GD32_RTC_PSC_BITS                   GD32_RTC_PSC(GD32_RTC_PRESA_VALUE, \
                                                         GD32_RTC_PRESS_VALUE)

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of an RTC event.
 */
typedef enum {
  RTC_EVENT_ALARM_A     = 0,            /** Alarm 0.                        */
  RTC_EVENT_ALARM_B     = 1,            /** Alarm 1.                        */
  RTC_EVENT_TS          = 2,            /** Time stamp.                     */
  RTC_EVENT_TS_OVF      = 3,            /** Time stamp overflow.            */
  RTC_EVENT_TAMP1       = 4,            /** Tamper 0.                       */
  RTC_EVENT_TAMP2       = 5,            /** Tamper 1.                       */
  RTC_EVENT_WAKEUP      = 7             /** Wakeup.                         */
 } rtcevent_t;

/**
 * @brief   Type of a generic RTC callback.
 */
typedef void (*rtccb_t)(RTCDriver *rtcp, rtcevent_t event);

/**
 * @brief   Type of a structure representing an RTC alarm time stamp.
 */
typedef struct hal_rtc_alarm {
  /**
   * @brief   Alarm as encoded in the RTC_ALRMxTD registers.
   */
  uint32_t                  alrmtd;
} RTCAlarm;

#if GD32_RTC_HAS_PERIODIC_WAKEUPS || defined(__DOXYGEN__)
/**
 * @brief   Type of a wakeup as encoded in the RTC_WUT register.
 */
typedef struct hal_rtc_wakeup {
  /**
   * @brief   Wakeup as encoded in the RTC_WUT register.
   * @note    ((WUT == 0) || (WTCS == 3)) is a forbidden combination.
   * @note    Bits 16..18 are copied in the RTC_CTL bits 0..2 (WTCS).
   */
  uint32_t                  wut;
} RTCWakeup;
#endif

/**
 * @brief   Implementation-specific @p RTCDriver fields.
 */
#define rtc_lld_driver_fields                                               \
  /* Callback pointer.*/                                                    \
  rtccb_t                   callback;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void rtc_lld_init(void);
  void rtc_lld_set_time(RTCDriver *rtcp, const RTCDateTime *timespec);
  void rtc_lld_get_time(RTCDriver *rtcp, RTCDateTime *timespec);
#if RTC_SUPPORTS_CALLBACKS == TRUE
  void rtc_lld_set_callback(RTCDriver *rtcp, rtccb_t callback);
#endif
#if RTC_ALARMS > 0
  void rtc_lld_set_alarm(RTCDriver *rtcp,
                         rtcalarm_t alarm,
                         const RTCAlarm *alarmspec);
  void rtc_lld_get_alarm(RTCDriver *rtcp,
                         rtcalarm_t alarm,
                         RTCAlarm *alarmspec);
#endif
#if GD32_RTC_HAS_PERIODIC_WAKEUPS
  void rtcGD32SetPeriodicWakeup(RTCDriver *rtcp, const RTCWakeup *wakeupspec);
  void rtcGD32GetPeriodicWakeup(RTCDriver *rtcp, RTCWakeup *wakeupspec);
#endif /* GD32_RTC_HAS_PERIODIC_WAKEUPS */
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_RTC */

#endif /* HAL_RTC_LLD_H */

/** @} */
