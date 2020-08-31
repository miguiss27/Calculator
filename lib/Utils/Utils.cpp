#include "Utils.h"

bool InfoInit(){

	pinMode(Joy_x,INPUT);
	pinMode(Joy_y,INPUT);
	pinMode(Joy_button,INPUT);
}

int Info(char type){

    int returnvar;

	switch (type){
    
	case 'c':

		if ((analogRead(Joy_x) > 341 && analogRead(Joy_x) < 682) && (analogRead(Joy_y) > 341 && analogRead(Joy_y) < 682)){

			returnvar = 1;
		}
		else{

			returnvar = 0;
		};
		break;

	case 'b':

		returnvar = digitalRead(Joy_button);
		break;

	case 'x':

		if (analogRead(Joy_x) < 512){

			returnvar = -1;
		}
		if (analogRead(Joy_x) > 512){

			returnvar = 1;
		}
		if (analogRead(Joy_x) > 341 && analogRead(Joy_x) < 682){

			returnvar = 0;
		}
		break;

	case 'y':

		if (analogRead(Joy_y) < 512){

			returnvar = -1;
		}
		if (analogRead(Joy_y) > 512){

			returnvar = 1;
		}
		if (analogRead(Joy_y) > 341 && analogRead(Joy_y) < 682){

			returnvar = 0;
		}
		break;
	}

	return returnvar;
}


void DebugInit(int baudrate){

	Serial.begin(baudrate);
}

void Debug(){

	Serial.print("/*\\");Serial.print(Var_Key);
	Serial.print("-#-"); Serial.print(Var_Center);
	Serial.print("-#-"); Serial.print(Var_X_Axis);
	Serial.print("-#-"); Serial.print(Var_Y_Axis);
	Serial.print("-#-"); Serial.println(Var_Button);
}