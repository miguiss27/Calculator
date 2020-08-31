#include <Keypad.h>
#include <math.h>

#define Px A0
#define Py A1
#define Pb 13

const int maxwires = 2;
const int PO[maxwires] = {56, 57};
char mode = 'i';
int submode = 0;
int timewait = 200;

int bitssec = 2;
const int datalenght = 10;
bool dataout[datalenght];

const int lngstickddata = 4;
const int Jdictlenght = 1 + pow(2, lngstickddata);
bool Jdict[Jdictlenght][lngstickddata] = {
	{0, 0, 0, 0},
	{1, 0, 0, 0},
	{0, 1, 0, 0},
	{1, 1, 0, 0},
	{0, 0, 1, 0},
	{1, 0, 1, 0},
	{0, 1, 1, 0},
	{1, 1, 1, 0},
	{0, 0, 0, 1},
	{1, 0, 0, 1},
};

const int lngkeydata = datalenght - 4;
const int Kdictlenght = 1 + pow(2, lngkeydata);
bool Kdict[Kdictlenght][lngkeydata] = {
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
	{0, 0, 0, 0, 1}};

#define debug true

const byte rowsCount = 4;
const byte columsCount = 4;

char keys[rowsCount][columsCount] = {
	{'1', '2', '3', 'A'},
	{'4', '5', '6', 'B'},
	{'7', '8', '9', 'C'},
	{'#', '0', '*', 'D'}};

const byte rowPins[rowsCount] = {2, 3, 4, 5};
const byte columnPins[columsCount] = {6, 7, 8, 9};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, columnPins, rowsCount, columsCount);

//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//

//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//

void setup()
{

	if (debug)
		Serial.begin(9600);

	pinMode(Px, INPUT);
	pinMode(Py, INPUT);
	pinMode(Pb, INPUT);

	for (int i = 0; i < maxwires; i++)
	{
		pinMode(PO[i], OUTPUT);
	}

	resetdata(dataout, datalenght);
}

//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//

void loop()
{

	char K = keypad.getKey();
	bool C = getstickinfo('c'); //center
	bool B = getstickinfo('b'); //button
	int X = getstickinfo('x');	//axisx
	int Y = getstickinfo('y');	//axisy

	converttodata(C, X, Y, Jdict, B, K, keys, rowsCount, columsCount, Kdict, datalenght, dataout);

	debugprint(debug, dataout, K, X, Y, B);

	writeWire(PO, dataout, datalenght, mode, submode, bitssec);

	resetdata(dataout, datalenght);

	delay(timewait);
}

//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//

//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//

void writeWire(int pinout[], bool *data, int datalngt, char transmissionMode, int subMode, int bps)
{
	int bitdelay = (1000 / bps);

	switch (transmissionMode)
	{

	case 'i':
		switch (submode)
		{
		case 0:
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

		case 1:

			digitalWrite(PO[2], HIGH);
			delay(bitdelay / 4);

			for (int i = 0; i < datalngt; i++)
			{
				digitalWrite(PO[0], data[i]);
				delay(bitdelay / 4);
				digitalWrite(PO[1], HIGH);
				delay(bitdelay / 4);
				digitalWrite(PO[1], LOW);
			}
			digitalWrite(PO[0], LOW);
			digitalWrite(PO[2], LOW);
			delay(bitdelay / 4);

			break;
		}
		break;

	case 'p':

		for (int i = 0; i < datalngt; i++)
		{
			digitalWrite(pinout[i], data[i]);
		}

		delay(bitdelay);

		for (int i = 0; i < datalngt; i++)
		{
			digitalWrite(pinout[i], LOW);
		}

		break;
	}
}

//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//

void converttodata(bool Center, int AxisX, int AxisY, bool JoyDict[Jdictlenght][lngstickddata], bool Button, char Key, char KeyRef[rowsCount][columsCount], int RefRow, int RefCol, bool KeyDict[Kdictlenght][lngkeydata], int MaxDataLng, bool *data)
{
	int Jdictkey;
	if (!Center)
	{
		if ((AxisX == -1) && (AxisY == -1))
		{
			Jdictkey = 1;
		}
		else if ((AxisX == -1) && (AxisY == 0))
		{
			Jdictkey = 2;
		}
		else if ((AxisX == -1) && (AxisY == 1))
		{
			Jdictkey = 3;
		}
		else if ((AxisX == 0) && (AxisY == 1))
		{
			Jdictkey = 4;
		}
		else if ((AxisX == 1) && (AxisY == 1))
		{
			Jdictkey = 5;
		}
		else if ((AxisX == 1) && (AxisY == 0))
		{
			Jdictkey = 6;
		}
		else if ((AxisX == 1) && (AxisY == -1))
		{
			Jdictkey = 7;
		}
		else if ((AxisX == 0) && (AxisY == -1))
		{
			Jdictkey = 8;
		}
	}
	else
	{
		Jdictkey = 0;

		if (Button)
		{
			Jdictkey = 9;
		}
	}

	for (int i = 0; i < (MaxDataLng - 4); i++)
	{
		int Kdictpart = KeyDict[Jdictkey][i];
		data[i] = Kdictpart;
	}

	int indexdict;
	if (Key != '\0')
	{

		for (int r = 0; r < RefRow; r++)
		{

			for (int c = 0; c < RefCol; c++)
			{
				if (KeyRef[r][c] == Key)
				{
					indexdict = 1 + r * RefCol + c;
				}
			}
		}
	}
	else
	{
		indexdict = 0;
	};

	for (int i = 4; i < MaxDataLng; i++)
	{
		int Kdictpart = KeyDict[indexdict][i - 4];
		data[i] = Kdictpart;
	}
}

//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//

int getstickinfo(char Type)
{
	int returnvar;
	switch (Type)
	{
	case 'c':
		if ((analogRead(Px) > 341 && analogRead(Px) < 682) && (analogRead(Py) > 341 && analogRead(Py) < 682))
		{
			returnvar = 1;
		}
		else
		{
			returnvar = 0;
		};
		break;
	case 'b':
		returnvar = digitalRead(Pb);
		break;
	case 'x':
		if (analogRead(Px) < 512)
		{
			returnvar = -1;
		}
		if (analogRead(Px) > 512)
		{
			returnvar = 1;
		}
		if (analogRead(Px) > 341 && analogRead(Px) < 682)
		{
			returnvar = 0;
		}
		break;

	case 'y':
		if (analogRead(Py) < 512)
		{
			returnvar = -1;
		}
		if (analogRead(Py) > 512)
		{
			returnvar = 1;
		}
		if (analogRead(Py) > 341 && analogRead(Py) < 682)
		{
			returnvar = 0;
		}
		break;
	}
	return returnvar;
}

//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//

void resetdata(bool *data, int lngdata)
{
	for (int i = 0; i < lngdata; i++)
	{
		data[i] = 0;
	}
}

//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//

void debugprint(bool DebugExecute, bool *dataprint, char Key, int X, int Y, bool B)
{
	if (DebugExecute)
	{
		for (int i = 0; i < datalenght; i++)
		{
			Serial.print(dataprint[i]);
			Serial.print("-");
		}

		Serial.print("/*\\");
		Serial.print(Key);
		Serial.print("-#-");
		Serial.print(X);
		Serial.print("-#-");
		Serial.print(Y);
		Serial.print("-#-");
		Serial.println(B);
	}
}