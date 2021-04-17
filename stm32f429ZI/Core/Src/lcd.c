/*
 ******************************************************************************
 * @file    lcd.c
 * @author  Sofiane AOUCI
 * @date 	Oct 19, 2020
 * @brief   This file contains all the functions prototypes and declarations
 * 			for the RA8875 Display Controller
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


#include "lcd.h"

#define LCD_CMD *((volatile uint16_t *) 0x60100000)  // ST datasheet RM0090 page 1608 : 0x6 [bank1] and 0x0100000 [HDDR<<1] corresponding to the A19 bit
#define LCD_DATA *((volatile uint16_t *) 0x60000000)

void writeReg(uint16_t CMD, uint16_t DATA){
	LCD_CMD = CMD;
	LCD_DATA = DATA;
}

void writeData(uint16_t DATA){
	LCD_DATA = DATA;
}

void writeCMD(uint16_t CMD){
	LCD_CMD = CMD;
}

uint16_t readReg(uint16_t CMD){
	LCD_CMD = CMD;
	return LCD_DATA;
}

void LCD_init(){

	 /* Hardware Reset */
	  HAL_GPIO_WritePin(FMC_RST_GPIO_Port, FMC_RST_Pin, 1);
	  HAL_Delay(100);
	  HAL_GPIO_WritePin(FMC_RST_GPIO_Port, FMC_RST_Pin, 0);
	  HAL_Delay(100);
	  HAL_GPIO_WritePin(FMC_RST_GPIO_Port, FMC_RST_Pin, 1);
	  HAL_Delay(100);

	 /* PLL config */
	  writeReg(RA8875_PLLC1, RA8875_PLLC1_PLLDIV1 + 11);
	  HAL_Delay(1);
	  writeReg(RA8875_PLLC2, RA8875_PLLC2_DIV4);
	  HAL_Delay(1);

	 /* configure MCU interface and color format : 16bit mcu + rgb565 */
	  writeReg(RA8875_SYSR, RA8875_SYSR_16BPP | RA8875_SYSR_MCU16);

	  /* Timing values */
	  uint8_t pixclk;
	  uint8_t hsync_start;
	  uint8_t hsync_pw;
	  uint8_t hsync_finetune;
	  uint8_t hsync_nondisp;
	  uint8_t vsync_pw;
	  uint16_t vsync_nondisp;
	  uint16_t vsync_start;

	  /* Set the correct values for the display being used */
	  pixclk          = RA8875_PCSR_PDATL | RA8875_PCSR_2CLK;
	  hsync_nondisp   = 26;
	  hsync_start     = 32;
	  hsync_pw        = 96;
	  hsync_finetune  = 0;
	  vsync_nondisp   = 32;
	  vsync_start     = 23;
	  vsync_pw        = 2;

	  writeReg(RA8875_PCSR, pixclk);
	  HAL_Delay(1);

	  /* Horizontal settings registers */
	  writeReg(RA8875_HDWR, (_tft_width / 8) - 1);                          // H width: (HDWR + 1) * 8 = 480
	  writeReg(RA8875_HNDFTR, RA8875_HNDFTR_DE_HIGH + hsync_finetune);
	  writeReg(RA8875_HNDR, (hsync_nondisp - hsync_finetune - 2) / 8);  // H non-display: HNDR * 8 + HNDFTR + 2 = 10
	  writeReg(RA8875_HSTR, hsync_start / 8 - 1);                       // Hsync start: (HSTR + 1)*8
	  writeReg(RA8875_HPWR, RA8875_HPWR_LOW + (hsync_pw / 8 - 1));      // HSync pulse width = (HPWR+1) * 8

	  /* Vertical settings registers */
	  writeReg(RA8875_VDHR0, (uint16_t)(_tft_height - 1) & 0xFF);
	  writeReg(RA8875_VDHR1, (uint16_t)(_tft_height - 1) >> 8);
	  writeReg(RA8875_VNDR0, vsync_nondisp - 1);                        // V non-display period = VNDR + 1
	  writeReg(RA8875_VNDR1, vsync_nondisp >> 8);
	  writeReg(RA8875_VSTR0, vsync_start - 1);                          // Vsync start position = VSTR + 1
	  writeReg(RA8875_VSTR1, vsync_start >> 8);
	  writeReg(RA8875_VPWR, RA8875_VPWR_LOW + vsync_pw - 1);            // Vsync pulse width = VPWR + 1

	  /* Set active window X */
	  writeReg(RA8875_HSAW0, 0);                                        // horizontal start point
	  writeReg(RA8875_HSAW1, 0);
	  writeReg(RA8875_HEAW0, (uint16_t)(_tft_width - 1) & 0xFF);            // horizontal end point
	  writeReg(RA8875_HEAW1, (uint16_t)(_tft_width - 1) >> 8);

	  /* Set active window Y */
	  writeReg(RA8875_VSAW0, 0);                                        // vertical start point
	  writeReg(RA8875_VSAW1, 0);
	  writeReg(RA8875_VEAW0, (uint16_t)(_tft_height - 1) & 0xFF);           // horizontal end point
	  writeReg(RA8875_VEAW1, (uint16_t)(_tft_height - 1) >> 8);


	  /* Interrupts config */
	  writeReg(RA8875_INTC1, 3);

	  /* Clear the entire window */
	  /* Set background Color */
	  writeCMD(0x60);
	  writeData((0xffff & 0xf800) >> 11);
	  writeCMD(0x61);
	  writeData((0xffff & 0x07e0) >> 5);
	  writeCMD(0x62);
	  writeData((0xffff & 0x001f));
	  writeReg(RA8875_MCLR, RA8875_MCLR_START | RA8875_MCLR_FULL);
	  HAL_Delay(500);

	  /* power ON */
	  writeReg(RA8875_PWRR, RA8875_PWRR_NORMAL | RA8875_PWRR_DISPON);
	  /* set graphics mode */
	  LCD_graphicsMode();

}

void LCD_waitPoll(uint8_t regname, uint8_t waitflag) {
  /* Wait for the command to finish */
	uint8_t temp = 0;
	do {
    temp = readReg(regname);
  } while(temp & waitflag);
}

uint8_t LCD_isBusy(uint8_t regname, uint8_t waitflag) {
	uint8_t temp = readReg(regname);
    return (temp & waitflag);
}

void LCD_backlight(uint8_t on){
	writeReg(RA8875_P1CR, (!on) << 6);
	//writeReg(RA8875_P2CR, on << 6);
}

void LCD_graphicsMode(void) {

  uint16_t temp = readReg(RA8875_MWCR0);
  temp &= ~RA8875_MWCR0_TXTMODE; // bit #7
  writeData(temp);
}

void LCD_ClearDisplay(uint16_t color){
	LCD_setBackgroundColor(color);
	writeReg(RA8875_MCLR, RA8875_MCLR_START | RA8875_MCLR_FULL);
	LCD_waitPoll(RA8875_MCLR, RA8875_MCLR_READSTATUS);
}

void LCD_drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color, uint8_t filled) {
	// set X1
	writeReg(0x91, x & 0xFF);
	writeReg(0x92, x >> 8);
	// set Y1
	writeReg(0x93, y & 0xFF);
	writeReg(0x94, y >> 8);
	// set X2
	writeReg(0x95, (x + w) & 0xFF);
	writeReg(0x96, (x + w) >> 8);
	// set Y2
	writeReg(0x97, (y + h) & 0xFF);
	writeReg(0x98, (y + h) >> 8);

	LCD_setForgroundColor(color);

	// Start drawing
	writeReg(RA8875_DCR, RA8875_DCR_DRAWSQUARE | (filled << 5) | RA8875_DCR_LINESQUTRI_START);

	  /* Wait for the command to finish */
	  LCD_waitPoll(RA8875_DCR, RA8875_DCR_LINESQUTRI_STATUS);
}

void LCD_setBackgroundColor(uint16_t color){
	writeReg(0x60, (color & 0xf800) >> 11);
	writeReg(0x61, (color & 0x07e0) >> 5);
	writeReg(0x62, (color & 0x001f));
}

void LCD_setForgroundColor(uint16_t color){
	writeReg(0x63, (color & 0xf800) >> 11);
	writeReg(0x64, (color & 0x07e0) >> 5);
	writeReg(0x65, (color & 0x001f));
}

void LCD_displayOn(uint8_t on) {
	  if (on)
	    writeReg(RA8875_PWRR, RA8875_PWRR_NORMAL | RA8875_PWRR_DISPON);
	  else
	    writeReg(RA8875_PWRR, RA8875_PWRR_NORMAL | RA8875_PWRR_DISPOFF);
}


void LCD_BTE(int16_t xS, int16_t yS, int16_t xD, int16_t yD, int16_t w, int16_t h, uint16_t backColor, uint16_t forColor){
	// set XS
	/*writeReg(0x54, xS & 0xFF);
	writeReg(0x55, xS >> 8);
	// set YS
	writeReg(0x56, yS & 0xFF);
	writeReg(0x57, yS >> 8);*/

	//writeReg(RA8875_INTC1, 0x03);

	// set XD
	writeReg(0x58, xD & 0xFF);
	writeReg(0x59, xD >> 8);
	// set YD + layer (0)
	writeReg(0x5a, yD & 0xFF);
	writeReg(0x5b, (yD >> 8) &0x03);

	//set Width
	writeReg(0x5c, w & 0xFF);
	writeReg(0x5d, w >> 8);
	//set height
	writeReg(0x5e, h & 0xFF);
	writeReg(0x5f, h >> 8);

	//LCD_setBackgroundColor(backColor);
	//LCD_setForgroundColor(forColor);

	writeReg(0x51, 0x00);
	writeReg(0x50, 0x80);
	//while(!LCD_waitPoll(0x50, 0x80));


}

/* DRAWING ---------------------------------------------------------*/

void LCD_DrawPixels(uint16_t *p, uint32_t num, int16_t x, int16_t y) {

	writeReg(RA8875_CURH0, x);
	writeReg(RA8875_CURH1, x >> 8);
	writeReg(RA8875_CURV0, y);
	writeReg(RA8875_CURV1, y >> 8);

	writeCMD(RA8875_MRWC);
	while(num--) {
		writeData(*p++);
	}
}

void LCD_drawBuffer(uint16_t *p, int16_t x, int16_t y, uint32_t w, uint32_t h){
	int i;
	LCD_BTE(0, 0, x, y, w, h, 0, 0);
	for(i = 0; i < h; i++){
		  LCD_DrawPixels(p + w * i, w, x, y + i);
	  }
}

/* TP ---------------------------------------------------------*/

/*void LCD_TP_init(){
	writeReg(RA8875_TPCR0, RA8875_TPCR0_ENABLE | RA8875_TPCR0_WAIT_4096CLK | RA8875_TPCR0_WAKEENABLE | RA8875_TPCR0_ADCCLK_DIV16);
	writeReg(RA8875_TPCR1, RA8875_TPCR1_DEBOUNCE | RA8875_TPCR1_VREFINT | RA8875_TPCR1_AUTO);
	writeReg(RA8875_INTC1, RA8875_INTC1_TP);
}

uint8_t LCD_TP_isTouched(){
	return !((readReg(RA8875_TPXYL) >> 7) & 0x01);
}*/

uint8_t LCD_resetIRQFlags(){
	uint8_t in = readReg(RA8875_INTC2);
	if(in & 0x03) {
		writeReg(RA8875_INTC2, in);
		return 0;
	}
	return 1;
}

