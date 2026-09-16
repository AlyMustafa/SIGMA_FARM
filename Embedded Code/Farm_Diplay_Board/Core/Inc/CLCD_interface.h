/*************************************************************/
/*************************************************************/
/***********       Author : Mohamed Ahmed      ***************/
/***********       File : CLCD_interface.h     ***************/
/***********       Version : 1.0               ***************/
/*************************************************************/
/*************************************************************/



#ifndef CLCD_INTERFACE_H_
#define CLCD_INTERFACE_H_

void CLCD_voidSendCmd(uint8_t Copy_u8Cmd);


void CLCD_voidSendData(uint8_t Copy_u8Data);


void CLCD_voidInit(void);

uint8_t CLCD_u8SendString(const char* Copy_chString);

void CLCD_voidSendNumber(int32_t Copy_s32Number);

void CLCD_voidGoToXY(uint8_t Copy_u8XPos,uint8_t Copy_u8YPos);

void CLCD_voidSendSpecialCharacter(uint8_t *Copy_pu8Pattern, uint8_t Copy_u8PatternNum,uint8_t Copy_u8XPos, uint8_t Copyu8YPos);

uint8_t CLCD_u8SendLenghOfString(const char* Copy_chString,uint8_t Copy_u8Lengh);

void CLCD_voidSendFloatNumber(int32_t Copy_s32Number);

#endif
