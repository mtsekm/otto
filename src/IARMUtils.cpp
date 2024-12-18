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

#include "IARMUtils.h"
#include "Logger.h"

#include <irMgr.h>
#include <irMgrInternal.h>
#include <libIBus.h>

#include <atomic>
#include <sstream>

std::function<void(int, int)> irKeyHandler = nullptr;
static bool handlerRegistered = false;

namespace IARMUtils {

std::string formatIARMResult(IARM_Result_t result) {
    switch (result) {
    case IARM_RESULT_SUCCESS:
        return "IARM_RESULT_SUCCESS [success]";
    case IARM_RESULT_INVALID_PARAM:
        return "IARM_RESULT_INVALID_PARAM [invalid input parameter]";
    case IARM_RESULT_INVALID_STATE:
        return "IARM_RESULT_INVALID_STATE [invalid state encountered]";
    case IARM_RESULT_IPCCORE_FAIL:
        return "IARM_RESULT_IPCORE_FAIL [underlying IPC failure]";
    case IARM_RESULT_OOM:
        return "IARM_RESULT_OOM [out of memory]";
    default:
        std::ostringstream tmp;
        tmp << "Unknown IARM_Result_t (" << result << ")";
        return tmp.str();
    }
}

void IREventHandler(const char *owner, IARM_EventId_t eventId, void *data, size_t len) {
    (void)len;
    if (strcmp(owner, IARM_BUS_IRMGR_NAME) == 0 && eventId == IARM_BUS_IRMGR_EVENT_IRKEY) {
        auto *eventData = static_cast<IARM_Bus_IRMgr_EventData_t *>(data);
        if (irKeyHandler) {
            irKeyHandler(eventData->data.irkey.keyType, eventData->data.irkey.keyCode);
        } else {
            logWarn("IR key handler is not set. Ignoring event.");
        }
    } else {
        logDebug("Unhandled event: Owner = " + std::string(owner) + ", EventId = " + std::to_string(eventId));
    }
}

bool initialize(const char *name) {
    IARM_Result_t result;

    result = IARM_Bus_Init(name);
    if (result != IARM_RESULT_SUCCESS) {
        logError("Failed to initialize IARM with name '" + std::string(name) + "': " + formatIARMResult(result));
        return false;
    }

    result = IARM_Bus_Connect();
    if (result != IARM_RESULT_SUCCESS) {
        logError("Failed to connect to IARM bus: " + formatIARMResult(result));
        IARM_Bus_Term();
        return false;
    }

    if (UINPUT_init(true) != 0) {
        logError("Failed to initialize UINPUT module.");
        IARM_Bus_Disconnect();
        IARM_Bus_Term();
        return false;
    }

    logDebug("IARM initialized and connected successfully with name: '" + std::string(name) + "'.");
    return true;
}

void terminate() {
    static std::atomic<bool> isTerminated(false);

    if (isTerminated.exchange(true)) {
        logWarn("Terminate called more than once. Ignoring subsequent calls.");
        return;
    }

    if (handlerRegistered) {
        IARM_Result_t result =
            IARM_Bus_RemoveEventHandler(IARM_BUS_IRMGR_NAME, IARM_BUS_IRMGR_EVENT_IRKEY, IREventHandler);
        if (result != IARM_RESULT_SUCCESS) {
            logError("Failed to remove IR key event handler: " + formatIARMResult(result));
        } else {
            logDebug("IR key event handler removed successfully.");
        }
        handlerRegistered = false;
    }

    if (UINPUT_term() != 0) {
        logError("Failed to terminate UINPUT module.");
    } else {
        logDebug("UINPUT module terminated successfully.");
    }

    IARM_Result_t result = IARM_Bus_Disconnect();
    if (result != IARM_RESULT_SUCCESS) {
        logError("Failed to disconnect IARM bus: " + formatIARMResult(result));
    } else {
        logDebug("IARM bus disconnected successfully.");
    }

    logDebug("Calling IARM_Bus_Term");
    result = IARM_Bus_Term();
    if (result != IARM_RESULT_SUCCESS) {
        logError("Failed to terminate IARM: " + formatIARMResult(result));
    } else {
        logDebug("IARM terminated successfully.");
    }
}

void sendKeyEvent(int keyType, int keyCode) {
    auto dispatcher = UINPUT_GetDispatcher();
    if (!dispatcher) {
        logError("UINPUT dispatcher is not available. Cannot send key event.");
        return;
    }

    try {
        dispatcher(keyCode, keyType, 0); // Sending the key event
        logDebug("Key event dispatched: KeyCode = " + std::to_string(keyCode) +
                 ", KeyType = " + std::to_string(keyType));
    } catch (const std::exception &e) {
        logError("Failed to dispatch key event: " + std::string(e.what()));
    }
}

void registerIRKeyHandler(const std::function<void(int keyType, int keyCode)> &handler) {
    irKeyHandler = handler;
    IARM_Result_t result =
        IARM_Bus_RegisterEventHandler(IARM_BUS_IRMGR_NAME, IARM_BUS_IRMGR_EVENT_IRKEY, IREventHandler);
    if (result != IARM_RESULT_SUCCESS) {
        logError("Failed to register IR key event handler: " + formatIARMResult(result));
        throw std::runtime_error("Failed to register IR key handler.");
    }
    handlerRegistered = true;
    logDebug("IR key event handler registered successfully.");
}

} // namespace IARMUtils
