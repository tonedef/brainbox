#include "PreferencesManager.h"
#include "shared_variables.h"
#include "timer.h"
#include "buttons.h"

void ellapsedTime(String &minSec) {
  unsigned long ellapsedMillis = playing ? (max_time - (millis() - startTime)) / 1000 : max_time / 1000;
  int minutes = ellapsedMillis / 60;
  int seconds = ellapsedMillis % 60;
  minSec = String(minutes) + ":" + (seconds < 10 ? "0" : "") + String(seconds);
}
