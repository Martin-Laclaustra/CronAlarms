//  CronAlarms.h - Arduino cron alarms header

#ifndef CronAlarms_h
#define CronAlarms_h

#include <Arduino.h>
#include <time.h> 

extern "C" {
#include "ccronexpr/ccronexpr.h"
}

#if !defined(dtNBR_ALARMS )
#if defined(__AVR__)
#define dtNBR_ALARMS 6   // max is 255
#elif defined(ESP8266)
#define dtNBR_ALARMS 20  // for esp8266 chip - max is 255
#else
#define dtNBR_ALARMS 12  // assume non-AVR has more memory
#endif
#endif

#define USE_SPECIALIST_METHODS  // define this for testing

typedef uint8_t CronID_t;
typedef CronID_t CronId;  // Arduino friendly name

#define dtINVALID_ALARM_ID 255
#define dtINVALID_TIME     (time_t)(-1)

typedef void (*OnTick_t)();  // alarm callback function typedef

// class defining an alarm instance, only used by dtAlarmsClass
class CronEventClass
{
public:
  CronEventClass();
  void updateNextTrigger(bool forced=false);
  cron_expr expr;
  OnTick_t onTickHandler;
  time_t nextTrigger;
  bool isEnabled;  // the timer is only actioned if isEnabled is true
  bool isOneShot;  // the timer will be de-allocated after trigger is processed
};

// class containing the collection of alarms
class CronClass
{
private:
  CronEventClass Alarm[dtNBR_ALARMS];
  uint8_t isServicing;
  uint8_t servicedCronId; // the alarm currently being serviced
  bool globalEnabled = true;
  void serviceAlarms();

public:
  CronClass();

  // Function to create alarms and timers with cron
  CronID_t create(const char * cronstring, OnTick_t onTickHandler, bool isOneShot);
  // isOneShot - trigger once at the given time in the future

  // Function that must be evaluated often (at least once every main loop)
  void delay(unsigned long ms = 0);

  // low level methods
  void globalUpdateNextTrigger();
  void globalenable();                // stop silencing all alarms
  void globaldisable();               // silence all alarms
  bool isGlobalEnabled();
  void enable(CronID_t ID); // enable the alarm to trigger
  void disable(CronID_t ID);               // prevent the alarm from triggering
  bool isEnabled(const CronID_t ID);
  CronID_t getTriggeredCronId() const;        // returns the currently triggered  alarm id
  bool getIsServicing() const;                    // returns isServicing

  void free(CronID_t ID);                  // free the id to allow its reuse

#ifndef USE_SPECIALIST_METHODS
private:  // the following methods are for testing and are not documented as part of the standard library
#endif
  uint8_t count() const;                          // returns the number of allocated timers
  time_t getNextTrigger() const;                  // returns the time of the next scheduled alarm
  time_t getNextTrigger(CronID_t ID) const;      // returns the time of scheduled alarm
  bool isAllocated(CronID_t ID) const;           // returns true if this id is allocated
};

extern CronClass Cron;  // make an instance for the user

#endif /* CronAlarms_h */
