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
 * event ����ת��
 * pEvt ָ��Msg���������ָ��
 * type ��Ҫת���ɵ�����
 *
 * ����: type���͵�ָ��
 */
#define EVT_CAST(pEvt, type) ((type *) pEvt)

/*
 * event �����¼�����
 *
 * pEvt ָ��Msg���������ָ��
 * msg ��Ҫ�л����¼�����
 *
 * ���� ����
 */
#define EVT_SETTYPE(pEvt, msg) EVT_CAST(pEvt, Msg)->evt = (int) msg

/*
 *  �л�ģʽ����
 */
typedef struct {
	Msg super;
	/*
	 * ģʽ��ת
	 */
	int mode;
	int errorcode;
}evt_chmode_t;

typedef struct {
	Msg super;
	char mem[512];
}evt_placeholder_t;

#endif // __CAR_H_
