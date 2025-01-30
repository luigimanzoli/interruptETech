#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"
#include "pico/bootrom.h"
#include "hardware/pwm.h"

//arquivo .pio
#include "interrupt.pio.h"

//número de LEDs
#define NUM_PIXELS 25

//pino de saída
#define OUT_PIN 7

// Definição dos LEDs RGB
#define RLED_PIN 13
#define GLED_PIN 11
#define BLED_PIN 12

// Definição dos botões
#define BTNA_PIN 5
#define BTNB_PIN 6

void init_all() {
    gpio_init(RLED_PIN);
    gpio_set_dir(RLED_PIN, GPIO_OUT);
    gpio_put(RLED_PIN, 0);

    gpio_init(GLED_PIN);
    gpio_set_dir(GLED_PIN, GPIO_OUT);
    gpio_put(GLED_PIN, 0);

    gpio_init(BLED_PIN);
    gpio_set_dir(BLED_PIN, GPIO_OUT);
    gpio_put(BLED_PIN, 0);

    gpio_init(BTNA_PIN);
    gpio_set_dir(BTNA_PIN, GPIO_IN);
    gpio_pull_up(BTNA_PIN);

    gpio_init(BTNB_PIN);
    gpio_set_dir(BTNB_PIN, GPIO_IN);
    gpio_pull_up(BTNB_PIN);
}

void get_led(bool R, bool G, bool B) {
    gpio_put(RLED_PIN, R);
    gpio_put(GLED_PIN, G);
    gpio_put(BLED_PIN, B);
}

//todos apagados
double desenho_apagado[25] = {
    0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0
};

//ativando todas as cores
double luz_total[25] = {
    1.0, 1.0, 1.0, 1.0, 1.0,
    1.0, 1.0, 1.0, 1.0, 1.0,
    1.0, 1.0, 1.0, 1.0, 1.0,
    1.0, 1.0, 1.0, 1.0, 1.0,
    1.0, 1.0, 1.0, 1.0, 1.0
};
//ativando todas as cores com 80% de intensidade
double luz_80_total[25] = {
    0.8, 0.8, 0.8, 0.8, 0.8,
    0.8, 0.8, 0.8, 0.8, 0.8,
    0.8, 0.8, 0.8, 0.8, 0.8,
    0.8, 0.8, 0.8, 0.8, 0.8,
    0.8, 0.8, 0.8, 0.8, 0.8
};
//ativando todas as cores com 50% de intensidade
double luz_50_total[25] = {
    0.5, 0.5, 0.5, 0.5, 0.5,
    0.5, 0.5, 0.5, 0.5, 0.5,
    0.5, 0.5, 0.5, 0.5, 0.5,
    0.5, 0.5, 0.5, 0.5, 0.5,
    0.5, 0.5, 0.5, 0.5, 0.5
};

//ativando todas as cores com 20% de intensidade
double luz_20_total[25] = {
    0.2, 0.2, 0.2, 0.2, 0.2,
    0.2, 0.2, 0.2, 0.2, 0.2,
    0.2, 0.2, 0.2, 0.2, 0.2,
    0.2, 0.2, 0.2, 0.2, 0.2,
    0.2, 0.2, 0.2, 0.2, 0.2
};

//rotina para definição da intensidade de cores do led
uint32_t matrix_rgb(double b, double r, double g)
{
  unsigned char R, G, B;
  R = r * 255;
  G = g * 255;
  B = b * 255;
  return (G << 24) | (R << 16) | (B << 8);
}

//rotina para acionar a matrix de leds - ws2812b
void desenho_pio(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b){

    for (int16_t i = 0; i < NUM_PIXELS; i++) {
        // Define a cor vermelha para cada LED
        valor_led = matrix_rgb(0.0, desenho[24 - i], 0.0); // Apenas o valor vermelho está ativo
        pio_sm_put_blocking(pio, sm, valor_led); // Envia o valor para o LED
    }
}
void desenho_apagado_total(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b){

    for (int16_t i = 0; i < NUM_PIXELS; i++) {
        // Define a cor nenhuma
        valor_led = matrix_rgb(0.0, 0.0, 0.0); // Nenhum ativo
        pio_sm_put_blocking(pio, sm, valor_led); // Envia o valor para o LED
    }
}
//desenhando o azul completo
void desenho_azul(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b){

    for (int16_t i = 0; i < NUM_PIXELS; i++) {
        // Define a cor azul para cada LED
        valor_led = matrix_rgb(desenho[24 - i], 0.0, 0.0); // Apenas o valor azul está ativo
        pio_sm_put_blocking(pio, sm, valor_led); // Envia o valor para o LED
    }
}

void desenho_verde(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b){

    for (int16_t i = 0; i < NUM_PIXELS; i++) {
        // Define a cor azul para cada LED
        valor_led = matrix_rgb(0.0, 0.0, desenho[24 - i]); // Apenas o valor verde está ativo
        pio_sm_put_blocking(pio, sm, valor_led); // Envia o valor para o LED
    }
}
void desenho_branco(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b){

    for (int16_t i = 0; i < NUM_PIXELS; i++) {
        // Define a cor azul para cada LED
        valor_led = matrix_rgb(desenho[24 - i], desenho[24 - i], desenho[24 - i]); // Todos ativos
        pio_sm_put_blocking(pio, sm, valor_led); // Envia o valor para o LED
    }
}
// Inicializa o sistema de clock
void inicializar_clock() {
    bool ok = set_sys_clock_khz(128000, false);
    if (ok) {
        printf("Clock set to %ld\n", clock_get_hz(clk_sys));
    } else {
        printf("Falha ao configurar o clock\n");
    }
}

// Configura a PIO
void configurar_pio(PIO pio, uint *offset, uint *sm) {
    *offset = pio_add_program(pio, &interrupt_program);
    *sm = pio_claim_unused_sm(pio, true);
    interrupt_program_init(pio, *sm, *offset, OUT_PIN);
}

// Função principal
int main() {
    // Inicializa clock, stdio e configurações
    stdio_init_all();
    init_all();
    inicializar_clock();

    PIO pio = pio0;
    uint offset, sm;
    uint32_t valor_led;
    double r = 0.0, b = 0.0, g = 0.0;

    configurar_pio(pio, &offset, &sm);

    printf("Sistema inicializado. Aguardando entrada...\n");

    desenho_verde(luz_total, valor_led, pio, sm, r, g ,b);

    while (true) {
        if (!gpio_get(BTNA_PIN)){
            printf("Botão A on.\n");
            desenho_azul(luz_total, valor_led, pio, sm, r, g ,b);

            sleep_ms(50);
        }
        else if(!gpio_get(BTNB_PIN)){
            //get_led(1,0,0);
            printf("Botão B on.\n");
            gpio_put(RLED_PIN, 100);
            desenho_verde(luz_total, valor_led, pio, sm, r, g ,b);

            sleep_ms(50);
        }
        sleep_ms(5); // Atraso para evitar múltiplas leituras
    }

    return 0;

}
