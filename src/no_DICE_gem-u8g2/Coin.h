/*
  Class for handling tossing a coin of no_DICE.

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

#ifndef HEADER_COIN
#define HEADER_COIN

#include <U8g2lib.h>
#include <GEM_u8g2.h>
#include "Battery.h"
#include "constants.h"
#include "sprites-coin.h"
#include "SettingsGeneral.h"

#define ALLOW_TOSS_INTERRUPTION true

class Coin {
  public:
    Coin(U8G2& u8g2_, Battery& battery_, SettingsGeneral& settingsGeneral_);

    // Managing context
    void enter();
    void repeat(byte key);
    void exit();

  protected:
    U8G2& _u8g2;
    Battery& _battery;
    SettingsGeneral& _settingsGeneral;

    // Toss a coin routines
    byte _result;
    bool _readyForToss = false;
    void waitForToss(byte key);
    void toss();
    void prepareForToss();
    void promptForToss();
    
    // Animation
    byte _tickInterval = 50;
    byte _tickCount = 0;
    byte _framesCount = 12;
    byte _currentFrame = 0;
    byte _currentVirtualFrame = 0;
    byte _offsetY = 0;
    byte _revolutionsCount = 2;
    bool _animationRunning = true;
    bool _introPlaying = true;
    bool _tossPlaying = false;
    unsigned long _previousMillis = 0;
    void drawSprite(GEMSprite sprite, byte offsetY = 0);
    void drawFrame(bool intro = false);
    void tweenParam(byte& param, byte fromVal, byte toVal, byte startTick, byte duration, byte easing = NO_DICE_EASING_LINEAR, bool everyOther = false);
    void tick();
    void checkTimer();
    void resetTimer();
};

#endif
