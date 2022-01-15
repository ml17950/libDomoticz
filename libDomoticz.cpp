#include "libDomoticz.h"

Domoticz::Domoticz(const char* url) {
  this->DOMOTICZ_URL  = url;
  this->RSSI_LEVEL    = 12;
  this->BATTERY_LEVEL = 255;
}

const char* Domoticz::getVersion() {
  return "22.01.15";
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
  Serial.printf("  %s [%s] -> ", FPSTR(__func__), url);

  WiFiClient client;
  HTTPClient http;
  
  http.begin(client, url);
  int httpResponseCode = http.GET();
  http.end(); // Free resources

  Serial.printf("%d\n", httpResponseCode);
}

void Domoticz::switchOn(int id) {
  Serial.printf(" %s [%d]\n", FPSTR(__func__), id);
  char urlbuf[255];
  sprintf(urlbuf, "%sjson.htm?type=command&param=switchlight&idx=%d&switchcmd=%s", this->DOMOTICZ_URL, id, "On");
  this->sendApiGet(urlbuf);
}
void Domoticz::switchOff(int id) {
  Serial.printf(" %s [%d]\n", FPSTR(__func__), id);
  char urlbuf[255];
  sprintf(urlbuf, "%sjson.htm?type=command&param=switchlight&idx=%d&switchcmd=%s", this->DOMOTICZ_URL, id, "Off");
  this->sendApiGet(urlbuf);
}
void Domoticz::switchToggle(int id) {
  Serial.printf(" %s [%d]\n", FPSTR(__func__), id);
  char urlbuf[255];
  sprintf(urlbuf, "%sjson.htm?type=command&param=switchlight&idx=%d&switchcmd=%s", this->DOMOTICZ_URL, id, "Toggle");
  this->sendApiGet(urlbuf);
}
void Domoticz::switchStop(int id) {
  Serial.printf(" %s [%d]\n", FPSTR(__func__), id);
  char urlbuf[255];
  sprintf(urlbuf, "%sjson.htm?type=command&param=switchlight&idx=%d&switchcmd=%s", this->DOMOTICZ_URL, id, "Stop");
  this->sendApiGet(urlbuf);
}

void Domoticz::groupOn(int id) {
  Serial.printf(" %s [%d]\n", FPSTR(__func__), id);
  char urlbuf[255];
  sprintf(urlbuf, "%sjson.htm?type=command&param=switchscene&idx=%d&switchcmd=%s", this->DOMOTICZ_URL, id, "On");
  this->sendApiGet(urlbuf);
}
void Domoticz::groupOff(int id) {
  Serial.printf(" %s [%d]\n", FPSTR(__func__), id);
  char urlbuf[255];
  sprintf(urlbuf, "%sjson.htm?type=command&param=switchscene&idx=%d&switchcmd=%s", this->DOMOTICZ_URL, id, "Off");
  this->sendApiGet(urlbuf);
}
void Domoticz::groupToggle(int id) {
  Serial.printf(" %s [%d]\n", FPSTR(__func__), id);
  char urlbuf[255];
  sprintf(urlbuf, "%sjson.htm?type=command&param=switchscene&idx=%d&switchcmd=%s", this->DOMOTICZ_URL, id, "Toggle");
  this->sendApiGet(urlbuf);
}
void Domoticz::sceneOn(int id) {
  this->groupOn(id);
}

void Domoticz::sensorTemperature(int id, float temperature) {
  Serial.printf(" %s [%d = %f]\n", FPSTR(__func__), id, temperature);
  char urlbuf[255];
  sprintf(urlbuf, "%sjson.htm?type=command&param=udevice&idx=%d&nvalue=0&svalue=%f&rssi=%d&battery=%d", this->DOMOTICZ_URL, id, temperature, this->RSSI_LEVEL, this->BATTERY_LEVEL);
  this->sendApiGet(urlbuf);
}
void Domoticz::sensorHumidity(int id, int humidity) {
  Serial.printf(" %s [%d = %d]\n", FPSTR(__func__), id, humidity);
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
  
  sprintf(urlbuf, "%sjson.htm?type=command&param=udevice&idx=%d&nvalue=%d&svalue=%d&rssi=%d&battery=%d", this->DOMOTICZ_URL, id, humidity, humi_status, this->RSSI_LEVEL, this->BATTERY_LEVEL);
  this->sendApiGet(urlbuf);
}
void Domoticz::sensorTemperatureHumidity(int id, float temperature, int humidity) {
  Serial.printf(" %s [%d = %f/%d]\n", FPSTR(__func__), id, temperature, humidity);
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

  sprintf(urlbuf, "%sjson.htm?type=command&param=udevice&idx=%d&nvalue=0&svalue=%f;%d;%d&rssi=%d&battery=%d", this->DOMOTICZ_URL, id, temperature, humidity, humi_status, this->RSSI_LEVEL, this->BATTERY_LEVEL);
  this->sendApiGet(urlbuf);
}
void Domoticz::sensorPercentage(int id, int percentage) { /* TODO */ }
void Domoticz::sensorLux(int id, int lux) { /* TODO */ }
void Domoticz::sensorVoltage(int id, float voltage) {
  Serial.printf(" %s [%f]\n", FPSTR(__func__), id, voltage);
  char urlbuf[255];
  sprintf(urlbuf, "%sjson.htm?type=command&param=udevice&idx=%d&nvalue=0&svalue=%f&rssi=%d&battery=%d", this->DOMOTICZ_URL, id, voltage, this->RSSI_LEVEL, this->BATTERY_LEVEL);
  this->sendApiGet(urlbuf);
}
void Domoticz::sensorDistance(int id, float distance) { /* TODO */ }
void Domoticz::sensorCustom(int id, int nvalue, const char* svalue) { /* TODO */ }
void Domoticz::sensorText(int id, const char* text) { /* TODO */ }

void Domoticz::uservarCreate(const char* name, int val) {
  Serial.printf(" %s [%s = %d]\n", FPSTR(__func__), name, val);
  char urlbuf[255];
  sprintf(urlbuf, "%sjson.htm?type=command&param=adduservariable&vname=%s&vtype=0&vvalue=%d", this->DOMOTICZ_URL, name, val);
  this->sendApiGet(urlbuf);
}
void Domoticz::uservarCreate(const char* name, float val) {
  Serial.printf(" %s [%s = %f]\n", FPSTR(__func__), name, val);
  char urlbuf[255];
  sprintf(urlbuf, "%sjson.htm?type=command&param=adduservariable&vname=%s&vtype=1&vvalue=%f", this->DOMOTICZ_URL, name, val);
  this->sendApiGet(urlbuf);
}
void Domoticz::uservarCreate(const char* name, const char* val) {
  Serial.printf(" %s [%s = %s]\n", FPSTR(__func__), name, val);
  char urlbuf[255];
  sprintf(urlbuf, "%sjson.htm?type=command&param=adduservariable&vname=%s&vtype=2&vvalue=%s", this->DOMOTICZ_URL, name, val);
  this->sendApiGet(urlbuf);
}

void Domoticz::uservarUpdate(const char* name, int val) {
  Serial.printf(" %s [%s = %d]\n", FPSTR(__func__), name, val);
  char urlbuf[255];
  sprintf(urlbuf, "%sjson.htm?type=command&param=updateuservariable&vname=%s&vtype=0&vvalue=%d", this->DOMOTICZ_URL, name, val);
  this->sendApiGet(urlbuf);
}
void Domoticz::uservarUpdate(const char* name, float val) {
  Serial.printf(" %s [%s = %f]\n", FPSTR(__func__), name, val);
  char urlbuf[255];
  sprintf(urlbuf, "%sjson.htm?type=command&param=updateuservariable&vname=%s&vtype=1&vvalue=%f", this->DOMOTICZ_URL, name, val);
  this->sendApiGet(urlbuf);
}
void Domoticz::uservarUpdate(const char* name, const char* val) {
  Serial.printf(" %s [%s = %s]\n", FPSTR(__func__), name, val);
  char urlbuf[255];
  sprintf(urlbuf, "%sjson.htm?type=command&param=updateuservariable&vname=%s&vtype=2&vvalue=%s", this->DOMOTICZ_URL, name, val);
  this->sendApiGet(urlbuf);
}

void Domoticz::uservarDelete(int id) {
  Serial.printf(" %s [%d]\n", FPSTR(__func__), id);
  char urlbuf[255];
  sprintf(urlbuf, "%sjson.htm?type=command&param=deleteuservariable&idx=%d", this->DOMOTICZ_URL, id);
  this->sendApiGet(urlbuf);
}
