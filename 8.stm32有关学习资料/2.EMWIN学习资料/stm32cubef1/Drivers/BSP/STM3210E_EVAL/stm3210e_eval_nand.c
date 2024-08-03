/**
  ******************************************************************************
  * @file    stm3210e_eval_nand.c
  * @author  MCD Application Team
  * @version V6.0.0
  * @date    16-December-2014
  * @brief   This file includes a standard driver for the NAND512W3A2CN6E NAND flash memory 
  *          device mounted on STM3210E-EVAL evaluation board.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */ 

/* File Info : -----------------------------------------------------------------
                                   User NOTES
1. How To use this driver:
--------------------------
   - This driver is used to drive the NAND512W3A2CN6E NAND flash external memory mounted
     on STM3210E-EVAL evaluation board.
   - This driver does not need a specific component driver for the NAND device
     to be included with.

2. Driver description:
---------------------
  + Initialization steps:
     o Initialize the NAND external memory using the BSP_NAND_Init() function. This 
       function includes the MSP layer hardware resources initialization and the
       FMC controller configuration to interface with the external NAND memory.
  
  + NAND flash operations
     o NAND external memory can be accessed with read/write operations once it is
       initialized.
       Read/write operation can be performed with AHB access using the functions
       BSP_NAND_ReadData()/BSP_NAND_WriteData(). The BSP_NAND_WriteData() performs write operation
       of an amount of data by unit (halfword).
     o The function BSP_NAND_Read_ID() returns the chip IDs stored in the structure 
       "NAND_IDTypeDef". (see the NAND IDs in the memory data sheet)
     o Perform erase block operation using the function BSP_NAND_Erase_Block() and by
       specifying the block address. 
    
------------------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
#include "stm3210e_eval_nand.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM3210E_EVAL
  * @{
  */ 
  
/** @defgroup STM3210E_EVAL_NAND STM3210E_EVAL NAND
  * @{
  */ 
  
/* Private typedef -----------------------------------------------------------*/

/** @defgroup STM3210E_EVAL_NAND_Private_Types_Definitions Private_Types_Definitions
  * @{
  */ 
  
/**
  * @}
  */ 

/* Private define ------------------------------------------------------------*/

/** @defgroup STM3210E_EVAL_NAND_Private_Defines Private_Defines
  * @{
  */
  
/**
  * @}
  */ 

/* Private macro -------------------------------------------------------------*/

/** @defgroup STM3210E_EVAL_NAND_Private_Macros Private_Macros
  * @{
  */  
  
/**
  * @}
  */ 

/* Private variables ---------------------------------------------------------*/

/** @defgroup STM3210E_EVAL_NAND_Private_Variables Private_Variables
  * @{
  */       
static NAND_HandleTypeDef nandHandle;
static FSMC_NAND_PCC_TimingTypeDef Timing;

/**
  * @}
  */ 

/* Private function prototypes -----------------------------------------------*/

/** @defgroup STM3210E_EVAL_NAND_Private_Function_Prototypes Private_Function_Prototypes
  * @{
  */ 
static void NAND_MspInit(void);
 
/**
  * @}
  */ 

/* Private functions ---------------------------------------------------------*/
    
/** @defgroup STM3210E_EVAL_NAND_Exported_Functions Exported_Functions
  * @{
  */ 

/**
  * @brief  Initializes the NAND device.
  * @retval NAND memory status
  */
uint8_t BSP_NAND_Init(void)
{ 
  nandHandle.Instance  = FSMC_NAND_DEVICE;
  
  /*NAND Configuration */  
  Timing.SetupTime     = 0;
  Timing.WaitSetupTime = 2; 
  Timing.HoldSetupTime = 1;
  Timing.HiZSetupTime  = 0;
  
  nandHandle.Init.NandBank        = FSMC_NAND_BANK2;
  nandHandle.Init.Waitfeature     = FSMC_NAND_PCC_WAIT_FEATURE_ENABLE;
  nandHandle.Init.MemoryDataWidth = FSMC_NAND_PCC_MEM_BUS_WIDTH_8;
  nandHandle.Init.EccComputation  = FSMC_NAND_ECC_ENABLE;
  nandHandle.Init.ECCPageSize     = FSMC_NAND_ECC_PAGE_SIZE_512BYTE;
  nandHandle.Init.TCLRSetupTime   = 0;
  nandHandle.Init.TARSetupTime    = 0;
  
  nandHandle.Info.BlockNbr      = NAND_MAX_ZONE;
  nandHandle.Info.BlockSize     = NAND_BLOCK_SIZE;
  nandHandle.Info.ZoneSize      = NAND_ZONE_SIZE;
  nandHandle.Info.PageSize      = NAND_PAGE_SIZE; 
  nandHandle.Info.SpareAreaSize = NAND_SPARE_AREA_SIZE;
  
  /* NAND controller initialization */
  NAND_MspInit();
  
  if(HAL_NAND_Init(&nandHandle, &Timing, &Timing) != HAL_OK)
  {
    return NAND_ERROR;
  }
  else
  {
    return NAND_OK;
  }
}

/**
  * @brief  Reads an amount of data from the NAND device.
  * @param  BlockAddress: Block address to Read
  * @param  pData: Pointer to data to be read
  * @param  uwNumPage: Number of Pages to read to Block    
  * @retval NAND memory status
  */
uint8_t BSP_NAND_ReadData(NAND_AddressTypeDef BlockAddress, uint8_t* pData, uint32_t uwNumPage)
{
  /* Read data from NAND */
  if (HAL_NAND_Read_Page(&nandHandle, &BlockAddress, pData, uwNumPage) != HAL_OK)
  {
    return NAND_ERROR;
  }
  
  return NAND_OK;
}

/**
  * @brief  Writes an amount of data to the NAND device.
  * @param  BlockAddress: Block address to Write
  * @param  pData: Pointer to data to be written
  * @param  uwNumPage: Number of Pages to write to Block
  * @retval NAND memory status
  */
uint8_t BSP_NAND_WriteData(NAND_AddressTypeDef BlockAddress, uint8_t* pData, uint32_t uwNumPage)
{
  /* Write data to NAND */
  if (HAL_NAND_Write_Page(&nandHandle, &BlockAddress, pData, uwNumPage) != HAL_OK)
  {
    return NAND_ERROR;
  }
  
  return NAND_OK;
}

/**
  * @brief  Erases the specified block of the NAND device. 
  * @param  BlockAddress: Block address to Erase  
  * @retval NAND memory status
  */
uint8_t BSP_NAND_Erase_Block(NAND_AddressTypeDef BlockAddress)
{
  /* Send NAND erase block operation */
  if (HAL_NAND_Erase_Block(&nandHandle, &BlockAddress) != HAL_OK)
  {
    return NAND_ERROR;
  }
  
  return NAND_OK;
}

/**
  * @brief  Reads NAND flash IDs.
  * @param  pNAND_ID : Pointer to NAND ID structure
  * @retval NAND memory status
  */
uint8_t BSP_NAND_Read_ID(NAND_IDTypeDef *pNAND_ID)
{
  if(HAL_NAND_Read_ID(&nandHandle, pNAND_ID) != HAL_OK)
  {
    return NAND_ERROR;
  }
  else
  {
    return NAND_OK;
  }
}

/**
  * @brief  Initializes the NAND MSP.
  * @retval None
  */
static void NAND_MspInit(void)
{
  GPIO_InitTypeDef gpioinitstruct = {0};
  
  /* Enable FSMC clock */
  __HAL_RCC_FSMC_CLK_ENABLE();
  
  /* Enable GPIOs clock */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  
  /* Common GPIO configuration */
  gpioinitstruct.Mode      = GPIO_MODE_AF_PP;
  gpioinitstruct.Pull      = GPIO_PULLUP;
  gpioinitstruct.Speed     = GPIO_SPEED_HIGH;
  
/*-- GPIO Configuration ------------------------------------------------------*/
/*!< CLE, ALE, D0->D3, NOE, NWE and NCE2  NAND pin configuration  */
  gpioinitstruct.Pin =  GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_14 | GPIO_PIN_15 |  
                                 GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 | 
                                 GPIO_PIN_7;
  HAL_GPIO_Init(GPIOD, &gpioinitstruct); 

/*!< D4->D7 NAND pin configuration  */  
  gpioinitstruct.Pin = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10;

  HAL_GPIO_Init(GPIOE, &gpioinitstruct);


/*!< NWAIT NAND pin configuration */
  gpioinitstruct.Pin = GPIO_PIN_6;
  gpioinitstruct.Mode      = GPIO_MODE_INPUT;

  HAL_GPIO_Init(GPIOD, &gpioinitstruct); 

/*!< INT2 NAND pin configuration */  
  gpioinitstruct.Pin = GPIO_PIN_6;
  HAL_GPIO_Init(GPIOG, &gpioinitstruct);
}

/**
  * @}
  */  
  
/**
  * @}
  */ 
  
/**
  * @}
  */ 
  
/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
