#include "shared_variables.h"
#include "timer.h"

/**
 * @brief Calculates and formats time for display as "MM:SS".
 * If `playing` is true, it calculates the remaining time based on `max_time` and `startTime`.
 * If `playing` is false, it formats the total `max_time`.
 * 
 * @param minSec Reference to a String object where the formatted time will be stored.
 */
void ellapsedTime(String &minSec) {
  unsigned long ellapsedMillis = playing ? (max_time - (millis() - startTime)) / 1000 : max_time / 1000;
  int minutes = ellapsedMillis / 60;
  int seconds = ellapsedMillis % 60;
  minSec = String(minutes) + ":" + (seconds < 10 ? "0" : "") + String(seconds);
}
