/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include <esp_spiffs.h>
#include <string.h>

/* Can use project configuration menu (idf.py menuconfig) to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/
#define BLINK_GPIO CONFIG_BLINK_GPIO


void app_main(void)
{
    /* Configure the IOMUX register for pad BLINK_GPIO (some pads are
       muxed to GPIO on reset already, but some default to other
       functions and need to be switched to GPIO. Consult the
       Technical Reference for a list of pads and their default
       functions.)
    */
    gpio_pad_select_gpio(BLINK_GPIO);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);

    esp_vfs_spiffs_conf_t conf = {
        .base_path = "/spiffs",
        .partition_label = NULL,
        .max_files = 5,
        .format_if_mount_failed = false};

    esp_err_t ret = esp_vfs_spiffs_register(&conf);

    if (ret != ESP_OK)
        asm("break 1, 1");

    for (;;)
    {
        FILE *f = fopen("/spiffs/program.txt", "r");
        if (f == NULL)
            asm("break 1, 1");

        for (char buffer[256], *line; (line = fgets(buffer, sizeof(buffer), f));)
        {
            for (int i = 0; i < sizeof(buffer); i++)
                if (buffer[i] == '\r' || buffer[i] == '\n')
                {
                    buffer[i] = 0;
                    break;
                }
            
            if (!strcmp(line, "on"))
                gpio_set_level(BLINK_GPIO, 0);
            else if (!strcmp(line, "off"))
                gpio_set_level(BLINK_GPIO, 1);
            else if (!strncmp(line, "wait ", 5))
                vTaskDelay(atoi(line + 5) / portTICK_PERIOD_MS);
        }

        fclose(f);
    }
}
