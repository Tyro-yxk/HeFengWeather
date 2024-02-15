#ifndef _WEATHERNOW_H_
#define _WEATHERNOW_H_

#include <Arduino.h>
#include <ArduinoJson.h>

#include "ConfigUtils.h"

class WeatherNow : public ConfigUtils
{
public:
    WeatherNow();
    void config(String userKey, String location); // 获取城市的天气
    bool get();
    String getServerCode();
    String getLastUpdate();
    int getTemp();
    int getFeelLike();
    int getIcon();
    String getWeatherText();
    String getWindDir();
    int getWindScale();
    int getHumidity();
    float getPrecip();
    int getWind360();

private:
    void
    _parseNowJson(char *input, size_t inputLength); // 解析json信息
    String _url;
    String _response_code = "";          // API状态码
    String _last_update_str = "";        // 当前API最近更新时间
    int _now_temp_int = 999;             // 实况温度
    int _now_feelsLike_int = 999;        // 实况体感温度
    int _now_icon_int = 999;             // 当前天气状况和图标的代码
    String _now_text_str = "no_init";    // 实况天气状况的文字描述
    String _now_windDir_str = "no_init"; // 实况风向
    int _now_windScale_int = 999;        // 实况风力等级
    int _now_humidity_int = 999;         // 实况相对湿度百分比数值
    float _now_precip_float = 999;       // 实况降水量,毫米
    int _now_wind360_int;                // 风向角度
};

#endif