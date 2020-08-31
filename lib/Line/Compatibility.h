#ifndef Line_Compat
#define Line_Compat


#include <Arduino.h>

#define IOMode pinMode
#define IORead digitalRead
#define IOWrite digitalWrite
#define TimeWait delay
#define IOInteruptOn attachInterrupt
#define IOInteruptOff detachInterrupt


#endif //Line_Compat