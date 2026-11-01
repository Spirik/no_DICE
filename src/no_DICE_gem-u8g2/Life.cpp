/*
  Class for handling life counter of no_DICE.

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

#include "Life.h"

Life::Life(U8G2& u8g2_, Battery& battery_, SettingsLife& settings_)
  : _u8g2(u8g2_)
  , _battery(battery_)
  , _settings(settings_)
{ }

//====================== Managing Context

void Life::enter() {
  _u8g2.clear();
  if (!_initialized) {
    initializeWithInitialValue();
  }
  drawCounter();
}

void Life::repeat(byte key) {
  waitForInteraction(key);
}

void Life::exit() {
}

//====================== Managing Counter

void Life::initializeWithInitialValue() {
  _counter = _settings.startingValue;
  _initialized = true;
}

void Life::waitForInteraction(byte key) {
  if (key != GEM_KEY_NONE) {
    if (key == GEM_KEY_DOWN) {
      // Increment
      _counter = _counter == NO_DICE_COUNTER_MAX ? NO_DICE_COUNTER_MIN : _counter + 1;
    } else if (key == GEM_KEY_UP) {
      // Decrement
      _counter = _counter == NO_DICE_COUNTER_MIN ? NO_DICE_COUNTER_MAX : _counter - 1;
    } else if (key == GEM_KEY_OK) {
      // Reset
      initializeWithInitialValue();
    }
    drawCounter();
  }
}

void Life::drawCounter() {
  _u8g2.clearBuffer();
  _u8g2.setFont(NO_DICE_COUNTER_FONT);
  char valueStringTmp[NO_DICE_COUNTER_LEN + 1];
  itoa((int)_counter, valueStringTmp, 10);
  _u8g2.drawStr((_u8g2.getDisplayWidth() - _u8g2.getStrWidth(valueStringTmp)) / 2, _u8g2.getDisplayHeight() / 2 - 32, valueStringTmp);
  _battery.drawWarning(false);
  _u8g2.sendBuffer();
  Serial.print(F("Counter value: "));
  Serial.println(_counter);
}
