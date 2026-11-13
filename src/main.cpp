#include <Arduino.h>
#include <LedControl.h>

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

struct MatrixCol {
  bool alive{false};
  int head{0};
  int length{0};
};

MatrixCol cols[8]{};

void loop() {

  int nc = random(0, 8);
  if (cols[nc].alive == false) {
    cols[nc].length = random(3, 8);
    cols[nc].alive = true;
  }

  for (int i = 0; i < 8; i++) {
    if (cols[i].alive == true) {
      cols[i].head++;
    }
    if (cols[i].head - cols[i].length > 8) {
      cols[i] = {};
    }
  }

  LC.clearDisplay(0);
  for (int x = 0; x < 8; x++) {
    const MatrixCol& col = cols[x];
    if (col.alive == false) {
      continue;
    }
    for (int y = max(col.head - col.length, 0); y < min(col.head, 8); y++) {
      LC.setLed(0, y, x, true);
    }
  }
  
  delay(1000/20);
}