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

#include "Coin.h"
#include "helpers.h"

Coin::Coin(U8G2& u8g2_, Battery& battery_, SettingsGeneral& settingsGeneral_)
  : _u8g2(u8g2_)
  , _battery(battery_)
  , _settingsGeneral(settingsGeneral_)
{ }

//====================== Managing Context

void Coin::enter() {
  _u8g2.clear();
  _currentVirtualFrame = 0;
  _currentFrame = 0;
  _readyForToss = false;
  _animationRunning = true;
  _introPlaying = true;
  _tossPlaying = false;
}

void Coin::repeat(byte key) {
  checkTimer();
  if (_readyForToss || ALLOW_TOSS_INTERRUPTION) {
    waitForToss(key);
  }
}

void Coin::exit() {
  resetTimer();
}

//====================== Toss a coin routines

void Coin::waitForToss(byte key) {
  if (key == GEM_KEY_OK) {
    toss();
  }
}

void Coin::toss() {
  _result = random(2);
  _revolutionsCount = 2 + random(3);

  Serial.print(F("Toss a coin result: "));
  Serial.print(_result);
  Serial.print(F(" | Revolutions: "));
  Serial.println(_revolutionsCount);

  _animationRunning = true;
  _introPlaying = false;
  _tossPlaying = true;
  
  if (ALLOW_TOSS_INTERRUPTION) {
    prepareForToss();
  } else {
    _readyForToss = false;
  }
  
  resetTimer();
}

void Coin::prepareForToss() {
  _currentVirtualFrame = 0;
  _currentFrame = 0;
  _readyForToss = true;
}

void Coin::promptForToss() {
  _u8g2.clearBuffer();
  _u8g2.enableUTF8Print();
  if (_settingsGeneral.language == NO_DICE_LANG_RU) {
    _u8g2.setFont(GEM_FONT_BIG_CYR);
  } else {
    _u8g2.setFont(NO_DICE_FONT_BIG);
  }
  const char* promptForToss[] = {"Press to toss", "Нажми для броска", "Zum werfen drücken", "Appuyer pour lancer"};
  _u8g2.drawUTF8((_u8g2.getDisplayWidth() - _u8g2.getUTF8Width(promptForToss[_settingsGeneral.language])) / 2, _u8g2.getDisplayHeight() / 2 - 4, promptForToss[_settingsGeneral.language]);
  _battery.drawWarning(false);
  _u8g2.sendBuffer();
}

//====================== Animation

// Draw sprite on screen
// (note that GEMSprite is the custom type used internally in GEM library for convenient way of storing XBM graphics of various sprites)
void Coin::drawSprite(GEMSprite sprite, byte offsetY) {
  _u8g2.drawXBMP((_u8g2.getDisplayWidth() - sprite.width) / 2, (_u8g2.getDisplayHeight() - sprite.height) / 2 + offsetY, sprite.width, sprite.height, sprite.image);
}

// Draw frame of coin animation
void Coin::drawFrame(bool intro) {
  // _currentFrame = (_currentFrame == _framesCount ? 1 : _currentFrame + 1);
  // _currentFrame = _currentVirtualFrame > 0 ? (_currentVirtualFrame + 1) % _framesCount : 0;
  _currentFrame = _currentVirtualFrame % _framesCount;
  /* Serial.print("        -> ");
  Serial.print(_currentVirtualFrame);
  Serial.print(" | ");
  Serial.println(_currentFrame); */
  // Draw frame on screen
  _u8g2.clearBuffer();
  drawSprite(intro ? coinIntroFrames[_currentFrame] : coinVerticalFrames[_currentFrame], _offsetY);
  _battery.drawWarning(false);
  _u8g2.sendBuffer();
}

void Coin::tweenParam(byte& param, byte fromVal, byte toVal, byte startTick, byte duration, byte easing, bool everyOther) {
  if (_tickCount >= startTick) {
    if (!everyOther || (everyOther && ((_tickCount & 1) == 0))) {
      float progress = ((float)_tickCount - startTick) / (duration - 1);
      if (easing) {
        progress = ease(progress, easing);
      }
      if (progress <= 1) {
        param = fromVal + (int)round((toVal - fromVal) * progress);
        /* if (_introPlaying) {
          Serial.print(_tickCount);
          Serial.print(" | ");
          Serial.print(progress);
          Serial.print(" | ");
          Serial.print(param);
          Serial.print(" | ");
          Serial.print(_currentFrame);
          Serial.println();
        } */
      }
    }
  }
}

void Coin::tick() {
  // Intro animation is currently playing
  if (_introPlaying) {
    // Spin coin horizontally (intro)
    // tweenParam(_currentVirtualFrame, 0, 12, 2, 26); // for 100ms tick
    tweenParam(_currentVirtualFrame, 0, 12, 4, 52);
    // Slide coin in from the bottom
    // tweenParam(_offsetY, 64, 0, 0, 4); // for 100ms tick
    tweenParam(_offsetY, 64, 0, 0, 10, NO_DICE_EASING_EASE_OUT_CUBIC);
    // Slide coin out to the bottom
    // tweenParam(_offsetY, 0, 64, 26, 4);
    tweenParam(_offsetY, 0, 64, 52, 10, NO_DICE_EASING_EASE_IN_CUBIC);
    drawFrame(true);
    if (_tickCount > 66) { // 33 for 100ms tick
      _introPlaying = false;
      _animationRunning = false;
      Serial.println(F("Intro animation finished"));
      prepareForToss();
      promptForToss();
    } else {
      _tickCount += 1;
    }
  }

  // Toss animation is currently playing
  if (_tossPlaying) {
    byte startTick = 4;
    byte fromVal = _result * _framesCount / 2;
    byte duration = _framesCount * (_revolutionsCount * 0.5 + 0.25) + 1;
    byte toVal = fromVal + duration - 1;
    // Spin coin horizontally (intro)
    tweenParam(_currentVirtualFrame, fromVal, toVal, startTick, 2 * duration, NO_DICE_EASING_LINEAR, true);
    // Slide coin in from the bottom
    tweenParam(_offsetY, 64, 0, 0, 8, NO_DICE_EASING_EASE_OUT_CUBIC);
    drawFrame();
    if (_tickCount > startTick + 2 * duration) {
      _tossPlaying = false;
      _animationRunning = false;
      Serial.println(F("Toss animation finished"));
      prepareForToss();
    } else {
      _tickCount += 1;
    }
  }
}

void Coin::checkTimer() {
  if (_animationRunning) {
    // Test millis timer and draw frame accordingly
    unsigned long currentMillis = millis();
    if (currentMillis - _previousMillis >= _tickInterval) {
      _previousMillis = currentMillis;
      tick();
    }
  }
}

void Coin::resetTimer() {
  _previousMillis = 0;
  _currentFrame = 0;
  _tickCount = 0;
}
