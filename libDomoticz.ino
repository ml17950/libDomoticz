#define SKETCH_NAME __FILE__
#define SKETCH_VERS "23.11.02"

#include "libDomoticz.h"
#include "libDomoticz.h"

#define WIFI_SSID       "your-ssid"
#define WIFI_PASS       "your-pass"

#define DOMOTICZ_URL    "http://ip.of.domoticz.host:1080/"

Domoticz domo;

void setup() {
  // init serial
  Serial.begin(115200);
  Serial.setDebugOutput(false);
  delay(500);
  Serial.printf("\n\n%s / version %s\n\n", SKETCH_NAME, SKETCH_VERS);

  initWiFi();

  domo.setControllerUrl(DOMOTICZ_URL);
  domo.debugOn();

  domo.updateOnOffAction(12, "on", "off");

  if (1 == 0) {
    domo.uservarUpdate("lib-test-vari", "eins");
    delay(5000);
    domo.uservarUpdate("lib-test-vari", "zwei");
    delay(5000);
    domo.uservarUpdate("lib-test-vari", 123);
  }

  Serial.println("setup ready");
}

void loop() {
/*
  domo.setRssiLevel(50);
  domo.setBatteryLevel(50);

  domo.setRssiLevel(255);
  domo.setBatteryLevel(255);

  
  domo.switchOn(197);
  delay(1000);
  domo.switchOff(197);
  delay(1000);
  domo.switchToggle(197);
  delay(1000);
  domo.switchStop(197);
  delay(1000);

  domo.groupOn(6);
  delay(1000);
  domo.groupOff(6);
  delay(1000);
  domo.groupToggle(6);
  delay(1000);
  domo.sceneOn(6);
  delay(1000);

  domo.uservarCreate("lib-test-int", 1);
  domo.uservarCreate("lib-test-float", (float)1.23);
  domo.uservarCreate("lib-test-str", "eins");
  delay(10000);
  domo.uservarUpdate("lib-test-int", 2);
  domo.uservarUpdate("lib-test-float", (float)2.45);
  domo.uservarUpdate("lib-test-str", "zwei");
  delay(10000);
  domo.uservarDelete(14);
  domo.uservarDelete(15);
  domo.uservarDelete(16);

  domo.sensorTemperature(181, (float)-22.22);
  domo.sensorHumidity(193, 88);

  domo.sensorTemperatureHumidity(199, (float)0.34, 60);
  domo.sensorVoltage(200, (float)2.82);
*/
  
  delay(10000);
}

void initWiFi() {
  Serial.print("Connecting to WiFi ...... ");

  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
  }

  char buf[100];
  sprintf(buf, "WiFi connected to %s with IP %s", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
  Serial.println(buf);
}
