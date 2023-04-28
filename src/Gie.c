/*
 * Gie.c
 *
 *  Created on: Sep 10, 2022
 *      Author: HFCS
 */
#include <macros.h>
#include <registers.h>
#include <std_types.h>
#include <Gie.h>

#define SREG_I 7

void Gie_Enable(void){
	SET_BIT(SREG,SREG_I);
}

void Gie_Disable(void){
	CLR_BIT(SREG,SREG_I);
}
