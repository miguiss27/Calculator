//include keypad library for using the keypad
#include <Keypad.h>

//include math library to use pow function
#include <math.h>

//define the pins to use with the joystick
#define Px A0
#define Py A1
#define Pb 13

//declare the bits to be sent per second
#define bitssec 2

//declare the lengt of the data to be sent
#define datalenght 9

//initialice an array of booleans of size datalenght
int dataout[datalenght];

//calcule all posibilities that could hold the dictionary (the first 4 bits of information are for the joystick)
const int dictlenght = pow(2, (datalenght - 4));

//define an array to use as a dictionary for converting keypresses to data (first one indicates no key pressed)
int Kdict[dictlenght][datalenght-4] = {
    {0,0,0,0,0},
    {1,0,0,0,0},
    {0,1,0,0,0},
    {1,1,0,0,0},
    {0,0,1,0,0},
    {1,0,1,0,0},
    {0,1,1,0,0},
    {1,1,1,0,0},
    {0,0,0,1,0},
    {1,0,0,1,0},
    {0,1,0,1,0},
    {1,0,1,1,0},
    {0,0,1,1,0},
    {1,1,0,1,0},
    {0,1,1,1,0},
    {1,1,1,1,0},
    {0,0,0,0,1}
   };

//declare the number of wires needed for conection
#define maxchanels 2

//define the mode for transmission('i'="I2C", 'p'="PARALEL")
#define mode 'i'

//define the pins to use for comunication (in I2C mode, first is data and second is clock)
const int PO[maxchanels] = {56, 57};

//define the time to wait between reading
#define timewait 200

//specify if debugging is enabled
#define debug true

//declare the sice of the keypad
const byte rowsCount = 4;
const byte columsCount = 4;

//declare the keys in the keypad
char keys[rowsCount][columsCount] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'#', '0', '*', 'D'}};

//declare the pins that the keypad will be using
const byte rowPins[rowsCount] = {2, 3, 4, 5};
const byte columnPins[columsCount] = {6, 7, 8, 9};

//initialice the keypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, columnPins, rowsCount, columsCount);

//define a function to return the axis or button state
int GetValue(char axis)
{

   int val;

   switch (axis)
   {
   case 'x':
      if (analogRead(Px) < 512)
      {
         val = 1;
      }
      if (analogRead(Px) > 512)
      {
         val = -1;
      }
      if (analogRead(Px) > 341 && analogRead(Px) < 682)
      {
         val = 0;
      }
      break;

   case 'y':
      if (analogRead(Py) < 512)
      {
         val = 1;
      }
      if (analogRead(Py) > 512)
      {
         val = -1;
      }
      if (analogRead(Py) > 341 && analogRead(Py) < 682)
      {
         val = 0;
      }
      break;

   case 'b':
      val = digitalRead(Pb);
      break;
   }

   return val;
}

//define a function to transfer data throught the wires
void writeWire(int *data, int datalngt, char transmissionMode, int bps)
{
   int bitdelay = (1000 / bps);
   switch (transmissionMode)
   {

   case 'i':

      for (int i = 0; i < datalngt; i++)
      {
         digitalWrite(PO[0], data[i]);
         delay(bitdelay / 2);
         digitalWrite(PO[1], HIGH);
         delay(bitdelay / 2);
         digitalWrite(PO[1], LOW);
      }
      digitalWrite(PO[0], LOW);

      break;

   case 'p':

      for (int i = 0; i < datalngt; i++)
      {
         digitalWrite(PO[i], data[i]);
      }

      delay(bitdelay);

      for (int i = 0; i < datalngt; i++)
      {
         digitalWrite(PO[i], LOW);
      }

      break;
   }
}

//define a function to convert variables to data that could be transfered throught the wires
void convertdata(int axisX, int axisY, int Button, char key, int *data, int keydict[][datalenght-4], int maxdataLenght)
{
   if ((axisX != 0) and (axisY != 0))
   {
      data[0] = 0;
      switch (axisX)
      {

      case 1:
         data[1] = 1;
         break;

      case -1:
         data[1] = 0;
         break;
      }

      switch (axisY)
      {

      case 1:
         data[2] = 1;
         break;

      case -1:
         data[2] = 0;
         break;
      }
   }
   else
   {
      data[0] = 1;
   };

   data[3]=Button;

   int indexdict;

   if (key !='\0')
   {

      for (int r = 0; r < rowsCount; r++)
      {
         for (int c = 0; c < columsCount; c++)
         {
            if (keys[r][c] == key)
            {
               indexdict = r * c;
            }
         }
      }
   }
   else
   {
      indexdict = 0;
   };

   for (int i=4; i< maxdataLenght; i++)
   {
      int dictpart= keydict[indexdict][i-4];
      data[i]=dictpart;

   }
}

void setup()
{
   //enable serial comunication if in debbugging mode
   if (debug)
   {
      Serial.begin(9600);
   }

   //initialice the joystick pins
   pinMode(Px, INPUT);
   pinMode(Py, INPUT);
   pinMode(Pb, INPUT);

   //initialice the output wires
   for (int i = 0; i < maxchanels; i++)
   {
      pinMode(PO[i], OUTPUT);
   }

   //initialice the data variable to false
   for (int i = 0; i < datalenght; i++)
   {
      dataout[i] = 0;
   }
}

void loop()
{

   //store the actual state of the joystick and the keyboard

   char Key = keypad.getKey();
   int X = GetValue('x');
   int Y = GetValue('y');
   int B = GetValue('b');

   //convert variables to a data format to be sent
   convertdata(X, Y, B, Key, dataout, Kdict, datalenght);

   //output the state of the joystick, the keyboard and the data to send throught serial port if in debbuging mode
   if (debug)
   {
      for (int i = 0; i< datalenght; i++){
      Serial.print(dataout[i]);
      Serial.print("-");
      }

      Serial.print("/*\\");
      Serial.print(Key);
      Serial.print("-#-");
      Serial.print(X);
      Serial.print("-#-");
      Serial.print(Y);
      Serial.print("-#-");
      Serial.print(B);
      


   }


   //transfer the data throught the wires
   writeWire(dataout, datalenght, mode, bitssec);

   //reset the data variable to 0
   for (int i = 0; i < datalenght; i++)
   {
      dataout[i] = 0;
   }

   //wait some time to read again
   delay(timewait);
}