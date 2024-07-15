#include <Adafruit_CircuitPlayground.h>
#include <Keyboard.h>

#define CAP_THRESHOLD 780

uint8_t pads[] = {3, 0, 1, 10};
uint8_t numberOfPads = sizeof(pads)/sizeof(uint8_t);

void setup() {
  Serial.begin(9600);

  Keyboard.begin();
  
  CircuitPlayground.begin();
  CircuitPlayground.setBrightness(30);

  delay(250);
  cycleLights();
}

void cycleLights() {
  static int sequence = 0;
  static unsigned long previousMillis = 0;

  if (!((unsigned long)(millis() - previousMillis) >= 250)) return;

  int i;
  for (i = 0; i < 10; i++) {
    CircuitPlayground.strip.setPixelColor(i,(((sequence + i) % 3) == 0) * 255,(((sequence + i) % 3) == 2) * 255,(((sequence + i) % 3) == 1) * 255);
  }
  CircuitPlayground.strip.show();
  sequence = (sequence + 1) % 3;

  previousMillis = millis();
}

void flashLights() {
  CircuitPlayground.clearPixels();
  int i;
  for (i = 0; i < 10; ++i) {
    CircuitPlayground.strip.setPixelColor(i,255,255,255);
  }
  CircuitPlayground.strip.show();
  delay(100);
  CircuitPlayground.clearPixels();
}

void leftButton() {
  static bool previousButtonState = false;
  bool buttonState = CircuitPlayground.leftButton();

  if ((buttonState != previousButtonState) && (buttonState == true)) {
    Keyboard.write(KEY_ESC);
  }

  previousButtonState = buttonState;
}

void rightButton() {
  static bool previousButtonState = false;
  bool buttonState = CircuitPlayground.rightButton();

  if ((buttonState != previousButtonState) && (buttonState == true)) {
    Keyboard.write(KEY_RETURN);
  }

  previousButtonState = buttonState;
}

bool meetsCapThreshold(uint8_t pad) {
  if (CircuitPlayground.readCap(pad) > CAP_THRESHOLD) {
    return true;
  } else {
    return false;
  }
}

void capSensor(uint8_t pad) {
  switch (pad) {
    case 3:
      Keyboard.write(KEY_LEFT_ARROW);
      flashLights();
      break;
    case 10:
      Keyboard.write(KEY_RIGHT_ARROW);
      flashLights();
      break;
    case 0:
      Keyboard.write(KEY_UP_ARROW);
      flashLights();
      break;
    case 1:
      Keyboard.write(KEY_DOWN_ARROW);
      flashLights();
      break;
    default:
      Serial.println("Not an arrow key");
  }
}

void loop() {
  bool slideSwitch;
  slideSwitch = CircuitPlayground.slideSwitch();

  if (slideSwitch) {
    for (int i = 0; i < numberOfPads; i++) {
      if (meetsCapThreshold(pads[i])) {
        capSensor(pads[i]);
      }
    }
    
    leftButton();
    rightButton();

    cycleLights();
  }
  delay(25);
}
