#pragma once

#ifndef __cplusplus
extern "C"
{
#endif
    
    void led_init();
    void led_on();
    void led_off();
    void delay(int msec);
    
#ifndef __cplusplus
}
#endif
