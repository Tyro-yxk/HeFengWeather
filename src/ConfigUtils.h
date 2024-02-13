#ifndef _CONFIGUTILS_H_
#define _CONFIGUTILS_H_

#include <Arduino.h>
#include <ArduinoJson.h>

class ConfigUtils
{
private:
public:
    static String _Uesrkey;
    static String _UserCityId;
    static bool _dev;
    /**
     * @param key 用户私钥
     * @param cityID 城市编码Id
     */
    ConfigUtils(String k, String c);
    ConfigUtils() {}
    ~ConfigUtils() {}
};

#endif