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
#include "camera_regs.h"

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
TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// This code can be used as simple configurator for camera OV7670 from device Nucleo-F072RB over SCCB (I2C).
// (For example to get raw data from generated timings in expect mode assure by device like VGA,QVGA,QQVGA & RGB,YUV).
// RCC-MCO (PA8) have clock configured to 24MHz.
// PC2/PC3 GPIO have Open - Drain.
// Pin (Device - Camera): PC2 - SDA, PC3 - SCL, PA8 - XCLK, RESET - 3.3V, PWDN - GND (Power 3.3V).
// Tested device have added 4,7k resistors to pull-up for SDA/SCL.

// Very Thanks Good Man qitas
// https://raw.githubusercontent.com/qitas/NBCoAP/110fb9c6017a120977071c8e182acabf60b56c00/SYS/SCCB.c
// https://raw.githubusercontent.com/qitas/NBCoAP/110fb9c6017a120977071c8e182acabf60b56c00/SYS/SCCB.h

#define SCCB_ID_W 0x42
#define SCCB_ID_R 0x43
#define OV7670_OK 0
#define OV7670_ERROR 1
#define HALDELAY 50

typedef unsigned char u8;

static void delay_us(unsigned int us)
{
//	int len; for (;us > 0; us --) for (len = 0; len < 20; len++ );
	int len; for (;us > 0; us --) HAL_GetTick();
//	HAL_Delay(us);
//	uint32_t tickstart = HAL_GetTick();
//	uint32_t wait = us;
//
//	/* Add a period to guaranty minimum wait */
//	if (wait < HAL_MAX_DELAY)
//	{
//	wait += (uint32_t)(uwTickFreq);
//	}
//
//	while((HAL_GetTick() - tickstart) < wait)
//	{
//	}
}

#define SDA_PORT GPIOC
#define SCL_PORT GPIOC
#define SDA_PORT_PIN GPIO_PIN_2
#define SCL_PORT_PIN GPIO_PIN_3

#define SDA_IN()  SDAPortIN()
#define SDA_OUT() SDAPortOUT();

#define SDA_H  HAL_GPIO_WritePin(SDA_PORT, SDA_PORT_PIN, GPIO_PIN_SET)
#define SDA_L  HAL_GPIO_WritePin(SDA_PORT, SDA_PORT_PIN, GPIO_PIN_RESET)

#define SCL_H  HAL_GPIO_WritePin(SCL_PORT, SCL_PORT_PIN, GPIO_PIN_SET)
#define SCL_L  HAL_GPIO_WritePin(SCL_PORT, SCL_PORT_PIN, GPIO_PIN_RESET)

#define READ_SDA HAL_GPIO_ReadPin(SDA_PORT, SDA_PORT_PIN)

static void SDAPortIN(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	memset(&GPIO_InitStructure,0x0,sizeof(GPIO_InitStructure));
	GPIO_InitStructure.Pin = SDA_PORT_PIN;
	GPIO_InitStructure.Speed = GPIO_SPEED_LOW;
	GPIO_InitStructure.Mode =  GPIO_MODE_INPUT;
	GPIO_InitStructure.Pull =  GPIO_NOPULL;
//	GPIO_InitStructure.Pull =  GPIO_PULLUP;
//	GPIO_InitStructure.Pull =  GPIO_PULLDOWN;
	HAL_GPIO_Init(SDA_PORT,&GPIO_InitStructure);
}

static void SDAPortOUT(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	memset(&GPIO_InitStructure,0x0,sizeof(GPIO_InitStructure));
	GPIO_InitStructure.Pin = SDA_PORT_PIN;
	GPIO_InitStructure.Speed = GPIO_SPEED_LOW;
	GPIO_InitStructure.Mode =  GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(SDA_PORT,&GPIO_InitStructure);
}

#define SIO_D_SET			SDA_H
#define SIO_C_SET			SCL_H
#define SIO_D_CLR			SDA_L
#define SIO_C_CLR			SCL_L

#define SIO_D_IN			SDAPortIN()
#define SIO_D_OUT			SDAPortOUT()
#define SIO_D_STATE			READ_SDA

#define SIO_D_SET			SDA_H
#define SIO_C_SET			SCL_H
#define SIO_D_CLR			SDA_L
#define SIO_C_CLR			SCL_L

#define SIO_D_IN			SDAPortIN()
#define SIO_D_OUT			SDAPortOUT()
#define SIO_D_STATE			READ_SDA

void SCCB_Start(void)
{
//	SDA_H;
//	SCL_H;
//	delay_us(HALDELAY);
	SDA_H;
	SCL_H;
	delay_us(HALDELAY);
//	SDA_L;
	SDA_L;
	delay_us(HALDELAY);
//	SCL_L;
	SCL_L;
}

void SCCB_Stop(void)
{
//	SDA_L;
	SDA_L;
	delay_us(HALDELAY);
//	SCL_H;
	SCL_H;
	delay_us(HALDELAY);
//	SDA_H;
	SDA_H;
	delay_us(HALDELAY);
}

void SCCB_Ack(void)
{
	delay_us(HALDELAY);
	SDA_H;
	SCL_H;
	SDA_H;
	SCL_H;
	delay_us(HALDELAY);
	//SCL_L;
	SCL_L;
	delay_us(HALDELAY);
	//SDA_L;
	SDA_L;
	delay_us(HALDELAY);
}

void SCCB_No_Ack(void)
{
	delay_us(HALDELAY);
	//SDA_H;
	//SCL_H;
	SDA_H;
	SCL_H;
	delay_us(HALDELAY);
	//SCL_L;
	SCL_L;
	delay_us(HALDELAY);
	//SDA_L;
	SDA_L;
	delay_us(HALDELAY);
}

u8 SCCB_WR_Byte(u8 dat)
{
	u8 j,res;
	for(j=0;j<8;j++)
	{
		if(dat&0x80)
		{
			SDA_H;
		}
		else
		{
			SDA_L;
		}
		dat<<=1;
		delay_us(HALDELAY);
		SCL_H;
		delay_us(HALDELAY);
		SCL_L;
	}
	SDAPortIN();
	delay_us(HALDELAY);
	SCL_H;
	delay_us(HALDELAY);
	if(READ_SDA)res=OV7670_OK;
	else res=OV7670_ERROR;
	SCL_L;
	SDAPortOUT();
	return res;
}

u8 SCCB_RD_Byte(void)
{
	u8 temp=0,j;
	SDA_IN();
	for(j=8;j>0;j--)
	{
		delay_us(HALDELAY);
		SCL_H;
		temp=temp<<1;
		if(READ_SDA)temp++;
		delay_us(HALDELAY);
		SCL_L;
	}
	SDA_OUT();
	return temp;
}

u8 _sccbw(u8 reg,u8 data)
{
	u8 res=0;
	SCCB_Start();
	if(SCCB_WR_Byte(SCCB_ID_W)) res=OV7670_OK;
	delay_us(HALDELAY);
	if(SCCB_WR_Byte(reg)) res=OV7670_OK;
	delay_us(HALDELAY);
	if(SCCB_WR_Byte(data)) res=OV7670_OK;
	SCCB_Stop();
	return res;
}

u8 _sccbr(u8 reg, u8*data)
{
	u8 val=0;
	SCCB_Start();
//	SCCB_WR_Byte(SCCB_ID_W | 0x01);
	SCCB_WR_Byte(SCCB_ID_W);
//	delay_us(HALDELAY);
	SCCB_WR_Byte(reg);
//	delay_us(HALDELAY);
	SCCB_Stop();
//	delay_us(HALDELAY);
	SCCB_Start();
	SCCB_WR_Byte(SCCB_ID_R);
//	delay_us(HALDELAY);
//	SCCB_WR_Byte(0x00);
//	SCCB_Stop();
//	delay_us(HALDELAY);
//	SCCB_Start();
	*data=SCCB_RD_Byte();
	SCCB_Ack();
//	SCCB_No_Ack();
	SCCB_Stop();
	return OV7670_OK;
}

// based on https://github.com/coelamon/stm32f4_ov7670_uvc/tree/master/OV7670

void Camera_SignalError()
{
	for (int i = 0; i < 3; i++)
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
		HAL_Delay(100);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
		HAL_Delay(100);
	}
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
}

int OV7670_ReadReg(uint8_t regAddr, uint8_t *pData)
{
	if (_sccbr (regAddr, pData) == OV7670_OK)
	{
		return OV7670_OK;
	}
	else
	{
		return OV7670_ERROR;
	}
}

int OV7670_WriteReg(uint8_t regAddr, uint8_t pData)
{
	if (_sccbw  (regAddr, pData) == OV7670_OK)
	{
		return OV7670_OK;
	}
	else
	{
		return OV7670_ERROR;
	}
}

int OV7670_WriteRegList(const struct regval_t *reg_list)
{
	const struct regval_t *pReg = reg_list;
	while (pReg->reg_addr != 0xFF && pReg->value != 0xFF)
	{
		int write_result = OV7670_WriteReg(pReg->reg_addr, pReg->value);
		if (write_result != OV7670_OK)
		{
			return write_result;
		}
		pReg++;
	}
	return OV7670_OK;
}

int OV7670_Reset()
{
	HAL_Delay(100);
	uint8_t data = COM7_RESET;
	if (OV7670_WriteReg(REG_COM7, data) != OV7670_OK)
	{
		return OV7670_ERROR;
	}
	HAL_Delay(100);
	return OV7670_OK;
}

int OV7670_Config()
{
	int ov_reset_result = OV7670_Reset();
	if (ov_reset_result != OV7670_OK)
	{
		return ov_reset_result;
	}
	int ov_read_reg_result = OV7670_ERROR;
	int ov_write_reg_result = OV7670_ERROR;

	ov_write_reg_result = OV7670_WriteRegList(ov7670_default_regs);
	if (ov_write_reg_result != OV7670_OK)
	{
		return ov_write_reg_result;
	}

	uint8_t ov_com3 = 0x04; // REG_COM3 enable scaling
	ov_write_reg_result = OV7670_WriteReg(REG_COM3, ov_com3);
	if (ov_write_reg_result != OV7670_OK)
	{
		return ov_write_reg_result;
	}

	ov_write_reg_result = OV7670_WriteRegList(vga_ov7670);
	if (ov_write_reg_result != OV7670_OK)
	{
		return ov_write_reg_result;
	}

	ov_write_reg_result = OV7670_WriteRegList(yuv422_ov7670);
	if (ov_write_reg_result != OV7670_OK)
	{
		return ov_write_reg_result;
	}

	// configure PCLK to 24MHz

	uint8_t ov_clk_rc = 0;
	ov_read_reg_result = OV7670_ReadReg(REG_CLKRC, &ov_clk_rc);
	if (ov_read_reg_result != OV7670_OK)
	{
		return ov_read_reg_result;
	}
//	ov_clk_rc = (ov_clk_rc & 0x80) | 0x01; // to enable prescaler by 2
	ov_clk_rc = (ov_clk_rc & 0x80) | 0x00;
	ov_write_reg_result = OV7670_WriteReg(REG_CLKRC, ov_clk_rc);
	if (ov_write_reg_result != OV7670_OK)
	{
		return ov_write_reg_result;
	}

	uint8_t ov_dblv = 0;
	ov_read_reg_result = OV7670_ReadReg(REG_DBLV, &ov_dblv);
	if (ov_read_reg_result != OV7670_OK)
	{
		return ov_read_reg_result;
	}
//	ov_dblv = (ov_dblv & 0x3F) | DBLV_PLL6; // to enable PLL x6
	ov_dblv = (ov_dblv & 0x3F) | 0x00;
	ov_write_reg_result = OV7670_WriteReg(REG_DBLV, ov_dblv);
	if (ov_write_reg_result != OV7670_OK)
	{
		return ov_write_reg_result;
	}

	// after all, read default values (the same as in datasheet)
	uint8_t ov_info = 0;
	HAL_Delay(100);
	ov_read_reg_result = OV7670_ReadReg(0x1c, &ov_info); // MIDH - 0x7F
	HAL_Delay(100);
	ov_read_reg_result = OV7670_ReadReg(0x1d, &ov_info); // MIDL - 0xA2
	HAL_Delay(100);
	ov_read_reg_result = OV7670_ReadReg(0x0a, &ov_info); // PID - 0x76
	HAL_Delay(100);
	ov_read_reg_result = OV7670_ReadReg(0x0b, &ov_info); // VER - 0x73

	return OV7670_OK;
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
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  //TIM2->CCR1 = 1; // clock a0 ~83ns
  /* USER CODE END WHILE */
  if (OV7670_Config() == OV7670_ERROR) {
	  while (1) {
		  Camera_SignalError();
	  }
  }
  /* Infinite loop */
  while (1) { };
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI48;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL2;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_RCC_MCOConfig(RCC_MCO, RCC_MCO1SOURCE_PLLCLK, RCC_MCODIV_2);
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 1;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2|GPIO_PIN_3, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PC2 PC3 */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : USART_TX_Pin USART_RX_Pin */
  GPIO_InitStruct.Pin = USART_TX_Pin|USART_RX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF1_USART2;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PA8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF0_MCO;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
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
