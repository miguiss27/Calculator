#ifndef Line_Constants
#define Line_Constants


//default parameters

#define Line_Second				1000
#define Line_BPS				8
#define Line_Address			Line_Master_Address
#define Line_Transmision_Mode	Line_Parallel_Mode


//max parameters

#define Line_Max_Conected_Devices	2


//pin indexes

#define Line_Pin_CLK		0 

#define Line_Pin_I2C_SDA	1
#define Line_Pin_SPI_MOSI	1
#define Line_Pin_SPI_MISO	2

#define Line_Pin_I2C_CODE	2
#define Line_Pin_SPI_CODE	3


//transmision modes

#define Line_Min_Transmision_Mode	5
#define Line_Max_Transmision_Mode	5

#define Line_Parallel_Mode			0
#define Line_I2C_Mode				1
#define Line_SPI_Mode				2
#define Line_I2C_Extra_Mode			3
#define Line_SPI_Extra_Mode			4

#define Line_Suport_Codes			{3,4}

//address parameters

#define Line_Master_Address		0
#define Line_Min_Slave_Address	1
#define Line_Max_Slave_Address	2


//Line codes parameters 

#define Line_Code_Buffer_Size	8
#define Line_Code_BPS			80


//Line codes for comunication

#define Line_Max_Codes			7

#define Line_Code_Null			0
#define Line_Code_Conect		1
#define Line_Code_Disconect		2
#define Line_Code_Select		3
#define Line_Code_Unselect		4
#define Line_Code_Request		5
#define Line_Code_Data_Request	6


#endif //Line_Constants