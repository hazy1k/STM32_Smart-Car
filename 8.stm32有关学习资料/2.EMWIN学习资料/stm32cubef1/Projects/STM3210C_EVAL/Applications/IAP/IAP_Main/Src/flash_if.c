/**
  ******************************************************************************
  * @file    IAP_Main/Src/flash_if.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    17-December-2014
  * @brief   This file provides all the memory related operation functions.
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

/** @addtogroup STM32F1xx_IAP
  * @{
  */

/* Includes ------------------------------------------------------------------*/
#include "flash_if.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Unlocks Flash for write access
  * @param  None
  * @retval None
  */
void FLASH_If_Init(void)
{ 
  HAL_FLASH_Unlock(); 

  /* Clear pending flags (if any) */  
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);
}

/**
  * @brief  This function does an erase of all user flash area
  * @param  StartPage: start of user flash area
  * @retval 0: user flash area successfully erased
  *         1: error occurred
  */
uint32_t FLASH_If_Erase(uint32_t StartPage)
{
  uint32_t NbrOfPages = 0;
  uint32_t PageError = 0;
  FLASH_EraseInitTypeDef pEraseInit;
  HAL_StatusTypeDef status;

  /* Unlock the Flash to enable the flash control register access *************/ 
  HAL_FLASH_Unlock();

  /* Get the sector where start the user flash area */
  NbrOfPages = (USER_FLASH_END_ADDRESS - StartPage)/FLASH_PAGE_SIZE;

  pEraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
  pEraseInit.PageAddress = StartPage;
  pEraseInit.Banks = FLASH_BANK_1;
  pEraseInit.NbPages = NbrOfPages;
  status = HAL_FLASHEx_Erase(&pEraseInit, &PageError);

  /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) *********/
  HAL_FLASH_Lock();

  if (status != HAL_OK)
  {
    /* Error occurred while page erase */
    return (1);
  }

  return (0);
}

/**
  * @brief  This function writes a data buffer in flash (data are 32-bit aligned).
  * @note   After writing data buffer, the flash content is checked.
  * @param  FlashAddress: start address for writing data buffer
  * @param  Data: pointer on data buffer
  * @param  DataLength: length of data buffer (unit is 32-bit word)   
  * @retval 0: Data successfully written to Flash memory
  *         1: Error occurred while writing data in Flash memory
  *         2: Written Data in flash memory is different from expected one
  */
uint32_t FLASH_If_Write(__IO uint32_t* FlashAddress, uint32_t* Data ,uint32_t DataLength)
{
  uint32_t i = 0;

  /* Unlock the Flash to enable the flash control register access *************/ 
  HAL_FLASH_Unlock();

  for (i = 0; (i < DataLength) && (*FlashAddress <= (USER_FLASH_END_ADDRESS-4)); i++)
  {
    /* Device voltage range supposed to be [2.7V to 3.6V], the operation will
       be done by word */ 
    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, *FlashAddress, *(uint32_t*)(Data+i)) == HAL_OK)      
    {
     /* Check the written value */
      if (*(uint32_t*)*FlashAddress != *(uint32_t*)(Data+i))
      {
        /* Flash content doesn't match SRAM content */
        return(2);
      }
      /* Increment FLASH destination address */
      *FlashAddress += 4;
    }
    else
    {
      /* Error occurred while writing data in Flash memory */
      return (1);
    }
  }

  /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) *********/
  HAL_FLASH_Lock();

  return (0);
}

/**
  * @brief  Returns the write protection status of user flash area.
  * @param  None
  * @retval 0: Some sectors inside the user flash area are write protected
  *         1: No write protected sectors inside the user flash area
  */
uint16_t FLASH_If_GetWriteProtectionStatus(void)
{
  uint32_t ProtectedPAGE = 0x0;
  FLASH_OBProgramInitTypeDef OptionsBytesStruct;

  /* Unlock the Flash to enable the flash control register access *************/ 
  HAL_FLASH_Unlock();

  /* Check if there are write protected sectors inside the user flash area */
  HAL_FLASHEx_OBGetConfig(&OptionsBytesStruct);

  /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) *********/
  HAL_FLASH_Lock();

  /* Get pages already write protected ****************************************/
  ProtectedPAGE = ~(OptionsBytesStruct.WRPPage) & FLASH_PAGE_TO_BE_PROTECTED;

  /* Check if desired pages are already write protected ***********************/
  if(ProtectedPAGE != 0)
  {
    /* Some sectors inside the user flash area are write protected */
    return 0;
  }
  else
  { 
    /* No write protected sectors inside the user flash area */
    return 1;
  }
}

/**
  * @brief  Disables the write protection of user flash area.
  * @param  None
  * @retval 1: Write Protection successfully disabled
  *         2: Error: Flash write unprotection failed
  */
uint32_t FLASH_If_DisableWriteProtection(void)
{
  uint32_t ProtectedPAGE = 0x0;
  FLASH_OBProgramInitTypeDef OptionsBytesStruct;
  HAL_StatusTypeDef result;
  
  /* Unlock the Flash to enable the flash control register access *************/ 
  HAL_FLASH_Unlock();

  /* Unlock the Options Bytes *************************************************/
  HAL_FLASH_OB_Unlock();

  /* Get pages write protection status ****************************************/
  HAL_FLASHEx_OBGetConfig(&OptionsBytesStruct);

  /* Get pages already write protected ****************************************/
  ProtectedPAGE = ~(OptionsBytesStruct.WRPPage);
  /* Remove user Flash area pages from this set */
  ProtectedPAGE &= ~FLASH_PAGE_TO_BE_PROTECTED;

  /* Erase all the option Bytes *********************************************/
  result = HAL_FLASHEx_OBErase();
  if (result == HAL_OK)
  {
    /* Check if there is write protected pages ********************************/
    if(ProtectedPAGE != 0x0)
    {
      /* Restore write protected pages */
      OptionsBytesStruct.OptionType   = OPTIONBYTE_WRP;
      OptionsBytesStruct.WRPState   = OB_WRPSTATE_ENABLE;
      OptionsBytesStruct.WRPPage    = ProtectedPAGE;
      result = HAL_FLASHEx_OBProgram(&OptionsBytesStruct);
    }
  }

  /* Lock the Options Bytes *************************************************/
  HAL_FLASH_OB_Lock();

  /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) *********/
  HAL_FLASH_Lock();
  
  if (result != HAL_OK)
  {
    /* Error: Flash write unprotection failed */
    return (2);
  }

  /* Write Protection successfully disabled */
  return (1);

}

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
