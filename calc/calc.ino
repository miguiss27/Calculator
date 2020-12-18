#include "Data.h"
#include "Utils.h"



#define Joy_Dict_Data_Lenght 4

#define Joy_x 4
#define Joy_y 3
#define Joy_button 5

#define Out_dat 0
#define Out_clk 1
#define Out_syn 2


const int Data_Lenght = 4;
const int Joy_Data_Lenght = 4;

const int Joy_Dict_Lenght = 10;

int bitdelay = 25 ;


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

Joy_T joy;

Data data = Data(Data_Lenght);


void setup(){

  pinMode(Out_dat, OUTPUT);
  pinMode(Out_clk, OUTPUT);
  pinMode(Out_syn, OUTPUT);

  joy.init(Joy_Dict_Lenght,Joy_Data_Lenght);

  joy.makejoy(Joy_Dict);

  data.updatereferences(joy/*,key*/);

  InfoInit();

}

void loop(){
  
  bool Var_Center = Info('c');
  bool Var_Button = Info('b');
  int Var_X_Axis = Info('x');
  int Var_Y_Axis = Info('y');

  data.converttodata(Var_Center,Var_X_Axis,Var_Y_Axis,Var_Button,"h");

  lineWrite(data.getdata());

  data.resetdata();
}

void lineWrite(bool data[Data_Lenght]){

  for (int i = 0; i <= (sizeof(data)/sizeof(data[0])); i++){

      digitalWrite(Out_dat, data[i]);
      delay(bitdelay / 3);

      digitalWrite(Out_clk, HIGH);
      delay(bitdelay / 3);
            
      digitalWrite(Out_clk, LOW);

    }

    digitalWrite(Out_dat, LOW);
    digitalWrite(Out_syn,HIGH);
    delay(bitdelay / 3);

}
