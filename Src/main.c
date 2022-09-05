/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "crc.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ili9341.h"
#include "ili9341-gfx.h"
#include "stars_mono.h"
#include "chess_mono.h"
#include "font_Arial_12_Bold.h"
#include "font_Arial_24_Bold.h"
#include "font_Arial_36_Bold.h"

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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* Wrapper functions required by ILI9341 driver */
void gpio_cs_pin (ili9341_gpio_pin_value_t value);
void gpio_dc_pin (ili9341_gpio_pin_value_t value);
void gpio_rst_pin (ili9341_gpio_pin_value_t value);
bool spi_tx_dma_ready (void);
int spi_tx_dma_b (const uint8_t* data, uint32_t len);

void test_h_line(const ili9341_desc_ptr_t desc, ili_sgfx_brush_t brush, ili9341_orientation_t orientation);
void draw_h_stripes(const ili9341_desc_ptr_t desc, ili_sgfx_brush_t brush, uint8_t cnt);
void test_v_line(const ili9341_desc_ptr_t desc, ili_sgfx_brush_t brush, ili9341_orientation_t orientation);
void draw_v_stripes(const ili9341_desc_ptr_t desc, ili_sgfx_brush_t brush, uint8_t cnt);
void test_rect(const ili9341_desc_ptr_t desc, ili_sgfx_brush_t brush, ili9341_orientation_t orientation);
void test_filled_rect(const ili9341_desc_ptr_t desc, ili_sgfx_brush_t brush, ili9341_orientation_t orientation);
void test_pixel(const ili9341_desc_ptr_t desc, ili_sgfx_brush_t brush, ili9341_orientation_t orientation);
void test_line(const ili9341_desc_ptr_t desc, ili_sgfx_brush_t brush, uint16_t num_lines, ili9341_orientation_t orientation);
void test_pixmap(const ili9341_desc_ptr_t desc, ili_sgfx_brush_t brush, ili9341_orientation_t orientation);
void test_printf(const ili9341_desc_ptr_t desc, ili_sgfx_brush_t brush, ili9341_orientation_t orientation);
void test_printf_region(const ili9341_desc_ptr_t desc, ili_sgfx_brush_t brush, ili9341_orientation_t orientation);


/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	  ili9341_desc_ptr_t display;
	  const ili9341_cfg_t display_cfg = {
			  .cs_pin = gpio_cs_pin,
			  .dc_pin = gpio_dc_pin,
			  .rst_pin = gpio_rst_pin,
			  .spi_tx_dma = spi_tx_dma_b,
			  .spi_tx_ready = spi_tx_dma_ready,
			  .orientation = ILI9341_ORIENTATION_HORIZONTAL,
			  .width = 320,
			  .height = 240,
			  .timeout_ms = 10000,
			  .wup_delay_ms = 20,
			  .restart_delay_ms = 20
	  };

	  const ili9341_hw_cfg_t hw_cfg = ili9341_get_default_hw_cfg();
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
  MX_CRC_Init();
  MX_SPI5_Init();
  MX_TIM1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  display = ili9341_init(&display_cfg, &hw_cfg);
  if (display == NULL) {
	Error_Handler();
  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  ili_sgfx_brush_t brush = {
		.bg_color = BLACK,
		.fg_color = GREEN,
		.size = 1
	  };

	  test_h_line(display, brush, ILI9341_ORIENTATION_HORIZONTAL_UD);
	  test_h_line(display, brush, ILI9341_ORIENTATION_VERTICAL);

	  test_v_line(display, brush, ILI9341_ORIENTATION_HORIZONTAL_UD);
	  test_v_line(display, brush, ILI9341_ORIENTATION_VERTICAL);

	  test_rect(display, brush, ILI9341_ORIENTATION_HORIZONTAL_UD);
	  test_rect(display, brush, ILI9341_ORIENTATION_VERTICAL);
	  test_filled_rect(display, brush, ILI9341_ORIENTATION_HORIZONTAL_UD);
	  test_filled_rect(display, brush, ILI9341_ORIENTATION_VERTICAL);

	  test_pixel(display, brush, ILI9341_ORIENTATION_HORIZONTAL_UD);

	  test_line(display, brush, 40, ILI9341_ORIENTATION_HORIZONTAL_UD);

	  brush.transparent = false;
	  test_pixmap(display, brush, ILI9341_ORIENTATION_HORIZONTAL_UD);
	  brush.transparent = true;
	  test_pixmap(display, brush, ILI9341_ORIENTATION_HORIZONTAL_UD);
	  brush.transparent = false;

	  ili_sgfx_clear_screen(display, &brush);
	  brush.font = &font_Arial_12_Bold;
	  brush.transparent = false;
	  test_printf(display, brush, ILI9341_ORIENTATION_HORIZONTAL_UD);
	  HAL_Delay(2000);
	  ili_sgfx_clear_screen(display, &brush);
	  brush.font = &font_Arial_24_Bold;
	  brush.transparent = false;
	  test_printf(display, brush, ILI9341_ORIENTATION_HORIZONTAL_UD);
	  HAL_Delay(2000);
	  ili_sgfx_clear_screen(display, &brush);
	  brush.font = &font_Arial_36_Bold;
	  brush.transparent = false;
	  test_printf(display, brush, ILI9341_ORIENTATION_HORIZONTAL_UD);
	  HAL_Delay(2000);

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

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 3;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
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
void gpio_cs_pin (ili9341_gpio_pin_value_t value)
{
	HAL_GPIO_WritePin(CSX_GPIO_Port, CSX_Pin, value);
}

void gpio_dc_pin (ili9341_gpio_pin_value_t value)
{
	HAL_GPIO_WritePin(WRX_DCX_GPIO_Port, WRX_DCX_Pin, value);

}

void gpio_rst_pin (ili9341_gpio_pin_value_t value)
{
	HAL_GPIO_WritePin(RST_NC_GPIO_Port, RST_NC_Pin, value);
}

bool spi_tx_dma_ready (void)
{
	return HAL_SPI_GetState(&hspi5) == HAL_SPI_STATE_READY;
}

int spi_tx_dma_b (const uint8_t* data, uint32_t len)
{
	/* STM HAL lhas limited data length to 16b */
	int segments = len/65536;
	int rest = len%65536;
	int i;

	for (i = 0; i < segments; i++) {
		HAL_SPI_Transmit_DMA(&hspi5, (unsigned char*)(data+i*65535), 65535);
		while(HAL_SPI_GetState(&hspi5) == HAL_SPI_STATE_BUSY_TX) ;
	}

	HAL_SPI_Transmit_DMA(&hspi5, (unsigned char*)(data+i*65535), rest);

	return 0;
}

void test_h_line(const ili9341_desc_ptr_t desc, ili_sgfx_brush_t brush, ili9341_orientation_t orientation) {
	const uint8_t NUM_CLRS = 3;
	uint16_t colors[] = {RED, GREEN, BLUE};

	ili9341_set_orientation(desc, orientation);
	for (int i = 0; i < NUM_CLRS; i++) {
		brush.fg_color = colors[i];
		ili_sgfx_clear_screen(desc, &brush);
		draw_h_stripes(desc, brush, 18);
		HAL_Delay(2000);
	}
}

void draw_h_stripes(const ili9341_desc_ptr_t desc, ili_sgfx_brush_t brush, uint8_t cnt) {
	coord_2d_t pos = {
		.x = 1,
		.y = 1
	};
	int16_t length = ili9341_get_screen_width(desc);
	for (int i = 0; i < cnt; i++) {
		ili_sgfx_draw_h_line(desc, &brush, pos, length);
		brush.size++;
		pos.y += brush.size*2 + 1;
	}
}

void test_v_line(const ili9341_desc_ptr_t desc, ili_sgfx_brush_t brush, ili9341_orientation_t orientation) {
	const uint8_t NUM_CLRS = 3;
	uint16_t colors[] = {RED, GREEN, BLUE};

	ili9341_set_orientation(desc, orientation);
	for (int i = 0; i < NUM_CLRS; i++) {
		brush.fg_color = colors[i];
		ili_sgfx_clear_screen(desc, &brush);
		draw_v_stripes(desc, brush, 18);
		HAL_Delay(2000);
	}
}

void draw_v_stripes(const ili9341_desc_ptr_t desc, ili_sgfx_brush_t brush, uint8_t cnt) {
	coord_2d_t pos = {
		.x = 1,
		.y = 1
	};
	int16_t length = ili9341_get_screen_height(desc);
	for (int i = 0; i < cnt; i++) {
		ili_sgfx_draw_v_line(desc, &brush, pos, length);
		brush.size++;
		pos.x += brush.size*2 + 1;
	}
}

void test_rect(const ili9341_desc_ptr_t desc, ili_sgfx_brush_t brush, ili9341_orientation_t orientation) {
	ili9341_set_orientation(desc, orientation);
	ili_sgfx_clear_screen(desc, &brush);
	coord_2d_t top_left, bottom_right;
	top_left.x = ili9341_get_screen_width(desc)/2 - 10;
	top_left.y = ili9341_get_screen_height(desc)/2 - 10;
	bottom_right.x = ili9341_get_screen_width(desc)/2 + 10;
	bottom_right.y = ili9341_get_screen_height(desc)/2 + 10;

	for (int i = 0; i < 9; i++) {
		ili_sgfx_draw_rect(desc, &brush, top_left, bottom_right);
		brush.size++;
		top_left.x -= brush.size*2 + 1;
		top_left.y -= brush.size*2 + 1;
		bottom_right.x += brush.size*2 + 1;
		bottom_right.y += brush.size*2 + 1;
		HAL_Delay(500);
	}
}

void test_filled_rect(const ili9341_desc_ptr_t desc, ili_sgfx_brush_t brush, ili9341_orientation_t orientation) {
	ili9341_set_orientation(desc, orientation);
	ili_sgfx_clear_screen(desc, &brush);
	coord_2d_t top_left, bottom_right;
	top_left.x = ili9341_get_screen_width(desc)/2 - 10;
	top_left.y = ili9341_get_screen_height(desc)/2 - 10;
	bottom_right.x = ili9341_get_screen_width(desc)/2 + 10;
	bottom_right.y = ili9341_get_screen_height(desc)/2 + 10;
	brush.bg_color = BLUE;
	brush.fg_color = RED;

	for (int i = 0; i < 9; i++) {
		ili_sgfx_draw_filled_rect(desc, &brush, top_left, bottom_right);
		brush.size++;
		top_left.x -= brush.size*2 + 1;
		top_left.y -= brush.size*2 + 1;
		bottom_right.x += brush.size*2 + 1;
		bottom_right.y += brush.size*2 + 1;
		HAL_Delay(1000);
	}
}

void test_pixel(const ili9341_desc_ptr_t desc, ili_sgfx_brush_t brush, ili9341_orientation_t orientation) {
	ili9341_set_orientation(desc, orientation);
	ili_sgfx_clear_screen(desc, &brush);

	coord_2d_t start = {
			.x = 1,
			.y = 1
	};
	coord_2d_t end = {
			.x = ili9341_get_screen_width(desc),
			.y = ili9341_get_screen_height(desc)
	};
	coord_2d_t coord = start;

	uint16_t dx = end.x - start.x;
	uint16_t dy = end.y - start.y;
	for (coord.x = start.x; coord.x < end.x; coord.x++) {
		coord.y = start.y + dy*(coord.x - start.x) / dx;
		ili_sgfx_draw_pixel(desc, &brush, coord);
	}
	HAL_Delay(2000);
}

void test_line(const ili9341_desc_ptr_t desc, ili_sgfx_brush_t brush, uint16_t num_lines, ili9341_orientation_t orientation) {
	ili9341_set_orientation(desc, orientation);
	ili_sgfx_clear_screen(desc, &brush);

	coord_2d_t start = {
			.x = 1,
			.y = 1
	};
	coord_2d_t end = {
				.x = ili9341_get_screen_width(desc),
				.y = 1
		};
	uint8_t x_step = ili9341_get_screen_width(desc)/num_lines;
	uint8_t y_step = ili9341_get_screen_height(desc)/num_lines;

	for (int i = 0; i < num_lines; i++) {
		ili_sgfx_draw_line(desc, &brush, start, end);
		end.x -= x_step;
		end.y += y_step;
	}

	brush.fg_color = BLUE;
	start.x = ili9341_get_screen_width(desc);
	start.y = ili9341_get_screen_height(desc);
	end.x = 1;
	end.y = ili9341_get_screen_height(desc);
	for (int i = 0; i < num_lines; i++) {
		ili_sgfx_draw_line(desc, &brush, start, end);
		end.x += x_step;
		end.y -= y_step;
	}

}

void test_pixmap(const ili9341_desc_ptr_t desc, ili_sgfx_brush_t brush, ili9341_orientation_t orientation) {
	ili9341_set_orientation(desc, orientation);

	ili_sgfx_brush_t base_bg = {
			.bg_color = BLUE,
			.fg_color = WHITE,
			.size = 1
	};

	ili_sgfx_clear_screen(desc, &base_bg);
	{
		coord_2d_t coord = {.x = 0, .y = 0};
		ili_sgfx_pixmap_t bmp = {
				.width = CHESS_MONO_WIDTH,
				.height = CHESS_MONO_HEIGHT,
				.data = chess_mono_bits,
				.inverted = true
		};

		ili_sgfx_draw_pixmap(desc, &brush, coord, &bmp);
	}

	HAL_Delay(2000);
	ili_sgfx_clear_screen(desc, &base_bg);
	{
		coord_2d_t coord = {.x = 0, .y = 0};
		ili_sgfx_pixmap_t bmp = {
				.width = STARS_MONO_WIDTH,
				.height = STARS_MONO_HEIGHT,
				.data = stars_mono_bits,
				.inverted = true
		};

		ili_sgfx_draw_pixmap(desc, &brush, coord, &bmp);
	}
	HAL_Delay(2000);
}

void test_printf(const ili9341_desc_ptr_t desc, ili_sgfx_brush_t brush, ili9341_orientation_t orientation) {
	ili9341_set_orientation(desc, orientation);
	coord_2d_t coord = {.x = 0, .y = 0};
	ili_sgfx_printf(desc, &brush, &coord, L" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~ÄÖÜßäöü\n");
}

/* USER CODE END 4 */

 /**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */
  /* Update Display driver timers. */
  ili9341_1ms_timer_cb();
  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
