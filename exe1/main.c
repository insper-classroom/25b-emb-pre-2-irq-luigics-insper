#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

const int BTN_PIN_R = 28;
volatile int desc = 0;
volatile int sub = 0;

void btn_callback(uint gpio, uint32_t events) {
  if (events == 0x4) { // fall edge
    desc = 1;
  } 
  if (events == 0x8){
    sub = 1;
  }
}

int main() {
  stdio_init_all();

  gpio_init(BTN_PIN_R);
  gpio_set_dir(BTN_PIN_R, GPIO_IN);
  gpio_pull_up(BTN_PIN_R);

  gpio_set_irq_enabled_with_callback(
      BTN_PIN_R, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &btn_callback);

  // while (1) {
  //   if(desc){
  //     printf("btn pressed \n");
  //     sleep_ms(1);
  //     desc = 0;
  //   }
  //   if(sub){
  //     printf("btn released \n");
  //     sub = 0;
  //   }
  // } 
  
}