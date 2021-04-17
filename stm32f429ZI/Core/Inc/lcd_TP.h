/*
 ******************************************************************************
 * @file    lcd_TP.h
 * @author  Sofiane AOUCI
 * @date 	Oct 25, 2020
 * @brief   This file contains all the functions prototypes and declarations
 * 			for the GT811 Touch Panel Controller
 ******************************************************************************
 *****************	Copyright (C) Toulouse III University	*******************
 *
 *   This program is a free software: you can redistribute it and/or modify it
 *   under the terms of the GNU General Public License as published by the Free
 *   Software Foundation, either version 3 of the License, or any later version.
 *
 *   This program is distributed in the hope that it will be useful, but WITHOUT
 *   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 *   more details.
 *                   <http://www.gnu.org/licenses/>.
 *
 *******************************************************************************
*/



#ifndef INC_LCD_TP_H_
#define INC_LCD_TP_H_


#include "main.h"

#define LCD_TP_ADDRESS 0xBA
#define LCD_TP_REGISTERS_CONFIGURATION 0x6A2
#define LCD_TP_REGISTERS_ALL_DATA 0x721
#define LCD_TP_REGISTERS_STATUS 0x721
#define LCD_TP_REGISTERS_DATA_T1 0x723


uint8_t LCD_TP_init(void);
uint8_t LCD_TP_getStatus(void);
void LCD_TP_getT1(uint16_t* X, uint16_t* Y);

#endif /* INC_LCD_TP_H_ */
