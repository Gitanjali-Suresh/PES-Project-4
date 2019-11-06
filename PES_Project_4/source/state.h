#ifndef _STATE_H_

#include <stdint.h>

typedef enum
{
    Temp_Read,
    Temp_Alert,
    Temp_Avg_Wait,
    Disconnected,
    State_Change,
    End_State
}eTempState;

typedef enum
{
    Start_Event,
    Timeout_1,
    Timeout_2,
    Timeout_3,
    Timeout_4,
    Complete,
    Alert,
    Disconnect,
    End_Event
}eTempEvent;

typedef eTempState (*Event_Handler)(void);

typedef struct
{
    eTempState state;
    eTempEvent event;
    Event_Handler handler;
}sState_Machine_2;

extern int32_t temp, temp_avg, temp_alert;
extern int16_t count, flag, structvar, terminate_flag;
extern eTempState Next_State;
extern eTempEvent New_Event;
extern int32_t systick_flag;
extern int16_t interrupt_flag;

void delay(int milliseconds);
extern eTempState input_Temp(void);
extern eTempState alert(void);
extern eTempState avg_Temp(void);
extern eTempState error_Temp(void);
extern eTempState change_State(void);

extern sState_Machine_2 SM[10];

#endif
