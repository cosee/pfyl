// Mandelbrot-Set Demo Application
// Created by mohamad on 11.12.19.
//

#include <stm32f7xx_hal.h>
#include <stm32746g_discovery.h>
#include <stdlib.h>
#include <cmsis_os.h>
#include <stm32f7xx_hal_tim.h>
#include <backpressure.h>
#include <flow_helper.h>
#include "sink_entity.h"
#include "FreeRTOS.h"
#include "gfx.h"


#pragma clang diagnostic ignored "-Wmissing-noreturn" // disable clang-tidy errors

static UART_HandleTypeDef uart1;


#ifdef BACKPRESSURE_ENABLED
static UBaseType_t tempUxSavedInterruptStatus;
#endif

//TODO: move to processor
inline void __attribute__((no_instrument_function)) pfyl_begin_transfer() {
#if defined(BACKPRESSURE_ENABLED)
    tempUxSavedInterruptStatus = taskENTER_CRITICAL_FROM_ISR();
#else
    taskENTER_CRITICAL();
#endif
}

uint32_t __attribute__((no_instrument_function)) pfyl_transfer(const uint8_t *buf, size_t size) {
#ifdef UART1_ENABLED
    return HAL_UART_Transmit(&uart1, buf, size, HAL_UART_TIMEOUT_VALUE);
#else
    return 0;
#endif
}

inline void __attribute__((no_instrument_function)) pfyl_end_transfer() {
#if defined(BACKPRESSURE_ENABLED)
    taskEXIT_CRITICAL_FROM_ISR(tempUxSavedInterruptStatus);
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
//        flush_buffer();
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


#define LOG_DEBUG_LEVEL 24
#define LOG_INFO_LEVEL 25
#define LOG_WARN_LEVEL 26
#define LOG_ERROR_LEVEL 27

#define LOG_DEBUG(msg) println(msg, LOG_DEBUG_LEVEL)
#define LOG_INFO(msg) println(msg, LOG_INFO_LEVEL)
#define LOG_WARN(msg) println(msg, LOG_WARN_LEVEL)
#define LOG_ERROR(msg) println(msg, LOG_ERROR_LEVEL)

void print(char _char, uint8_t level) {
    while (ITM->PORT[level].u32 == 0UL) {
        __NOP();
    }
    ITM->PORT[level].u8 = (uint8_t) _char;
}

void println(const char* msg, uint8_t level) {
    if(msg == NULL || level > 31) {
        return;
    }
    for(int i = 0; i<strlen(msg); i++) {
        print(msg[i], level);
    }
    print('\n', level);
}

void itm_test(void *pvParameters) {
    while(1) {
        LOG_DEBUG("Hello World from DEBUG!");
        LOG_INFO("Hello World from INFO!");
        LOG_WARN("Hello World from WARN!");
        LOG_ERROR("Hello World from ERROR!");
        vTaskDelay(1000);
    }
}

// copyright ugfx.io
void mandelbrot(float x1, float y1, float x2, float y2) {
    unsigned int i, j, width, height;
    uint16_t iter;
    color_t color;
    float fwidth, fheight;

    float sy = y2 - y1;
    float sx = x2 - x1;
    const int MAX = 1024;

    width = (unsigned int) gdispGetWidth();
    height = (unsigned int) gdispGetHeight();
    fwidth = width;
    fheight = height;
    void* test_entity = malloc(256);

    for (i = 0; i < width; i++) {
        for (j = 0; j < height; j++) {
            float cy = j * sy / fheight + y1;
            float cx = i * sx / fwidth + x1;
            float x = 0.0f, y = 0.0f, xx = 0.0f, yy = 0.0f;
            for (iter = 0; iter <= MAX && xx + yy < 4.0f; iter++) {
                xx = x * x;
                yy = y * y;
                y = 2.0f * x * y + cy;
                x = xx - yy + cx;
            }
            color = ((iter << 8) | (iter & 0xFF));
            gdispDrawPixel(i, j, color);
        }
    }
    free(test_entity);
}

void zoomIntoMandelbrot(float factor, float zoomThresholdReset, float offsetX, float offsetY) {
    static float zoom = 1.0f;
    mandelbrot(-2.0f * zoom + offsetX, -1.5f * zoom + offsetY, 2.0f * zoom + offsetX, 1.5f * zoom + offsetY);

    zoom *= factor;
    if (zoom <= zoomThresholdReset)
        zoom = 1.0f;
}

void mainThread(void *pvParameters) {
    gfxInit();
    while (TRUE) {
        zoomIntoMandelbrot(0.7f, 0.001f, -0.086f, 0.85f);
#ifdef PFYL_USE_COOPERATIVE
        taskYIELD();
#endif
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

#ifdef USE_HW_TIMER
static TIM_HandleTypeDef s_TimerInstance;

void  __attribute__((no_instrument_function)) InitTimer() {
    __HAL_RCC_GPIOA_CLK_ENABLE();
            __TIM2_CLK_ENABLE();
    s_TimerInstance.Init.Prescaler = 80; // -> 1 MHz
    s_TimerInstance.Init.CounterMode = TIM_COUNTERMODE_UP;
    s_TimerInstance.Init.Period = 100; // -> 1ms
    s_TimerInstance.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    s_TimerInstance.Init.RepetitionCounter = 0;
    s_TimerInstance.Instance = TIM2;
    HAL_TIM_Base_Init(&s_TimerInstance);
    HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
    HAL_TIM_Base_Start_IT(&s_TimerInstance);
}



void __attribute__((no_instrument_function)) TIM2_IRQHandler() {
    HAL_TIM_IRQHandler(&s_TimerInstance);
}

void __attribute__((no_instrument_function))  HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    increase_tick();
}
#endif

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
