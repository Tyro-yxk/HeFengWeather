#include <HeFengWeather.h>

const char *ssid = "";             // WiFi名称
const char *password = ""; // WiFi密码

String UserKey = ""; // 私钥 https://dev.heweather.com/docs/start/get-api-key
String Location = "101010100";                       // 城市代码 https://github.com/heweather/LocationList,表中的 Location_ID

AirQuality AirQuality;

void setup()
{
    Serial.begin(115200);
    Serial.println("");

    initWiFi(); // 连接WiFi

    AirQuality.config(UserKey, Location); // 配置请求信息
}

void loop()
{
    if (AirQuality.get())
    { // 获取更新
        Serial.println(F("======AirQuality Info======"));
        Serial.print("Server Response: ");
        Serial.println(AirQuality.getServerCode()); // 获取API状态码
        Serial.print(F("Last Update: "));
        Serial.println(AirQuality.getLastUpdate()); // 获取服务器更新天气信息时间
        Serial.print(F("AirQuality Aqi: "));
        Serial.println(AirQuality.getAqi()); // 实时空气质量指数
        Serial.print(F("Category: "));
        Serial.println(AirQuality.getCategory()); // 实时空气质量指数级别
        Serial.print(F("Primary: "));
        Serial.println(AirQuality.getPrimary()); // 实时空气质量的主要污染物，优时返回值为 NA
        Serial.println(F("========================"));
    }
    else
    { // 更新失败
        Serial.println("Update Failed...");
        Serial.print("Server Response: ");
        Serial.println(AirQuality.getServerCode()); // 参考 https://dev.heweather.com/docs/start/status-code
    }
    delay(50000);
}

// 连接wifi
void initWiFi()
{
    Serial.print("Connecting to ");
    Serial.println(ssid);

    // 定义工作模式，8266 不仅能连接到WI-FI，也可以建立热点让别人连接自己哟。
    WiFi.mode(WIFI_STA);

    // 开始连接
    WiFi.begin(ssid, password);

    // 检查连接是否成功
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    

    // 设置自动重联
    WiFi.setAutoReconnect(true);
    WiFi.persistent(true);

    // 连接完毕，打印自身的IP地址
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.println("");
}
