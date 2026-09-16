/*********************************************************/
/* Author      : MOHAMED AHMED                           */
/* Date        : 29 JUL 2022                             */
/* Version     : V01                                     */
/*********************************************************/
#ifndef BIT_MATH_H
#define BIT_MATH_H

//#define SET_BIT(VAR,BIT)          VAR |=  (1 << (BIT))
#define CLR_BIT(VAR,BIT)          VAR &= ~(1 << (BIT)) 
#define GET_BIT(VAR,BIT)          ((VAR>>BIT)&1)
#define TOG_BIT(VAR,BIT)          VAR ^=  (1 << (BIT))

#define bit_clear(sec,bit)	sec &= (~(1<<bit))
#define bit_set(sec,bit)	  sec |= (1<<bit)
#define bit_test(reg,bit)  ((reg>>bit) & 1)


#endif 
