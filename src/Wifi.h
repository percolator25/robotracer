#pragma once
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>  

class Wifi
{
public:
    static const char *ssid();
    static void hotspot();
    static void development();
    static WiFiManager manager;
};
WiFiManager Wifi::manager = WiFiManager();

void Wifi::hotspot()
{   
    WiFi.softAP(ssid(), "password");
    Serial.println("WiFi created");
    Serial.println("IP address: ");
    Serial.println(WiFi.softAPIP());
}

void Wifi::development()
{
    manager.setEnableConfigPortal(false);
    bool success = manager.autoConnect(ssid());
    if (!success)
    {
        manager.setEnableConfigPortal(true);
        manager.autoConnect(ssid());
    }
}

const char *Wifi::ssid()
{
    String hostString = String(ESP.getChipId(), HEX);
    hostString.toUpperCase();
    String ssid = "or_mctrl_" + hostString;
    return ssid.c_str();
};