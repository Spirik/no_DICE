/*
  Common useful helper functions of no_DICE.

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

#include "helpers.h"

float ease(float x, byte fn) {
  switch (fn) {
    case NO_DICE_EASING_LINEAR:
      return x;
    case NO_DICE_EASING_EASE_IN_CUBIC:
      return x * x * x;
    case NO_DICE_EASING_EASE_OUT_CUBIC:
      return 1 - pow(1 - x, 3);
    case NO_DICE_EASING_EASE_IN_OUT_CUBIC:
      return x < 0.5 ? 4 * x * x * x : 1 - pow(-2 * x + 2, 3) / 2;
    default:
      return x;
  }
}
