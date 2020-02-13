#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "board.h"
#include "debug_console_imx.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "app.h"
#include "app_ecspi.h"
#include "app_gpt.h"
#include "app_time.h"
#include "app_log.h"


#define APP_TASK_STACK_SIZE    256


static void APP_Task_EcspiSend(void *param) {
    uint8_t txbuf[4];
    SemaphoreHandle_t send_sem = (SemaphoreHandle_t)param;

    APP_INFO("SPI send task start");

    uint32_t counter = 0;
    while (true) {
        uint32_t data = counter;
        for (int i = 3; i >= 0; --i) {
            txbuf[i] = data & 0xFF;
            data = data >> 8;
        }
        ASSERT(xSemaphoreTake(send_sem, portMAX_DELAY) == pdTRUE);

        //APP_INFO("Transfer SPI message %d", counter);
        if (APP_ECSPI_Transfer(txbuf, NULL, 4, APP_FOREVER_MS) != 0) {
            PANIC_("Cannot transfer SPI message");
        }
        counter += 1;
    }
}

int main(void) {
    /* Initialize board specified hardware. */
    hardware_init();

    PRINTF("\r\n\r\n");
    APP_INFO("Program start");

    SemaphoreHandle_t send_sem = xSemaphoreCreateBinary();
    ASSERT(send_sem);

    ASSERT(APP_ECSPI_Init(50000000) == 0);
    
    /* Create tasks. */
    xTaskCreate(
        APP_Task_EcspiSend, "ECSPI send task",
        APP_TASK_STACK_SIZE, (void *)send_sem, tskIDLE_PRIORITY + 1, NULL
    );

    APP_INFO("Start GPT");
    APP_GPT_Init(APP_GPT_SEC/100000, send_sem);


    /* Start FreeRTOS scheduler. */
    vTaskStartScheduler();

    /* Should never reach this point. */
    PANIC_("End of main()");
    return 0;
}
