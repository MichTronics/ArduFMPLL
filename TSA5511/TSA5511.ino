////////////////////////////////////////////////
//                                            //
//           ArduFMPLL by MichTronics         //
//                      v0.1                  //
//  Working with the DRFS6 v2.0 FM 6W TX Kit  //
//                                            //
////////////////////////////////////////////////

#include <Wire.h>

int readLb = 1;

void setup()
{
  Serial.begin(9600);
  Wire.begin(); // join i2c bus
  pll_set_frequency(94500);
}

void loop()
{
  if (readLb == 1){
    readLockbyte();
  }
  //pll_set_frequency(94500);
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
  if ( Wire.available() >= 1 | readLb == 1)
  {
    if (Wire.read() == 0x48){
      Serial.println("Lock");
      startTransmitting();
      readLb = 0;
    } else
    {
      Serial.println("NoLock");
    }
    //Serial.println( result, HEX );
  }
}

void startTransmitting(){
  // byte begin transmitting DRFS6 v2.0
  Wire.beginTransmission(96);
  Wire.write(0xCE);
  Wire.write(0x24);
  Wire.endTransmission();
}
