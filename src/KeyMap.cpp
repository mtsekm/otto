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

#include "KeyMap.h"
#include "Logger.h"

KeyMap::KeyMap() { loadDefaultMappings(); }

void KeyMap::loadDefaultMappings() {
    keyMappings = {
        {"0", KEY_DIGIT0},       {"1", KEY_DIGIT1},           {"2", KEY_DIGIT2},      {"3", KEY_DIGIT3},
        {"4", KEY_DIGIT4},       {"5", KEY_DIGIT5},           {"6", KEY_DIGIT6},      {"7", KEY_DIGIT7},
        {"8", KEY_DIGIT8},       {"9", KEY_DIGIT9},           {"up", KEY_ARROWUP},    {"down", KEY_ARROWDOWN},
        {"left", KEY_ARROWLEFT}, {"right", KEY_ARROWRIGHT},   {"enter", KEY_SELECT},  {"mute", KEY_MUTE},
        {"volup", KEY_VOLUMEUP}, {"voldown", KEY_VOLUMEDOWN}, {"play", KEY_PLAY},     {"pause", KEY_PAUSE},
        {"stop", KEY_STOP},      {"exit", KEY_EXIT},          {"info", KEY_INFO},     {"red", KEY_C},
        {"green", KEY_D},        {"yellow", KEY_A},           {"blue", KEY_B},        {"power", KEY_POWER},
        {"home", KEY_GUIDE},     {"settings", KEY_HELP},      {"record", KEY_RECORD}, {"input", KEY_INPUTKEY}};

    logDebug("Default key mappings loaded.");
}

int KeyMap::getKeyCode(const std::string &command) const {
    auto it = keyMappings.find(command);
    if (it != keyMappings.end()) {
        return it->second;
    } else {
        logWarn("Command not found in key map: " + command);
        return -1;
    }
}

std::string KeyMap::getKeyName(int keyCode) const {
    for (const auto &pair : keyMappings) {
        if (pair.second == keyCode) {
            return pair.first;
        }
    }
    logWarn("Key code not found in key map: " + std::to_string(keyCode));
    return "";
}

void KeyMap::addMapping(const std::string &command, int keyCode) {
    keyMappings[command] = keyCode;
    logDebug("Added/Updated mapping: " + command + " -> " + std::to_string(keyCode));
}
