/*
  Class for handling battery monitoring of no_DICE.

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

#ifndef HEADER_BATTERY
#define HEADER_BATTERY

#include <U8g2lib.h>
#include <GEM_u8g2.h>
#include "constants.h"
#include "sprites-battery.h"
#include "SettingsBattery.h"

class Battery {
  public:
    Battery(U8G2& u8g2_, GEM_u8g2& menu_, SettingsBattery& settings_, byte batteryPin_, int* pBatteryVolt_ = nullptr);
    void draw(bool updateScreen = true);
    void clearWarning(bool updateScreen = true);
    void drawWarning(bool updateScreen = true);
    void measure();
    void manage();

  protected:
    U8G2& _u8g2;
    GEM_u8g2& _menu;
    SettingsBattery& _settings;
    byte _batteryPin;

    // Managing Battery
    byte _batteryLevel = 3;   // Currently displayed battery icon variant
    int _batteryPerc = 100;   // Battery percentage left
    char _percBuffer[5];      // Variable to hold string representation of parcentage for printing on the screen
    int _batteryVolt = 0;     // Battery voltage, mV
    int* _pBatteryVolt = nullptr; // Optional pointer to external variable to pass Battery voltage to (e.g. to display in a menu item)
    const int _batteryMonitoringInterval = BATTERY_MONITORING_INTERVAL; // How often to measure battery level
    long _now; // Variable to hold current time taken with millis() function
    long _batteryMonitoringTime = 0; // Variable to hold time of the latest battery measurement
};

#endif
