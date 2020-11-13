#include "Data.h"



Data::Data(int data_size, Joy_T joy_store, Key_T key_store){

    Dat_store.container= new bool[data_size];
	Dat_store.size = data_size;

	updatereferences(joy_store,key_store);

	resetdata();
}

Data::Data(int data_size){

    Dat_store.container= new bool[data_size];
	Dat_store.size = data_size;

	resetdata();
}

Data::~Data(){

    delete [] Dat_store.container;
}



void Data::converttodata(bool center, int axis_x, int axis_y, bool button, char key){

	int Joy_dict_key;
	int Key_dict_key;


	if (!center)
	{
		if ((axis_x == -1) && (axis_y == -1)){

			Joy_dict_key = 1;
		}
		else if ((axis_x == -1) && (axis_y == 0)){

			Joy_dict_key = 2;
		}
		else if ((axis_x == -1) && (axis_y == 1)){

			Joy_dict_key = 3;
		}
		else if ((axis_x == 0) && (axis_y == 1)){

			Joy_dict_key = 4;
		}
		else if ((axis_x == 1) && (axis_y == 1)){

			Joy_dict_key = 5;
		}
		else if ((axis_x == 1) && (axis_y == 0)){

			Joy_dict_key = 6;
		}
		else if ((axis_x == 1) && (axis_y == -1)){

			Joy_dict_key = 7;
		}
		else if ((axis_x == 0) && (axis_y == -1)){

			Joy_dict_key = 8;
		}
	}
	else{

		Joy_dict_key = 0;

		if (button){

			Joy_dict_key = 9;
		}
	}



	for (int i= 0; i < (Key_store.ref_rows * Key_store.ref_columns); i++){

		if (Key_store.ref[i] == key){
			
			Key_dict_key = 1 + i;
			break;
		}
		else{

			Key_dict_key = 0;
		}
		
	}


	for (int i = 0; i < (Dat_store.size - 4); i++){

		int Jdictpart = Joy_store.dict[Joy_dict_key * Joy_store.columns + i];
		Dat_store.container[i] = Jdictpart;
	}

	for (int i = 4; i < Dat_store.size; i++){

		int Kdictpart = Key_store.dict[Key_dict_key * Key_store.dict_columns + (i - 4)];
		Dat_store.container[i] = Kdictpart;
	}

}

void Data::converttoval(bool *center, int *axis_x, int *axis_y, bool *button, char *key){

    *center = 0;
    *axis_x = 0;
    *axis_y = 0;
    *button = 0;
    *key = '\0';
    
    bool joydata [4], keydata[Dat_store.size-4];

    for (int i = 0; i < Dat_store.size; i++){

		if (i < Joy_Dict_Data_Lenght){
		
		joydata[i] = Dat_store.container[i];

		}
		else{		
		
		keydata[i] = Dat_store.container[i];
		
		}	
		
	}


    int joy_index;
	int key_index;

	bool joy_tmp[Joy_store.columns];
	bool key_tmp[Key_store.dict_columns];

	int joy_matches = 0;
	int key_matches = 0;

	for (int r = 0; r < Joy_store.rows; r++){

		for (int c = 0; c < Joy_store.columns; c++){

			joy_tmp[c] = Joy_store.dict[r * Joy_store.columns + c];

			if (joy_tmp[c] == joydata[c]){

				joy_matches++;
			}
		}

		if (joy_matches == Joy_store.columns){

			joy_index = r;
		}
		
		joy_matches = 0;
	}


	for (int r = 0; r < Key_store.dict_rows; r++){

		for (int c = 0; c < Key_store.dict_columns; c++){

			key_tmp[c] = Key_store.dict[r * Key_store.dict_columns + c];

			if (key_tmp[c] == keydata[c]){

				key_matches++;
			}
		}

		if (key_matches == Key_store.dict_columns){

			key_index = r;
		}
		
		key_matches = 0;
	}


    

    switch (joy_index)
    {
    case 0:
        *center = 1;
        break;
    
    case 1:
        *axis_x = -1; *axis_y = -1;
        break;
    
    case 2:
        *axis_x = -1; *axis_y = 0;
        break;

    case 3:
        *axis_x = -1; *axis_y = 1;
        break;

    case 4:
        *axis_x = 0; *axis_y = 1;
        break;

    case 5:
        *axis_x = 1; *axis_y = 1;
        break;

    case 6:
        *axis_x = 1; *axis_y = 0;
        break;

    case 7:
        *axis_x = 1; *axis_y = -1;
        break;

    case 8:
        *axis_x = 0; *axis_y = -1;
        break;
    
    case 9:
        *button = 1;
		*center = 1;
        break;
	default:
		*center = 1;
		break;
    }


    if (key_index != 0){

    	*key = Key_store.ref[key_index - 1];
	}

}



void Data::resetdata(){

	for (int i = 0; i < Dat_store.size; i++){
		Dat_store.container[i] = 0;
	}

}



bool* Data::getdata(){

	return Dat_store.container;

}

int Data::getdatasize(){

    return Dat_store.size;
    
}



void Data::newdata(bool *data_in, int data_size){

    updatedatasize(data_size);
	resetdata();
    updatedata(data_in);

}



void Data::updatedata(bool *data_in){

    for (int i = 0; i <= Dat_store.size; i++){
		Dat_store.container[i] = data_in[i];
	}

}

void Data::updatedatasize(int data_size){

    Dat_store.size = data_size;
	delete [] Dat_store.container;
    Dat_store.container = new bool[data_size];
    
}


void Data::updatereferences(Joy_T New_Joy, Key_T New_Key){

    Joy_store = New_Joy;

    Key_store = New_Key;

}
