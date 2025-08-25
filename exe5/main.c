#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

const int BTN_PIN_R = 28;
const int LED_PIN = 4;
volatile int btn_flag = 0;

volatile uint64_t tempo_press = 0;
volatile int soltou = 0;

void btn_callback(uint gpio, uint32_t events) {
    if (events == 0x4 && GPIO_IRQ_EDGE_FALL) { // fall edge
        tempo_press = time_us_64();
    }
    if (events == 0x8 && GPIO_IRQ_EDGE_RISE){
        soltou = 1;
    }
}

int main() {
    stdio_init_all();

    int led_status = 0;
    gpio_init(BTN_PIN_R);
    gpio_set_dir(BTN_PIN_R, GPIO_IN);
    gpio_pull_up(BTN_PIN_R);
    gpio_set_irq_enabled_with_callback(BTN_PIN_R, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true,
                                       &btn_callback);
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 0);

    while (true) {
        if (soltou) {
            led_status = !led_status;
            uint64_t variacao_t = (time_us_64() - tempo_press) / 1000; //dado q eh em ms
            gpio_put(LED_PIN,led_status);
            if(variacao_t >= 800){
                printf("Aperto longo!\n");
            }else if(variacao_t < 800) {
                printf("Aperto curto!\n");
            }
            soltou = 0;
        }
    }
}
