// Universidad del Valle de Guatemala
// BE3023 Digital 2
// Marcela Tobar 22297
// Cristopher Portillo 22071
// Laboratorio 2
// Micro: ESP32 DEV Kit 1.0
// Enciende leds con los botones

[env:esp32dev]
platform = espressif32
board = esp32dev
framework = ESP-IDF

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define TAG "ESP32_LED"

// Definiciones de pines
#define BUTTON_PIN1 12
#define BUTTON_PIN2 25
#define BUTTON_PIN3 13
#define LED_PIN1 32
#define LED_PIN2 33
#define LED_PIN3 26
#define LED_PIN4 27

// Configuración de pines y estado inicial
#define GPIO_OUTPUT_PIN_SEL ((1ULL << LED_PIN1) | (1ULL << LED_PIN2) | (1ULL << LED_PIN3) | (1ULL << LED_PIN4))
#define GPIO_INPUT_PIN_SEL ((1ULL << BUTTON_PIN1) | (1ULL << BUTTON_PIN2) | (1ULL << BUTTON_PIN3))

int buttonState1 = 0;
int lastButtonState1 = 0;
int buttonState2 = 0;
int lastButtonState2 = 0;
int buttonState3 = 0;
int lastButtonState3 = 0;
int mode = 0;
int ledIndex = -1;
int counter = 0;

void configure_pins() {
    // Configurar los pines de los LEDs como salida
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);

    // Configurar los pines de los botones como entrada con pull-up
    io_conf.intr_type = GPIO_INTR_ANYEDGE;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
    io_conf.pull_up_en = 1;
    gpio_config(&io_conf);
}

void button_task(void *pvParameter) {
    while (1) {
        // Lee el estado actual de los botones
        buttonState1 = gpio_get_level(BUTTON_PIN1);
        buttonState2 = gpio_get_level(BUTTON_PIN2);
        buttonState3 = gpio_get_level(BUTTON_PIN3);

        // Verifica si el estado del botón 2 ha cambiado
        if (buttonState2 != lastButtonState2) {
            if (buttonState2 == 0) { // LOW
                mode = !mode;
                ledIndex = -1;
                counter = 0;
                for (int i = LED_PIN1; i <= LED_PIN4; i++) {
                    gpio_set_level(i, 0);
                }
                if (mode == 0) {
                    ESP_LOGI(TAG, "Mode: One LED at a time");
                } else {
                    ESP_LOGI(TAG, "Mode: Binary Counter");
                }
            }
        }

        // Verifica si el estado del botón 1 ha cambiado
        if (buttonState1 != lastButtonState1) {
            if (buttonState1 == 0) { // LOW
                if (mode == 0) {
                    for (int i = LED_PIN1; i <= LED_PIN4; i++) {
                        gpio_set_level(i, 0);
                    }
                    ledIndex = (ledIndex + 1) % (4 + 1); // 4 LEDs + estado de "ninguna luz"
                    if (ledIndex != 4) {
                        gpio_set_level(LED_PIN1 + ledIndex, 1);
                    }
                } else {
                    counter = (counter + 1) % (1 << 4);
                    for (int i = 0; i < 4; i++) {
                        gpio_set_level(LED_PIN1 + i, (counter >> i) & 1);
                    }
                }
            }
        }

        // Verifica si el estado del botón 3 ha cambiado
        if (buttonState3 != lastButtonState3) {
            if (buttonState3 == 0) { // LOW
                if (mode == 0) {
                    for (int i = LED_PIN1; i <= LED_PIN4; i++) {
                        gpio_set_level(i, 0);
                    }
                    ledIndex = (ledIndex - 1 + 4 + 1) % (4 + 1); // 4 LEDs + estado de "ninguna luz"
                    if (ledIndex != 4) {
                        gpio_set_level(LED_PIN1 + ledIndex, 1);
                    }
                } else {
                    counter = (counter - 1 + (1 << 4)) % (1 << 4);
                    for (int i = 0; i < 4; i++) {
                        gpio_set_level(LED_PIN1 + i, (counter >> i) & 1);
                    }
                }
            }
        }

        // Almacena los estados actuales de los botones como los últimos estados
        lastButtonState1 = buttonState1;
        lastButtonState2 = buttonState2;
        lastButtonState3 = buttonState3;

        // Pequeño retardo para evitar el rebote del botón
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

void app_main(void) {
    configure_pins();
    xTaskCreate(&button_task, "button_task", 2048, NULL, 5, NULL);
}