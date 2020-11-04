CronAlarms
==========
Using expressions suitable for the program cron (crontab syntax), the library allows performing tasks at specific times or after specific intervals.

It depends on ctime library, provided by SDKs.

API resembles the popular TimeAlarms library. Tasks can be created to continuously repeat or to occur only once. It is a wrapper of ccronexpr.

Usage
-----
Your sketch should call the `Cron.delay()` function in the main loop. It can also replace the Arduino delay() function if a time in milliseconds is specified as argument. The timeliness of triggers depends on sketch calling this function often. Alarms are serviced in the `Cron.delay()` method.

Here is how you create an alarm to trigger a task repeatedly at a particular time of day:

`Cron.create("0 30 8 * * *", MorningAlarm, false);`

This would call the function MorningAlarm() at 8:30 am every day.

If you want the alarm to trigger only once you can set the last argument to true:

`Cron.create("0 30 8 * * *", MorningAlarm, true);`

This calls a MorningAlarm() function in a sketch once only (when the time is next 8:30am)

Alarms can be specified to trigger a task repeatedly at a particular day of week and time of day:

`Cron.create("0 15 9 * * 6", WeeklyAlarm, false);`

This would call the function WeeklyAlarm() at 9:15am every Saturday.

If you want the alarm to trigger once only on a particular day and time you can do this:

`Cron.create("0 15 9 * * 6", SaturdayMorning, true);`

This would call the function SaturdayMorning() Alarm on the next Saturday at 9:15am.

Periodicity can trigger tasks that occur after a specified interval of time has passed.
Intervals can be in fractions of minutes, of hours, or of days.

`Cron.create("*/15 * * * * *", Repeats, false);`

This calls the Repeats() function in your sketch every 15 seconds.
Please take into account that only round fractions work well. This is a limitation of cron.

If you want an action to happen once only, this might not be the optimal library, although you can define the next fraction of time in which it will occur.

`Cron.create("*/10 * * * * *", OnceOnly, true);`

This calls the onceOnly() function the next time seconds in the clock reaches a multiple of 10, after the timer is created.

You can also set specific dates and times within a year, i.e. noon of 4th July.

`Cron.create("0 0 12 4 7 *", Celebration, true);`

Other low level functions:
- disable( ID);  -  prevent the alarm associated with the given ID from triggering   
- enable(ID);  -  enable the alarm 
- getTriggeredAlarmId();   -  returns the currently triggered  alarm id, only valid in an alarm callback

- globalUpdateNextTrigger(), globalenable(), and globaldisable() - can be used to temporarily suspend activity during timesetting or time zone change

FAQ
---
_Q: What hardware and software is needed to use this library?_

A: This library requires an SDK with a ctime implementation. No internal or external hardware is used by the Alarm library.

_Q: Why must I use Cron.delay() instead of delay()?_

A: Task scheduling is handled in the Cron.delay function.
Tasks are monitored and triggered from within the Cron.delay call so Cron.delay should be called whenever a delay is required in your sketch.
If your sketch waits on an external event (for example, a sensor change), make sure you repeatedly call Cron.delay while checking the sensor.
You can call Cron.delay() if you need to service the scheduler without a delay.

_Q: Are there any restrictions on the code in a task handler function?_

A: No. The scheduler does not use interrupts so your task handling function is no different from other functions you create in your sketch. 

_Q: What are the intervals that can be scheduled?_

A: You can find an introduction to crontab here:
https://en.wikipedia.org/wiki/Cron#CRON_expression

(If you need timer intervals shorter than 1 second then you should look for a different library)

_Q: How are scheduled tasks affected if the system time is changed?_

A: Tasks are scheduled for specific times designated by the system clock. If the system time is reset to a later time (for example one hour ahead) then all alarms will occur one hour later.

If the system time is set backwards (for example one hour back) then the alarms will occur an hour earlier.

If the time is reset before the time a task was scheduled, then the task will be triggered on the next service (the next call to Cron.delay).
This is  the expected behaviour for Alarms (tasks scheduled for a specific time of day will trigger at that time), but the affect on task for subfractions may not be intuitive. If a timer is scheduled to trigger in 5 minutes time and the clock is set ahead by one hour, that timer will not trigger until one hour and 5 minutes has elapsed.

_Q: How many alarms can be created?_

A: It depends on the system. Up to six alarms can be scheduled in Arduino.
The number of alarms can be changed in the CronAlarms header file (set by the constant dtNBR_ALARMS)

onceOnly Alarms are freed when they are triggered so another onceOnly alarm can be set to trigger again.

There is no limit to the number of times a onceOnly alarm can be reset.

