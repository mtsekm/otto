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

#ifndef OTTO_EVENTMANAGER_H
#define OTTO_EVENTMANAGER_H

#include "KeyMap.h"

#include <atomic>
#include <map>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

/**
 * EventManager handles sending and recording key events using uinput/evdev (direct) or libuinput (via IARMUtils).
 */
class EventManager {
public:
    static EventManager &getInstance();

    /**
     * Sends a key event.
     *
     * @param keyType The type of the key event, press or release.
     * @param keyCode The key code to send.
     */
    void sendEvent(int keyType, int keyCode);

    /**
     * Starts recording key events to a specified file.
     *
     * @param outputFile The path to the output file for recording.
     */
    void startRecording(const std::string &outputFile);

    /**
     * Stops the ongoing recording and saves events to the file.
     */
    void stopRecording();

    /**
     * Handles incoming events for recording.
     *
     * @param keyType The type of the key event.
     * @param keyCode The key code of the event.
     */
    void handleEvent(int keyType, int keyCode);

private:
    EventManager();
    ~EventManager();

    EventManager(const EventManager &) = delete;
    EventManager &operator=(const EventManager &) = delete;

    void writeRecordedEventsToFile();

#ifdef ENABLE_UINPUT
    void sendUInputEvent(int keyType, int keyCode);
    void setupUInput();
    void cleanupUInput();
    void discoverInputDevices();
    void stopEvdevThread();
    void evdevRecordingLoop();

    int uinputFd = -1;
    std::atomic<bool> isEvdevRecording{false};
    std::map<int, std::string> evdevDevices;
    std::thread evdevRecordingThread;
#else
    void sendLibUInputEvent(int keyType, int keyCode);
#endif

    std::atomic<bool> isRecording{false};
    std::string recordFilePath;
    std::vector<std::string> recordedEvents;
    std::mutex recordingMutex;
    KeyMap keyMap;
};

#endif // OTTO_EVENTMANAGER_H
