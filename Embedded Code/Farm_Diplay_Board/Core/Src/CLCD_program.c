/*************************************************************/
/*************************************************************/
/***********       Author : Mohamed Ahmed      ***************/
/***********       File : CLCD_program.c       ***************/
/***********       Version : 1.0               ***************/
/*************************************************************/
/*************************************************************/


#include"stdint.h"
#include "BIT_MATH.h"

#include "ErrType.h"
#include "main.h"

#include "CLCD_interface.h"
#include "CLCD_prv.h"
#include "CLCD_cfg.h"

static void voidSendEnablePulse(void)
{
	/*Send enable pulse*/
	HAL_GPIO_WritePin(CLCD_CTRL_PORT,CLCD_E_PIN,GPIO_PIN_SET);
	HAL_Delay(2);// delay for more than 1ms
	HAL_GPIO_WritePin(CLCD_CTRL_PORT,CLCD_E_PIN,GPIO_PIN_RESET);

}

#if CLCD_OP_MODE == FOUR_BIT_MODE
static void voidSetLCDHalfDataPort(uint8_t Copy_u8Nipple)
{
	HAL_GPIO_WritePin(CLCD_DATA_PORT,CLCD_D4_PIN,GET_BIT(Copy_u8Nipple,0));
	HAL_GPIO_WritePin(CLCD_DATA_PORT,CLCD_D5_PIN,GET_BIT(Copy_u8Nipple,1));
	HAL_GPIO_WritePin(GPIOD,CLCD_D6_PIN,GET_BIT(Copy_u8Nipple,2));
	HAL_GPIO_WritePin(CLCD_DATA_PORT,CLCD_D7_PIN,GET_BIT(Copy_u8Nipple,3));
}
#endif

void CLCD_voidSendCmd(uint8_t Copy_u8Cmd)
{
	/* Set RS pin to low for command */
	HAL_GPIO_WritePin(CLCD_CTRL_PORT,CLCD_RS_PIN,GPIO_PIN_RESET);

#if CLCD_RW_CTRL_EN == ENABLED
	/* Set RW pin to low for write */
	HAL_GPIO_WritePin(CLCD_CTRL_PORT,CLCD_RW_PIN,GPIO_PIN_RESET);
#endif

#if CLCD_OP_MODE == Eigh_BIT_MODE
	/*Send the command on data pins*/
	DIO_u8SetPortValue(CLCD_DATA_PORT,Copy_u8Cmd);
	voidSendEnablePulse();

#elif CLCD_OP_MODE == FOUR_BIT_MODE
	voidSetLCDHalfDataPort(Copy_u8Cmd>>4); /* send the most four bits to the LCD*/
	voidSendEnablePulse();

	voidSetLCDHalfDataPort(Copy_u8Cmd);     /* send the least four bits to the LCD*/
	voidSendEnablePulse();
#endif
}


void CLCD_voidSendData(uint8_t Copy_u8Data)
{
	/* Set RS pin to high for data */
	HAL_GPIO_WritePin(CLCD_CTRL_PORT,CLCD_RS_PIN,GPIO_PIN_SET);
#if CLCD_RW_CTRL_EN == ENABLED
	/* Set RW pin to low for write */
	DIO_u8SetPinValue(CLCD_CTRL_PORT,CLCD_RW_PIN,DIO_u8PIN_LOW);
#endif


#if CLCD_OP_MODE == Eigh_BIT_MODE
	/*Send the command on data pins*/
	DIO_u8SetPortValue(CLCD_DATA_PORT,Copy_u8Data);
	voidSendEnablePulse();

#elif CLCD_OP_MODE == FOUR_BIT_MODE
	voidSetLCDHalfDataPort(Copy_u8Data>>4); /* send the most four bits to the LCD*/
	voidSendEnablePulse();

	voidSetLCDHalfDataPort(Copy_u8Data);     /* send the least four bits to the LCD*/
	voidSendEnablePulse();
#endif
}


void CLCD_voidInit(void)
{
	/* wait for more 30ms after power on */
	HAL_Delay(40);

	/*function set command  2  lines    5*7  font size*/
#if CLCD_OP_MODE == EIGHT_BIT_MODE
	CLCD_voidSendCmd(0b00111000);
#elif CLCD_OP_MODE == FOUR_BIT_MODE

//	CLCD_voidSendCmd(0b00101000);
//	voidSendEnablePulse();

	//HAL_Delay(40);

	/* Initialize LCD in 4-bit mode */
	voidSetLCDHalfDataPort(0x03);
	voidSendEnablePulse();
	HAL_Delay(5);

	voidSetLCDHalfDataPort(0x03);
	voidSendEnablePulse();
	HAL_Delay(1);

	voidSetLCDHalfDataPort(0x03);
	voidSendEnablePulse();
	HAL_Delay(1);

	voidSetLCDHalfDataPort(0x02);
	voidSendEnablePulse();
	HAL_Delay(1);

	/* Function Set: 4-bit, 2 lines, 5x8 font */
	CLCD_voidSendCmd(0x28);

	/* Display OFF */
	CLCD_voidSendCmd(0x08);

	/* Clear Display */
	CLCD_voidSendCmd(0x01);
	HAL_Delay(2);

	/* Entry Mode Set */
	CLCD_voidSendCmd(0x06);

	/* Display ON, Cursor OFF, Blink OFF */
	CLCD_voidSendCmd(0x0C);

#endif

	/*display on/off control: display enable ,cursor and blink disable for upper tow rows*/
//	CLCD_voidSendCmd(0b00101000);
//
//	/*display on/off control: display enable ,cursor and blink disable for lower tow rows*/
//	CLCD_voidSendCmd(0b00101000);

	//CLCD_voidSendCmd(0b00001100);

	/*Display clear*/
	CLCD_voidSendCmd(0b1);

}


uint8_t CLCD_u8SendString(const char* Copy_chString)
{
	uint8_t Local_u8ErrorState= OK;
	if(Copy_chString != NULL)
	{
		uint8_t Local_u8Counter=0u;
		while(Copy_chString[Local_u8Counter] != '\0')
		{
			CLCD_voidSendData(Copy_chString[Local_u8Counter]);
			Local_u8Counter++;
		}
	}
	else
	{
		Local_u8ErrorState= NOK;
	}
	return Local_u8ErrorState;
}


void CLCD_voidSendNumber(int32_t Copy_s32Number)
{
	char Local_chNumberArr[10];
	uint8_t Local_u8RightDigit, Local_u8Counter=0;
	int32_t Local_s8Counter2;
	if (Copy_s32Number == 0)
	{
		CLCD_voidSendData('0');
		return ;
	}

	else if (Copy_s32Number < 0)
	{
		CLCD_voidSendData('-');
		Copy_s32Number *= -1;
	}
	while (Copy_s32Number != 0)
	{
		Local_u8RightDigit = Copy_s32Number%10;  /* extract the right most digit*/
		Copy_s32Number /= 10;                    /* get rid of the most right digit*/
		Local_chNumberArr[Local_u8Counter] = Local_u8RightDigit + '0';   /* convert to ascii value*/
		Local_u8Counter ++;
	}
	for (Local_s8Counter2 = Local_u8Counter-1; Local_s8Counter2>=0; Local_s8Counter2--)
	{
		CLCD_voidSendData(Local_chNumberArr[Local_s8Counter2]);
	}

}

void CLCD_voidGoToXY(uint8_t Copy_u8XPos,uint8_t Copy_u8YPos)
{
	uint8_t Local_u8DDRamAdd = 0;

	switch (Copy_u8YPos)
	{
	case 0:
		Local_u8DDRamAdd = 0x00 + Copy_u8XPos;
		break;
	case 1:
		Local_u8DDRamAdd = 0x40 + Copy_u8XPos;
		break;
	case 2:
		Local_u8DDRamAdd = 0x14 + Copy_u8XPos;
		break;
	case 3:
		Local_u8DDRamAdd = 0x54 + Copy_u8XPos;
		break;
	}
	/* set bit 7 for set DDRam Address Command*/
	Local_u8DDRamAdd |= (1<<7u);
	CLCD_voidSendCmd(Local_u8DDRamAdd);
}

void CLCD_voidSendSpecialCharacter(uint8_t *Copy_pu8Pattern, uint8_t Copy_u8PatternNum,uint8_t Copy_u8XPos, uint8_t Copyu8YPos)
{
	uint8_t Local_u8LoopCounter;
	uint8_t Local_u8CGRAMAdd = Copy_u8PatternNum * 8u;
	/* set bit number 6 for set CGRAM Address command */
	Local_u8CGRAMAdd |= 1<<6u;
	/* set CGRAM address */
	CLCD_voidSendCmd(Local_u8CGRAMAdd);
	/* write the pattern into CGRAM*/
	for (Local_u8LoopCounter=0; Local_u8LoopCounter<8u;Local_u8LoopCounter++)
	{
		CLCD_voidSendData(Copy_pu8Pattern[Local_u8LoopCounter]);
	}
	/* GoTo DDRAM to Display the pattern*/
	CLCD_voidGoToXY(Copy_u8XPos,Copyu8YPos);
	/* Display the pattern written inside CGRAM*/
	CLCD_voidSendData(Copy_u8PatternNum);

}

uint8_t CLCD_u8SendLenghOfString(const char* Copy_chString,uint8_t Copy_u8Lengh)
{
	uint8_t Local_u8ErrorState= OK;
	if(Copy_chString != NULL)
	{
		uint8_t Local_u8Counter=0u;
		for(Local_u8Counter = 0 ; Local_u8Counter < Copy_u8Lengh ; Local_u8Counter++)
		{
			CLCD_voidSendData(Copy_chString[Local_u8Counter]);

		}
	}
	else
	{
		Local_u8ErrorState= NOK;
	}
	return Local_u8ErrorState;
}


/*

void CLCD_voidSend(uint8_t y,uint8_t x,uint32_t temp)      //send Number(max 99999) to lcd at position(x,y)
{
                    switch(y)
                    {
                    case 1:              SetDDRAMAddr(x + 0x7F);  break;   //first  line
                    case 2:              SetDDRAMAddr(x + 0xBF);  break;   //second line
                    case 3:              SetDDRAMAddr(x + 0x93);  break;   //third  line
                    case 4:              SetDDRAMAddr(x + 0xD3);  break;   //forth  line
                    }
                    if (temp >= 100000)            {SENDNUM(temp/100000);}          //send 5th char
                    if (temp >= 10000)             {SENDNUM((temp%100000)/10000);}  //send 5th char
                    if (temp >= 1000)              {SENDNUM((temp%10000)/1000);}    //send 4th char
                    if (temp >= 100)               {SENDNUM((temp%1000)/100);}      //send 3th char
                    if (temp >= 10)                {SENDNUM((temp%100)/10);}        //send 2nd char
                    SENDNUM(temp%10);                                               //send 1st char
}
void LCD_OUT_Num10(uint8_t y,uint8_t x,uint16_t temp)       //send Number/10 (as 12.5) to lcd at position(x,y)
{
                    switch(y)
                    {
                    case 1:              SetDDRAMAddr(x + 0x7F);  break;   //first  line
                    case 2:              SetDDRAMAddr(x + 0xBF);  break;   //second line
                    case 3:              SetDDRAMAddr(x + 0x93);  break;   //third  line
                    case 4:              SetDDRAMAddr(x + 0xD3);  break;   //forth  line
                    }
                    if (temp >= 10000)             {SENDNUM(temp/10000);}           //send 5th char
                    if (temp >= 1000)              {SENDNUM((temp%10000)/1000);}    //send 4th char
                    if (temp >= 100)               {SENDNUM((temp%1000)/100);}      //send 3th char
                    if (temp >= 10)                {SENDNUM((temp%100)/10);}        //send 2nd char
                    else
                    {SENDNUM(0);}
                    SendChar('.',1);     //send point
                    SENDNUM(temp%10);
}

*/
void CLCD_voidSendFloatNumber(int32_t Copy_s32Number)
{
	CLCD_voidSendData('.');
	CLCD_voidSendNumber(Copy_s32Number);
}
