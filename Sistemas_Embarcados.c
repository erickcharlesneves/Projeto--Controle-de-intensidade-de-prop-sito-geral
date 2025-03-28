
/*********************************| includes |*****************************/
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "hardware/gpio.h"

//Bibliotecas do buzzer
#include "hardware/pwm.h"
#include "hardware/clocks.h"

#include "display/inc/ssd_1306.h"
#include "display/inc/ssd_1306_font.h"
#include "joystick/inc/joystick.h"

//Variáveis do buzzer
//-----------------------------------

// Configuração do pino do buzzer
#define BUZZER_PIN 21

// Configuração da frequência do buzzer (em Hz)
#define BUZZER_FREQUENCY 4000

void pwm_init_buzzer(uint pin) {
    gpio_set_function(pin, GPIO_FUNC_PWM);
    
    uint slice_num = pwm_gpio_to_slice_num(pin);

    pwm_config config = pwm_get_default_config();

    // Definição do TOP (resolução do PWM)
    uint16_t top = 4096; // 12 bits de resolução

    // Cálculo do divisor de clock correto
    float clkdiv = (float) clock_get_hz(clk_sys) / (BUZZER_FREQUENCY * top);

    pwm_config_set_clkdiv(&config, clkdiv);
    pwm_config_set_wrap(&config, top - 1); // Define o período do PWM

    pwm_init(slice_num, &config, true);
    pwm_set_gpio_level(pin, 0); // Inicia com som desligado
}

// Definição de uma função para emitir um beep com duração especificada
void beep(uint pin, uint duration_ms) {
    // Obter o slice do PWM associado ao pino
    uint slice_num = pwm_gpio_to_slice_num(pin);

    // Configurar o duty cycle para 50% (ativo)
    pwm_set_gpio_level(pin, 4080);

    // Temporização
    sleep_ms(duration_ms);

    // Desativar o sinal PWM (duty cycle 0)
    pwm_set_gpio_level(pin, 0);

    // Pausa entre os beeps
    sleep_ms(100); // Pausa de 100ms
}

int map(int valor, int in_min, int in_max, int out_min, int out_max) {
    return (valor - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


//-----------------------------------

/*********************************| variaveis |*****************************/
uint16_t vrx_value, vry_value;

/*********************************| cabecalho das uncoes |******************/

//----------------------
//Média móvel

#define MEDIA_MOVEL_N 10  // Tamanho da janela da média móvel

uint16_t buffer_vrx[MEDIA_MOVEL_N] = {0};
uint8_t indice_vrx = 0;
uint32_t soma_vrx = 0;

uint16_t media_movel_vrx(uint16_t novo_valor) {
    soma_vrx -= buffer_vrx[indice_vrx];  // Remove o valor antigo da soma
    buffer_vrx[indice_vrx] = novo_valor; // Insere o novo valor no buffer
    soma_vrx += novo_valor;              // Adiciona o novo valor à soma

    indice_vrx = (indice_vrx + 1) % MEDIA_MOVEL_N; // Atualiza o índice circular

    return soma_vrx / MEDIA_MOVEL_N; // Retorna a média móvel
}

//----------------------

//---------------------
//Configuração dos leds RGB

// Função para atualizar os estados dos LEDs
void set_led_color(uint red_pin, uint green_pin, uint blue_pin, bool R, bool G, bool B) {
    gpio_put(red_pin, R);   // Configura o estado do LED vermelho
    gpio_put(green_pin, G); // Configura o estado do LED verde
    gpio_put(blue_pin, B);  // Configura o estado do LED azul
} 

//---------------------

int main(){

    //-------------------------------
    //Configuração dos Leds RGB

    // Configuração dos pinos GPIO
    const uint red_pin = 13;   // Pino para o LED vermelho
    const uint green_pin = 11; // Pino para o LED verde
    const uint blue_pin = 12;  // Pino para o LED azul

    gpio_init(red_pin);
    gpio_init(green_pin);
    gpio_init(blue_pin);

    gpio_set_dir(red_pin, GPIO_OUT);
    gpio_set_dir(green_pin, GPIO_OUT);
    gpio_set_dir(blue_pin, GPIO_OUT);

    //-------------------------------

    stdio_init_all();
    joystick_init();
    ssd_1306_init();

    sleep_ms(1000);
    
    // Inicializar o PWM no pino do buzzer
    pwm_init_buzzer(BUZZER_PIN);

    while (true) {

        joystick_read_x(&vrx_value);
        vrx_value = media_movel_vrx(vrx_value);

        joystick_read_y(&vry_value);
        sleep_ms(1);

        if(joystick_read_button()){

            gpio_init(PIN_LED_A);
            gpio_set_dir(PIN_LED_A, GPIO_OUT);
            gpio_put(PIN_LED_A, true);
            sleep_ms(500);
        }

        ssd_1306_fill(white);
        ssd_1306_draw_rectangle(0, 0, 127, 63, black);
        char string_num_x[5];
        char string_num_y[5];

        sprintf(string_num_x, "%u", vrx_value);
        sprintf(string_num_y, "%u", vry_value);

        int valor_mapeado = map(vrx_value, 2100, 4096, 0, 500); //Antes era 3600

        int saida_para_o_display_2 = 0;
        char display_str[4];

        if (vrx_value >= 2100) {
            saida_para_o_display_2 = map(vrx_value, 2100, 4096, 0, 100);
        }
        else {
            saida_para_o_display_2 = 0;
        }

        sprintf(display_str, "%d", saida_para_o_display_2);

        pwm_set_gpio_level(BUZZER_PIN, valor_mapeado);

        ssd_1306_set_cursor(35, 10);
        ssd_1306_write_string(display_str, Font_7x10, black);

        ssd_1306_set_cursor(10, 10);
        ssd_1306_write_string("i =", Font_7x10, black);

        int saida_do_display_mapeada = 0;

        if (vrx_value >= 2100) {
            saida_do_display_mapeada = map(vrx_value, 2100, 4096, 0, 100);
        }
        else {
            saida_do_display_mapeada = 0;
        }

        if (saida_do_display_mapeada <= 25) {
            // Variáveis binárias para os estados dos LEDs - começa com todos apagados
            bool R = 0; // Vermelho
            bool G = 0; // Verde
            bool B = 0; // Azul 
            set_led_color(red_pin, green_pin, blue_pin, R, G, B);
        }
        if (saida_do_display_mapeada > 25) {
            // Variáveis binárias para os estados dos LEDs - começa com todos apagados
            bool R = 0; // Vermelho
            bool G = 1; // Verde
            bool B = 0; // Azul 
            set_led_color(red_pin, green_pin, blue_pin, R, G, B);
        }
        if (saida_do_display_mapeada > 50) {
            // Variáveis binárias para os estados dos LEDs - começa com todos apagados
            bool R = 1; // Vermelho
            bool G = 1; // Verde
            bool B = 0; // Azul 
            set_led_color(red_pin, green_pin, blue_pin, R, G, B);
        }
        if (saida_do_display_mapeada > 75) {
            // Variáveis binárias para os estados dos LEDs - começa com todos apagados
            bool R = 1; // Vermelho
            bool G = 0; // Verde
            bool B = 0; // Azul 
            set_led_color(red_pin, green_pin, blue_pin, R, G, B);
        }

        /* ssd_1306_draw_rectangle(0, 50, saida_do_display_mapeada, 25, black); */
        ssd_1306_fill_rectangle(0, 50, saida_do_display_mapeada, 25, black);
        ssd_1306_up_date_screen();
        sleep_ms(5);
        
    }
}
