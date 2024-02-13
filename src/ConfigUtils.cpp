#include "ConfigUtils.h"
ConfigUtils::ConfigUtils(String k, String c)
{
    this->_Uesrkey = k;
    this->_UserCityId = c;
}
String ConfigUtils::_Uesrkey = "";
String ConfigUtils::_UserCityId = "";