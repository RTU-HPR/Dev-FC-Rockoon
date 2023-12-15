#pragma once
#include <TinyGPS++.h>

//* Custom definitions for working with Heltec (V3)
#define RX2 9
#define TX2 8

namespace gps
{
    TinyGPSPlus gps;
    boolean hasData = false;
    int _syncTime = 0;

    void setup(uint gpsRate = 9600)
    {
        Serial2.begin(gpsRate, SERIAL_8N1, RX2, TX2); // hardware serial 2 (RX-16, TX-17)
        Serial.println("Init GPS: " + String(gpsRate));
    }

    void readGps()
    {
        hasData = false;

        while (Serial2.available())
        {
            gps.encode(Serial2.read());
            hasData = true;
        }
    }

    int getSyncTime()
    {
        if (hasData && gps.time.isValid()) // if got new valid time data
        {
            _syncTime = gps.time.second() % 4;
        }
        return _syncTime;
    }

    bool checkSyncTimeTrigger(int syncTime)
    {
        return syncTime == 5; //GPS_TIME_TRIGGER
    }

    void printData()
    {
        if (hasData)
        {
            Serial.print("Lat: ");
            Serial.println(gps.location.lat(), 6);
            Serial.print("Long: ");
            Serial.println(gps.location.lng(), 6);
            Serial.print("Alt: ");
            Serial.println(gps.altitude.meters(), 1);
            Serial.print("Sats: ");
            Serial.println(gps.satellites.value(), 1);
            Serial.println();
        }

    }

    void printTime()
    {
        if (hasData)
        {
            Serial.println("Time:\t" + String(gps.time.hour()) + ":" + String(gps.time.minute()) + ":" + String(gps.time.second()));
        }
    }

    double lastLatitude()
    {
        return gps.location.lat();
    }

    double lastLongitude()
    {
        return gps.location.lng();
    }

    double lastAltitude()
    {
        return gps.altitude.meters();
    }

    int getSatellites()
    {
        return gps.satellites.value();
    }

    uint8_t getHour()
    {
        return gps.time.hour();
    }

    uint8_t getMinute()
    {
        return gps.time.minute();
    }

    uint8_t getSecond()
    {
        return gps.time.second();
    }

}