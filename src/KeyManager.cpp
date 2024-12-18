#include "KeyManager.h"
#include "EventManager.h"
#include "Logger.h"

#include <chrono>
#include <thread>

KeyManager::KeyManager(int intervalMs) : intervalMs(intervalMs), keyMap() {}

KeyManager::~KeyManager() {
    stopRecording();
    logDebug("Terminating KeyManager...");
}

void KeyManager::sendKeyPress(const std::string &key, int repeat) {
    int keyCode = keyMap.getKeyCode(key);
    if (keyCode == -1) {
        logError("Invalid key: " + key);
        return;
    }

    for (int i = 0; i < repeat; ++i) {
        sendEvent(KET_KEYDOWN, keyCode);
        std::this_thread::sleep_for(std::chrono::milliseconds(intervalMs));
        sendEvent(KET_KEYUP, keyCode);
    }

    logDebug("Sent key press: " + key + " (repeat: " + std::to_string(repeat) +
             ", interval: " + std::to_string(intervalMs) + "ms)");
}

void KeyManager::sendKeyRelease(const std::string &key) {
    int keyCode = keyMap.getKeyCode(key);
    if (keyCode == -1) {
        logError("Invalid key: " + key);
        return;
    }

    sendEvent(KET_KEYUP, keyCode);
    logDebug("Sent key release: " + key);
}

void KeyManager::sendKeyHold(const std::string &key, int durationMs) {
    int keyCode = keyMap.getKeyCode(key);
    if (keyCode == -1) {
        logError("Invalid key: " + key);
        return;
    }

    sendEvent(KET_KEYDOWN, keyCode);
    std::this_thread::sleep_for(std::chrono::milliseconds(durationMs));
    sendEvent(KET_KEYUP, keyCode);

    logDebug("Sent key hold: " + key + " (duration: " + std::to_string(durationMs) + "ms)");
}

void KeyManager::sendEvent(int keyType, int keyCode) { EventManager::getInstance().sendEvent(keyType, keyCode); }

void KeyManager::startRecording(const std::string &outputFile) {
    EventManager::getInstance().startRecording(outputFile);
}

void KeyManager::stopRecording() { EventManager::getInstance().stopRecording(); }
