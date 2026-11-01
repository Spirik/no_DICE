/*
  Sprites for battery management of no_DICE.

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

//====================== Custom battery icons

#define battery_width  10
#define battery_height 6
static const unsigned char battery0_bits [] U8X8_PROGMEM = {
  0xFF, 0x01, 0x01, 0x01, 0x01, 0x03, 0x01, 0x03, 0x01, 0x01, 0xFF, 0x01
};
static const unsigned char battery1_bits [] U8X8_PROGMEM = {
  0xFF, 0x01, 0x01, 0x01, 0x05, 0x03, 0x05, 0x03, 0x01, 0x01, 0xFF, 0x01
};
static const unsigned char battery2_bits [] U8X8_PROGMEM = {
  0xFF, 0x01, 0x01, 0x01, 0x15, 0x03, 0x15, 0x03, 0x01, 0x01, 0xFF, 0x01
};
static const unsigned char battery3_bits [] U8X8_PROGMEM = {
  0xFF, 0x01, 0x01, 0x01, 0x55, 0x03, 0x55, 0x03, 0x01, 0x01, 0xFF, 0x01
};

const GEMSprite batteryIcon0 = {battery_width, battery_height, battery0_bits};
const GEMSprite batteryIcon1 = {battery_width, battery_height, battery1_bits};
const GEMSprite batteryIcon2 = {battery_width, battery_height, battery2_bits};
const GEMSprite batteryIcon3 = {battery_width, battery_height, battery3_bits};

const GEMSprite batteryIcons[] = {
  batteryIcon0,
  batteryIcon1,
  batteryIcon2,
  batteryIcon3,
};
