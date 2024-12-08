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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum {
    LED_OFF,
    LED_RED,
    LED_GREEN,
    LED_BLUE,
    LED_PURPLE
} LED_State;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define BUFFER_SIZE 512
#define TICKS_PER_REV 20
uint8_t byte;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim5;
TIM_HandleTypeDef htim10;

UART_HandleTypeDef huart2;
UART_HandleTypeDef huart6;

/* USER CODE BEGIN PV */
char buffer[BUFFER_SIZE];
volatile int buffer_index = 0;
volatile uint8_t rcv_intpt_flag = 0;
uint8_t txd_msg_buffer[1024] = {0};
uint8_t rx_data;
volatile int wall_clock_hr_update_flag = 0;
volatile int rpm_tick_count = 0;

volatile int inlet_pwm, zone1_pwm, zone2_pwm, zone3_pwm;
volatile int inlet_start, inlet_stop, zone1_start, zone1_stop, zone2_start, zone2_stop, zone3_start, zone3_stop;
volatile int check_water_level = 0;
int current_water_level, current_motor_pwm, current_zone, current_motor_rpm, current_water_depth;

volatile uint8_t clock_hours = 0;
volatile uint8_t clock_mins = 0;
volatile uint8_t clock_secs = 0;

volatile uint8_t hcsr04_Rx_flag = 0;
volatile uint8_t first_edge = 0;
volatile uint16_t time_edge1 = 0;
volatile uint16_t time_edge2 = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART6_UART_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM10_Init(void);
static void MX_TIM5_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void DIGITS_Display(uint8_t DIGIT_A, uint8_t DIGIT_B)
{
	 uint8_t DIGITA_VAL = 0x0F & DIGIT_A; //mask off higher4 bits
	 int Abit0 = (DIGITA_VAL ) & 1;  	// extract Abit0 of the 4-bit value
	 int Abit1 = (DIGITA_VAL >> 1) & 1;  // extract Abit1 of the 4-bit value
	 int Abit2 = (DIGITA_VAL >> 2) & 1;  // extract Abit2 of the 4-bit value
	 int Abit3 = (DIGITA_VAL >> 3) & 1;  // extract Abit3 of the 4-bit value

	 uint8_t DIGITB_VAL = 0x0F & DIGIT_B; //mask off higher4 bits
	 int Bbit0 = (DIGITB_VAL ) & 1;  	// extract Bbit0 of the 4-bit value
	 int Bbit1 = (DIGITB_VAL >> 1) & 1;  // extract Bbit1 of the 4-bit value
	 int Bbit2 = (DIGITB_VAL >> 2) & 1;  // extract Bbit2 of the 4-bit value
	 int Bbit3 = (DIGITB_VAL >> 3) & 1;  // extract Bbit3 of the 4-bit value

	 if (Abit0 == (0))
	 {
		 HAL_GPIO_WritePin(GPIOB, DIGIT_A0_Pin, GPIO_PIN_RESET);
	 }
	 else
	 {
		 HAL_GPIO_WritePin(GPIOB, DIGIT_A0_Pin, GPIO_PIN_SET);

	 }
	 if (Abit1 == (0))
	 {
		 HAL_GPIO_WritePin(GPIOB, DIGIT_A1_Pin, GPIO_PIN_RESET);
	 }
	 else
	 {
		 HAL_GPIO_WritePin(GPIOB, DIGIT_A1_Pin, GPIO_PIN_SET);

	 }
	 if (Abit2 == (0))
	 {
		 HAL_GPIO_WritePin(GPIOB, DIGIT_A2_Pin, GPIO_PIN_RESET);
	 }
	 else
	 {
		 HAL_GPIO_WritePin(GPIOB, DIGIT_A2_Pin, GPIO_PIN_SET);

	 }
	 if (Abit3 == (0))
	 {
		 HAL_GPIO_WritePin(GPIOB, DIGIT_A3_Pin, GPIO_PIN_RESET);
	 }
	 else
	 {
		 HAL_GPIO_WritePin(GPIOB, DIGIT_A3_Pin, GPIO_PIN_SET);

	 }


	 if (Bbit0 == (0))
	 {
		 HAL_GPIO_WritePin(GPIOC, DIGIT_B0_Pin, GPIO_PIN_RESET);
	 }
	 else
	 {
		 HAL_GPIO_WritePin(GPIOC, DIGIT_B0_Pin, GPIO_PIN_SET);

	 }
	 if (Bbit1 == (0))
	 {
		 HAL_GPIO_WritePin(GPIOC, DIGIT_B1_Pin, GPIO_PIN_RESET);
	 }
	 else
	 {
		 HAL_GPIO_WritePin(GPIOC, DIGIT_B1_Pin, GPIO_PIN_SET);

	 }
	 if (Bbit2 == (0))
	 {
		 HAL_GPIO_WritePin(GPIOC, DIGIT_B2_Pin, GPIO_PIN_RESET);
	 }
	 else
	 {
		 HAL_GPIO_WritePin(GPIOC, DIGIT_B2_Pin, GPIO_PIN_SET);

	 }
	 if (Bbit3 == (0))
	 {
		 HAL_GPIO_WritePin(GPIOC, DIGIT_B3_Pin, GPIO_PIN_RESET);
	 }
	 else
	 {
		 HAL_GPIO_WritePin(GPIOC, DIGIT_B3_Pin, GPIO_PIN_SET);

	 }
}

void HCSR04_TRIG_PULSE(void) {
	HAL_GPIO_WritePin(HCSR04_TRIG_GPIO_Port, HCSR04_TRIG_Pin, GPIO_PIN_SET);
	for (int j = 0; j != 15; ++j) {};
	HAL_GPIO_WritePin(HCSR04_TRIG_GPIO_Port, HCSR04_TRIG_Pin, GPIO_PIN_RESET);
}

void prompt_and_receive(const char *prompt, volatile int *value) {
  HAL_UART_Transmit(&huart6, (uint8_t *)prompt, strlen(prompt), HAL_MAX_DELAY);
  rcv_intpt_flag = 0;
  while (!rcv_intpt_flag); // Wait for input
  sscanf(buffer, "%d", value); // load value with the inputed characters
}

void print_terminal(char *text) {
	sprintf((char *) txd_msg_buffer, text);
    HAL_UART_Transmit(&huart6, txd_msg_buffer, strlen((char *)txd_msg_buffer), 100);
}

void display_water_level(int level) {
	int tens = level / 10;
	int ones = level % 10;

	DIGITS_Display(tens, ones);
}

void convert_option_to_pwm(int *pipe) {
	if (*pipe == 0) {
		*pipe = 0;
	} else if (*pipe == 1) {
		*pipe = 70;
	} else if (*pipe == 2) {
		*pipe = 85;
	} else if (*pipe == 3) {
		*pipe = 99;
	}
}

void setLEDState(LED_State state) {
    // Turn off all LEDs first
    HAL_GPIO_WritePin(GPIOB, RED_Pin, GPIO_PIN_RESET);  // RED
    HAL_GPIO_WritePin(GPIOB, GRN_Pin, GPIO_PIN_RESET);  // GRN
    HAL_GPIO_WritePin(GPIOB, BLU_Pin, GPIO_PIN_RESET);  // BLU

    // Set the desired state
    switch (state) {
        case LED_OFF:
            break;
        case LED_RED:
            HAL_GPIO_WritePin(GPIOB, RED_Pin, GPIO_PIN_SET);  // RED on
            break;
        case LED_GREEN:
            HAL_GPIO_WritePin(GPIOB, GRN_Pin, GPIO_PIN_SET);  // GREEN on
            break;
        case LED_BLUE:
            HAL_GPIO_WritePin(GPIOB, BLU_Pin, GPIO_PIN_SET);  // BLUE on
            break;
        case LED_PURPLE:
            HAL_GPIO_WritePin(GPIOB, RED_Pin, GPIO_PIN_SET);  // RED on
            HAL_GPIO_WritePin(GPIOB, BLU_Pin, GPIO_PIN_SET);  // BLUE on
            break;
        default:
            break;
    }
}

void ADC_Select_CH(int CH) {
	ADC_ChannelConfTypeDef sConfig = {0};
	switch(CH)
	{
	case 0:
	sConfig.Channel = ADC_CHANNEL_0;
	sConfig.Rank = 1;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
	Error_Handler();
	}
	break;
	case 1:
	sConfig.Channel = ADC_CHANNEL_1;
	sConfig.Rank = 1;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
	Error_Handler();
	}
	break;
	case 2:
	sConfig.Channel = ADC_CHANNEL_2;
	sConfig.Rank = 1;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
	Error_Handler();
	}
	break;
	case 3:
	sConfig.Channel = ADC_CHANNEL_3;
	sConfig.Rank = 1;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
	Error_Handler();
	}
	break;
	case 4:
	sConfig.Channel = ADC_CHANNEL_4;
	sConfig.Rank = 1;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
	Error_Handler();
	}
	break;
	case 5:
	sConfig.Channel = ADC_CHANNEL_5;
	sConfig.Rank = 1;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
	Error_Handler();
	}
	break;
	case 6:
	sConfig.Channel = ADC_CHANNEL_6;
	sConfig.Rank = 1;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
	Error_Handler();
	}
	break;
	case 7:
	sConfig.Channel = ADC_CHANNEL_7;
	sConfig.Rank = 1;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
	Error_Handler();
	}
	break;
	case 8:
	sConfig.Channel = ADC_CHANNEL_8;
	sConfig.Rank = 1;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
	Error_Handler();
	}
	break;
	case 9:
	sConfig.Channel = ADC_CHANNEL_9;
	sConfig.Rank = 1;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
	Error_Handler();
	}
	break;
	case 10:
	sConfig.Channel = ADC_CHANNEL_10;
	sConfig.Rank = 1;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
	Error_Handler();
	}
	break;
	case 11:
	sConfig.Channel = ADC_CHANNEL_11;
	sConfig.Rank = 1;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
	Error_Handler();
	}
	break;
	case 12:
	sConfig.Channel = ADC_CHANNEL_12;
	sConfig.Rank = 1;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
	Error_Handler();
	}
	break;
	case 13:
	sConfig.Channel = ADC_CHANNEL_13;
	sConfig.Rank = 1;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
	Error_Handler();
	}
	break;
	case 14:
	sConfig.Channel = ADC_CHANNEL_14;
	sConfig.Rank = 1;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
	Error_Handler();
	}
	break;
	case 15:
	sConfig.Channel = ADC_CHANNEL_15;
	sConfig.Rank = 1;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
	Error_Handler();
	}
	break;
	}
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
	// Servo
	int TIM2_Ch1_DCVAL = 500;
	int TIM2_Ch1_STEP = 20;
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
  MX_USART2_UART_Init();
  MX_USART6_UART_Init();
  MX_ADC1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM10_Init();
  MX_TIM5_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Init(&htim3);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
  HAL_TIM_Base_Start(&htim2);
  TIM2->PSC = 16-1;
  TIM2->ARR = 20000-1;
  TIM2->CCR1 = TIM2_Ch1_DCVAL;
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  HAL_TIM_Base_Init(&htim10);
  HAL_TIM_Base_Start(&htim10);
  HAL_TIM_IC_Start_IT(&htim10, TIM_CHANNEL_1);
  HAL_TIM_Base_Start_IT(&htim5);

  // HCSR04
  uint16_t time_diff = 0;
  uint16_t distance = 0;
  uint16_t empty_distance = 10; // cm
  uint16_t full_distance = 50; // cm
  int empty = 0;

  int pump_not_running = 0;

  // Setup mode operations
  HAL_GPIO_WritePin(GPIOA, LD2_Pin, GPIO_PIN_RESET);
  TIM3->CCR1 = 0;
  TIM3->CCR3 = 0;

  // Clear terminal
  print_terminal("\033[2J\033[H");

  print_terminal("SETUP MODE \r\n");

  HAL_UART_Receive_IT(&huart6, &rx_data, 1);
  prompt_and_receive("Inlet Pump PWM (options: 0 to 3): ", &inlet_pwm);
  prompt_and_receive("Pump 1 PWM (options: 0 to 3): ", &zone1_pwm);
  prompt_and_receive("Pump 2 PWM (options: 0 to 3): ", &zone2_pwm);
  prompt_and_receive("Pump 3 PWM (options: 0 to 3): ", &zone3_pwm);
  prompt_and_receive("Pipeline 0  Pump FIRST HOUR (options: 00-23): ", &inlet_start);
  prompt_and_receive("Pipeline 0  Pump LAST HOUR (options: 00-23): ", &inlet_stop);
  prompt_and_receive("Pipeline 1  Pump FIRST HOUR (options: 00-23): ", &zone1_start);
  prompt_and_receive("Pipeline 1  Pump LAST HOUR (options: 00-23): ", &zone1_stop);
  prompt_and_receive("Pipeline 2  Pump FIRST HOUR (options: 00-23): ", &zone2_start);
  prompt_and_receive("Pipeline 2  Pump LAST HOUR (options: 00-23): ", &zone2_stop);
  prompt_and_receive("Pipeline 3  Pump FIRST HOUR (options: 00-23): ", &zone3_start);
  prompt_and_receive("Pipeline 3  Pump LAST HOUR (options: 00-23): ", &zone3_stop);

  convert_option_to_pwm(&inlet_pwm);
  convert_option_to_pwm(&zone1_pwm);
  convert_option_to_pwm(&zone2_pwm);
  convert_option_to_pwm(&zone3_pwm);

  sprintf(txd_msg_buffer, "\nPrinting SETUP Parameters \r\n\n");
  HAL_UART_Transmit(&huart6, (uint8_t*)txd_msg_buffer, strlen(txd_msg_buffer), HAL_MAX_DELAY);
  if (inlet_pwm == 0) {
	  sprintf(txd_msg_buffer, "PIPELINE: INLET	Pump PWM: Manual Control	Pump FIRST HOUR: %d	Pump LAST HOUR: %d\r\n", inlet_start, inlet_stop);
  } else {
	  sprintf(txd_msg_buffer, "PIPELINE: INLET	Pump PWM: %d	Pump FIRST HOUR: %d	Pump LAST HOUR: %d\r\n", inlet_pwm, inlet_start, inlet_stop);
  }
  HAL_UART_Transmit(&huart6, (uint8_t*)txd_msg_buffer, strlen(txd_msg_buffer), HAL_MAX_DELAY);
  if (zone1_pwm == 0) {
	  sprintf(txd_msg_buffer, "PIPELINE: 1	Pump PWM: Manual Control	Pump FIRST HOUR: %d	Pump LAST HOUR: %d\r\n", zone1_start, zone1_stop);
  } else {
	  sprintf(txd_msg_buffer, "PIPELINE: 1	Pump PWM: %d	Pump FIRST HOUR: %d	Pump LAST HOUR: %d\r\n", zone1_pwm, zone1_start, zone1_stop);
  }
  HAL_UART_Transmit(&huart6, (uint8_t*)txd_msg_buffer, strlen(txd_msg_buffer), HAL_MAX_DELAY);
  if (zone2_pwm == 0) {
	  sprintf(txd_msg_buffer, "PIPELINE: 2	Pump PWM: Manual Control	Pump FIRST HOUR: %d	Pump LAST HOUR: %d\r\n",  zone2_start, zone2_stop);
  } else {
	  sprintf(txd_msg_buffer, "PIPELINE: 2	Pump PWM: %d	Pump FIRST HOUR: %d	Pump LAST HOUR: %d\r\n",  zone2_pwm, zone2_start, zone2_stop);
  }
  HAL_UART_Transmit(&huart6, (uint8_t*)txd_msg_buffer, strlen(txd_msg_buffer), HAL_MAX_DELAY);
  if (zone3_pwm == 0) {
	  sprintf(txd_msg_buffer, "PIPELINE: 3	Pump PWM: Manual Control	Pump FIRST HOUR: %d	Pump LAST HOUR: %d\r\n", zone3_start, zone3_stop);
  } else {
	  sprintf(txd_msg_buffer, "PIPELINE: 3	Pump PWM: %d	Pump FIRST HOUR: %d	Pump LAST HOUR: %d\r\n", zone3_pwm, zone3_start, zone3_stop);
  }

  HAL_UART_Transmit(&huart6, (uint8_t*)txd_msg_buffer, strlen(txd_msg_buffer), HAL_MAX_DELAY);
  HAL_UART_Transmit(&huart6, "\r\n\n", 2, HAL_MAX_DELAY);
  HAL_UART_Transmit(&huart6, "SETUP is done. Press Blue Button for RUN MODE \r\n\n", 46, HAL_MAX_DELAY);

  while (1) {
	  // Toggle MCU LED
	  HAL_GPIO_TogglePin(GPIOA, LD2_Pin);
	  HAL_Delay(500);

	  // Check for button press
	  if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == 0) {
		  break;
	  }
  }


  // Run mode
  print_terminal("\r\n\nRUN MODE \r\n");
  HAL_GPIO_WritePin(GPIOA, LD2_Pin, GPIO_PIN_SET);

  clock_hours = 0;
  clock_mins = 0;
  clock_secs = 0;

  print_terminal("WALL CLOCK (hr):, ZONE/INLET:, MOTOR Speed %% PWM:, MOTOR RPM:, WATER DEPTH (%%):\r\n");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if (clock_hours == 24) {
		  break;
	  }
	  if (clock_mins == 0) {
		  pump_not_running = 0;
		  rpm_tick_count = 0;
	  } else if (clock_mins == 15) {
		  rpm_tick_count = 0;
	  } else if (clock_mins == 55) {
		  current_motor_rpm = rpm_tick_count / TICKS_PER_REV * 60 / 8;
	  } else if (clock_mins == 59) {
		  if (pump_not_running) {
			  sprintf(txd_msg_buffer, "     %d                                                  %d              %d \r\n", clock_hours, current_motor_rpm, current_water_depth);
		  } else {
			  sprintf(txd_msg_buffer, "     %d               %d              %d                 %d              %d \r\n", clock_hours, current_zone, current_motor_pwm, current_motor_rpm, current_water_depth);
		  }
		  HAL_UART_Transmit(&huart6, (uint8_t*)txd_msg_buffer, strlen(txd_msg_buffer), HAL_MAX_DELAY);
	  }
	  hcsr04_Rx_flag = 0;
	  first_edge = 0;
	  time_edge1 = 0;
	  time_edge2 = 0;
	  time_diff = 0;
	  distance = 0;

	  HCSR04_TRIG_PULSE();

	  while(hcsr04_Rx_flag == 0) {};

	  time_diff = time_edge2 - time_edge1;
	  distance = time_diff / 58;

	  current_water_depth = (distance - empty_distance) * 100 / full_distance;

	  if (current_water_depth >= full_distance) {
		  current_water_depth = 99;
	  }

	  display_water_level(current_water_depth);

	  // Special case: reservoir is empty
	  if (distance <= empty_distance) {
		  empty = 1;
		  break;
	  }


	  // Check current pipe
	  if (clock_hours >= inlet_start && clock_hours <= inlet_stop) {
		  setLEDState(LED_PURPLE);
		  current_zone = 0;
		  pump_not_running = 0;
		  TIM2->CCR1 = 500;
	  } else if (clock_hours >= zone1_start && clock_hours <= zone1_stop) {
		  setLEDState(LED_RED);
		  current_zone = 1;
		  pump_not_running = 0;
		  TIM2->CCR1 = 1300;
	  } else if (clock_hours >= zone2_start && clock_hours <= zone2_stop) {
		  setLEDState(LED_GREEN);
		  current_zone = 2;
		  pump_not_running = 0;
		  TIM2->CCR1 = 1900;
	  } else if (clock_hours >= zone3_start && clock_hours <= zone3_stop) {
		  setLEDState(LED_BLUE);
		  current_zone = 3;
		  pump_not_running = 0;
		  TIM2->CCR1 = 2500;
	  } else {
		  // Pump is not to run
		  setLEDState(LED_OFF);
		  pump_not_running = 1;
	  }

	  // Manual control for PWM
	  if ((inlet_pwm == 0 && current_zone == 0) || (zone1_pwm == 0 && current_zone == 1) || (zone2_pwm == 0 && current_zone == 2) || (zone3_pwm == 0 && current_zone == 3)) {
		  // Read ADC
		  ADC_Select_CH(9);
		  HAL_ADC_Start(&hadc1);
		  HAL_ADC_PollForConversion(&hadc1, 1000);
		  int ADC_CH9 = HAL_ADC_GetValue(&hadc1);
		  HAL_ADC_Stop(&hadc1);

		  current_motor_pwm = ADC_CH9 * 100 / 255;
	  } else {
		  if (current_zone == 0) {
			  current_motor_pwm = inlet_pwm;
		  } else if (current_zone == 1) {
			  current_motor_pwm = zone1_pwm;
		  } else if (current_zone == 2) {
			  current_motor_pwm = zone2_pwm;
		  } else if (current_zone == 3) {
			  current_motor_pwm = zone3_pwm;
		  }
	  }
	  if (current_zone == 0 && !pump_not_running) {
		  unsigned long speed = current_motor_pwm * 60000 / 100;
		  TIM3->CCR1 = 0;
		  TIM3->CCR3 = speed;
	  } else if (current_zone != 0 && !pump_not_running) {
		  unsigned long speed = current_motor_pwm * 60000 / 100;
		  TIM3->CCR1 = speed;
		  TIM3->CCR3 = 0;
	  } else {
		  TIM3->CCR1 = 0;
		  TIM3->CCR3 = 0;
	  }

	  HAL_Delay(200);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  if (empty) {
	  // Resolve special case
	  print_terminal("\r\n\n RESERVOIR IS EMPTY\r\n");
	  HAL_GPIO_WritePin(GPIOA, LD2_Pin, GPIO_PIN_RESET);

	  TIM3->CCR1 = 0;
	  TIM3->CCR3 = 0;
	  while (1) {
		  HAL_GPIO_WritePin(GPIOB, RED_Pin, GPIO_PIN_SET);  // RED
		  HAL_GPIO_WritePin(GPIOB, GRN_Pin, GPIO_PIN_SET);  // GRN
		  HAL_GPIO_WritePin(GPIOB, BLU_Pin, GPIO_PIN_SET);  // BLU
		  HAL_Delay(500);

		  HAL_GPIO_WritePin(GPIOB, RED_Pin, GPIO_PIN_RESET);  // RED
		  HAL_GPIO_WritePin(GPIOB, GRN_Pin, GPIO_PIN_RESET);  // GRN
		  HAL_GPIO_WritePin(GPIOB, BLU_Pin, GPIO_PIN_RESET);  // BLU
		  HAL_Delay(500);
	  }
  } else {
	  TIM3->CCR1 = 0;
	  TIM3->CCR3 = 0;
	  setLEDState(LED_OFF);
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
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_8B;
  hadc1.Init.ScanConvMode = ENABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_9;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

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
  htim2.Init.Prescaler = 16-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 20000-1;
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
  sConfigOC.Pulse = 500-1;
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
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 16-1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 2000-1;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 1200-1;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.Pulse = 0;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * @brief TIM5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM5_Init(void)
{

  /* USER CODE BEGIN TIM5_Init 0 */

  /* USER CODE END TIM5_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM5_Init 1 */

  /* USER CODE END TIM5_Init 1 */
  htim5.Instance = TIM5;
  htim5.Init.Prescaler = 53;
  htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim5.Init.Period = 1000-1;
  htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim5) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim5, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM5_Init 2 */

  /* USER CODE END TIM5_Init 2 */

}

/**
  * @brief TIM10 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM10_Init(void)
{

  /* USER CODE BEGIN TIM10_Init 0 */

  /* USER CODE END TIM10_Init 0 */

  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM10_Init 1 */

  /* USER CODE END TIM10_Init 1 */
  htim10.Instance = TIM10;
  htim10.Init.Prescaler = 16-1;
  htim10.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim10.Init.Period = 65535;
  htim10.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim10.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim10) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_Init(&htim10) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_BOTHEDGE;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim10, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM10_Init 2 */

  /* USER CODE END TIM10_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USART6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART6_UART_Init(void)
{

  /* USER CODE BEGIN USART6_Init 0 */

  /* USER CODE END USART6_Init 0 */

  /* USER CODE BEGIN USART6_Init 1 */

  /* USER CODE END USART6_Init 1 */
  huart6.Instance = USART6;
  huart6.Init.BaudRate = 9600;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART6_Init 2 */

  /* USER CODE END USART6_Init 2 */

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
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, BLU_Pin|RED_Pin|GRN_Pin|HCSR04_TRIG_Pin
                          |DIGIT_A0_Pin|DIGIT_A1_Pin|DIGIT_A2_Pin|DIGIT_A3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, DIGIT_B3_Pin|DIGIT_B0_Pin|DIGIT_B1_Pin|DIGIT_B2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : RPM_TICK_Pin */
  GPIO_InitStruct.Pin = RPM_TICK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(RPM_TICK_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : BLU_Pin RED_Pin GRN_Pin HCSR04_TRIG_Pin
                           DIGIT_A0_Pin DIGIT_A1_Pin DIGIT_A2_Pin DIGIT_A3_Pin */
  GPIO_InitStruct.Pin = BLU_Pin|RED_Pin|GRN_Pin|HCSR04_TRIG_Pin
                          |DIGIT_A0_Pin|DIGIT_A1_Pin|DIGIT_A2_Pin|DIGIT_A3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : DIGIT_B3_Pin DIGIT_B0_Pin DIGIT_B1_Pin DIGIT_B2_Pin */
  GPIO_InitStruct.Pin = DIGIT_B3_Pin|DIGIT_B0_Pin|DIGIT_B1_Pin|DIGIT_B2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART6)
	  {
	    if (rx_data == '\r') // ENTER key pressed
	    {
	      buffer[buffer_index] = '\0';
	      rcv_intpt_flag = 1;
	      buffer_index = 0; // Reset buffer index for next input
	      HAL_UART_Transmit(&huart6, (uint8_t *)"\r\n", 2, HAL_MAX_DELAY); // Echo newline
	    }
	    else
	    {
	      buffer[buffer_index++] = rx_data;
	      HAL_UART_Transmit(&huart6, &rx_data, 1, HAL_MAX_DELAY); // Echo the received character
	    }
	    HAL_UART_Receive_IT(&huart6, &rx_data, 1);
	  }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if ((htim->Instance == TIM5)) {
		wall_clock_hr_update_flag = 0;
		clock_secs += 1;

		if ((clock_secs == 60)) {
			clock_mins += 1;
			clock_secs = 0;

			if ((clock_mins == 60)) {
				clock_hours += 1;
				clock_mins = 0;
			}
			wall_clock_hr_update_flag = 1;
		}
	}
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM10) {
		if (htim->Channel == 1) {
			if (first_edge == 0) {
				time_edge1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
				first_edge = 1;
			} else {
				time_edge2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
				__HAL_TIM_SET_COUNTER(htim, 0);
				hcsr04_Rx_flag = 1;
			}
		}
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == RPM_TICK_Pin) {
		rpm_tick_count += 1;
	}
}
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
