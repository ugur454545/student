#include "stm32f4xx_hal.h"
#include <stdio.h>
#include "MAX7219.h"

SPI_HandleTypeDef hspi1;

extern ChrArr asciiCharacters[];
unsigned char Array[100];
unsigned char Disp[250];
unsigned char lengthStr;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);

void Arrange(unsigned char *s)
{
	unsigned char i = 0, j = 0, k, m;
	unsigned char *p = (unsigned char *) &Disp[0];

	while(*s)
	{
		k = *s - ' ';
		i = asciiCharacters[k].ChrNo + j;

		for(m=0; j<i; j++,m++)
		{
			*p++ = asciiCharacters[k].ChrSet[m];
		}

		*p++ = 0;
		s++;

		lengthStr += (asciiCharacters[k].ChrNo + 1);
	}
}


void delay(void)
{
	uint32_t i=0;
	for(i=0; i<2000000; i++);
}


void Shift(void)
{
	unsigned char i,j;

	j =  Disp[0];
	for(i=0; i<(lengthStr-1); i++)
		Disp[i] = Disp[i+1];

	Disp[(lengthStr-1)] = j;
}


int main(void)
{
	uint8_t Buffer[DISPLAY_NUM];
	
  HAL_Init();

  SystemClock_Config();
	
  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_0);
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);

  MX_GPIO_Init();
  MX_SPI1_Init();
	
	MAX7219_INIT();
	
  sprintf((char *)Array, "  15 DK ICIN NOT AL  HER DK YENI BILGI  CTECH HW RF");
	Arrange(Array);

  for(;;)
  {
		for(int i=0; i<DISPLAY_NUM; i++)
		{
				for(int j=0; j<DISPLAY_NUM; j++)
					Buffer[j] = Disp[i + j*8];
			
			MAX7219_SendDisplayData(DIGIT_0+i, Buffer);
		}
		
		delay();
		Shift();
  }
}


void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  __PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1
                              |RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);

}


void MX_SPI1_Init(void)
{

  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_1LINE;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLED;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED;
  HAL_SPI_Init(&hspi1);

}

void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __GPIOH_CLK_ENABLE();
  __GPIOA_CLK_ENABLE();

  /*Configure GPIO pin : PA6 */
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}
