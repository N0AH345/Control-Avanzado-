#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#define LOG_LOCAL_LEVEL ESP_LOG_DEBUG
#include "esp_log.h"
#include "esp_err.h"

static const char *TAG = "LED_BLINK"; 
#define LED 2
static uint8_t led_level = 0;


TimerHandle_t xTimers;
const int timer_interval = 2000;
const int timerId = 1;

// Inicializador pin digital 
esp_err_t init_led(void){

    ESP_LOGI(TAG, "Inicializando LED");

    gpio_reset_pin(LED);
    esp_err_t err = gpio_set_direction(LED, GPIO_MODE_OUTPUT);

    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error al configurar el GPIO %d", LED);
    } else {
        ESP_LOGD(TAG, "GPIO %d configurado como salida", LED);
    }

    return err;
}

// Encendido / Apagado del LED 
esp_err_t blink_led(void)
{
    led_level = !led_level;
    esp_err_t err = gpio_set_level(LED, led_level);

    if (err != ESP_OK) {
        ESP_LOGE(TAG, "estado del LED");
    } else {
        if (led_level) {
            ESP_LOGI(TAG, "LED ENCENDIDO");
        } else {
            ESP_LOGW(TAG, "LED APAGADO");
        }
        ESP_LOGD(TAG, "Nivel del LED: %d", led_level);
    }

    return err;
}

// Regreso Timer
void vTimerCallback(TimerHandle_t pxTimer)
{
    ESP_LOGI(TAG, "TIMER ");
    ESP_ERROR_CHECK(blink_led());
}

// Configuración del Timer
esp_err_t set_timer(void){

    ESP_LOGI(TAG, "Configurando timer ");

    xTimers = xTimerCreate(
        "Timer_LED",
        pdMS_TO_TICKS(timer_interval),
        pdTRUE,
        (void *)timerId,
        vTimerCallback
    );

    if (xTimers == NULL){
        ESP_LOGE(TAG, "Error al crear el timer");
        return ESP_FAIL;
    }

    if (xTimerStart(xTimers, 0) != pdPASS){
        ESP_LOGE(TAG, "Error al iniciar el timer");
        return ESP_FAIL;
    }

    return ESP_OK;
}

void app_main(void){
    

    esp_log_level_set(TAG, ESP_LOG_INFO);

    ESP_LOGI(TAG, "Aplicación iniciada");

    ESP_ERROR_CHECK(init_led());
    ESP_ERROR_CHECK(set_timer());

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}