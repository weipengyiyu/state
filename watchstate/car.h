#ifndef __CAR_H_
#define __CAR_H_
#include "hsm.h"

typedef struct
{
    Hsm super;
    State manual, setting, automode, forcebrake;
    State automode_running, automode_idle;
    State running_adjust, running_stop, running_foward, running_back, running_turn;
    State *carstarthist;
}Car;

enum CarEvents{
    Car_CHMODE_EVT,
    Car_SET_EVT,
    Car_AUTO_EVT,
    Car_TICK_EVT,
    Car_RFID_EVT
};

/*
 * event 类型转换
 * pEvt 指向Msg及其子类的指针
 * type 想要转换成的子类
 *
 * 返回: type类型的指针
 */
#define EVT_CAST(pEvt, type) ((type *) pEvt)

/*
 * event 设置事件类型
 *
 * pEvt 指向Msg及其子类的指针
 * msg 想要切换的事件类型
 *
 * 返回 ：无
 */
#define EVT_SETTYPE(pEvt, msg) EVT_CAST(pEvt, Msg)->evt = (int) msg

/*
 *  切换模式命令
 */
typedef struct {
	Msg super;
	/*
	 * 模式跳转
	 */
	int mode;
	int errorcode;
}evt_chmode_t;

typedef struct {
	Msg super;
	char mem[512];
}evt_placeholder_t;

#endif // __CAR_H_
