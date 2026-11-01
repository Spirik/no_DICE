/*
  Presets of no_DICE (default settings).

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

#ifndef HEADER_PRESET
#define HEADER_PRESET

#include "Settings.h"

//====================== GENERAL SETTINGS

SettingsGeneral settingsGeneral = {
  .menuPointer = GEM_POINTER_ROW,
  .language = NO_DICE_LANG_EN
};

//====================== BATTERY SETTINGS

SettingsBattery settingsBattery = {
  .enableMonitoring = 0,
  .showIcon = true,
  .showNumber = false
};

//====================== DICE SETTINGS

SettingsDice settingsDice = {
  .sides = 6
};

//====================== LIFE SETTINGS

SettingsLife settingsLife = {
  .startingValue = 20
};

//====================== SETTINGS

Settings settings = {
  .settingsGeneral = settingsGeneral,
  .settingsBattery = settingsBattery,
  .settingsDice = settingsDice,
  .settingsLife = settingsLife
};

#endif
