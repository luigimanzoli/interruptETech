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
#define BTNJ_PIN 22 // Botão do Joystick

// counter para mudança dos números da matriz
int counter = 0;

// counter para mudança das cores
int count_color = 0;

// Valores para definição das cores da matriz
static volatile double r = 1.0; 
static volatile double g = 1.0; 
static volatile double b = 1.0;

// Variável ligada ao debounce dos botões
static volatile uint32_t last_time = 0; 

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

    gpio_init(BTNJ_PIN);
    gpio_set_dir(BTNJ_PIN, GPIO_IN);
    gpio_pull_up(BTNJ_PIN);
}

// Matriz com todos os dígitos
double digits[10][25] = {
{ // Digito 0
    0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 1.0, 0.0, 1.0, 0.0,
    0.0, 1.0, 0.0, 1.0, 0.0,
    0.0, 1.0, 0.0, 1.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0
},
{ // Digito 1
    0.0, 0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 1.0, 0.0,
    0.0, 0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0
},
{ // Digito 2
    0.0, 0.0, 1.0, 0.0, 0.0,
    0.0, 1.0, 0.0, 1.0, 0.0,
    0.0, 0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 1.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0
},
{ // Digito 3
    0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 1.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 1.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0
},
{ // Digito 4
    0.0, 1.0, 0.0, 1.0, 0.0,
    0.0, 1.0, 0.0, 1.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 1.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 1.0, 0.0
},
{ // Digito 5
    0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 1.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 1.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0
},
{ // Digito 6
    0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 1.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 1.0, 0.0, 1.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0
},
{ // Digito 7
    0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 1.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0, 0.0
},
{ // Digito 8
    0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 1.0, 0.0, 1.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 1.0, 0.0, 1.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0
},
{ // Digito 9
    0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 1.0, 0.0, 1.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 1.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 1.0, 0.0
}

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

// Inicializa o sistema de clock
void clock_init() {
    bool ok = set_sys_clock_khz(100000, false);
    if (ok) {
        printf("Clock setado para %ld Hz\n", clock_get_hz(clk_sys));
    } else {
        printf("Falha ao configurar o clock\n");
    }
}

// Configura a PIO
void pio_config(PIO pio, uint *offset, uint *sm) {
    *offset = pio_add_program(pio, &interrupt_program);
    *sm = pio_claim_unused_sm(pio, true);
    interrupt_program_init(pio, *sm, *offset, OUT_PIN);
}

void print_digit(int digit, PIO pio, uint sm, double r, double g, double b){
    // Valor para intensidade dos LEDs
    int ity = 1;

    // Iniciando a variável que detém informação das cores de cada LED da matriz
    uint32_t led_value;

    // Condição para que os valores não ultrapassem o intervalor desejado
    if (digit <= 9 && digit >= 0){
        for (int16_t i = 0; i < NUM_PIXELS; i++) {

            // Define a cor dos LEDs baseados nos valores de r, g e b
            led_value = matrix_rgb(b*ity*(digits[digit][24 - i]), r*ity*(digits[digit][24 - i]), g*ity*(digits[digit][24 - i]));
            pio_sm_put_blocking(pio, sm, led_value); // Envia o valor para o LED

        }
    } else if (digit < 0) {
        printf("Valor incompatível.\n");
        counter = 0;
    } else if (digit > 9){
        printf("Valor incompatível.\n");
        counter = 9;
    }
}

// Inicialização do PIO e das variáveis necessárias
PIO pio = pio0;
uint32_t led_value;
uint offset, sm;

// Função que é chamada quando ocorre a interrupção
void gpio_irq_handler(uint gpio, uint32_t events){

    // Definição da variável do tempo atual do sistema dês do começo
    uint32_t current_time = to_us_since_boot(get_absolute_time());
        if (current_time - last_time > 200000){
            last_time = current_time;
            
            // Se o botão A for ativado, acrescenta na contagem
            if (gpio == BTNA_PIN){

                counter++;
                print_digit(counter, pio, sm, r, g, b);
                printf("counter = %i\n", counter);

            }
            // Se o botão B for ativado, descresce na contagem   
            else if (gpio == BTNB_PIN){

                counter = counter - 1;
                print_digit(counter, pio, sm, r, g, b);
                printf("counter = %i\n", counter);

            } 
            // Se o botão do Joystick for ativado, a cor dos números da contagem muda
            else if (gpio == BTNJ_PIN){
                count_color++;
                printf("Mudança de cor. Count_color = %i\n", count_color);
                if (count_color > 3){
                    count_color = 0;
                    r = 1.0;
                    g = 1.0;
                    b = 1.0;
                    print_digit(counter, pio, sm, r, g, b);
                }
                else if (count_color == 1){
                    r = 1.0;
                    g = 0.0;
                    b = 0.0;
                    print_digit(counter, pio, sm, r, g, b);
                }
                else if (count_color == 2){
                    r = 0.0;
                    g = 1.0;
                    b = 0.0;
                    print_digit(counter, pio, sm, r, g, b);
                }
                else if (count_color == 3){
                    r = 0.0;
                    g = 0.0;
                    b = 1.0;
                    print_digit(counter, pio, sm, r, g, b);
                }
            }
    }

}

// Função principal
int main() {
    // Inicializa clock, stdio e configurações
    stdio_init_all();
    init_all();
    clock_init();

    double r = 0.0, b = 0.0, g = 0.0;

    pio_config(pio, &offset, &sm);

    printf("Sistema inicializado.\n");

    // Configuração dos botões como interrupções
    gpio_set_irq_enabled_with_callback(BTNA_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(BTNB_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(BTNJ_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    // Iniciando o sistema escrevendo 0 na matriz de LEDs
    print_digit(0, pio, sm, 1, 1, 1);

    // Rotina inicial do programa para teste
    gpio_put(RLED_PIN, 1);
    sleep_ms(300);
    gpio_put(RLED_PIN, 0);

    while (true) {
        /*
        if (!gpio_get(BTNA_PIN)){
            counter++;
            print_digit(counter, pio, sm);
        }
        else if (!gpio_get(BTNB_PIN)){
            counter--;
            print_digit(counter, pio, sm);
        }
        */
       // Repetição para que o LED vermelho pisque 5 vezes por segundo.
       for (int i = 0; i < 5; i++){
            gpio_put(RLED_PIN, 1);
            sleep_ms(100);
            gpio_put(RLED_PIN, 0);
            sleep_ms(100);
       }
    }

    return 0;

}
