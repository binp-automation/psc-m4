/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "board.h"
#include "pin_mux.h"
#include "gpio_pins.h"
#include "gpio_imx.h"

#include "app.h"


static gpio_config_t app_gpio_pin = {
    "AppGpio",                           /* name */
    &IOMUXC_SW_MUX_CTL_PAD_EPDC_DATA04,  /* muxReg */
    5,                                   /* muxConfig */
    &IOMUXC_SW_PAD_CTL_PAD_EPDC_DATA04,  /* padReg */
    0,                                   /* padConfig */
    GPIO2,                               /* base */
    4                                    /* pin */
};

void APP_GPIO_HardwareInit() {
    /* In this demo, we need to share board GPIO, we can set sreq argument to true
     * when the peer core could also access GPIO with RDC_SEMAPHORE, or the peer
     * core doesn't access the GPIO at all */
    //RDC_SetPdapAccess(RDC, BOARD_GPIO_KEY_RDC_PDAP, 0xFF, false/*true*/, false);

    /* Configure gpio pin IOMUX */
    configure_gpio_pin(&app_gpio_pin);
}

uint8_t APP_GPIO_Init() {
    gpio_init_config_t pinInit = {
        .pin           = app_gpio_pin.pin,
        .direction     = gpioDigitalOutput,
        .interruptMode = gpioNoIntmode
    };

    GPIO_Init(app_gpio_pin.base, &pinInit);
    return 0;
}

uint8_t APP_GPIO_Set(uint8_t on) {
    GPIO_WritePinOutput(app_gpio_pin.base, app_gpio_pin.pin, on ? gpioPinSet : gpioPinClear);
    return 0;
}
