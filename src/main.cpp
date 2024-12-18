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

#include "AppExecutor.h"
#include "CommandExecutor.h"
#include "CommandHandler.h"
#include "KeyManager.h"
#include "KeyPressExecutor.h"
#include "Logger.h"
#include "LoopExecutor.h"
#include "VariableExecutor.h"
#include "WaitExecutor.h"

#include <atomic>
#include <csignal>
#include <iostream>
#include <sstream>
#include <thread>

std::atomic<bool> isRecordingActive(true);

void handleSignal(int signal) {
    if (signal == SIGINT) {
        isRecordingActive = false;
    }
}

void printUsage() {
    std::cout << "Usage: ./otto [options]\n"
              << "Options:\n"
              << "  <commands_file>: Path to the commands file for execution.\n"
              << "  --intervalMs=<value>: (Optional) Interval between key presses in milliseconds. Default: 100ms.\n"
              << "  --logLevel=<level>: (Optional) Logging level. Values: DEBUG, INFO, WARN, ERROR. Default: INFO.\n"
              << "  --record=<output_file>: (Optional) Start in record mode and save events to a file.\n";
}

int main(int argc, char *argv[]) {
    if (argc < 2 || argc > 4) {
        printUsage();
        return 1;
    }

    std::string commandsFile;
    std::string recordFile;
    int intervalMs = 100;

    // Parse command-line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg.find("--intervalMs=") == 0) {
            std::istringstream iss(arg.substr(13));
            if (!(iss >> intervalMs) || intervalMs < 0) {
                logError("Invalid value for --intervalMs. Must be a non-negative integer.");
                return 1;
            }
            logInfo("Interval between key presses set to " + std::to_string(intervalMs) + "ms.");
        } else if (arg.find("--logLevel=") == 0) {
            try {
                LoggerConfig::setLogLevel(stringToLogLevel(arg.substr(11)));
            } catch (const std::exception &e) {
                logError(e.what());
                return 1;
            }
        } else if (arg.find("--record=") == 0) {
            recordFile = arg.substr(9);
            logInfo("Record mode enabled. Output file: " + recordFile);
        } else {
            commandsFile = arg;
        }
    }

    KeyManager keyManager(intervalMs);

    // Record mode
    if (!recordFile.empty()) {
        std::signal(SIGINT, handleSignal);
        try {
            keyManager.startRecording(recordFile);
            std::cout << "\n\nRecording IR key events. Press Ctrl+C to stop.\n\n" << std::endl;
            while (isRecordingActive) {
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }
            keyManager.stopRecording();
            return 0;
        } catch (const std::exception &e) {
            logError("Error during recording: " + std::string(e.what()));
            return 1;
        }
    }

    if (commandsFile.empty()) {
        printUsage();
        return 1;
    }

    // Command execution mode
    logInfo("Starting in execution mode with commands file: " + commandsFile);

    std::unordered_map<std::string, std::string> variables;
    auto commandExecutor = std::make_shared<CommandExecutor>(variables);

    commandExecutor->registerCommand("var", std::make_shared<VariableExecutor>(variables));
    commandExecutor->registerCommand("key_press", std::make_shared<KeyPressExecutor>(keyManager));

    auto loopExecutor = std::make_shared<LoopExecutor>(commandExecutor);
    commandExecutor->registerCommand("loop_start", loopExecutor);
    commandExecutor->registerCommand("loop_end", loopExecutor);

    auto appExecutor = std::make_shared<AppExecutor>();
    commandExecutor->registerCommand("launch_app", appExecutor);
    commandExecutor->registerCommand("close_app", appExecutor);

    commandExecutor->registerCommand("wait", std::make_shared<WaitExecutor>());

    CommandHandler commandHandler(commandExecutor);

    try {
        commandHandler.parseFile(commandsFile);
        commandHandler.executeCommands();
    } catch (const std::exception &e) {
        logError("Error during execution: " + std::string(e.what()));
        return 1;
    }

    return 0;
}
