#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/gpio.h"

// El GPIO 2 es el LED azul interno en la mayoría de las placas ESP32
#define LED_GPIO 2  

static const char *TAG = "CONTROL_AVANZADO";

void app_main(void)
{
    // Configuración del pin como salida
    gpio_reset_pin(LED_GPIO);
    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);

    ESP_LOGI(TAG, "¡Proyecto iniciado correctamente!");

    while (1)
    {
        // Encender LED
        gpio_set_level(LED_GPIO, 1);
        ESP_LOGI(TAG, "LED ENCENDIDO");
        vTaskDelay(pdMS_TO_TICKS(1000)); // Esperar 1 segundo

        // Apagar LED
        gpio_set_level(LED_GPIO, 0);
        ESP_LOGW(TAG, "LED APAGADO");
        vTaskDelay(pdMS_TO_TICKS(1000)); // Esperar 1 segundo
    }
}