/**
  ******************************************************************************
  * @file    Project/STM32_CPAL_Examples/STM32_CPAL_I2C/Two_Boards/cpal_usercallback.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    17-June-2011
  * @brief   This file provides all the CPAL UserCallback functions .
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

                                                                                                                                                                                                                                                                                                           
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern const uint8_t tStateSignal[];
extern const uint8_t tSignal1[];
extern const uint8_t tSignal2[];
extern CPAL_TransferTypeDef  sRxStructure, sTxStructure;
extern uint8_t tRxBuffer[];
extern uint32_t BufferSize;
extern __IO uint32_t ActionState;
extern __IO uint32_t RecieverMode;

extern I2C_TypeDef* CPAL_I2C_DEVICE[];

/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
extern void Switch_Color(void); 
extern void Switch_ErrorColor(void);
extern uint8_t Buffer_Check(uint8_t* pBuffer, uint8_t* pBuffer1, uint8_t* pBuffer2,  uint8_t* pBuffer3, uint16_t BufferLength);

/* Private functions ---------------------------------------------------------*/


/*------------------------------------------------------------------------------
                     CPAL User Callbacks implementations 
------------------------------------------------------------------------------*/


/*=========== Timeout UserCallback ===========*/


/**
  * @brief  User callback that manages the Timeout error.
  * @param  pDevInitStruct .
  * @retval None.
  */
uint32_t CPAL_TIMEOUT_UserCallback(CPAL_InitTypeDef* pDevInitStruct)
{
  I2C_DevStructure.CPAL_State = CPAL_STATE_READY;
  I2C_DevStructure.wCPAL_DevError = CPAL_I2C_ERR_NONE ;        /* No Device Error */
  I2C_DevStructure.wCPAL_Timeout  = CPAL_I2C_TIMEOUT_DEFAULT;  /* Stop timeout countdown */ 
  
  /* DeInitialize CPAL device */
  CPAL_I2C_DeInit(&I2C_DevStructure);  
  
  /* Initialize CPAL device with the selected parameters */
  CPAL_I2C_Init(pDevInitStruct);    
  
//  /* Switch the LCD write color */
//  Switch_ErrorColor();
  
//  LCD_DisplayStringLine(Line9, (uint8_t*)"  Timeout Recovered ");
  
  ActionState = ACTION_NONE; 
  
  return CPAL_PASS;  
}



/*=========== Transfer UserCallback ===========*/


/**
  * @brief  Manages the End of Tx transfer event.
  * @param  pDevInitStruct 
  * @retval None
  */
void CPAL_I2C_TXTC_UserCallback(CPAL_InitTypeDef* pDevInitStruct)
{
//  STM_EVAL_LEDOff(LED3);
//  STM_EVAL_LEDToggle(LED2);
     
//  /* Switch the LCD write color */
//  Switch_Color();
  
//  LCD_DisplayStringLine(Line3, (uint8_t*)"TRANSMIT MODE ACTIVE");
//  LCD_DisplayStringLine(Line5, (uint8_t*)"   Signal Sent OK   ");
//  LCD_DisplayStringLine(Line9, MEASSAGE_EPTY);
  
  ActionState  = ACTION_NONE;
}


/**
  * @brief  Manages the End of Rx transfer event.
  * @param  pDevInitStruct 
  * @retval None
  */  
void CPAL_I2C_RXTC_UserCallback(CPAL_InitTypeDef* pDevInitStruct)
{
  uint8_t result = 0xFF, i = 0;
  
  /* Activate the mode receiver only */
  RecieverMode = 1;
  
//  /* Switch the LCD write color */
//  Switch_Color();
  
//  LCD_DisplayStringLine(Line3, (uint8_t*)"RECEIVER MODE ACTIVE");
//  LCD_DisplayStringLine(Line5, MEASSAGE_EPTY);
//  LCD_DisplayStringLine(Line9, MEASSAGE_EPTY);
  
//  STM_EVAL_LEDOff(LED2);
//  STM_EVAL_LEDToggle(LED3);
    
  /* Initialize local Reception structures */
  sRxStructure.wNumData = BufferSize;       /* Maximum Number of data to be received */
  sRxStructure.pbBuffer = tRxBuffer;        /* Common Rx buffer for all received data */
      
//  /* Check the Received Buffer */
//  result = Buffer_Check(tRxBuffer, (uint8_t*)tStateSignal, (uint8_t*)tSignal1,(uint8_t*)tSignal2, (uint16_t)BufferSize);
    
//  switch(result)
//  {
//    case 0:
//      LCD_DisplayStringLine(Line7, (uint8_t*)"  State message OK  ");
//      break;
      
//    case 1:
//      /* Display Reception Complete */
//      LCD_DisplayStringLine(Line5, (uint8_t*)" Signal1 message OK ");
//      break;
    
//    case 2:
//      /* Display Reception Complete */
//      LCD_DisplayStringLine(Line5, (uint8_t*)" Signal2 message OK ");
//      break;
    
//    default:
//      LCD_DisplayStringLine(Line7, (uint8_t*)"       Failure     ");
//      break;
//  }
  
//  /* Reinitialize RXBuffer */
//  for(i = 0; i < MAX_BUFF_SIZE; i++)
//  {
//    tRxBuffer[i]=0;
//  }
}

/**
  * @brief  Manages Tx transfer event.
  * @param  pDevInitStruct 
  * @retval None
  */
/*void CPAL_I2C_TX_UserCallback(CPAL_InitTypeDef* pDevInitStruct)
{
 
}*/


/**
  * @brief  Manages Rx transfer event.
  * @param  pDevInitStruct 
  * @retval None
  */ 
/*void CPAL_I2C_RX_UserCallback(CPAL_InitTypeDef* pDevInitStruct)
{
 
}*/


/**
  * @brief  Manages the End of DMA Tx transfer event.
  * @param  pDevInitStruct 
  * @retval None
  */
/*void CPAL_I2C_DMATXTC_UserCallback(CPAL_InitTypeDef* pDevInitStruct)
{
 
}*/


/**
  * @brief  Manages the Half of DMA Tx transfer event.
  * @param  pDevInitStruct 
  * @retval None
  */
/*void CPAL_I2C_DMATXHT_UserCallback(CPAL_InitTypeDef* pDevInitStruct)
{
   
}*/


/**
  * @brief  Manages Error of DMA Tx transfer event.
  * @param  pDevInitStruct 
  * @retval None
  */
/*void CPAL_I2C_DMATXTE_UserCallback(CPAL_InitTypeDef* pDevInitStruct)
{
   
}*/


/**
  * @brief  Manages the End of DMA Rx transfer event.
  * @param  pDevInitStruct 
  * @retval None
  */
/*void CPAL_I2C_DMARXTC_UserCallback(CPAL_InitTypeDef* pDevInitStruct)
{
 
}*/


/**
  * @brief  Manages the Half of DMA Rx transfer event.
  * @param  pDevInitStruct 
  * @retval None
  */ 
/*void CPAL_I2C_DMARXHT_UserCallback(CPAL_InitTypeDef* pDevInitStruct)
{
   
}*/


/**
  * @brief  Manages Error of DMA Rx transfer event.
  * @param  pDevInitStruct 
  * @retval None
  */ 
/*void CPAL_I2C_DMARXTE_UserCallback(CPAL_InitTypeDef* pDevInitStruct)
{
   
}*/



/*=========== Error UserCallback ===========*/

 
/**
  * @brief  User callback that manages the I2C device errors.
  * @note   Make sure that the define USE_SINGLE_ERROR_CALLBACK is uncommented in
  *         the cpal_conf.h file, otherwise this callback will not be functional.
  * @param  pDevInitStruct. 
  * @param  DeviceError.
  * @retval None
  */ 
void CPAL_I2C_ERR_UserCallback(CPAL_DevTypeDef pDevInstance, uint32_t DeviceError)
{
  /* if an Acknowledge failure error occurred */
  if (I2C_DevStructure.wCPAL_DevError == CPAL_I2C_ERR_AF )
  {
    //LCD_DisplayStringLine(Line9, (uint8_t*)" Slave Not yet Ready");
  } 
  else
  {
   // LCD_DisplayStringLine(Line9, (uint8_t*)" Device Err occurred ");
  }
  
  I2C_DevStructure.CPAL_State = CPAL_STATE_READY;
  I2C_DevStructure.wCPAL_DevError = CPAL_I2C_ERR_NONE ;        /* No Device Error */
  
  /* Deinitialize CPAL device */
  CPAL_I2C_DeInit(&I2C_DevStructure);
  
  /* Initialize CPAL device with the selected parameters */
  CPAL_I2C_Init(&I2C_DevStructure);    
  
  /* Switch the LCD write color */
  //Switch_ErrorColor();
  
  ActionState = ACTION_NONE;
}

/**
  * @brief  User callback that manages BERR I2C device errors.
  * @note   Make sure that the define USE_MULTIPLE_ERROR_CALLBACK is uncommented in
  *         the cpal_conf.h file, otherwise this callback will not be functional.
  * @param  pDevInstance.
  * @retval None
  */  
/*void CPAL_I2C_BERR_UserCallback(CPAL_DevTypeDef pDevInstance)
{
   
}*/


/**
  * @brief  User callback that manages ARLO I2C device errors.
  * @note   Make sure that the define USE_MULTIPLE_ERROR_CALLBACK is uncommented in
  *         the cpal_conf.h file, otherwise this callback will not be functional.
  * @param  pDevInstance.
  * @retval None
  */
/*void CPAL_I2C_ARLO_UserCallback(CPAL_DevTypeDef pDevInstance)
{
   
}*/


/**
  * @brief  User callback that manages OVR I2C device errors.
  * @note   Make sure that the define USE_MULTIPLE_ERROR_CALLBACK is uncommented in
  *         the cpal_conf.h file, otherwise this callback will not be functional.
  * @param  pDevInstance.
  * @retval None
  */
/*void CPAL_I2C_OVR_UserCallback(CPAL_DevTypeDef pDevInstance)
{
   
}*/


/**
  * @brief  User callback that manages AF I2C device errors.
  * @note   Make sure that the define USE_MULTIPLE_ERROR_CALLBACK is uncommented in
  *         the cpal_conf.h file, otherwise this callback will not be functional.
  * @param  pDevInstance.
  * @retval None
  */ 
/*void CPAL_I2C_AF_UserCallback(CPAL_DevTypeDef pDevInstance)
{
   
}*/


/*=========== Addressing Mode UserCallback ===========*/


/**
  * @brief  User callback that manage General Call Addressing mode.
  * @param  pDevInitStruct 
  * @retval None
  */ 
/*void CPAL_I2C_GENCALL_UserCallback(CPAL_InitTypeDef* pDevInitStruct)
{
   
}*/


/**
  * @brief  User callback that manage Dual Address Addressing mode.
  * @param  pDevInitStruct 
  * @retval None
  */  
/*void CPAL_I2C_DUALF_UserCallback(CPAL_InitTypeDef* pDevInitStruct)
{
   
}*/
 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
