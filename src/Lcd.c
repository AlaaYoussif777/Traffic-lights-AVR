#include<std_types.h>
#include <Macros.h>
#include <Dio.h>
#include <cpu_freq.h>
#include <stdarg.h>
#include <Lcd.h>
#include <Lcd_cfg.h>
#include <Utils.h>

static void Lcd_InitPins(void);
static void Lcd_SendCommand(u8 command);
static void Lcd_SendData(u8 data);

void Lcd_Init(void)
{
    Lcd_InitPins();
    _delay_ms(30);
    Lcd_DisplayControlType display = {
        LCD_DISPLAY_STATUS,
        LCD_CURSOR_STATUS,
        LCD_LINE_STATUS,
        LCD_FONT_STATUS};
    Lcd_DisplayControl(display);
    Lcd_DisplayClear();
}

void Lcd_DisplayClear(void)
{
    Lcd_SendCommand(0b00000001);
}

void Lcd_DisplayCharacter(u8 character)
{
    Lcd_SendData(character);
}

void Lcd_DisplayString(const u8 *str)
{
    while (*str != 0)
    {
        Lcd_SendData(*str);
        str++;
    }
}
void Lcd_SetCursorPosition(u8 row, u8 col)
{
    u8 address = col + (0x40 * row);
    SET_BIT(address, 7);
    Lcd_SendCommand(address);
}

void Lcd_DisplayControl(Lcd_DisplayControlType display)
{
    /* Set Lines and Font */
    u8 command = 0b00110000;
    if (display.line_status == LCD_TWO_LINES)
    {
        SET_BIT(command, 3);
    }
    if (display.font_status == LCD_FONT_5X10)
    {
        SET_BIT(command, 2);
    }
    Lcd_SendCommand(command);

    /* Set Display and Cursor */
    if (display.display_status == LCD_DISPLAY_OFF)
    {
        Lcd_SendCommand(0b00001000);
    }
    else if (display.display_status == LCD_DISPLAY_ON)
    {
        switch (display.cursor_status)
        {
        case LCD_CURSOR_OFF:
            Lcd_SendCommand(0b00001100);
            break;
        case LCD_CURSOR_ON_BLINK:
            Lcd_SendCommand(0b00001111);
            break;
        case LCD_CURSOR_ON_NOT_BLINK:
            Lcd_SendCommand(0b00001110);
            break;
        default:
            break;
        }
    }
}

void Lcd_DisplayNumber(s32 num)
{
    u32 reversed = 0;
    u8 counter = 0;
    if (num < 0)
    {
        Lcd_SendData('-');
        num *= -1;
    }
    do
    {
        reversed = reversed * 10 + (num % 10);
        num = num / 10;
        counter++;
    } while (num != 0);
    do
    {
        Lcd_SendData(reversed % 10 + '0');
        reversed = reversed / 10;
        counter--;
    } while (counter != 0);
}

void Lcd_DisplayShiftLeft(u8 counter)
{
    u8 i;
    for (i=0; i<counter; i++) {
        Lcd_SendCommand(0b00011000);
    }
}

void Lcd_DisplayShiftRight(u8 counter)
{
    u8 i;
    for (i=0; i<counter; i++) {
        Lcd_SendCommand(0b00011100);
    }
}

void Lcd_ReturnHome(void)
{
    Lcd_SendCommand(0b00000010);
}

static void Lcd_SendCommand(u8 command)
{
    Dio_SetPinLevel(LCD_PIN_RS, STD_LOW);
    Dio_SetPinLevel(LCD_PIN_RW, STD_LOW);
    Dio_SetPinLevel(LCD_PIN_D0, GET_BIT(command, 0));
    Dio_SetPinLevel(LCD_PIN_D1, GET_BIT(command, 1));
    Dio_SetPinLevel(LCD_PIN_D2, GET_BIT(command, 2));
    Dio_SetPinLevel(LCD_PIN_D3, GET_BIT(command, 3));
    Dio_SetPinLevel(LCD_PIN_D4, GET_BIT(command, 4));
    Dio_SetPinLevel(LCD_PIN_D5, GET_BIT(command, 5));
    Dio_SetPinLevel(LCD_PIN_D6, GET_BIT(command, 6));
    Dio_SetPinLevel(LCD_PIN_D7, GET_BIT(command, 7));
    Dio_SetPinLevel(LCD_PIN_EN, STD_HIGH);
    _delay_ms(2);
    Dio_SetPinLevel(LCD_PIN_EN, STD_LOW);
}

static void Lcd_SendData(u8 data)
{
    Dio_SetPinLevel(LCD_PIN_RS, STD_HIGH);
    Dio_SetPinLevel(LCD_PIN_RW, STD_LOW);
    Dio_SetPinLevel(LCD_PIN_D0, GET_BIT(data, 0));
    Dio_SetPinLevel(LCD_PIN_D1, GET_BIT(data, 1));
    Dio_SetPinLevel(LCD_PIN_D2, GET_BIT(data, 2));
    Dio_SetPinLevel(LCD_PIN_D3, GET_BIT(data, 3));
    Dio_SetPinLevel(LCD_PIN_D4, GET_BIT(data, 4));
    Dio_SetPinLevel(LCD_PIN_D5, GET_BIT(data, 5));
    Dio_SetPinLevel(LCD_PIN_D6, GET_BIT(data, 6));
    Dio_SetPinLevel(LCD_PIN_D7, GET_BIT(data, 7));
    Dio_SetPinLevel(LCD_PIN_EN, STD_HIGH);
    _delay_ms(2);
    Dio_SetPinLevel(LCD_PIN_EN, STD_LOW);
}

static void Lcd_InitPins(void)
{
    Dio_SetPinMode(LCD_PIN_RS, DIO_PIN_OUTPUT);
    Dio_SetPinMode(LCD_PIN_RW, DIO_PIN_OUTPUT);
    Dio_SetPinMode(LCD_PIN_EN, DIO_PIN_OUTPUT);
    Dio_SetPinMode(LCD_PIN_D0, DIO_PIN_OUTPUT);
    Dio_SetPinMode(LCD_PIN_D1, DIO_PIN_OUTPUT);
    Dio_SetPinMode(LCD_PIN_D2, DIO_PIN_OUTPUT);
    Dio_SetPinMode(LCD_PIN_D3, DIO_PIN_OUTPUT);
    Dio_SetPinMode(LCD_PIN_D4, DIO_PIN_OUTPUT);
    Dio_SetPinMode(LCD_PIN_D5, DIO_PIN_OUTPUT);
    Dio_SetPinMode(LCD_PIN_D6, DIO_PIN_OUTPUT);
    Dio_SetPinMode(LCD_PIN_D7, DIO_PIN_OUTPUT);
}

void Lcd_Print(const u8* str, ...) {
    u32 i = 0;
    va_list valist;
    va_start(valist, str);
    while (str[i] != '\0')
    {
        if (str[i] == '%') {
            u8 arr[33];
            i++;
            switch (str[i])
            {
            case 'c':
                Lcd_SendData((u8)va_arg(valist, int));
                break;
            case 'd':
                Lcd_DisplayNumber(va_arg(valist, int));
                break;
            case 'x':
                Utils_NumberToHex(va_arg(valist, int), arr);
                Lcd_DisplayString(arr);
                break;
            case 'b':
                Utils_NumberToBin(va_arg(valist, int), arr);
                Lcd_DisplayString(arr);
                break;
            default:
                if (str[i] == '\0') {
                    i--;
                }
                else {
                    Lcd_SendData((u8)va_arg(valist, int));
                }
                break;
            }
        }
        else {
            Lcd_SendData((u8)va_arg(valist, int));
        }
        i++;
    }
    va_end(valist);
}
