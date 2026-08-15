//
// Created by dung on 8/15/26.
//

#include "hcsr04.h"

#define HCSR04_US_TO_CM   0.017f
#define HCSR04_TRIG_PULSE_US   15u

__weak void HCSR04_Complete_Callback(HCSR04_HandleTypeDef *hcsr, float distance_cm)
{
    UNUSED(hcsr);
    UNUSED(distance_cm);
}

void delay_us(TIM_HandleTypeDef *htim, uint32_t us) {
    htim->Instance->CNT = 0;
    HAL_TIM_Base_Start(htim);
    while (htim->Instance->CNT < us){}
    HAL_TIM_Base_Stop(htim);
}

void HCSR04_Init(HCSR04_HandleTypeDef *hcsr, TIM_HandleTypeDef *htim, GPIO_TypeDef *trig_port, uint16_t trig_pin, GPIO_TypeDef *echo_port, uint16_t echo_pin)
{
    hcsr->htim = htim;
    hcsr->trig_port = trig_port;
    hcsr->trig_pin = trig_pin;
    hcsr->echo_port = echo_port;
    hcsr->echo_pin = echo_pin;

    hcsr->state = HCSR04_IDLE;
    hcsr->last_distance_cm = 0.0f;
    hcsr->data_ready        = 0;

    HAL_GPIO_WritePin(hcsr->trig_port, hcsr->trig_pin, GPIO_PIN_RESET);
}

void HCSR04_Start(HCSR04_HandleTypeDef *hcsr) {
    //Write is trigger than 10 ms
    if (hcsr->state == HCSR04_IDLE) {
        HAL_GPIO_WritePin(hcsr->trig_port, hcsr->trig_pin, GPIO_PIN_SET);
        delay_us(hcsr->htim, HCSR04_TRIG_PULSE_US);
        HAL_GPIO_WritePin(hcsr->trig_port, hcsr->trig_pin, GPIO_PIN_RESET);
        hcsr->state = HCSR04_WAIT_RISING;
    }
}

void HCSR04_EXTI_Callback(HCSR04_HandleTypeDef *hcsr, uint16_t GPIO_Pin)
{
    if (GPIO_Pin != hcsr->echo_pin) {
        return;
    }
    switch (hcsr->state) {

        case HCSR04_WAIT_RISING:
            if (HAL_GPIO_ReadPin(hcsr->echo_port, hcsr->echo_pin) == GPIO_PIN_SET) {
                /* bat duoc canh len -> bat dau dem thoi gian xung echo */
                hcsr->htim->Instance->CNT = 0;
                HAL_TIM_Base_Start(hcsr->htim);
                hcsr->state = HCSR04_WAIT_FALLING; /* SUA LOI: truoc la gan nham lai WAIT_RISING */
            } else {
                hcsr->state = HCSR04_IDLE;
            }
            break;

        case HCSR04_WAIT_FALLING:
            if (HAL_GPIO_ReadPin(hcsr->echo_port, hcsr->echo_pin) == GPIO_PIN_RESET) {
                /* bat duoc canh xuong -> dung dem, co ket qua */
                HAL_TIM_Base_Stop(hcsr->htim);
                hcsr->state = HCSR04_COMPLETE;
            } else {
                hcsr->state = HCSR04_IDLE;
            }
            break;
        default:
            hcsr->state = HCSR04_IDLE;
            break;
    }
}

void HCSR04_Handle(HCSR04_HandleTypeDef *hcsr)
{
    if (hcsr->state == HCSR04_COMPLETE) {
        float distance_cm = (float)hcsr->htim->Instance->CNT * HCSR04_US_TO_CM;

        hcsr->last_distance_cm = distance_cm;
        hcsr->data_ready = 1;
        HCSR04_Complete_Callback(hcsr, distance_cm);
        hcsr->state = HCSR04_IDLE;
    }
}

float HCSR04_GetLastDistance(HCSR04_HandleTypeDef *hcsr)
{
    hcsr->data_ready = 0;
    return hcsr->last_distance_cm;
}
