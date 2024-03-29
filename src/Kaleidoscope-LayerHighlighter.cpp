#include "Kaleidoscope-LayerHighlighter.h"

#include "Kaleidoscope-LEDControl.h"

#include <Kaleidoscope.h>

kaleidoscope::EventHandlerResult LayerHighlighter::onSetup(void) {
  return kaleidoscope::EventHandlerResult::OK;
}

kaleidoscope::EventHandlerResult LayerHighlighter::onKeyswitchEvent(Key &mappedKey, byte row, byte col, uint8_t keyState) {
  if (mappedKey != LockLayer(layer)
      && mappedKey != ShiftToLayer (layer)) {
    return kaleidoscope::EventHandlerResult::OK;
  }

  if (keyToggledOn(keyState)) {
    savedLEDMode = LEDControl.get_mode_index();
  } else if (keyToggledOff(keyState)) {
    LEDControl.set_mode(LEDControl.get_mode_index());
  }

  return kaleidoscope::EventHandlerResult::OK;
}

kaleidoscope::EventHandlerResult LayerHighlighter::afterEachCycle() {
  if (!Layer.isActive (layer)) {
    return kaleidoscope::EventHandlerResult::OK;
  }

  LEDControl.set_mode(LEDControl.get_mode_index());
  for (uint8_t r = 0; r < ROWS; r++) {
    for (uint8_t c = 0; c < COLS; c++) {
      Key k = Layer.lookupOnActiveLayer(r, c);
      Key layer_key = Layer.getKey(layer, KeyAddr (r, c));

      if (k == LockLayer(layer)) {
        cRGB lock_color = breath_compute(lockHue);
        LEDControl.setCrgbAt(r, c, lock_color);
      }

      if ((k != layer_key) || (k == Key_NoKey)) {
        LEDControl.refreshAt(r, c);
      } else {
        LEDControl.setCrgbAt(r, c, color);
      }
    }
  }

  return kaleidoscope::EventHandlerResult::OK;
}
