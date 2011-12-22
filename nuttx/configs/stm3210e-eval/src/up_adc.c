/************************************************************************************
 * configs/stm3210e-eval/src/up_adc.c
 * arch/arm/src/board/up_adc.c
 *
 *   Copyright (C) 2011 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ************************************************************************************/

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <nuttx/config.h>

#include <debug.h>

#include <nuttx/analog/adc.h>
#include <arch/board/board.h>

#include "chip.h"
#include "up_arch.h"

#include "stm32_pwm.h"
#include "stm3210e-internal.h"

#ifdef CONFIG_ADC

/************************************************************************************
 * Definitions
 ************************************************************************************/

/* Configuration ************************************************************/
/* Up to 3 ADC interfaces are supported */

#if STM32_NADC < 3
#  undef CONFIG_STM32_ADC3
#endif

#if STM32_NADC < 2
#  undef CONFIG_STM32_ADC2
#endif

#if STM32_NADC < 1
#  undef CONFIG_STM32_ADC1
#endif

#if defined(CONFIG_STM32_ADC1) || defined(CONFIG_STM32_ADC2) || defined(CONFIG_STM32_ADC3)

/* The number of ADC channels in the conversion list */

#define ADC_NCHANNELS 2

/************************************************************************************
 * Private Data
 ************************************************************************************/

/* Identifying number of each ADC channel: Variable Resistor and BNC_CN5 */

static const uint8_t  g_chanlist[ADC_NCHANNELS] = {14, 11};

/* Configurations of pins used byte each ADC channels */

static const uint32_t g_pinlist[ADC_NCHANNELS]  = {GPIO_ADC1_IN14 , GPIO_ADC1_IN11};

/************************************************************************************
 * Private Functions
 ************************************************************************************/

/************************************************************************************
 * Public Functions
 ************************************************************************************/

/************************************************************************************
 * Name: stm32_boardinitialize
 *
 * Description:
 *   All STM32 architectures must provide the following entry point.  This entry point
 *   is called early in the intitialization -- after all memory has been configured
 *   and mapped but before any devices have been initialized.
 *
 ************************************************************************************/

int adc_devinit(void)
{
  struct adc_dev_s *adc;
  int ret;
  int i;

  /* Configure the pins as analog inputs for the selected channels */

  for(i = 0; i < ADC_NCHANNELS; i++)
    {
      stm32_configgpio(g_pinlist[i]);
    }

  /* Call stm32_adcinitialize() to get an instance of the ADC interface */

  adc = stm32_adcinitialize(1, g_chanlist, ADC_NCHANNELS);

  /* Register the ADC driver at "/dev/adc0" */

  ret = adc_register("/dev/adc0", adc);
  if (ret < 0)
    {
      adbg("adc_register failed: %d\n", ret);
    }

  return OK;
}

#endif /* CONFIG_STM32_ADC || CONFIG_STM32_ADC2 || CONFIG_STM32_ADC3 */
#endif /* CONFIG_ADC */