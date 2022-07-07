#include <Adafruit_LiquidCrystal.h>
#include <EEPROM.h>
//Visitors Counter and Automatic Room Light
/*
    LCD Pin     -  NANO Pin
    VSS, RW, K  -  GND
    VDD, A      -  5v
    V0  - D10    FOR CONTRAST
    RS  - D12
    E   - D11
    D4  - D5
    D5  - D4
    D6  - D3
    D7  - D2
*/
//#include <LiquidCrystal.h>
Adafruit_LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

#define in A5     //21
#define out A4    //19
#define relay A3  //17 relay pin
#define pushBtn 9 // to reset counter
#define count_loc 00 //eeprom location to store the count.
#define light_state_loc 01 // eeprom location to store last state of light 

int Contrast = 60;
float pressed_ms = 1500; //delay for push button to be pressed
unsigned long press_begin_ms = 0; // will be used to store the time elapsed while button is pressed.

int count = EEPROM.read(count_loc);

void walkIn() {
  count++;
  EEPROM.write(count_loc, count);
  Serial.println(count);

  /*when first person enters then.
       turn on relay
       clear lcd and print People in room

  */
  if (count == 1) {
    // change this to high as per your relay connection set the same inside setup too
    // and set the opposite inside WalkOut function.
    digitalWrite(relay, LOW);

    //    digitalWrite(13, HIGH);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("People In Room:");
  }

  lcd.setCursor(0, 1);
  lcd.print(count);
}

void walkOut() {
  count--;
  //reset count to zero if it goes negative
  if (count < 0)
    count = 0;
  EEPROM.write(count_loc, count);


  /*
         when last person walks out
         turn off relay
         clear lcd and display room is empty
  */
  if (count == 0) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Room Is Empty..");
    lcd.setCursor(0, 1);
    lcd.print("Light Is Off");
    digitalWrite(relay, HIGH);      //set it as high or low as per the conection of relay.
    //    digitalWrite(13, LOW);
  } else {
    lcd.print("People In Room:");
    Serial.println(count);
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(0, 1);
    lcd.print(count);
  }

  //delay(1000);
}

void setup() {
  //code written here runs only at the beginning and never again

  Serial.begin(9600);
  delay(1000);
  analogWrite(10, Contrast);
  lcd.begin(16, 2);

  lcd.setCursor(0, 0);
  lcd.print("Visitor Counter");
  delay(1000);
  lcd.setCursor(8, 1);
  lcd.print("-By RSR");
  delay(1000);

  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("*    *");
  lcd.setCursor(6, 1);
  lcd.print("*__*");
  delay(500);

  pinMode(in, INPUT);
  pinMode(out, INPUT);
  pinMode(relay, OUTPUT);
  pinMode(pushBtn, INPUT_PULLUP);
  lcd.clear();
  if (count != 0) {
    digitalWrite(relay, LOW);
    count++;
    walkOut();
  }
  else {
    count++;
    walkOut();
  }
}


void loop() {
  // code written here runs in an endless loop.

  //for understanding IR sensor input
  //  Serial.print("\ndigitalread in = ");
  //  Serial.print(digitalRead(in));
  //  Serial.print(" out = ");
  //  Serial.print(digitalRead(out));
  //  delay(1000);

  /*
     ir detected = 0;
     no detected = 1;
  */
beg:

// touch any line means come between line of sight of the IR
  //digitalRead(in) and  digitalRead(out) will output 1 unless the person touches any line
  // digitalRead(pushBtn) return 1 until button is pressed. when button is pressed it will be grounded
  // hence returning zero.
  while (digitalRead(in) & digitalRead(out) & digitalRead( pushBtn)) {
  };  //waiting for the person to touch any line
  // touch any line means come between line of sight of the IR

  //    Serial.print("\nwaiting for the person to touch any line");

  if (digitalRead(pushBtn) == 0) //resetting count if push button is pressed
  {
    Serial.println("Reset button pressed");
    delay(100);
    press_begin_ms += 100;

    if (press_begin_ms >= pressed_ms) {
      press_begin_ms = 0;
      count = 0;
      lcd.clear();
      lcd.print("Resetting Count.");
      delay(1000);
      lcd.setCursor(0, 1);
      lcd.print("Count Reset to 0");
      Serial.println("Count Reset to 0");
      delay(2000);
      walkOut();
    }
  }
  else
    Serial.println("Person touched a line ");


  bool reset = false;
  unsigned long enter_millis = millis();


  if (!digitalRead(in) & digitalRead(out))
  {
    Serial.print("\nPerson touched 1st line to enter");

    while (digitalRead(out)) {    //Waiting for person to touch the  2nd line
      //      Serial.print("\n\tWaiting for person to touch the  2nd line ");
      if (millis() - enter_millis >= 2500) {
        //        Serial.print("\n!!!!!!THe person didnt touch the second line resetting");
        //        reset = true;
        break;
      }
    }
    if (reset)
      goto beg;

    //    Serial.print("\nPerson touched the second line ");

    //    Serial.print("\nMaking Sure Person Crosses the first line ");
    while (!digitalRead(in)); //Waiting for first line to be crossed
    //      Serial.print("\n\tWaiting for first line to be crossed.");

    //    Serial.print("\nMaking Sure Person Crosses the 2nd line ");
    while (!digitalRead(out)); //Waiting for 2nd line to be crossed.
    //       Serial.print("\n\tWaiting for 2nd line to be crossed.");

    Serial.print("\nThe Person Is Inside ");
    walkIn();
  }


  //checking for exit
  if (!digitalRead(out) & digitalRead(in))
  {
    Serial.print("\nPerson touched 1st line to exit");

    while (digitalRead(in)) { //Waiting for person to touch the  2nd line
      //      Serial.print("\n\tWaiting for person to touch the  2nd line ");
      if (millis() - enter_millis >= 2500) {
        //        Serial.print("\n!!!!!!THe person didnt touch the second line resetting");
        reset = true;
        break;
      }
    }

    if (reset)
      goto beg;

    //    Serial.print("\nPerson touched the second line ");
    //    Serial.print("\nMaking Sure Person Crosses the first line ");

    while (!digitalRead(out)); //Waiting for first line to be crossed.
    //      Serial.print("\n\tWaiting for first line to be crossed.");
    //    Serial.print("\nMaking Sure Person Crosses the 2nd line ");

    while (!digitalRead(in)); //
    //      Serial.print("\n\tWaiting for 2nd line to be crossed.");

    Serial.print("\nThe Person Left ");
    walkOut();
  }

  delay(100);
}
