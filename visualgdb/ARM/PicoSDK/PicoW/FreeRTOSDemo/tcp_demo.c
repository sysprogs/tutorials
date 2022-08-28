#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"

#include "lwip/apps/lwiperf.h"
#include "lwip/ip4_addr.h"
#include "lwip/netif.h"

#include "FreeRTOS.h"
#include "task.h"

#define TEST_TASK_PRIORITY (tskIDLE_PRIORITY + 2UL)

static void run_server()
{
    while (true)
    {
        vTaskDelay(100);
    }
}

static void main_task(__unused void *params)
{
    if (cyw43_arch_init())
    {
        printf("failed to initialise\n");
        return;
    }
    
    cyw43_arch_enable_sta_mode();
    printf("Connecting to WiFi...\n");
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000))
    {
        printf("failed to connect.\n");
        exit(1);
    }
    else
    {
        printf("Connected.\n");
    }

    run_server();
    cyw43_arch_deinit();
}

int main(void)
{
    stdio_init_all();

    TaskHandle_t task;
    xTaskCreate(main_task, "MainThread", configMINIMAL_STACK_SIZE, NULL, TEST_TASK_PRIORITY, &task);
    vTaskStartScheduler();
}
