#include "WeatherNow.h"
#include "HttpsGetUtils.h"
WeatherNow::WeatherNow()
{
    _url = String(HttpsGetUtils::host) + "/v7/weather/now?location=" + _UserCityId +
           "&key=" + _Uesrkey + "&unit=m&lang=zh";
}

// 例如 location="101010100"，城市相关ID从https://github.com/qwd/LocationList下载。
void WeatherNow::config(String userKey, String location)
{
    _url = String(HttpsGetUtils::host) + "/v7/weather/now?location=" + location +
           "&key=" + userKey + "&unit=m&lang=zh";
}

bool WeatherNow::get()
{
    uint8_t *outbuf = NULL;
    size_t len = 0;
    Serial.println("Get WeatherNow...");
    bool result = HttpsGetUtils::getString(_url.c_str(), outbuf, len);
    if (outbuf && len)
    {
        _parseNowJson((char *)outbuf, len);
    }
    else
    {
        Serial.println("Get WeatherNow failed");
    }
    // 一定要记得释放内存
    if (outbuf != NULL)
    {
        free(outbuf);
        outbuf = NULL;
    }
    return result;
}

// 解析Json信息
void WeatherNow::_parseNowJson(char *input, size_t inputLength)
{
    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, input, inputLength);

    _response_code = doc["code"].as<String>(); // API状态码
    Serial.println("code-----------------------" + _response_code);
    if (_response_code == "200")
    {
        JsonObject now = doc["now"];
        _last_update_str = doc["updateTime"].as<String>(); // 当前API最近更新时间
        _now_temp_int = now["temp"].as<int>();             // 实况温度
        _now_feelsLike_int = now["feelsLike"].as<int>();   // 实况体感温度
        _now_icon_int = now["icon"].as<int>();             // 当前天气状况和图标的代码
        _now_text_str = now["text"].as<String>();          // 实况天气状况的文字描述
        _now_windDir_str = now["windDir"].as<String>();    // 实况风向
        _now_windScale_int = now["windScale"].as<int>();   // 实况风力等级
        _now_humidity_int = now["humidity"].as<int>();     // 实况相对湿度百分比数值
        _now_precip_str = now["precip"].as<String>();      // 实况降水量,毫米
        _now_wind360_double = now["wind360"].as<double>(); // 风向角度
    }
}

// API状态码
String WeatherNow::getServerCode()
{
    return _response_code;
}

// 当前API最近更新时间
String WeatherNow::getLastUpdate()
{
    return _last_update_str;
}

// 实况温度
int WeatherNow::getTemp()
{
    return _now_temp_int;
}

// 实况体感温度
int WeatherNow::getFeelLike()
{
    return _now_feelsLike_int;
}

// 当前天气状况和图标的代码
int WeatherNow::getIcon()
{
    return _now_icon_int;
}

// 实况天气状况的文字描述
String WeatherNow::getWeatherText()
{
    return _now_text_str;
}

// 实况风向
String WeatherNow::getWindDir()
{
    return _now_windDir_str;
}

// 实况风力等级
int WeatherNow::getWindScale()
{
    return _now_windScale_int;
}

// 实况相对湿度百分比数值
int WeatherNow::getHumidity()
{
    return _now_humidity_int;
}
// 实况降水量,毫米
String WeatherNow::getPrecip()
{
    return _now_precip_str;
}
// 风向角度
double WeatherNow::getWind360()
{
    return _now_wind360_double;
}