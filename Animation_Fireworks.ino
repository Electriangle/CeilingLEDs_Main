/* Fireworks for FastLED 2.1 or greater
By: Andrew Tuline
Date: Oct, 2014
Non-bouncing one dimensional fireworks demonstration.
The variables are tuned to a pretty short LED strip. You'll need to play around for a longer strip.
FastLED is available at https://github.com/FastLED/FastLED
*/


// Initialize global variables for sequences
int thisdelay = 50;                                           // A delay value for the sequence(s)                        ********* CHANGE ME **************

int thissat = 255;                                            // Standard fully saturated LED
int thisbright = 255;                                         // Standard fully bright LED

// GRAVBALL VARIABLES
int gravity = -6;                                             // Gravity (-8)                                              ********* CHANGE ME **************
int drag = 0;                                                 // Not required due to losses in 16 bit math. Not used.
int timeinc = 2;                                              // A time increment.
int maxcount = 100;                                           // Maximum number of explosion frames.
int mycount_1 = 0;                                              // Repeat the explosion counter.
int mycount_2 = 0;                                              // Repeat the explosion counter.


// Inital speed variables can be changed to make it go HIGHER!!!
int streamervelocity = 500;                                   // Velocity of the initial streamer that goes into the air. ********* CHANGE ME **************
int explosionvelocity = 500;                                  // Maximum velocity of the explosion.                       ********* CHANGE ME **************


uint8_t thisstatus_1 = 0;                                       // Used to determine which state our finite state machine is in.
uint8_t thisstatus_2 = 0;                                       // Used to determine which state our finite state machine is in.

#define numgravs 6                                            // How many gravs we are using. The first one is our streamer.  ********* CHANGE ME **************

typedef struct {                                              // Define a structure for the gravs.
      long distold;                                           // I defined this as 'long' so that it'll work with longer strips.
      long distance;                                          // Ditto.
      int velold;
      int velocity;
      int thishue;
      int thissat;
      int thisbright;
  }  gravs;

gravs mygravs[numgravs];

void firework_action() {
  fire_1();
  fire_2();
  FastLED.show();
  delay(thisdelay);
}


void fire_1() {                                                 // States of fireworks, need to be loop friendly.
  switch (thisstatus_1) {
    case 0: firestart_1(); break;                               // Initialize the first element of array to shoot into the 'sky'.
    case 1: fireup_1(); break;                                  // Send it up. Once it reaches the top, we change status to '2'
    case 2: explodeinit_1(); break;                             // Initialize the array for the remainder of the fireworks elements. and change status.
    case 3: explode_1(); break;                                 // Now, let's explode them for a count duration and reset to 0 when done.
    default: break;
  }
} // fire()


void firestart_1() {                                            // Initialize variable for a new streamer being shot into the 'sky'.
  mygravs[0].distold = 0;
  mygravs[0].distance = 0;
  mygravs[0].velold = streamervelocity;                       // Goes to a maximum distance of ~18000. Needs more LED's
  mygravs[0].velocity = streamervelocity;
  mygravs[0].thishue = random8();                             // Might as well make it multi-coloured
  mygravs[0].thissat = 50;                                    // But not very saturated
  mygravs[0].thisbright = 128;
  thisstatus_1 = 1;
  Serial.println("firestart");
} // firestart()


void fireup_1() {                                               // Shoot fireworks into the air.

  mygravs[0].velocity = mygravs[0].velold + gravity*timeinc;  // Split gravity math into two lines for simplicity.
  mygravs[0].distance = mygravs[0].distold + mygravs[0].velocity*timeinc;

  int i = map(mygravs[0].distance, 0, 32767, 0, NUM_LEDS_1);
    
  mygravs[0].velold = mygravs[0].velocity;                    // Capture the current velocity/distance.
  mygravs[0].distold = mygravs[0].distance;

  if (i < NUM_LEDS_1) led1[i] = CHSV(mygravs[0].thishue, mygravs[0].thissat, mygravs[0].thisbright);     // Let's get ready to display it, but not if it's too high.
  if(mygravs[0].velold <= 0) thisstatus_1 = 2;                  // Starting to fall, so let's stop this nonsense.

  for (int j = 0; j < NUM_LEDS_1; j++) led1[j].fadeToBlackBy(16);  // The trail fades slowly over time.

  if (i > 3) {                                                // Add a bit of a sparkly trailer above the ground, but make sure that trailer is ABOVE the ground.
    if (random8() >220) {
      led1[i-3] = CHSV(mygravs[0].thishue, random8(100, 200), mygravs[0].thisbright);  // Add a bit more saturation to the colour.
    }
  }
  Serial.println("fireup");
} // fireup()


void explodeinit_1 () {
  for (int k=1; k< numgravs; k++) {
    mygravs[k].distold = mygravs[0].distance;                 // We start off with a height where the original element stopped.
    mygravs[k].distance = mygravs[0].distance;
    mygravs[k].velold = random16(-50, explosionvelocity);     // Kablooey!!
    mygravs[k].velocity = mygravs[k].velold;
    mygravs[k].thishue = random8();                           // Different coloured elements, which are
    mygravs[k].thissat = thissat;                             // rich in colour
    mygravs[k].thisbright = thisbright;                       // and very bright.
  }
  mycount_1 = 0;                                                // This is a counter for the # of times to go through the exploding loop.
  thisstatus_1 = 3;                                             // Once done initializing, let's move onto the next step of exploding.
  Serial.println("explodeinit");
} // explodeinit()


void explode_1() {

  if (mycount_1++ >=maxcount) thisstatus_1 = 0;                   // Only do this so many times.

  for (int k=1; k < numgravs; k++) {

    mygravs[k].velocity = mygravs[k].velold + gravity*timeinc;               // Split gravity math into two lines for simplicity
    mygravs[k].distance = mygravs[k].distold + mygravs[k].velocity*timeinc;

    int i = map(mygravs[k].distance, 0, 32767, 0, NUM_LEDS_1);
    
    mygravs[k].velold = mygravs[k].velocity;                                 // Capture the current velocity/distance
    mygravs[k].distold = mygravs[k].distance;

    if (mygravs[k].distance < 0) {mygravs[k].thisbright = 0;}                // It landed. Sorry, no bounce.
    
    if (i < NUM_LEDS_1 && i >= 0) led1[i] += CHSV(mygravs[k].thishue, thissat, thisbright);    // Let's get ready to display it, but be careful of limits.
  }
  for (int j = 0; j < NUM_LEDS_1; j++) led1[j].fadeToBlackBy(16);              // Fade everything over time. I could also use nscale8(224) or thereabouts.
  Serial.println("exploded");
} // explode()




// ******************************************************************************************************************************




void fire_2() {                                                 // States of fireworks, need to be loop friendly.
  switch (thisstatus_2) {
    case 0: firestart_2(); break;                               // Initialize the first element of array to shoot into the 'sky'.
    case 1: fireup_2(); break;                                  // Send it up. Once it reaches the top, we change status to '2'
    case 2: explodeinit_2(); break;                             // Initialize the array for the remainder of the fireworks elements. and change status.
    case 3: explode_2(); break;                                 // Now, let's explode them for a count duration and reset to 0 when done.
    default: break;
  }
} // fire()


void firestart_2() {                                            // Initialize variable for a new streamer being shot into the 'sky'.
  mygravs[0].distold = 0;
  mygravs[0].distance = 0;
  mygravs[0].velold = streamervelocity;                       // Goes to a maximum distance of ~18000. Needs more LED's
  mygravs[0].velocity = streamervelocity;
  mygravs[0].thishue = random8();                             // Might as well make it multi-coloured
  mygravs[0].thissat = 50;                                    // But not very saturated
  mygravs[0].thisbright = 128;
  thisstatus_2 = 1;
  Serial.println("firestart2");
} // firestart()


void fireup_2() {                                               // Shoot fireworks into the air.

  mygravs[0].velocity = mygravs[0].velold + gravity*timeinc;  // Split gravity math into two lines for simplicity.
  mygravs[0].distance = mygravs[0].distold + mygravs[0].velocity*timeinc;

  int i = map(mygravs[0].distance, 0, 32767, 0, NUM_LEDS_2);
    
  mygravs[0].velold = mygravs[0].velocity;                    // Capture the current velocity/distance.
  mygravs[0].distold = mygravs[0].distance;

  if (i < NUM_LEDS_2) led2[i] = CHSV(mygravs[0].thishue, mygravs[0].thissat, mygravs[0].thisbright);     // Let's get ready to display it, but not if it's too high.
  if(mygravs[0].velold <= 0) thisstatus_2 = 2;                  // Starting to fall, so let's stop this nonsense.

  for (int j = 0; j < NUM_LEDS_2; j++) led2[j].fadeToBlackBy(16);  // The trail fades slowly over time.

  if (i > 3) {                                                // Add a bit of a sparkly trailer above the ground, but make sure that trailer is ABOVE the ground.
    if (random8() >220) {
      led2[i-3] = CHSV(mygravs[0].thishue, random8(100, 200), mygravs[0].thisbright);  // Add a bit more saturation to the colour.
    }
  }
  Serial.println("fireup2");
} // fireup()


void explodeinit_2 () {
  for (int k=1; k< numgravs; k++) {
    mygravs[k].distold = mygravs[0].distance;                 // We start off with a height where the original element stopped.
    mygravs[k].distance = mygravs[0].distance;
    mygravs[k].velold = random16(-50, explosionvelocity);     // Kablooey!!
    mygravs[k].velocity = mygravs[k].velold;
    mygravs[k].thishue = random8();                           // Different coloured elements, which are
    mygravs[k].thissat = thissat;                             // rich in colour
    mygravs[k].thisbright = thisbright;                       // and very bright.
  }
  mycount_2 = 0;                                                // This is a counter for the # of times to go through the exploding loop.
  thisstatus_2 = 3;                                             // Once done initializing, let's move onto the next step of exploding.
  Serial.println("explodeinit2");
} // explodeinit()


void explode_2() {

  if (mycount_2++ >=maxcount) thisstatus_2 = 0;                   // Only do this so many times.

  for (int k=1; k < numgravs; k++) {

    mygravs[k].velocity = mygravs[k].velold + gravity*timeinc;               // Split gravity math into two lines for simplicity
    mygravs[k].distance = mygravs[k].distold + mygravs[k].velocity*timeinc;

    int i = map(mygravs[k].distance, 0, 32767, 0, NUM_LEDS_2);
    
    mygravs[k].velold = mygravs[k].velocity;                                 // Capture the current velocity/distance
    mygravs[k].distold = mygravs[k].distance;

    if (mygravs[k].distance < 0) {mygravs[k].thisbright = 0;}                // It landed. Sorry, no bounce.
    
    if (i < NUM_LEDS_2 && i >= 0) led2[i] += CHSV(mygravs[k].thishue, thissat, thisbright);    // Let's get ready to display it, but be careful of limits.
  }
  for (int j = 0; j < NUM_LEDS_2; j++) led2[j].fadeToBlackBy(16);              // Fade everything over time. I could also use nscale8(224) or thereabouts.
  Serial.println("exploded2");
} // explode()
