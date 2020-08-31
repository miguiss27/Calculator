
#define Key_Ref_Data_Lenght 4
#define Key_Dict_Data_Lenght 5
#define Joy_Dict_Data_Lenght 4

#define Joy_x A0
#define Joy_y A1
#define Joy_button 13

#define DEBUG true

#include <Data.h>
#include <Utils.h>
#include <Keypad.h>


const int Data_Lenght = 9;
const int Joy_Data_Lenght = 4;
const int Key_Data_Lenght = Data_Lenght-Joy_Data_Lenght;

const int Joy_Dict_Lenght = 10;
const int Key_Dict_Lenght = 17;

const byte Keypad_Rows = 4;
const byte Keypad_Colums = 4;
const byte Keypad_Rows_Pins[Keypad_Rows] = {2, 3, 4, 5};
const byte Keypad_Colums_Pins[Keypad_Colums] = {6, 7, 8, 9};


int Wire_Out[9] = {1,2,3,4,5,6,7,8,9};
int Wire_In[9] = {1,2,3,4,5,6,7,8,9};

int Wire_Mode = 0;
int Wire_bps = 9;

int Time_Wait = 200;


char Var_Key;
bool Var_Center;
bool Var_Button;
int Var_X_Axis;
int Var_Y_Axis;


bool Joy_Dict[Joy_Dict_Lenght][Joy_Data_Lenght] = {
	{0, 0, 0, 0},
	{1, 0, 0, 0},
	{0, 1, 0, 0},
	{1, 1, 0, 0},
	{0, 0, 1, 0},
	{1, 0, 1, 0},
	{0, 1, 1, 0},
	{1, 1, 1, 0},
	{0, 0, 0, 1},
	{1, 0, 0, 1}
};

bool Key_Dict[Key_Dict_Lenght][Key_Data_Lenght] = {
	{0, 0, 0, 0, 0},
	{1, 0, 0, 0, 0},
	{0, 1, 0, 0, 0},
	{1, 1, 0, 0, 0},
	{0, 0, 1, 0, 0},
	{1, 0, 1, 0, 0},
	{0, 1, 1, 0, 0},
	{1, 1, 1, 0, 0},
	{0, 0, 0, 1, 0},
	{1, 0, 0, 1, 0},
	{0, 1, 0, 1, 0},
	{1, 0, 1, 1, 0},
	{0, 0, 1, 1, 0},
	{1, 1, 0, 1, 0},
	{0, 1, 1, 1, 0},
	{1, 1, 1, 1, 0},
	{0, 0, 0, 0, 1}
};

char Keypad_Keys[Keypad_Rows][Keypad_Colums] = {
	{'1', '2', '3', 'A'},
	{'4', '5', '6', 'B'},
	{'7', '8', '9', 'C'},
	{'#', '0', '*', 'D'}
};


Key_T key;
Joy_T joy;

Data data = Data(Data_Lenght);

Wire wire = Wire(Wire_Out,Wire_In,Wire_Mode,Wire_bps);

Keypad keypad = Keypad(makeKeymap(Keypad_Keys), Keypad_Rows_Pins, Keypad_Colums_Pins, Keypad_Rows, Keypad_Colums);



void setup(){

	DebugInit(9600);

	joy.init(Joy_Dict_Lenght,Joy_Data_Lenght);
	key.init(Key_Dict_Lenght,Key_Data_Lenght,Keypad_Rows,Keypad_Colums);

	joy.makejoy(Joy_Dict);
	key.makekey(Key_Dict,Keypad_Keys);

	data.updatereferences(joy,key);

	InfoInit();

}

void loop(){

	char Var_Key = keypad.getKey();
	bool Var_Center = Info('c');
	bool Var_Button = Info('b');
	int Var_X_Axis = Info('x');
	int Var_Y_Axis = Info('y');

	Debug();

	data.converttodata(Var_Center,Var_X_Axis,Var_Y_Axis,Var_Button,Var_Key);

	wire.send(data.getdata(),data.getdatasize());

	data.resetdata();


	delay(Time_Wait);
}
