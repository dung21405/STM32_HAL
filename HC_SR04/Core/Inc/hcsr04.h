//
// Created by dung on 8/15/26.
//

#ifndef HC_SR04_HCSR04_H
#define HC_SR04_HCSR04_H

#include "stm32f1xx_hal.h"

typedef enum {
    HCSR04_IDLE = 0,
    HCSR04_WAIT_RISING,
    HCSR04_WAIT_FALLING,
    HCSR04_COMPLETE,
}HCSR04_state;

typedef struct {
    GPIO_TypeDef      *trig_port;
    uint16_t           trig_pin;
    GPIO_TypeDef      *echo_port;
    uint16_t           echo_pin;
    TIM_HandleTypeDef *htim;
    volatile HCSR04_state state;
    volatile float last_distance_cm;
    volatile uint8_t data_ready;
} HCSR04_HandleTypeDef;

void  HCSR04_Init(HCSR04_HandleTypeDef *hcsr, TIM_HandleTypeDef *htim, GPIO_TypeDef *trig_port, uint16_t trig_pin, GPIO_TypeDef *echo_port, uint16_t echo_pin);

void  HCSR04_Start(HCSR04_HandleTypeDef *hcsr);

void  HCSR04_EXTI_Callback(HCSR04_HandleTypeDef *hcsr, uint16_t GPIO_Pin);

void  HCSR04_Handle(HCSR04_HandleTypeDef *hcsr);

float HCSR04_GetLastDistance(HCSR04_HandleTypeDef *hcsr);

void  HCSR04_Complete_Callback(HCSR04_HandleTypeDef *hcsr, float distance_cm);

void delay_us(TIM_HandleTypeDef *htim, uint32_t us);

#endif //HC_SR04_HCSR04_H
