#include <Arduino.h>
#include <TimeLib.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

//Includes handling of NTP time and RTC time
class Time
{
private:
    const char *monthName_[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    String weekDays[7]={};
    long timeOffset_;
    bool DST_ = false;
public:
    Time(tmElements_t, NTPClient);
    ~Time();
    void setupNTP();
    bool setTime(int hour, int min, int sec);
    bool setDate(const char *str);
    void getTime(char * instring);
    void getDate(char * instring);
    void printTime();
    void printDate();

    tmElements_t tm_;
    NTPClient timeClient_;
};
