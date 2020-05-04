// math function Demo Application
// Created by mohamad on 11.12.19.
//
#include <stm32f7xx_hal.h>
#include <stm32746g_discovery.h>
#include <stdlib.h>
#include <cmsis_os.h>
#include <stm32f7xx_hal_tim.h>
#include <backpressure.h>
#include <flow_helper.h>
#include <math.h>
#include "sink_entity.h"
#include "FreeRTOS.h"
#include "gfx.h"
#include "itm_bootstrap.h"

#ifndef SINE_WAVE_BENCHMARK
#error you must define "SINE_WAVE_BENCHMARK"
#endif

#pragma clang diagnostic ignored "-Wmissing-noreturn" // disable clang-tidy errors

static UART_HandleTypeDef uart1;


#ifdef BACKPRESSURE_ENABLED
static UBaseType_t tempUxSavedInterruptStatus;
#endif

inline void __attribute__((no_instrument_function)) pfyl_begin_transfer() {
#ifdef BACKPRESSURE_ENABLED
//    tempUxSavedInterruptStatus = taskENTER_CRITICAL_FROM_ISR();
#else
    taskENTER_CRITICAL();
#endif
}

uint32_t __attribute__((no_instrument_function)) pfyl_transfer(uint8_t *buf, size_t size) {
#ifdef UART1_ENABLED
    return HAL_UART_Transmit(&uart1, buf, size, HAL_UART_TIMEOUT_VALUE);
#else
    return 0;
#endif
}

inline void __attribute__((no_instrument_function)) pfyl_end_transfer() {
#ifdef BACKPRESSURE_ENABLED
//    taskEXIT_CRITICAL_FROM_ISR(tempUxSavedInterruptStatus);
#else
    taskEXIT_CRITICAL();
#endif
}

void __attribute__((no_instrument_function)) HAL_UART_MspInit(UART_HandleTypeDef *huart) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (huart->Instance == USART1) {
        /* USER CODE BEGIN USART1_MspInit 0 */

        /* USER CODE END USART1_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_USART1_CLK_ENABLE();

        __HAL_RCC_GPIOB_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**USART1 GPIO Configuration
        PB7     ------> USART1_RX
        PA9     ------> USART1_TX
        */
        GPIO_InitStruct.Pin = GPIO_PIN_7;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_9;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* USER CODE BEGIN USART1_MspInit 1 */

        /* USER CODE END USART1_MspInit 1 */
    }

}

void __attribute__((no_instrument_function)) HAL_MspInit(void) {
    /* USER CODE BEGIN MspInit 0 */

    /* USER CODE END MspInit 0 */

    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_RCC_SYSCFG_CLK_ENABLE();

    /* System interrupt init*/

    /* USER CODE BEGIN MspInit 1 */

    /* USER CODE END MspInit 1 */
}

void __attribute__((no_instrument_function)) SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

    /** Configure the main internal regulator output voltage
    */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
    /** Initializes the CPU, AHB and APB busses clocks
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 10;
    RCC_OscInitStruct.PLL.PLLN = 210;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 2;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);
    /** Initializes the CPU, AHB and APB busses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                  | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART1;
    PeriphClkInitStruct.Usart1ClockSelection = RCC_USART1CLKSOURCE_HSI;
    HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
}

void __attribute__((no_instrument_function)) initAll(void) {
    HAL_Init();
    SystemClock_Config();

}

void togggle() {
    volatile char temp[60] = {0};
    BSP_LED_Toggle(LED1);

}

void toogleLed1() {
    volatile char temp[6] = {0};
    togggle();
}

void toogleLed2() {
    volatile char temp[60] = {0};
    BSP_LED_Toggle(LED1);
}

void backgroundThread(void *pvParameters) {
    int i = 1;
    void* persisted_test_entity = malloc(256);
    for (;;) {
        void *test_entity = malloc(5+i);
        void *test_entity2 = malloc(5+2*i);
        void *test_entity3 = malloc(5);
        void *test_entity4 = malloc(5);
        void *test_entity5 = malloc(5);
        void *leaked_memory = malloc(4);
        toogleLed1();
        vTaskDelay(1000);
        free(test_entity);
        free(test_entity2);
        free(test_entity3);
        free(test_entity4);
        free(test_entity5);
        i++;
        if(i > 30) {
            i = 1;
        }
#ifdef PFYL_USE_COOPERATIVE
        taskYIELD();
#endif
    }
}


/* ---------------------------------------------------------------------- */

#define N 1024			/* Number of dots */
#define SCALE 8192
#define INCREMENT 512		/* INCREMENT = SCALE / sqrt(N) * 2 */
#define PI2 6.283185307179586476925286766559

#define background RGB2COLOR(0,0,0)

gU16 width, height;
gI16 sine[SCALE+(SCALE/4)];
gI16 *cosi = &sine[SCALE/4]; /* cos(x) = sin(x+90d)... */


void initialize (void)
{
    gU16 i;

    /* if you change the SCALE*1.25 back to SCALE, the program will
     * occassionally overrun the cosi array -- however this actually
     * produces some interesting effects as the BUBBLES LOSE CONTROL!!!!
     */
    for (i = 0; i < SCALE+(SCALE/4); i++)
        //sine[i] = (-SCALE/2) + (int)(sinf(PI2 * i / SCALE) * sinf(PI2 * i / SCALE) * SCALE);
        sine[i] = (int)(sinf(PI2 * i / SCALE) * SCALE);
}


void matrix (gI16 xyz[3][N], gColor col[N])
{
    static gU32 t = 0;
    gI16 x = -SCALE, y = -SCALE;
    gU16 i, s, d;
    gU8 red,grn,blu;

#define RED_COLORS (32)
#define GREEN_COLORS (64)
#define BLUE_COLORS (32)

    for (i = 0; i < N; i++)
    {
        xyz[0][i] = x;
        xyz[1][i] = y;

        d = sqrt(x * x + y * y);	/* originally a fastsqrt() call */
        s = sine[(t * 30) % SCALE] + SCALE;

        xyz[2][i] = sine[(d + s) % SCALE] * sine[(t * 10) % SCALE] / SCALE / 2;

        red = (cosi[xyz[2][i] + SCALE / 2] + SCALE) *
              (RED_COLORS - 1) / SCALE / 2;
        grn = (cosi[(xyz[2][i] + SCALE / 2 + 2 * SCALE / 3) % SCALE] + SCALE) *
              (GREEN_COLORS - 1) / SCALE / 2;
        blu = (cosi[(xyz[2][i] + SCALE / 2 + SCALE / 3) % SCALE] + SCALE) *
              (BLUE_COLORS - 1) / SCALE / 2;
        col[i] = ((red << 11) + (grn << 5) + blu);

        x += INCREMENT;

        if (x >= SCALE) x = -SCALE, y += INCREMENT;
    }
    t++;
}


void rotate (gI16 xyz[3][N], gU16 angleX, gU16 angleY, gU16 angleZ)
{
    gU16 i;
    gI16 tmpX, tmpY;
    gI16 sinx = sine[angleX], cosx = cosi[angleX];
    gI16 siny = sine[angleY], cosy = cosi[angleY];
    gI16 sinz = sine[angleZ], cosz = cosi[angleZ];

    for (i = 0; i < N; i++)
    {
        tmpX      = (xyz[0][i] * cosx - xyz[2][i] * sinx) / SCALE;
        xyz[2][i] = (xyz[0][i] * sinx + xyz[2][i] * cosx) / SCALE;
        xyz[0][i] = tmpX;

        tmpY      = (xyz[1][i] * cosy - xyz[2][i] * siny) / SCALE;
        xyz[2][i] = (xyz[1][i] * siny + xyz[2][i] * cosy) / SCALE;
        xyz[1][i] = tmpY;

        tmpX      = (xyz[0][i] * cosz - xyz[1][i] * sinz) / SCALE;
        xyz[1][i] = (xyz[0][i] * sinz + xyz[1][i] * cosz) / SCALE;
        xyz[0][i] = tmpX;
    }
}


void draw(gI16 xyz[3][N], gColor col[N])
{
    static gU16 oldProjX[N] = {0};
    static gU16 oldProjY[N] = {0};
    static gU8 oldDotSize[N] = {0};
    gU16 i, projX, projY, projZ, dotSize;

    for (i = 0; i < N; i++)
    {
        projZ = SCALE - (xyz[2][i] + SCALE) / 4;
        projX = width / 2 + (xyz[0][i] * projZ / SCALE) / 25;
        projY = height / 2 + (xyz[1][i] * projZ / SCALE) / 25;
        dotSize = 3 - (xyz[2][i] + SCALE) * 2 / SCALE;

        gdispDrawCircle (oldProjX[i], oldProjY[i], oldDotSize[i], background);

        if (projX > dotSize &&
            projY > dotSize &&
            projX < width - dotSize &&
            projY < height - dotSize)
        {
            gdispDrawCircle (projX, projY, dotSize, col[i]);
            oldProjX[i] = projX;
            oldProjY[i] = projY;
            oldDotSize[i] = dotSize;
        }
    }

    ITM->PORT[2].u8 = 1;           // Write the data
}


/* ---------------------------------------------------------------------- */

gI16 angleX = 0, angleY = 0, angleZ = 0;
gI16 speedX = 0, speedY = 0, speedZ = 0;

gI16 xyz[3][N];
gColor col[N];

void mainThread(void *pvParameters) {
    int pass = 0;

    gfxInit();

    gfxSleepMilliseconds (10);
    gdispClear (background); /* glitches.. */
    gfxSleepMilliseconds (10);
    gdispClear (background);  /* glitches.. */
    gfxSleepMilliseconds (10);
    gdispClear (background);  /* glitches.. */

    width = (gU16)gdispGetWidth();
    height = (gU16)gdispGetHeight();

    initialize();

    for (;;)
    {
        matrix(xyz, col);
        rotate(xyz, angleX, angleY, angleZ);
        draw(xyz, col);

        angleX += speedX;
        angleY += speedY;
        angleZ += speedZ;

        if (pass > 400) speedY = 1;
        if (pass > 800) speedX = 1;
        if (pass > 1200) speedZ = 1;
        pass++;

        if (angleX >= SCALE) angleX -= SCALE;
        else if (angleX < 0) angleX += SCALE;

        if (angleY >= SCALE) angleY -= SCALE;
        else if (angleY < 0) angleY += SCALE;

        if (angleZ >= SCALE) angleZ -= SCALE;
        else if (angleZ < 0) angleZ += SCALE;
    }
}

void __attribute__((no_instrument_function)) InitUART1() {

    GPIO_InitTypeDef gpioInitStructure;
    __HAL_RCC_GPIOI_CLK_ENABLE();
    gpioInitStructure.Pin = GPIO_PIN_1;
    gpioInitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    gpioInitStructure.Pull = GPIO_PULLUP;
    gpioInitStructure.Speed = GPIO_SPEED_HIGH;

    HAL_GPIO_Init(GPIOI, &gpioInitStructure);

    uart1.Instance = USART1;
    uart1.Init.BaudRate = 9600;
    uart1.Init.WordLength = UART_WORDLENGTH_8B;
    uart1.Init.StopBits = UART_STOPBITS_1;
    uart1.Init.Parity = UART_PARITY_NONE;
    uart1.Init.Mode = UART_MODE_TX_RX;
    uart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    uart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    HAL_UART_Init(&uart1);

}

int main(void) {
    BaseType_t xReturned;
    TaskHandle_t xHandle = NULL;
    initAll();
    void* some_persisted_pointer_to_trace = malloc(415);
#ifdef USE_HW_TIMER
    InitTimer();
#endif
#ifdef BACKPRESSURE_ENABLED
    activateBackpressure();
#endif
#ifdef ITM_ENABLED
    initITM();
#endif
#ifdef UART1_ENABLED
    InitUART1();
#endif
    xReturned = xTaskCreate(
            mainThread,       /* Function that implements the task. */
            "Main",          /* Text name for the task. */
            1024,      /* Stack size in words, not bytes. */
            (void *) 1,    /* Parameter passed into the task. */
            tskIDLE_PRIORITY,/* Priority at which the task is created. */
            &xHandle);


    xReturned = xTaskCreate(
            backgroundThread,       /* Function that implements the task. */
            "Background",          /* Text name for the task. */
            256,      /* Stack size in words, not bytes. */
            (void *) 1,    /* Parameter passed into the task. */
            tskIDLE_PRIORITY,/* Priority at which the task is created. */
            &xHandle);

    vTaskStartScheduler();
    for (;;) {
    }
}

#pragma clang diagnostic pop
