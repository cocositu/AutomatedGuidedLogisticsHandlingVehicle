#ifndef _CONFIG_H_
#define _CONFIG_H_

#define STM32_STD_DRVER 
#define STEPPER_MOTOR_DRIVER
//#define TOP_LEVEL
#define BOTTOM_LEVEL

#ifdef  STM32_STD_DRVER

    #ifdef STM32F4xx
        #include "stm32f4xx.h"
    #elif  STM32F1xx
        #include "stm32f10x.h"
    #endif
#endif 

#define NULL    0
#define True    1
#define False   0
#define true    1
#define false   0

#define PI                   (3.1415926535f)


typedef unsigned char uint8_t;
typedef uint8_t bool;
// typedef enum {
// 	false = 0, true = 1
// } bool;

#ifdef TOP_LEVEL

#define QRCODE_UART                      UART5
#define QRCODE_GPIO_AF_UART              GPIO_AF_UART5
#define QRCODE_UART_IRQn                 UART5_IRQn
#define QRCODE_UART_CLK                  RCC_APB1Periph_UART5
#define QRCODE_RX_GPIO                   GPIOD
#define QRCODE_TX_GPIO                   GPIOC
#define QRCODE_RX_GPIO_PIN               GPIO_Pin_2
#define QRCODE_TX_GPIO_PIN               GPIO_Pin_12
#define QRCODE_RX_GPIO_PINSOURCE         GPIO_PinSource2
#define QRCODE_TX_GPIO_PINSOURCE         GPIO_PinSource12
#define QRCODE_RX_GPIO_CLK               RCC_AHB1Periph_GPIOD
#define QRCODE_TX_GPIO_CLK               RCC_AHB1Periph_GPIOC
#define QRCODE_FUN_UART_CLK(NewState)    RCC_APB1PeriphClockCmd(QRCODE_UART_CLK, NewState)
#define QRCODE_RX_FUN_GPIO_CLK(NewState) RCC_AHB1PeriphClockCmd(QRCODE_RX_GPIO_CLK, NewState)
#define QRCODE_TX_FUN_GPIO_CLK(NewState) RCC_AHB1PeriphClockCmd(QRCODE_TX_GPIO_CLK, NewState)
#define QRCODE_IRQHandler(x)             UART5_IRQHandler(x)

// #define COMM_UART                      USART1
// #define COMM_GPIO_AF_UART              GPIO_AF_USART1
// #define COMM_UART_IRQn                 USART1_IRQn
// #define COMM_UART_CLK                  RCC_APB2Periph_USART1
// #define COMM_RX_GPIO                   GPIOB
// #define COMM_TX_GPIO                   GPIOB
// #define COMM_RX_GPIO_PIN               GPIO_Pin_7
// #define COMM_TX_GPIO_PIN               GPIO_Pin_6
// #define COMM_RX_GPIO_PINSOURCE         GPIO_PinSource7
// #define COMM_TX_GPIO_PINSOURCE         GPIO_PinSource6
// #define COMM_RX_GPIO_CLK               RCC_AHB1Periph_GPIOB
// #define COMM_TX_GPIO_CLK               RCC_AHB1Periph_GPIOB
// #define COMM_FUN_UART_CLK(NewState)    RCC_APB2PeriphClockCmd(COMM_UART_CLK, NewState)
// #define COMM_RX_FUN_GPIO_CLK(NewState) RCC_AHB1PeriphClockCmd(COMM_RX_GPIO_CLK, NewState)
// #define COMM_TX_FUN_GPIO_CLK(NewState) RCC_AHB1PeriphClockCmd(COMM_TX_GPIO_CLK, NewState)
// #define COMM_IRQHandler(x)             USART1_IRQHandler(x)

#define MAN_SEP_UART                           USART6
#define MAN_SEP_UART_GPIO_AF_UART              GPIO_AF_USART6
#define MAN_SEP_UART_UART_CLK                  RCC_APB2Periph_USART6
#define MAN_SEP_UART_RX_GPIO                   GPIOG
#define MAN_SEP_UART_TX_GPIO                   GPIOG
#define MAN_SEP_UART_RX_GPIO_PIN               GPIO_Pin_9
#define MAN_SEP_UART_TX_GPIO_PIN               GPIO_Pin_14
#define MAN_SEP_UART_RX_GPIO_PINSOURCE         GPIO_PinSource9
#define MAN_SEP_UART_TX_GPIO_PINSOURCE         GPIO_PinSource14
#define MAN_SEP_UART_RX_GPIO_CLK               RCC_AHB1Periph_GPIOG
#define MAN_SEP_UART_TX_GPIO_CLK               RCC_AHB1Periph_GPIOG
#define MAN_SEP_UART_FUN_UART_CLK(NewState)    RCC_APB2PeriphClockCmd(MAN_SEP_UART_UART_CLK, NewState)
#define MAN_SEP_UART_RX_FUN_GPIO_CLK(NewState) RCC_AHB1PeriphClockCmd(MAN_SEP_UART_RX_GPIO_CLK, NewState)
#define MAN_SEP_UART_TX_FUN_GPIO_CLK(NewState) RCC_AHB1PeriphClockCmd(MAN_SEP_UART_TX_GPIO_CLK, NewState)
#define MAN_SEP_UART_DMA                       DMA2
#define MAN_SEP_UART_DMA_STREAM                DMA2_Stream6
#define MAN_SEP_UART_DMA_FLAG_TCIF             DMA_FLAG_TCIF6
#define MAN_SEP_UART_DMA_CHANNEL               DMA_Channel_5
#define MAN_SEP_UART_SEND_ADRESS               USART6->DR
#define MAN_SEP_UART_DMA_CLK                   RCC_AHB1Periph_DMA2                            
#define MAN_SEP_UART_FUN_DMA_CLK(NewState)     RCC_AHB1PeriphClockCmd(MAN_SEP_UART_DMA_CLK , NewState)

#define MAN_SERVO_UART                           USART2
#define MAN_SERVO_UART_GPIO_AF_UART              GPIO_AF_USART2
#define MAN_SERVO_UART_UART_CLK                  RCC_APB1Periph_USART2
#define MAN_SERVO_UART_RX_GPIO                   GPIOD
#define MAN_SERVO_UART_TX_GPIO                   GPIOD
#define MAN_SERVO_UART_RX_GPIO_PIN               GPIO_Pin_6
#define MAN_SERVO_UART_TX_GPIO_PIN               GPIO_Pin_5
#define MAN_SERVO_UART_RX_GPIO_PINSOURCE         GPIO_PinSource6
#define MAN_SERVO_UART_TX_GPIO_PINSOURCE         GPIO_PinSource5
#define MAN_SERVO_UART_RX_GPIO_CLK               RCC_AHB1Periph_GPIOD
#define MAN_SERVO_UART_TX_GPIO_CLK               RCC_AHB1Periph_GPIOD
#define MAN_SERVO_UART_FUN_UART_CLK(NewState)    RCC_APB1PeriphClockCmd(MAN_SERVO_UART_UART_CLK, NewState)
#define MAN_SERVO_UART_RX_FUN_GPIO_CLK(NewState) RCC_AHB1PeriphClockCmd(MAN_SERVO_UART_RX_GPIO_CLK, NewState)
#define MAN_SERVO_UART_TX_FUN_GPIO_CLK(NewState) RCC_AHB1PeriphClockCmd(MAN_SERVO_UART_TX_GPIO_CLK, NewState)
#define MAN_SERVO_UART_DMA                       DMA1
#define MAN_SERVO_UART_DMA_STREAM                DMA1_Stream6
#define MAN_SERVO_UART_DMA_FLAG_TCIF             DMA_FLAG_TCIF6
#define MAN_SERVO_UART_DMA_CHANNEL               DMA_Channel_4
#define MAN_SERVO_UART_SEND_ADRESS               USART2->DR
#define MAN_SERVO_UART_DMA_CLK                   RCC_AHB1Periph_DMA1                             
#define MAN_SERVO_UART_FUN_DMA_CLK(NewState)     RCC_AHB1PeriphClockCmd(MAN_SERVO_UART_DMA_CLK , NewState)

#define ToOPENMV_UART                           UART4
#define ToOPENMV_UART_IRQn                      UART4_IRQn
#define ToOPENMV_UART_IRQHandler(x)             UART4_IRQHandler(x)
#define ToOPENMV_UART_GPIO_AF_UART              GPIO_AF_UART4
#define ToOPENMV_UART_UART_CLK                  RCC_APB1Periph_UART4
#define ToOPENMV_UART_RX_GPIO                   GPIOC
#define ToOPENMV_UART_TX_GPIO                   GPIOC
#define ToOPENMV_UART_RX_GPIO_PIN               GPIO_Pin_11
#define ToOPENMV_UART_TX_GPIO_PIN               GPIO_Pin_10
#define ToOPENMV_UART_RX_GPIO_PINSOURCE         GPIO_PinSource11
#define ToOPENMV_UART_TX_GPIO_PINSOURCE         GPIO_PinSource10
#define ToOPENMV_UART_RX_GPIO_CLK               RCC_AHB1Periph_GPIOC
#define ToOPENMV_UART_TX_GPIO_CLK               RCC_AHB1Periph_GPIOC
#define ToOPENMV_UART_FUN_UART_CLK(NewState)    RCC_APB1PeriphClockCmd(ToOPENMV_UART_UART_CLK, NewState)
#define ToOPENMV_UART_RX_FUN_GPIO_CLK(NewState) RCC_AHB1PeriphClockCmd(ToOPENMV_UART_RX_GPIO_CLK, NewState)
#define ToOPENMV_UART_TX_FUN_GPIO_CLK(NewState) RCC_AHB1PeriphClockCmd(ToOPENMV_UART_TX_GPIO_CLK, NewState)
// #define ToOPENMV_UART_RX_DMA                       DMA1
// #define ToOPENMV_UART_RX_DMA_STREAM                DMA1_Stream2
// #define ToOPENMV_UART_DMA_IRQn                     DMA1_Stream2_IRQn
// #define ToOPENMV_UART_RX_DMA_IRQHandler(x)         DMA1_Stream2_IRQHandler(x)
// #define ToOPENMV_UART_RX_DMA_FLAG_TCIF             DMA_FLAG_TCIF2
// #define ToOPENMV_UART_RX_DMA_CHANNEL               DMA_Channel_4
// #define ToOPENMV_UART_RES_ADRESS                   UART4->DR
// #define ToOPENMV_UART_RX_DMA_CLK                   RCC_AHB1Periph_DMA1                             
// #define ToOPENMV_UART_RX_FUN_DMA_CLK(NewState)     RCC_AHB1PeriphClockCmd(ToOPENMV_UART_RX_DMA_CLK , NewState)
// #define ToOPENMV_UART_TX_DMA                       DMA1
// #define ToOPENMV_UART_TX_DMA_STREAM                DMA1_Stream4
// #define ToOPENMV_UART_TX_DMA_CHANNEL               DMA_Channel_4
// #define ToOPENMV_UART_SEND_ADRESS                  UART4->DR
// #define ToOPENMV_UART_TX_DMA_CLK                   RCC_AHB1Periph_DMA1                             
// #define ToOPENMV_UART_TX_FUN_DMA_CLK(NewState)     RCC_AHB1PeriphClockCmd(ToOPENMV_UART_TX_DMA_CLK , NewState)


#endif //TOP_LEVEL

#ifdef BOTTOM_LEVEL

#define MOTOR_LF_EN_GPIO                     GPIOA
#define MOTOR_LF_EN_GPIO_PIN                 GPIO_Pin_4
#define MOTOR_LF_EN_GPIO_CLK                 RCC_AHB1Periph_GPIOA
#define MOTOR_LF_FUN_EN_GPIO_CLK(NewState)   RCC_AHB1PeriphClockCmd(MOTOR_LF_EN_GPIO_CLK, NewState)
#define MOTOR_LF_DIR_GPIO                    GPIOA
#define MOTOR_LF_DIR_GPIO_PIN                GPIO_Pin_6
#define MOTOR_LF_DIR_GPIO_CLK                RCC_AHB1Periph_GPIOA
#define MOTOR_LF_FUN_DIR_GPIO_CLK(NewState)  RCC_AHB1PeriphClockCmd(MOTOR_LF_DIR_GPIO_CLK, NewState)
#define MOTOR_LF_TIM                     TIM2
#define MOTOR_LF_AF_TIM                  GPIO_AF_TIM2
#define MOTOR_LF_TIM_CC_ADRESS           TIM2->ARR
#define MOTOR_LF_GPIO                    GPIOA
#define MOTOR_LF_GPIO_PIN                GPIO_Pin_5
#define MOTOR_LF_GPIO_PINSOURCE          GPIO_PinSource5
#define MOTOR_LF_GPIO_CLK                RCC_AHB1Periph_GPIOA
#define MOTOR_LF_FUN_GPIO_CLK(NewState)  RCC_AHB1PeriphClockCmd(MOTOR_LF_GPIO_CLK, NewState)
#define MOTOR_LF_TIM_CLK                 RCC_APB1Periph_TIM2
#define MOTOR_LF_FUN_TIM_CLK(NewState)   RCC_APB1PeriphClockCmd(MOTOR_LF_TIM_CLK, NewState)
#define MOTOR_LF_FUN_TIM_OC_INIT         TIM_OC1Init
#define MOTOR_LF_FUN_TIM_OC_PRE          TIM_OC1PreloadConfig
#define MOTOR_LF_DMA                     DMA1
#define MOTOR_LF_DMA_STREAM              DMA1_Stream5
#define MOTOR_LF_DMA_CHANNEL             DMA_Channel_3
#define MOTOR_LF_TIM_DMA_CC              TIM_DMA_CC1
#define MOTOR_LF_TIM_DMA_FLAG_TCIF       DMA_FLAG_TCIF5
#define MOTOR_LF_TIM_DMA_IT_TCIF         DMA_IT_TCIF5
#define MOTOR_LF_TIM_DMA_IRQn            DMA1_Stream5_IRQn
#define MOTOR_LF_TIM_DMA_IRQHandler(x)   DMA1_Stream5_IRQHandler(x)
#define MOTOR_LF_DMA_CLK                 RCC_AHB1Periph_DMA1                             
#define MOTOR_LF_FUN_DMA_CLK(NewState)   RCC_AHB1PeriphClockCmd(MOTOR_LF_DMA_CLK , NewState)

#define MOTOR_LR_EN_GPIO                     GPIOE
#define MOTOR_LR_EN_GPIO_PIN                 GPIO_Pin_10
#define MOTOR_LR_EN_GPIO_CLK                 RCC_AHB1Periph_GPIOE
#define MOTOR_LR_FUN_EN_GPIO_CLK(NewState)   RCC_AHB1PeriphClockCmd(MOTOR_LR_EN_GPIO_CLK, NewState)
#define MOTOR_LR_DIR_GPIO                    GPIOE
#define MOTOR_LR_DIR_GPIO_PIN                GPIO_Pin_12
#define MOTOR_LR_DIR_GPIO_CLK                RCC_AHB1Periph_GPIOE
#define MOTOR_LR_FUN_DIR_GPIO_CLK(NewState)  RCC_AHB1PeriphClockCmd(MOTOR_LR_DIR_GPIO_CLK, NewState)
#define MOTOR_LR_TIM                        TIM1
#define MOTOR_LR_AF_TIM                     GPIO_AF_TIM1
#define MOTOR_LR_TIM_CC_ADRESS              TIM1->ARR
#define MOTOR_LR_GPIO                       GPIOE
#define MOTOR_LR_GPIO_PIN                   GPIO_Pin_11
#define MOTOR_LR_GPIO_PINSOURCE             GPIO_PinSource11
#define MOTOR_LR_TIM_CLK                    RCC_APB2Periph_TIM1
#define MOTOR_LR_FUN_TIM_CLK(NewState)      RCC_APB2PeriphClockCmd(MOTOR_LR_TIM_CLK, NewState)
#define MOTOR_LR_GPIO_CLK                   RCC_AHB1Periph_GPIOE
#define MOTOR_LR_FUN_GPIO_CLK(NewState)     RCC_AHB1PeriphClockCmd(MOTOR_LR_GPIO_CLK, NewState)
#define MOTOR_LR_FUN_TIM_OC_INIT            TIM_OC2Init
#define MOTOR_LR_FUN_TIM_OC_PRE             TIM_OC2PreloadConfig
#define MOTOR_LR_DMA                        DMA2
#define MOTOR_LR_DMA_STREAM                 DMA2_Stream2
#define MOTOR_LR_DMA_CHANNEL                DMA_Channel_6
#define MOTOR_LR_TIM_DMA_CC                 TIM_DMA_CC2
#define MOTOR_LR_TIM_DMA_FLAG_TCIF          DMA_FLAG_TCIF2
#define MOTOR_LR_TIM_DMA_IT_TCIF            DMA_IT_TCIF2
#define MOTOR_LR_TIM_DMA_IRQn               DMA2_Stream2_IRQn
#define MOTOR_LR_TIM_DMA_IRQHandler(x)      DMA2_Stream2_IRQHandler(x)
#define MOTOR_LR_DMA_CLK                    RCC_AHB1Periph_DMA2                             
#define MOTOR_LR_FUN_DMA_CLK(NewState)      RCC_AHB1PeriphClockCmd(MOTOR_LR_DMA_CLK , NewState)

#define MOTOR_RF_EN_GPIO                    GPIOB
#define MOTOR_RF_EN_GPIO_PIN                GPIO_Pin_3
#define MOTOR_RF_EN_GPIO_CLK                RCC_AHB1Periph_GPIOB
#define MOTOR_RF_FUN_EN_GPIO_CLK(NewState)  RCC_AHB1PeriphClockCmd(MOTOR_RF_EN_GPIO_CLK, NewState)
#define MOTOR_RF_DIR_GPIO                   GPIOB
#define MOTOR_RF_DIR_GPIO_PIN               GPIO_Pin_5
#define MOTOR_RF_DIR_GPIO_CLK               RCC_AHB1Periph_GPIOB
#define MOTOR_RF_FUN_DIR_GPIO_CLK(NewState) RCC_AHB1PeriphClockCmd(MOTOR_RF_DIR_GPIO_CLK, NewState)
#define MOTOR_RF_TIM                        TIM3
#define MOTOR_RF_AF_TIM                     GPIO_AF_TIM3
#define MOTOR_RF_TIM_CC_ADRESS              TIM3->ARR
#define MOTOR_RF_GPIO                       GPIOB
#define MOTOR_RF_GPIO_PIN                   GPIO_Pin_4
#define MOTOR_RF_GPIO_PINSOURCE             GPIO_PinSource4
#define MOTOR_RF_TIM_CLK                    RCC_APB1Periph_TIM3
#define MOTOR_RF_FUN_TIM_CLK(NewState)      RCC_APB1PeriphClockCmd(MOTOR_RF_TIM_CLK, NewState)
#define MOTOR_RF_GPIO_CLK                   RCC_AHB1Periph_GPIOB
#define MOTOR_RF_FUN_GPIO_CLK(NewState)     RCC_AHB1PeriphClockCmd(MOTOR_RF_GPIO_CLK, NewState)
#define MOTOR_RF_FUN_TIM_OC_INIT            TIM_OC1Init
#define MOTOR_RF_FUN_TIM_OC_PRE             TIM_OC1PreloadConfig
#define MOTOR_RF_DMA                        DMA1
#define MOTOR_RF_DMA_STREAM                 DMA1_Stream4
#define MOTOR_RF_DMA_CHANNEL                DMA_Channel_5
#define MOTOR_RF_TIM_DMA_CC                 TIM_DMA_CC1
#define MOTOR_RF_TIM_DMA_FLAG_TCIF          DMA_FLAG_TCIF4
#define MOTOR_RF_TIM_DMA_IT_TCIF            DMA_IT_TCIF4
#define MOTOR_RF_TIM_DMA_IRQn               DMA1_Stream4_IRQn
#define MOTOR_RF_TIM_DMA_IRQHandler(x)      DMA1_Stream4_IRQHandler(x)
#define MOTOR_RF_DMA_CLK                    RCC_AHB1Periph_DMA1                             
#define MOTOR_RF_FUN_DMA_CLK(NewState)      RCC_AHB1PeriphClockCmd(MOTOR_RF_DMA_CLK , NewState)

#define MOTOR_RR_EN_GPIO                    GPIOC
#define MOTOR_RR_EN_GPIO_PIN                GPIO_Pin_10
#define MOTOR_RR_EN_GPIO_CLK                RCC_AHB1Periph_GPIOC
#define MOTOR_RR_FUN_EN_GPIO_CLK(NewState)  RCC_AHB1PeriphClockCmd(MOTOR_RR_EN_GPIO_CLK, NewState)
#define MOTOR_RR_DIR_GPIO                    GPIOC
#define MOTOR_RR_DIR_GPIO_PIN                GPIO_Pin_11
#define MOTOR_RR_DIR_GPIO_CLK                RCC_AHB1Periph_GPIOC
#define MOTOR_RR_FUN_DIR_GPIO_CLK(NewState)  RCC_AHB1PeriphClockCmd(MOTOR_RR_DIR_GPIO_CLK, NewState)
#define MOTOR_RR_TIM                     TIM8
#define MOTOR_RR_AF_TIM                  GPIO_AF_TIM8
#define MOTOR_RR_TIM_CC_ADRESS           TIM8->ARR
#define MOTOR_RR_GPIO                    GPIOC
#define MOTOR_RR_GPIO_PIN                GPIO_Pin_8
#define MOTOR_RR_GPIO_PINSOURCE          GPIO_PinSource8
#define MOTOR_RR_TIM_CLK                 RCC_APB2Periph_TIM8
#define MOTOR_RR_FUN_TIM_CLK(NewState)   RCC_APB2PeriphClockCmd(MOTOR_RR_TIM_CLK, NewState)
#define MOTOR_RR_GPIO_CLK                RCC_AHB1Periph_GPIOC
#define MOTOR_RR_FUN_GPIO_CLK(NewState)  RCC_AHB1PeriphClockCmd(MOTOR_RR_GPIO_CLK, NewState)
#define MOTOR_RR_FUN_TIM_OC_INIT         TIM_OC3Init
#define MOTOR_RR_FUN_TIM_OC_PRE          TIM_OC3PreloadConfig
#define MOTOR_RR_DMA                     DMA2
#define MOTOR_RR_DMA_STREAM              DMA2_Stream4
#define MOTOR_RR_DMA_CHANNEL             DMA_Channel_7
#define MOTOR_RR_TIM_DMA_CC              TIM_DMA_CC3
#define MOTOR_RR_TIM_DMA_FLAG_TCIF       DMA_FLAG_TCIF4
#define MOTOR_RR_TIM_DMA_IT_TCIF         DMA_IT_TCIF4
#define MOTOR_RR_TIM_DMA_IRQn            DMA2_Stream4_IRQn
#define MOTOR_RR_TIM_DMA_IRQHandler(x)   DMA2_Stream4_IRQHandler(x)
#define MOTOR_RR_DMA_CLK                 RCC_AHB1Periph_DMA2                             
#define MOTOR_RR_FUN_DMA_CLK(NewState)   RCC_AHB1PeriphClockCmd(MOTOR_RR_DMA_CLK , NewState)

#define IMU_UART                            UART4
#define IMU_GPIO_AF_UART                    GPIO_AF_UART4
#define IMU_UART_IRQn                       UART4_IRQn
#define IMU_UART_IRQHandler(x)              UART4_IRQHandler(x)
#define IMU_UART_CLK                        RCC_APB1Periph_UART4
#define IMU_RX_GPIO                         GPIOA
#define IMU_TX_GPIO                         GPIOA
#define IMU_RX_GPIO_PIN                     GPIO_Pin_1
#define IMU_TX_GPIO_PIN                     GPIO_Pin_0
#define IMU_RX_GPIO_PINSOURCE               GPIO_PinSource1
#define IMU_TX_GPIO_PINSOURCE               GPIO_PinSource0
#define IMU_RX_GPIO_CLK                     RCC_AHB1Periph_GPIOA
#define IMU_TX_GPIO_CLK                     RCC_AHB1Periph_GPIOA
#define IMU_FUN_UART_CLK(NewState)          RCC_APB1PeriphClockCmd(IMU_UART_CLK, NewState)
#define IMU_RX_FUN_GPIO_CLK(NewState)       RCC_AHB1PeriphClockCmd(IMU_RX_GPIO_CLK, NewState)
#define IMU_TX_FUN_GPIO_CLK(NewState)       RCC_AHB1PeriphClockCmd(IMU_TX_GPIO_CLK, NewState)
#define IMU_UART_DMA                        DMA1
#define IMU_UART_DMA_STREAM                 DMA1_Stream2
#define IMU_UART_DMA_IRQn                   DMA1_Stream2_IRQn
#define IMU_UART_DMA_IRQHandler(x)          DMA1_Stream2_IRQHandler(x)
#define IMU_UART_DMA_FLAG_TCIF              DMA_FLAG_TCIF2
#define IMU_UART_DMA_CHANNEL                DMA_Channel_4
#define IMU_UART_SEND_ADRESS                UART4->DR
#define IMU_UART_DMA_CLK                    RCC_AHB1Periph_DMA1                             
#define IMU_UART_FUN_DMA_CLK(NewState)      RCC_AHB1PeriphClockCmd(IMU_UART_DMA_CLK , NewState)

#define MOTOR_LF_UART                           USART2
#define MOTOR_LF_UART_GPIO_AF_UART              GPIO_AF_USART2
#define MOTOR_LF_UART_UART_CLK                  RCC_APB1Periph_USART2
#define MOTOR_LF_UART_RX_GPIO                   GPIOA
#define MOTOR_LF_UART_TX_GPIO                   GPIOA
#define MOTOR_LF_UART_RX_GPIO_PIN               GPIO_Pin_3
#define MOTOR_LF_UART_TX_GPIO_PIN               GPIO_Pin_2
#define MOTOR_LF_UART_RX_GPIO_PINSOURCE         GPIO_PinSource3
#define MOTOR_LF_UART_TX_GPIO_PINSOURCE         GPIO_PinSource2
#define MOTOR_LF_UART_RX_GPIO_CLK               RCC_AHB1Periph_GPIOA
#define MOTOR_LF_UART_TX_GPIO_CLK               RCC_AHB1Periph_GPIOA
#define MOTOR_LF_UART_FUN_UART_CLK(NewState)    RCC_APB1PeriphClockCmd(MOTOR_LF_UART_UART_CLK, NewState)
#define MOTOR_LF_UART_RX_FUN_GPIO_CLK(NewState) RCC_AHB1PeriphClockCmd(MOTOR_LF_UART_RX_GPIO_CLK, NewState)
#define MOTOR_LF_UART_TX_FUN_GPIO_CLK(NewState) RCC_AHB1PeriphClockCmd(MOTOR_LF_UART_TX_GPIO_CLK, NewState)
#define MOTOR_LF_UART_DMA                       DMA1
#define MOTOR_LF_UART_DMA_STREAM                DMA1_Stream6
#define MOTOR_LF_UART_DMA_CHANNEL               DMA_Channel_4
#define MOTOR_LF_UART_SEND_ADRESS               USART2->DR
#define MOTOR_LF_UART_DMA_CLK                   RCC_AHB1Periph_DMA1                             
#define MOTOR_LF_UART_FUN_DMA_CLK(NewState)     RCC_AHB1PeriphClockCmd(MOTOR_LF_UART_DMA_CLK , NewState)

#define MOTOR_LR_UART                           USART3
#define MOTOR_LR_UART_GPIO_AF_UART              GPIO_AF_USART3
#define MOTOR_LR_UART_UART_CLK                  RCC_APB1Periph_USART3
#define MOTOR_LR_UART_RX_GPIO                   GPIOB
#define MOTOR_LR_UART_TX_GPIO                   GPIOB
#define MOTOR_LR_UART_RX_GPIO_PIN               GPIO_Pin_11
#define MOTOR_LR_UART_TX_GPIO_PIN               GPIO_Pin_10
#define MOTOR_LR_UART_RX_GPIO_PINSOURCE         GPIO_PinSource11
#define MOTOR_LR_UART_TX_GPIO_PINSOURCE         GPIO_PinSource10
#define MOTOR_LR_UART_RX_GPIO_CLK               RCC_AHB1Periph_GPIOB
#define MOTOR_LR_UART_TX_GPIO_CLK               RCC_AHB1Periph_GPIOB
#define MOTOR_LR_UART_FUN_UART_CLK(NewState)    RCC_APB1PeriphClockCmd(MOTOR_LR_UART_UART_CLK, NewState)
#define MOTOR_LR_UART_RX_FUN_GPIO_CLK(NewState) RCC_AHB1PeriphClockCmd(MOTOR_LR_UART_RX_GPIO_CLK, NewState)
#define MOTOR_LR_UART_TX_FUN_GPIO_CLK(NewState) RCC_AHB1PeriphClockCmd(MOTOR_LR_UART_TX_GPIO_CLK, NewState)
#define MOTOR_LR_UART_DMA                       DMA1
#define MOTOR_LR_UART_DMA_STREAM                DMA1_Stream3
#define MOTOR_LR_UART_DMA_CHANNEL               DMA_Channel_4
#define MOTOR_LR_UART_SEND_ADRESS               USART3->DR
#define MOTOR_LR_UART_DMA_CLK                   RCC_AHB1Periph_DMA1                             
#define MOTOR_LR_UART_FUN_DMA_CLK(NewState)     RCC_AHB1PeriphClockCmd(MOTOR_LR_UART_DMA_CLK , NewState)

#define MOTOR_RF_UART                           USART1
#define MOTOR_RF_UART_GPIO_AF_UART              GPIO_AF_USART1
#define MOTOR_RF_UART_UART_CLK                  RCC_APB2Periph_USART1
#define MOTOR_RF_UART_RX_GPIO                   GPIOB
#define MOTOR_RF_UART_TX_GPIO                   GPIOB
#define MOTOR_RF_UART_RX_GPIO_PIN               GPIO_Pin_7
#define MOTOR_RF_UART_TX_GPIO_PIN               GPIO_Pin_6
#define MOTOR_RF_UART_RX_GPIO_PINSOURCE         GPIO_PinSource7
#define MOTOR_RF_UART_TX_GPIO_PINSOURCE         GPIO_PinSource6
#define MOTOR_RF_UART_RX_GPIO_CLK               RCC_AHB1Periph_GPIOB
#define MOTOR_RF_UART_TX_GPIO_CLK               RCC_AHB1Periph_GPIOB
#define MOTOR_RF_UART_FUN_UART_CLK(NewState)    RCC_APB2PeriphClockCmd(MOTOR_RF_UART_UART_CLK, NewState)
#define MOTOR_RF_UART_RX_FUN_GPIO_CLK(NewState) RCC_AHB1PeriphClockCmd(MOTOR_RF_UART_RX_GPIO_CLK, NewState)
#define MOTOR_RF_UART_TX_FUN_GPIO_CLK(NewState) RCC_AHB1PeriphClockCmd(MOTOR_RF_UART_TX_GPIO_CLK, NewState)
#define MOTOR_RF_UART_DMA                       DMA2
#define MOTOR_RF_UART_DMA_STREAM                DMA2_Stream7
#define MOTOR_RF_UART_DMA_CHANNEL               DMA_Channel_4
#define MOTOR_RF_UART_SEND_ADRESS               USART1->DR
#define MOTOR_RF_UART_DMA_CLK                   RCC_AHB1Periph_DMA2                             
#define MOTOR_RF_UART_FUN_DMA_CLK(NewState)     RCC_AHB1PeriphClockCmd(MOTOR_RF_UART_DMA_CLK , NewState)


#define MOTOR_RR_UART                           UART5
#define MOTOR_RR_UART_GPIO_AF_UART              GPIO_AF_UART5
#define MOTOR_RR_UART_UART_CLK                  RCC_APB1Periph_UART5
#define MOTOR_RR_UART_RX_GPIO                   GPIOD
#define MOTOR_RR_UART_TX_GPIO                   GPIOC
#define MOTOR_RR_UART_RX_GPIO_PIN               GPIO_Pin_2
#define MOTOR_RR_UART_TX_GPIO_PIN               GPIO_Pin_12
#define MOTOR_RR_UART_RX_GPIO_PINSOURCE         GPIO_PinSource2
#define MOTOR_RR_UART_TX_GPIO_PINSOURCE         GPIO_PinSource12
#define MOTOR_RR_UART_RX_GPIO_CLK               RCC_AHB1Periph_GPIOD
#define MOTOR_RR_UART_TX_GPIO_CLK               RCC_AHB1Periph_GPIOC
#define MOTOR_RR_UART_FUN_UART_CLK(NewState)    RCC_APB1PeriphClockCmd(MOTOR_RR_UART_UART_CLK, NewState)
#define MOTOR_RR_UART_RX_FUN_GPIO_CLK(NewState) RCC_AHB1PeriphClockCmd(MOTOR_RR_UART_RX_GPIO_CLK, NewState)
#define MOTOR_RR_UART_TX_FUN_GPIO_CLK(NewState) RCC_AHB1PeriphClockCmd(MOTOR_RR_UART_TX_GPIO_CLK, NewState)
#define MOTOR_RR_UART_DMA                       DMA1
#define MOTOR_RR_UART_DMA_STREAM                DMA1_Stream7
#define MOTOR_RR_UART_DMA_CHANNEL               DMA_Channel_4
#define MOTOR_RR_UART_SEND_ADRESS               UART5->DR
#define MOTOR_RR_UART_DMA_CLK                   RCC_AHB1Periph_DMA1                             
#define MOTOR_RR_UART_FUN_DMA_CLK(NewState)     RCC_AHB1PeriphClockCmd(MOTOR_LF_UART_DMA_CLK , NewState)

#define COMM_UART                      USART6
#define COMM_GPIO_AF_UART              GPIO_AF_USART6
#define COMM_UART_IRQn                 USART6_IRQn
#define COMM_UART_CLK                  RCC_APB2Periph_USART6
#define COMM_RX_GPIO                   GPIOC
#define COMM_TX_GPIO                   GPIOC
#define COMM_RX_GPIO_PIN               GPIO_Pin_7
#define COMM_TX_GPIO_PIN               GPIO_Pin_6
#define COMM_RX_GPIO_PINSOURCE         GPIO_PinSource7
#define COMM_TX_GPIO_PINSOURCE         GPIO_PinSource6
#define COMM_RX_GPIO_CLK               RCC_AHB1Periph_GPIOC
#define COMM_TX_GPIO_CLK               RCC_AHB1Periph_GPIOC
#define COMM_FUN_UART_CLK(NewState)    RCC_APB2PeriphClockCmd(COMM_UART_CLK, NewState)
#define COMM_RX_FUN_GPIO_CLK(NewState) RCC_AHB1PeriphClockCmd(COMM_RX_GPIO_CLK, NewState)
#define COMM_TX_FUN_GPIO_CLK(NewState) RCC_AHB1PeriphClockCmd(COMM_TX_GPIO_CLK, NewState)
#define COMM_IRQHandler(x)             USART6_IRQHandler(x)

#endif //BOTTOM_LEVEL
#endif //_CONFIG_H_


