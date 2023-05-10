#include <Time.hpp>

Time::Time(tmElements_t tm, NTPClient timeClient) : tm_(tm), timeClient_(timeClient)
{
}

Time::~Time()
{
}

void Time::setupNTP()
{
  timeOffset_ = (DST_)? 3600 : 7200; 
  timeClient_.begin(); 
  timeClient_.setTimeOffset(timeOffset_); //7200 summer time // 3600 winter time
  timeClient_.update();
  setDate(__DATE__);
  setTime(timeClient_.getHours(), timeClient_.getMinutes(), timeClient_.getSeconds());
}

bool Time::setTime(int hour, int min, int sec)
{
  tm_.Hour = hour;
  tm_.Minute = min;
  tm_.Second = sec;
  return true;
}

bool Time::setDate(const char *str)
{
  char Month[12];
  int Day, Year;
  uint8_t monthIndex;

  if (sscanf(str, "%s %d %d", Month, &Day, &Year) != 3) return false;
  for (monthIndex = 0; monthIndex < 12; monthIndex++) {
    if (strcmp(Month, monthName_[monthIndex]) == 0) break;
  }
  if (monthIndex >= 12) return false;
  tm_.Day = Day;
  tm_.Month = monthIndex + 1;
  tm_.Year = CalendarYrToTm(Year);
  return true;
}

void Time::printTime()
{
  Serial.print("Ok, Time = ");
  Serial.print(tm_.Hour);
  Serial.write(':');
  Serial.print(tm_.Minute);
  Serial.write(':');
  Serial.print(tm_.Second);
  Serial.println("");
}

void Time::printDate()
{
  //Reads and displays time from RTC tm
  Serial.print("Date (D/M/Y) = ");
  Serial.print(tm_.Day);
  Serial.write('/');
  Serial.print(tm_.Month);
  Serial.write('/');
  Serial.println(tmYearToCalendar(tm_.Year));
}

void Time::getTime(char * instring)
{
  timeClient_.update();
  setTime(timeClient_.getHours(), timeClient_.getMinutes(), timeClient_.getSeconds());
  sprintf(instring, "%02d:%02d:%02d\0", tm_.Hour, tm_.Minute, tm_.Second);
}

void Time::getDate(char * instring)
{
  setDate(__DATE__);
  sprintf(instring, "%02d/%02d/%04d\0", tm_.Day, tm_.Month, tmYearToCalendar(tm_.Year));
}