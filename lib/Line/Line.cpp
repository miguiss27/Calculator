#include "Line.h"

Line::Line(int *pinout, int *pinin, int transmisionmode, int bps, int addr){

	for (int i; i < Line_Max_Conected_Devices; i++){

		_conectedAddr[i] = Line_Null_Address;
	}

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

	_transmisionMode = ((Line_Min_Transmision_Mode <= transmisionmode) and (transmisionmode < Line_Max_Transmision_Mode)) ? transmisionmode : Line_Transmision_Mode;

	_bps = ((0 < bps) and (bps < Line_Second)) ? bps : Line_BPS;
	
	_addr = ((addr == Line_Master_Address) or ((Line_Min_Slave_Address <= addr) and (addr <= Line_Max_Slave_Address))) ? addr : Line_Address;

	begin();

}


//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//


void Line::begin(){

	switch (_transmisionMode){

	case Line_Parallel_Mode:
		
		for (int i = 0; i <= (sizeof(_pinOut) / sizeof(_pinOut[0])); i++){

			IOMode(_pinOut[i], OUTPUT);
		}

		for (int i = 0; i <= (sizeof(_pinIn) / sizeof(_pinIn[0])); i++){

			IOMode(_pinIn[i], INPUT);
		}		

		break;

	case Line_I2C_Extra_Mode:

		if(_addr == Line_Master_Address	){

			IOMode(_pinOut[Line_Pin_I2C_CODE], OUTPUT);
		}

		else{

			IOMode(_pinIn[Line_Pin_I2C_CODE], INPUT);
		}

	case Line_I2C_Mode:

		if(_addr == Line_Master_Address	){

			IOMode(_pinOut[Line_Pin_I2C_SDA], OUTPUT);
			IOMode(_pinOut[Line_Pin_CLK], OUTPUT);
		}
		else{

			IOMode(_pinIn[Line_Pin_I2C_SDA], INPUT);
			IOMode(_pinIn[Line_Pin_CLK], INPUT);
		}

		break;

	case Line_SPI_Extra_Mode:

		if(_addr == Line_Master_Address){

			IOMode(_pinOut[Line_Pin_SPI_CODE], OUTPUT);
		}

		else{

			IOMode(_pinIn[Line_Pin_SPI_CODE], INPUT);
		}

	case Line_SPI_Mode:

		if(_addr == Line_Master_Address){

			IOMode(_pinOut[Line_Pin_SPI_MISO], INPUT);
			IOMode(_pinOut[Line_Pin_SPI_MOSI], OUTPUT);
			IOMode(_pinOut[Line_Pin_CLK], OUTPUT);
		}
		else{

			IOMode(_pinIn[Line_Pin_SPI_MISO], OUTPUT);
			IOMode(_pinIn[Line_Pin_SPI_MOSI], INPUT);
			IOMode(_pinIn[Line_Pin_CLK], INPUT);
		}

		break;
	}


}

void Line::end(){

	for (int i = 0; i < Line_Max_Conected_Devices; i++){

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
	

	for (int i = 0; i < Line_Max_Conected_Devices; i++){

		if (_conectedAddr[i] = device) repeated = true;
	}

	for (int i = 0; i < Line_Max_Conected_Devices; i++){

		if (_conectedAddr[i] == Line_Null_Address){

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


	if (returnCode == Line_Code_Disconect){
		
		for (int i = 0; i < Line_Max_Conected_Devices; i++){

			if (_conectedAddr[i] == device) _conectedAddr[i] = Line_Null_Address;

		}

		return true;
	}

	else return false;
}


//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//


bool Line::isConected(){

	for (int i = 0; i < Line_Max_Conected_Devices; i++){

		if (sendLineCode(Line_Code_Conect,_conectedAddr[i]) == Line_Code_Conect){

			return true;
		}			
	}

	return false;
	
}

bool Line::isConected(int device){

	if (sendLineCode(Line_Code_Conect,device) == Line_Code_Conect) return true;

	else return false;
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

	int bitdelay = (Line_Second / _bps);
	
	int datasize = (sizeof(data) / sizeof(data[0]));

	switch (_transmisionMode){

	case Line_Parallel_Mode:

		for (int i = 0; i <= datasize; i++){

			IOWrite(_pinOut[i], data[i]);
		}

		TimeWait(bitdelay);

		for (int i = 0; i <= datasize; i++){
        
			IOWrite(_pinOut[i], LOW);
		}

		break;

	case Line_I2C_Mode:

		for (int i = 0; i <= datasize; i++){

			IOWrite(_pinOut[Line_Pin_I2C_SDA], data[i]);
			TimeWait(bitdelay / 2);

			IOWrite(_pinOut[Line_Pin_CLK], HIGH);
			TimeWait(bitdelay / 2);
            
			IOWrite(_pinOut[Line_Pin_CLK], LOW);

		}

		IOWrite(_pinOut[Line_Pin_I2C_SDA], LOW);

		break;

	
	case Line_I2C_Extra_Mode:

		sendLineCode(Line_Code_Select,device);

		

		sendLineCode(Line_Code_Unselect,device);	
		
		break;
	

	case Line_SPI_Mode:

		break;

	case Line_SPI_Extra_Mode:

		sendLineCode(Line_Code_Select,device);

		
		
		sendLineCode(Line_Code_Unselect,device);

		break;
	}
}

bool * Line::read(int bits){

	bool buffer[bits];

	switch (_transmisionMode){

	case Line_Parallel_Mode:

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
	
	case Line_Parallel_Mode:

		write(data, device);

		return read(datasize);

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

	case Line_Parallel_Mode:

		return transfer(data, device);

		break;

	case Line_I2C_Extra_Mode:
	case Line_SPI_Extra_Mode:

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
	int responseBuffer[Line_Max_Binary_Code_Lenght];
	bool *codeBuffer = codeToBinary(code);

	bool isMaster = (addr == Line_Master_Address);
	bool isSlave = ((Line_Min_Slave_Address <= addr) and (addr <= Line_Max_Slave_Address));

	bool addrValid = ((isMaster) or (isSlave));

	int bitdelay = Line_Second / Line_Code_BPS;
	int responsePin = Line_Pin_CLK + 1;
	int codePin = Line_Pin_CLK + 2;

	switch (_transmisionMode){

	case Line_I2C_Extra_Mode:

		codePin = Line_Pin_I2C_CODE;
		responsePin = Line_Pin_I2C_SDA;
		
		break;
	
	case Line_SPI_Extra_Mode:

		codePin = Line_Pin_SPI_CODE;
		responsePin = Line_Pin_SPI_MISO;
		
		break;
	}
	

	
	if ((addrValid)){

		for (int i = 0; i <= Line_Code_Buffer_Size; i++){

			IOWrite(_pinOut[codePin], codeBuffer[i]);
			TimeWait(bitdelay / 4);

			IOWrite(_pinOut[Line_Pin_CLK], HIGH);
			TimeWait(bitdelay / 4);

			responseBuffer[i] = IORead(responseCode);
			TimeWait(bitdelay / 4);
            
			IOWrite(_pinOut[Line_Pin_CLK], LOW);

		}

		IOWrite(_pinOut[codePin], LOW);
		TimeWait(bitdelay / 4);
	}

	else if (addr == _addr){

		responseCode = Line_Code_Conect;
	}

	else{

		responseCode = Line_Code_Null;
	}

	return responseCode;
}

void readInterupt(){


}

void codeToBinary(int num, bool * bin){  

	for (int j = 0; j < Line_Max_Binary_Code_Lenght; j++){

		bin[j] = 0;
	}
	
   
    int i = 0;

    while (0 < num) { 
  
        bin[i] = num % 2; 
        num = num / 2; 
        i++; 
    } 

	int start = 0;
	int end = Line_Max_Binary_Code_Lenght - 1;

	while (start < end){

        int temp = bin[start];  
        bin[start] = bin[end]; 
        bin[end] = temp;

        start++; 
        end--; 
    }
}

int binaryToCode(bool * bin){


}