#include <dht.h> 
#include <Wire.h>
#include <math.h> 
#include <Adafruit_BMP280.h>
#include <Adafruit_GFX.h>
//#include <Adafruit_SSD1306.h>   
#include <SoftwareSerial.h>

SoftwareSerial gpsSerial(4,7);  // GPS软串口
//-----------GPS、NBIot初始化----------------
int loop_num = 0;   //循环变量，控制nb发送
struct
{
  char GPS_Buffer[71];
  bool isGetData;     //是否获取到GPS数据
  bool isParseData;   //是否解析完成
  char UTCTime[10];   //UTC时间
  char latitude[11];  //纬度
  char N_S[2];        //N/S
  char longitude[12]; //经度
  char E_W[2];        //E/W
  bool isUsefull;     //定位信息是否有效
} Save_Data;
const unsigned int gpsRxBufferLength = 500;
char gpsRxBuffer[gpsRxBufferLength];
unsigned int ii = 0;



//-----------OLED初始化----------------
//float str1,str2; 
dht DHT222;  
#define DHT22_PIN A2
#define OLED_RESET 4
//Adafruit_SSD1306 display(OLED_RESET);  

//-----------bmp280高度计初始化-------------
Adafruit_BMP280 bmp; // I2C
int BH1750address = 0x23;//芯片地址为16位23
byte buff[2];
//float pressure;float altitude;

//-----------gy30光强初始化-----------------
int LightIntensity=0;
//------------S12D紫外线初始化---------------
float ultra=0.00;

//-------------PM2.5初始化------------------
#define measurePin A3
#define ledPower A1          
unsigned int samplingTime = 280;
unsigned int deltaTime = 40;
unsigned int sleepTime = 9680; 
//float voMeasured = 0;
float calcVoltage = 0;
//float dustDensity = 0; 

void setup()  
{  
  gpsSerial.begin(9600);
  Serial.begin(9600);  
  Wire.begin();
  //display.begin(SSD1306_SWITCHCAPVCC);//初始化
  //display.display();
  //display.clearDisplay();
  nbiot_setup();
//--------------------gps初始化-------------------
  Save_Data.isGetData = false;
  Save_Data.isParseData = false;
  Save_Data.isUsefull = false;
//--------------------pm2.5初始化-----------------
  pinMode(ledPower,OUTPUT);

//-----------------------------bmp280 part-------------------------
 // Serial.println(F("BMP280 test"));
  if (!bmp.begin()) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
//    while (1);
  }
  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

//---------------------------serialprint part--------------------------------
//  Serial.println("DHT22 TEST PROGRAM ");
//  Serial.print("LIBRARY VERSION: ");
//  Serial.println(DHT_LIB_VERSION);
//  Serial.println();
//  Serial.println("Type,\tstatus,\tHumidity (%),\tTemperature (C)");
}    
void loop()  
{ 
  //Serial.write("AT+NMGS=5,68656c6c6f\r\n");
  float voMeasured = 0;
  //float calcVoltage = 0;
  //float dustDensity = 0; 
  //----------------OLED loop-------------------
//  Serial.print("DHT22, \t");
  int chk = DHT222.read22(DHT22_PIN);  //读取数据
  Serial.print("BMP press:");
  Serial.println(bmp.readPressure());
  switch (chk)
  {
    case DHTLIB_OK:  
                Serial.print("OK,\t"); 
                break;
    case DHTLIB_ERROR_CHECKSUM: 
                Serial.print("Checksum error,\t"); 
                break;
    case DHTLIB_ERROR_TIMEOUT: 
                Serial.print("Time out error,\t"); 
                break;
    default: 
                Serial.print("Unknown error,\t"); 
                break;
  }
  // 显示数据
//  Serial.print(DHT222.humidity, 1);
//  Serial.print(",\t");
//  Serial.println(DHT222.temperature, 1);
  
  //display.setTextSize(1);
  //display.setTextColor(WHITE);
  //display.setCursor(0,0);
  //str1=DHT222.humidity;
  //display.print(F("Humidity:"));
  //display.println(DHT222.humidity);
  //str2=DHT222.temperature;
  //display.print(F("Temperature:"));
  //display.println(DHT222.temperature); 
  //pressure=bmp.readPressure();
  //display.print(F("Pressure:"));
  //display.println(bmp.readPressure()); 
 // altitude=bmp.readAltitude(1013.25);
//  display.print("Altitude:");
//  display.println(bmp.readAltitude(1013.25)); 
//  display.display();//显示出来
  delay(2048);  
  //display.clearDisplay();

  //display.setTextSize(1);
  //display.setTextColor(WHITE);
  //display.setCursor(0,0);
  //display.print(F("LightIntensity:"));
  //display.println(LightIntensity); 
  //display.print(F("UltraViolet:"));
  //display.println(ultra); 
  //display.display();//显示出来
  //display.print(F("calcVoltage:"));
  //display.println(calcVoltage); 
  //display.display();//显示出来
  //display.clearDisplay();
  
  //--------------------BMP280 loop------------------------
//  Serial.print(F("Temperature = "));
//  Serial.print(bmp.readTemperature());
//  Serial.println(" *C");
//
//  Serial.print(F("Pressure = "));
//  Serial.print(bmp.readPressure());
//  Serial.println(" Pa");
//
//  Serial.print(F("Approx altitude = "));
//  Serial.print(bmp.readAltitude(1013.25)); /* Adjusted to local forecast! */
//  Serial.println(" m");
//
//  Serial.println();

 //------------------GY30 loop------------------------
  int i;
  uint16_t val=0;
  BH1750_Init(BH1750address);
  delay(1000);
  if(2==BH1750_Read(BH1750address))
  {
      val=((buff[0]<<8)|buff[1])/1.2;
      Serial.println(val,DEC);     
  }
  LightIntensity=int(val);
  delay(150);
  //----------------ultraviolet loop---------------------
  int sensorValue;
  long  sum=0;
  for(int i=0;i<1024;i++)
   {  
      sensorValue=analogRead(A0);
      sum=sensorValue+sum;
      delay(2);
   }   
   sum = sum >> 10;
   //ultra=sum*4980.0/1023.0;
   ultra=sum*4.868;
//   Serial.print("The voltage value:");
//   Serial.print(ultra);
//   Serial.println("mV");
//   Serial.print("\n");
   //------------------pm2.5 loop---------------
   digitalWrite(ledPower,LOW); 
  delayMicroseconds(samplingTime);   
  voMeasured = analogRead(measurePin);   
  delayMicroseconds(deltaTime); 
  digitalWrite(ledPower,HIGH);  
  delayMicroseconds(sleepTime);   
  calcVoltage = voMeasured*(5.0/1024); 
  //dustDensity = 0.17*calcVoltage-0.1;  
  //if ( dustDensity < 0)  {  
  //  dustDensity = 0.00;  }  
//  Serial.println("Raw Signal Value (0-1023):");
//  Serial.println(voMeasured);  
//  Serial.println("Voltage:");  
//  Serial.println(calcVoltage);   
//  Serial.println("Dust Density:"); 
//  Serial.println(dustDensity);  
  delay(1000);
//--------------GPS loop---------------
    gpsSerial.listen();
    delay(2000);
    gpsRead();  //获取GPS数据
    parseGpsBuffer();//解析GPS数据
    printGpsBuffer();//输出解析后的数据

    delay(1000);
    
    mergeData();
    delay(1000);
    readNB();
}


//------------------BMP280 functions------------------------------
int BH1750_Read(int address) //
{
  int i=0;
  Wire.beginTransmission(address);
  Wire.requestFrom(address, 2);
  while(Wire.available()) //
  {
    buff[i] = Wire.read();  // read one byte
    i++;
  }
  Wire.endTransmission();  
  return i;
}

void BH1750_Init(int address) 
{
  Wire.beginTransmission(address);
  Wire.write(0x10);//1lx reolution 120ms
  Wire.endTransmission();
}
//------------------GPS functions------------------------------
void errorLog(int num)
{
  Serial.print("gpsERROR");
  Serial.println(num);
}

void printGpsBuffer()
{
  if (Save_Data.isParseData)
  {
    Save_Data.isParseData = false;
    
    Serial.print("Save_Data.UTCTime = ");
    Serial.println(Save_Data.UTCTime);

    if(Save_Data.isUsefull)
    {
      Save_Data.isUsefull = false;
  //    Serial.print("latitude = ");
  //    Serial.println(Save_Data.latitude);
      //Serial.print("Save_Data.N_S = ");
      //Serial.println(Save_Data.N_S);
  //    Serial.print("longitude = ");
    //  Serial.println(Save_Data.longitude);
      //Serial.print("Save_Data.E_W = ");
      //Serial.println(Save_Data.E_W);
    }
    else
    {
      Serial.println("GPS not usefull!");
    }
    
  }
}

void parseGpsBuffer()
{
  char *subString;
  char *subStringNext;
  if (Save_Data.isGetData)
  {
    Save_Data.isGetData = false;
    //Serial.println(Save_Data.GPS_Buffer);  
    for (int i = 0 ; i <= 6 ; i++)
    {
      if (i == 0)
      {
        if ((subString = strstr(Save_Data.GPS_Buffer, ",")) == NULL)
          errorLog(1);  //解析错误
      }
      else
      {
        subString++;
        if ((subStringNext = strstr(subString, ",")) != NULL)
        {
          //Serial.println(subStringNext-subString);
          char usefullBuffer[2]; 
          switch(i)
          {
            case 1:memcpy(Save_Data.UTCTime, subString, subStringNext - subString);Save_Data.UTCTime[9] = '\0';break; //获取UTC时间
            case 2:memcpy(usefullBuffer, subString, subStringNext - subString);usefullBuffer[1] = '\0';break;         //定位状态
            case 3:memcpy(Save_Data.latitude, subString, subStringNext - subString);Save_Data.latitude[10] = '\0';break;  //获取纬度信息
            case 4:memcpy(Save_Data.N_S, subString, subStringNext - subString);Save_Data.N_S[1] = '\0';break;         //获取N/S
            case 5:memcpy(Save_Data.longitude, subString, subStringNext - subString);Save_Data.longitude[11] = '\0';break; //获取经度信息
            case 6:memcpy(Save_Data.E_W, subString, subStringNext - subString);Save_Data.E_W[1] = '\0';break;         //获取E/W

            default:break;
          }

          subString = subStringNext;
          Save_Data.isParseData = true;
          if(usefullBuffer[0] == 'A')
            Save_Data.isUsefull = true;
          else if(usefullBuffer[0] == 'V')
            Save_Data.isUsefull = false;

        }
        else
        {
          errorLog(2);  //解析错误
        }
      }
    }
  }
}

void gpsRead() {
  //const unsigned int gpsRxBufferLength = 500;
  //char gpsRxBuffer[500];//500
  while(Serial.read()>=0){}
  while (!gpsSerial.available()){}
  if (gpsSerial.available())
  {
    delay(100);
    ii=0;
    while (gpsSerial.available())
  {
    gpsRxBuffer[ii++] = gpsSerial.read();
  }
  }
  
  //Serial.println(gpsRxBuffer);

  char* GPS_BufferHead;
  char* GPS_BufferTail;
  if ((GPS_BufferHead = strstr(gpsRxBuffer, "$GPRMC,")) != NULL || (GPS_BufferHead = strstr(gpsRxBuffer, "$GNRMC,")) != NULL )
  {
    memcpy(Save_Data.GPS_Buffer, GPS_BufferHead, 70);
    Save_Data.isGetData = true;
  }
}
//-----------------nbiot--------------
void nbiot_setup()
{
    Serial.write("AT+CGSN=1\r\n");
    Serial.write("AT+NCDP=117.60.157.137，5683\r\n");
    Serial.write("AT+NRB\r\n");
    delay(1000);
    Serial.write("AT+NCDP?\r\n");
    Serial.write("AT+NSMI=1\r\n");
    Serial.write("AT+NNMI=2\r\n");
}

void readNB(){
  while(Serial.available())
    {
      char ch = Serial.read();
      Serial.print(ch);
    }
}

//--------merge all-------
// 将所有采集到的数据合并进char数组，并转换成nb要求的格式
void mergeData(){
  char mergeAll[66] = "0000000000000000000000000000000000000000000000000000000000000000";
  char c[9];
  //温度为5个char字符，mergeAll从0-4
  dtostrf(DHT222.temperature,2,2,c);
  if(strlen(c)==5)
  {
    for(int i=0; i<5; i++){
      mergeAll[i] = c[i];
    }
  }
  else if(strlen(c)==4)
  {
    mergeAll[0] = '0';
    for(int i=0; i<4; i++){
      mergeAll[i+1] = c[i];
    }
  }

  //湿度为5个char字符，mergeAll从5-9
  dtostrf(DHT222.humidity,2,2,c);
  if(strlen(c)==5)
  {
    for(int i=0; i<5; i++){
      mergeAll[i+5] = c[i];
    }
  }
  else if(strlen(c)==4)
  {
    mergeAll[5] = '0';
    for(int i=0; i<4; i++){
      mergeAll[i+6] = c[i];
    }
  }

  //紫外线为5个char字符，mergeAll从10-14
  dtostrf(ultra,2,2,c);
  if(strlen(c)==5)
  {
    for(int i=0; i<5; i++){
      mergeAll[i+10] = c[i];
    }
  }
  else if(strlen(c)==4)
  {
    mergeAll[10] = '0';
    for(int i=0; i<4; i++){
      mergeAll[i+11] = c[i];
    }
  }

  //pm2.5为7个char字符，mergeAll从15-21 calcVoltage xxxx.xx
  //itoa(pm25, c, 10);
  dtostrf(calcVoltage,4,2,c);
  for(int i=0; i<7; i++)
  {
      if(i<7-strlen(c))mergeAll[15+i] = '0';
      else mergeAll[15+i] = c[i-7+strlen(c)];
  }

  
  //时间为6位字符，mergeAll从22-27
  for(int i=0; i<6; i++)
  {
    mergeAll[i+22] = Save_Data.UTCTime[i];
  }
  //经度为11个字符，mergeAll从28-38
  for(int i=0; i<11; i++)
  {
    mergeAll[i+28] = Save_Data.longitude[i];
  }

  //纬度为10个字符，mergeAll从39-48
  for(int i=0; i<10; i++)
  {
    mergeAll[i+39] = Save_Data.latitude[i];
  }

//压强为9个char字符，mergeAll从49-57
  dtostrf(bmp.readPressure(),6,2,c);
  if(strlen(c)==9)
  {
    for(int i=0; i<9; i++){
      mergeAll[i+49] = c[i];
    }
  }
  else if(strlen(c)==8)
  {
    mergeAll[49] = '0';
    for(int i=0; i<8; i++){
      mergeAll[i+50] = c[i];
    }
  }

// //高度为7个char字符，mergeAll从58-64
//  dtostrf(bmp.readAltitude(1013.25)+1000,5,2,c);
//  l=strlen(c);
//  for(int i=0; i<7; i++)
//  {
//      if(i<7-l)mergeAll[58+i] = '0';
//      else mergeAll[58+i] = c[i-7+l];
//  }

  //光强为4个char字符，mergeAll从58-61
  itoa(LightIntensity,c,10);
  for(int i=0; i<4; i++)
  {
      if(i<4-strlen(c))mergeAll[58+i] = '0';
      else mergeAll[58+i] = c[i-4+strlen(c)];
  }
  //把字符个数存在ch中
  //int len = strlen(mergeAll);
  char ch[3]; 
  itoa(strlen(mergeAll), ch, 10);
  //命令格式如下
  //Serial.write("AT+NMGS=5,68656c6c6f\r\n");
  char abc[200] = {'0',};
  char atnmgs[] = "AT+NMGS=";
  for (int i=0; i<8; i++)
  {
    abc[i] = atnmgs[i];
  }
  Serial.print("ch:");
  Serial.println(ch);
  Serial.print("mergeall:");
  Serial.println(mergeAll);
  if(strlen(ch)==2)
  {
    abc[8]=ch[0];
    abc[9]=ch[1];
  }
  else
  {
    abc[8]='0';
    abc[9]=ch[0];
  }
  abc[10]=',';

  for(int i=0; i<strlen(mergeAll); i++)
  {
    char a = mergeAll[i];
    char temp[2];
    switch(a)
    {
      case '0':
        temp[0]='3';temp[1]='0';break;
      case '1':
        temp[0]='3';temp[1]='1';break;
      case '2':
        temp[0]='3';temp[1]='2';break;
      case '3':
        temp[0]='3';temp[1]='3';break;
      case '4':
        temp[0]='3';temp[1]='4';break;
      case '5':
        temp[0]='3';temp[1]='5';break;
      case '6':
        temp[0]='3';temp[1]='6';break;
      case '7':
        temp[0]='3';temp[1]='7';break;
      case '8':
        temp[0]='3';temp[1]='8';break;
      case '9':
        temp[0]='3';temp[1]='9';break;
      case '.':
        temp[0]='2';temp[1]='E';break;   
    }
    abc[11+2*i] = temp[0];
    abc[12+2*i] = temp[1];
  }
  abc[strlen(abc)] = '\r';
  abc[strlen(abc)] = '\n';
  loop_num++;
  Serial.print("loop_num:");
  Serial.println(loop_num);
  if (loop_num == 10)
  {
    loop_num = 0;
    Serial.print("abc:");
    Serial.write(abc);
  }
}
