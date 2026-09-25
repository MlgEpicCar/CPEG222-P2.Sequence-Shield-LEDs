/****************************************************************
* Author: Carlos Munar (MlgEpicCar)
* Project 2 - Sequence 8 LEDs on CPEG222 Shield, 9/24/26
****************************************************************/

#include "stm32f4xx.h"
#include <stdbool.h>

/* Button Definitions */
#define LEFT_PIN 9 // GPIOF
#define RIGHT_PIN 6 // NOTE: Right uses GPIOE (for some fucking reason)
#define CENTER_PIN 8 // also GPIOF

/* Switch Definitions */
#define S1 8
#define S2 9
#define S3 10
#define S4 11
#define SWITCH_PORT GPIOC

/* Unlike the NUCLEO-F446ZE onboard LEDs, the CPEG222 Shield LEDs are PD0-7 (making it easy)*/
#define LED_PORT GPIOD

volatile uint32_t button_debounce = 0;

void SysTick_Handler(void) {
    /* SUPER AWESOME DEBOUNCE TIME */
    if (button_debounce == 0)
    {
        /* 100 ms debounce/cooldown */
        button_debounce = 100;
    }

    /* Decrease cooldown every millisecond */
    if (button_debounce > 0)
    {
        button_debounce--;
    }
}

int main(void)
{
    /* Enable clock for GPIOC, GPIOD, GPIOE, and GPIOF */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN |
                     RCC_AHB1ENR_GPIODEN|
                     RCC_AHB1ENR_GPIOEEN|
                     RCC_AHB1ENR_GPIOFEN;

    /* Clear PD0-7 so we can set them to general purpose output */
    LED_PORT->MODER &= ~((3U << (0 * 2)) |
                        (3U << (1 * 2)) |
                        (3U << (2 * 2)) |
                        (3U << (3 * 2)) |
                        (3U << (4 * 2)) |
                        (3U << (5 * 2)) |
                        (3U << (6 * 2)) |
                        (3U << (7 * 2)));
    /* Set PD0-7 as general purpose output (01) */
    LED_PORT->MODER |= (1U << (0 * 2))|
                    (1U << (1 * 2))|
                    (1U << (2 * 2))|
                    (1U << (3 * 2))|
                    (1U << (4 * 2))|
                    (1U << (5 * 2))|
                    (1U << (6 * 2))|
                    (1U << (7 * 2));

    /* Ensure all LEDs start off */
    LED_PORT->BSRR = (1U << (0 + 16))|
                  (1U << (1 + 16))|
                  (1U << (2 + 16))|
                  (1U << (3 + 16))|
                  (1U << (4 + 16))|
                  (1U << (5 + 16))|
                  (1U << (6 + 16))|
                  (1U << (7 + 16));

    /* Configure Buttons */
    GPIOF->MODER &= ~((3U << (LEFT_PIN * 2))|
                     (3U << (CENTER_PIN * 2)));  
    GPIOE->MODER &= ~(3U << (RIGHT_PIN * 2)); // RIGHT uses it's own GPIO port 

    /* Configure Switches */
    SWITCH_PORT->MODER &= ~((3U << (S1 * 2))|
                     (3U << (S2 * 2))|
                     (3U << (S3 * 2))|
                     (3U << (S4 * 2))); 

    /* Configure SysTick*/
    SysTick_Config(SystemCoreClock / 1000);

    while (1)
    {
        /* Turn on/off LEDs when switches are #switched */
        if (SWITCH_PORT->IDR & (1U << S1)) { // SWITCH 1 & D1(LED 1)
            LED_PORT->BSRR = (1U << 0); } else {
            LED_PORT->BSRR = (1U << (0 + 16)); }

        if (SWITCH_PORT->IDR & (1U << S2)) { // SWITCH 2 & D2(LED 2)
            LED_PORT->BSRR = (1U << 1); } else {
            LED_PORT->BSRR = (1U << (1 + 16)); }

        if (SWITCH_PORT->IDR & (1U << S3)) { // SWITCH 3 & D3
            LED_PORT->BSRR = (1U << 2); } else {
            LED_PORT->BSRR = (1U << (2 + 16)); }

        if (SWITCH_PORT->IDR & (1U << S4)) { // SWITCH 4 & D4
            LED_PORT->BSRR = (1U << 3); } else {
            LED_PORT->BSRR = (1U << (3 + 16)); }
    }
}
