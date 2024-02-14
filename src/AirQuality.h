#ifndef _AIRQUALITY_H_
#define _AIRQUALITY_H_

#include <Arduino.h>
#include <ArduinoJson.h>

#include "ConfigUtils.h"

class AirQuality : public ConfigUtils
{
public:
    AirQuality();
    void config(String userKey, String location);
    bool get();
    String getLastUpdate();
    String getServerCode();
    int getAqi();
    String getCategory();
    String getPrimary();
    int getLevel();

private:
    String _url;
    void _parseNowJson(char *input, size_t inputLength); // 解析json信息
    String _response_code = "no_init";                   // API状态码
    String _last_update_str = "no_init";                 // 当前API最近更新时间
    int _now_aqi_int = 999;                              // 实时空气质量指数
    String _now_category_str = "no_init";                // 实时空气质量指数级别
    String _now_primary_str = "no_init";                 // 实时空气质量的主要污染物，优时返回 NA
    int _now_level_int = -1;                             // 空气质量指数等级
};

#endif