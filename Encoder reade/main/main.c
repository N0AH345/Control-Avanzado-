#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "encoder_pcnt.h"

static const char *TAG = "MAIN";

void app_main(void)
{
    encoder_init();

    while (1)
    {
        int64_t position = encoder_get_position();
        float rpm = encoder_get_rpm();

        const char *direction;

        if (rpm > 0)
            direction = "Horario";
        else if (rpm < 0)
            direction = "Antihorario";
        else
            direction = "Detenido";

        ESP_LOGI(TAG,
                 "Conteo: %lld | RPM: %.2f | Sentido: %s",
                 position,
                 rpm,
                 direction);

        vTaskDelay(pdMS_TO_TICKS(SAMPLE_TIME_MS));
    }
}