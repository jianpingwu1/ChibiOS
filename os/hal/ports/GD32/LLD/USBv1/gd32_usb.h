/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

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
 * @file    USBv1/gd32_otg.h
 * @brief   GD32 USBHS registers layout header.
 * @note    This file requires definitions from the GD32 header file.
 *
 * @addtogroup USB
 * @{
 */

#ifndef GD32_USB_H
#define GD32_USB_H

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Number of the implemented endpoints in each USBHS core.
 * @details This value does not include the endpoint 0 which is always present.
 */
#define GD32_USBHS_ENDPOINTS                7

/**
 * @brief   Maximum number of the host channels in each USBHS core.
 */
#define GD32_USBHS_CHANNELS                 16

/**
 * @brief   Total number of the TX FIFOs, one per implemented channel.
 */
#define GD32_USBHS_FIFOS                    16

/**
 * @brief   USBHS0 registers block base address.
 */
#define GD32_USBHS0_ADDR                    ((uint32_t)USBHS_BASE)

/**
 * @brief   USBHS1 registers block base address.
 */
#define GD32_USBHS1_ADDR                    ((uint32_t)(USBHS_BASE + 0x00040000U))

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   USBHS host channel registers group.
 */
typedef struct {
  volatile uint32_t HCHCTL;     /**< @brief Host channel control.           */
  volatile uint32_t HCHSTCTL;   /**< @brief Host channel split control.     */
  volatile uint32_t HCHINTF;    /**< @brief Host channel interrupt flag.    */
  volatile uint32_t HCHINTEN;   /**< @brief Host channel interrupt enable.  */
  volatile uint32_t HCHLEN;     /**< @brief Host channel transfer length.   */
  volatile uint32_t HCHDMAADDR; /**< @brief Host channel DMA address.       */
  volatile uint32_t resvd18;
  volatile uint32_t resvd1c;
} gd32_otg_chan_t;

/**
 * @brief   USBHS device IN endpoint registers group.
 */
typedef struct {
  volatile uint32_t DIEPCTL;    /**< @brief IN endpoint control.            */
  volatile uint32_t resvd4;
  volatile uint32_t DIEPINTF;   /**< @brief IN endpoint interrupt flag.     */
  volatile uint32_t resvdC;
  volatile uint32_t DIEPLEN;    /**< @brief IN endpoint transfer length.    */
  volatile uint32_t DIEPDMAADDR;/**< @brief IN endpoint DMA address.        */
  volatile uint32_t DIEPTFSTAT; /**< @brief IN endpoint TX FIFO status.     */
  volatile uint32_t resvd1C;
} gd32_otg_in_ep_t;

/**
 * @brief   USBHS device OUT endpoint registers group.
 */
typedef struct {
  volatile uint32_t DOEPCTL;    /**< @brief OUT endpoint control.           */
  volatile uint32_t resvd4;
  volatile uint32_t DOEPINTF;   /**< @brief OUT endpoint interrupt flag.    */
  volatile uint32_t resvdC;
  volatile uint32_t DOEPLEN;    /**< @brief OUT endpoint transfer length.   */
  volatile uint32_t DOEPDMAADDR;/**< @brief OUT endpoint DMA address.       */
  volatile uint32_t resvd18;
  volatile uint32_t resvd1C;
} gd32_otg_out_ep_t;

/**
 * @brief   USBHS registers block.
 */
typedef struct {
  volatile uint32_t GOTGCS;     /**< @brief Global USB control and status.  */
  volatile uint32_t GOTGINTF;   /**< @brief Global USB interrupt flag.      */
  volatile uint32_t GAHBCS;     /**< @brief Global AHB control and status.  */
  volatile uint32_t GUSBCS;     /**< @brief Global USB control and status.  */
  volatile uint32_t GRSTCTL;    /**< @brief Global reset control.           */
  volatile uint32_t GINTF;      /**< @brief Global interrupt flag.          */
  volatile uint32_t GINTEN;     /**< @brief Global interrupt enable.        */
  volatile uint32_t GRSTATR;    /**< @brief Receive status debug read.      */
  volatile uint32_t GRSTATP;    /**< @brief Receive status and pop.         */
  volatile uint32_t GRFLEN;     /**< @brief Global receive FIFO length.     */
  volatile uint32_t DIEP0TFLEN; /**< @brief IN endpoint 0 TX FIFO length.   */
  volatile uint32_t HNPTFQSTAT; /**< @brief Host non-periodic FIFO status.  */
  volatile uint32_t resvd30[2];
  volatile uint32_t GCCFG;      /**< @brief Global core configuration.      */
  volatile uint32_t CID;        /**< @brief Core ID.                        */
  volatile uint32_t resvd40[5];
  volatile uint32_t GLPMCFG;    /**< @brief LPM configuration.              */
  volatile uint32_t PWRD;       /**< @brief Power down.                     */
  volatile uint32_t resvd5C;
  volatile uint32_t ADPCTL;     /**< @brief ADP control and status.         */
  volatile uint32_t resvd64[3];
  volatile uint32_t PHYCTL;     /**< @brief USBHS PHY control.              */
  volatile uint32_t resvd74[35];
  volatile uint32_t HPTFLEN;    /**< @brief Host periodic TX FIFO length.   */
  volatile uint32_t DIEPTFLEN[15];/**< @brief IN endpoint TX FIFO length.   */
  volatile uint32_t resvd140[176];
  volatile uint32_t HCTL;       /**< @brief Host control.                   */
  volatile uint32_t HFT;        /**< @brief Host frame interval.            */
  volatile uint32_t HFINFR;     /**< @brief Host frame information.         */
  volatile uint32_t resvd40C;
  volatile uint32_t HPTFQSTAT;  /**< @brief Host periodic FIFO status.      */
  volatile uint32_t HACHINT;    /**< @brief Host all channels interrupt.    */
  volatile uint32_t HACHINTEN;  /**< @brief Host all channels int. enable.  */
  volatile uint32_t resvd41C[9];
  volatile uint32_t HPCS;       /**< @brief Host port control and status.   */
  volatile uint32_t resvd444[47];
  gd32_otg_chan_t   hc[16];     /**< @brief Host channels.                  */
  volatile uint32_t resvd700[64];
  volatile uint32_t DCFG;       /**< @brief Device configuration.           */
  volatile uint32_t DCTL;       /**< @brief Device control.                 */
  volatile uint32_t DSTAT;      /**< @brief Device status.                  */
  volatile uint32_t resvd80C;
  volatile uint32_t DIEPINTEN;  /**< @brief IN endpoint common int. enable. */
  volatile uint32_t DOEPINTEN;  /**< @brief OUT endpoint common int. enable.*/
  volatile uint32_t DAEPINT;    /**< @brief All endpoints interrupt.        */
  volatile uint32_t DAEPINTEN;  /**< @brief All endpoints interrupt enable. */
  volatile uint32_t resvd820[2];
  volatile uint32_t DVBUSDT;    /**< @brief VBUS discharge time.            */
  volatile uint32_t DVBUSPT;    /**< @brief VBUS pulsing time.              */
  volatile uint32_t resvd830;
  volatile uint32_t DIEPFEINTEN;/**< @brief IN endpoint FIFO empty enable.  */
  volatile uint32_t DEP1INT;    /**< @brief Endpoint 1 interrupt.           */
  volatile uint32_t DEP1INTEN;  /**< @brief Endpoint 1 interrupt enable.    */
  volatile uint32_t resvd840;
  volatile uint32_t DIEP1INTEN; /**< @brief IN endpoint 1 interrupt enable. */
  volatile uint32_t resvd848[15];
  volatile uint32_t DOEP1INTEN; /**< @brief OUT endpoint 1 interrupt enable.*/
  volatile uint32_t resvd888[30];
  gd32_otg_in_ep_t  ie[8];      /**< @brief Device IN endpoints.            */
  volatile uint32_t resvdA00[64];
  gd32_otg_out_ep_t oe[8];      /**< @brief Device OUT endpoints.           */
  volatile uint32_t resvdC00[128];
  volatile uint32_t PWRCLKCTL;  /**< @brief Power and clock control.        */
  volatile uint32_t resvdE04[127];
  volatile uint32_t FIFO[16][1024];/**< @brief Data FIFO access windows.    */
} gd32_otg_t;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @name    GOTGCS register bit definitions
 * @{
 */
#define GOTGCS_BSV              (1U << 19) /**< B-session valid.            */
#define GOTGCS_ASV              (1U << 18) /**< A-session valid.            */
#define GOTGCS_DI               (1U << 17) /**< Debounce interval.          */
#define GOTGCS_IDPS             (1U << 16) /**< ID pin status.              */
#define GOTGCS_EHE              (1U << 12) /**< Embedded host enable.       */
#define GOTGCS_DHNPEN           (1U << 11) /**< Device HNP enable.          */
#define GOTGCS_HHNPEN           (1U << 10) /**< Host HNP enable.            */
#define GOTGCS_HNPREQ           (1U << 9)  /**< HNP request.                */
#define GOTGCS_HNPS             (1U << 8)  /**< HNP success.                */
#define GOTGCS_BVOV             (1U << 7)  /**< B-session override value.   */
#define GOTGCS_BVOE             (1U << 6)  /**< B-session override enable.  */
#define GOTGCS_AVOV             (1U << 5)  /**< A-session override value.   */
#define GOTGCS_AVOE             (1U << 4)  /**< A-session override enable.  */
#define GOTGCS_VOV              (1U << 3)  /**< VBUS override value.        */
#define GOTGCS_VOE              (1U << 2)  /**< VBUS override enable.       */
#define GOTGCS_SRPREQ           (1U << 1)  /**< SRP request.                */
#define GOTGCS_SRPS             (1U << 0)  /**< SRP success.                */
/** @} */

/**
 * @name    GOTGINTF register bit definitions
 * @{
 */
#define GOTGINTF_IDCHG          (1U << 20) /**< ID input change.            */
#define GOTGINTF_DF             (1U << 19) /**< Debounce finish.            */
#define GOTGINTF_ADTO           (1U << 18) /**< A-device timeout.           */
#define GOTGINTF_HNPDET         (1U << 17) /**< HNP request detected.       */
#define GOTGINTF_HNPEND         (1U << 9)  /**< HNP end.                    */
#define GOTGINTF_SRPEND         (1U << 8)  /**< SRP end.                    */
#define GOTGINTF_SESEND         (1U << 2)  /**< Session end.                */
/** @} */

/**
 * @name    GAHBCS register bit definitions
 * @{
 */
#define GAHBCS_PTXFTH           (1U << 8)  /**< Periodic TX FIFO threshold. */
#define GAHBCS_TXFTH            (1U << 7)  /**< TX FIFO threshold.          */
#define GAHBCS_DMAEN            (1U << 5)  /**< DMA function enable.        */
#define GAHBCS_BURST_MASK       (15U << 1) /**< DMA AHB burst type mask.    */
#define GAHBCS_BURST(n)         ((n) << 1) /**< DMA AHB burst type field.   */
#define GAHBCS_GINTEN           (1U << 0)  /**< Global interrupt enable.    */
/** @} */

/**
 * @name    GUSBCS register bit definitions
 * @{
 */
#define GUSBCS_FDM              (1U << 30) /**< Force device mode.          */
#define GUSBCS_FHM              (1U << 29) /**< Force host mode.            */
#define GUSBCS_ULPIEOI          (1U << 21) /**< ULPI over-current indicator.*/
#define GUSBCS_ULPIEVD          (1U << 20) /**< ULPI external VBUS driver.  */
#define GUSBCS_UTT_MASK         (15U << 10)/**< Turnaround time mask.       */
#define GUSBCS_UTT(n)           ((n) << 10)/**< Turnaround time field.      */
#define GUSBCS_HNPCEN           (1U << 9)  /**< HNP capability enable.      */
#define GUSBCS_SRPCEN           (1U << 8)  /**< SRP capability enable.      */
#define GUSBCS_EMBPHY_FS        (1U << 6)  /**< Embedded FS PHY selected.   */
#define GUSBCS_EMBPHY_HS        (1U << 5)  /**< Embedded HS PHY selected.   */
#define GUSBCS_HS_CUR_FE        (1U << 4)  /**< HS current software enable. */
#define GUSBCS_TOC_MASK         (7U << 0)  /**< Timeout calibration mask.   */
#define GUSBCS_TOC(n)           ((n) << 0) /**< Timeout calibration field.  */
/** @} */

/**
 * @name    GRSTCTL register bit definitions
 * @{
 */
#define GRSTCTL_DMAIDL          (1U << 31) /**< DMA idle state.             */
#define GRSTCTL_DMABSY          (1U << 30) /**< DMA busy.                   */
#define GRSTCTL_TXFNUM_MASK     (31U << 6) /**< TX FIFO number mask.        */
#define GRSTCTL_TXFNUM(n)       ((n) << 6) /**< TX FIFO number field.       */
#define GRSTCTL_TXFF            (1U << 5)  /**< TX FIFO flush.              */
#define GRSTCTL_RXFF            (1U << 4)  /**< RX FIFO flush.              */
#define GRSTCTL_HFCRST          (1U << 2)  /**< Host frame counter reset.   */
#define GRSTCTL_HCSRST          (1U << 1)  /**< HCLK soft reset.            */
#define GRSTCTL_CSRST           (1U << 0)  /**< Core soft reset.            */
/** @} */

/**
 * @name    GINTF register bit definitions
 * @{
 */
#define GINTF_WKUPIF            (1U << 31) /**< Wakeup.                     */
#define GINTF_SESIF             (1U << 30) /**< Session.                    */
#define GINTF_DISCIF            (1U << 29) /**< Disconnect.                 */
#define GINTF_IDPSC             (1U << 28) /**< ID pin status change.       */
#define GINTF_LPMIF             (1U << 27) /**< LPM.                        */
#define GINTF_PTXFEIF           (1U << 26) /**< Periodic TX FIFO empty.     */
#define GINTF_HCIF              (1U << 25) /**< Host channels.              */
#define GINTF_HPIF              (1U << 24) /**< Host port.                  */
#define GINTF_ISOONCIF          (1U << 21) /**< ISO OUT not complete.       */
#define GINTF_ISOINCIF          (1U << 20) /**< ISO IN not complete.        */
#define GINTF_OEPIF             (1U << 19) /**< OUT endpoints.              */
#define GINTF_IEPIF             (1U << 18) /**< IN endpoints.               */
#define GINTF_EOPFIF            (1U << 15) /**< End of periodic frame.      */
#define GINTF_ISOOPDIF          (1U << 14) /**< ISO OUT packet dropped.     */
#define GINTF_ENUMFIF           (1U << 13) /**< Enumeration finished.       */
#define GINTF_RST               (1U << 12) /**< USB reset.                  */
#define GINTF_SP                (1U << 11) /**< USB suspend.                */
#define GINTF_ESP               (1U << 10) /**< Early suspend.              */
#define GINTF_GONAK             (1U << 7)  /**< Global OUT NAK effective.   */
#define GINTF_GNPINAK           (1U << 6)  /**< Global IN NAK effective.    */
#define GINTF_NPTXFEIF          (1U << 5)  /**< Non-periodic TX FIFO empty. */
#define GINTF_RXFNEIF           (1U << 4)  /**< RX FIFO non-empty.          */
#define GINTF_SOF               (1U << 3)  /**< Start of frame.             */
#define GINTF_OTGIF             (1U << 2)  /**< USB.                        */
#define GINTF_MFIF              (1U << 1)  /**< Mode fault.                 */
#define GINTF_COPM              (1U << 0)  /**< Current operation mode.     */
/** @} */

/**
 * @name    GINTEN register bit definitions
 * @{
 */
#define GINTEN_WKUPIE           (1U << 31) /**< Wakeup.                     */
#define GINTEN_SESIE            (1U << 30) /**< Session.                    */
#define GINTEN_DISCIE           (1U << 29) /**< Disconnect.                 */
#define GINTEN_IDPSCIE          (1U << 28) /**< ID pin status change.       */
#define GINTEN_LPMIE            (1U << 27) /**< LPM.                        */
#define GINTEN_PTXFEIE          (1U << 26) /**< Periodic TX FIFO empty.     */
#define GINTEN_HCIE             (1U << 25) /**< Host channels.              */
#define GINTEN_HPIE             (1U << 24) /**< Host port.                  */
#define GINTEN_ISOONCIE         (1U << 21) /**< ISO OUT not complete.       */
#define GINTEN_ISOINCIE         (1U << 20) /**< ISO IN not complete.        */
#define GINTEN_OEPIE            (1U << 19) /**< OUT endpoints.              */
#define GINTEN_IEPIE            (1U << 18) /**< IN endpoints.               */
#define GINTEN_EOPFIE           (1U << 15) /**< End of periodic frame.      */
#define GINTEN_ISOOPDIE         (1U << 14) /**< ISO OUT packet dropped.     */
#define GINTEN_ENUMFIE          (1U << 13) /**< Enumeration finished.       */
#define GINTEN_RSTIE            (1U << 12) /**< USB reset.                  */
#define GINTEN_SPIE             (1U << 11) /**< USB suspend.                */
#define GINTEN_ESPIE            (1U << 10) /**< Early suspend.              */
#define GINTEN_GONAKIE          (1U << 7)  /**< Global OUT NAK effective.   */
#define GINTEN_GNPINAKIE        (1U << 6)  /**< Global IN NAK effective.    */
#define GINTEN_NPTXFEIE         (1U << 5)  /**< Non-periodic TX FIFO empty. */
#define GINTEN_RXFNEIE          (1U << 4)  /**< RX FIFO non-empty.          */
#define GINTEN_SOFIE            (1U << 3)  /**< Start of frame.             */
#define GINTEN_OTGIE            (1U << 2)  /**< OTG.                        */
#define GINTEN_MFIE             (1U << 1)  /**< Mode fault.                 */
/** @} */

/**
 * @name    GRSTATR/GRSTATP register bit definitions
 * @{
 */
#define GRSTATP_RPCKST_MASK     (15U << 17)/**< Received packet status mask.*/
#define GRSTATP_RPCKST_OFF      17         /**< Packet status offset.       */
#define GRSTATP_OUT_GLOBAL_NAK  (1U << 17) /**< Global OUT NAK.             */
#define GRSTATP_OUT_DATA        (2U << 17) /**< OUT data packet received.   */
#define GRSTATP_OUT_COMP        (3U << 17) /**< OUT transfer completed.     */
#define GRSTATP_SETUP_COMP      (4U << 17) /**< SETUP transaction completed.*/
#define GRSTATP_SETUP_DATA      (6U << 17) /**< SETUP data packet received. */
#define GRSTATP_DPID_MASK       (3U << 15) /**< Data PID mask.              */
#define GRSTATP_DPID_OFF        15         /**< Data PID offset.            */
#define GRSTATP_BCOUNT_MASK     (0x7FFU << 4)/**< Byte count mask.          */
#define GRSTATP_BCOUNT_OFF      4          /**< Byte count offset.          */
#define GRSTATP_EPNUM_MASK      (15U << 0) /**< Endpoint number mask.       */
#define GRSTATP_EPNUM_OFF       0          /**< Endpoint number offset.     */
/** @} */

/**
 * @name    GRFLEN register bit definitions
 * @{
 */
#define GRFLEN_RXFD_MASK        (0xFFFFU)  /**< RX FIFO depth mask.         */
#define GRFLEN_RXFD(n)          ((n) << 0) /**< RX FIFO depth field.        */
/** @} */

/**
 * @name    DIEP0TFLEN register bit definitions
 * @{
 */
#define DIEP0TFLEN_IEP0TXFD_MASK    (0xFFFFU << 16)/**< FIFO depth mask.    */
#define DIEP0TFLEN_IEP0TXFD(n)      ((n) << 16)    /**< FIFO depth field.   */
#define DIEP0TFLEN_IEP0TXRSAR_MASK  (0xFFFFU << 0) /**< RAM address mask.   */
#define DIEP0TFLEN_IEP0TXRSAR(n)    ((n) << 0)     /**< RAM address field.  */
/** @} */

/**
 * @name    DIEPTFLEN register bit definitions
 * @{
 */
#define DIEPTFLEN_IEPTXFD_MASK      (0xFFFFU << 16)/**< FIFO depth mask.    */
#define DIEPTFLEN_IEPTXFD(n)        ((n) << 16)    /**< FIFO depth field.   */
#define DIEPTFLEN_IEPTXRSAR_MASK    (0xFFFFU << 0) /**< RAM address mask.   */
#define DIEPTFLEN_IEPTXRSAR(n)      ((n) << 0)     /**< RAM address field.  */
/** @} */

/**
 * @name    GCCFG register bit definitions
 * @{
 */
#define GCCFG_VDEN              (1U << 21) /**< VBUS sensing enable.        */
#define GCCFG_SOFOEN            (1U << 20) /**< SOF output enable.          */
#define GCCFG_PWRON             (1U << 16) /**< Transceiver power on.       */
#define GCCFG_SDMEN             (1U << 15) /**< Secondary detection enable. */
#define GCCFG_PEMEN             (1U << 14) /**< Primary detection enable.   */
#define GCCFG_DCDEN             (1U << 13) /**< Data connect detection.     */
#define GCCFG_BCDEN             (1U << 12) /**< Battery charging detection. */
/** @} */

/**
 * @name    PHYCTL register bit definitions
 * @{
 */
#define PHYCTL_HS_CALEN         (1U << 31) /**< HS PHY calibration enable.  */
#define PHYCTL_HS_CALR_MASK     (0x3FU << 25)/**< HS PHY calibration mask.  */
#define PHYCTL_HS_RCALR_MASK    (15U << 21)/**< HS PHY R calibration mask.  */
#define PHYCTL_SQUECH           (1U << 16) /**< HS PHY squelch flag.        */
/** @} */

/**
 * @name    DCFG register bit definitions
 * @{
 */
#define DCFG_EOPFT_MASK         (3U << 11) /**< End of periodic frame mask. */
#define DCFG_EOPFT(n)           ((n) << 11)/**< End of periodic frame field.*/
#define DCFG_EOPFT_80           DCFG_EOPFT(0U)  /**< 80% of the frame.      */
#define DCFG_EOPFT_85           DCFG_EOPFT(1U)  /**< 85% of the frame.      */
#define DCFG_EOPFT_90           DCFG_EOPFT(2U)  /**< 90% of the frame.      */
#define DCFG_EOPFT_95           DCFG_EOPFT(3U)  /**< 95% of the frame.      */
#define DCFG_DAR_MASK           (0x7FU << 4)/**< Device address mask.       */
#define DCFG_DAR(n)             ((n) << 4) /**< Device address field.       */
#define DCFG_NZLSOH             (1U << 2)  /**< Non-zero-length status OUT. */
#define DCFG_DS_MASK            (3U << 0)  /**< Device speed mask.          */
#define DCFG_DS(n)              ((n) << 0) /**< Device speed field.         */
#define DCFG_DS_HS              DCFG_DS(0U)/**< Internal PHY, high speed.   */
#define DCFG_DS_FS              DCFG_DS(1U)/**< Internal PHY, full speed.   */
/** @} */

/**
 * @name    DCTL register bit definitions
 * @{
 */
#define DCTL_L1RJCT            (1U << 18) /**< Deep sleep reject.           */
#define DCTL_POIF              (1U << 11) /**< Power-on init finished.      */
#define DCTL_CGONAK            (1U << 10) /**< Clear global OUT NAK.        */
#define DCTL_SGONAK            (1U << 9)  /**< Set global OUT NAK.          */
#define DCTL_CGINAK            (1U << 8)  /**< Clear global IN NAK.         */
#define DCTL_SGINAK            (1U << 7)  /**< Set global IN NAK.           */
#define DCTL_DTEST_MASK        (7U << 4)  /**< Test control mask.           */
#define DCTL_GONS              (1U << 3)  /**< Global OUT NAK status.       */
#define DCTL_GINS              (1U << 2)  /**< Global IN NAK status.        */
#define DCTL_SD                (1U << 1)  /**< Soft disconnect.             */
#define DCTL_RWKUP             (1U << 0)  /**< Remote wakeup.               */
/** @} */

/**
 * @name    DSTAT register bit definitions
 * @{
 */
#define DSTAT_FNRSOF_MASK      (0x3FFFU << 8)/**< Frame number mask.        */
#define DSTAT_FNRSOF_OFF       8          /**< Frame number offset.         */
#define DSTAT_FNRSOF_ODD       (1U << 8)  /**< Odd frame number.            */
#define DSTAT_ES_MASK          (3U << 1)  /**< Enumerated speed mask.       */
#define DSTAT_ES_HS            (0U << 1)  /**< High speed enumerated.       */
#define DSTAT_ES_FS_PHY30_60   (1U << 1)  /**< Full speed, HS PHY clock.    */
#define DSTAT_ES_LS            (2U << 1)  /**< Low speed enumerated.        */
#define DSTAT_ES_FS_PHY48      (3U << 1)  /**< Full speed, FS PHY clock.    */
#define DSTAT_SPST             (1U << 0)  /**< Suspend status.              */
/** @} */

/**
 * @name    DIEPINTEN register bit definitions
 * @{
 */
#define DIEPINTEN_NAKEN        (1U << 13) /**< NAK handshake sent.          */
#define DIEPINTEN_TXFEEN       (1U << 7)  /**< TX FIFO empty.               */
#define DIEPINTEN_IEPNEEN      (1U << 6)  /**< IN endpoint NAK effective.   */
#define DIEPINTEN_EPTXFUDEN    (1U << 4)  /**< TX FIFO underrun.            */
#define DIEPINTEN_CITOEN       (1U << 3)  /**< Control IN timeout.          */
#define DIEPINTEN_EPDISEN      (1U << 1)  /**< Endpoint disabled.           */
#define DIEPINTEN_TFEN         (1U << 0)  /**< Transfer finished.           */
/** @} */

/**
 * @name    DOEPINTEN register bit definitions
 * @{
 */
#define DOEPINTEN_NYETEN       (1U << 14) /**< NYET handshake sent.         */
#define DOEPINTEN_BTBSTPEN     (1U << 6)  /**< Back-to-back SETUP packets.  */
#define DOEPINTEN_EPRXFOVREN   (1U << 4)  /**< RX FIFO overrun.             */
#define DOEPINTEN_STPFEN       (1U << 3)  /**< SETUP phase finished.        */
#define DOEPINTEN_EPDISEN      (1U << 1)  /**< Endpoint disabled.           */
#define DOEPINTEN_TFEN         (1U << 0)  /**< Transfer finished.           */
/** @} */

/**
 * @name    DAEPINT/DAEPINTEN register bit definitions
 * @{
 */
#define DAEPINT_OEPITB_MASK    (0xFFU << 16)/**< OUT endpoints mask.        */
#define DAEPINT_OEPITB_OFF     16         /**< OUT endpoints offset.        */
#define DAEPINT_IEPITB_MASK    (0xFFU << 0) /**< IN endpoints mask.         */
#define DAEPINT_IEPITB_OFF     0          /**< IN endpoints offset.         */
#define DAEPINTEN_OEPIE(n)     ((1U << 16) << (n))/**< OUT endpoint enable. */
#define DAEPINTEN_IEPIE(n)     ((1U << 0) << (n)) /**< IN endpoint enable.  */
/** @} */

/**
 * @name    DIEPFEINTEN register bit definitions
 * @{
 */
#define DIEPFEINTEN_IEPTXFEIE(n) (1U << (n))/**< IN endpoint FIFO empty.    */
/** @} */

/**
 * @name    DIEPCTL/DOEPCTL register bit definitions
 * @{
 */
#define DEPCTL_EPEN            (1U << 31) /**< Endpoint enable.             */
#define DEPCTL_EPD             (1U << 30) /**< Endpoint disable.            */
#define DEPCTL_SODDFRM         (1U << 29) /**< Set odd frame.               */
#define DEPCTL_SD1PID          (1U << 29) /**< Set DATA1 PID.               */
#define DEPCTL_SEVENFRM        (1U << 28) /**< Set even frame.              */
#define DEPCTL_SD0PID          (1U << 28) /**< Set DATA0 PID.               */
#define DEPCTL_SNAK            (1U << 27) /**< Set NAK.                     */
#define DEPCTL_CNAK            (1U << 26) /**< Clear NAK.                   */
#define DIEPCTL_TXFNUM_MASK    (15U << 22)/**< TX FIFO number mask.         */
#define DIEPCTL_TXFNUM(n)      ((n) << 22)/**< TX FIFO number field.        */
#define DEPCTL_STALL           (1U << 21) /**< STALL handshake.             */
#define DOEPCTL_SNOOP          (1U << 20) /**< Snoop mode.                  */
#define DEPCTL_EPTYPE_MASK     (3U << 18) /**< Endpoint type mask.          */
#define DEPCTL_EPTYPE(n)       ((n) << 18)/**< Endpoint type field.         */
#define DEPCTL_EPTYPE_CTRL     DEPCTL_EPTYPE(0U)/**< Control endpoint.      */
#define DEPCTL_EPTYPE_ISO      DEPCTL_EPTYPE(1U)/**< Isochronous endpoint.  */
#define DEPCTL_EPTYPE_BULK     DEPCTL_EPTYPE(2U)/**< Bulk endpoint.         */
#define DEPCTL_EPTYPE_INTR     DEPCTL_EPTYPE(3U)/**< Interrupt endpoint.    */
#define DEPCTL_NAKS            (1U << 17) /**< NAK status.                  */
#define DEPCTL_EOFRM           (1U << 16) /**< Even/odd frame.              */
#define DEPCTL_DPID            (1U << 16) /**< Endpoint data PID.           */
#define DEPCTL_EPACT           (1U << 15) /**< Endpoint active.             */
#define DEPCTL_MPL_MASK        (0x7FFU << 0)/**< Maximum packet length mask.*/
#define DEPCTL_MPL(n)          ((n) << 0) /**< Maximum packet length field. */
#define DEP0CTL_MPL_64         DEPCTL_MPL(0U)/**< 64 bytes, endpoint 0.     */
#define DEP0CTL_MPL_32         DEPCTL_MPL(1U)/**< 32 bytes, endpoint 0.     */
#define DEP0CTL_MPL_16         DEPCTL_MPL(2U)/**< 16 bytes, endpoint 0.     */
#define DEP0CTL_MPL_8          DEPCTL_MPL(3U)/**< 8 bytes, endpoint 0.      */
/** @} */

/**
 * @name    DIEPINTF register bit definitions
 * @{
 */
#define DIEPINTF_NAK           (1U << 13) /**< NAK handshake sent.          */
#define DIEPINTF_TXFE          (1U << 7)  /**< TX FIFO empty.               */
#define DIEPINTF_IEPNE         (1U << 6)  /**< IN endpoint NAK effective.   */
#define DIEPINTF_EPTXFUD       (1U << 4)  /**< TX FIFO underrun.            */
#define DIEPINTF_CITO          (1U << 3)  /**< Control IN timeout.          */
#define DIEPINTF_EPDIS         (1U << 1)  /**< Endpoint disabled.           */
#define DIEPINTF_TF            (1U << 0)  /**< Transfer finished.           */
/** @} */

/**
 * @name    DOEPINTF register bit definitions
 * @{
 */
#define DOEPINTF_NYET          (1U << 14) /**< NYET handshake sent.         */
#define DOEPINTF_BTBSTP        (1U << 6)  /**< Back-to-back SETUP packets.  */
#define DOEPINTF_EPRXFOVR      (1U << 4)  /**< RX FIFO overrun.             */
#define DOEPINTF_STPF          (1U << 3)  /**< SETUP phase finished.        */
#define DOEPINTF_EPDIS         (1U << 1)  /**< Endpoint disabled.           */
#define DOEPINTF_TF            (1U << 0)  /**< Transfer finished.           */
/** @} */

/**
 * @name    DIEPLEN/DOEPLEN register bit definitions
 * @{
 */
#define DOEPLEN_STPCNT_MASK    (3U << 29) /**< SETUP packet count mask.     */
#define DOEPLEN_STPCNT(n)      ((n) << 29)/**< SETUP packet count field.    */
#define DIEPLEN_MCNT_MASK      (3U << 29) /**< Multi count mask.            */
#define DIEPLEN_MCNT(n)        ((n) << 29)/**< Multi count field.           */
#define DEPLEN_PCNT_MASK       (0x3FFU << 19)/**< Packet count mask.        */
#define DEPLEN_PCNT(n)         ((n) << 19)/**< Packet count field.          */
#define DEPLEN_TLEN_MASK       (0x7FFFFU << 0)/**< Transfer length mask.    */
#define DEPLEN_TLEN(n)         ((n) << 0) /**< Transfer length field.       */
/** @} */

/**
 * @name    DIEPTFSTAT register bit definitions
 * @{
 */
#define DIEPTFSTAT_IEPTFS_MASK (0xFFFFU << 0)/**< TX FIFO space mask.       */
/** @} */

/**
 * @name    PWRCLKCTL register bit definitions
 * @{
 */
#define PWRCLKCTL_DSLEEP       (1U << 7)  /**< PHY in deep sleep.           */
#define PWRCLKCTL_SSLEEP       (1U << 6)  /**< PHY in shallow sleep.        */
#define PWRCLKCTL_SCGEN        (1U << 5)  /**< Internal clock gating.       */
#define PWRCLKCTL_SUSP         (1U << 4)  /**< PHY in suspend.              */
#define PWRCLKCTL_SHCLK        (1U << 1)  /**< Stop HCLK.                   */
#define PWRCLKCTL_SUCLK        (1U << 0)  /**< Stop the USB clock.          */
/** @} */

/**
 * @brief   Accesses to the USBHS0 registers block.
 */
#define USBHSD0                 ((gd32_otg_t *)GD32_USBHS0_ADDR)

/**
 * @brief   Accesses to the USBHS1 registers block.
 */
#define USBHSD1                 ((gd32_otg_t *)GD32_USBHS1_ADDR)

#endif /* GD32_OTG_H */

/** @} */
