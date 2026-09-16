

#ifndef __Varible_HH
#define __Varible_HH
#include "main.h"
#include "stm32f0xx_hal.h"

//--------------------------------------------------------------------------------------
extern 			uint8_t		test_flag;
extern 			uint32_t  EqualizeTimePerSecond , BoostTimePerSecond  , DeltaTimePerSecond;
extern 			uint8_t		Flag_Send;
extern      uint8_t  	ScreenLockingState ;
extern      uint8_t 	TransitionPrivilge ; 
extern      uint8_t 	CnfrmdKey;
extern      uint8_t 	DtctdKey ;
extern      uint8_t 	LastDtctdKey;
extern      uint8_t 	DebnceTimer ;
extern      uint8_t 	KeyChngFlag ;
extern      uint8_t  	CountWelcome;
extern      uint8_t  	Event;
extern      uint8_t 	count_pass_utility;
extern      uint8_t   BatteryChargingStatus , RectifierBattery;
extern      uint8_t   key;
extern      uint8_t	  FlagRectifierRam[5];
extern      uint8_t   event_warning[15];                  // used to display an old warning in Event log
extern      uint32_t 	FirstTime;
extern      uint8_t 	Flag_EQ;
extern      uint8_t 	Num_Array;		
//extern      uint8_t 	ParametersInRAM[ParametersSizeInEEPROM];
extern      uint16_t  setup_battery1[11];
extern      uint16_t  offset_Array[4];
extern 			uint8_t 	Serial_Nummber[20];
extern			uint32_t	Flags1,Flags2;  
extern 			uint32_t  Freq;
extern			uint8_t 	Data_again[20];
extern			uint8_t 	Again_Send;
extern			uint8_t		Data_again_Len;
extern			uint32_t 	tmr_again;  
extern			uint8_t 	last_key;
extern 			uint8_t 	Flag_Event_Log;
extern			uint32_t	timer_reset;

//-------------------------------------NEW

void 			Setting_TIMER16(uint16_t PRESCALER , uint16_t AUTORELOAD);
void Start_Time(uint8_t  num);
uint8_t		Flag_Time(uint8_t  num,uint32_t Max_Time); 

extern			uint16_t 	Counter_Buzzer,Flag_Buzzer,Level_Counter_Buzzer;
extern			uint16_t	Num_Item ;
extern			uint8_t		Access_level[3];
extern			uint8_t		index_Access_level;
extern 			uint8_t		Flag_Lock;
extern			uint8_t		Lock_Data;
extern      uint8_t  	Page_Setup;
extern      uint8_t   RlySetup[NoOfFaultRLYs];
extern 			uint8_t 	display_IN;
extern      uint8_t  	Start_Cursor,Pos_Cursor,Select_Cursor;



extern	uint8_t 		aString[15];
extern	uint8_t 		arr[300];
extern	uint8_t 		arr2[300];
extern	uint32_t 		Frequency;
extern	uint8_t 		init_first;
extern	uint32_t 	  RMS[16];
extern  uint32_t 	  RMSx[16];
extern	uint32_t 	  RMS_Array[85];
extern	uint32_t 	  RMS_S[15];
extern	uint8_t 	  sign_I[5];
extern	uint32_t 		REG[40];
extern	uint32_t 		Setting_Parameter[80];
extern	uint32_t 		Setting_Parameter_Last[80]; 
extern	uint8_t 		BatteryStatus;
extern	uint8_t 		buffer[50];
extern	uint32_t 		Timer[10];
extern  uint8_t 		state,pre_state;
extern	uint32_t 		Flag_Switch , FlagArray , FlagArray_Event;
extern  uint8_t 		show;
extern  uint16_t    ALFA;
extern	uint32_t    CurrentTime,RemainedTime,CurrentTimeBoost;
extern  uint32_t 		DateTimeRecordArry[RecordSizeNBytes];
extern  uint32_t		DateTimeRecordArryE[RecordSizeNBytes];
extern	uint16_t 	 	CurrentEvent; 
extern	uint16_t 	  x1 , x2 , x3 , x4 , x5 , x6;
extern  uint8_t     state_setup,Pass,CalibCount,PassCount,PassChar,cs;	

extern	uint8_t		  Alarm;
extern  uint8_t  	  event;
extern  uint8_t  		display ;
extern  uint8_t  	  EventBrowsingFlag;
//--------------------------------------clock
extern			uint8_t     UserInterfaceState;
extern			uint16_t		Data_Clock[10];
extern 			uint8_t* 		WeekDays[7] ;
extern			uint8_t 		ClockMenuXpositions[ClockMenuEnd-ClockMenuBegin+1];  
extern			uint8_t 		ClockMenuYpositions[ClockMenuEnd-ClockMenuBegin+1];
extern			uint8_t 		UserInterfaceState;
extern			uint8_t 		ParameterUserTensTemporaryValue ;
extern			uint8_t 		ParameterUserUnitsTemporaryValue ;
extern			uint8_t 		menuParser ;//for transfer between parameters in the same menu
extern			uint8_t 		ParameterParser ;//for transfer between digits in the same parameter 

//-----------------Calibration
extern      uint8_t  	CalibX , CalibY;

extern uint16_t co;

extern uint16_t BtryTstProgress ;
extern uint16_t BtryTstResult ;
extern uint16_t BtryTstResultReason ;
extern uint8_t  BtryTstPreviousState ;

//--------------------------------------NEW
extern uint8_t 		ACK_Rec,Request_Send_Par;

extern uint8_t		ManualMode_Slave,ManualMode_Slave2;
extern uint32_t		ManualMode_Voltage;
extern uint32_t		Flot_Volt,	HiRt_Volt;
//------------------------------------------------------------------------

//------------------------------------------------------------------------

//---------------------------------------------------------
extern 		uint16_t 		ArrNewRect[20];
extern		uint32_t 		ArrNewRect32[20];
//---------------------------------------------------------

//----------------display_file
void display_Leds();
void displayRelaysInStup(uint8_t Rlystup );
void Display_LEDs_Test	(void);	
void display_sys();
void state_text(uint8_t s , uint8_t y ,uint8_t x);
void warn_text();
void Get_Serial_Number(uint8_t x ,uint8_t y);
void Function_Buzzer_Alarm();
void test_screen();
//----------------display_file


//----------------UART_Recieve
void UART_Rec();
void Comm_Data();
void Send_Text1(uint8_t *str);
void Send_Text2(uint8_t *str);
void Send_Num1(uint32_t count);
void Send_Num2(uint32_t count);					
uint32_t strTOint(uint8_t * ptr,uint8_t len);
void Send_Num_Without_Send(uint32_t count);
void Clear_arr_uart1();
void Clear_arr_uart2();
void Clear_arr_uart1_IT();
void Clear_arr_uart2_IT();
//----------------UART_Recieve

//----------------EEPROM
void init_EEPROM(); 
uint16_t EE_Init(void);
//----------------EEPROM

void state_machine();

//----------------SubFunctions
void Auto_Manual_Function(uint8_t New_key);
void ClockDateSetup(uint8_t Key);
void Password_Function(uint8_t key);
void ClockDateSetup_display();
void Auto_Manual_Function_display();
void Password_Function_display();
//----------------SubFunctions
void KeypadDetection(uint8_t key);
void Set_FlagMode(uint32_t bin);
void Clr_FlagMode(uint32_t bin);
uint8_t Get_FlagMode(uint32_t bin);
void Set_FlagArray(uint32_t bin);
void Clr_FlagArray(uint32_t bin);
uint8_t Get_FlagArray(uint32_t bin);
void Set_FunctionLck(uint32_t bin);
void Clr_FunctionLck(uint32_t bin);
uint8_t Get_FunctionLck(uint32_t bin);
void Set_Flag_Bin(uint32_t bin);
void Clr_Flag_Bin(uint32_t bin);
uint8_t Get_Flag(uint16_t bin);
void DELAY_US(uint32_t del);
void delay_ms_1(uint32_t del);
void Horn_Buzzer(uint16_t Del);
void init_System();
uint8_t PIN(uint8_t port, uint8_t pin);
uint8_t Check_Range_Save(uint16_t index);
void DCvltRatng_Edit_Function();
//----------------RECT_Sys.c


//----------------LCD
void InitLCD();
void ClearLCD();
void LCD_OUT_text(uint8_t y,uint8_t x,uint8_t * temp);
void LCD_OUT_text_L(uint8_t y,uint8_t x,uint8_t * temp,uint8_t len);
void LCD_OUT_Num100(uint8_t y,uint8_t x,uint16_t temp);
void LCD_OUT_Num10(uint8_t y,uint8_t x,uint16_t temp);
void LCD_OUT_Num(uint8_t y,uint8_t x,uint32_t temp);
void LCD_Char(uint8_t y,uint8_t x,uint16_t temp);
void LCD_DisplayOnOffControl(uint8_t y,uint8_t x,uint8_t DisplayStatus,uint8_t CurrsorStatus,uint8_t BlinkStatus);
void SendChar(uint8_t data,uint8_t cmd);
//----------------LCD

//----------------Switches
uint8_t Keypad_delay(uint8_t delay);
uint8_t Keypad();
//----------------Switches

//----------------RTC
void ds1307_init();
void ds1307_set_date_time();
void ds1307_get_date();
void ds1307_get_time();
void get_RTC_External();
//----------------RTC

//----------------EEPROM
void Write_External_EEPROM(uint16_t addr , uint16_t val);
void Write_External_EEPROM16(uint16_t addr , uint16_t val);
void Write_External_EEPROM_array(uint16_t addr , uint8_t *pr ,uint8_t NBytes);
uint16_t Read_External_EEPROM(uint16_t addr);
uint16_t Read_External_EEPROM16(uint16_t addr);
//----------------EEPROM

//----------------Setup_Parameters
void SetupFunction(uint8_t key);
void SetupFunction_display();
void Init_SetupFunction();
//----------------Setup_Parameters

//----------------EventLog
uint8_t Load_Eventlog(uint8_t Event);
uint8_t Load_Eventlog_AcessLvl(uint8_t Event);
void Get_last_EventLog();
void SaveNewEventLog();
void Clear_Event_Log();
void Function_Event_Log(uint8_t key);
void Function_Event_Log_display();
void Set_FlagArray_Event(uint32_t bin);
void Clr_FlagArray_Event(uint32_t bin);
uint8_t Get_FlagArray_Event(uint32_t bin);
void Update_EventLog();
uint16_t EE_WriteVariable(uint16_t VirtAddress, uint32_t Data);

void Moving_Average();
void PWM_Buzzer_ON(uint16_t count);
void PWM_Buzzer_OFF();
void init_calibration(uint8_t sel);
//----------------EEPROM
//----------------EEPROM
//----------------EEPROM
//----------------EEPROM
//----------------EEPROM
//----------------EEPROM
//-------------------------------------------------------------------------
#endif

void HornAlarmSilence(void);
void Control_Fault_Reset(void);

void Recieve_Ahmed();

void WriteBufText2(uint8_t *s,uint16_t len);
void WriteBufNum2(uint32_t num , uint8_t type);
void SendUartBuf2();
void SendUartBufX2();

void WriteBufText1(uint8_t *s,uint16_t len);
void WriteBufNum1(uint32_t num , uint8_t type);
void SendUartBuf1();
void SendUartBufX1();


void Send_Text_Show(uint8_t * str);
void Send_Text_len_Show(uint8_t * str,uint8_t len);
void Send_Num_Show(uint32_t num);

void Send_UART1_Order(uint8_t * str,uint16_t len);
void Send_UART2_Order(uint8_t * str,uint16_t len);
void Send_SettingParX(uint8_t type,uint16_t first_addr,uint16_t len);
void Send_SettingParShow(uint16_t first_addr,uint16_t len);

uint8_t Compare(uint8_t * s11 , uint8_t * s22);
void Set_FlagACK(uint32_t bin);
void Clr_FlagACK(uint32_t bin);
uint8_t Get_FlagACK(uint32_t bin);
void Check_UART_Communication();
void Periodic_Send_Data_To_Computer(uint8_t direct);
uint16_t TwoByte(uint8_t XH,uint8_t XL);
uint32_t ThreeByte(uint8_t XH,uint8_t XM,uint8_t XL);
uint32_t FourByte(uint8_t XLH,uint8_t XH,uint8_t XM,uint8_t XL);
void Send_Setting_Parameters();
void delay_ms(uint32_t ms);
void delay_us(uint32_t us);


void Setting_TIMERX(TIM_HandleTypeDef *htim,uint16_t PRESCALER , uint16_t AUTORELOAD);
void Send_OneSetPar(uint8_t index,uint8_t type);
void Send_FlagMode(uint16_t flg);
void Send_AutoManual(uint8_t sel);
void Send_FlagModePin(uint8_t pin,uint8_t sel);	
void Send_OperationSystem(uint8_t sel);
void Send_Request_SetPar_From_Rectifier(uint8_t count);
void Send_Special_Order(uint8_t order,uint8_t count);
void InitParameter_IF_Change();
void Send_Parameter_IF_Change();
void Func_AddrBit();
uint8_t Check_Continue(uint8_t num,uint16_t level);
