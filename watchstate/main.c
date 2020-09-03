#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "car.h"
#include "hsm.h"

static int g_mode = 0;

void set_mode(int mode)
{
    g_mode = mode;
}

int get_mode(void)
{
    return g_mode;
}
int main()
{
    Car car;
    evt_placeholder_t hsmEvt;
    evt_chmode_t *p;
    int rand_value = 0;
    int flag = 0;
    CarCtor(&car);
    HsmOnStart((Hsm*)&car);

    rand_value = 0;
    while(1)
    {
        switch(rand_value)
        {
            case 0:
                EVT_SETTYPE(&hsmEvt, Car_CHMODE_EVT);//0
                p = EVT_CAST(&hsmEvt, evt_chmode_t);
                p->mode = 3;
                rand_value = 1;
                break;
            case 1:
                EVT_SETTYPE(&hsmEvt, Car_RFID_EVT);//0
                rand_value = 3;
                break;
            case 2:
                //EVT_SETTYPE(&hsmEvt, Car_RFID_EVT);//0
                rand_value = 3;
                break;
            case 3:
                EVT_SETTYPE(&hsmEvt, Car_TICK_EVT);//0
                set_mode(2);
                rand_value = 4;
                break;
            case 4:
                EVT_SETTYPE(&hsmEvt, Car_TICK_EVT);//0
                set_mode(3);
                rand_value = 5;
                break;
            case 5:
                EVT_SETTYPE(&hsmEvt, Car_TICK_EVT);//0
                set_mode(1);
                rand_value = 6;
                break;
            case 6:
                EVT_SETTYPE(&hsmEvt, Car_TICK_EVT);//0
                set_mode(3);
                rand_value = 7;
                break;
            case 7:
                EVT_SETTYPE(&hsmEvt, Car_TICK_EVT);//0
                set_mode(2);
                rand_value = 8;
                break;
            case 8:
                EVT_SETTYPE(&hsmEvt, Car_TICK_EVT);//0
                set_mode(1);
                rand_value = 9;
                break;
            case 9:
                EVT_SETTYPE(&hsmEvt, Car_CHMODE_EVT);//0
                p = EVT_CAST(&hsmEvt, evt_chmode_t);
                p->mode = 2;
                rand_value = 10;
                break;
            case 10:
                EVT_SETTYPE(&hsmEvt, Car_CHMODE_EVT);//0
                p = EVT_CAST(&hsmEvt, evt_chmode_t);
                p->mode = 1;
                rand_value = 11;
                break;
            case 11:
                EVT_SETTYPE(&hsmEvt, Car_CHMODE_EVT);//0
                p = EVT_CAST(&hsmEvt, evt_chmode_t);
                p->mode = 4;
                rand_value = 12;
                break;
            default:
                flag = 1;
                break;
        }


        if(flag)
            break;
        //Car_Event(hsmEvt);
        HsmOnEvent((Hsm*)&car, (Msg*)&hsmEvt);
    }


    return 0;
}
