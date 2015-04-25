/* Arduino SdSpi Library
 * Copyright (C) 2013 by William Greiman
 *
 * STM32F1 code for Maple Mini support copyleft 2015 by Victor Perez
 *
 * This file is part of the Arduino SdSpi Library
 *
 * This Library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the Arduino SdSpi Library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */
#if defined(__STM32F1__)
#include "SdSpi.h"
#include <SPI.h>
#include <libmaple/dma.h>
/** Use STM32 DMAC if nonzero */
//#define USE_STM32F1_DMAC 1
/** Time in ms for DMA receive timeout */
#define STM32F1_DMA_TIMEOUT 100
/** DMAC receive channel */
//#define SPI_DMAC_RX_CH  DMA_CH2
/** DMAC transmit channel */
//#define SPI_DMAC_TX_CH  DMA_CH3
/** DMAC Channel HW Interface Number for SPI TX. */
//#define SPI_TX_IDX  1
/** DMAC Channel HW Interface Number for SPI RX. */
//#define SPI_RX_IDX  2

/*
volatile bool SPI_DMA_TX_Active = false;
volatile bool SPI_DMA_RX_Active = false;

inline void SPI_DMA_TX_Event() {
  SPI_DMA_TX_Active = false;
  dma_disable(DMA1, DMA_CH3);
}

inline void SPI_DMA_RX_Event() {
  SPI_DMA_RX_Active = false;
  dma_disable(DMA1, DMA_CH2);
}
*/
//------------------------------------------------------------------------------
/** Disable DMA Controller. */
//static void dmac_disable() {
//  DMAC->DMAC_EN &= (~DMAC_EN_ENABLE);
//}
/** Enable DMA Controller. */
//static void dmac_enable() {
//  DMAC->DMAC_EN = DMAC_EN_ENABLE;
//}
/** Disable DMA Channel. */
/*
static void dmac_channel_disable(dma_channel ul_num) {
  dma_disable(DMA1, ul_num);
}
*/
/** Enable DMA Channel. */
/*
static void dmac_channel_enable(dma_channel ul_num) {
  dma_enable(DMA1, ul_num);
}
*/
/** Poll for transfer complete. */
//static bool dmac_channel_transfer_done(dma_tube tube) {
//    uint8 shift = (tube - 1) * 4;
//    return ((DMA1->regs->ISR >> shift) & 0x02) ? false : true;

//  return (DMAC->DMAC_CHSR & (DMAC_CHSR_ENA0 << ul_num)) ? false : true;
//}
//------------------------------------------------------------------------------
void SdSpi::begin() {
  SPI.begin();
//  SPI.setClockDivider(SPI_CLOCK_DIV2);
//  SPI.setBitOrder(MSBFIRST);
//  SPI.setDataMode(SPI_MODE0);
  // DMA setup stuff. We use a line buffer and usa DMA for filling lines and blocks.




}
//------------------------------------------------------------------------------
// start RX DMA
/*
static void spiDmaRX(uint8_t* dst, uint16_t count) {
//  spi_rx_dma_enable(SPI1);
  if (count < 1) return;
  dma_setup_transfer(DMA1, DMA_CH2, &SPI1->regs->DR, DMA_SIZE_8BITS,
                     dst, DMA_SIZE_8BITS, (DMA_MINC_MODE | DMA_TRNS_CMPLT));
  dma_set_num_transfers(DMA1, DMA_CH2, count); // 2 bytes per pixel
  SPI_DMA_RX_Active = true;
  dma_enable(DMA1, DMA_CH2);


}
*/
//------------------------------------------------------------------------------
// start TX DMA
/*
static void spiDmaTX(const uint8_t* src, uint16_t count) {
  if (count < 1) return;
  static uint8_t ff = 0XFF;

//  spi_tx_dma_enable(SPI1);
  //    dma_init(DMA1);
  //    dma_attach_interrupt(DMA1, DMA_CH3, SPI_DMA_TX_Event);


  if (!src) {
    src = &ff;
    dma_setup_transfer(DMA1, DMA_CH3, &SPI1->regs->DR, DMA_SIZE_8BITS,
                       const_cast<uint8_t*>(src), DMA_SIZE_8BITS, (DMA_FROM_MEM | DMA_TRNS_CMPLT));
  }
  else {
    dma_setup_transfer(DMA1, DMA_CH3, &SPI1->regs->DR, DMA_SIZE_8BITS,
                       const_cast<uint8_t*>(src), DMA_SIZE_8BITS, (DMA_MINC_MODE  |  DMA_FROM_MEM | DMA_TRNS_CMPLT));
  }
  dma_set_num_transfers(DMA1, DMA_CH3, count); // 2 bytes per pixel
  SPI_DMA_TX_Active = true;
  dma_enable(DMA1, DMA_CH3);

}
*/
//------------------------------------------------------------------------------
//  initialize SPI controller STM32F1
void SdSpi::init(uint8_t sckDivisor) {
  if (sckDivisor < SPI_CLOCK_DIV2 || sckDivisor > SPI_CLOCK_DIV256) sckDivisor=SPI_CLOCK_DIV2; //may not be needed, testing.
  SPI.setClockDivider(sckDivisor);
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  
#if USE_STM32F1_DMAC
//  dma_init(DMA1);
//  dma_attach_interrupt(DMA1, DMA_CH3, SPI_DMA_TX_Event);
//  dma_attach_interrupt(DMA1, DMA_CH2, SPI_DMA_RX_Event);
//  spi_tx_dma_enable(SPI1);
//  spi_rx_dma_enable(SPI1);
#endif  // USE_STM32F1_DMAC
  
}
//------------------------------------------------------------------------------
// STM32
static inline uint8_t spiTransfer(uint8_t b) {
  return SPI.transfer(b);
}
//------------------------------------------------------------------------------
// should be valid for STM32
/** SPI receive a byte */
uint8_t SdSpi::receive() {
  return spiTransfer(0xFF);
}
//------------------------------------------------------------------------------
/** SPI receive multiple bytes */
// check and finish.

uint8_t SdSpi::receive(uint8_t* buf, size_t n) {
  int rtn = 0;

#if USE_STM32F1_DMAC

  SPI.dmaTransfer(0, const_cast<uint8*>(buf), n);

/*
  spiDmaRX(buf, n);
  spiDmaTX(0, n);

  uint32_t m = millis();
  while (SPI_DMA_RX_Active) {
    if ((millis() - m) > STM32F1_DMA_TIMEOUT)  {
      dmac_channel_disable(SPI_DMAC_RX_CH);
      dmac_channel_disable(SPI_DMAC_TX_CH);
      rtn = 2;
      break;
    }
  }
*/

  //  if (pSpi->SPI_SR & SPI_SR_OVRES) rtn |= 1;
#else  // USE_STM32F1_DMAC
  for (size_t i = 0; i < n; i++) {
    buf[i] = SPI.transfer (0xFF);
  }
#endif  // USE_STM32F1_DMAC
  return rtn;
}
//------------------------------------------------------------------------------
/** SPI send a byte */
void SdSpi::send(uint8_t b) {
  spiTransfer(b);
}
//------------------------------------------------------------------------------
void SdSpi::send(const uint8_t* buf , size_t n) {

#if USE_STM32F1_DMAC
//  spiDmaTX(buf, n);
//  while (SPI_DMA_TX_Active) {}
  SPI.dmaSend(const_cast<uint8*>(buf), n);
  //  uint32_t m = millis();
  //  while (SPI_DMA_TX_Active) {
  //    if ((millis() - m) > STM32F1_DMA_TIMEOUT)  {
  //      dmac_channel_disable(SPI_DMAC_TX_CH);
  //      break;
  //    }
  //  }
#else  // #if USE_STM32F1_DMAC
  SPI.write (buf, n);
  if (spi_is_rx_nonempty(SPI1)) {
    uint8_t b = spi_rx_reg(SPI1);
  }
#endif  // #if USE_STM32F1_DMAC
  // leave RDR empty
  //  while (spi_is_rx_nonempty(SPI1))
//  uint8_t b = SPI.read();
//  uint8_t b = spi_rx_reg(SPI1);
}
#endif  // USE_NATIVE_STM32F1_SPI
