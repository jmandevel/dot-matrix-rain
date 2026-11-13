#include <Arduino.h>
#include <LedControl.h>

constexpr unsigned LED_X_DIMENSION = 8;
constexpr unsigned LED_Y_DIMENSION = 8;

LedControl LC(12, 10, 11);

void setup() {
  // The MAX72XX is in power-saving mode on startup,
  // we have to do a wakeup call
  LC.shutdown(0, false);
  // Set the brightness to a medium values /
  LC.setIntensity(0, 16);
  // and clear the display
  LC.clearDisplay(0);
}

struct Trail {
  bool is_alive{false};
  int head{0};
  int length{0};
};

Trail TRAILS[8]{};

void loop() {

  {
    int new_trail_i = random(0, LED_Y_DIMENSION);
    Trail &new_trail = TRAILS[new_trail_i];
    if (new_trail.is_alive == false) {
      new_trail.length = random(3, 8);
      new_trail.is_alive = true;
    }
  }

  for (int trail_i = 0; trail_i < LED_X_DIMENSION; trail_i++) {
    Trail &trail = TRAILS[trail_i];
    if (trail.is_alive == true) {
      trail.head++;
    }
    if (trail.head - trail.length > 8) {
      trail = {};
    }
  }

  LC.clearDisplay(0);
  for (int x = 0; x < LED_X_DIMENSION; x++) {
    const Trail &trail = TRAILS[x];
    if (trail.is_alive == false) {
      continue;
    }
    for (int y = max(trail.head - trail.length, 0); y < min(trail.head, LED_Y_DIMENSION);
         y++) {
      LC.setLed(0, x, y, true);
    }
  }

  delay(1000 / 20); // 20 fps
}