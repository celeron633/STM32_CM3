/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>

#include "atomic.h"
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

/* USER CODE BEGIN PV */
extern UART_HandleTypeDef huart1;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int __io_putchar(int ch)
{
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
  return 1;
}

void setValue(int *ptr, int val)
{
  asm volatile
  (
    "ldr r0, %1\n"
    "str r0, [%0]\n"
    :
    :"p"(ptr), "m"(val)
    : "r0"
  );
}

void setValue2(int *ptr, int val)
{
  asm volatile
  (
    "mov r0, %1\n"
    "str r0, [%0]\n"
    :
    :"r"(ptr), "r"(val)
    : "r0", "r1"
  );
}

void ldrTest(void)
{
  uint8_t arr[] = {0x12, 0x34, 0x56, 0x78};
  int outVal = 0;

  printf("&arr=0x%08x\r\n", (int)arr);

  asm volatile
  (
    "mov r0, %1\n"
    "ldr r1, [r0]\n"
    "mov %0, r1\n"
    :"=r"(outVal)
    :"r"(arr)
    :"r0", "r1"
  );
  printf("outVal=0x%08x\r\n", outVal);
  // outVal=0x78563412

  outVal = 0;
  asm volatile
  (
    "mov r0, %1\n"
    "ldrh r1, [r0]\n"
    "mov %0, r1\n"
    :"=r"(outVal)
    :"r"(arr)
    :"r0", "r1"
  );
  printf("outVal=0x%08x\r\n", outVal);
  // outVal=0x00003412

  outVal = 0;
  int arrAddr = (int)arr;
  asm volatile
  (
    "ldr r0, %1\n"
    "ldr r1, [r0]\n"
    "mov %0, r1\n"
    :"=r"(outVal)
    :"m"(arrAddr)
    :"r0"
  );
  printf("outVal=0x%08x\r\n", outVal);
  // outVal=0x78563412

  outVal = 0;
  char *arrAddr2 = (char *)&arr;
  asm volatile
  (
    "ldr r0, %1\n"
    "mov %0, r0\n"
    :"=r"(outVal)
    :"m"(arrAddr2)
    :"r0"
  );
  printf("outVal=0x%08x\r\n", outVal);
  // outVal=0x2000ffd0

}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

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
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  printf("hello world\r\n");

  int mspVal;
  int pspVal;
  int inputListVal = 0;

  int xPSRVal = 0;

  // volatile: 汇编不进行优化, 保持原样
  asm volatile (
    "mrs r0, msp\n\t"
    "str r0, %0\n\t"
    "mrs r0, psp\n\t"
    "str r0, %1\n\t"
    "mov r0, #3\n\t"
    "str r0, [%3]\n\t"
    "movs r1, #1\n\t"
    "subs r1, r1, #2\n\t"
  
    "mrs r0, APSR\n\t"
    "str r0, %2"
    // 输出列表
    :"=m"(mspVal), "=m"(pspVal), "=m"(xPSRVal)
    // 输入列表
    :"r"(&inputListVal)
    // 破坏描述
    : "r0", "r1", "memory"
    );

    printf("msp=%08x, psp=%08x\r\n", mspVal, pspVal);
    printf("inputListVal=%08x\r\n", inputListVal);
    printf("xPSR=%08x\r\n", xPSRVal);


    int testVal = 0;
    printf("testVal=%d\r\n", testVal);
    setValue(&testVal, 100);
    printf("testVal=%d\r\n", testVal);
    setValue2(&testVal, 200);
    printf("testVal=%d\r\n", testVal);

    // ldrTest();
    atomic_test();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
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
