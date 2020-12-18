#include <Arduino.h>

#ifndef Joy_x
#define Joy_x A0
#endif

#ifndef Joy_y
#define Joy_y A1
#endif

#ifndef Joy_button
#define Joy_button 13
#endif

#ifndef DEBUG
#define DEBUG false
#endif

#ifndef Utils_h
#define Utils_h


    bool InfoInit();
    int Info(char type);
    
#endif