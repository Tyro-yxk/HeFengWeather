#include "AirQuality.h"
#include "HttpsGetUtils.h"

AirQuality::AirQuality()
{
    this->_url = String(HttpsGetUtils::host) + "/v7/air/now?location=" + this->_UserCityId +
                 "&key=" + this->_Uesrkey + "&unit=m&lang=zh";
}

// 配置请求信息，私钥、位置、单位、语言
void AirQuality::config(String userKey, String location)
{
    this->_url = String(HttpsGetUtils::host) + "/v7/air/now?location=" + location +
                 "&key=" + userKey + "&unit=m&lang=zh";
}

// 尝试获取信息，成功返回true，失败返回false
bool AirQuality::get()
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
void AirQuality::_parseNowJson(char *input, size_t inputLength)
{
    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, input, inputLength);

    // const char *code = doc["code"];
    _response_code = doc["code"].as<String>(); // API状态码
    Serial.println("code-----------------------" + _response_code);
    if (_response_code == "200")
    {
        const char *updateTime = doc["updateTime"];
        JsonObject now = doc["now"];
        _last_update_str = doc["updateTime"].as<String>(); // 当前API最近更新时间
        _now_aqi_int = now["aqi"].as<int>();               // 实时空气质量指数
        _now_category_str = now["category"].as<String>();  // 实时空气质量指数级别
        _now_primary_str = now["primary"].as<String>();    // 实时空气质量的主要污染物，优时返回 NA
    }
}

// API状态码
String AirQuality::getServerCode()
{
    return _response_code;
}

// 当前API最近更新时间
String AirQuality::getLastUpdate()
{
    return _last_update_str;
}

// 实时空气质量指数
int AirQuality::getAqi()
{
    return _now_aqi_int;
}

// 实时空气质量指数级别
String AirQuality::getCategory()
{
    return _now_category_str;
}

// 实时空气质量的主要污染物，空气质量为优时，返回值为NA
String AirQuality::getPrimary()
{
    return _now_primary_str;
}