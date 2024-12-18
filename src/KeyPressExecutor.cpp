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

#include "KeyPressExecutor.h"
#include "Logger.h"

KeyPressExecutor::KeyPressExecutor(KeyManager &km) : keyManager(km) {}

void KeyPressExecutor::execute(const std::vector<std::string> &args) {
    if (args.size() < 2) {
        logError("Invalid key_press command format. Usage: key_press <key> [repeat]");
        return;
    }

    const std::string &key = args[1];
    int repeat = 1;

    if (args.size() > 2) {
        try {
            repeat = std::stoi(args[2]);
            if (repeat < 1) {
                logWarn("Repeat count must be at least 1. Using default value: 1.");
                repeat = 1;
            }
        } catch (const std::exception &e) {
            logWarn("Invalid repeat count. Using default value: 1.");
        }
    }

    logDebug("Sending key press: " + key + ", repeat: " + std::to_string(repeat));

    keyManager.sendKeyPress(key, repeat);
}
