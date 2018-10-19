#include <stm32f4xx_hal.h>
#include <stm32f4xx_hal_gpio.h>


void LedInit()
{
	GPIO_InitTypeDef gpioc;

	gpioc.Mode = GPIO_MODE_OUTPUT_PP;
	gpioc.Pin = GPIO_PIN_12;
	gpioc.Pull = GPIO_NOPULL;
	gpioc.Speed = GPIO_SPEED_FREQ_HIGH;

	HAL_GPIO_Init(GPIOC, &gpioc);
}


void LedSetMode(int mode)
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, mode);
}
