//
// Created by dung on 8/5/26.
//

#ifndef BUTTON_BUTTON_H
#define BUTTON_BUTTON_H
#include "stm32f1xx_hal.h"
/*******************************************************************************
* Structure Name  : Button_Typedef
* Description     : Holds button state and timing information for debouncing
*                   and press-duration detection. Also stores GPIO port/pin.
* Members         : btn_current      - Current debounced button state (0 or 1)
*                   btn_last         - Previous button state
*                   btn_filter       - Software filter accumulator for noisy input
*                   is_debouncing    - Flag indicating debounce in progress (0/1)
*                   time_debounce    - Debounce timer / timestamp (ms)
*                   time_start_press - Timestamp when button press started (ms)
*                   is_press_timeout - Flag set when a press hold timeout occurred (0/1)
*                   GPIOx            - Pointer to GPIO port (e.g., GPIOA)
*                   GPIO_Pin         - GPIO pin (e.g., GPIO_PIN_0)
* Return          : None
*******************************************************************************/
typedef struct
{
    uint8_t btn_current;
    uint8_t btn_last;
    uint8_t btn_filter;  // Luu lai gia tri khi con nhieu.
    uint32_t is_debouncing;
    uint32_t time_debounce;
    uint32_t time_start_press;
    uint8_t is_press_timeout;
    GPIO_TypeDef *GPIOx;
    uint16_t GPIO_Pin;
}Button_Typedef;

/*******************************************************************************
* Function Name  : button_init
* Description    : Initialize a Button_Typedef structure.
*                  Configure GPIO port/pin and set initial filter, debounce and timeout values.
* Input          : ButtonX   - pointer to Button_Typedef to initialize (must not be NULL)
*                  GPIOx     - GPIO port (e.g. GPIOA)
*                  GPIO_Pin  - GPIO pin (e.g. GPIO_PIN_0)
* Return         : None
*******************************************************************************/
void button_init(Button_Typedef* ButtonX, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

/*******************************************************************************
* Function Name  : button_handle
* Description    : Process the button state machine; should be called periodically
*                  (e.g., from SysTick or a timer interrupt). Performs debounce
*                  filtering, updates btn_current/btn_last, tracks press duration,
*                  and detects press timeout conditions.
* Input          : ButtonX - pointer to Button_Typedef to process (must not be NULL)
* Return         : None
*******************************************************************************/
void button_handle(Button_Typedef* ButtonX);

void btn_pressing_callback(const Button_Typedef* ButtonX);
void btn_release_callback(const Button_Typedef* ButtonX);
void btn_press_short_callback(const Button_Typedef* ButtonX);
void btn_press_timeout_callback(const Button_Typedef* ButtonX);

#endif //BUTTON_BUTTON_H
