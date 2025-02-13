/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
/* Kullanıcı tanımlı ek kütüphaneler buraya eklenebilir */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* Özel typedef tanımları buraya eklenebilir */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* Özel sabit tanımları buraya eklenebilir */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* Özel makrolar buraya eklenebilir */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
/* Özel değişken tanımları buraya eklenebilir */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */
/* Kullanıcı tarafından tanımlanan özel fonksiyon prototipleri buraya eklenebilir */
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/**
 * @brief SPI için GPIO pinlerini başlatır.
 */
void SPI_GPIO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOA_CLK_ENABLE();  // GPIOA portunun saat sinyalini etkinleştir

    // SPI giriş pinleri: PA4 (SS), PA5 (SCK), PA7 (MOSI)
    GPIO_InitStruct.Pin = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // SPI çıkış pini: PA6 (MISO)
    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // LED pinleri: PA0 ve PA1
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // LED'leri başlangıçta söndür
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
}

/**
 * @brief SPI üzerinden veri alır.
 * @retval uint8_t Alınan veri
 */
uint8_t SPI_Receive(void) {
    uint8_t receivedData = 0;  // Alınan veriyi saklamak için değişken
    
    // 8 bitlik veriyi al
    for (int i = 7; i >= 0; i--) {
        // Clock HIGH olana kadar bekle
        while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5) == GPIO_PIN_RESET);
        
        // MOSI pinindeki veriyi oku ve kaydet
        receivedData = (receivedData << 1);
        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7)) {
            receivedData |= 0x01;
        }
        
        // Clock LOW olana kadar bekle
        while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5) == GPIO_PIN_SET);
    }
    
    return receivedData;  // Alınan veriyi döndür
}

/**
 * @brief SPI üzerinden veri gönderir.
 * @param data Gönderilecek veri (8 bit)
 */
void SPI_Send(uint8_t data) {
    // MISO pinine en düşük biti gönder
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, (data & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    
    // Clock sinyalini bekle
    while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5) == GPIO_PIN_RESET);
    while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5) == GPIO_PIN_SET);
}

/**
 * @brief Slave Select (SS) pininin durumunu kontrol eder.
 * @retval uint8_t SS pin durumu (1: HIGH, 0: LOW)
 */
uint8_t SPI_CheckSlaveSelect(void) {
    return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4);
}

/* USER CODE END 0 */

/**
  * @brief  Ana programın giriş noktası.
  * @retval int
  */
int main(void)
{
  HAL_Init();                // HAL kütüphanesini başlat
  SystemClock_Config();      // Sistem saat yapılandırmasını yap
  MX_GPIO_Init();            // Genel GPIO yapılandırmasını yap
  SPI_GPIO_Init();           // SPI için GPIO yapılandırmasını yap

  // Başlangıçta LED'leri söndür
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0 | GPIO_PIN_1, GPIO_PIN_RESET);

  while (1)
  {
    // SS aktif olana kadar bekle
    while (SPI_CheckSlaveSelect() == GPIO_PIN_SET);

    // SPI üzerinden veriyi al
    uint8_t data = SPI_Receive();

    // Verinin tek/çift durumunu kontrol et
    uint8_t isOdd = (data & 0x01);  // Son bit 1 ise tek, 0 ise çift

    // LED'leri kontrol et
    if (isOdd) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);  // Çift LED söndür
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);    // Tek LED yak
    } else {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);    // Çift LED yak
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);  // Tek LED söndür
    }

    // Tek/çift bilgisini SPI ile gönder
    SPI_Send(isOdd);

    // SS'in tekrar deaktif olmasını bekle
    while (SPI_CheckSlaveSelect() == GPIO_PIN_RESET);
  }
}

/**
  * @brief Sistem saat yapılandırma fonksiyonu.
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

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
  * @brief GPIO yapılandırma fonksiyonu.
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  // Gerekli GPIO portlarının saat sinyallerini etkinleştir
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  // LED'leri başlangıçta söndür
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_6, GPIO_PIN_RESET);

  // GPIOC için pin yapılandırması
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  // GPIOA için pin yapılandırması
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/**
  * @brief Hata durumunda çağrılan fonksiyon.
  * @retval None
  */
void Error_Handler(void)
{
  __disable_irq();  // Kesme sinyallerini devre dışı bırak
  while (1)
  {
    // Sonsuz döngü (hata durumu)
  }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief assert_param hataları için hata rapor fonksiyonu.
  * @param file Hatanın bulunduğu dosya
  * @param line Hatanın bulunduğu satır
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  // Kullanıcı tanımlı hata işleme
}
#endif /* USE_FULL_ASSERT */
