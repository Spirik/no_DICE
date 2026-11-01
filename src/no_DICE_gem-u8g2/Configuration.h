/*
  Configuration of no_DICE: class that sets and loads previously saved settings.

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

#ifndef HEADER_CONFIGURATION
#define HEADER_CONFIGURATION

#include <EEPROM.h>
#include "constants.h"
#include "Settings.h"
#include "StoreData.h"

class Configuration {
  public:
    static void initConfiguration(Settings &config);
    static void loadConfiguration(Settings &config);        // Load previously saved settings
    static void saveConfiguration(const Settings &config);  // Save settings
    static void resetConfiguration();                       // Reset settings to default values
  
  private:
    Configuration() = delete; // Disallow creating an instance of this object (as it acts as a static class)
};

#endif
