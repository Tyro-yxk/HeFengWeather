#ifndef WEATHERDAY_H
#define WEATHERDAY_H

#include <Arduino.h>
#include <ArduinoJson.h>

#include "ConfigUtils.h"

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
    int wind360Day;       // 白天风向360角度
};
enum dayNum
{
    dayNum1 = 3,
    dayNum2 = 7,
    dayNum3 = 10,
    dayNum4 = 15,
    dayNum5 = 30
};

class WeatherDay : public ConfigUtils
{
public:
    WeatherDay();
    WeatherDay(int dayNumber);
    void config(String userKey, String location);
    bool get();
    String getServerCode();
    String getLastUpdate();
    Daily getDaily(int dayNumber = 1);
    ~WeatherDay();

private:
    void _parseNowJson(char *input, size_t inputLength); // 解析json信息
    void setUrl();
    int _dayNumber = 3;
    String _url;
    String _response_code = "";   // API状态码
    String _last_update_str = ""; // 当前API最近更新时间
    Daily *daily = nullptr;
};

#endif // WEATHERDAY_H
