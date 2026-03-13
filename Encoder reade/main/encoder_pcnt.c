#include "encoder_pcnt.h"
#include "driver/pulse_cnt.h"
#include "esp_log.h"

static const char *TAG = "ENCODER";

#define PCNT_HIGH_LIMIT  32767
#define PCNT_LOW_LIMIT  -32768

static pcnt_unit_handle_t pcnt_unit = NULL;

static int16_t prev_count = 0;
static int64_t total_count = 0;

void encoder_init(void)
{
    pcnt_unit_config_t unit_config = {
        .high_limit = PCNT_HIGH_LIMIT,
        .low_limit  = PCNT_LOW_LIMIT,
    };

    pcnt_new_unit(&unit_config, &pcnt_unit);

    pcnt_glitch_filter_config_t filter_config = {
        .max_glitch_ns = 1000,
    };

    pcnt_unit_set_glitch_filter(pcnt_unit, &filter_config);

    pcnt_channel_handle_t chan_a = NULL;
    pcnt_channel_handle_t chan_b = NULL;

    pcnt_chan_config_t chan_a_config = {
        .edge_gpio_num = ENCODER_GPIO_A,
        .level_gpio_num = ENCODER_GPIO_B,
    };

    pcnt_chan_config_t chan_b_config = {
        .edge_gpio_num = ENCODER_GPIO_B,
        .level_gpio_num = ENCODER_GPIO_A,
    };

    pcnt_new_channel(pcnt_unit, &chan_a_config, &chan_a);
    pcnt_new_channel(pcnt_unit, &chan_b_config, &chan_b);

    pcnt_channel_set_edge_action(
        chan_a,
        PCNT_CHANNEL_EDGE_ACTION_DECREASE,
        PCNT_CHANNEL_EDGE_ACTION_INCREASE);

    pcnt_channel_set_level_action(
        chan_a,
        PCNT_CHANNEL_LEVEL_ACTION_KEEP,
        PCNT_CHANNEL_LEVEL_ACTION_INVERSE);

    pcnt_channel_set_edge_action(
        chan_b,
        PCNT_CHANNEL_EDGE_ACTION_INCREASE,
        PCNT_CHANNEL_EDGE_ACTION_DECREASE);

    pcnt_channel_set_level_action(
        chan_b,
        PCNT_CHANNEL_LEVEL_ACTION_KEEP,
        PCNT_CHANNEL_LEVEL_ACTION_INVERSE);

    pcnt_unit_enable(pcnt_unit);
    pcnt_unit_clear_count(pcnt_unit);
    pcnt_unit_start(pcnt_unit);

    ESP_LOGI(TAG, "Encoder inicializado");
}

int64_t encoder_get_position(void)
{
    int16_t count;
    pcnt_unit_get_count(pcnt_unit, (int *)&count);

    int32_t delta = count - prev_count;

    if (delta > 32767)
        delta -= 65536;
    else if (delta < -32768)
        delta += 65536;

    total_count += delta;
    prev_count = count;

    return total_count;
}

float encoder_get_rpm(void)
{
    static int64_t prev_pos = 0;

    int64_t pos = encoder_get_position();

    int64_t delta = pos - prev_pos;
    prev_pos = pos;

    float pulses_per_rev = ENCODER_PPR * 4.0;

    float rpm = (delta * 60000.0) /
                (pulses_per_rev * SAMPLE_TIME_MS);

    return rpm;
}