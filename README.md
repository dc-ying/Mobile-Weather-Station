# Mobile-Weather-Station
Mobile Weather Station based on Arduino and NB-Iot.  (SJTU course project)

## Modules
- [ ]  NB75-B5 NB-Iot模块
- [ ]  Arduino Nano
- [ ]  GY-30 光照传感器
- [ ]  GP2Y1014AU 粉尘传感器
- [ ]  GY-BMP280 高度传感器
- [ ]  OLED 显示器
- [ ]  GUVA 紫外线传感器
- [ ]  DHT22 温湿度传感器

## GY-BMP280
引脚：
- [21]  SDA
- [22]  SCL

在库文件中默认定义的传感器I2C地址是0x77，而我使用的时候没有给SD0高电位，因此我们在Adafruit_BMP280.h中，找到下面这一行

bool  begin(uint8_t addr = BMP280_ADDRESS, uint8_t chipid = BMP280_CHIPID);

改为
bool  begin(uint8_t addr = BMP280_ADDRESS_ALT, uint8_t chipid = BMP280_CHIPID);

## NB-Iot and "透传云"platform
* [USR-NB75-B5 硬件设计手册](http://www.usr.cn/Down/WH-NB75_hardware_V2.0.1.pdf)
* [透传云使用说明书](http://console.usr.cn/uploads/pdf/%E9%80%8F%E4%BC%A0%E4%BA%91%E4%BD%BF%E7%94%A8%E8%AF%B4%E6%98%8E%E4%B9%A6.pdf)
