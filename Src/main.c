/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"
#include "string.h"
#include "float.h"
#include "stdlib.h"
#include "stdio.h"
/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);


/* Function processing DMA Rx data. Counts how many capital and small letters are in sentence.
 * Result is supposed to be stored in global variable of type "letter_count_" that is defined in "main.h"
 *
 * @param1 - received sign
 */

void proccesDmaData(const uint8_t* sign, uint16_t len);
void print();
/* Space for your global variables. */

	// type your global variables here:
  uint8_t count = 0;
  int male = 0, velke = 0;
  int start = 0;
  int velkost = 256;
  letter_count_ letters;

  uint8_t string[20];

  uint8_t tx_data[] = "Data to send over UART DMA!\n\r";
  uint8_t rx_data[10];

  uint8_t buffer[4];
  uint8_t memory[4];
  uint8_t load[4];
  uint8_t male_pismena[4];
  uint8_t velke_pismena[4];

  uint16_t occupiedMem;
int main(void)
{
  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* Configure the system clock */
  SystemClock_Config();
  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();

  /* Space for your local variables, callback registration ...*/

  	  //type your code here:
  USART2_RegisterCallback(proccesDmaData);

  while (1)
  {
	  /* Periodic transmission of information about DMA Rx buffer state.
	   * Transmission frequency - 5Hz.
	   * Message format - "Buffer capacity: %d bytes, occupied memory: %d bytes, load [in %]: %f%"
	   * Example message (what I wish to see in terminal) - Buffer capacity: 1000 bytes, occupied memory: 231 bytes, load [in %]: 23.1%
	   */

  	   //type your code here:

	print();
  	LL_mDelay(5000);
  	
  }
  /* USER CODE END 3 */
}

void print(){
	uint8_t s1[] = "Buffer capacity: ";
	uint8_t s2[] = "bytes, occupied memory: ";
	uint8_t s3[] = "bytes, load [in %]: ";
	uint8_t s4[] = "%\n\r";
	uint8_t s5[] = "Pocet malych pismen: ";
	uint8_t s6[] = "	Pocet velkych pismen: ";
	uint8_t s7[] = "\n\r";
	uint16_t buffer_size =  DMA_USART2_BUFFER_SIZE;
	float pom_occupiedMem = occupiedMem;
	float pom_buffer_size = buffer_size;
	float percento = (pom_occupiedMem/pom_buffer_size)*100.0;

	sprintf(buffer, "%d", buffer_size);
	strcat(s1, buffer);

	USART2_PutBuffer(s1, strlen(s1));
	LL_mDelay(50);

	sprintf(memory, "%d", occupiedMem);
	strcat(s2, memory);

	USART2_PutBuffer(s2, strlen(s2));
	LL_mDelay(50);

	gcvt(percento, 6, load);
	strcat(s3, load);

	USART2_PutBuffer(s3, strlen(s3));
	LL_mDelay(50);

	USART2_PutBuffer(s4, strlen(s4));
	LL_mDelay(50);

	sprintf(male_pismena, "%d", letters.small_letter);
	strcat(s5, male_pismena);

	USART2_PutBuffer(s5, strlen(s5));
	LL_mDelay(50);

	sprintf(velke_pismena, "%d", letters.capital_letter);
	strcat(s6, velke_pismena);

	USART2_PutBuffer(s6, strlen(s6));
	LL_mDelay(50);

	USART2_PutBuffer(s7, strlen(s7));
	LL_mDelay(50);
	USART2_PutBuffer(s7, strlen(s7));
	LL_mDelay(50);
}

void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);

  if(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_0)
  {
  Error_Handler();  
  }
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {
    
  }
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
  {
  
  }
  LL_Init1msTick(8000000);
  LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);
  LL_SetSystemCoreClock(8000000);
}

/*
 * Implementation of function processing data received via USART.
 */
void proccesDmaData(const uint8_t* sign, uint16_t len)
{
	/* Process received data */
	
	uint8_t pom = 0;
	int count;
		// type your algorithm here:

	while(pom < len){
	uint8_t akt = *(sign+pom);
		if(*(sign+pom) == '#'){
			start = 1;
			male = 0;
			count = 0;
			velke = 0;
		}
		
		if(start == 1){
			if(*(sign+pom) >= 'a' && *(sign+pom) <= 'z'){
				male = male+1;
			}
			if(*(sign+pom) >= 'A' && *(sign+pom) <= 'Z'){
				velke = velke + 1;
			}

			count++;
		}

		if(*(sign+pom) == '$'){
			start = 0;


			letters.capital_letter = letters.capital_letter + velke;
			letters.small_letter = letters.small_letter + male;

		}
		
		if(count == 35 && *(sign+pom) != '$'){
			start = 0;
			male = 0;
			count = 0;
			velke = 0;
		}
		pom++;
	}
}


void Error_Handler(void)
{

}

#ifdef  USE_FULL_ASSERT

void assert_failed(char *file, uint32_t line)
{ 

}

#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
