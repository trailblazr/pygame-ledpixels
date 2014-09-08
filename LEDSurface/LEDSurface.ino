#include <OctoWS2811.h>

#define LED_WIDTH    90
#define LED_HEIGHT   20
#define LEDS_PER_STRIP 360
#define CONFIG       WS2811_800kHz

const int frameLength = LED_WIDTH * LED_HEIGHT;
const int frameSize = frameLength * 4;

DMAMEM int displayMemory[LEDS_PER_STRIP*6];
int drawingMemory[LEDS_PER_STRIP*6];
int receiveMem[frameLength];

int frame = 0;

OctoWS2811 leds(LEDS_PER_STRIP, displayMemory, drawingMemory, CONFIG);

void setup() {
  Serial.setTimeout(50);
  leds.begin();
  leds.show();
}

void loop() {
  char messageType = Serial.read();
  
  
  if (messageType == '?') {
    // print config
    Serial.print(LED_WIDTH);
    Serial.print(",");
    Serial.print(LED_HEIGHT);
    Serial.println();
  } else if (messageType == '*') {
    // receive a frame
    Serial.readBytes((char*)receiveMem, frameSize);
    int pixel = 0;
    int x, y, col = 0;
    char r, g, b;
    for (int i = 0; i < frameLength; i++) {
      x = i % LED_WIDTH;
      y = i / LED_WIDTH;
      
      if (y % 2 == 0) {
        pixel = i;
      } else {
        pixel = y * LED_WIDTH + LED_WIDTH - (x + 1);
      }
      col = receiveMem[i];
      r = (0x0000ff00 & col) >> 8;
      g = (0x00ff0000 & col) >> 16;
      b = (0xff000000 & col) >> 24;
      
      leds.setPixel(pixel, g, r, b);
    }
  } else {
    // swallow garbage
  }
  leds.show();
}
