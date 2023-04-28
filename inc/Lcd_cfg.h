/*
 * Lcd_cfg.h
 *
 *  Created on: Sep 2, 2022
 *      Author: HFCS
 */

#ifndef INC_LCD_CFG_H_
#define INC_LCD_CFG_H_


/************* LCD Control Pins *************/
#define LCD_PIN_RS DIO_PORTD,DIO_PIN0
#define LCD_PIN_RW DIO_PORTD,DIO_PIN1
#define LCD_PIN_EN DIO_PORTD,DIO_PIN2

/************* LCD Data Pins *************/
#define LCD_PIN_D0 DIO_PORTB,DIO_PIN0
#define LCD_PIN_D1 DIO_PORTB,DIO_PIN1
#define LCD_PIN_D2 DIO_PORTB,DIO_PIN2
#define LCD_PIN_D3 DIO_PORTD,DIO_PIN3
#define LCD_PIN_D4 DIO_PORTB,DIO_PIN4
#define LCD_PIN_D5 DIO_PORTB,DIO_PIN5
#define LCD_PIN_D6 DIO_PORTB,DIO_PIN6
#define LCD_PIN_D7 DIO_PORTB,DIO_PIN7

/************* LCD Display Control *************/
#define LCD_DISPLAY_STATUS  LCD_DISPLAY_ON
#define LCD_CURSOR_STATUS   LCD_CURSOR_OFF
#define LCD_LINE_STATUS     LCD_TWO_LINES
#define LCD_FONT_STATUS     LCD_FONT_5X7
#endif /* INC_LCD_CFG_H_ */

