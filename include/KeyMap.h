/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2024 Sky UK
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef OTTO_KEYMAP_H
#define OTTO_KEYMAP_H

#include <string>
#include <unordered_map>

// Key Event Types
#ifdef ENABLE_UINPUT
enum KeyEventType { KET_KEYDOWN = 1, KET_KEYUP = 0, KET_KEYREPEAT = 2 };

enum KeyCodes {
    KEY_DIGIT0 = 11,
    KEY_DIGIT1 = 2,
    KEY_DIGIT2 = 3,
    KEY_DIGIT3 = 4,
    KEY_DIGIT4 = 5,
    KEY_DIGIT5 = 6,
    KEY_DIGIT6 = 7,
    KEY_DIGIT7 = 8,
    KEY_DIGIT8 = 9,
    KEY_DIGIT9 = 10,
    KEY_POWER = 116,
    KEY_GUIDE = 102,
    KEY_ARROWUP = 103,
    KEY_ARROWDOWN = 108,
    KEY_ARROWLEFT = 105,
    KEY_ARROWRIGHT = 106,
    KEY_SELECT = 28,
    KEY_VOLUMEUP = 78,
    KEY_VOLUMEDOWN = 74,
    KEY_MUTE = 55,
    KEY_PLAY = 207,
    KEY_PAUSE = 119,
    KEY_STOP = 128,
    KEY_RECORD = 63,
    KEY_EXIT = 1,
    KEY_INFO = 358,
    KEY_A = 62,
    KEY_B = 110,
    KEY_C = 107,
    KEY_D = 111,
    KEY_INPUTKEY = 67,
    KEY_HELP = 60
};
#else
enum KeyEventType { KET_KEYDOWN = 0x00008000UL, KET_KEYUP = 0x00008100UL, KET_KEYREPEAT = 0x00008200UL };

enum KeyCodes {
    KEY_DIGIT0 = 0x00000030UL,
    KEY_DIGIT1 = 0x00000031UL,
    KEY_DIGIT2 = 0x00000032UL,
    KEY_DIGIT3 = 0x00000033UL,
    KEY_DIGIT4 = 0x00000034UL,
    KEY_DIGIT5 = 0x00000035UL,
    KEY_DIGIT6 = 0x00000036UL,
    KEY_DIGIT7 = 0x00000037UL,
    KEY_DIGIT8 = 0x00000038UL,
    KEY_DIGIT9 = 0x00000039UL,
    KEY_POWER = 0x00000080UL,
    KEY_MENU = 0x000000C0UL,
    KEY_GUIDE = 0x0000008DUL,
    KEY_ARROWUP = 0x00000081UL,
    KEY_ARROWDOWN = 0x00000082UL,
    KEY_ARROWLEFT = 0x00000083UL,
    KEY_ARROWRIGHT = 0x00000084UL,
    KEY_SELECT = 0x00000085UL,
    KEY_VOLUMEUP = 0x0000008AUL,
    KEY_VOLUMEDOWN = 0x0000008BUL,
    KEY_MUTE = 0x0000008CUL,
    KEY_PLAY = 0x00000099UL,
    KEY_PAUSE = 0x0000009BUL,
    KEY_STOP = 0x0000009AUL,
    KEY_RECORD = 0x0000009CUL,
    KEY_EXIT = 0x00000087UL,
    KEY_INFO = 0x0000008EUL,
    KEY_KEYA = 0x00000092UL,
    KEY_KEYB = 0x00000093UL,
    KEY_KEYC = 0x00000094UL,
    KEY_KEYD = 0x0000009FUL,
    KEY_INPUTKEY = 0x000000D0UL,
    KEY_HELP = 0x000000A1UL
};
#endif

/**
 * The KeyMap class manages the mapping of command strings to key codes.
 */
class KeyMap {
public:
    /**
     * Initializes the key map with default mappings.
     */
    KeyMap();

    /**
     * Gets the key code corresponding to a given command.
     *
     * @param command The command string (e.g., "power", "mute").
     * @return The corresponding key code, or -1 if the command is not found.
     */
    int getKeyCode(const std::string &command) const;

    /**
     * Gets the command string corresponding to a given key code.
     *
     * @param keyCode The key code.
     * @return The corresponding command string, or an empty string if the key code is not found.
     */
    std::string getKeyName(int keyCode) const;

    /**
     * Adds or updates a command-keyCode mapping.
     *
     * @param command The command string.
     * @param keyCode The key code to associate with the command.
     */
    void addMapping(const std::string &command, int keyCode);

private:
    std::unordered_map<std::string, int> keyMappings;
    void loadDefaultMappings();
};

#endif // OTTO_KEYMAP_H
