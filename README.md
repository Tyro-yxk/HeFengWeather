# Tyro的Esp8266获取和风天气解压

## 说明
在以下两个代码基础上修改而成，同时添加了自己的代码
同时***ESP8266_Heweather***这个库里面的部分我喜欢的数据未解析，而且获取空气质量的url是使用的C类ip地址（未测试获取数据，不知道是否能获取到数据），担心第三方代理服务器挂机导致无法获取数据，修改了获取空气质量的代码，同时添加获取24小时数据的代码

github -> [ESP8266_Heweather](https://github.com/tignioj/ESP8266_Heweather)

csdn -> [ESP32（Arduino)获取和风天气数据](https://blog.csdn.net/y275903528/article/details/131192585) 

写了一个示例，但是将包导入Arduino时没找到示例，貌似没识别，就不写了。
本项目只是我恰好需要使用，就写了并分享出来，没做过开源项目的开发，并不知道开源协议具体的内容，本项目就添加了Apache的协议，有问题请联系

可能都不会有人注意到这个项目，如果有人看到的话能否帮我解决以下Arduino未识别出示例的问题

未说清楚的配置类使用
1. 未使用配置类获取实时天气和空气质量
   ```c++
   AirQuality airQuality;              // 创建对象
   airQuality.config(userKey, cityId); // 配置对象
   airQuality.get();                   // 获取数据

   WeatherNow weatherNow;              // 创建对象
   airQuality.config(userKey, cityId); // 配置对象
   airQuality.get();                   // 获取数据
   ```
   
2. 使用配置类获取实时天气和空气质量
   ```c++
   ConfigUtils con(userKey, cityId); // 创建配置类对象

   AirQuality airQuality; // 创建对象
   airQuality.get();      // 获取数据

   WeatherNow weatherNow; // 创建对象
   airQuality.get();      // 获取数据
   ```
   
   使用多个类时，使用配置类可以省略配置
   
   更多使用可以看下面的详细配置或源码，下面得使用说明中并未全部列出函数

## 配置类

1. [源码](./src/ConfigUtils.h)
2. 使用及说明
    说明：如果创建并配置了该对象，下面的对象可以只创建不配置
   
   ```c++
   ConfigUtils con(userKey, cityId); // 创建对象并配置
   ```

### 空气质量

1. [源码](./src/AirQuality.h)

2. 使用规范

   ```c++
   AirQuality airQuality;              // 创建对象
   airQuality.config(userKey, cityId); // 配置对象
   airQuality.get();                   // 获取数据
   airQuality.getAqi();                // 空气质量指数
   airQuality.getCategory();           // 实时空气质量指数级别
   airQuality.getPrimary()             // 实时空气质量的主要污染物，空气质量为优时，返回值为NA
   ```

### 实时天气

1. [源码](./src/WeatherNow.h)
2. 使用规范
   ```c++
   WeatherNow weatherNow;              // 创建对象
   airQuality.config(userKey, cityId); // 配置对象
   airQuality.get();                   // 获取数据
   weatherNow.getTemp();               // 获取温度
   weatherNow.getIcon();               // 获取图标
   ```

### 多日天气

1. [源码](./src/WeatherDay.h)

2. 使用说明
   
   > 和风官网支持3，7，10，15，30天的天气预报，实测免费版是能获取到7天的，后面的回返回403
   >
   > 配置时可以修改天数，前提时单片机的内存足够，实测4096能放下7日天气
   >
   > 如果内存不够可修改[源码](./src/WeatherDay.cpp)第64行的4096
   >
   > 单片机内存有限，部分数据为获取到结构体中
   
3. 使用规范

   ```c++
   WeatherDay weatherDay(7);           // 有参构造，可以填入3，7...,与下一行二选一
   WeatherDay weatherDay;              // 无参构造，获取3天的天气，与上一行二选一
   weatherDay.config(userKey, cityId); // 配置对象
   weatherDay.get();                   // 获取数据
   weatherDay.getDaily();              // 获取某一日天气的，取值范围0，构造函数的取值-1，默认为1明天，返回类型如下
   weatherDay.getDaily(0).sunrise;     // 例如获取今天的日出时间
   ```
   ```C++
   struct Daily
   {
      String fxDate;        // 预报日期
      String sunrise;       // 日出时间
      String sunset;        // 日落时间
      String moonrise;      // 月升时间
      String moonset;       // 月落时间
      String moonPhase;     // 月相名称
      int moonPhaseIcon;    // 月相图标
      int tempMax;          // 最高温度
      int tempMin;          // 最低温度
      int iconDay;          // 白天天气状况的图标
      String textDay;       // 白天天气状况文字描述
      int iconNight;        // 夜间天气状况的图标
      String textNight;     // 晚间天气状况文字描述
      String windDirDay;    // 白天风向
      int windScaleDay;     // 白天风力等级
      float windSpeedDay;   // 白天风速，公里/小时
      float humidity;       // 相对湿度，百分比数值
      float precip;         // 降水量
      int uvIndex;          // 紫外线强度指数
      float pressure = -99; // 大气压强
   };
   ```
   ### 小时天气
   
1. [源码](./src/WeatherHour.h)
   
2. 使用说明
   
   > 和风官网支持24，72，168小时的天气预报，实测免费版是能获取到24小时的，后面的回返回403
      >
      > 配置时可以修改天数，前提时单片机的内存足够，实测6144好像能放下24时天气
      >
      > 强迫症，选的6114，5663好像也能，为啥说好像能，因为使用更小的数字在调试打印时，后面几小时的数据会为0，所以我觉得6144应该能保存数据，但是由于我的单片机会无线重启，应该是内存不足吧，所以源码默认采用2048，保存的只有5小时的天气数据
      >
      > 如果内存够，可以修改[源码](./src/WeatherHour.cpp)第79行的2048
      >
      > 创建对象时可以选择保存几小时的数据，获取几小时的数据
      >
   > 个人觉得获取到下一小时的天气，时间可以只要小时即可，但是返回结果包括年月日时分秒，并未做处理
   
3. 使用规范
   
   ```C++
   WeatherHour WeatherHour;                      // 无参构造，默认获取24小时天气，保存5小时;与下一行二选一
   WeatherHour WeatherHour(hourSum, hourNumber); // hourSum获取几小时的数据，官方的24，72，168;hourNumber为保存几小时的数据，输入的值应当小于等于hourSum，否则取hourSum;当输入hourSum不是官方提供的几个数据时，将获取24小时，保存5小时数据;与上一行二选一
   WeatherHour.config(userKey, cityId);          // 配置对象
   WeatherHour.get();                            // 获取数据
   WeatherHour.getHourLy();                      // 获取未来{hourNumber}小时的天气数据，默认为0下一小时，返回如下
   WeatherHour.getHourLy(0).temp;                // 获取下一小时的温度
   ```
   
   ```c++
   struct HourLy
   {
      String fxTime;    // 预报日期
      int temp;         // 温度
      int icon;         // 图标
      String text;      // 天气描述
      float wind360;    // 风向角度
      String windDir;   // 风向角度
      String windScale; // 风力等级
      int windSpeed;    // 风速
      int humidity;     // 相对湿度，百分比数值
      float precip;     // 当前小时累计降水
      int pop;          // 降水概率
      int pressure;     // 大气压强
      int dew = -99;    // 露点温度
   };
   ```
   
      


