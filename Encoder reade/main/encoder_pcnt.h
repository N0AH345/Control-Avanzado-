#ifndef ENCODER_PCNT_H
#define ENCODER_PCNT_H

#include <stdint.h>

#define ENCODER_GPIO_A 33
#define ENCODER_GPIO_B 25

// Pulsos por revolución del encoder (ANTES del x4)
#define ENCODER_PPR 600

// Intervalo de muestreo en ms
#define SAMPLE_TIME_MS 100

void encoder_init(void);
int64_t encoder_get_position(void);
float encoder_get_rpm(void);

#endif