#ifndef _WEATHERHOUR_H_
#define _WEATHERHOUR_H_

#include <Arduino.h>
#include <ArduinoJson.h>

#include "ConfigUtils.h"

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
enum Num
{
    hourNum1 = 24;
    hourNum2 = 72;
    hourNum3 = 168;
};

class WeatherHour : public ConfigUtils
{
public:
    WeatherHour();
    /**
     * @param hourSum 链接中的多少小时
     * @param hourNumber 获取多少小时的天气
     */
    WeatherHour(int hourSum, int hourNumber = 5);
    void config(String userKey, String location);
    bool get();
    String getServerCode();
    String getLastUpdate();
    HourLy getHourLy(int hourNumber = 0);
    ~WeatherHour();

private:
    void _parseNowJson(char *input, size_t inputLength); // 解析json信息
    void setUrl();
    int _HourNumber = 5;
    int _hourSum = 24;
    String _url;
    String _response_code = "";   // API状态码
    String _last_update_str = ""; // 当前API最近更新时间
    HourLy *hourLy = nullptr;
};

#endif