#include "TimerOne.h"

//========================腳位宣告====================
//---------------按鍵腳位宣告----------------------------
#define Sw1Pin 10      
#define Sw2Pin 11
#define Sw3Pin 12
//--------------595腳位宣告---------------------------
#define DataPin 2   //DS腳位
#define ClkPin 3    //SH_CP腳位
#define LatchPin 4  //ST_CP腳位
//----------------------------------------------------

//========================變數宣告========================
int Led_Mode=0;                                  //Led模式 1:閃爍 2:左移 3:右移
boolean Flag_Led=0;                              //閃爍Led期標
int Led_State_Cun=0;                             //左右移Led狀態計數器
int Led_State[]={128,64,32,16,8,4,2,1};          //Led狀態
int Timer_1s=100,Timer_20ms=2;                   //計時器宣告 100ms*10s=1000ms
unsigned char Sw1, Sw1_last,Sw2, Sw2_last, Sw3, Sw3_last;         //鍵盤狀態暫存器
int Loop_Cun=255;                                             //迴圈計數器
//========================函數宣告===============================================================
//------------------------LED顯示-------------------------------------------------------------
//------------------------LED從595輸出------------------------------------------------------------
void LedShow595(int LedData595){
   digitalWrite(LatchPin, LOW);                       // 送資料前要先把 LatchPin 設成低電位
   shiftOut(DataPin, ClkPin, LSBFIRST, LedData595);     //送出資料
   digitalWrite(LatchPin, HIGH);                      // 送完資料後要把 LatchPin 設成高電位
}
void LedShow(){
   if(Timer_1s == 0){                      //1sLED顯示一次
      Timer_1s = 100;                     //Timer初值
      switch(Led_Mode){
       case 1:          
          if(Flag_Led==0){                               //Led右移
            LedShow595(Led_State[Led_State_Cun]);        //Led資料輸出
            Led_State_Cun++;                             //Led狀態指標加1
            if(Led_State_Cun>7){                         //判斷是否右移完
               Flag_Led=!(Flag_Led);                     //Led指標反向改左移
               Led_State_Cun=7;                          //指標指向左移第一個
            }           
          }            
          else{
           LedShow595(Led_State[Led_State_Cun]);         //Led左移
           Led_State_Cun--;                              //Led指標加1
           if(Led_State_Cun<0){                          //判斷左移是否移完
             Flag_Led=!(Flag_Led);                       //Led指標反向改右移
             Led_State_Cun=0;                            //指標指向右移第一個
           }           
          }
       
          
         break;
      //----------------------------------------------
      case 2:                                          //Led右移
        if(Loop_Cun>0){                                //判斷要做幾次'
          LedShow595(Led_State[Led_State_Cun]);        //Led資料輸出
          Led_State_Cun++;                             //Led狀態指標加1
          if(Led_State_Cun>7){                         //判斷是否右移完
            Led_State_Cun=0;                           //Led狀態暫存器復歸
            Loop_Cun--;                                //迴圈暫存器減1
          }
        }
        else{                                          
          LedShow595(0);                               //做完後讓Led全暗
        }
        break;
       //-----------------------------------------
       case 3:
        if(Loop_Cun>0){      
         LedShow595(Led_State[Led_State_Cun]);
         Led_State_Cun++;
         if(Led_State_Cun>7) Led_State_Cun=0;
        }
        else{
            LedShow595(0);
        }
        break;
             
        //---------------------------------------
        default:
          LedShow595(0);
          break;
      //
      }
    }   
}
//-------------------------按鍵程式-------------------------------------------------------------
void KeyScan(){
  if(Timer_20ms == 0){                    //20ms偵測一次鍵盤 
      Timer_20ms = 2;                     //Timer初值
      Sw1 = digitalRead(Sw1Pin);          //偵測按鍵1狀態
      if((Sw1_last == 1)&&(Sw1 == 0)){    //負緣觸發
        Led_Mode=1;                       //設定為閃爍模式
        Led_State_Cun=0;    
               
      }            
      //--------------------------------------
      Sw2 = digitalRead(Sw2Pin);          //偵測按鍵2狀態
      if((Sw2_last == 1)&&(Sw2 == 0)){    //負緣觸發     
         Led_Mode=2;
         Loop_Cun=3;     
      }
      //------------------------------------------
      Sw3 = digitalRead(Sw3Pin);          //偵測按鍵3狀態
      if((Sw3_last == 1)&&(Sw3 == 0)){    //負緣觸發     
         Led_Mode=3;
         Loop_Cun=1;     
      }

      

      Sw1_last = Sw1;                     //保留上次按鍵1狀態
      Sw2_last = Sw2;                     //保留上次按鍵2狀態
      Sw3_last = Sw3;                     //保留上次按鍵3狀態
   }  
}


//------------------------計數器中斷----------------------------------------------------------------
void MyTimeInterrupt(){
  if(Timer_1s > 0) 
     Timer_1s--;
//---------------------------
   if(Timer_20ms > 0) 
     Timer_20ms--;
}
//=====================================================================================================
void setup() {
  // put your setup code here, to run once:
  
//**************Urta初始化*****************************************   
  Serial.begin(9600); 
//**************計時器宣告*****************************************
  Timer1.attachInterrupt(MyTimeInterrupt);
  Timer1.initialize(10000);          //10ms為中斷單位
//**************LED腳位宣告*****************************************
   pinMode(Sw1, INPUT_PULLUP);
   pinMode(Sw2, INPUT_PULLUP);
   pinMode(Sw3, INPUT_PULLUP);
//**************LED腳位宣告*****************************************
//----------- 74HC595宣告----------------------------------
  pinMode(2,OUTPUT);    //DataPin
  pinMode(3,OUTPUT);    //ClkPin
  pinMode(4,OUTPUT);    //LatchPin

}
//*******************************************************************
void loop() {
     KeyScan();
     LedShow();
}
  
