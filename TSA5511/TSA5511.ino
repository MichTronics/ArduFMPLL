////////////////////////////////////////
//                                    //
//       ArduFMPLL by MichTronics     //
//                  v0.1              //
//                                    //
////////////////////////////////////////

#include <Wire.h>

void setup()
{
  Wire.begin(); // join i2c bus
  setfreq(); // Test function to set freq
}

void loop()
{

}
void setfreq(){
  Wire.beginTransmission(96);
  Wire.write(0xCE);
  Wire.write(0x00);
  Wire.endTransmission();
  delay(12);
  pll_set_frequency(94500);
  delay(27);
  Wire.beginTransmission(96);
  Wire.write(0xCE);
  Wire.write(0x20);
  Wire.endTransmission();
  delay(115);
}

void pll_set_frequency(long pllfreq) {
  char init[2];
  char data[2];
  char end[2];
  pllfreq = pllfreq / 8;
  pllfreq = pllfreq / 1;
  pllfreq = pllfreq * 1000;
  pllfreq = pllfreq / 6250;
  //init[0] = 0xCE;
  //init[1] = 0x00;
  data[0] = (pllfreq & 0xFF00) >> 8;
  data[1] = pllfreq & 0x00FF;
  //end[0] = 0xCE;
  //end[1] = 0x20;
  //Wire.beginTransmission(96);
  //Wire.write(init, 2);
  //Wire.endTransmission();
  //delay(12);
  Wire.beginTransmission(96);
  Wire.write(data, 2);
  Wire.endTransmission();
  //delay(26);
  //Wire.beginTransmission(96);
  //Wire.write(end, 2);
  //Wire.endTransmission();
  //delay(115);
}
