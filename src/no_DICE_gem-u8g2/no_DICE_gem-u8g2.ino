/*
  no_DICE: simulator of dice throws, coin flips and a score counter.

  U8g2lib library is used for drawing operations.
  U8g2 version of GEM library is used to create and draw menu.
  KeyDetector library is used to detect rotary encoder operation.
  
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

#include <GEM_u8g2.h>
#include <KeyDetector.h>

// Custom menu icons are stored here
#include "sprites-u8g2-custom.h"

// Configurable settings and their default values (and methods for their retrieval) are stored in the following files
#include "Configuration.h"
#include "preset.h"

// Service features
#include "Battery.h"

// Main features
#include "Dice.h"
#include "Coin.h"
#include "Life.h"

//====================== ROTARY ENCODER SETTINGS

// Define signal identifiers for three outputs of encoder (channel A, channel B and a push-button)
#define KEY_A 1
#define KEY_B 2
#define KEY_C 3

// Pins encoder is connected to (update to fit your particular configuration):

// Arduino Uno R4 (Minima) with GEM Encoder Shield (https://github.com/Spirik/GEM/wiki/Encoder-SH1106-shield)
// const byte channelA = 2;
// const byte channelB = 3;
// const byte buttonPin = 4;
// const byte batteryPin = A3;

// Wemos D1 R32 ESP32 Dev Board with GEM Encoder Shield
// const byte channelA = 26;
// const byte channelB = 25;
// const byte buttonPin = 17;
// const byte batteryPin = 34;

// XIAO RP2040 with Grove Shield
const byte channelA = 26;   // A0
const byte channelB = 27;   // A1
const byte buttonPin = 28;  // A2
const byte batteryPin = A3; // A3

byte chanB = HIGH; // Variable to store Channel B readings

//====================== INPUT CONTROLS AND U8G2 CONSTRUCTOR

// Array of Key objects that will link GEM key identifiers with dedicated pins
// (it is only necessary to detect signal change on a single channel of the encoder, either A or B;
// order of the channel and push-button Key objects in an array is not important)
Key keys[] = {{KEY_A, channelA}, {KEY_C, buttonPin}};
//Key keys[] = {{KEY_C, buttonPin}, {KEY_A, channelA}};

// Create KeyDetector object
// KeyDetector myKeyDetector(keys, sizeof(keys)/sizeof(Key));
// To account for switch bounce effect of the buttons (if occur) you may want to specify debounceDelay
// as the third argument to KeyDetector constructor.
// Make sure to adjust debounce delay to better fit your rotary encoder.
// Also it is possible to enable pull-up mode when buttons wired with pull-up resistors (as in this case).
// Analog threshold is not necessary for this example and is set to default value 16.
KeyDetector myKeyDetector(keys, sizeof(keys)/sizeof(Key), /* debounceDelay= */ 5, /* analogThreshold= */ 16, /* pullup= */ true);

bool secondaryPressed = false;  // If encoder rotated while key was being pressed; used to prevent unwanted triggers
bool cancelPressed = false;  // Flag indicating that Cancel action was triggered, used to prevent it from triggering multiple times
const int keyPressDelay = 1000; // How long to hold key in pressed state to trigger Cancel action, ms
long keyPressTime = 0; // Variable to hold time of the key press event
long now; // Variable to hold current time taken with millis() function at the beginning of loop()

// Create an instance of the U8g2 library.
// Use constructor that matches your setup (see https://github.com/olikraus/u8g2/wiki/u8g2setupcpp for details).
// This instance is used to call all the subsequent U8g2 functions (internally from GEM library,
// or manually in your sketch if it is required).
// Please update the pin numbers according to your setup. Use U8X8_PIN_NONE if the reset pin is not connected
// U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

//====================== MENU ITEMS

// Define a custom ID that will help to identify specific button
#define DICE_BUTTON_ID 1
#define COIN_BUTTON_ID 2
#define LIFE_BUTTON_ID 3

//--- Main Menu Items

// Create menu button that will trigger launchDice() function.
// We should forward-declare it in order to pass to GEMItem constructor
void launchDice(GEMCallbackData callbackData); // Forward declaration
const char* menuItemButtonDiceTitle[] = {"DICE", "КУБИК", "WÜRFEL", "DÉS"};
// Set a custom ID associated with a button to help to identify it later in launchDice() and/or drawCustomIcon() callbacks
GEMItem menuItemButtonDice(menuItemButtonDiceTitle[settings.settingsGeneral.language], launchDice, DICE_BUTTON_ID);

// Create menu button that will trigger launchCoin() function.
// We should forward-declare it in order to pass to GEMItem constructor
void launchCoin(GEMCallbackData callbackData); // Forward declaration
const char* menuItemButtonCoinTitle[] = {"COIN", "МОНЕТКА", "MÜNZE", "PIÈCE DE MONNAIE"};
// Set a custom ID associated with a button to help to identify it later in launchCoin() and/or drawCustomIcon() callbacks
GEMItem menuItemButtonCoin(menuItemButtonCoinTitle[settings.settingsGeneral.language], launchCoin, COIN_BUTTON_ID);

// Create menu button that will trigger launchLife() function.
// We should forward-declare it in order to pass to GEMItem constructor
void launchLife(GEMCallbackData callbackData); // Forward declaration
const char* menuItemButtonLifeTitle[] = {"LIFE", "ЖИЗНИ", "PUNKTE", "POINTS"};
// Set a custom ID associated with a button to help to identify it later in launchLife() and/or drawCustomIcon() callbacks
GEMItem menuItemButtonLife(menuItemButtonLifeTitle[settings.settingsGeneral.language], launchLife, LIFE_BUTTON_ID);

//--- Settings Menu Items

// Common callback function for saving settings
void saveSettings(); // Forward declaration

// Create menu item for dice sides option select
SelectOptionByte selectDiceSidesOptions[] = {
  {"D4", 4},
  {"D6", 6},
  {"D8", 8},
  {"D10", 10},
  {"D12", 12},
  {"D20", 20},
  {"D100", 100}
};
GEMSelect selectDiceSides(sizeof(selectDiceSidesOptions)/sizeof(SelectOptionByte), selectDiceSidesOptions);
const char* menuItemDiceSidesTitle[] = {"Default dice:", "Старт. кубик:", "Standardwürfel:", "Dés par défaut:"};
GEMItem menuItemDiceSides(menuItemDiceSidesTitle[settings.settingsGeneral.language], settings.settingsDice.sides, selectDiceSides, saveSettings);

// Create menu item for life starting value spinner
GEMSpinnerBoundariesInt lifeStartingValueSpinnerBoundaries = { .step = 1, .min = NO_DICE_COUNTER_MIN, .max = NO_DICE_COUNTER_MAX };
GEMSpinner lifeStartingValueSpinner(lifeStartingValueSpinnerBoundaries, GEM_LOOP);
const char* menuItemLifeStartingValueTitle[] = {"Starting life:", "Старт. жизнь:", "Startpunkte:", "Pts. de départ:"};
GEMItem menuItemLifeStartingValue(menuItemLifeStartingValueTitle[settings.settingsGeneral.language], settings.settingsLife.startingValue, lifeStartingValueSpinner, saveSettings);

// Create menu item for language option select with applyLanguage() callback function
SelectOptionByte selectLangOptions[] = {{"EN", NO_DICE_LANG_EN}, {"RU", NO_DICE_LANG_RU}, {"DE", NO_DICE_LANG_DE}, {"FR", NO_DICE_LANG_FR}};
GEMSelect selectLanguage(sizeof(selectLangOptions)/sizeof(SelectOptionByte), selectLangOptions);
void applyLanguage(); // Forward declaration
const char* menuItemLanguageTitle[] = {"Language:", "Язык:", "Sprache:", "Langue:"};
GEMItem menuItemLanguage(menuItemLanguageTitle[settings.settingsGeneral.language], settings.settingsGeneral.language, selectLanguage, applyLanguage);

// Create menu item for menu pointer option select with applyMenuPointer() callback function
SelectOptionByte selectMenuPointerOptions[][2] = {
  {{"Row", GEM_POINTER_ROW}, {"Dash", GEM_POINTER_DASH}},     // English
  {{"Стрк.", GEM_POINTER_ROW}, {"Линия", GEM_POINTER_DASH}},  // Russian
  {{"Zeile", GEM_POINTER_ROW}, {"Linie", GEM_POINTER_DASH}},  // German
  {{"Ligne", GEM_POINTER_ROW}, {"Tiret", GEM_POINTER_DASH}}   // French
};
GEMSelect selectMenuPointer(sizeof(selectMenuPointerOptions[settings.settingsGeneral.language])/sizeof(SelectOptionByte), selectMenuPointerOptions[settings.settingsGeneral.language]);
void applyMenuPointer(); // Forward declaration
const char* menuItemMenuPointerTitle[] = {"Menu pointer:", "Курсор меню:", "Cursor:", "Curseur:"};
GEMItem menuItemMenuPointer(menuItemMenuPointerTitle[settings.settingsGeneral.language], settings.settingsGeneral.menuPointer, selectMenuPointer, applyMenuPointer);

//--- Battery Settings Menu Items

// Create menu item for battery monitoring option select
SelectOptionByte selectBatteryMonitoringOptions[][2] = {
  {{"Off", 0}, {"On", 1}},     // English
  {{"Выкл.",0}, {"Вкл.", 1}},  // Russian
  {{"Aus", 0}, {"Aktiv", 1}},  // German
  {{"Off", 0}, {"On", 1}}      // French (keeping English variant to fit into 5 characters)
};
GEMSelect selectBatteryMonitoring(sizeof(selectBatteryMonitoringOptions[settings.settingsGeneral.language])/sizeof(SelectOptionByte), selectBatteryMonitoringOptions[settings.settingsGeneral.language]);
void enableBatteryMonitoringCallback(); // Forward declaration of callback function for showing or hiding options of battery indicator settings based on monitoring status
const char* menuItemBatteryMonitoringTitle[] = {"Monitoring:", "Мониторинг:", "Überwachung:", "Observation:"};
GEMItem menuItemBatteryMonitoring(menuItemBatteryMonitoringTitle[settings.settingsGeneral.language], settings.settingsBattery.enableMonitoring, selectBatteryMonitoring, enableBatteryMonitoringCallback);

// Create menu items for boolean options of battery indicator settings
const char* menuItemBatteryShowIconTitle[] = {"Icon:", "Иконка:", "Symbol:", "Icône:"};
GEMItem menuItemBatteryShowIcon(menuItemBatteryShowIconTitle[settings.settingsGeneral.language], settings.settingsBattery.showIcon, saveSettings);
const char* menuItemBatteryShowNumberTitle[] = {"Percentage:", "Проценты:", "Prozentsatz:", "Pourcentage:"};
GEMItem menuItemBatteryShowNumber(menuItemBatteryShowNumberTitle[settings.settingsGeneral.language], settings.settingsBattery.showNumber, saveSettings);

// Readonly variably for displaying current voltage reading
int batteryVolt = 0; // Battery voltage, mV
const char* menuItemBatteryVoltageTitle[] = {"Voltage, mV:", "Напряж., мВ:", "Spannung, mV:", "Tension, mV:"};
GEMItem menuItemBatteryVoltage(menuItemBatteryVoltageTitle[settings.settingsGeneral.language], batteryVolt, GEM_READONLY);

//--- System Settings Menu Items

// Create readonly system info menu items
char versionFW[GEM_STR_LEN] = NO_DICE_VER;
const char* menuItemInfoVerFWTitle[] = {"Firmware:", "Прошивка:", "Firmware:", "Micrologiciel:"};
GEMItem menuItemInfoVerFW(menuItemInfoVerFWTitle[settings.settingsGeneral.language], versionFW, GEM_READONLY);
char versionSettings[GEM_STR_LEN] = SETTINGS_VER;
const char* menuItemInfoVerSettingsTitle[] = {"Settings:", "Настройки:", "Einstellungen:", "Paramètres:"};
GEMItem menuItemInfoVerSettings(menuItemInfoVerSettingsTitle[settings.settingsGeneral.language], versionSettings, GEM_READONLY);

// Create menu button that will trigger resetSettings() function.
// We should forward-declare it in order to pass to GEMItem constructor
void resetSettings(); // Forward declaration
const char* menuItemButtonResetSettingsTitle[] = {"Factory Reset", "Сброс настроек", "Zurücksetzen", "Réinitialisation"};
GEMItem menuItemButtonResetSettings(menuItemButtonResetSettingsTitle[settings.settingsGeneral.language], resetSettings);

//--- Menu Pages

// Create menu page object of class GEMPage. Menu page holds menu items (GEMItem) and represents menu level.
// Menu can have multiple menu pages (linked to each other) with multiple menu items each
GEMPage menuPageMain("no_DICE"); // Main page
const char* menuPageSettingsTitle[] = {"Settings", "Настройки", "Einstellungen", "Paramètres"};
GEMPage menuPageSettings(menuPageSettingsTitle[settings.settingsGeneral.language]); // Settings submenu
const char* menuPageSettingsBatteryTitle[] = {"Battery Settings", "Настройки батареи", "Batterieeinstellungen", "Paramètres batterie"};
GEMPage menuPageSettingsBattery(menuPageSettingsBatteryTitle[settings.settingsGeneral.language]); // Battery Settings submenu
const char* menuPageSettingsSystemTitle[] = {"System Settings", "Системные настройки", "Systemeinstellungen", "Paramètres système"};
GEMPage menuPageSettingsSystem(menuPageSettingsSystemTitle[settings.settingsGeneral.language]); // System Settings submenu

// Create menu item linked to menu pages
const char* menuItemMainSettingsTitle[] = {"SETTINGS", "НАСТРОЙКИ", "EINSTELLUNGEN", "PARAMÈTRES"};
GEMItem menuItemMainSettings(menuItemMainSettingsTitle[settings.settingsGeneral.language], menuPageSettings);
const char* menuItemSettingsBatteryTitle[] = {"BATTERY", "БАТАРЕЯ", "BATTERIE", "BATTERIE"};
GEMItem menuItemSettingsBattery(menuItemSettingsBatteryTitle[settings.settingsGeneral.language], menuPageSettingsBattery);
const char* menuItemSettingsSystemTitle[] = {"SYSTEM", "СИСТЕМА", "SYSTEM", "SYSTÈME"};
GEMItem menuItemSettingsSystem(menuItemSettingsSystemTitle[settings.settingsGeneral.language], menuPageSettingsSystem);

//====================== INITIALIZATION

// Create GEMAppearance object
GEMAppearance appearanceGeneral;

// Create menu object of class GEM_u8g2. Supply its constructor with reference to u8g2 object we created earlier
GEM_u8g2 menu(u8g2);

Battery battery(u8g2, menu, settings.settingsBattery, batteryPin, &batteryVolt);
Dice dice(u8g2, battery, settings.settingsDice, selectDiceSidesOptions, sizeof(selectDiceSidesOptions)/sizeof(SelectOptionByte));
Coin coin(u8g2, battery, settings.settingsGeneral);
Life life(u8g2, battery, settings.settingsLife);

//====================== SETUP

void setup() {
  // Pin modes
  pinMode(channelA, INPUT_PULLUP);
  pinMode(channelB, INPUT_PULLUP);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(batteryPin, INPUT);
  analogReadResolution(12);

  // Serial communication setup
  Serial.begin(115200);
  /* while (!Serial) {
    ; // Wait for serial port to connect (useful for debug)
  } */

  // Seed random number generator
  seedRnd();

  // U8g2 library init
  u8g2.begin();

  // Set custom splash
  menu.setSplash(logo);

  // Turn inverted order of characters during edit mode on (feels more natural when using encoder)
  menu.invertKeysDuringEdit(true);

  // Init configuration store
  delay(1000);
  Configuration::initConfiguration(settings);

  applyLanguage();
  battery.measure();

  // Menu init, setup and draw
  menu.init();
  setupMenu();
  menu.drawMenu();

  Serial.println("Initialized");
}

void seedRnd() {
  // Not very good solution as Battery level is reletively stable...
  // unsigned long seed = analogRead(RANDOM_SEED_PIN);

  // ...trying multiple reads of lower bits (LSB) instead
  unsigned long seed = 0;

  // Read an analog pin (and internal temperature sensor on RP2040) 32 times and shift the lowest bits into the seed.
  // By default RANDOM_SEED_PIN points to a pin battery is connected to via voltage divider (A3). Not all RP2040 boards
  // have A3 pin exposed (e.g. Raspberry Pi Pico RP2040), so mixing in an internal temperature sensor (available via analogReadTemp()
  // function from Earle Philhower Arduino-Pico core) is probably a good idea
  for (int i = 0; i < 32; i++) {
    seed = seed | ((analogRead(RANDOM_SEED_PIN) & 0x01) << i);
    #if defined(ARDUINO_ARCH_RP2040) || defined(ARDUINO_ARCH_MBED_RP2040) || defined(ARDUINO_ARCH_MBED_NANO)
      if (i % 2 == 0) {
        seed = seed | ((analogRead(RANDOM_SEED_PIN) & 0x01) << i);
      } else {
        seed = seed | (((int)(analogReadTemp() * 10000) & 0x01) << i);
      }
    #else
      seed = seed | ((analogRead(RANDOM_SEED_PIN) & 0x01) << i);
    #endif
    delayMicroseconds(10); // Delay to get different noise samples
  }

  Serial.print(F("Seeding random with: "));
  Serial.println(seed);
  randomSeed(seed);
}

void setupMenu() {
  // Populate GEMAppearance object with general values (that will be used for every menu page if not overridden)
  appearanceGeneral.menuPointerType = settings.settingsGeneral.menuPointer;
  appearanceGeneral.menuItemsPerScreen = GEM_ITEMS_COUNT_AUTO;
  appearanceGeneral.menuItemHeight = 10;
  appearanceGeneral.menuPageScreenTopOffset = 10;
  appearanceGeneral.menuValuesLeftOffset = 92;
  appearanceGeneral.sprites = spritesCustom;

  // Set visibility of dynamic menu items
  setVisibilityOfBatterySettingsItems();

  // Set appearanceGeneral as a general appearance of the menu
  menu.setAppearance(appearanceGeneral);

  // Set callback that will be called when icon is drawn
  menu.setDrawSpriteCallback(drawCustomIcon);

  // Set callback that will be called at the end of drawMenu()
  menu.setDrawMenuCallback(drawStatus);

  // Add menu items to Battery Settings menu page
  menuPageSettingsBattery
    .addMenuItem(menuItemBatteryMonitoring)
    .addMenuItem(menuItemBatteryShowIcon)
    .addMenuItem(menuItemBatteryShowNumber)
    .addMenuItem(menuItemBatteryVoltage);

  // Add menu items to System Settings menu page
  menuPageSettingsSystem
    .addMenuItem(menuItemInfoVerFW)
    .addMenuItem(menuItemInfoVerSettings)
    .addMenuItem(menuItemButtonResetSettings);

  // Add menu items to Settings menu page
  menuPageSettings
    .addMenuItem(menuItemDiceSides)
    .addMenuItem(menuItemLifeStartingValue)
    .addMenuItem(menuItemLanguage)
    .addMenuItem(menuItemMenuPointer)
    .addMenuItem(menuItemSettingsBattery)
    .addMenuItem(menuItemSettingsSystem);

  // Add menu items to Main menu page
  menuPageMain
    .addMenuItem(menuItemMainSettings)
    .addMenuItem(menuItemButtonDice)
    .addMenuItem(menuItemButtonCoin)
    .addMenuItem(menuItemButtonLife);

  // Specify parent menu pages for inner  menu pages
  menuPageSettingsBattery.setParentMenuPage(menuPageSettings);
  menuPageSettingsSystem.setParentMenuPage(menuPageSettings);
  menuPageSettings.setParentMenuPage(menuPageMain);

  // Add menu page to menu and set it as current
  menu.setMenuPageCurrent(menuPageMain);
}

//====================== LOOP

byte detectEncoder() {
  byte btn = GEM_KEY_NONE;

  chanB = digitalRead(channelB); // Reading Channel B signal beforehand to account for possible delays due to polling nature of KeyDetector algorithm
  // ...detect key press using KeyDetector library
  // and pass pressed button to menu
  myKeyDetector.detect();

  switch (myKeyDetector.trigger) {
    case KEY_A:
      // Signal from Channel A of encoder was detected
      if (chanB == LOW) {
        // If channel B is low then the knob was rotated CCW
        if (myKeyDetector.current == KEY_C) {
          // If push-button was pressed at that time, then treat this action as GEM_KEY_LEFT,...
          btn = GEM_KEY_LEFT;
          // Button was in a pressed state during rotation of the knob, acting as a modifier to rotation action
          secondaryPressed = true;
        } else {
          // ...or GEM_KEY_UP otherwise
          btn = GEM_KEY_UP;
        }
      } else {
        // If channel B is high then the knob was rotated CW
        if (myKeyDetector.current == KEY_C) {
          // If push-button was pressed at that time, then treat this action as GEM_KEY_RIGHT,...
          btn = GEM_KEY_RIGHT;
          // Button was in a pressed state during rotation of the knob, acting as a modifier to rotation action
          secondaryPressed = true;
        } else {
          // ...or GEM_KEY_DOWN otherwise
          btn = GEM_KEY_DOWN;
        }
      }
      break;
    case KEY_C:
      // Button was pressed
      // Save current time as a time of the key press event
      keyPressTime = now;
      break;
  }
  switch (myKeyDetector.triggerRelease) {
    case KEY_C:
      // Button was released
      if (!secondaryPressed) {
        // If button was not used as a modifier to rotation action...
        if (now <= keyPressTime + keyPressDelay) {
          // ...and if not enough time passed since keyPressTime,
          // treat key that was pressed as Ok button
          btn = GEM_KEY_OK;
        }
      }
      secondaryPressed = false;
      cancelPressed = false;
      break;
  }
  // After keyPressDelay passed since keyPressTime
  if (now > keyPressTime + keyPressDelay) {
    switch (myKeyDetector.current) {
      case KEY_C:
        if (!secondaryPressed && !cancelPressed) {
          // If button was not used as a modifier to rotation action, and Cancel action was not triggered yet
          // Treat key that was pressed as Cancel button
          btn = GEM_KEY_CANCEL;
          cancelPressed = true;
        }
        break;
    }
  }
  return btn;
}

void loop() {
  // Get current time to use later on
  now = millis();

  // If menu is ready to accept button press...
  if (menu.readyForKey()) {
    byte key = detectEncoder();
    menu.registerKeyPress(key);
  }

  battery.manage();
}

//====================== ROUTINES

//--- Save callbacks

void switchFonts(byte lang) {
  if (lang == NO_DICE_LANG_RU) {
    menu.enableCyrillic();
  } else {
    menu
      .enableCyrillic(false)
      .enableUTF8() // enableCyrillyc(false) disables UTF8 internally, so we should reenable it explicitly here, and load custom fonts with extended glyph set afterwards
      .setFontSmall(NO_DICE_FONT_SMALL, 4, 6)
      .setFontBig(NO_DICE_FONT_BIG, 6, 8);
  }
}

void applyLanguage() {
  byte lang = settings.settingsGeneral.language;

  switchFonts(lang);

  // Main menu page
  menuItemMainSettings.setTitle(menuItemMainSettingsTitle[lang]);
  menuItemButtonDice.setTitle(menuItemButtonDiceTitle[lang]);
  menuItemButtonCoin.setTitle(menuItemButtonCoinTitle[lang]);
  menuItemButtonLife.setTitle(menuItemButtonLifeTitle[lang]);

  // Settings menu page
  menuPageSettings.setTitle(menuPageSettingsTitle[lang]);
  menuItemDiceSides.setTitle(menuItemDiceSidesTitle[lang]);
  menuItemLifeStartingValue.setTitle(menuItemLifeStartingValueTitle[lang]);
  menuItemLanguage.setTitle(menuItemLanguageTitle[lang]);
  menuItemMenuPointer.setTitle(menuItemMenuPointerTitle[lang]);
  selectMenuPointer.setOptions(selectMenuPointerOptions[lang]);
  menuItemSettingsBattery.setTitle(menuItemSettingsBatteryTitle[lang]);
  menuItemSettingsSystem.setTitle(menuItemSettingsSystemTitle[lang]);

  // Battery Settings menu page
  menuPageSettingsBattery.setTitle(menuPageSettingsBatteryTitle[lang]);
  menuItemBatteryMonitoring.setTitle(menuItemBatteryMonitoringTitle[lang]);
  selectBatteryMonitoring.setOptions(selectBatteryMonitoringOptions[lang]);
  menuItemBatteryShowIcon.setTitle(menuItemBatteryShowIconTitle[lang]);
  menuItemBatteryShowNumber.setTitle(menuItemBatteryShowNumberTitle[lang]);
  menuItemBatteryVoltage.setTitle(menuItemBatteryVoltageTitle[lang]);

  // System Settings menu page
  menuPageSettingsSystem.setTitle(menuPageSettingsSystemTitle[lang]);
  menuItemInfoVerFW.setTitle(menuItemInfoVerFWTitle[lang]);
  menuItemInfoVerSettings.setTitle(menuItemInfoVerSettingsTitle[lang]);
  menuItemButtonResetSettings.setTitle(menuItemButtonResetSettingsTitle[lang]);

  // Print language to Serial
  Serial.print(F("Language applied: "));
  Serial.println(selectLanguage.getSelectedOptionName(&settings.settingsGeneral.language));

  saveSettings();
}

void applyMenuPointer() {
  appearanceGeneral.menuPointerType = settings.settingsGeneral.menuPointer;
  menu.setAppearance(appearanceGeneral); // Need to call setAppearance() when changing general appearance

  // Print settings.settingsGeneral.menuPointer value to Serial
  Serial.print(F("Menu pointer set: "));
  Serial.println(settings.settingsGeneral.menuPointer);

  saveSettings();
}

//--- Dice button action and context routines

void launchDice(GEMCallbackData callbackData) {
  Serial.println(F("Throw a dice started"));
  menu.context.loop = diceContextLoop;
  menu.context.enter = diceContextEnter;
  menu.context.exit = diceContextExit;
  menu.context.allowExit = false; // Setting to false will require manual exit from the loop
  menu.context.enter();
}

void diceContextEnter() {
  dice.enter();
}

// Invoked every loop iteration
void diceContextLoop() {
  byte key = detectEncoder();
  if (key == GEM_KEY_CANCEL) {
    // Exit animation routine if GEM_KEY_CANCEL key was pressed
    menu.context.exit();
  } else {
    dice.repeat(key);
  }
}

// Invoked once when the GEM_KEY_CANCEL action is triggered
void diceContextExit() {
  dice.exit();

  // Draw menu back on screen and clear context
  menu.reInit();
  menu.drawMenu();
  menu.clearContext();
}

//--- Coin button action and context routines

void launchCoin(GEMCallbackData callbackData) {
  Serial.println(F("Flip a coin started"));
  menu.context.loop = coinContextLoop;
  menu.context.enter = coinContextEnter;
  menu.context.exit = coinContextExit;
  menu.context.allowExit = false; // Setting to false will require manual exit from the loop
  menu.context.enter();
}

void coinContextEnter() {
  coin.enter();
}

// Invoked every loop iteration
void coinContextLoop() {
  byte key = detectEncoder();
  if (key == GEM_KEY_CANCEL) {
    // Exit animation routine if GEM_KEY_CANCEL key was pressed
    menu.context.exit();
  } else {
    coin.repeat(key);
  }
}

// Invoked once when the GEM_KEY_CANCEL action is triggered
void coinContextExit() {
  coin.exit();

  // Draw menu back on screen and clear context
  menu.reInit();
  menu.drawMenu();
  menu.clearContext();
}

//--- Life button action and context routines

void launchLife(GEMCallbackData callbackData) {
  Serial.println(F("Life counter started"));
  menu.context.loop = lifeContextLoop;
  menu.context.enter = lifeContextEnter;
  menu.context.exit = lifeContextExit;
  menu.context.allowExit = false; // Setting to false will require manual exit from the loop
  menu.context.enter();
}

void lifeContextEnter() {
  life.enter();
}

// Invoked every loop iteration
void lifeContextLoop() {
  byte key = detectEncoder();
  if (key == GEM_KEY_CANCEL) {
    // Exit animation routine if GEM_KEY_CANCEL key was pressed
    menu.context.exit();
  } else {
    life.repeat(key);
  }
}

// Invoked once when the GEM_KEY_CANCEL action is triggered
void lifeContextExit() {
  life.exit();

  // Draw menu back on screen and clear context
  menu.reInit();
  menu.drawMenu();
  menu.clearContext();
}

//--- Other buttons actions

void saveSettings() {
  Configuration::saveConfiguration(settings);
}

void resetSettings() {
  GEMItem* menuItemButtonTmp = menu.getCurrentMenuPage()->getCurrentMenuItem();
  const char* menuItemButtonTmpTitle[] = {"Rebooting...", "Перезагрузка...", "Neustart...", "Redémarrage..."};
  menuItemButtonTmp->setTitle(menuItemButtonTmpTitle[settings.settingsGeneral.language]);
  menu.drawMenu();
  Configuration::resetConfiguration();
  // In case software reset is not supported or didn't work, ask user to restart manually
  const char* menuItemButtonTmpManualTitle[] = {"Reboot manually!", "Перезагрузитесь!", "Jetzt neu starten!", "Veuillez redémarrer!"};
  menuItemButtonTmp->setTitle(menuItemButtonTmpManualTitle[settings.settingsGeneral.language]);
  menu.drawMenu();
  delay(3600000);
}

//--- Battery settings callbacks

void setVisibilityOfBatterySettingsItems() {
  menuItemBatteryShowIcon.hide(settings.settingsBattery.enableMonitoring == 0);
  menuItemBatteryShowNumber.hide(settings.settingsBattery.enableMonitoring == 0);
  menuItemBatteryVoltage.hide(settings.settingsBattery.enableMonitoring == 0);
}

void enableBatteryMonitoringCallback() {
  setVisibilityOfBatterySettingsItems();
  saveSettings();
}

//--- Draw callbacks

void drawStatus() {
  if (settings.settingsBattery.enableMonitoring != 0) {
    battery.draw(false);  // Draw battery status w/o sending u8g2 buffer to screen (screen will be updated internally in GEM loop)
  }
}

bool drawCustomIcon(u8g2_uint_t x, u8g2_uint_t y, byte spriteId, GEMItem* menuItem) {
  // Validity check: we only want to override an icon of a specific buttons...
  if (spriteId == GEM_ICON_ARROW_BTN && menuItem != nullptr) {
    const byte buttonId = menuItem->getCallbackData().valByte;

    // ...and that button has an ID attached to it as a part of GEMCallbackData object
    switch (buttonId) {
        case DICE_BUTTON_ID:
          u8g2.drawXBMP(x-1, y, arrowBtn_dice.width, arrowBtn_dice.height, arrowBtn_dice.image);
          return false;
          break;
        case COIN_BUTTON_ID:
          u8g2.drawXBMP(x-1, y, arrowBtn_coin.width, arrowBtn_coin.height, arrowBtn_coin.image);
          return false;
          break;
        case LIFE_BUTTON_ID:
          u8g2.drawXBMP(x-1, y, arrowBtn_life.width, arrowBtn_life.height, arrowBtn_life.image);
          return false;
          break;
        default:
          break;
    }
  }

  // Return true to draw default icon
  return true;
}
