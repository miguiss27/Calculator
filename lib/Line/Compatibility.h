#ifndef Line_Compatibity
#define Line_Compatibity


#include <Arduino.h>

#define IOMode pinMode
#define IORead digitalRead
#define IOWrite digitalWrite
#define TimeWait delay
#define IOInteruptOn attachInterrupt
#define IOInteruptOff detachInterrupt


#endif //Line_Compatibity