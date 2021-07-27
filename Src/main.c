/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#include "FLASH_SECTOR_H7.h"
#include <string.h>
#include <stdlib.h>
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART1_UART_Init(void);
int BufferCheck(char *arr);
void Run();
void EndlessCom(void);
void afterReady(void);
uint32_t Read_From_Flash(uint32_t f_address);
void Write_To_Flash(uint32_t F_Address, uint32_t F_Data);

/* Enum Struct vs -------------------------------------------------------------------------------------*/


typedef enum {
	false, true
} bool;

typedef enum {
	Null, WAITING_FIRST, FIRST_RECIEVED

} Status;



struct UsersData {
    int ID;
    char field [15];
    struct UsersData   *next;
};
/* -------------------------------------------------------------------------------------*/



/* Variables -------------------------------------------------------------------------------------------*/
uint32_t *flashdata = "Flash Deneme";

int32_t FLASH_START_ADR = (uint32_t *)0x08040000;
char FlashRX[30];
char rxBuf[30];
char txBuf[30] = "Welcome";
char controlStr[30] = "Welcome received from stm32";
uint8_t MainBuf[30];



/*  -------------------------------------------------------------------------------------------*/








int main(void) {
	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */


	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_USART1_UART_Init();
	/* USER CODE BEGIN 2 */
	/* USER CODE END 2 */
//	Flash();
	Run();
	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {

	if (huart->Instance == USART1) {
		memcpy(MainBuf, (uint8_t*) rxBuf, Size);

		HAL_UARTEx_ReceiveToIdle_DMA(&huart1, (uint8_t*) rxBuf, sizeof(rxBuf));
		__HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
	}

}

void Run() {

	bool flag = true;
	bool firstCheck = true;
	bool isReady = false;

	while (flag) {

		int content =BufferCheck(rxBuf) ;



		if (content == WAITING_FIRST && firstCheck) {

			if (!isReady) {
				char readyMessage[20] = "Ready";
				HAL_Delay(500);
				HAL_UART_Transmit(&huart1, (uint8_t*) readyMessage,
						strlen(readyMessage), 1000);

				char toReceive[20];
				HAL_UARTEx_ReceiveToIdle_DMA(&huart1, (uint8_t*) toReceive,
						sizeof(toReceive));


				if (!strcmp(toReceive, "Ready2")) {
				isReady = true;
				HAL_Delay(5000);
				HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);

				}
				strcpy(toReceive, "");

			} else {
				HAL_Delay(50);
								HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);

				HAL_UART_Transmit(&huart1, (uint8_t*) txBuf, strlen(txBuf),
						1000);
				HAL_Delay(100);
				HAL_UARTEx_ReceiveToIdle_DMA(&huart1, (uint8_t*) rxBuf,
						sizeof(rxBuf));

				__HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
				HAL_Delay(200);
				if(strcmp(rxBuf,"Error") != 0){
					firstCheck = false;

				}
				memset(rxBuf, 0, strlen(rxBuf));
			}

		}
		if (content == FIRST_RECIEVED) {
	//		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
			memset(rxBuf,0,strlen(rxBuf));
			HAL_Delay(100);
			char ThreadDeneme[40] = "UI begin";
			HAL_UART_Transmit(&huart1, (uint8_t*) ThreadDeneme, sizeof(ThreadDeneme),	1000);
			HAL_Delay(100);
			HAL_UARTEx_ReceiveToIdle_DMA(&huart1, (uint8_t*) rxBuf,
									sizeof(rxBuf));;
			if(strcmp(rxBuf,"UI begin2")){
				afterReady();
			}

		//	EndlessCom();


		}

	}

}



void EndlessCom(){
	while(1){
		char EndlessRX[40] = {};

	//	char EndlessTX[40] ="Lorem ipsum dolor sit amet, consectetur";
	//	HAL_UART_Transmit(&huart1, (uint8_t*) EndlessTX, sizeof(EndlessTX),	1000);
		HAL_Delay(200);
		HAL_UARTEx_ReceiveToIdle_DMA(&huart1, (uint8_t*) EndlessRX,
									sizeof(EndlessRX));
		if(strcmp(EndlessRX,"I don't know latin") ==0){
	//		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
			HAL_Delay(400);
		//	HAL_UART_Transmit(&huart1, (uint8_t*) EndlessTX, sizeof(EndlessTX),	1000);
			}
	}
}
void afterReady(){

	while(1){
		memset(rxBuf,0,strlen(rxBuf));
		HAL_UARTEx_ReceiveToIdle_DMA(&huart1, (uint8_t*) rxBuf,
						sizeof(rxBuf));;
		HAL_Delay(300);

		if(strcmp(rxBuf,"On") == 0){


			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);

		}
		if(0==strcmp(rxBuf,"Off")){
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
		}
	}

}


int BufferCheck(char *arr) {

	Status current;
	/*if (arr[0] == '\0') {
		current = WAITING_FIRST;
		return 1;
	}*/
	if(current == WAITING_FIRST) {

		for(int i=0;i<strlen(controlStr); i++){
			current = FIRST_RECIEVED;
			if(arr[i] != controlStr[i]){

				current = 	WAITING_FIRST;
				break;
			}
		}


	}
	if(strcmp(rxBuf,controlStr)==0){
		current = FIRST_RECIEVED;
	}
	return current;

}


int StructSize(struct UsersData user){
	int IDsize =  sizeof(user.ID);
	int fieldLenght = strlen(user.field);
	int nextRef = 8;
	return IDsize+fieldLenght+nextRef;
}



void Flash(){
	myDeletefunction();

	/* HAL_UARTEx_ReceiveToIdle_DMA(&huart1, (uint8_t*) rxBuf,sizeof(rxBuf));
	char totalData[40];
	strcpy(totalData,rxBuf);
	memset(rxBuf,0,strlen(rxBuf));

	struct UsersData *NewUser = NULL;
	NewUser = malloc(sizeof(struct UsersData));

   char * token = strtok(totalData, " ");
   int ID_recieved = atoi(token);
   NewUser->ID = ID_recieved;
   token = strtok(NULL, " ");
   strcpy(NewUser->field,token);
   Flash_Write_Data(FLASH_START_ADR,(uint32_t *)Erhan , sizeof( struct UsersData));

	Flash_Read_Data(FLASH_START_ADR,(uint32_t *)userArr, sizeof( struct UsersData));

 */

	// Size of struct = 12
	struct UsersData *Doruk = NULL;
	struct UsersData *Erhan = NULL;
	struct UsersData *Ali = NULL;


	Erhan = malloc(sizeof(struct UsersData));
	Doruk = malloc(sizeof(struct UsersData));
	Ali = malloc(sizeof(struct UsersData));

	Doruk-> ID= 2;
	strcpy(Doruk->field,"Stajyer");
//	Doruk-> field = "Stajyer";
	Doruk-> next = NULL;

	Erhan->ID = 1;
	strcpy(Erhan->field,"Kidemli");
//	Erhan->field = "Kidemli";
	Erhan->next = Doruk;

	strcpy(Ali->field,"Manager");

	struct UsersData userArr [10];





	char * controlString;


	int a = StructSize(*Doruk);
	uint32_t addressAfterWrite;

	addressAfterWrite = Flash_Write_Data(FLASH_START_ADR,(uint32_t *)Doruk , StructSize(*Doruk));

	Flash_Read_Data(FLASH_START_ADR,(uint32_t *)userArr,StructSize(*Doruk));

	controlString = userArr[0].field;

	FLASH_START_ADR = addressAfterWrite;

	Flash_Write_Data(FLASH_START_ADR,(uint32_t *)Erhan , StructSize(*Erhan));

	Flash_Read_Data(FLASH_START_ADR,(uint32_t *)userArr, StructSize(*Erhan));


	myDeletefunction();


	controlString =  userArr[0].field;


	char * merhaba = "merhaba";

}




/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Configure the main internal regulator output voltage
	 */
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief USART1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART1_UART_Init(void) {

	/* USER CODE BEGIN USART1_Init 0 */

	/* USER CODE END USART1_Init 0 */

	/* USER CODE BEGIN USART1_Init 1 */

	/* USER CODE END USART1_Init 1 */
	huart1.Instance = USART1;
	huart1.Init.BaudRate = 9600;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart1) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN USART1_Init 2 */

	/* USER CODE END USART1_Init 2 */

}

/**
 * Enable DMA controller clock
 */
static void MX_DMA_Init(void) {

	/* DMA controller clock enable */
	__HAL_RCC_DMA2_CLK_ENABLE();

	/* DMA interrupt init */
	/* DMA2_Stream2_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

	/*Configure GPIO pin : PA5 */
	GPIO_InitStruct.Pin = GPIO_PIN_5;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
