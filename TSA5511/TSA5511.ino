////////////////////////////////////////////////
//                                            //
//           ArduFMPLL by MichTronics         //
//                      v0.1                  //
//                Working with the            //
//        DRFS6 v2.0 3meter FM 6W TX Kit      //
//                                            //
////////////////////////////////////////////////

#include <Wire.h>
#include <OneButton.h>
#include <SimpleRotary.h>
#include <LiquidCrystal_I2C.h>

#define encoderClk 2
#define encoderData 3

int readLbs = 1;
int configScreenEnable = 0;
int clicks = 0;
int configMenuLine = 1;
int menuLine = 1;

//bool up = false;
//bool down = false;
//bool middle = false;

byte rotaryPos;

byte arrowChar[] = {
  B01100,
  B00110,
  B00011,
  B11111,
  B11111,
  B00011,
  B00110,
  B01100
};

LiquidCrystal_I2C lcd(0x3f, 20, 4);
OneButton button(A0,true);
SimpleRotary rotary(2,3,4);

void setup()
{
  Wire.begin(); // join i2c bus
  Serial.begin(9600);
  
  rotary.setTrigger(HIGH);
  rotary.setDebounceDelay(5);
  rotary.setErrorDelay(250);

  button.attachClick(singleClick);
  button.attachDoubleClick(doubleClick);
  button.attachPress(longClick);
  
  lcd.init();
  lcd.backlight();
  lcd.setCursor(4,0);
  lcd.print("MichTronics");
  lcd.setCursor(3,1);
  lcd.print("ArduFMPLL v0.1");
  lcd.setCursor(0,3);
  lcd.print("Radio West-Friesland");
  delay(1000);
  lcd.clear();
//  lcd.createChar(0, arrowChar);
//  lcd.setCursor(0,1);
//  lcd.write(0);
  
//  pll_set_frequency(94500);
}

void loop()
{  
  button.tick();
  
//  if (readLbs == 1){
//    readLockbyte();
//  }

  rotaryPos = rotary.rotate();
  //Serial.println(menuLine);
  if ( rotaryPos == 1 ) {
    //Serial.println("CW");
    if (menuLine <= 3){
      menuLine++;
      //Serial.println(menuLine);
    }
  } else if ( rotaryPos == 2 ) {
    //Serial.println("CCW");
    if (menuLine >= 1){
      menuLine--;
      //Serial.println(menuLine);
    }
  }
  startScreen();
  configScreen();
  
  
  //pll_set_frequency(94500);
}

void startScreen() {
  //Serial.println(menuLine);
  if (configScreenEnable == 0){
    lcd.setCursor(7, 0);
    lcd.print(" MAIN ");  
  }
//  if (rotaryPos == 1 && configScreenEnable == 0) {
//    lcd.createChar(0, arrowChar);
//    if (menuLine <= 3) {
//      lcd.clear();
//      lcd.setCursor(0,menuLine);
//      lcd.write(0);
//    } 
////    Serial.println("RIGHT");
//  } else if (rotaryPos == 2 && configScreenEnable == 0) {
//    lcd.createChar(0, arrowChar);
//    if (menuLine >= 1 ) {
//      lcd.clear();
//      lcd.setCursor(0,menuLine);
//      lcd.write(0);
//    }
////    Serial.println("LEFT");
//  }
}

void configScreen() {
  if (clicks == 1 & configScreenEnable == 1){
    if(configScreenEnable == 1){
      //lcd.clear();
      lcd.setCursor(4,0);
      lcd.print("Instellingen");
    }
    if (rotaryPos == 1 & configScreenEnable == 1){
      lcd.createChar(0, arrowChar);
      if (menuLine <= 3) {
        lcd.clear();
        //configMenuLine++;
        lcd.setCursor(0,menuLine);
        lcd.write(0);
      }            
      //Serial.println("LEFT");
    }
    if (rotaryPos == 2 & configScreenEnable == 1) {
    lcd.createChar(0, arrowChar);
    if (menuLine >= 1 ) {
      lcd.clear();
      //configMenuLine--;
      lcd.setCursor(0,menuLine);
      lcd.write(0);
    }
    //Serial.println("RIGHT");
  }
  }
}

void pll_set_frequency(long pllfreq) {
  char data[2];
  pllfreq = pllfreq / 8;
  pllfreq = pllfreq / 1;
  pllfreq = pllfreq * 1000;
  pllfreq = pllfreq / 6250;
  data[0] = (pllfreq & 0xFF00) >> 8;
  data[1] = pllfreq & 0x00FF;

  // Init byte
  Wire.beginTransmission(96);
  Wire.write(0xCE);
  Wire.write(0x00);
  Wire.endTransmission();

  delay(12);

  // Frequentie byte
  Wire.beginTransmission(96);
  Wire.write(data, 2);
  Wire.endTransmission();

  delay(27);

  // Lock byte
  Wire.beginTransmission(96);
  Wire.write(0xCE);
  Wire.write(0x20);
  Wire.endTransmission();
}

void readLockbyte(){
  Wire.requestFrom(96, 2);
  if ( Wire.available() >= 1 | readLbs == 1)
  {
    if (Wire.read() == 0x48){
      //Serial.println("Lock");
      startTransmitting();
      readLbs = 0;
    } else
    {
      //Serial.println("NoLock");
    }
  }
}

void startTransmitting(){
  // byte begin transmitting DRFS6 v2.0
  Wire.beginTransmission(96);
  Wire.write(0xCE);
  Wire.write(0x24);
  Wire.endTransmission();
}

void singleClick() {
  Serial.println("SINGLE");
  lcd.clear();
  lcd.createChar(0, arrowChar);
  lcd.setCursor(0,1);
  lcd.write(0);
  clicks = 1;
  configScreenEnable = 1; 
}

void doubleClick() {
  Serial.println("DOUBLE");
}

void longClick() {
  Serial.println("LONG");
  lcd.clear();
//  lcd.createChar(0, arrowChar);
//  lcd.setCursor(0,1);
//  lcd.write(0);
  clicks = 0;
  configScreenEnable = 0;
  startScreen();
}
