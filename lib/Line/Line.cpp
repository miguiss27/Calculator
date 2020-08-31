#include "Line.h"

Line::Line(int *pinout, int *pinin, int transmisionmode, int bps, int addr){

	changeSetings(pinout,pinin,transmisionmode,bps,addr);
}

Line::~Line(){

	end();
}


//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//


void Line::changeSetings(int *pinout, int *pinin, int transmisionmode, int bps, int addr){

	if (pinout != nullptr) _pinOut = pinout;
	if (pinin != nullptr) _pinIn = pinin;

	int oldTransmisionMode = _transmisionMode;
	int oldAddr = _addr;

	_transmisionMode = ((0 <= transmisionmode) and (transmisionmode < Transmision_Modes))? transmisionmode : oldTransmisionMode;
	_bps = ((0 < bps) and (bps < 1000))? bps : Buffer_Size;
	_addr = ((0 < addr) and (addr < Max_Address_Number))? addr : oldAddr;

	begin();

}


//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//


void Line::begin(){

	switch (_transmisionMode){

	case 0:
		
		for (int i = 0; i <= (sizeof(_pinOut) / sizeof(_pinOut[0])); i++){

			IOMode(_pinOut[i], OUTPUT);
		}

		for (int i = 0; i <= (sizeof(_pinIn) / sizeof(_pinIn[0])); i++){

			IOMode(_pinIn[i], INPUT);
		}		

		break;

	case 2:

		if(_addr == 0){

			IOMode(_pinOut[2], OUTPUT);
		}

		else{

			IOMode(_pinIn[2], INPUT);
		}

	case 1:

		if(_addr == 0){

			IOMode(_pinOut[1], OUTPUT);
			IOMode(_pinOut[0], OUTPUT);
		}
		else{

			IOMode(_pinIn[1], INPUT);
			IOMode(_pinIn[0], INPUT);
		}

		break;

	case 4:

		if(_addr == 0){

			IOMode(_pinOut[3], OUTPUT);
		}

		else{

			IOMode(_pinIn[3], INPUT);
		}

	case 3:

		if(_addr == 0){

			IOMode(_pinOut[2], INPUT);
			IOMode(_pinOut[1], OUTPUT);
			IOMode(_pinOut[0], OUTPUT);
		}
		else{

			IOMode(_pinIn[2], OUTPUT);
			IOMode(_pinIn[1], INPUT);
			IOMode(_pinIn[0], INPUT);
		}

		break;
	}


}

void Line::end(){

	for (int i = 0; i < Max_Conected_Devices; i++){

		disconect(_conectedAddr[i]);
	}
}


//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//


bool Line::conect(int device){

	int returnCode;

	int index = 0;

	bool repeated = false;
	

	for (int i = 0; i < Max_Conected_Devices; i++){

		if (_conectedAddr[i] = device) repeated = true;
	}

	for (int i = 0; i < Max_Conected_Devices; i++){

		if (_conectedAddr[i] == 0){

			index = i;
			break;
		}

	}

	if (!repeated) _conectedAddr[index] = device;

	returnCode = sendLineCode(Line_Code_Conect,device);			
	

	if (returnCode == Line_Code_Conect) return true;

	else return false;
}

bool Line::disconect(int device){

	int returnCode;
		
	returnCode = sendLineCode(Line_Code_Disconect,device);


	if (returnCode == Line_Code_Disconect) return true;

	else return false;
}


//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//


bool Line::isConected(int device){

	for (int i; i < Max_Conected_Devices; i++){

		if (sendLineCode(Line_Code_Conect,_conectedAddr[i]) == Line_Code_Conect){

			return true;
		}			
	}

	return false;
	
}

void Line::waitConection(int device){

	while (!isConected(device)){

		sendLineCode(Line_Code_Conect,device);
	}
	
}


//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//


bool Line::write(bool * data, int device){

	int bitdelay = (1000 / _bps);
	
	int datasize = (sizeof(data) / sizeof(data[0]));

	switch (_transmisionMode){

	case 0:

		for (int i = 0; i <= datasize; i++){

			IOWrite(_pinOut[i], data[i]);
		}

		TimeWait(bitdelay);

		for (int i = 0; i <= datasize; i++){
        
			IOWrite(_pinOut[i], LOW);
		}

		break;

	case 1:

		for (int i = 0; i <= datasize; i++){

			IOWrite(_pinOut[1], data[i]);
			TimeWait(bitdelay / 2);

			IOWrite(_pinOut[0], HIGH);
			TimeWait(bitdelay / 2);
            
			IOWrite(_pinOut[0], LOW);

		}

		IOWrite(_pinOut[1], LOW);

		break;

	
	case 2:

		sendLineCode(Line_Code_Select,device);

		for (int i = 0; i <= datasize; i++){

			IOWrite(_pinOut[1], data[i]);
			TimeWait(bitdelay / 2);

			IOWrite(_pinOut[0], HIGH);
			TimeWait(bitdelay / 2);
            
			IOWrite(_pinOut[0], LOW);

		}

		IOWrite(_pinOut[1], LOW);

		sendLineCode(Line_Code_Unselect,device);	
		
		break;
	

	case 3:

		break;

	case 4:

		break;
	}
}

bool * Line::read(int bits){

	bool buffer[bits];

	switch (_transmisionMode){

	case 0:

		for (int i = 0; i < bits; i++){

			buffer[i] = IORead(_pinIn[i]);
		}

		break;

	}

	return buffer;
}

bool * Line::transfer(bool * data, int device){

	int datasize = (sizeof(data) / sizeof(data[0]));

	switch (_transmisionMode){
	
	case 0:

		write(data, device);

		return read(datasize);

		break;

	case 2:

		break;

	case 3:
		
		break;

	case 4:
		
		break;
	
	default:

		break;
	}
}


//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//


void Line::Request(int device){

	sendLineCode(Line_Code_Request,device);
}

bool * Line::Request(int device, bool * data){

	switch (_transmisionMode){

	case 0:

		return transfer(data, device);

		break;

	case 2:
	case 4:

		sendLineCode(Line_Code_Data_Request,device);
		
		return transfer(data,device);

		break;
	
	default:

		return nullptr;

		break;
	}
}


//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//


void Line::onRequest(void (*callback)(bool *)){

	
}

void Line::onReceive(void (*callback)(bool *)){

	
}


//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//


int sendLineCode(int code, int addr){

	int responseCode = Line_Code_Null;

	bool isMaster = (addr == Line_Master_Address);
	bool isSlave = ((Line_Min_Slave_Address <= addr) and (addr <= Line_Min_Slave_Address));

	bool addrValid = ((isMaster) or (isSlave));
	bool supportCodes = ((_transmisionMode == Line_I2C_Extra_Mode) or (_transmisionMode == Line_Spi_Extra_Mode));

	int bitdelay = 1000 / Line_Code_Time;



	if ((addrValid) and (supportCodes)){

		for (int i = 0; i <= Line_Code_Buffer_Size; i++){

			IOWrite(_pinOut[1], data[i]);
			TimeWait(bitdelay / 2);

			IOWrite(_pinOut[0], HIGH);
			TimeWait(bitdelay / 2);
            
			IOWrite(_pinOut[0], LOW);

		}

		IOWrite(_pinOut[1], LOW);
	}

	else if ((addr == _addr)){

		responseCode = Line_Code_Conect;
	}

	else{

		responseCode = Line_Code_Null;
	}

	return responseCode;
}

void readInterupt(){


}
