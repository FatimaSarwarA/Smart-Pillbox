#include<SPI.h>
#include<Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display( OLED_RESET);
#define button         A0
#define NUMFLAKES     10 
#include <HX711_ADC.h> // https://github.com/olkal/HX711_ADC

HX711_ADC LoadCell(2, 3); // parameters: dt pin, sck pin

const int buzzerPin = 12;
const int taree = 13;
float weight = 0.9;   //Weight of one pill (set as necessary)
unsigned int secondsCount = 0;
int a = 0;
float b = 0;
int previousCount;
int currentCount;
bool pill_taken;

void setup() {
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);

  
  noInterrupts();          // Disable interrupts
  TCCR1A = 0;              // Clear Timer1 control registers
  TCCR1B = 0;

  // Set Timer1 to CTC mode (Clear Timer on Compare Match)
  TCCR1B |= (1 << WGM12);

  // Set Timer1 prescaler to 1024 (16 MHz / 1024 = 15625 Hz)
  TCCR1B |= (1 << CS12) | (1 << CS10);

  // Set OCR1A for 1-second interrupts
  OCR1A = 15624;

  // Enable Timer1 compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  interrupts(); 
  
  Serial.begin(9600);
   
  
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.print("Starting.....");
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  // Draw a single pixel in white
  display.drawPixel(10, 10, SSD1306_WHITE);

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
  delay(2000);


  pinMode (button, INPUT);
  LoadCell.begin(); // start connection to HX711
  LoadCell.start(2000); // load cells gets 2000ms of time to stabilize
  LoadCell.setCalFactor(-1850.26); // calibration factor for load cell => strongly dependent on your individual setup
testdrawrect();
delay(200);
  display.clearDisplay();
intro();
delay(2000); 
  display.clearDisplay();
testdrawcircle();
delay(20);
  display.clearDisplay();


}
 void intro() {
 display.setTextSize(1.5);
 display.setTextColor(WHITE);
 display.setCursor(4,4);
 display.println("SMART PILLBOX");
display.setTextSize(1);
 display.setCursor(20,20);
 display.println("EMBEDDED PROJECT");
 display.display();

}
void testdrawcircle(void) {
  display.clearDisplay();

  for(int16_t i=0; i<max(display.width(),display.height())/2; i+=4) {
    display.drawCircle(display.width()/2, display.height()/2, i, SSD1306_WHITE);
    display.display();
    delay(1);
  }

  delay(2000);
}
void testdrawrect(void) {
  display.clearDisplay();

  for(int16_t i=0; i<display.height()/2; i+=2) {
    display.drawRect(i, i, display.width()-2*i, display.height()-2*i, SSD1306_WHITE);
    display.display(); // Update screen with each newly-drawn rectangle
    delay(1);
  }

  delay(2000);
}

float count(float i)
{
  return round(i/weight);
}

ISR(TIMER1_COMPA_vect) {
  secondsCount++;
    previousCount = currentCount;
  if (secondsCount == 30) { // 120 seconds = 2 minutes
    digitalWrite(buzzerPin, HIGH); // Turn on the buzzer
    Serial.println("TAKE THE PILL");
    delay(1000000);
    digitalWrite(buzzerPin, LOW);
    currentCount = count(LoadCell.getData());
    if(currentCount < previousCount)
     { pill_taken = true;
      Serial.print("Pill Taken");
     }
    else {
      pill_taken = false;
      Serial.print("Pill not Taken");
    }
  }
  else if(secondsCount == 60) {
    secondsCount = 0;
    if(pill_taken == true)
      Serial.println("All pills were taken on time.");
    else 
      Serial.println("You missed taking the Pill.");
  }
}

void loop() { 
  
 display.setTextSize(1);
 display.setTextColor(WHITE);
 display.setCursor(0,1);
 display.println("Pills");

  
  
  LoadCell.update(); // retrieves data from the load cell
  float i = LoadCell.getData(); // get output value
  currentCount = count(i);
 if (i<0)
 {
   i = i * (-1);
   display.setCursor(10,12);
  display.print("-");
 }
    display.setCursor(10,12);
    display.print("Count:");
     display.setTextSize(2);
     display.setCursor(48,12);
     display.print(currentCount, 1);
 
  

//  if (i>=1000)
//  {
//    display.clearDisplay();
//    display.setTextSize(1);
// display.setTextColor(WHITE);
// display.setCursor(0,1);
// display.println("OVER LOAD");
//  }

  if (digitalRead (taree) == HIGH)
  {
    display.clearDisplay();
    delay(5000);
      
    display.clearDisplay();
    LoadCell.start(2000);
  
  }
  delay(10);
  
  display.display();
  display.clearDisplay();
  
}
