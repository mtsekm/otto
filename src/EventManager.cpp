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

#include "EventManager.h"
#include "Logger.h"

#include <algorithm>
#include <chrono>
#include <cstring>
#include <dirent.h>
#include <fcntl.h>
#include <fstream>
#include <linux/input.h>
#include <poll.h>
#include <stdexcept>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#ifdef ENABLE_UINPUT
#include <linux/uinput.h>
#else
#include "IARMUtils.h"
#endif

EventManager::EventManager() {
    logDebug("EventManager constructor");
#ifdef ENABLE_UINPUT
    setupUInput();
#else
    IARMUtils::initialize("OTTO");
#endif
}

EventManager::~EventManager() {
    stopRecording();
#ifdef ENABLE_UINPUT
    cleanupUInput();
#else
    IARMUtils::terminate();
#endif
}

EventManager &EventManager::getInstance() {
    static EventManager instance;
    return instance;
}

void EventManager::sendEvent(int keyType, int keyCode) {
#ifdef ENABLE_UINPUT
    sendUInputEvent(keyType, keyCode);
#else
    sendLibUInputEvent(keyType, keyCode);
#endif
}

void EventManager::startRecording(const std::string &outputFile) {
    if (isRecording) {
        logWarn("Recording already in progress.");
        return;
    }

    recordFilePath = outputFile;
    recordedEvents.clear();
    isRecording = true;

#ifdef ENABLE_UINPUT
    discoverInputDevices();

    isEvdevRecording = true;
    evdevRecordingThread = std::thread(&EventManager::evdevRecordingLoop, this);
#else
    IARMUtils::registerIRKeyHandler([this](int keyType, int keyCode) { handleEvent(keyType, keyCode); });
#endif

    logInfo("Started recording key events to: " + outputFile);
}

void EventManager::stopRecording() {
    if (!isRecording) {
        logDebug("No recording in progress.");
        return;
    }

    isRecording = false;
#ifdef ENABLE_UINPUT
    stopEvdevThread();
#endif

    writeRecordedEventsToFile();

    logInfo("Stopped recording. Events saved to: " + recordFilePath);
}

void EventManager::writeRecordedEventsToFile() {
    std::lock_guard<std::mutex> lock(recordingMutex);
    std::ofstream outFile(recordFilePath);
    if (!outFile.is_open()) {
        logError("Failed to open file for writing: " + recordFilePath);
        return;
    }

    for (const auto &event : recordedEvents) {
        outFile << event << "\n";
    }

    outFile.close();
    logInfo("Recorded events written to file: " + recordFilePath);
}

void EventManager::handleEvent(int keyType, int keyCode) {
    if (!isRecording) {
        logWarn("No recording in progress.");
        return;
    }

    // Log the received event
    logDebug("Received event: keyType=" + std::to_string(keyType) + ", keyCode=" + std::to_string(keyCode));

    static thread_local int lastKeyType = -1;
    static thread_local int lastKeyCode = -1;
    static thread_local std::chrono::steady_clock::time_point lastTimestamp;

    auto now = std::chrono::steady_clock::now();
    const int duplicateThresholdMs = 200;

    // Avoid duplicate key presses within the threshold
    if (keyType == lastKeyType && keyCode == lastKeyCode) {
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTimestamp).count();
        if (elapsed < duplicateThresholdMs && keyType == KET_KEYDOWN) {
            logDebug("Filtered duplicate event: keyCode=" + std::to_string(keyCode) +
                     ", elapsed=" + std::to_string(elapsed) + "ms");
            return;
        }
    }

    lastKeyType = keyType;
    lastKeyCode = keyCode;
    lastTimestamp = now;

    // Only process key down events
    if (keyType != static_cast<int>(KET_KEYDOWN)) {
        logDebug("Ignoring non-keydown event: keyType=" + std::to_string(keyType));
        logDebug("Debug: keyType=" + std::to_string(keyType) + ", KET_KEYDOWN=" + std::to_string(KET_KEYDOWN));
        return;
    }

    // Get the key name from the key code
    std::string keyName = keyMap.getKeyName(keyCode);
    if (keyName.empty()) {
        logWarn("Unknown key code: " + std::to_string(keyCode));
        keyName = "KEY_UNKNOWN_" + std::to_string(keyCode);
    }

    // Create the command string
    std::string command = "key_press " + keyName;

    // Add the command to the recorded events
    {
        std::lock_guard<std::mutex> lock(recordingMutex);
        recordedEvents.push_back(command);
    }

    logInfo("Recorded key event: " + command);
}

#ifdef ENABLE_UINPUT
void EventManager::discoverInputDevices() {
    std::map<int, std::string> tempDevices;

    DIR *dir = opendir("/dev/input");
    if (!dir) {
        logError("Failed to open /dev/input directory.");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != nullptr) {
        if (strncmp(entry->d_name, "event", 5) == 0) {
            std::string devicePath = "/dev/input/" + std::string(entry->d_name);
            int fd = open(devicePath.c_str(), O_RDONLY | O_NONBLOCK);
            if (fd >= 0) {
                tempDevices[fd] = devicePath;
                logInfo("Discovered input device: " + devicePath);
            } else {
                logWarn("Failed to open input device: " + devicePath);
            }
        }
    }
    closedir(dir);

    if (tempDevices.empty()) {
        logError("No usable input devices found in /dev/input.");
        return;
    }

    std::lock_guard<std::mutex> lock(recordingMutex);
    evdevDevices = std::move(tempDevices);
}

void EventManager::stopEvdevThread() {
    isEvdevRecording = false;

    if (evdevRecordingThread.joinable()) {
        evdevRecordingThread.join();
    }

    std::lock_guard<std::mutex> lock(recordingMutex);
    for (const auto &[fd, device] : evdevDevices) {
        close(fd);
    }

    evdevDevices.clear();
    logInfo("Evdev recording thread terminated.");
}

void EventManager::evdevRecordingLoop() {
    std::vector<struct pollfd> fds;
    {
        std::lock_guard<std::mutex> lock(recordingMutex);
        for (const auto &[fd, device] : evdevDevices) {
            logDebug("Adding device to poll set: " + device + " (fd: " + std::to_string(fd) + ")");
            fds.push_back({fd, POLLIN, 0});
        }
    }

    if (fds.empty()) {
        logError("No input devices found for polling.");
        return;
    }

    struct input_event ev;

    while (isEvdevRecording) {
        logDebug("Polling for events...");
        int pollResult = poll(fds.data(), fds.size(), 500);

        if (pollResult < 0) {
            logError("Poll failed during evdev recording: " + std::string(strerror(errno)));
            break;
        } else if (pollResult == 0) {
            logDebug("Poll timed out. No events received.");
            continue;
        }

        for (auto &fdStruct : fds) {
            if (fdStruct.revents & POLLIN) {
                logDebug("Event ready on fd: " + std::to_string(fdStruct.fd));

                ssize_t bytesRead = read(fdStruct.fd, &ev, sizeof(ev));
                if (bytesRead < 0) {
                    logError("Read failed on fd: " + std::to_string(fdStruct.fd) +
                             ", error: " + std::string(strerror(errno)));
                    continue;
                }

                if (bytesRead != sizeof(ev)) {
                    logWarn("Incomplete event read on fd: " + std::to_string(fdStruct.fd));
                    continue;
                }

                logDebug("Event received: type=" + std::to_string(ev.type) + ", code=" + std::to_string(ev.code) +
                         ", value=" + std::to_string(ev.value));

                if (ev.type == EV_KEY) {
                    logDebug("Key event: code=" + std::to_string(ev.code) + ", value=" + std::to_string(ev.value));
                    handleEvent(ev.value, ev.code);
                } else {
                    logDebug("Non-key event ignored: type=" + std::to_string(ev.type));
                }
            } else if (fdStruct.revents & (POLLERR | POLLHUP)) {
                logWarn("Device disconnected (fd: " + std::to_string(fdStruct.fd) + "). Removing from poll set.");
                close(fdStruct.fd);
                fdStruct.fd = -1;
            }
        }

        // Remove invalid file descriptors from the poll set
        auto oldSize = fds.size();
        fds.erase(std::remove_if(fds.begin(), fds.end(), [](const pollfd &pfd) { return pfd.fd == -1; }), fds.end());

        if (fds.size() != oldSize) {
            logInfo("Removed disconnected devices. Remaining devices: " + std::to_string(fds.size()));
        }
    }

    logInfo("Evdev recording loop terminated.");
}

void EventManager::setupUInput() {
    uinputFd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if (uinputFd < 0) {
        throw std::runtime_error("Failed to open /dev/uinput");
    }

    ioctl(uinputFd, UI_SET_EVBIT, EV_KEY);
    ioctl(uinputFd, UI_SET_EVBIT, EV_SYN);

    for (int i = 0; i < 256; ++i) {
        if (ioctl(uinputFd, UI_SET_KEYBIT, i) < 0) {
            close(uinputFd); // Ensure immediate cleanup
            throw std::runtime_error("Failed to configure key bit for uinput");
        }
    }

    struct uinput_setup setup {};
    setup.id.bustype = BUS_USB;
    setup.id.vendor = 0x1234;
    setup.id.product = 0x5678;
    strncpy(setup.name, "OttoUInput", sizeof(setup.name) - 1);
    setup.name[sizeof(setup.name) - 1] = '\0';

    if (ioctl(uinputFd, UI_DEV_SETUP, &setup) < 0 || ioctl(uinputFd, UI_DEV_CREATE) < 0) {
        close(uinputFd);
        throw std::runtime_error("Failed to setup uinput device");
    }

    logInfo("UInput setup completed.");
}

void EventManager::cleanupUInput() {
    if (uinputFd >= 0) {
        ioctl(uinputFd, UI_DEV_DESTROY);
        close(uinputFd);
        uinputFd = -1;
        logInfo("UInput device cleaned up.");
    }
}

void EventManager::sendUInputEvent(int keyType, int keyCode) {
    struct input_event ev;

    // MSC_SCAN: Send the scancode of the key
    memset(&ev, 0, sizeof(ev));
    ev.type = EV_MSC;
    ev.code = MSC_SCAN;
    ev.value = keyCode; // Use the key code as the scan code
    if (write(uinputFd, &ev, sizeof(ev)) < 0) {
        logError("Failed to send MSC_SCAN event.");
        return;
    }

    // EV_KEY: Send the key press or release event
    memset(&ev, 0, sizeof(ev));
    ev.type = EV_KEY;
    ev.code = keyCode;
    ev.value = (keyType == KET_KEYDOWN) ? 1 : 0;
    if (write(uinputFd, &ev, sizeof(ev)) < 0) {
        logError("Failed to send EV_KEY event.");
        return;
    }

    // EV_SYN: Synchronize the event
    memset(&ev, 0, sizeof(ev));
    ev.type = EV_SYN;
    ev.code = SYN_REPORT;
    ev.value = 0;
    if (write(uinputFd, &ev, sizeof(ev)) < 0) {
        logError("Failed to send SYN_REPORT event.");
        return;
    }

    logDebug("Event sent: KeyCode = " + std::to_string(keyCode) + ", KeyType = " + std::to_string(keyType));
}
#else
void EventManager::sendLibUInputEvent(int keyType, int keyCode) { IARMUtils::sendKeyEvent(keyType, keyCode); }
#endif
