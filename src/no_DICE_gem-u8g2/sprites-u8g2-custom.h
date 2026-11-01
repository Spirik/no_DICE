/*
  Custom icons and sprites for u8g2 version of no_DICE.

  For documentation visit:
  https://github.com/Spirik/no_DICE

  Copyright (c) 2026 Alexander 'Spirik' Spiridonov

  This file is part of no_DICE program.

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 3 of the License, or (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.
  
  You should have received a copy of the GNU Lesser General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

//====================== Sprites of the UI elements used to draw menu

#define arrowRight_width  6
#define arrowRight_height 8
static const unsigned char arrowRight_bits_c [] U8X8_PROGMEM = {
  0x00, 0x02, 0x04, 0x08, 0x04, 0x02, 0x00, 0x00
};

#define arrowLeft_width  6
#define arrowLeft_height 8
static const unsigned char arrowLeft_bits_c [] U8X8_PROGMEM = {
  0x00, 0x04, 0x02, 0x01, 0x02, 0x04, 0x00, 0x00
};

#define arrowBtn_width  6
#define arrowBtn_height 8
static const unsigned char arrowBtn_bits_c [] U8X8_PROGMEM = {
  0xc0, 0xc3, 0xc5, 0xc9, 0xc5, 0xc3, 0xc0, 0xc0
};

#define checkboxUnchecked_width  12
#define checkboxUnchecked_height 8
static const unsigned char checkboxUnchecked_bits_c [] U8X8_PROGMEM = {
  0xFE, 0x07, 0x03, 0x0C, 0x19, 0x08, 0x25, 0x08, 0x19, 0x08, 0x03, 0x0C, 
  0xFE, 0x07, 0x00, 0x00
};

#define checkboxChecked_width  12
#define checkboxChecked_height 8
static const unsigned char checkboxChecked_bits_c [] U8X8_PROGMEM = {
  0xFE, 0x07, 0x03, 0x0C, 0x81, 0x09, 0xC1, 0x0B, 0x81, 0x09, 0x03, 0x0C, 
  0xFE, 0x07, 0x00, 0x00
};

#define selectArrows_width  6
#define selectArrows_height 8
static const unsigned char selectArrows_bits_c [] U8X8_PROGMEM = {
  0x00, 0x04, 0x0A, 0x00, 0x0A, 0x04, 0x00, 0x00
};

const GEMSprite arrowRightC = {arrowRight_width, arrowRight_height, arrowRight_bits_c};

const GEMSprite arrowLeftC = {arrowLeft_width, arrowLeft_height, arrowLeft_bits_c};

const GEMSprite arrowBtnC = {arrowBtn_width, arrowBtn_height, arrowBtn_bits_c};

const GEMSprite checkboxUncheckedC = {checkboxUnchecked_width, checkboxUnchecked_height, checkboxUnchecked_bits_c};

const GEMSprite checkboxCheckedC = {checkboxChecked_width, checkboxChecked_height, checkboxChecked_bits_c};

const GEMSprite selectArrowsC = {selectArrows_width, selectArrows_height, selectArrows_bits_c};

const GEMSprite* spritesCustom[] = {
  &arrowRightC,         // GEM_ICON_ARROW_RIGHT
  &arrowLeftC,          // GEM_ICON_ARROW_LEFT
  &arrowBtnC,           // GEM_ICON_ARROW_BTN
  &checkboxUncheckedC,  // GEM_ICON_CHECKBOX_UNCHECKED
  &checkboxCheckedC,    // GEM_ICON_CHECKBOX_CHECKED
  &selectArrowsC,       // GEM_ICON_SELECT_ARROWS
  &selectArrowsC        // GEM_ICON_SPINNER_ARROWS
};

//====================== Sprites of a custom button icons used in drawCustomIcon() callback

#define arrowBtn_dice_width 6
#define arrowBtn_dice_height 8
static const unsigned char arrowBtn_dice_bits [] U8X8_PROGMEM = {
  // 0x00, 0x00, 0x1E, 0x21, 0x2D, 0x2D, 0x21, 0x1E //v1
  // 0x00, 0x00, 0x12, 0x00, 0x12, 0x00, 0x12, 0x00 //v3 !
  // 0x00, 0x00, 0x11, 0x00, 0x04, 0x00, 0x11, 0x00 //v4
  0x00, 0x1F, 0x3F, 0x23, 0x23, 0x23, 0x3E, 0x00 //v9 !!!
  // 0x00, 0x1F, 0x3F, 0x23, 0x2B, 0x23, 0x3E, 0x00 //v10
  // 0x00, 0x00, 0x1E, 0x3E, 0x26, 0x26, 0x3C, 0x00 //v11 !!
};

GEMSprite arrowBtn_dice = {arrowBtn_dice_width, arrowBtn_dice_height, arrowBtn_dice_bits};

#define arrowBtn_coin_width 6
#define arrowBtn_coin_height 8
static const unsigned char arrowBtn_coin_bits [] U8X8_PROGMEM = {
  // 0x04, 0x0A, 0x12, 0x12, 0x12, 0x14, 0x08, 0x00 //v1
  0x00, 0x04, 0x0A, 0x11, 0x11, 0x11, 0x0A, 0x04 //v2 !!!
  // 0x04, 0x0E, 0x1B, 0x11, 0x11, 0x1B, 0x0E, 0x04 //v3
  // 0x00, 0x08, 0x14, 0x22, 0x22, 0x14, 0x08, 0x00 //v4 !!
};

GEMSprite arrowBtn_coin = {arrowBtn_coin_width, arrowBtn_coin_height, arrowBtn_coin_bits};

#define arrowBtn_life_width 6
#define arrowBtn_life_height 8
static const unsigned char arrowBtn_life_bits [] U8X8_PROGMEM = {
  // 0x00, 0x00, 0x14, 0x3E, 0x3E, 0x1C, 0x08, 0x00 //v1 !!
  // 0x00, 0x33, 0x3F, 0x3F, 0x3F, 0x1E, 0x0C, 0x00 //v2
  // 0x00, 0x21, 0x33, 0x3F, 0x3F, 0x1E, 0x0C, 0x00 //v3
  // 0x00, 0x12, 0x3F, 0x3F, 0x3F, 0x1E, 0x0C, 0x00 //v4
  // 0x00, 0x12, 0x3F, 0x21, 0x33, 0x1E, 0x0C, 0x00 //v5
  // 0x00, 0x0A, 0x1F, 0x1F, 0x1F, 0x0E, 0x04, 0x00 //v6 !!!
  0x00, 0x0A, 0x1F, 0x11, 0x1B, 0x0E, 0x04, 0x00 //v7 !!!
};

GEMSprite arrowBtn_life = {arrowBtn_life_width, arrowBtn_life_height, arrowBtn_life_bits};

//====================== Custom splash screen logo

#define logo_width  42
#define logo_height 8
static const unsigned char logo_bits [] U8X8_PROGMEM = {
  0x00, 0x00, 0x3C, 0x8E, 0xF3, 0x01, 0x8F, 0x03, 0x40, 0x40, 0x00, 0x00,
  0x51, 0x04, 0x44, 0x44, 0x00, 0x00, 0x51, 0x04, 0x44, 0x44, 0xF0, 0x00,
  0x51, 0x04, 0x44, 0x44, 0x00, 0x00, 0x51, 0x04, 0x44, 0x44, 0x00, 0x00,
  0x91, 0xF3, 0x3D, 0x8E, 0xF3, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const GEMSprite logo = {logo_width, logo_height, logo_bits};
