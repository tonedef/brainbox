/**
 * @file ServerManager.h
 * @brief Manages the web server for device configuration and control.
 */
#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

#include "arduinoGlue.h"

// Function declarations
void handleRoot();
void handleUpdate();
void setupWebServer();
void handleClient();

#endif // SERVERMANAGER_H