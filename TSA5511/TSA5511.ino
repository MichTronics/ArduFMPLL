////////////////////////////////////////////////
//                                            //
//           ArduFMPLL by MichTronics         //
//                      v0.1                  //
//                Working with the            //
//        DRFS6 v2.0 3meter FM 6W TX Kit      //
//                                            //
////////////////////////////////////////////////

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

LiquidCrystal_I2C lcd(0x3F, 20, 4);

#define COUNT(x) sizeof(x)/sizeof(*x)  
int readLbs = 1;            
const byte pENCO_SW   = 2; 
const byte pENCO_DT   = 3;
const byte pENCO_CLK  = 4;    
const byte rowsLCD    = 4;
const byte columnsLCD = 20;
const byte iARROW     = 0;
const byte bARROW[]   = {   
  B00000, B00100, B00110, B11111,
  B00110, B00100, B00000, B00000
};

enum Button { Unknown, Ok, Left, Right } btnPressed;    
enum Screen { Menu1, Menu2, Menu3, Menu4, Flag, Number, Freq };      

const char *txMENU[] = {                          
  "Show Frequency     ",//0
  "Set Band           ",//1
  "Set Step           ",//2
  "Set Frequency      ",//3
  "Save and Quit      ",//4
  "Quit               "//13
};
const byte iMENU = COUNT(txMENU);

enum eSMENU1 { A_band, B_band, C_band };
const char *txSMENU1[] = {
  "        3m        ",
  "       70cm       ",
  "       23cm       "
};

enum eSMENU2 { A_step, B_step, C_step, D_step, E_step, F_step, G_step };
const char *txSMENU2[] = {
  "        1 KHz     ",
  "      100 KHz     ",
  "      250 KHz     ",
  "       1 Mhz      ",
  "      10 Mhz      ",
  "      50 Mhz      ",
  "      100 Mhz     "
};

long stepsizearray[] = {1, 100, 250, 1000, 10000, 50000, 100000};

struct MYDATA {     
  long initialized;
  long frq_show;
  long frq_band;
  long frq_step;
  long frq_set;
};
union MEMORY {
  MYDATA d;
  byte b[sizeof(MYDATA)];
}
memory;


void setup()
{
  pinMode(pENCO_SW,  INPUT_PULLUP);
  pinMode(pENCO_DT,  INPUT_PULLUP);
  pinMode(pENCO_CLK, INPUT_PULLUP);

  readConfiguration();

  Wire.begin();

  lcd.init();
  lcd.backlight();
  lcd.createChar(iARROW, bARROW);

  lcd.setCursor(5, 0);
  lcd.print("PieTronics");
  lcd.setCursor(3, 1);
  lcd.print("ArduFMPLL v0.1");
  lcd.setCursor(2, 2);
  lcd.print("Echo FM 94.5 Mhz");
  lcd.setCursor(0, 3);
  for ( int i = 0 ; i < columnsLCD ; i++ )
  {
    lcd.print(".");
    delay(150);
  }
  lcd.clear();
  pll_set_frequency(memory.d.frq_set);
  Serial.begin(9600);
}

void loop()
{
  static unsigned long tNow      = 0;
  static unsigned long tPrevious = 0;

  tNow = millis();
  btnPressed = readButtons();

  if (readLbs == 1){
    readLockbyte();
  }

  if ( btnPressed == Button::Ok )
    openMenu();


  if ( tNow - tPrevious >= 1000 )
  {
    tPrevious = tNow;

    if ( memory.d.frq_show == 1 )
      lcd.clear();

    if ( memory.d.frq_show == 1 )
    {
      lcd.setCursor(0, 0);
      lcd.print("     Frequentie     ");
      lcd.setCursor(5, 1);
      lcd.print(memory.d.frq_set / 1000);
      lcd.print(".");
      lcd.print(memory.d.frq_set - (memory.d.frq_set / 1000) * 1000);
      if (((memory.d.frq_set - (memory.d.frq_set / 1000) * 1000)) < 1) {
        lcd.print("00");
      }
      lcd.print(" Mhz  ");
      if (readLbs == 0){
        lcd.setCursor(0,3);
        lcd.print("   Locked    ");
      }
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

void openMenu()
{
  byte idxMenu       = 0;
  boolean exitMenu   = false;
  boolean forcePrint = true;

  lcd.clear();

  while ( !exitMenu )
  {
    btnPressed = readButtons();

    if ( btnPressed == Button::Left && idxMenu - 1 >= 0 )
    {
      idxMenu--;
    }
    else if ( btnPressed == Button::Right && idxMenu + 1 < iMENU )
    {
      idxMenu++;
    }
    else if ( btnPressed == Button::Ok )
    {
      switch ( idxMenu )
      {
        case 0: openSubMenu( idxMenu, Screen::Flag,   &memory.d.frq_show,  0, 1                   ); break;
        case 1: break;
        case 2: openSubMenu( idxMenu, Screen::Menu2, &memory.d.frq_step,   0, COUNT(txSMENU2) - 1 ); break;
        case 3: openSubMenu( idxMenu, Screen::Freq,  &memory.d.frq_set,   80000, 108000           ); break;
        case 4: writeConfiguration(); pll_set_frequency(memory.d.frq_set); exitMenu = true;                                               break; 
        case 5: readConfiguration();  exitMenu = true;                                               break;
      }
      forcePrint = true;
    }


    if ( !exitMenu && (forcePrint || btnPressed != Button::Unknown) )
    {
      forcePrint = false;

      static const byte endFor1 = (iMENU + rowsLCD - 1) / rowsLCD;
      int graphMenu     = 0;

      for ( int i = 1 ; i <= endFor1 ; i++ )
      {
        if ( idxMenu < i * rowsLCD )
        {
          graphMenu = (i - 1) * rowsLCD;
          break;
        }
      }

      byte endFor2 = graphMenu + rowsLCD;

      for ( int i = graphMenu, j = 0; i < endFor2 ; i++, j++ )
      {
        lcd.setCursor(1, j);
        lcd.print( (i < iMENU) ? txMENU[i] : "                    " );
      }

      for ( int i = 0 ; i < rowsLCD ; i++ )
      {
        lcd.setCursor(0, i);
        lcd.print(" ");
      }
      lcd.setCursor(0, idxMenu % rowsLCD );
      lcd.write(iARROW);
    }
  }

  lcd.clear();
}

void openSubMenu( byte menuID, Screen screen, long *value, long minValue, long maxValue )
{
  boolean exitSubMenu = false;
  boolean forcePrint  = true;
  int freq_set_screen = 0;

  lcd.clear();

  while ( !exitSubMenu )
  {
    btnPressed = readButtons();

    if ( btnPressed == Button::Ok )
    {
      exitSubMenu = true;
    }
    else if ( btnPressed == Button::Left && (*value) - 1 >= minValue )
    {
      if ( freq_set_screen == 1 ) {
        *value = *value - stepsizearray[memory.d.frq_step];
      } else {
        (*value)--;
      }
    }
    else if ( btnPressed == Button::Right && (*value) + 1 <= maxValue )
    {
      if ( freq_set_screen == 1) {
        *value = *value + stepsizearray[memory.d.frq_step];
      } else {
        (*value)++;
      }
    }


    if ( !exitSubMenu && (forcePrint || btnPressed != Button::Unknown) )
    {
      forcePrint = false;

      lcd.setCursor(0, 0);
      lcd.print(txMENU[menuID]);

      lcd.setCursor(0, 1);
      lcd.print("<");
      lcd.setCursor(columnsLCD - 1, 1);
      lcd.print(">");

      if ( screen == Screen::Menu1 )
      {
        lcd.setCursor(1, 1);
        lcd.print(txSMENU1[*value]);
      }
      else if ( screen == Screen::Menu2 )
      {
        lcd.setCursor(1, 1);
        lcd.print(txSMENU2[*value]);
      }
      else if ( screen == Screen::Flag )
      {
        lcd.setCursor(columnsLCD / 2 - 1, 1);
        lcd.print(*value == 0 ? "No " : "Yes");
      }
      else if ( screen == Screen::Number )
      {
        lcd.setCursor(columnsLCD / 2 - 1, 1);
        lcd.print(*value);
        lcd.print(" ");
      }
      else if ( screen == Screen::Freq )
      {
        freq_set_screen = 1;
        lcd.setCursor(columnsLCD / 2 - 5, 1);
        lcd.print(*value / 1000);
        lcd.print(".");
        lcd.print(*value - (*value / 1000) * 1000);
        if (((*value - (*value / 1000) * 1000)) < 1) {
          lcd.print("00");
        }
        lcd.print(" Mhz  ");
      }
    }
  }
  lcd.clear();
}

void readConfiguration()
{
  for ( int i = 0 ; i < sizeof(memory.d) ; i++  )
    memory.b[i] = EEPROM.read(i);

  if ( memory.d.initialized != 'Y' )
  {
    memory.d.initialized = 'Y';
    memory.d.frq_show    = 1;
    memory.d.frq_band    = 0;
    memory.d.frq_step    = 0;
    memory.d.frq_set     = 94500;
    writeConfiguration();
  }
  
}

void writeConfiguration()
{
  for ( int i = 0 ; i < sizeof(memory.d) ; i++  )
    EEPROM.write( i, memory.b[i] );
}

Button readButtons()
{
  static boolean oldA = HIGH;
  static boolean newA = LOW;
  static boolean newB = LOW;

  btnPressed = Button::Unknown;
  newA = digitalRead(pENCO_DT);
  newB = digitalRead(pENCO_CLK);

  if ( !oldA && newA )
  {
    btnPressed = !newB ? Button::Left : Button::Right;
    delay(50);
  }
  else if ( !digitalRead(pENCO_SW) )
  {
    while (!digitalRead(pENCO_SW));
    btnPressed = Button::Ok;
    delay(50);
  }

  oldA = newA;
  return btnPressed;
}
