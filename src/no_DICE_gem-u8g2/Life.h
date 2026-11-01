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

#ifndef HEADER_LIFE
#define HEADER_LIFE

#include <U8g2lib.h>
#include <GEM_u8g2.h>
#include "Battery.h"
#include "SettingsLife.h"

#define NO_DICE_COUNTER_FONT u8g2_font_spleen32x64_mn
#define NO_DICE_COUNTER_LEN 4
#define NO_DICE_COUNTER_MAX 9999
#define NO_DICE_COUNTER_MIN -999

class Life {
  public:
    Life(U8G2& u8g2_, Battery& battery_, SettingsLife& settings_);

    // Managing context
    void enter();
    void repeat(byte key);
    void exit();

  protected:
    U8G2& _u8g2;
    Battery& _battery;
    SettingsLife& _settings;

    // Managing Counter
    bool _initialized = false;
    int _counter = 0;
    void initializeWithInitialValue();
    void waitForInteraction(byte key);
    void drawCounter();
};

#endif
