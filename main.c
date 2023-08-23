#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

const uint MEASURE_PIN = 3; // GPIO pin connected to Hall Sensor
const uint RELAY_PIN = 5; // GPIO pin connected to relay control

float measure_duty_cycle(uint gpio)
{
    // Only the PWM B pins can be used as inputs.
    assert(pwm_gpio_to_channel(gpio) == PWM_CHAN_B);
    uint slice_num = pwm_gpio_to_slice_num(gpio);

    // Count once for every 100 cycles the PWM B input is high
    pwm_config cfg = pwm_get_default_config();
    pwm_config_set_clkdiv_mode(&cfg, PWM_DIV_B_HIGH);
    pwm_config_set_clkdiv(&cfg, 100);
    pwm_init(slice_num, &cfg, false);
    gpio_set_function(gpio, GPIO_FUNC_PWM);

    pwm_set_enabled(slice_num, true);
    sleep_ms(10);
    pwm_set_enabled(slice_num, false);
    float counting_rate = clock_get_hz(clk_sys) / 100;
    float max_possible_count = counting_rate * 0.01;
    return pwm_get_counter(slice_num) / max_possible_count;
}

int main()
{
#ifndef PICO_DEFAULT_LED_PIN
#warning requires a board with a regular LED
#else
    stdio_init_all();
    gpio_init(RELAY_PIN);
    gpio_set_dir(RELAY_PIN, GPIO_OUT);
    /* const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 1);
    // Turning Relay Pin ON by Default as making it Ground makes relay active
    gpio_put(RELAY_PIN, 1); */

    printf("\nAutomatic Water Pressure Pump Controller\n");
    while (true)
    {
        sleep_ms(250);
        float measured_duty_cycle = measure_duty_cycle(MEASURE_PIN);
        printf("measured_duty_cycle = %.1f \n", measured_duty_cycle);
        //printf("Output duty cycle = %.1f%% \n", measured_duty_cycle * 100.f);

        if (measured_duty_cycle > 0 && measured_duty_cycle < 1.0)
        {
            gpio_put(RELAY_PIN, 0); // Turn on the relay [0 -> 10A Relay 1-> Solid State Relay]
            printf("\nRelay ON\n");
        }
        else
        {
            gpio_put(RELAY_PIN, 1); // Turn off the relay [0 -> 10A Relay 1-> Solid State Relay]
            printf("\nRelay OFF\n");
        }

        sleep_ms(250);
    }
#endif
}