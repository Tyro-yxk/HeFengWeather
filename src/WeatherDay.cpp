#include "WeatherDay.h"
#include "HttpsGetUtils.h"
WeatherDay::WeatherDay()
{
    this->setUrl();
}

WeatherDay::WeatherDay(int dayNumber)
{
    switch (dayNumber)
    {
    case dayNum1:
    case dayNum2:
    case dayNum3:
    case dayNum4:
    case dayNum5:
        this->_dayNumber = dayNumber;
        this->setUrl();
        break;
    default:
        this->setUrl();
        Serial.println("输入枚举无效，默认取值3");
        break;
    }
}
WeatherDay::~WeatherDay()
{
    delete[] this->daily;
}
void WeatherDay::setUrl()
{
    this->daily = new Daily[_dayNumber];
    this->_url = String(HttpsGetUtils::host) + "/v7/weather/" + _dayNumber + "d?location=" + _UserCityId +
                 "&key=" + _Uesrkey + "&unit=m&lang=zh";
}

// 例如 location="101010100"，城市相关ID从https://github.com/qwd/LocationList下载。
//@param userKey 用户密钥
//@param location 城市ID
void WeatherDay::config(String userKey, String location)
{
    _url = String(HttpsGetUtils::host) + "/v7/weather/" + _dayNumber + "d?location=" + location +
           "&key=" + userKey + "&unit=m&lang=zh";
}

bool WeatherDay::get()
{
    uint8_t *outbuf = NULL;
    size_t len = 0;
    Serial.println("Get Weather" + String(this->_dayNumber) + "Day...");
    Serial.println("get" + _url);
    bool result = HttpsGetUtils::getString(_url.c_str(), outbuf, len);
    if (outbuf && len)
    {
        _parseNowJson((char *)outbuf, len);
    }
    else
    {
        Serial.println("Get Weather" + String(this->_dayNumber) + "Day failed");
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
void WeatherDay::_parseNowJson(char *input, size_t inputLength)
{
    StaticJsonDocument<4096> doc;
    DeserializationError error = deserializeJson(doc, input, inputLength);

    _response_code = doc["code"].as<String>(); // API状态码
    Serial.println("code-----------------------" + _response_code);
    if (_response_code == "200")
    {
        _last_update_str = doc["updateTime"].as<String>(); // 当前API最近更新时间
        JsonArray daily_json = doc["daily"];
        for (int i = 0; i < this->_dayNumber; i++)
        {
            JsonObject daily_data = daily_json[i];
            daily[i].fxDate = daily_data["fxDate"].as<String>();
            daily[i].sunrise = daily_data["sunrise"].as<String>();
            daily[i].sunset = daily_data["sunset"].as<String>();
            daily[i].moonrise = daily_data["moonrise"].as<String>();
            daily[i].moonset = daily_data["moonset"].as<String>();
            daily[i].moonPhase = daily_data["moonPhase"].as<String>();
            daily[i].moonPhaseIcon = daily_data["moonPhaseIcon"].as<int>();
            daily[i].tempMax = daily_data["tempMax"].as<int>();
            daily[i].tempMin = daily_data["tempMin"].as<int>();
            daily[i].iconDay = daily_data["iconDay"].as<int>();
            daily[i].textDay = daily_data["textDay"].as<String>();
            daily[i].iconNight = daily_data["iconNight"].as<int>();
            daily[i].textNight = daily_data["textNight"].as<String>();
            daily[i].windDirDay = daily_data["windDirDay"].as<String>();
            daily[i].windScaleDay = daily_data["windScaleDay"].as<int>();
            daily[i].windSpeedDay = daily_data["windSpeedDay"].as<float>();
            daily[i].humidity = daily_data["humidity"].as<float>();
            daily[i].precip = daily_data["precip"].as<float>();
            daily[i].uvIndex = daily_data["uvIndex"].as<int>();
            daily[i].pressure = daily_data["pressure"].as<float>();
            daily[i].wind360Day = daily_data["wind360Day"].as<int>();
        }
    }
}

// API状态码
String WeatherDay::getServerCode()
{
    return _response_code;
}

// 当前API最近更新时间
String WeatherDay::getLastUpdate()
{
    return _last_update_str;
}
// 某一天的天气预报
Daily WeatherDay::getDaily(int day)
{
    return daily[day];
}