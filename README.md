# Tyro的Esp8266获取和风天气解压

## 空气质量

1. [源码](./src/AirQuality.cpp)

2. 使用

   ```c++
   AirQuality airQuality;  	                //创建对象
   airQuality.config(userKey，cityId);         //配置对象
   airQuality.get();						  //获取数据
```
   

   
   