#ifndef _TMP_H_
#define _TMP_H_

//fan
//#define FAN_OUT  5//FAN 
//spi
//#define DATA_OUT 11//MOSI
#define DATA_IN  12//MISO 
#define SPI_CLOCK  13//sck
#define SLAVE_SELECT 10//ss

unsigned long resistance=0;
double iTemp=0;

PROGMEM const unsigned long PLATE_RESISTANCE_TABLE[] = {
  3364790, 3149040, 2948480, 2761940, 2588380, 2426810, 2276320, 2136100, 2005390, 1883490,
  1769740, 1663560, 1564410, 1471770, 1385180, 1304210, 1228470, 1157590, 1091220, 1029060,
  970810, 916210, 865010, 816980, 771900, 729570, 689820, 652460, 617360, 584340,
  553290, 524070, 496560, 470660, 446260, 423270, 401590, 381150, 361870, 343680,
  326500, 310290, 294980, 280520, 266850, 253920, 241700, 230130, 219180, 208820,
  199010, 189710, 180900, 172550, 164630, 157120, 149990, 143230, 136810, 130720,
  124930, 119420, 114190, 109220, 104500, 100000, 95720, 91650, 87770, 84080,
  80570, 77220, 74020, 70980, 68080, 65310, 62670, 60150, 57750, 55450,
  53260, 51170, 49170, 47250, 45430, 43680, 42010, 40410, 38880, 37420,
  36020, 34680, 33400, 32170, 30990, 29860, 28780, 27740, 26750, 25790,
  24880, 24000, 23160, 22350, 21570, 20830, 20110, 19420, 18760, 18130,
  17520, 16930, 16370, 15820, 15300, 14800, 14320, 13850, 13400, 12970,
  12550, 12150, 11770, 11400, 11040, 10700, 10370, 10050, 9738, 9441,
  9155, 8878, 8612, 8354, 8106, 7866, 7635, 7412, 7196, 6987, 6786,
  6591, 6403, 6222, 6046, 5876 };

void tmp_init()
{
  SPCR = (1<<SPE)|(1<<MSTR)|(1<<4);
  delay(10);
  
//  pinMode(DATA_OUT, OUTPUT);
  //pinMode(FAN_OUT, OUTPUT);
  //digitalWrite(FAN_OUT,LOW);//disable fan
  pinMode(DATA_IN, INPUT);
  pinMode(SPI_CLOCK,OUTPUT);
  pinMode(SLAVE_SELECT,OUTPUT);
  digitalWrite(SLAVE_SELECT,HIGH); //disable device 
}

  char SPITransfer(volatile char data) 
{
  SPDR = data;   
  digitalWrite(SLAVE_SELECT, LOW); 
  //Serial.println("in +");
  // Start the transmission
  while (!(SPSR & (1<<SPIF)))     // Wait the end of the transmission
  {};
  //Serial.println(SPDR);
  return SPDR;                    // return the received byte
}

float TableLookup(const unsigned long lookupTable[], unsigned int tableSize, int startValue, unsigned long searchValue) {
  //simple linear search for now
  int i;
  for (i = 0; i < tableSize; i++) {
    if (searchValue >= pgm_read_dword_near(lookupTable + i))
      break;
  }
  
  if (i > 0) {
    unsigned long high_val = pgm_read_dword_near(lookupTable + i - 1);
    unsigned long low_val = pgm_read_dword_near(lookupTable + i);
    return i + startValue - (float)(searchValue - low_val) / (float)(high_val - low_val);
  } else {
    return startValue;
  }
}

void ReadTemp() 
{
  digitalWrite(SLAVE_SELECT, LOW);

  //read data
  //Serial.println("0"); 
  while(digitalRead(DATA_IN)) { }
  //Serial.println("1"); 
  uint8_t spiBuf[4];
  memset(spiBuf, 0, sizeof(spiBuf));
//Serial.println("2"); 
  digitalWrite(SLAVE_SELECT, LOW);  
  for(int i = 0; i < 4; i++)
    spiBuf[i] = SPITransfer(0xFF);
    

  unsigned long conv = (((unsigned long)spiBuf[3] >> 7) & 0x01) + ((unsigned long)spiBuf[2] << 1) + ((unsigned long)spiBuf[1] << 9) + (((unsigned long)spiBuf[0] & 0x1F) << 17); //((spiBuf[0] & 0x1F) << 16) + (spiBuf[1] << 8) + spiBuf[2];
  //Serial.print("conv is:"); 
  //Serial.println(conv); 
  unsigned long adcDivisor = 0x1FFFFF;
  float voltage = (float)conv * 5.0 / adcDivisor;

  unsigned int convHigh = (conv >> 16);
  
  digitalWrite(SLAVE_SELECT, HIGH);
  
  unsigned long voltage_mv = voltage * 1000;
  unsigned long resistance = voltage_mv * 22000 / (5000 - voltage_mv); // in hecto ohms
  //Serial.print("resistance is:"); 
  //Serial.println(resistance); 
  iTemp = TableLookup(PLATE_RESISTANCE_TABLE, sizeof(PLATE_RESISTANCE_TABLE) / sizeof(PLATE_RESISTANCE_TABLE[0]), -40, resistance);
}

#endif
