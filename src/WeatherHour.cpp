#include "WeatherHour.h"
#include "HttpsGetUtils.h"
WeatherHour::WeatherHour()
{
    this->setUrl();
}

WeatherHour::WeatherHour(int hourSum, int hourNumber)
{
    switch (hourSum)
    {
    case hourNum1:
    case hourNum2:
    case hourNum3:
        if (hourSum >= hourNumber)
        {
            this->_HourNumber = hourNumber;
            this->_hourSum = hourSum;
        }
        else
        {
            this->_HourNumber = hourNumber;
            this->_hourSum = _HourNumber;
        }
        break;
    default:
        Serial.println("官方未提供获取" + String(hourSum) + "小时的天气");
        break;
    }
    this->setUrl();
}
WeatherHour::~WeatherHour()
{
    delete[] this->hourLy;
}
void WeatherHour::setUrl()
{
    this->hourLy = new HourLy[_HourNumber];
    this->_url = String(HttpsGetUtils::host) + "/v7/weather/" + 24 + "h?location=" + _UserCityId +
                 "&key=" + _Uesrkey + "&unit=m&lang=zh";
}

// 例如 location="101010100"，城市相关ID从https://github.com/qwd/LocationList下载。
//@param userKey 用户密钥
//@param location 城市ID
void WeatherHour::config(String userKey, String location)
{
    _url = String(HttpsGetUtils::host) + "/v7/weather/" + 24 + "h?location=" + location +
           "&key=" + userKey + "&unit=m&lang=zh";
}

bool WeatherHour::get()
{
    uint8_t *outbuf = NULL;
    size_t len = 0;
    Serial.println("Get Weather" + String(this->_HourNumber) + "Hour...");
    bool result = HttpsGetUtils::getString(_url.c_str(), outbuf, len);
    if (outbuf && len)
    {
        _parseNowJson((char *)outbuf, len);
    }
    else
    {
        Serial.println("Get Weather" + String(this->_HourNumber) + "Hour failed");
    }
    // 一定要记得释放内存
    if (outbuf != NULL)
    {
        free(outbuf);
        outbuf = NULL;
    }
    return true;
}

// 解析Json信息
void WeatherHour::_parseNowJson(char *input, size_t inputLength)
{
    StaticJsonDocument<2048> doc;
    DeserializationError error = deserializeJson(doc, input, inputLength);

    _response_code = doc["code"].as<String>(); // API状态码
    Serial.println("code-----------------------" + _response_code);
    if (_response_code == "200")
    {
        _last_update_str = doc["updateTime"].as<String>(); // 当前API最近更新时间
        JsonArray hourLy_json = doc["hourly"];
        for (int i = 0; i < this->_HourNumber; i++)
        {
            JsonObject hourLy_data = hourLy_json[i];
            hourLy[i].fxTime = hourLy_data["fxTime"].as<String>();
            hourLy[i].temp = hourLy_data["temp"].as<int>();
            hourLy[i].icon = hourLy_data["icon"].as<int>();
            hourLy[i].text = hourLy_data["text"].as<String>();
            hourLy[i].wind360 = hourLy_data["wind360"].as<float>();
            hourLy[i].windDir = hourLy_data["windDir"].as<String>();
            hourLy[i].windScale = hourLy_data["windScale"].as<String>();
            hourLy[i].windSpeed = hourLy_data["windSpeed"].as<int>();
            hourLy[i].humidity = hourLy_data["humidity"].as<int>();
            hourLy[i].precip = hourLy_data["precip"].as<int>();
            hourLy[i].pop = hourLy_data["pop"].as<int>();
            hourLy[i].pressure = hourLy_data["pressure"].as<int>();
            hourLy[i].dew = hourLy_data["dew"].as<int>();
        }
    }
}

// API状态码
String WeatherHour::getServerCode()
{
    return _response_code;
}

// 当前API最近更新时间
String WeatherHour::getLastUpdate()
{
    return _last_update_str;
}
// 某一天的天气预报
HourLy WeatherHour::getHourLy(int hour)
{
    return hourLy[hour];
}