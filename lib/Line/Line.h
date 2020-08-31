#include "Constants.h"
#include "Compatibility.h"

#ifndef Line_h
#define Line_h

class Line
{

private:
	int *_pinOut;
	int *_pinIn;
	int _transmisionMode;
	int _bps;
	int _addr;
	int _conectedAddr[Line_Max_Conected_Devices];
	int _codeReceived;
	bool *_buffer;

	friend int sendLineCode(int code, int addr);
	friend void readInterupt();

public:
	Line(int *pinout, int *pinin, int transmisionmode, int bps, int addr);
	~Line();

	void begin();
	void end();

	void changeSetings(int *pinout, int *pinin, int transmisionmode, int bps, int addr);

	bool conect(int device);
	bool disconect(int device);

	bool isConected(int device);
	void waitConection(int device);


	bool write(bool *data, int device);
	bool *read(int bits);
	bool *transfer(bool *data, int device);

	void Request(int device);
	bool *Request(int device, bool *data);

	void onRequest(void (*callback)(bool *));
	void onReceive(void (*callback)(bool *));

};

#endif //Line_h