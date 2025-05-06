#ifndef ServerManager_h
#define ServerManager_h

#include <WiFi.h>
#include <WebServer.h>
#include "PreferencesManager.h"
#include "shared_variables.h"

// Function declarations
void handleRoot();
void handleUpdate();
void setupWebServer();
void handleClient();

#endif // ServerManager_h
