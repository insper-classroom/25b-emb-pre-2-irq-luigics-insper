#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

const int BTN_PIN_R = 28;
const int BTN_PIN_G = 26;
volatile int btn_flag_r = 0;
volatile int btn_flag_g = 0;
const int LED_PIN_R = 4;
const int LED_PIN_G = 6;
  

void btn_callback(uint gpio, uint32_t events) {
    if (events == 0x4) {  // GPIO_IRQ_EDGE_FALL = event 0x4 /fall edge
        if(gpio == BTN_PIN_R){
            btn_flag_r = 1;
        }
    } else if(events == 0x8){ //   GPIO_IRQ_EDGE_RISE = event 0x8
        if(gpio == BTN_PIN_G){
            btn_flag_g = 1;
        }
    }
}

int main() {
  stdio_init_all();

  gpio_init(BTN_PIN_R);
  gpio_set_dir(BTN_PIN_R, GPIO_IN);
  gpio_pull_up(BTN_PIN_R);

  gpio_init(BTN_PIN_G);
  gpio_set_dir(BTN_PIN_G, GPIO_IN);
  gpio_pull_up(BTN_PIN_G);

  gpio_init(LED_PIN_R);
  gpio_set_dir(LED_PIN_R, GPIO_OUT);
  gpio_put(LED_PIN_R,0);

  gpio_init(LED_PIN_G);
  gpio_set_dir(LED_PIN_G, GPIO_OUT);
  gpio_put(LED_PIN_G,0);
  // callback led r (first)
  gpio_set_irq_enabled_with_callback(BTN_PIN_R, GPIO_IRQ_EDGE_FALL, true,
                                      &btn_callback);

  gpio_set_irq_enabled(BTN_PIN_G, GPIO_IRQ_EDGE_RISE, true);

  int led_r_status = 0;
  int led_g_status = 0;

  while (true) {
    if(btn_flag_g){
      led_g_status = !led_g_status;
      gpio_put(LED_PIN_G,led_g_status);
      btn_flag_g = 0;
      printf("green release\n");
    }
    if(btn_flag_r){
      led_r_status = !led_r_status;
      gpio_put(LED_PIN_R,led_r_status);
      btn_flag_r = 0;
      printf("red pressed\n");
    }
  }
}
