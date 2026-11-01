/*
  Some of the constants of no_DICE.

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

#define NO_DICE_VER "1.0.0"

//====================== STORAGE CONFIGURATION

// Status that is saved to internal memory (EEPROM) to check if settings are present
#define INIT_STATUS "NO_DICE"

// Current version of settings scheme for compatibility with later revisions
#define SETTINGS_VER "001"

// EEPROM address where data struct will be stored
#define EEPROM_ADDR 0
// EEPROM size (required by boards that emulate EEPROM in flash storage)
#define EEPROM_SIZE 256

//====================== PINS

#define RANDOM_SEED_PIN A3  // Shared with Battery monitoring (that's probably not ideal, but A0, A1, A2 pins are already occupied with encoder input by default on XIAO board with Grove Shield);
                            // also see comments in seedRnd() function of main sketch .ino file

//====================== BATTERY

#define BATTERY_MAX 3900  // Maximum battery voltage (battery voltage when fully charged), mV
#define BATTERY_MIN 3200  // Minimum battery voltage (battery voltage just before system shuts itself down), mV
#define BATTERY_VREF 3300 // Reference voltage of the board (3.3v for RP20240), mV
#define BATTERY_DIVIDER_RATIO 2 // Ratio of resistor voltage divider
#define BATTERY_MONITORING_INTERVAL 10000 // How often to measure battery level, ms
#define BATTERY_WARNING_PERC 10  // Percentage of alarmingly low battery level, %

//====================== EASINGS

#define NO_DICE_EASING_LINEAR 0
#define NO_DICE_EASING_EASE_IN_CUBIC 1
#define NO_DICE_EASING_EASE_OUT_CUBIC 2
#define NO_DICE_EASING_EASE_IN_OUT_CUBIC 3

//====================== LANGUAGE

#define NO_DICE_LANG_EN 0
#define NO_DICE_LANG_RU 1
#define NO_DICE_LANG_DE 2
#define NO_DICE_LANG_FR 3

//====================== FONTS

#define NO_DICE_FONT_BIG u8g2_font_6x12_te
#define NO_DICE_FONT_SMALL u8g2_font_tom_thumb_4x6_te
