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

#include "Battery.h"

Battery::Battery(U8G2& u8g2_, GEM_u8g2& menu_, SettingsBattery& settings_, byte batteryPin_, int* pBatteryVolt_)
  : _u8g2(u8g2_)
  , _menu(menu_)
  , _settings(settings_)
  , _batteryPin(batteryPin_)
  , _pBatteryVolt(pBatteryVolt_)
{
  _now = millis();
}

//====================== Managing Battery

void Battery::draw(bool updateScreen) {
  if (_settings.showIcon) {
      _u8g2.drawXBMP((_u8g2.getDisplayWidth() - batteryIcons[_batteryLevel].width) - 1, 0, batteryIcons[_batteryLevel].width, batteryIcons[_batteryLevel].height, batteryIcons[_batteryLevel].image);
  }
  if (_settings.showNumber) {
    // Backing up currently set font
    const uint8_t *fontBackup = _u8g2.getU8g2()->font;
    _u8g2.setFont(NO_DICE_FONT_SMALL);

    // Prcentage
    int offset = _settings.showIcon ? batteryIcons[_batteryLevel].width + 3 : 2;
    _u8g2.drawStr(_u8g2.getDisplayWidth() - _u8g2.getStrWidth(_percBuffer) - offset, 0, _percBuffer);

    // Voltage (for debug)
    /* char voltBuffer[5];
    itoa(_batteryVolt, voltBuffer, 10);
    _u8g2.drawStr(_u8g2.getDisplayWidth() - _u8g2.getStrWidth(voltBuffer) - _u8g2.getStrWidth(_percBuffer) - offset - 4, 2, voltBuffer); */

    // Restoring backed up font
    _u8g2.setFont(fontBackup);
  }
  if ((_settings.showIcon || _settings.showNumber) && updateScreen) {
    _u8g2.sendBuffer();
  }
}

void Battery::clearWarning(bool updateScreen) {
  int x = _u8g2.getDisplayWidth();
  if (_settings.showIcon) {
    x = x - batteryIcons[_batteryLevel].width - 1;
  }
  if (_settings.showNumber) {
    byte offset = _settings.showIcon ? 3 : 2;
    x = x - _u8g2.getStrWidth(_percBuffer) - offset;
  }
  int y = 0;
  int w = _u8g2.getDisplayWidth() - x;
  byte h = batteryIcons[_batteryLevel].height;
  _u8g2.setDrawColor(0);
  _u8g2.drawBox(x, y, w, h);
  if ((_settings.showIcon || _settings.showNumber) && updateScreen) {
    _u8g2.sendBuffer();
  }
  _u8g2.setDrawColor(1);
}

void Battery::drawWarning(bool updateScreen) {
  if (_settings.enableMonitoring != 0) {
    if (_batteryPerc < BATTERY_WARNING_PERC) {
      draw(updateScreen);
    } else {
      clearWarning(updateScreen);
    }
  }
}

void Battery::measure() {
  uint32_t adcRawSum = 0;
  for (int i = 0; i < 16; i++) {
    adcRawSum = adcRawSum + analogRead(_batteryPin);
  }
  uint32_t adcRaw = adcRawSum / 16; // Average of 16 samples
  float Vbat = adcRaw * BATTERY_DIVIDER_RATIO * BATTERY_VREF / 4095; // Voltage divider ratio, battery nominal voltage, 12bit resolution
  _batteryVolt = (int)Vbat;
  if (_pBatteryVolt != nullptr) {
    *_pBatteryVolt = _batteryVolt;
  }
  _batteryPerc = (int)(100.00 * (_batteryVolt - BATTERY_MIN) / (BATTERY_MAX - BATTERY_MIN));
  _batteryPerc = _batteryPerc > 100 ? 100 : (_batteryPerc < 0 ? 0 : _batteryPerc);

  if (_batteryPerc > 69) {
    _batteryLevel = 3;
  } else if (_batteryPerc > 39) {
    _batteryLevel = 2;
  } else if (_batteryPerc > 19) {
    _batteryLevel = 1;
  } else {
    _batteryLevel = 0;
  }

  itoa(_batteryPerc, _percBuffer, 10);
  // Add % sign
  char sign = '%';
  size_t len = strlen(_percBuffer);
  _percBuffer[len] = sign;
  _percBuffer[len + 1] = '\0';

  Serial.print(F("Battery: "));
  Serial.print(_batteryVolt);
  Serial.print(F("mV "));
  Serial.print(_batteryPerc);
  Serial.println(F("%"));
}

void Battery::manage() {
  _now = millis();
  if (_settings.enableMonitoring != 0) {
    if (_now > _batteryMonitoringTime + _batteryMonitoringInterval) {
      measure();
      if (_menu.context.loop == nullptr) {
        // We are in the GEM menu...
        // draw();
        _menu.drawMenu(); // Necessary for updating voltage reading in menu item, simple call to draw() won't be enough (to update just status bar icon)
      } else {
        // ...otherwise, we are in context (Dice, Coin or Life routine presumably) - then only low-battery warning is worth checking
        drawWarning();
      }
      _batteryMonitoringTime = _now;
    }
  }
}