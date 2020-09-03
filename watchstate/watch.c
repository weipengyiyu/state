/**
 * Simple digital watch example
 * M. Samek, 01/07/00
 */
#include <assert.h>
#include <stdio.h>
#include "hsm.h"
#include "watch.h"

typedef struct Watch Watch;
struct Watch {
  Hsm super;
  State timekeeping, time, date;
  State setting, hour, minute, day, month;
  State *timekeepingHist;
  int tsec, tmin, thour, dday, dmonth;
};


void WatchShowTime(Watch *me) {
  printf("time: %2d:%02d:%02d\n",me->thour, me->tmin, me->tsec);
}

void WatchShowDate(Watch *me) {
    printf("date: %02d-%02d\n", me->dmonth, me->dday);
}

void WatchTick(Watch *me) {
  static int const month[] = {
     31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
  };
  if (++me->tsec == 60) {
    me->tsec = 0;
    if (++me->tmin == 60) {
      me->tmin = 0;
      if (++me->thour == 24) {
        me->thour = 0;
        if (++me->dday == month[me->dmonth-1]+1) {
          me->dday = 1;
          if (++me->dmonth == 13)
            me->dmonth = 1;
        }
      }
    }
  }
}

Msg const *Watch_top(Watch *me, Msg *msg) {
  switch (msg->evt) {
  case START_EVT:
    printf("l entry top state !\n");
    STATE_START(me, &me->timekeeping);
    return 0;
//  case Watch_TICK_EVT:
//    if (++me->tsec == 60)
//      me->tsec = 0;
//    printf("Watch::top-TICK;");
//    WatchShowTime(me);
//    return 0;
  case EXIT_EVT:
    printf("l exit top state !\n");
    return 0;
  }
  return msg;
}

Msg const *Watch_timekeeping(Watch *me, Msg *msg) {
  switch (msg->evt) {
  case START_EVT://-1
      printf("l entry timekeeping state START_EVT!\n");
    STATE_START(me, me->timekeepingHist);
    return 0;

  case Watch_SET_EVT://1
      printf("l entry timekeeping state Watch_SET_EVT!\n");
    STATE_TRAN(me, &me->setting);
    printf("Watch::timekeeping-SET;\n");
    return 0;

  case EXIT_EVT://-3
      printf("l entry timekeeping state EXIT_EVT!\n");
    me->timekeepingHist = STATE_CURR(me);
    //printf("l exit state !\n");
    return 0;
  }
  return msg;
}

Msg const *Watch_time(Watch *me, Msg *msg) {
  switch (msg->evt) {
  case ENTRY_EVT:
      printf("l entry Watch_time state ENTRY_EVT!\n");
    WatchShowTime(me);
    return 0;
  case Watch_MODE_EVT:
      printf("l entry Watch_time state Watch_MODE_EVT!\n");
    //STATE_TRAN(me, &me->date);
    STATE_TRAN(me, &me->setting);
    printf("Watch::time-DATE;\n");
    return 0;
  case Watch_TICK_EVT:
      printf("l entry Watch_time state Watch_TICK_EVT!\n");
    printf("Watch::time-TICK;\n");
    WatchTick(me);
    WatchShowTime(me);
    return 0;
  }
  return msg;
}

Msg const *Watch_date(Watch *me, Msg *msg) {
  switch (msg->evt) {
  case ENTRY_EVT:
      printf("l entry Watch_date state ENTRY_EVT!\n");
    WatchShowDate(me);
    return 0;
  case Watch_MODE_EVT:
      printf("l entry Watch_date state Watch_MODE_EVT!\n");
    STATE_TRAN(me, &me->time);
    printf("Watch::date-DATE;\n");
    return 0;
  case Watch_TICK_EVT:
      printf("l entry Watch_date state Watch_TICK_EVT!\n");
    printf("Watch::date-TICK;\n");
    WatchTick(me);
    WatchShowDate(me);
    return 0;
  }
  return msg;
}

Msg const *Watch_setting(Watch *me, Msg *msg) {
  switch (msg->evt) {
  case START_EVT:
      printf("l entry Watch_setting state START_EVT!\n");
    STATE_START(me, &me->hour);
    return 0;
  }
  return msg;
}

Msg const *Watch_hour(Watch *me, Msg *msg) {
  switch (msg->evt) {
  case START_EVT:
      printf("l entry Watch_hour state START_EVT!\n");
	  printf("setting hour:\n");
	  WatchShowTime(me);
	  return 0;
  case Watch_SET_EVT:
      printf("l entry Watch_hour state Watch_SET_EVT!\n");
    STATE_TRAN(me, &me->minute);
    printf("Watch::hour-SET;\n");
    return 0;
  case Watch_MODE_EVT:
      printf("l entry Watch_hour state Watch_MODE_EVT!\n");
	  if(++me->thour==24){
		  me->thour =0;
	  }
	  STATE_TRAN(me, &me->hour);
	  return 0;

  }
  return msg;
}

Msg const *Watch_minute(Watch *me, Msg *msg) {
  switch (msg->evt) {
  case START_EVT:
      printf("l entry Watch_minute state START_EVT!\n");
	  printf("setting minute:\n");
	  WatchShowTime(me);
	  return 0;
  case Watch_SET_EVT:
      printf("l entry Watch_minute state Watch_SET_EVT!\n");
    STATE_TRAN(me, &me->day);
    printf("Watch::min-SET;\n");
    return 0;
  case Watch_MODE_EVT:
      printf("l entry Watch_minute state Watch_MODE_EVT!\n");
  	  if(++me->tmin==60){
  		  me->tmin =0;
  	  }
  	  STATE_TRAN(me, &me->minute);
  	  return 0;
  }
  return msg;
}

Msg const *Watch_day(Watch *me, Msg *msg) {
  switch (msg->evt) {
  case START_EVT:
      printf("l entry Watch_day state START_EVT!\n");
	  printf("setting day:\n");
	  WatchShowDate(me);
	  return 0;
  case Watch_SET_EVT:
      printf("l entry Watch_day state Watch_SET_EVT!\n");
    STATE_TRAN(me, &me->month);
    printf("Watch::day-SET;\n");
    return 0;

  case Watch_MODE_EVT:
       printf("l entry Watch_day state Watch_MODE_EVT!\n");
  	  if(++me->dday==32){
  		  me->dday =0;
  	  }
  	  STATE_TRAN(me, &me->day);
  	  return 0;

  }
  return msg;
}

Msg const *Watch_month(Watch *me, Msg *msg) {
  switch (msg->evt) {
  case START_EVT:
      printf("l entry Watch_month state START_EVT!\n");
  	  printf("setting month:\n");
  	  WatchShowDate(me);
  	  return 0;
  case Watch_SET_EVT:
      printf("l entry Watch_month state Watch_SET_EVT!\n");
    STATE_TRAN(me, &me->timekeeping);
    printf("Watch::month-SET;\n");
    return 0;

  case Watch_MODE_EVT:
      printf("l entry Watch_month state Watch_MODE_EVT!\n");
   	  if(++me->dmonth==13){
   		  me->dmonth =0;
   	  }
   	  STATE_TRAN(me, &me->month);
   	  return 0;

  }
  return msg;
}

void WatchCtor(Watch *me) {
  HsmCtor((Hsm *)me, "Watch", (EvtHndlr)Watch_top);
  StateCtor(&me->timekeeping, "timekeeping",&((Hsm *)me)->top, (EvtHndlr)Watch_timekeeping);

    StateCtor(&me->time, "time", &me->timekeeping,(EvtHndlr)Watch_time);

    StateCtor(&me->date, "date", &me->timekeeping,(EvtHndlr)Watch_date);

  StateCtor(&me->setting, "setting", &((Hsm *)me)->top,(EvtHndlr)Watch_setting);

    StateCtor(&me->hour, "hour", &me->setting,(EvtHndlr)Watch_hour);

    StateCtor(&me->minute, "minute", &me->setting,(EvtHndlr)Watch_minute);

    StateCtor(&me->day, "day", &me->setting,(EvtHndlr)Watch_day);

    StateCtor(&me->month, "month", &me->setting,(EvtHndlr)Watch_month);

  me->timekeepingHist = &me->time;
  //me->timekeepingHist = &me->date;

  me->tsec = me->tmin = me->thour = 0;
  me->dday = me->dmonth = 1;
}

const Msg watchMsg[] = {
  Watch_MODE_EVT,
  Watch_SET_EVT,
  Watch_TICK_EVT
};

static Watch watch;
void Watch_init() {

  WatchCtor(&watch);
  HsmOnStart((Hsm *)&watch);
}

void Watch_Event(enum WatchEvents e)
{
	  HsmOnEvent((Hsm *)&watch, (Msg const *) &e);
}
