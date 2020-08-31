#ifndef Key_Ref_Data_Lenght 
#define Key_Ref_Data_Lenght 4
#endif

#ifndef Key_Dict_Data_Lenght 
#define Key_Dict_Data_Lenght 5
#endif

#ifndef Joy_Dict_Data_Lenght
#define Joy_Dict_Data_Lenght 4
#endif

#ifndef Data_h
#define Data_h


typedef struct Joy_T{

  bool *dict;
  int rows;
  int columns;

  void init(int Rows, int Columns){

    this->dict = new bool[Rows * Columns];
    this->rows = Rows;
    this->columns = Columns;
    
  }

  void makejoy(bool dict_in [][Joy_Dict_Data_Lenght]){

    for (int r = 0; r < this->rows; r++){

      for (int c = 0; c < this->columns; c++){

        this->dict[r*(this->columns)+c] = dict_in[r][c];

      }
      
    }
    

  }

}Joy_T;

typedef struct Key_T{
  char *ref;
  bool *dict;
  int dict_rows;
  int dict_columns;
  int ref_rows;
  int ref_columns;

  void init(int Dict_rows, int Dict_columns, int Ref_rows, int Ref_columns){
    
    this->dict = new bool[Dict_rows * Dict_columns];
    this->dict_rows = Dict_rows;
    this->dict_columns = Dict_columns;

    this->ref = new char[Ref_rows * Ref_columns];
    this->ref_rows = Ref_rows;
    this->ref_columns = Ref_columns;

  }

  void makekey(bool dict_in [][Key_Dict_Data_Lenght], char ref_in [][Key_Ref_Data_Lenght]){

    for (int r = 0; r < this->dict_rows; r++){

      for (int c = 0; c < this->dict_columns; c++){

        this->dict[r*(this->dict_columns)+c] = dict_in[r][c];

      }
      
    }

    for (int r = 0; r < this->ref_rows; r++){

      for (int c = 0; c < this->ref_columns; c++){

        this->ref[r*(this->ref_columns)+c] = ref_in[r][c];

      }
      
    }

  }

}Key_T;

class Data{

private:

typedef struct Dat{

  int size;
  bool *container;

}Dat;

Dat Dat_store;
Joy_T Joy_store;
Key_T Key_store;
  
public:

  Data(int data_size, Joy_T joy_store, Key_T key_store);
  Data(int data_size);
  ~Data();


  void converttodata(bool center, int axis_x, int axis_y, bool button, char key);
  void converttoval(bool *center, int *axis_x, int *axis_y, bool *button, char *key);

  void resetdata();

  bool * getdata();
  int getdatasize();

  void newdata(bool *data_in, int data_size);

  void updatedata(bool *data_in);
  void updatedatasize(int data_size);

  void updatereferences(Joy_T New_Joy, Key_T New_Key);

};

#endif