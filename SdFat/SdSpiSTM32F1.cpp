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



//------------------------------------------------------------------------------
void SdSpi::begin() {
  SPI.begin();
}

//------------------------------------------------------------------------------
//  initialize SPI1 controller STM32F1
void SdSpi::init(uint8_t sckDivisor) {
  if (sckDivisor < SPI_CLOCK_DIV2 || sckDivisor > SPI_CLOCK_DIV256) sckDivisor=SPI_CLOCK_DIV2; //may not be needed, testing.
  SPI.setClockDivider(sckDivisor);
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
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

  rtn = SPI.dmaTransfer(0, const_cast<uint8*>(buf), n);

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
  SPI.dmaSend(const_cast<uint8*>(buf), n);

#else  // #if USE_STM32F1_DMAC
  SPI.write (buf, n);
  if (spi_is_rx_nonempty(SPI1)) {
    uint8_t b = spi_rx_reg(SPI1);
  }
#endif  
}
#endif  // USE_NATIVE_STM32F1_SPI
