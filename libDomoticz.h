#ifndef libDomoticz_h
#define libDomoticz_h

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <base64.h>

class Domoticz {
  public:
    Domoticz();

    const char*   getVersion();

    void          debugOn();
    void          debugOff();

    void          setControllerUrl(const char* url);
    void          setRssiLevel(int percent);
    void          setBatteryLevel(int percent);

    void          sendApiGet(const char* url);

    void          switchOn(int id);
    void          switchOff(int id);
    void          switchToggle(int id);
    void          switchStop(int id);
    void          switchSetOn(int id);
    void          switchSetOff(int id);

    void          groupOn(int id);
    void          groupOff(int id);
    void          groupToggle(int id);
    void          sceneOn(int id);

    void          sensorTemperature(int id, float temperature);
    void          sensorHumidity(int id, int humidity);
    void          sensorTemperatureHumidity(int id, float temperature, int humidity);
    void          sensorPercentage(int id, int percentage);
    void          sensorLux(int id, int lux);
    void          sensorVoltage(int id, float voltage);
    void          sensorDistance(int id, float distance);
    void          sensorCustom(int id, int nvalue, const char* svalue);
    void          sensorText(int id, const char* text);

    void          uservarCreate(const char* name, int val);
    void          uservarCreate(const char* name, float val);
    void          uservarCreate(const char* name, const char* val);

    void          uservarUpdate(const char* name, int val);
    void          uservarUpdate(const char* name, float val);
    void          uservarUpdate(const char* name, const char* val);
    
    void          uservarDelete(int id);

    void          triggerEvent(const char* name, const char* data);

    void          updateOnOffAction(int id, const char* on_action, const char* off_action);

  private:
    bool          DEBUG_ON;
    const char*   DOMOTICZ_CONTROLLER_URL;
    int           RSSI_LEVEL;
    int           BATTERY_LEVEL;
    int           LAST_RESPONSE_CODE;
    String        LAST_RESPONSE_DATA;
};

#endif
