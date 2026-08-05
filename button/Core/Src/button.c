//
// Created by dung on 8/5/26.
//
#include "button.h"

__weak void btn_pressing_callback(Button_Typedef* ButtonX);
__weak void btn_release_callback(Button_Typedef* ButtonX);
__weak void btn_press_short_callback(Button_Typedef* ButtonX);
__weak void btn_press_timeout_callback(Button_Typedef* ButtonX);

void button_init(Button_Typedef* ButtonX, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    ButtonX->GPIOx = GPIOx;
    ButtonX->GPIO_Pin = GPIO_Pin;
    ButtonX->btn_current = 1;
    ButtonX->btn_last =1;
    ButtonX->btn_filter =1;
}

void button_handle(Button_Typedef* ButtonX)
{
    /*  ----------------- Loc nhieu ----------------------*/
    uint8_t sta = HAL_GPIO_ReadPin(ButtonX->GPIOx, ButtonX->GPIO_Pin);
    if(sta != ButtonX->btn_filter)
    {
        ButtonX->btn_filter = sta;
        ButtonX->is_debouncing = 1;
        ButtonX->time_debounce = HAL_GetTick();
    }
    if(ButtonX->is_debouncing && (HAL_GetTick() - ButtonX->time_debounce >=15))
    {
        ButtonX->btn_current = ButtonX->btn_filter;
        ButtonX->is_debouncing = 0;
    }
    /*-------------- Xu li------------------------------*/
    if(ButtonX->btn_current != ButtonX->btn_last)
    {
        if(ButtonX->btn_current == 0) // Nhan xuong
        {
            btn_pressing_callback(ButtonX);
            ButtonX->time_start_press = HAL_GetTick();
            ButtonX->is_press_timeout = 1;
        }
        else  // Nha nut
        {
            if(HAL_GetTick() - ButtonX->time_start_press <= 1000)
            {
                btn_press_short_callback(ButtonX);
            }
            btn_release_callback(ButtonX);
            ButtonX->is_press_timeout = 0;
        }
        ButtonX->btn_last = ButtonX->btn_current;
    }
    if(ButtonX->is_press_timeout && (HAL_GetTick() - ButtonX->time_start_press > 3000))
    {
        btn_press_timeout_callback(ButtonX);
        ButtonX->is_press_timeout = 0;
    }
}

