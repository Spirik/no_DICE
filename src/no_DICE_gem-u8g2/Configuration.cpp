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

#include "Configuration.h"

void Configuration::initConfiguration(Settings &config) {
  #if defined(ESP8266) || defined(ESP32) || defined(ARDUINO_ARCH_RP2040) || defined(ARDUINO_ARCH_MBED_RP2040) || defined(ARDUINO_ARCH_MBED_NANO)
    EEPROM.begin(EEPROM_SIZE);
  #endif
  loadConfiguration(config);
}

void Configuration::loadConfiguration(Settings &config) {
  Serial.print(F("Loading settings from internal storage... "));

  StoreData dataTmp;
  EEPROM.get(EEPROM_ADDR, dataTmp);
  if ((strcmp(dataTmp.status, INIT_STATUS) == 0) && (strcmp(dataTmp.version, SETTINGS_VER) == 0)) {
    Serial.print(F("Existing data found in EEPROM. "));
    config = dataTmp.settings;
    Serial.println(F("Settings loaded successfully"));
  } else {
    Serial.println(F("No data found in EEPROM. Initializing with default values"));
    saveConfiguration(config);
  }
}

void Configuration::saveConfiguration(const Settings &config) {
  Serial.print(F("Saving settings to internal storage... "));

  StoreData dataTmp;
  dataTmp.settings = config;
  strcpy(dataTmp.version, SETTINGS_VER);
  strcpy(dataTmp.status, INIT_STATUS);
  EEPROM.put(EEPROM_ADDR, dataTmp);
  #if defined(ESP8266) || defined(ESP32) || defined(ARDUINO_ARCH_RP2040) || defined(ARDUINO_ARCH_MBED_RP2040) || defined(ARDUINO_ARCH_MBED_NANO)
    if (EEPROM.commit()) {
      Serial.print(F("EEPROM successfully committed... "));
      Serial.println(F("Settings saved successfully"));
    } else {
      Serial.println(F("ERROR! EEPROM commit failed"));
    }
  #else
    Serial.println(F("Settings saved successfully"));
  #endif
}

void Configuration::resetConfiguration() {
  Serial.println("Reset settings to default values");
  StoreData dataTmp;
  strcpy(dataTmp.status, "GARBAGE");
  EEPROM.put(EEPROM_ADDR, dataTmp);
  #if defined(ESP8266) || defined(ESP32) || defined(ARDUINO_ARCH_RP2040) || defined(ARDUINO_ARCH_MBED_RP2040) || defined(ARDUINO_ARCH_MBED_NANO)
    if (EEPROM.commit()) {
      Serial.print(F("EEPROM successfully committed... "));
      Serial.println(F("Settings reset successfully"));
    } else {
      Serial.println(F("ERROR! EEPROM commit failed"));
    }
  #else
    Serial.println(F("Settings reset successfully"));
  #endif
  delay(500);
  // Note that this function is hardware dependent!
  // Should work for Arduino Uno R4, RP2040 boards, ESP32, ESP8266 and most of the other ARM boards
  // (on AVR boards manual reset is required)
  #if defined(ARDUINO_ARCH_RP2040) || defined(ARDUINO_ARCH_MBED_RP2040) || defined(ARDUINO_ARCH_MBED_NANO)
    rp2040.reboot();
  #elif defined(ESP32) || defined(ARDUINO_ARCH_ESP32) || defined(ESP8266) || defined(ARDUINO_ARCH_ESP8266)
    ESP.restart();
  #elif defined(__arm__) || defined(__ARM_ARCH)
    NVIC_SystemReset();
  #endif
}
