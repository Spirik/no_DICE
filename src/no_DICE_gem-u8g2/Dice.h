/*
  Class for handling throwing a dice of no_DICE.

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

#ifndef HEADER_DICE
#define HEADER_DICE

#include <U8g2lib.h>
#include <GEM_u8g2.h>
#include "Battery.h"
#include "constants.h"
#include "sprites-dice.h"
#include "SettingsDice.h"

// #define NO_DICE_THROW_FONT u8g2_font_spleen32x64_mn
#define NO_DICE_THROW_FONT u8g2_font_spleen16x32_mf
#define NO_DICE_RESULT_LEN 3

class Dice {
  public:
    Dice(U8G2& u8g2_, Battery& battery_, SettingsDice& settings_, SelectOptionByte* selectDiceSidesOptions_, byte optionsLength_);

    // Managing context
    void enter();
    void repeat(byte key);
    void exit();

  protected:
    U8G2& _u8g2;
    Battery& _battery;
    SettingsDice& _settings;
    SelectOptionByte* _selectDiceSidesOptions;
    byte _optionsLength;

    // Throw a dice routines
    byte _selectedOptionNum = 0;
    byte _result = 0;
    bool _readyForThrow = false;
    void initializeWithInitialValue();
    byte getSelectedOptionNum();
    void waitForInteraction(byte key);
    void printDiceVariant();
    void resetDice();
    void throwDice();
    void prepareForThrow();

    // Animation
    byte _tickInterval = 50;
    byte _tickCount = 0;
    byte _offsetY = 0;
    bool _animationRunning = true;
    bool _introPlaying = true;
    bool _throwPlaying = false;
    unsigned long _previousMillis = 0;
    void printResult(byte offsetY = 0);
    void drawSprite(GEMSprite sprite, byte offsetY = 0);
    void drawFrame();
    void tweenParam(byte& param, byte fromVal, byte toVal, byte startTick, byte duration, byte easing = NO_DICE_EASING_LINEAR, bool everyOther = false);
    void tick();
    void checkTimer();
    void resetTimer();
};

#endif
