//--------------74595腳位-----------------------
#define latchPin 4  // Latch pin (STCP腳位)
#define clockPin 3 // Clock pin (SHCP腳位)
#define dataPin 2  // Data pin (DS腳位)
//--------------變數宣告--------------------------
int Led_State[]={1,3,7,15,31,63,127,255}; // LED陣列
int Led_State_Cun = 0; //LED Count
//--------------74595LED_Show副程式-----------------------
void Led_Show595(int LedData) //255 全亮
{
   digitalWrite(latchPin, LOW);
   shiftOut(dataPin, clockPin, MSBFIRST, LedData);  //將東西放入 LedData裡面
   digitalWrite(latchPin, HIGH);
}
//-------------74595的宣告-------------------
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
//-----------------74595LED-----------------------
Led_Show595 (Led_State[Led_State_Cun]);        //Led資料輸出
            Led_State_Cun++;
            if(Led_State_Cun>7) Led_State_Cun=0;

