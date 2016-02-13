#include "FastLED.h"

#include "Timer.h"

FASTLED_USING_NAMESPACE

// FastLED "100-lines-of-code" demo reel, showing just a few 
// of the kinds of animation patterns you can quickly and easily 
// compose using FastLED.  
//
// This example also shows one easy way to define multiple 
// animations patterns and have them automatically rotate.
//
// -Mark Kriegsman, December 2014

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif


#define FORWARD 0
#define BACKWARD 1
#define SLOW 250
#define MEDIUM 50
#define FAST 5


boolean direction = FORWARD;


#define DATA_PIN    2
//#define CLK_PIN   1
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS    51
CRGB leds[NUM_LEDS];

#define BRIGHTNESS          255
#define FRAMES_PER_SECOND  10000


//Delay for each cycle of random colors.
int dly = 0; //Adjust to your liking, Default: 10
int tdly = 250;
int pdly = 100;
int greenDLY = 50;

Timer t;

int cycle = 0;

int patternNum;
int rnum = 0;
//pulse
int pled = 0;
int pled1 = 0;
int pled2 = 0;
int pled3 = 0;
int pled4 = 0;

int mode1 = 3;
int mode2 = 4;
int mode3 = 5;
int modeA = 0;
int analog = 0;
int analog2 = 0;

int a = 0;
int b = 0;
int c = 0;
int d = 0;
int e = 0;
int f = 0;

int doFlash = 0;

void setup() {
  delay(3000); // 3 second delay for recovery
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

t.every(1000,incCycle);
  // set master BRIGHTNESS control
  FastLED.setBrightness(BRIGHTNESS);
  Serial.begin(115200);
}


// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { rainbow, juggleRed, juggleBlue, autoGreen, randColor, sinelonBlue, sinelonRed, nothing };

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns


void loop()
{
  t.update();
  
int mode = digitalRead(mode1)
           | (digitalRead(mode2) << 1)
           | (digitalRead(mode3) << 2);
           
           
    //Serial.println(mode);
    analog = analogRead(modeA);
    
    
    analogAdjust();
      

    
gPatt(mode);

//bpm();

  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow

}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))



void gPatt(int patternNum){
  // Call the currennt pattern function once, updating the 'leds' array
  gPatterns[patternNum]();

  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND);


}


void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16(13,0,NUM_LEDS);
  leds[pos] += CHSV( gHue, 255, BRIGHTNESS);
}

void sinelonRed()
{
  // a colored dot sweeping back and forth, with fading trails
 if(doFlash == 0){
  fadeToBlackBy( leds, NUM_LEDS, analog2/10);
  int pos = beatsin16(analog2/10,0,NUM_LEDS);
  leds[pos] += CHSV( 0, 255, BRIGHTNESS);
  }
  
  else if(doFlash == 1){
    fill_solid( leds, NUM_LEDS, CRGB(255,0,0));
      FastLED.show(); 
    delay(50);
    fill_solid( leds, NUM_LEDS, CRGB(0,0,0));
      FastLED.show(); 
    delay(50);
}
}

void sinelonBlue()
{
  // a colored dot sweeping back and forth, with fading trails
  if(doFlash == 0){
  fadeToBlackBy( leds, NUM_LEDS, analog2/10);
  int pos = beatsin16(analog2/10,0,NUM_LEDS);
  leds[pos] += CHSV( 160, 255, BRIGHTNESS);
  }
  
  else if(doFlash == 1){
    fill_solid( leds, NUM_LEDS, CRGB(0,0,255));
      FastLED.show(); 
    delay(50);
    fill_solid( leds, NUM_LEDS, CRGB(0,0,0));
      FastLED.show(); 
    delay(50);
  }
  
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = CHSV( 160, 255, BRIGHTNESS);
  }
}

void bpmRed()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = CHSV( 160, 255, BRIGHTNESS);
  }
}


void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16(i+7,0,NUM_LEDS)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

void juggleRed() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 14; i++) {
    leds[beatsin16(i+7,0,NUM_LEDS)] |= CHSV(dothue, 200, 255);
    //dothue += 32;
  }
}

void autoGreen() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 96;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16(i+7,0,NUM_LEDS)] |= CHSV(dothue, 200, 255);
    //dothue += 32;
  }
}


void juggleBlue() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 160;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16(i+7,0,NUM_LEDS)] |= CHSV(dothue, 200, 255);
    //dothue += 32;
  }
}
 
void randColor(){
  //Randomly Flashes Color on Each LED
  //While Loop
  a = 0;
 while(a < NUM_LEDS){
 	leds[a] = CHSV( random(50,256), 255, BRIGHTNESS);
 	//FastLED.delay(500/FRAMES_PER_SECOND);
 	a++;
 }
 
}




void doTimer(){
            //Do timer
           //Run Function 1: 15sec 
           if(cycle <= 15){
              gPatt(0);
              //scamTheGreen();
            }
           //Run Function 2: 15sec  
           if(cycle > 15 && cycle <= 30){
              
            }
           if(cycle > 30 && cycle <= 45){
               randColor();
               pulseLeds();
               scamTheGreen();
            }
           if(cycle > 45 && cycle <= 60){
               randAllColor();
               //scamTheGreen();
            }
           //Run Function 3: 15sec
           // if(cycle > 30 && cycle <= 45){
              // tjColors();
           // }
           //Reset cycle
           if(cycle > 60){
 
              cycle = 0;
    
            }
}

void randAllColor(){
  //Randomly Flashes Color on Each LED
  //While Loop
  rnum = random(50,256);
  b = 0;
 while(b < NUM_LEDS){
 	leds[b] = CHSV(rnum,255,BRIGHTNESS);
 	//Wait
 	delay(dly);
 	//Write to LED Strip
 	FastLED.show();
 	b++;
 }
}

/*
void tjColors(){
  f = 0;
  while(f < NUM_LEDS){
    leds[f] = CHSV(64,255,BRIGHTNESS);
    FastLED.show();
    f++;
    leds[f] = CHSV(32,255,BRIGHTNESS);
    FastLED.show();
    f++;
    leds[f] = CHSV(224,255,BRIGHTNESS);
    FastLED.show();
    f++;
    leds[f] = CHSV(193,255,BRIGHTNESS);
    FastLED.show();
    f++;
  }
}
*/

 void testGreen(){
 c = 0;
 while(c < NUM_LEDS){
    leds[c].setRGB(255,0,0);
    //FastLED.show();
    c++;

 }
    FastLED.show();
   delay(tdly);
 }
 
 
 void testRed(){
   d = 0;
 while( d < NUM_LEDS ){  
    leds[d].setRGB(0,255,0);
    d++;
    //FastLED.show();
  }
    FastLED.show();
   delay(tdly);
 }
 
 
  void scamTheGreen(){
   d = 0;
 while( d < NUM_LEDS ){  
    leds[d].setRGB(255,0,0);
    d++;
    //FastLED.show();
  }
    FastLED.show();
   delay(greenDLY);
 }
 
 
 void testBlue(){
   e = 0;
 while( e < NUM_LEDS ){  
    leds[e].setRGB(0,0,255);
    e++;
    //FastLED.show();
  }
    FastLED.show();
   delay(tdly);
 }
 
 void pulseLeds(){
   pled = BRIGHTNESS;
   FastLED.setBrightness(pled);
   FastLED.show();
   delay(pdly);
   pled1 = pled-(pled/4);
   FastLED.setBrightness(pled1);
   FastLED.show();
   delay(pdly);
   pled2 = pled1-(pled1/4);
   FastLED.setBrightness(pled2);
   FastLED.show();
   delay(pdly);
   pled3 = pled2-(pled2/4);
   FastLED.setBrightness(pled3);
   FastLED.show();
   delay(pdly);
   pled4 = pled3-(pled3/4);
   FastLED.setBrightness(pled4);
   FastLED.show();
   delay(pdly);
   FastLED.setBrightness(pled3);
   FastLED.show();
   delay(pdly);
   FastLED.setBrightness(pled2);
   FastLED.show();
   delay(pdly);
   FastLED.setBrightness(pled1);
   FastLED.show();
   delay(pdly);
   FastLED.setBrightness(pled);
   FastLED.show();
   delay(pdly);
   
   
 }
   
 // Changes all LEDS to given color
void allColor(CRGB c){
  for(int i=0; i<NUM_LEDS; i++){
    leds[i] = c;
  }
  FastLED.show();
}

void allRandom(){
  for(int i=0; i<NUM_LEDS; i++){
    leds[i] = randomColor();
  }
  FastLED.show(); 
}

// Random disolve colors
void disolve(int simultaneous, int cycles, int speed){
  for(int i=0; i<cycles; i++){
    for(int j=0; j<simultaneous; j++){
      int idx = random(NUM_LEDS);
      leds[idx] = CRGB::Black;
    }
    FastLED.show();
    delay(speed);
  }

  allColor(CRGB::Black);
}

// Flashes given color
// If c==NULL, random color flash
void flash(CRGB c, int count, int speed){
  for(int i=0; i<count; i++){
    if(c){
      allColor(c);
    }
    else{
      allColor(randomColor());
    }
    delay(speed);
    allColor(CRGB::Black);
    delay(speed);
  }
}

// Wipes color from end to end
void colorWipe(CRGB c, int speed, int direction){
  for(int i=0; i<NUM_LEDS; i++){
    if(direction == FORWARD){
      leds[i] = c;
    }
    else{
      leds[NUM_LEDS-1-i] = c;
    }
    FastLED.show();
    delay(speed);
  }
}

// Rainbow colors that slowly cycle across LEDs
void rainboW(int cycles, int speed){ // TODO direction
  if(cycles == 0){
    for(int i=0; i< NUM_LEDS; i++) {
      leds[i] = Wheel(((i * 256 / NUM_LEDS)) & 255);
    }
    FastLED.show();
  }
  else{
    for(int j=0; j<256*cycles; j++) {
      for(int i=0; i< NUM_LEDS; i++) {
        leds[i] = Wheel(((i * 256 / NUM_LEDS) + j) & 255);
      }
      FastLED.show();
      delay(speed);
    }
  }
}

// Theater-style crawling lights
void theaterChase(CRGB c, int cycles, int speed){ // TODO direction

  for (int j=0; j<cycles; j++) {  
    for (int q=0; q < 3; q++) {
      for (int i=0; i < NUM_LEDS; i=i+3) {
        int pos = i+q;
        leds[pos] = c;    //turn every third pixel on
      }
      FastLED.show();

      delay(speed);

      for (int i=0; i < NUM_LEDS; i=i+3) {
        leds[i+q] = CRGB::Black;        //turn every third pixel off
      }
    }
  }
}

// Theater-style crawling lights with rainbow effect
void theaterChaseRainbow(int cycles, int speed){ // TODO direction, duration
  for (int j=0; j < 256 * cycles; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (int i=0; i < NUM_LEDS; i=i+3) {
        int pos = i+q;
        leds[pos] = Wheel( (i+j) % 255);    //turn every third pixel on
      }
      FastLED.show();

      delay(speed);

      for (int i=0; i < NUM_LEDS; i=i+3) {
        leds[i+q] = CRGB::Black;  //turn every third pixel off
      }
    }
  }
}

// Random flashes of lightning
void lightning(CRGB c, int simultaneous, int cycles, int speed){
  int flashes[simultaneous];

  for(int i=0; i<cycles; i++){
    for(int j=0; j<simultaneous; j++){
      int idx = random(NUM_LEDS);
      flashes[j] = idx;
      leds[idx] = c ? c : randomColor();
    }
    FastLED.show();
    delay(speed);
    for(int s=0; s<simultaneous; s++){
      leds[flashes[s]] = CRGB::Black;
    }
    delay(speed);
  }
}

// Sliding bar across LEDs
void cylon(CRGB c, int width, int speed){
  // First slide the leds in one direction
  for(int i = 0; i <= NUM_LEDS-width; i++) {
    for(int j=0; j<width; j++){
      leds[i+j] = c;
    }

    FastLED.show();

    // now that we've shown the leds, reset to black for next loop
    for(int j=0; j<5; j++){
      leds[i+j] = CRGB::Black;
    }
    delay(speed);
  }

  // Now go in the other direction.  
  for(int i = NUM_LEDS-width; i >= 0; i--) {
    for(int j=0; j<width; j++){
      leds[i+j] = c;
    }
    FastLED.show();
    for(int j=0; j<width; j++){
      leds[i+j] = CRGB::Black;
    }

    delay(speed);
  }
}

// Display alternating stripes
void stripes(CRGB c1, CRGB c2, int width){
  for(int i=0; i<NUM_LEDS; i++){
    if(i % (width * 2) < width){
      leds[i] = c1;
    }
    else{
      leds[i] = c2;
    } 
  }
  FastLED.show();
}

// Theater-style crawling of stripes
void stripesChase(CRGB c1, CRGB c2, int width, int cycles, int speed){ // TODO direction

}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
CRGB Wheel(byte WheelPos) {
  if(WheelPos < 85) {
    return CRGB(WheelPos * 3, 255 - WheelPos * 3, 0);
  } 
  else if(WheelPos < 170) {
    WheelPos -= 85;
    return CRGB(255 - WheelPos * 3, 0, WheelPos * 3);
  } 
  else {
    WheelPos -= 170;
    return CRGB(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

CRGB randomColor(){
  return Wheel(random(256)); 
}



//Seconds Incrementer  
void incCycle() 
{
  cycle++;
}

void analogAdjust()
{

        if(analog<=49){
              analog2 = 10;
              doFlash = 0;
        }
              
              
        else if(50<=analog && analog<=150){
              analog2 = 100;
              doFlash = 0;
        }
              
              
        else if(151<=analog && analog<=250){
              analog2 = 200;
              doFlash = 0;
        }
              
              
        else if(251<=analog && analog<=350){
              analog2 = 300;
              doFlash = 0;
        }
              
              
        else if(351<=analog && analog<=450){
              analog2 = 400;
              doFlash = 0;
        }
              
              
        else if(451<=analog && analog<=550){
              analog2 = 500;
              doFlash = 0;
        }
              
              
        else if(551<=analog && analog<=650){
              analog2 = 600;
              doFlash = 0;
         }
              
              
        else if(651<=analog && analog<=750){
              analog2 = 700;
              doFlash = 0;
         }
              
              
        else if(751<=analog && analog<=850){
              analog2 = 800;
              doFlash = 0;
         }
              
              
        else if(851<=analog && analog<=950){
              analog2 = 900;
              doFlash = 0;
         }
              
              
        else if(951<=analog){
              analog2 = 1000;
              doFlash = 1;
         }
              
              

      
      
   // Serial.println(analog);
    Serial.println(analog2); 
}

void fill_solid( struct CRGB * leds, int numToFill,
                 const struct CRGB& color);

void nothing(){
}


  
  
  
  

  
  



