#include <Keypad.h>
//#include "Wire.h"

#define Px A0
#define Py A1
#define Pb 13

#define PO0
#define PO1
#define PO2
#define PO3
#define PO4 


#define timewait 200

#define debug true

//#define address
 
const byte rowsCount = 4;
const byte columsCount = 4;
 
char keys[rowsCount][columsCount] = {
   { '1','2','3', 'A' },
   { '4','5','6', 'B' },
   { '7','8','9', 'C' },
   { '#','0','*', 'D' }
};
 
const byte rowPins[rowsCount] = { 2, 3, 4, 5 };
const byte columnPins[columsCount] = { 6, 7, 8, 9 };
 
Keypad keypad = Keypad(makeKeymap(keys), rowPins, columnPins, rowsCount, columsCount);





  int GetValue(char axis){

    int val;
    
    switch (axis) {
      case 'x':
        if (analogRead(Px) < 512) {
          val = 2;
        }
        if (analogRead(Px) > 512) {
          val = 0;
        }
        if (analogRead(Px) > 341 && analogRead(Px) < 682) {
          val = 1;
        }
        break;

        
      case 'y':
        if (analogRead(Py) < 512) {
          val = 2;
        }
        if (analogRead(Py) > 512) {
          val = 0;
        }
        if (analogRead(Py) > 341 && analogRead(Py) < 682) {
          val = 1;
        }
        break;


      case 'b':
        val = digitalRead(Pb);
        break;
        
    }

    return val;
    
  }





void setup() {
   if (debug){Serial.begin(9600);}
   
   pinMode(Px, INPUT);
   pinMode(Py, INPUT);
   pinMode(Pb, INPUT);

   //Wire.begin(address);
   
}





 
void loop() {
   char key = keypad.getKey();
   int X = GetValue('x');
   int Y = GetValue('y');
   int B = GetValue('b');

   //Wire.write(X);Wire.write(Y);Wire.write(B);
   
   if (debug){
    Serial.print(key);
    Serial.print("-#-");
    Serial.print(X);
    Serial.print("-#-");
    Serial.print(Y);
    Serial.print("-#-");
    Serial.println(B);
    }

   delay(timewait);
   
}
