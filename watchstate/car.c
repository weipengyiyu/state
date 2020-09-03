#include <assert.h>
#include <stdio.h>
#include "hsm.h"
#include "car.h"

static int  mode;

/**
*ENTRY_EVT 不能跳转状态；每次进入状态，都会执行这个事件
*START_EVT 才能跳转状态，以及其他自定义事件才能跳转状态；不管进入多少次这个状态，只执行一次
**/

Msg const *Car_top(Car *me, Msg *msg)
{
    switch(msg->evt)
    {
        case START_EVT:
            printf("l am top start evt!\n");
            STATE_START(me, me->carstarthist);
            return 0;
        case ENTRY_EVT:
            printf("l am top ENTRY evt!\n");
            return 0;
        case Car_CHMODE_EVT:
            printf("l am top chmode evt!\n");
            evt_chmode_t* pevt = EVT_CAST(msg, evt_chmode_t);
            switch(pevt->mode)
            {
                case 1:
                    STATE_TRAN(me, &me->manual);
                    break;
                case 2:
                    STATE_TRAN(me, &me->setting);
                    break;
                case 3:
                    STATE_TRAN(me, &me->automode);
                    break;
                case 4:
                    STATE_TRAN(me, &me->forcebrake);
                    break;
				default:
					break;
            }
            return 0;
        case EXIT_EVT:
            printf("l am top exit evt!\n");
            return 0;
    }

    return msg;
}

Msg const *Car_manual(Car *me, Msg *msg)
{
    switch(msg->evt)
    {
        case START_EVT:
            printf("l am manual start evt!\n");
            return 0;
        case ENTRY_EVT:
            printf("l am manual entry evt!\n");
            return 0;
        case EXIT_EVT:
            printf("l am manual exit evt!\n");
            return 0;
    }

    return msg;
}

Msg const *Car_setting(Car *me, Msg *msg)
{
    switch(msg->evt)
    {
        case START_EVT:
            printf("l am setting start evt!\n");
            return 0;
        case ENTRY_EVT:
            printf("l am setting entry evt!\n");
            return 0;
        case EXIT_EVT:
            printf("l am setting exit evt!\n");
            return 0;
    }

    return msg;
}

Msg const *Car_automode(Car *me, Msg *msg)
{
    switch(msg->evt)
    {
        case START_EVT:
            printf("l am automode start evt!\n");
            STATE_START(me, &me->automode_idle);
            return 0;
        case ENTRY_EVT:
            printf("l am automode entry evt!\n");
            return 0;
        case EXIT_EVT:
            printf("l am automode exit evt!\n");
            return 0;
    }

    return msg;
}

Msg const *Car_automode_idle(Car *me, Msg *msg)
{
    switch(msg->evt)
    {
        case ENTRY_EVT:
            printf("l am automode_idle entry evt!\n");
            return 0;
        case EXIT_EVT:
            printf("l am automode_idle exit evt!\n");
            return 0;
        case Car_RFID_EVT:
            printf("l am automode_idle rfid evt!\n");
            STATE_TRAN(me, &me->automode_running);
            return 0;
    }

    return msg;
}

Msg const *Car_automode_running(Car *me, Msg *msg)
{
    switch(msg->evt)
    {
        case START_EVT:
            printf("l am automode_running start evt!\n");
            STATE_START(me, &me->running_adjust);
            return 0;
        case ENTRY_EVT:
            printf("l am automode_running entry evt!\n");
            return 0;

        case Car_RFID_EVT:
            printf("l am automode_running rfiddd evt!\n");
            //STATE_TRAN(me, &me->running_adjust);
            return 0;
    }

    return msg;
}

Msg const *Car_running_adjust(Car *me, Msg *msg)
{
    switch(msg->evt)
    {
        case ENTRY_EVT:
            printf("l am running_adjust entry evt!\n");
            return 0;
        case EXIT_EVT:
            printf("l am running_adjust exit evt!\n");
            return 0;
        case Car_TICK_EVT:
            printf("l am running_adjust tick evt!\n");
            mode = get_mode();
            if(mode == 2)
            {
                //printf("l am running_foward mode %d!\n", mode);
                STATE_TRAN(me, &me->running_foward);
            }
            if(mode == 3)
            {
                //printf("l am running_back mode %d!\n", mode);
                STATE_TRAN(me, &me->running_back);
            }
            return 0;
    }

    return msg;
}
Msg const *Car_running_foward(Car *me, Msg *msg)
{
    switch(msg->evt)
    {
        case ENTRY_EVT:
            printf("l am running_foward entry evt!\n");
            return 0;
        case EXIT_EVT:
            printf("l am running_foward exit evt!\n");
            return 0;
        case Car_TICK_EVT:
            printf("l am running_foward tick evt!\n");
            mode = get_mode();
            if(mode == 1)
            {
                //printf("l am running_adjust mode %d!\n", mode);
                STATE_TRAN(me, &me->running_adjust);
            }
            if(mode == 3)
            {
                //printf("l am running_back mode %d!\n", mode);
                STATE_TRAN(me, &me->running_back);
            }
            return 0;
    }

    return msg;
}
Msg const *Car_running_back(Car *me, Msg *msg)
{
    switch(msg->evt)
    {
        case ENTRY_EVT:
            printf("l am running_back entry evt!\n");
            return 0;
        case EXIT_EVT:
            printf("l am running_back exit evt!\n");
            return 0;
        case Car_TICK_EVT:
            printf("l am running_back tick evt!\n");
            mode = get_mode();
            if(mode == 2)
            {
                //printf("l am running_foward mode %d!\n", mode);
                STATE_TRAN(me, &me->running_foward);
            }
            if(mode == 1)
            {
                //printf("l am running_adjust mode %d!\n", mode);
                STATE_TRAN(me, &me->running_adjust);
            }
            return 0;
    }

    return msg;
}
#if 0
Msg const *Car_running_stop(Car *me, Msg *msg)
{
    switch(msg->evt)
    {
        case ENTRY_EVT:
            printf("l am running_stop entry evt!\n");
            return 0;
        case EXIT_EVT:
            printf("l am running_stop exit evt!\n");
            return 0;
        case Car_TICK_EVT:
            printf("l am running_stop tick evt!\n");
            mode = get_mode();
            printf("l am  mode %d !\n", mode);
            if(mode == 1)
            {
                STATE_START(me, &me->running_turn);
            }
            else if(mode == 2)
            {
                STATE_START(me, &me->running_stop);
            }
            else if(mode == 3)
            {
                STATE_START(me, &me->running_foward);
            }
            else if(mode == 4)
            {
                STATE_START(me, &me->running_back);
            }
    }

    return msg;
}
#endif

#if 0
Msg const *Car_running_turn(Car *me, Msg *msg)
{
    switch(msg->evt)
    {
        case ENTRY_EVT:
            printf("l am running_turn entry evt!\n");
            return 0;
        case EXIT_EVT:
            printf("l am running_turn exit evt!\n");
            return 0;
    }

    return msg;
}
#endif
Msg const *Car_forcebrake(Car *me, Msg *msg)
{
    switch(msg->evt)
    {
        case START_EVT:
            printf("l am forcebrake start evt!\n");
            return 0;
        case ENTRY_EVT:
            printf("l am forcebrake entry evt!\n");
            return 0;
        case EXIT_EVT:
            printf("l am forcebrake exit evt!\n");
            return 0;
    }

    return msg;
}

void CarCtor(Car *me)
{
    HsmCtor((Hsm*)me, "Car", (EvtHndlr)Car_top);

    StateCtor(&me->manual, "manual", &((Hsm*)me)->top, (EvtHndlr)Car_manual);
    StateCtor(&me->setting, "setting", &((Hsm*)me)->top, (EvtHndlr)Car_setting);
    StateCtor(&me->automode, "automode", &((Hsm*)me)->top, (EvtHndlr)Car_automode);
    StateCtor(&me->forcebrake, "forcebrake", &((Hsm*)me)->top, (EvtHndlr)Car_forcebrake);

    StateCtor(&me->automode_idle, "automode_idle", &me->automode, (EvtHndlr)Car_automode_idle);
    StateCtor(&me->automode_running, "automode_running", &me->automode, (EvtHndlr)Car_automode_running);
    //StateCtor(&me->running_stop, "running_stop", &me->automode_running, (EvtHndlr)Car_running_stop);
    //StateCtor(&me->running_turn, "running_turn", &me->automode_running, (EvtHndlr)Car_running_turn);

    StateCtor(&me->running_adjust, "running_adjust", &me->automode_running, (EvtHndlr)Car_running_adjust);
    StateCtor(&me->running_foward, "running_foward", &me->automode_running, (EvtHndlr)Car_running_foward);
    StateCtor(&me->running_back, "running_back", &me->automode_running, (EvtHndlr)Car_running_back);


    me->carstarthist = &me->manual;
}

const Msg carMsg[] = {
    Car_CHMODE_EVT,
    Car_SET_EVT,
    Car_AUTO_EVT,
    Car_TICK_EVT
};

static Car car;
void Car_init()
{
    CarCtor(&car);
    HsmOnStart((Hsm*)&car);
}

void Car_Event(enum CarEvents e)
{
    HsmOnEvent((Hsm*)&car, (Msg const *) &e);
}
