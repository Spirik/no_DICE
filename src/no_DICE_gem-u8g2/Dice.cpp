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

#include "Dice.h"
#include "helpers.h"

Dice::Dice(U8G2& u8g2_, Battery& battery_, SettingsDice& settings_, SelectOptionByte* selectDiceSidesOptions_, byte optionsLength_)
  : _u8g2(u8g2_)
  , _battery(battery_)
  , _settings(settings_)
  , _selectDiceSidesOptions(selectDiceSidesOptions_)
  , _optionsLength(optionsLength_)
{ }

//====================== Managing Context

void Dice::enter() {
  _u8g2.clear();
  initializeWithInitialValue();
  resetDice();
}

void Dice::repeat(byte key) {
  checkTimer();
  if (_readyForThrow) {
    waitForInteraction(key);
  }
}

void Dice::exit() {
  _result = 0;
  resetTimer();
  _u8g2.setFontMode(0);
}

//====================== Throw a dice routines

void Dice::initializeWithInitialValue() {
  _selectedOptionNum = getSelectedOptionNum();
}

byte Dice::getSelectedOptionNum() {
  SelectOptionByte* optsByte = (SelectOptionByte*)_selectDiceSidesOptions;
  bool found = false;
  for (byte i=0; i < _optionsLength; i++) {
    if (optsByte[i].val_byte == _settings.sides) {
      return i;
    }
  }
  return -1;
}

void Dice::waitForInteraction(byte key) {
  if (key != GEM_KEY_NONE) {
    if (key == GEM_KEY_RIGHT) {
      // Next dice variant
      _selectedOptionNum = _selectedOptionNum == _optionsLength - 1 ? 0 : _selectedOptionNum + 1;
      resetDice();
    } else if (key == GEM_KEY_LEFT) {
      // Privious dice variant
      _selectedOptionNum = _selectedOptionNum == 0 ? _optionsLength - 1 : _selectedOptionNum - 1;
      resetDice();
    } else if (key == GEM_KEY_OK) {
      // Throw
      throwDice();
    }
  }
}

void Dice::printDiceVariant() {
  _u8g2.setFont(GEM_FONT_BIG);
  _u8g2.drawStr(_u8g2.getDisplayWidth() - _u8g2.getStrWidth(_selectDiceSidesOptions[_selectedOptionNum].name), _u8g2.getDisplayHeight() - 8, _selectDiceSidesOptions[_selectedOptionNum].name);
}

void Dice::resetDice() {
  Serial.print(F("Variant: "));
  Serial.println(_selectDiceSidesOptions[_selectedOptionNum].name);

  _result = 0;

  _animationRunning = true;
  _introPlaying = true;
  _throwPlaying = false;

  _readyForThrow = false;

  resetTimer();
}

void Dice::throwDice() {
  _result = random(_selectDiceSidesOptions[_selectedOptionNum].val_byte) + 1;

  Serial.print(F("Throw a dice result: "));
  Serial.println(_result);

  _animationRunning = true;
  _introPlaying = false;
  _throwPlaying = true;

  _readyForThrow = false;

  resetTimer();
}

void Dice::prepareForThrow() {
  _readyForThrow = true;
}

//====================== Animation

void Dice::printResult(byte offsetY) {
  char valueStringTmp[NO_DICE_RESULT_LEN + 1] = "?";
  if (_selectDiceSidesOptions[_selectedOptionNum].val_byte == 6 && _result > 0) {
    drawSprite(diceD6Results[_result - 1], _offsetY);
  } else {
    if (_result > 0) {
      itoa((int)_result, valueStringTmp, 10);
    }
    _u8g2.setFont(NO_DICE_THROW_FONT);
    _u8g2.setFontMode(1);
    _u8g2.drawStr((_u8g2.getDisplayWidth() - _u8g2.getStrWidth(valueStringTmp)) / 2, _u8g2.getDisplayHeight() / 2 - 12 + offsetY, valueStringTmp);
  }
}

// Draw sprite on screen
// (note that GEMSprite is the custom type used internally in GEM library for convenient way of storing XBM graphics of various sprites)
void Dice::drawSprite(GEMSprite sprite, byte offsetY) {
  _u8g2.drawXBMP((_u8g2.getDisplayWidth() - sprite.width) / 2, (_u8g2.getDisplayHeight() - sprite.height) / 2 + offsetY, sprite.width, sprite.height, sprite.image);
}

// Draw frame of dice animation
void Dice::drawFrame() {
  // Draw frame on screen
  _u8g2.clearBuffer();
  drawSprite(diceVariants[_selectedOptionNum], _offsetY);
  printResult(_offsetY);
  printDiceVariant();
  _battery.drawWarning(false);
  _u8g2.sendBuffer();
}

void Dice::tweenParam(byte& param, byte fromVal, byte toVal, byte startTick, byte duration, byte easing, bool everyOther) {
  if (_tickCount >= startTick) {
    if (!everyOther || (everyOther && ((_tickCount & 1) == 0))) {
      float progress = ((float)_tickCount - startTick) / (duration - 1);
      if (easing) {
        progress = ease(progress, easing);
      }
      if (progress <= 1) {
        param = fromVal + (int)round((toVal - fromVal) * progress);
      }
    }
  }
}

void Dice::tick() {
  // Intro animation is currently playing
  if (_introPlaying) {
    // Slide dice in from the bottom
    tweenParam(_offsetY, 96, 0, 0, 8, NO_DICE_EASING_EASE_OUT_CUBIC);
    drawFrame();
    if (_tickCount > 8) {
      _introPlaying = false;
      _animationRunning = false;
      Serial.println(F("Intro animation finished"));
      prepareForThrow();
    } else {
      _tickCount += 1;
    }
  }

  // Throw animation is currently playing
  if (_throwPlaying) {
    // Slide dice in from the bottom
    tweenParam(_offsetY, 96, 0, 0, 8, NO_DICE_EASING_EASE_OUT_CUBIC);
    drawFrame();
    if (_tickCount > 8) {
      _throwPlaying = false;
      _animationRunning = false;
      Serial.println(F("Throw animation finished"));
      prepareForThrow();
    } else {
      _tickCount += 1;
    }
  }
}

void Dice::checkTimer() {
  if (_animationRunning) {
    // Test millis timer and draw frame accordingly
    unsigned long currentMillis = millis();
    if (currentMillis - _previousMillis >= _tickInterval) {
      _previousMillis = currentMillis;
      tick();
    }
  }
}

void Dice::resetTimer() {
  _previousMillis = 0;
  _tickCount = 0;
}
