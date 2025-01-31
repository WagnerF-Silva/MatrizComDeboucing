#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "ws2812.pio.h"

#define LED_RED 13         // LED vermelho do RGB
#define LED_GREEN 11       // LED Verde (Correção)
#define BTN_A 5           // Botão A (Incrementa número)
#define BTN_B 6           // Botão B (Decrementa número)
#define WS2812_PIN 7      // Pino dos LEDs WS2812
#define NUM_PIXELS 25     // Matriz 5x5
#define IS_RGBW false
#define TEMPO_LED 100     // Pisca 5 vezes por segundo
#define TEMPO 2500        // Tempo de pisca do LED Verde

volatile int num = 0; // Número exibido (0-9)
static volatile uint32_t last_time = 0; // Debouncing

const bool num_patterns[10][25] = {
    { 0,1,1,1,0, 0,1,0,1,0, 0,1,0,1,0, 0,1,0,1,0, 0,1,1,1,0 }, // 0
    { 0,0,1,0,0, 0,0,1,0,0, 0,0,1,0,0, 0,0,1,0,0, 0,0,1,0,0 }, // 1
    { 0,1,1,1,0, 0,1,0,0,0, 0,1,1,1,0, 0,0,0,1,0, 0,1,1,1,0 }, // 2
    { 0,1,1,1,0, 0,0,0,1,0, 0,1,1,1,0, 0,0,0,1,0, 0,1,1,1,0 }, // 3
    { 0,1,0,0,0, 0,0,0,1,0, 0,1,1,1,0, 0,1,0,1,0, 0,1,0,1,0 }, // 4
    { 0,1,1,1,0, 0,0,0,1,0, 0,1,1,1,0, 0,1,0,0,0, 0,1,1,1,0 }, // 5
    { 0,1,1,1,0, 0,1,0,1,0, 0,1,1,1,0, 0,1,0,0,0, 0,1,1,1,0 }, // 6
    { 0,1,0,0,0, 0,0,0,1,0, 0,1,0,0,0, 0,0,0,1,0, 0,1,1,1,0 }, // 7
    { 0,1,1,1,0, 0,1,0,1,0, 0,1,1,1,0, 0,1,0,1,0, 0,1,1,1,0 }, // 8
    { 0,1,1,1,0, 0,0,0,1,0, 0,1,1,1,0, 0,1,0,1,0, 0,1,1,1,0 }  // 9
};

static inline void put_pixel(uint32_t pixel_grb) {
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b);
}

void update_led_matrix() {
    for (int i = 0; i < NUM_PIXELS; i++) {
        put_pixel(num_patterns[num][i] ? urgb_u32(0, 10, 0) : urgb_u32(0, 0, 0));
    }
}

void debounce() {
    static uint32_t last_press = 0;
    uint32_t now = to_ms_since_boot(get_absolute_time());
    if (now - last_press < 200) return;
    last_press = now;
}

void button_callback(uint gpio, uint32_t events) {
    debounce();
    uint32_t current_time = to_us_since_boot(get_absolute_time());
    if (current_time - last_time > 200000) {
        last_time = current_time;
        int last_num = num;
        if (gpio == BTN_A && num < 9) num++;
        if (gpio == BTN_B && num > 0) num--;
        if (last_num != num) update_led_matrix();
    }
}

void setup_gpio() {
    gpio_init(LED_RED);
    gpio_set_dir(LED_RED, GPIO_OUT);
    gpio_init(LED_GREEN);
    gpio_set_dir(LED_GREEN, GPIO_OUT);
    gpio_init(BTN_A);
    gpio_set_dir(BTN_A, GPIO_IN);
    gpio_pull_up(BTN_A);
    gpio_set_irq_enabled_with_callback(BTN_A, GPIO_IRQ_EDGE_FALL, true, &button_callback);
    gpio_init(BTN_B);
    gpio_set_dir(BTN_B, GPIO_IN);
    gpio_pull_up(BTN_B);
    gpio_set_irq_enabled_with_callback(BTN_B, GPIO_IRQ_EDGE_FALL, true, &button_callback);
}

int main() {
    stdio_init_all();
    setup_gpio();
    
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);
    update_led_matrix();

    while (1) {
        gpio_put(LED_RED, 1);
        sleep_ms(TEMPO_LED);
        gpio_put(LED_RED, 0);
        sleep_ms(TEMPO_LED);
       //gpio_put(LED_GREEN, true);
        //sleep_ms(TEMPO);
        //gpio_put(LED_GREEN, false);
        //sleep_ms(TEMPO);
    }
}