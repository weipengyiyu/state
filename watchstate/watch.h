#ifndef __WATCH_H_
#define __WATCH_H_

enum WatchEvents {
  Watch_MODE_EVT,
  Watch_SET_EVT,
  Watch_TICK_EVT
};

void Watch_init();
void Watch_Event(enum WatchEvents e);
#endif // __WATCH_H_
