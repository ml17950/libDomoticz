#include "libDomoticz.h"

Domoticz::Domoticz() {
  this->DEBUG_ON                = false;
  this->RSSI_LEVEL              = 12;
  this->BATTERY_LEVEL           = 255;
}

const char* Domoticz::getVersion() {
  return "23.11.02";
}

void Domoticz::debugOn() {
  this->DEBUG_ON = true;
}

void Domoticz::debugOff() {
  this->DEBUG_ON = false;
}

void Domoticz::setControllerUrl(const char* url) {
  this->DOMOTICZ_CONTROLLER_URL = url;
}

void Domoticz::setRssiLevel(int percent) {
  if (percent == 255)
    this->RSSI_LEVEL = 12; // off
  else
    this->RSSI_LEVEL = map(percent, 0, 100, 0, 10);
}

void Domoticz::setBatteryLevel(int percent) {
  this->BATTERY_LEVEL = percent;
}

void Domoticz::sendApiGet(const char* url) {
  if (this->DEBUG_ON)
    Serial.printf("  %s [%s] -> ", __func__, url);

  WiFiClient client;
  HTTPClient http;

  this->LAST_RESPONSE_CODE = 0;
  this->LAST_RESPONSE_DATA = "";
  
  if (http.begin(client, url)) {
    int httpResponseCode = http.GET();
    //Serial.printf("%d\n", httpResponseCode);
    
    if (httpResponseCode > 0) {
      this->LAST_RESPONSE_CODE = httpResponseCode;
      
      if (httpResponseCode == HTTP_CODE_OK || httpResponseCode == HTTP_CODE_MOVED_PERMANENTLY) {
        String httpResponseData = http.getString();
        //Serial.println(httpResponseData);
        this->LAST_RESPONSE_DATA = httpResponseData;
      }
    }
    http.end(); // Free resources
  }
}

void Domoticz::switchOn(int id) {
  if (this->DEBUG_ON)
    Serial.printf(" %s [%d]\n", __func__, id);
  char urlbuf[255];
  sprintf(urlbuf, "%sjson.htm?type=command&param=switchlight&idx=%d&switchcmd=%s", this->DOMOTICZ_CONTROLLER_URL, id, "On");
  this->sendApiGet(urlbuf);
}
void Domoticz::switchOff(int id) {
  if (this->DEBUG_ON)
    Serial.printf(" %s [%d]\n", __func__, id);
  char urlbuf[255];
  sprintf(urlbuf, "%sjson.htm?type=command&param=switchlight&idx=%d&switchcmd=%s", this->DOMOTICZ_CONTROLLER_URL, id, "Off");
  this->sendApiGet(urlbuf);
}
void Domoticz::switchToggle(int id) {
  if (this->DEBUG_ON)
    Serial.printf(" %s [%d]\n", __func__, id);
  char urlbuf[255];
  sprintf(urlbuf, "%sjson.htm?type=command&param=switchlight&idx=%d&switchcmd=%s", this->DOMOTICZ_CONTROLLER_URL, id, "Toggle");
  this->sendApiGet(urlbuf);
}
void Domoticz::switchStop(int id) {
  if (this->DEBUG_ON)
    Serial.printf(" %s [%d]\n", __func__, id);
  char urlbuf[255];
  sprintf(urlbuf, "%sjson.htm?type=command&param=switchlight&idx=%d&switchcmd=%s", this->DOMOTICZ_CONTROLLER_URL, id, "Stop");
  this->sendApiGet(urlbuf);
}
void Domoticz::switchSetOn(int id) {
  if (this->DEBUG_ON)
    Serial.printf(" %s [%d]\n", __func__, id);
  char urlbuf[255];
  sprintf(urlbuf, "%sjson.htm?type=command&param=udevice&idx=%d&nvalue=%d&svalue=%s&parsetrigger=false", this->DOMOTICZ_CONTROLLER_URL, id, 1, "On");
  this->sendApiGet(urlbuf);
}
void Domoticz::switchSetOff(int id) {
  if (this->DEBUG_ON)
    Serial.printf(" %s [%d]\n", __func__, id);
  char urlbuf[255];
  sprintf(urlbuf, "%sjson.htm?type=command&param=udevice&idx=%d&nvalue=%d&svalue=%s&parsetrigger=false", this->DOMOTICZ_CONTROLLER_URL, id, 0, "Off");
  this->sendApiGet(urlbuf);
}

void Domoticz::groupOn(int id) {
  if (this->DEBUG_ON)
    Serial.printf(" %s [%d]\n", __func__, id);
  char urlbuf[255];
  sprintf(urlbuf, "%sjson.htm?type=command&param=switchscene&idx=%d&switchcmd=%s", this->DOMOTICZ_CONTROLLER_URL, id, "On");
  this->sendApiGet(urlbuf);
}
void Domoticz::groupOff(int id) {
  if (this->DEBUG_ON)
    Serial.printf(" %s [%d]\n", __func__, id);
  char urlbuf[255];
  sprintf(urlbuf, "%sjson.htm?type=command&param=switchscene&idx=%d&switchcmd=%s", this->DOMOTICZ_CONTROLLER_URL, id, "Off");
  this->sendApiGet(urlbuf);
}
void Domoticz::groupToggle(int id) {
  if (this->DEBUG_ON)
    Serial.printf(" %s [%d]\n", __func__, id);
  char urlbuf[255];
  sprintf(urlbuf, "%sjson.htm?type=command&param=switchscene&idx=%d&switchcmd=%s", this->DOMOTICZ_CONTROLLER_URL, id, "Toggle");
  this->sendApiGet(urlbuf);
}
void Domoticz::sceneOn(int id) {
  this->groupOn(id);
}

void Domoticz::sensorTemperature(int id, float temperature) {
  if (this->DEBUG_ON)
    Serial.printf(" %s [%d = %f]\n", __func__, id, temperature);
  char urlbuf[255];
  sprintf(urlbuf, "%sjson.htm?type=command&param=udevice&idx=%d&nvalue=0&svalue=%0.2f&rssi=%d&battery=%d", this->DOMOTICZ_CONTROLLER_URL, id, temperature, this->RSSI_LEVEL, this->BATTERY_LEVEL);
  this->sendApiGet(urlbuf);
}
void Domoticz::sensorHumidity(int id, int humidity) {
  if (this->DEBUG_ON)
    Serial.printf(" %s [%d = %d]\n", __func__, id, humidity);
  char urlbuf[255];
  int humi_status;

  if (humidity > 80)
    humi_status = 3; // wet
  else if (humidity > 60)
    humi_status = 0; // normal
  else if (humidity > 40)
    humi_status = 1; // comfortable
  else if (humidity > 20)
    humi_status = 0; // normal
  else // < 20
    humi_status = 2; // dry
  
  sprintf(urlbuf, "%sjson.htm?type=command&param=udevice&idx=%d&nvalue=%d&svalue=%d&rssi=%d&battery=%d", this->DOMOTICZ_CONTROLLER_URL, id, humidity, humi_status, this->RSSI_LEVEL, this->BATTERY_LEVEL);
  this->sendApiGet(urlbuf);
}
void Domoticz::sensorTemperatureHumidity(int id, float temperature, int humidity) {
  if (this->DEBUG_ON)
    Serial.printf(" %s [%d = %f/%d]\n", __func__, id, temperature, humidity);
  char urlbuf[255];
  int humi_status;

  if (humidity > 80)
    humi_status = 3; // wet
  else if (humidity > 60)
    humi_status = 0; // normal
  else if (humidity > 40)
    humi_status = 1; // comfortable
  else if (humidity > 20)
    humi_status = 0; // normal
  else // < 20
    humi_status = 2; // dry

  sprintf(urlbuf, "%sjson.htm?type=command&param=udevice&idx=%d&nvalue=0&svalue=%0.2f;%d;%d&rssi=%d&battery=%d", this->DOMOTICZ_CONTROLLER_URL, id, temperature, humidity, humi_status, this->RSSI_LEVEL, this->BATTERY_LEVEL);
  this->sendApiGet(urlbuf);
}
void Domoticz::sensorPercentage(int id, int percentage) {
  if (this->DEBUG_ON)
    Serial.printf(" %s [%d = %d]\n", __func__, id, percentage);
  char urlbuf[255];
  sprintf(urlbuf, "%sjson.htm?type=command&param=udevice&idx=%d&nvalue=0&svalue=%d&rssi=%d&battery=%d", this->DOMOTICZ_CONTROLLER_URL, id, percentage, this->RSSI_LEVEL, this->BATTERY_LEVEL);
  this->sendApiGet(urlbuf);
}
void Domoticz::sensorLux(int id, int lux) { /* TODO */ }
void Domoticz::sensorVoltage(int id, float voltage) {
  if (this->DEBUG_ON)
    Serial.printf(" %s [%d = %f]\n", __func__, id, voltage);
  char urlbuf[255];
  sprintf(urlbuf, "%sjson.htm?type=command&param=udevice&idx=%d&nvalue=0&svalue=%0.2f&rssi=%d&battery=%d", this->DOMOTICZ_CONTROLLER_URL, id, voltage, this->RSSI_LEVEL, this->BATTERY_LEVEL);
  this->sendApiGet(urlbuf);
}
void Domoticz::sensorDistance(int id, float distance) { /* TODO */ }
void Domoticz::sensorCustom(int id, int nvalue, const char* svalue) { /* TODO */ }
void Domoticz::sensorText(int id, const char* text) { /* TODO */ }

void Domoticz::uservarCreate(const char* name, int val) {
  if (this->DEBUG_ON)
    Serial.printf(" %s [%s = %d]\n", __func__, name, val);
  char urlbuf[255];
  sprintf(urlbuf, "%sjson.htm?type=command&param=adduservariable&vname=%s&vtype=0&vvalue=%d", this->DOMOTICZ_CONTROLLER_URL, name, val);
  this->sendApiGet(urlbuf);
}
void Domoticz::uservarCreate(const char* name, float val) {
  if (this->DEBUG_ON)
    Serial.printf(" %s [%s = %f]\n", __func__, name, val);
  char urlbuf[255];
  sprintf(urlbuf, "%sjson.htm?type=command&param=adduservariable&vname=%s&vtype=1&vvalue=%0.2f", this->DOMOTICZ_CONTROLLER_URL, name, val);
  this->sendApiGet(urlbuf);
}
void Domoticz::uservarCreate(const char* name, const char* val) {
  if (this->DEBUG_ON)
    Serial.printf(" %s [%s = %s]\n", __func__, name, val);
  char urlbuf[255];
  sprintf(urlbuf, "%sjson.htm?type=command&param=adduservariable&vname=%s&vtype=2&vvalue=%s", this->DOMOTICZ_CONTROLLER_URL, name, val);
  this->sendApiGet(urlbuf);
}

void Domoticz::uservarUpdate(const char* name, int val) {
  if (this->DEBUG_ON)
    Serial.printf(" %s [%s = %d]\n", __func__, name, val);
  char urlbuf[255];
  sprintf(urlbuf, "%sjson.htm?type=command&param=updateuservariable&vname=%s&vtype=0&vvalue=%d", this->DOMOTICZ_CONTROLLER_URL, name, val);
  this->sendApiGet(urlbuf);
  if (this->LAST_RESPONSE_DATA.indexOf("does not exist") > 0) {
    this->uservarCreate(name, val);
  }
}
void Domoticz::uservarUpdate(const char* name, float val) {
  if (this->DEBUG_ON)
    Serial.printf(" %s [%s = %f]\n", __func__, name, val);
  char urlbuf[255];
  sprintf(urlbuf, "%sjson.htm?type=command&param=updateuservariable&vname=%s&vtype=1&vvalue=%0.2f", this->DOMOTICZ_CONTROLLER_URL, name, val);
  this->sendApiGet(urlbuf);
  if (this->LAST_RESPONSE_DATA.indexOf("does not exist") > 0) {
    this->uservarCreate(name, val);
  }
}
void Domoticz::uservarUpdate(const char* name, const char* val) {
  if (this->DEBUG_ON)
    Serial.printf(" %s [%s = %s]\n", __func__, name, val);
  char urlbuf[255];
  sprintf(urlbuf, "%sjson.htm?type=command&param=updateuservariable&vname=%s&vtype=2&vvalue=%s", this->DOMOTICZ_CONTROLLER_URL, name, val);
  this->sendApiGet(urlbuf);
  if (this->LAST_RESPONSE_DATA.indexOf("does not exist") > 0) {
    this->uservarCreate(name, val);
  }
}

void Domoticz::uservarDelete(int id) {
  if (this->DEBUG_ON)
    Serial.printf(" %s [%d]\n", __func__, id);
  char urlbuf[255];
  sprintf(urlbuf, "%sjson.htm?type=command&param=deleteuservariable&idx=%d", this->DOMOTICZ_CONTROLLER_URL, id);
  this->sendApiGet(urlbuf);
}

void Domoticz::triggerEvent(const char* name, const char* data) {
  if (this->DEBUG_ON)
    Serial.printf(" %s [%s = %s]\n", __func__, name, data);
  char urlbuf[255];
  sprintf(urlbuf, "%sjson.htm?type=command&param=customevent&event=%s&data=%s", this->DOMOTICZ_CONTROLLER_URL, name, data);
  this->sendApiGet(urlbuf);
}

void Domoticz::updateOnOffAction(int id, const char* on_action, const char* off_action) {
  if (this->DEBUG_ON)
    Serial.printf(" %s [%s / %s]\n", __func__, on_action, off_action);

  char tmpbuf[255];
  char urlbuf[255];

  IPAddress ip = WiFi.localIP();
  
  sprintf(tmpbuf, "http://%u.%u.%u.%u/%s", ip[0], ip[1], ip[2], ip[3], on_action);
  size_t onbuf_length = sizeof(tmpbuf);
  char onbuf[base64::encodeLength(onbuf_length)];
  base64::encode((const uint8_t*)tmpbuf, onbuf_length, onbuf);

  sprintf(tmpbuf, "http://%u.%u.%u.%u/%s", ip[0], ip[1], ip[2], ip[3], off_action);
  size_t ofbuf_length = strlen(tmpbuf);
  char ofbuf[base64::encodeLength(ofbuf_length)];
  base64::encode((const uint8_t*)tmpbuf, ofbuf_length, ofbuf);

  sprintf(urlbuf, "%sjson.htm?type=setused&used=true&idx=%d&strparam1=%s&strparam2=%s", this->DOMOTICZ_CONTROLLER_URL, id, onbuf, ofbuf);
  this->sendApiGet(urlbuf);
}
