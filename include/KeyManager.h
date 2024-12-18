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

#ifndef OTTO_KEYMANAGER_H
#define OTTO_KEYMANAGER_H

#include "KeyMap.h"

#include <memory>
#include <vector>

/**
 * KeyManager manages key events and integrates with IARM or uinput for event dispatching.
 */
class KeyManager {
public:
    /**
     * Constructs a KeyManager with a default KeyMap.
     */
    KeyManager(int intervalMs = 50);

    ~KeyManager();

    /**
     * Sends a key press event with optional repeat count and interval.
     *
     * @param key The key name (e.g., "power", "volup").
     * @param repeat The number of times to repeat the key press (default: 1).
     */
    void sendKeyPress(const std::string &key, int repeat = 1);

    /**
     * Sends a key release event.
     *
     * @param key The key name (e.g., "power", "volup").
     */
    void sendKeyRelease(const std::string &key);

    /**
     * Sends a key hold event for a specified duration.
     *
     * @param key The key name (e.g., "power", "volup").
     * @param durationMs The duration to hold the key in milliseconds.
     */
    void sendKeyHold(const std::string &key, int durationMs);

    /**
     * Starts recording key events.
     *
     * @param outputFile The file to save the recorded events.
     */
    void startRecording(const std::string &outputFile);

    /**
     * Stops the ongoing recording and saves the events to the file.
     */
    void stopRecording();

private:
    int intervalMs;
    KeyMap keyMap;

    /**
     * Sends a key event to IRMGR via uinput dispatcher.
     *
     * @param keyType The type of the key event (e.g., press, release, repeat).
     * @param keyCode The key code of the event.
     */
    void sendEvent(int keyType, int keyCode);
};

#endif // OTTO_KEYMANAGER_H
