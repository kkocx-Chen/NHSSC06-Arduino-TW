//--------------引入library---------------------
#include "TimerOne.h"
//--------------74595腳位-----------------------
#define latchPin 4  // Latch pin (STCP腳位)
#define clockPin 3 // Clock pin (SHCP腳位)
#define dataPin 2  // Data pin (DS腳位)
//-------------按鍵的腳位-----------------------
#define Sw1_Pin 10 //按鈕一腳位
#define Sw2_Pin 11 //按鈕二腳位
#define Sw3_Pin 12 //按鈕三腳位
//--------------變數宣告-------------------------
int Timer_20ms = 2,Timer_1s = 100; //Timer 的宣告
int Sw1,Sw1_last,Sw1_Cut=0,Sw2,Sw2_last,Sw2_Cut=0,Sw3,Sw3_last,Sw3_Cut=0; //按鍵的宣告
int Led_Mode= 0; //LED模式的宣告
bool Flag_Led = false; //led 旗標
int Led_State[]={128,64,32,16,8,4,2,1}; // LED陣列
int Led_State_Cun = 0; //LED Count
int Loop_Cun = 0; //迴圈的次數
//--------------74595副程式-----------------------
void Led_Show595(int LedData) //255 全亮
//---------------74595以下都不用改----------------------
{
   digitalWrite(latchPin, LOW);
   shiftOut(dataPin, clockPin, MSBFIRST, LedData);  //將東西放入 LedData裡面
   digitalWrite(latchPin, HIGH);
}
//---------------------------------------------------
void MyTimeInterrupt() { 
    if(Timer_20ms>0) //毫秒格式
      Timer_20ms--; //開始 Timer 倒數
    //----------------------------
    if(Timer_1s>0) //秒數格式
      Timer_1s--; //開始 Timer 倒數
}
//==============按鍵的副程式===========================
void KeyScan(){
 if(Timer_20ms == 0){  //Timer 初始化
        Timer_20ms = 2; // 按鍵兩毫秒
        Sw1 = digitalRead(Sw1_Pin); //讀取按鈕一 腳位
        if((Sw1_last==1)&&(Sw1==0)){ //Sw1=0
            Sw1_Cut++; //
            Loop_Cun = 6; //執行次數 閃爍*2   
            Led_Mode = 1; //配合 case 1
        }
        //=============================== 
        Sw2 = digitalRead(Sw2_Pin); //讀取按鈕二 腳位
        if((Sw2_last==1)&&(Sw2==0)){
            Loop_Cun = 2; //執行次數
            Led_State_Cun=0; //流水燈從0開始
            Led_Mode = 2; //配合 case 2
        } 
        Sw3 = digitalRead(Sw3_Pin); //讀取按鈕三 腳位
        if((Sw3_last==1)&&(Sw3==0)){
           Loop_Cun = 3; //執行次數
           Led_State_Cun=7; //流水燈從7開始
           Led_Mode = 3;  //配合 case 3
        } 
         
         Sw1_last = Sw1; //將設定好的 Sw1_list 配給 Sw1 供使用
         Sw2_last = Sw2; //將設定好的 Sw2_list 配給 Sw1 供使用
         Sw3_last = Sw3; //將設定好的 Sw3_list 配給 Sw1 供使用
 }
}
    //---------LED的副程式--------------
void Led_Show(){
    if(Timer_1s == 0){
        Timer_1s = 50; //1s=100 0.5s=50
        switch (Led_Mode){
            case 1: //按鈕一
            //----------------按鈕按一下-------------------
                if(Sw1_Cut==1) //按第一下
                if(Loop_Cun>0){ //執行次數小於0不執行
                    if(Flag_Led==false){
                        Led_Show595(255);
                        Flag_Led=!(Flag_Led);
                    } 
                    else {
                        Led_Show595(0);
                        Flag_Led=!(Flag_Led);
                        
                    }
                    Loop_Cun--;
                }
                
            //-----------------按鈕按兩下-------------------
               if(Sw1_Cut==3) //按第二下 1+2 
               if(Loop_Cun>0){   
                    Led_Show595(Led_State[Led_State_Cun]);
                    Led_State_Cun++; //LED 左至右增加
                    if(Led_State_Cun>7){
                        Led_State_Cun=0; //預設為0~7
                        Loop_Cun--;
                    }
                }  
                else 
                    Led_Show595(0);
                
            //---------------按鈕按三下--------------------
                 if(Sw1_Cut==6) //按第三下 3+3
                 if(Loop_Cun>0){  
                    Led_Show595(Led_State[Led_State_Cun]);
                    Led_State_Cun--; //LED 右至左減少
                    if(Led_State_Cun<0){ 
                        Led_State_Cun=7; //預設為7~0
                        Loop_Cun--;
                        
                    }
            //---------------按鈕按四下清0--------------------
                    if(Sw1_Cut==10){               
                        Sw1_Cut=0;
                        Led_Show595(0);
                    }    
                    else{
                    }
                 }
                 
                else
                    Led_Show595(0);
                break;
            case 2: //按鈕二
                if(Loop_Cun>0){   
                    Led_Show595(Led_State[Led_State_Cun]);
                    Led_State_Cun++;
                    if(Led_State_Cun>7){
                        Led_State_Cun=0;
                        Loop_Cun--;
                    }
                }  
                else 
                    Led_Show595(0);
                break;
            case 3: //按鈕三
               if(Loop_Cun>0){  
                    Led_Show595(Led_State[Led_State_Cun]);
                    Led_State_Cun--;
                    if(Led_State_Cun<0){
                        Led_State_Cun=7;
                        Loop_Cun--;
                    }
               }
                else
                    Led_Show595(0);
                break;
        }
    }
//-------------74595LED--VB部分--------------------------
    void Led_ShowVB()
    {
        int inByte = Serial.read();
        switch (inByte) {
            case '1':
            if(Loop_Cun>0){ //執行次數小於0不執行
                    if(Flag_Led==false){
                        Led_Show595(255);
                        Flag_Led=!(Flag_Led);
                    } 
                    else {
                        Led_Show595(0);
                        Flag_Led=!(Flag_Led);
                        
                    }
                    Loop_Cun--;
                    break;
            case '2':
             if(Loop_Cun>0){   
                    Led_Show595(Led_State[Led_State_Cun]);
                    Led_State_Cun++;
                    if(Led_State_Cun>7){
                        Led_State_Cun=0;
                        Loop_Cun--;
                    }
                }else
                    Led_Show595(0);  
                    break;
            case '3':
                 if(Loop_Cun>0){  
                    Led_Show595(Led_State[Led_State_Cun]);
                    Led_State_Cun--;
                    if(Led_State_Cun<0){
                        Led_State_Cun=7;
                        Loop_Cun--;
                    }
               }
               break;
            }
        }
    }
}
//--------------程式初始化-------------------------
void setup() {
  //-------------urtal的宣告------------------
  Serial.begin(9600);
  //-------------74595的宣告-------------------
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
  //-----------------Timer的宣告--------------------
  Timer1.attachInterrupt(MyTimeInterrupt); //設定中斷副程式
  Timer1.initialize(10000); //設定10ms時間中
  //-----------------按鍵的宣告--------------------------
  pinMode(Sw1_Pin, INPUT_PULLUP);
  pinMode(Sw2_Pin, INPUT_PULLUP);
  pinMode(Sw3_Pin, INPUT_PULLUP);
}
//---------------主程式---------------------------
void loop() {
   KeyScan();
   Led_Show();
   Led_ShowVB();
}