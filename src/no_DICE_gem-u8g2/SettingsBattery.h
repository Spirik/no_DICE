/*
  Battery settings of no_DICE, configurable through corresponding menu item(s).

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

#ifndef HEADER_SETTINGS_BATTERY
#define HEADER_SETTINGS_BATTERY

#include <Arduino.h>

struct SettingsBattery {
  byte enableMonitoring;  // Using byte instead of bool for futureproofing (to support another kind of monitoring in the future);
                          // Values of 0 (off) and 1 (on) are available for now (for voltage divider measurement and calculations which requires some resistor soldering)
  bool showIcon;
  bool showNumber;
};

#endif