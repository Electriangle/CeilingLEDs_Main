// Corners: 90, (124, 184), 323; 88, 224
int corner_1 = 90;
int corner_a = 124;
//int corner_b = 144;
//int corner_c = 164;
int corner_d = 248;
int corner_2 = 323;
int corner_3 = 89;
int corner_4 = 224;
int NUM_HALLWAY_LEDS = 124;

int redColor = 0;
int greenColor = 0;
int blueColor = 255;
int colorTime = 1;

//void Sparkle(byte red, byte green, byte blue, int SpeedDelay) {
//  int Pixel = random(NUM_LEDS);
//  fill_solid(led, NUM_LEDS, CRGB(red, green, blue));
//  FastLED.show();
//  delay(SpeedDelay);
//  fill_solid(led, NUM_LEDS, CRGB(0, 0, 0));
//}

// ******************************************************************************************************

void corner_star(int r, int g, int b) {
  FastLED.clear();
  FastLED.show();
  for (int i = 0; i < 117; i++) {
    led1[i + corner_1].setRGB(r, g, b);
    if (i <= corner_1){led1[corner_1 - i].setRGB(r, g, b);}
    
    led1[i + corner_2].setRGB(r, g, b);
    led1[corner_2 - i].setRGB(r, g, b);

    led2[i + corner_3].setRGB(r, g, b);
    if (i <= corner_3){led2[corner_3 - i].setRGB(r, g, b);}
    
    if (i > 37) {led1[NUM_LEDS_1 - i + 37].setRGB(r, g, b);}
    led2[corner_4 - i].setRGB(r, g, b);

    if (i < corner_1) {led1[0].setRGB(0, 0, 0);}
    delay(30);
    FastLED.show();
  }
  fill_solid(led1, NUM_LEDS_1, CRGB(r, g, b));
  fill_solid(led2, NUM_LEDS_2, CRGB(r, g, b));
  FastLED.show();
}

// ******************************************************************************************************

void hallway_lights() {
  if (hallway_on == false) {
    for (int i = 0; i < NUM_HALLWAY_LEDS; i++) {
      led1[i + corner_a].setRGB(2, 20, 5);
      FastLED.show();
      delay(10);
    }
    hallway_on = true;
  }
}

// ******************************************************************************************************

void movie_mode() {
  if (movie_on == false) {
    corner_star(10, 5, 0);
    for (int i = 0; i <= 5; i++) {
      fill_solid(led2, NUM_LEDS_2, CRGB(10 - i*2, 5 - i, 0));
      for (int j = 0; j < corner_1; j++) {led1[j].setRGB(10 - i*2, 5 - i, 0);}
      for (int j = 0; j < (NUM_LEDS_1 - corner_2); j++) {led1[corner_2 + j].setRGB(10 - i*2, 5 - i, 0);}
      FastLED.show();
      delay(200);
    }
    movie_on = true;
  }
}

// ******************************************************************************************************

void simple_color_lines() {
  for (int i = 0; i < NUM_LEDS_1; i++) {
    led1[i].setRGB(20, 0, 0);
    delay(10);
    FastLED.show();
  }
  for (int i = NUM_LEDS_2; i >= 0; i--) {
    led2[i].setRGB(20, 0, 0);
    delay(10);
    FastLED.show();
  }
  delay(2000);

  for (int i = 0; i < NUM_LEDS_1; i++) {
    led1[i].setRGB(0, 20, 0);
    delay(10);
    FastLED.show();
  }
  for (int i = NUM_LEDS_2; i >= 0; i--) {
    led2[i].setRGB(0, 20, 0);
    delay(10);
    FastLED.show();
  }
  delay(2000);

  for (int i = 0; i < NUM_LEDS_1; i++) {
    led1[i].setRGB(0, 0, 20);
    delay(10);
    FastLED.show();
  }
  for (int i = NUM_LEDS_2; i >= 0; i--) {
    led2[i].setRGB(0, 0, 20);
    delay(10);
    FastLED.show();
  }
  delay(2000);
  
  for (int i = 0; i < NUM_LEDS_1; i++) {led1[i].setRGB(0, 0, 0);}
  for (int i = NUM_LEDS_2; i >= 0; i--) {led2[i].setRGB(0, 0, 0);}
  FastLED.show();
  delay(100);

  for (int i = 0; i < NUM_LEDS_1; i++) {
    led1[i].setRGB(20, 20, 20);
    delay(10);
    FastLED.show();
  }
  for (int i = NUM_LEDS_2; i >= 0; i--) {
    led2[i].setRGB(20, 20, 20);
    delay(10);
    FastLED.show();
  }
  delay(5000);
  
  for (int i = 0; i < NUM_LEDS_1; i++) {led1[i].setRGB(0, 0, 0);}
  for (int i = NUM_LEDS_2; i >= 0; i--) {led2[i].setRGB(0, 0, 0);}
  FastLED.show();
  delay(100);
}

// ******************************************************************************************************

void startup_fadeinout() {
  for (int i = 0; i <= 20; i++) {
    fill_solid(led1, NUM_LEDS_1, CRGB(i, i, i));
    fill_solid(led2, NUM_LEDS_2, CRGB(i, i, i));
    FastLED.show();
    delay(50);
  }
  delay(1000);
  for (int i = 20; i >= 0; i--) {
    fill_solid(led1, NUM_LEDS_1, CRGB(i, i, i));
    fill_solid(led2, NUM_LEDS_2, CRGB(i, i, i));
    FastLED.show();
    delay(50);
  }
  FastLED.clear();
  FastLED.show();
}

// ******************************************************************************************************

void rainbowCycle(int SpeedDelay) {   // about 60 delay is good.
  byte *c;
  uint16_t i, j;

  for(j=0; j<256; j++) { // 5 cycles of all colors on wheel*5
    for(i=0; i< NUM_LEDS_1; i++) {
      c=Wheel(((i * 256 / NUM_LEDS_1) + j) & 255);
      led1[i].setRGB(*c, *(c+1), *(c+2));
    }
    for(i=0; i< NUM_LEDS_2; i++) {
      c=Wheel(((i * 256 / NUM_LEDS_2) + j) & 255);
      led2[i].setRGB(*c, *(c+1), *(c+2));
    }
    FastLED.show();
    delay(SpeedDelay);
  }
}

byte * Wheel(byte WheelPos) {
  static byte c[3];
 
  if(WheelPos < 85) {
   c[0]=WheelPos * 3;
   c[1]=255 - WheelPos * 3;
   c[2]=0;
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   c[0]=255 - WheelPos * 3;
   c[1]=0;
   c[2]=WheelPos * 3;
  } else {
   WheelPos -= 170;
   c[0]=0;
   c[1]=WheelPos * 3;
   c[2]=255 - WheelPos * 3;
  }
  return c;
}


// ******************************************************************************************************

void rainbow_action(){
  rainbowCycle(80);
//  twinkleRainbow(random(0,255), random(0,255), random(0,255), 2);
}

// ******************************************************************************************************

void Sparkle(byte red, byte green, byte blue, int SpeedDelay) {
  int Pixel = random(NUM_LEDS_1);
  led1[Pixel].setRGB(red, green, blue);
  FastLED.show();
  delay(SpeedDelay);
  led1[Pixel].setRGB(0, 0, 0);
}

// ******************************************************************************************************

void twinkleRainbow(byte red, byte green, byte blue, int twinkleDelay){
  float r, g, b;
  int startingPoint = random(0,255);
  int stoppingPoint = random(0,255);
  int randomLED[10]={random(0,NUM_LEDS_1), random(0,NUM_LEDS_1), random(0,NUM_LEDS_1), random(0,NUM_LEDS_1), random(0,NUM_LEDS_1), 
                     random(0,NUM_LEDS_1), random(0,NUM_LEDS_1), random(0,NUM_LEDS_1), random(0,NUM_LEDS_1), random(0,NUM_LEDS_1)};
  
  for(int k=startingPoint; k < 256; k += 1) {
    r = (k/256.0)*red;
    g = (k/256.0)*green;
    b = (k/256.0)*blue;
    for (int i=0; i<random(2,6); i++) {
      led1[randomLED[i]].setRGB(r, g, b);
      delay(twinkleDelay);
      FastLED.show();
    }
  }
  
  for(int k = 255; k >= stoppingPoint; k -= 2) {
    r = (k/256.0)*red;
    g = (k/256.0)*green;
    b = (k/256.0)*blue;
    for (int i=0; i<random(2,6); i++) {
      led1[randomLED[i]].setRGB(r, g, b);
      delay(twinkleDelay);
      FastLED.show();
    }
  }
}

// ******************************************************************************************************

void fade_action() {
  FadeInOut(random(0, 255), random(0, 255), random(0, 255), 50);
//  FadeInOut(redColor, greenColor, blueColor, 50);
//  electromagneticSpectrum(20);
}

void FadeInOut(byte red, byte green, byte blue, int fadeDelay){
//  float r, g, b;
//  
//  for(int k = 0; k <= 20; k += 1) {
//    r = (k/20.0)*red;
//    g = (k/20.0)*green;
//    b = (k/20.0)*blue;
//    map(r, 0, 50, 50, 1);
//    fill_solid(led1, NUM_LEDS_1, CRGB(constrain(r, 0, 20), constrain(g, 0, 20), constrain(b, 0, 20)));
//    fill_solid(led2, NUM_LEDS_2, CRGB(constrain(r, 0, 20), constrain(g, 0, 20), constrain(b, 0, 20)));
//    FastLED.show();
//    delay(fadeDelay);
//  }

//  delay(500);
//  FastLED.clear();
//  FastLED.show();
  
//  for(int k = 20; k >= 0; k -= 1) {
//    r = (k/20.0)*red;
//    g = (k/20.0)*green;
//    b = (k/20.0)*blue;
//    fill_solid(led1, NUM_LEDS_1, CRGB(r, g, b));
//    fill_solid(led2, NUM_LEDS_2, CRGB(r, g, b));
//    FastLED.show();
//    delay(fadeDelay);
//  }


  float r, g, b;
  
  for(int k = 0; k <= 255; k += 5) {
    r = (k/255.0)*red;
    g = (k/255.0)*green;
    b = (k/255.0)*blue;
    Serial.println(k);
    Serial.print(r);
    Serial.print("  ");
    Serial.print(g);
    Serial.print("  ");
    Serial.println(b);
    fill_solid(led1, NUM_LEDS_1, CRGB(r, g, b));
    fill_solid(led2, NUM_LEDS_2, CRGB(r, g, b));
//    fill_solid(led1, NUM_LEDS_1, CRGB(map(r, 0, 255, 0, 20), map(g, 0, 255, 0, 20), map(b, 0, 255, 0, 20)));
//    fill_solid(led2, NUM_LEDS_2, CRGB(map(r, 0, 255, 0, 20), map(g, 0, 255, 0, 20), map(b, 0, 255, 0, 20)));
    FastLED.show();
    delay(fadeDelay);
  }
  
  for(int k = 255; k >= 0; k -= 5) {
    r = (k/255.0)*red;
    g = (k/255.0)*green;
    b = (k/255.0)*blue;
    Serial.println(k);
    Serial.print(r);
    Serial.print("  ");
    Serial.print(g);
    Serial.print("  ");
    Serial.println(b);
    fill_solid(led1, NUM_LEDS_1, CRGB(r, g, b));
    fill_solid(led2, NUM_LEDS_2, CRGB(r, g, b));
//    fill_solid(led1, NUM_LEDS_1, CRGB(map(r, 0, 255, 0, 20), map(g, 0, 255, 0, 20), map(b, 0, 255, 0, 20)));
//    fill_solid(led2, NUM_LEDS_2, CRGB(map(r, 0, 255, 0, 20), map(g, 0, 255, 0, 20), map(b, 0, 255, 0, 20)));
    FastLED.show();
    delay(fadeDelay);
  }
}

// ******************************************************************************************************

void electromagneticSpectrum(int transitionSpeed) {
  switch(colorTime) {
    case 1:
      greenColor += transitionSpeed;
      blueColor -= transitionSpeed;
      if (greenColor >= 255 or blueColor <= 0) {
        greenColor = 255;
        blueColor = 0;
        colorTime = 2;
      }
      break;
    case 2:
      redColor += transitionSpeed;
      greenColor -= transitionSpeed;
      if (redColor >= 255 or greenColor <= 0) {
        redColor = 255;
        greenColor = 0;
        colorTime = 3;
      }
      break;
    case 3:
      redColor -= transitionSpeed;
      blueColor += transitionSpeed;
      if (redColor <= 0 or blueColor >= 255) {
        redColor = 0;
        blueColor = 255;
        colorTime = 1;
      }
      break;
  }
}

// ******************************************************************************************************
