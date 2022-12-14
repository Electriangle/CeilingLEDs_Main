//  TwinkleSparkle2014 is a program that lets you make an LED strip
//  "twinkle" all the time and "sparkle" at a defined interval
//
//  Daniel Wilson, 2014
//
//  With BIG thanks to the FastLED community and especially Mark
//  Kriegsman whose "Fire2012" simulation inspired this program.
//  Some of Mark's original code and comments persist in this work.
///////////////////////////////////////////////////////////////////////////////////////////
#include<FastLED.h>

#define BRIGHTNESS         8
#define FRAMES_PER_SECOND  10
#define COOLING            5         // controls how quickly LEDs dim
#define TWINKLING          NUM_LEDS_1*3/4       // controls how many new LEDs twinkle
#define FLICKER            50        // controls how "flickery" each individual LED is

static int beatInterval =  8912;     // the interval at which you want the strip to "sparkle"
long nextBeat =            0;
long nextTwinkle =         3000;     // twinkling doesn't start until after the sanity check delay
unsigned int seeds =       0;
unsigned int seeds2 =      0;
long loops =               0;
long deltaTimeTwinkle =    0;
long deltaTimeSparkle =    0;
boolean beatStarted =      false;

static int heat[NUM_LEDS_1];
static int heat2[NUM_LEDS_2];
#define HWSERIAL Serial1
///////////////////////////////////////////////////////////////////////////////////////////

// Vocablulary lesson:
//   Twinkling - when individual LEDs "ignite" then slowly burn out
//   Sparkling - when a whole mess of LEDs "ignite" at the same time then burn out
//   Flickering - when a lit led modulates it brightness to simulate the flicker of a flame

// This Twinkle subroutine creates a slow "twinkling" of the strip.
// It uses the same "heating" methodology as Mark Kriegman's "Fire2012"
// where pixels are "heated" and "cooled" and then the tempreature of
// each pixel is mapped to a color and brightness.

//Play with this for different strip colors
CHSV TwinkleColor( int temperature)
{
  CHSV heatcolor;
  heatcolor.hue = 180;
  heatcolor.saturation = 200;
  heatcolor.value = temperature;
  return heatcolor;
}

void Twinkle()
{
  // Step 1. Create a randome number of seeds
  random16_add_entropy( random(4)); //random8() isn't very random, so this mixes things up a bit
  seeds = random16(10, NUM_LEDS_1 - 10);
  seeds2 = random16(10, NUM_LEDS_2 - 10);

  // Step 2. "Cool" down every location on the strip a little
  for ( int i = 0; i < NUM_LEDS_1; i++) {
    heat[i] = qsub8( heat[i], COOLING);
  }
  for ( int i = 0; i < NUM_LEDS_2; i++) {
    heat2[i] = qsub8( heat2[i], COOLING);
  }

  // Step 3. Make the seeds into heat on the string
  for ( int j = 0 ; j < seeds ; j++) {
    if (random16() < TWINKLING) {
      //again, we have to mix things up so the same locations don't always light up
      random16_add_entropy( random(4));
      heat[random16(NUM_LEDS_1)] = random16(50, 255);
    }
  }
  for ( int j = 0 ; j < seeds2 ; j++) {
    if (random16() < TWINKLING) {
      //again, we have to mix things up so the same locations don't always light up
      random16_add_entropy( random(4));
      heat2[random16(NUM_LEDS_2)] = random16(50, 255);
    }
  }

  // Step 4. Add some "flicker" to LEDs that are already lit
  //         Note: this is most visible in dim LEDs
  for ( int k = 0 ; k < NUM_LEDS_1 ; k++ ) {
    if (heat[k] > 0 && random8() < FLICKER) {
      heat[k] = qadd8(heat[k] , 10);
    }
  }
  for ( int k = 0 ; k < NUM_LEDS_2 ; k++ ) {
    if (heat2[k] > 0 && random8() < FLICKER) {
      heat2[k] = qadd8(heat2[k] , 10);
    }
  }

  // Step 5. Map from heat cells to LED colors
  for ( int j = 0; j < NUM_LEDS_1; j++)
  {
    led1[j] = TwinkleColor( heat[j] );
  }
  for ( int j = 0; j < NUM_LEDS_2; j++)
  {
    led2[j] = TwinkleColor( heat2[j] );
  }
  nextTwinkle += 1000 / FRAMES_PER_SECOND ; // assign the next time Twinkle() should happen
}

// Sparkle works very much like Twinkle, but with more LEDs lighting up at once
void Sparkle() {
  // Step 1. Make a random number of seeds
  seeds = random16(NUM_LEDS_1 - 20 , NUM_LEDS_1);
  seeds2 = random16(NUM_LEDS_2 - 20 , NUM_LEDS_2);

  // Step 2. Increase the heat at those locations
  for ( int i = 0 ; i < seeds ; i++) {
    {
      int pos = random16(NUM_LEDS_1);
      random16_add_entropy( random(4));
      heat[pos] = random8(50, 255);
    }
  }
  for ( int i = 0 ; i < seeds2 ; i++) {
    {
      int pos = random16(NUM_LEDS_2);
      random16_add_entropy( random(4));
      heat2[pos] = random8(50, 255);
    }
  }
  nextBeat += beatInterval; // assign the next time Twinkle() should happen
  loops++ ;
}

// Use Mark Krigsman's orignal "HeatColor" code if you want to
// get different colors at differet "temperatures"
/*
  CRGB HeatColor( uint8_t temperature)
  {
  CRGB heatcolor;
  // Scale 'heat' down from 0-255 to 0-191,
  // which can then be easily divided into three
  // equal 'thirds' of 64 units each.
  uint8_t t192 = scale8_video( temperature, 192);
  // calculate a value that ramps up from
  // zero to 255 in each 'third' of the scale.
  uint8_t heatramp = t192 & 0x3F; // 0..63
  heatramp <<= 2; // scale up to 0..252
  // now figure out which third of the spectrum we're in:
  if( t192 & 0x80) {
  // we're in the hottest third
  heatcolor.r = 255; // full red
  heatcolor.g = 255; // full green
  heatcolor.b = heatramp; // ramp up blue
  } else if( t192 & 0x40 ) {
  // we're in the middle third
  heatcolor.r = 255; // full red
  heatcolor.g = heatramp; // ramp up green
  heatcolor.b = 0; // no blue
  } else {
  // we're in the coolest third
  heatcolor.r = heatramp; // ramp up red
  heatcolor.g = 0; // no green
  heatcolor.b = 0; // no blue
  }
  return heatcolor;
  }
*/

void twinkle_action()
{
  // Wait for something in the serial monitor before "Sparkling" the first time.
  // This lets you time the sparkle to a particular beat in music.
  // In practice, just type a letter into the serial monitor and press enter
  // when you want the first sparkle to start.

  if (loops == 0 && !Serial.available()) {
    nextBeat = millis();
  }
  else {
    if (loops == 0 && beatStarted == false) {
      nextBeat = millis();
      beatStarted == true;
      Sparkle();
    }
    else {
      long deltaTimeSparkle = millis() - nextBeat;
      if ( deltaTimeSparkle > 0 ) Sparkle(); // if more time than
    }
  }

  deltaTimeTwinkle = millis() - nextTwinkle;
  if ( deltaTimeTwinkle > 0 ) {
    Twinkle();
  }
  FastLED.show(); // display this frame
}
