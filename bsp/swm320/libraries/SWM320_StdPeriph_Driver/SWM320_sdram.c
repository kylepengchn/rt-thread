/****************************************************************************************************************************************** 
* �ļ�����: SWM320_sdram.c
* ����˵��:	SWM320��Ƭ����SDRAM��������
* ����֧��:	http://www.synwit.com.cn/e/tool/gbook/?bid=1
* ע������:
* �汾����: V1.1.0		2017��10��25��
* ������¼: 
*
*
*******************************************************************************************************************************************
* @attention
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH CODING INFORMATION 
* REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME. AS A RESULT, SYNWIT SHALL NOT BE HELD LIABLE 
* FOR ANY DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT 
* OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION CONTAINED HEREIN IN CONN-
* -ECTION WITH THEIR PRODUCTS.
*
* COPYRIGHT 2012 Synwit Technology 
*******************************************************************************************************************************************/
#include "SWM320.h"
#include "SWM320_sdram.h"

/****************************************************************************************************************************************** 
* ��������:	SDRAM_Init()
* ����˵��:	SDRAM��������ʼ��
* ��    ��: SDRAM_InitStructure * initStruct    ���� SDRAM ����������趨ֵ�Ľṹ��
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void SDRAM_Init(SDRAM_InitStructure * initStruct)
{
	uint32_t row_n;
	
	SYS->CLKEN |= (1 << SYS_CLKEN_SDRAM_Pos);
	
	SYS->CLKDIV &= ~SYS_CLKDIV_SDRAM_Msk;
	SYS->CLKDIV |= (1 << SYS_CLKDIV_SDRAM_Pos);		//2��Ƶ
	
	SDRAMC->CR0 = (2 << SDRAMC_CR0_BURSTLEN_Pos) |	//2 Burst LengthΪ4
				  (initStruct->CASLatency << SDRAMC_CR0_CASDELAY_Pos);
	
	SDRAMC->CR1 = (initStruct->CellSize << SDRAMC_CR1_CELLSIZE_Pos) |
				  (initStruct->CellWidth << SDRAMC_CR1_CELL32BIT_Pos) |
				  (initStruct->CellBank << SDRAMC_CR1_BANK_Pos) |
				  (0 << SDRAMC_CR1_32BIT_Pos) |
				  (initStruct->TimeTMRD << SDRAMC_CR1_TMRD_Pos) |
				  (initStruct->TimeTRRD << SDRAMC_CR1_TRRD_Pos) |
				  (initStruct->TimeTRAS << SDRAMC_CR1_TRAS_Pos) |
				  (initStruct->TimeTRC << SDRAMC_CR1_TRC_Pos) |
				  (initStruct->TimeTRCD << SDRAMC_CR1_TRCD_Pos) |
				  (initStruct->TimeTRP << SDRAMC_CR1_TRP_Pos);
	
	SDRAMC->LATCH = 0x02;
	
	switch(initStruct->CellSize)
	{
	case SDRAM_CELLSIZE_16Mb:  row_n = 11; break;
	case SDRAM_CELLSIZE_64Mb:  row_n = 12; break;
	case SDRAM_CELLSIZE_128Mb: row_n = 12; break;
	case SDRAM_CELLSIZE_256Mb: row_n = 13; break;
	default:                   row_n = 13; break;
	}
	
	SDRAMC->REFRESH = (1 << SDRAMC_REFRESH_EN_Pos) |
					  (((SystemCoreClock/2)/1000*64 / (1 << row_n)) << SDRAMC_REFRESH_RATE_Pos);
	
    while(SDRAMC->REFDONE == 0);
}

/****************************************************************************************************************************************** 
* ��������:	SDRAM_Enable()
* ����˵��:	SDRAMʹ�ܣ�ʹ�ܺ�ɶ�дSDRAM
* ��    ��: ��
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void SDRAM_Enable(void)
{
	uint32_t i;
	
	SYS->CLKEN |= (1 << SYS_CLKEN_SDRAM_Pos);
	SDRAMC->REFRESH |= (1 << SDRAMC_REFRESH_EN_Pos);
	
	for(i = 0; i < 100; i++) __NOP();
}

/****************************************************************************************************************************************** 
* ��������:	SDRAM_Disable()
* ����˵��:	SDRAM���ܣ����ܺ�SDRAM��������͹���ģʽ������ˢ�£����ɶ�д
* ��    ��: ��
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void SDRAM_Disable(void)
{
	uint32_t i;
	
	SYS->CLKEN |= (1 << SYS_CLKEN_SDRAM_Pos);
	SDRAMC->REFRESH &= ~(1 << SDRAMC_REFRESH_EN_Pos);
	
	for(i = 0; i < 100; i++) __NOP();
}