//--------------引入library---------------------
#include "TimerOne.h"
//--------------74595腳位-----------------------
/*#define latchPin 4  // Latch pin (STCP腳位)
#define clockPin 3 // Clock pin (SHCP腳位)
#define dataPin 2  // Data pin (DS腳位)*/
//-------------按鍵的腳位-----------------------
const int Sw1_Pin=2;
const int Sw2_Pin=3;
const int Sw3_Pin=4;
//--------------四位數七段顯示器角為---------------
const int Seg4_DataPin[]={8,9,10,11,12,13,17,18};
const int Seg4_ScanPin[]={5,16,15,14};
const int IC273_ClkPin=7;
//------------74244腳位-------------------------
/*const int IC244_DataPin[]={8,9,13,14,15,16,17,18};
const int IC244_EnPin=19;*/
//-------------74240腳位(反向)----------------------------
const int IC240_DataPin[]={8,9,10,11,12,13,17,18};
const int IC240_EnPin=6;
//---------------74273腳位------------------------
/*const int IC273_DataPin[]={8,9,13,14,15,16,17,18};
const int IC273_MrPin=6;
const int IC273_ClkPin=5;*/
//--------------變數宣告-------------------------
int Timer_1s=100,Timer_20ms=2,Timer_30ms=3,Timer_50ms=5,Timer_3s=300;                   //計時器宣告 100ms*10s=1000ms
int Sw1,Sw1_last,Sw1_Cut=0,Sw2,Sw2_last,Sw2_Cut=0,Sw3,Sw3_last,Sw3_Cut=0; //按鍵的宣告
int Led_Mode= 0; //LED模式的宣告
bool Flag_Led = false; //led 旗標
int Led_State[]={1,3,7,15,31,63,127,255}; // LED陣列
int Led_State_Cun = 0; //LED Count
int Led_LoopOn = 0; //迴圈的次數
int Function = 0;
int Led_LoopOn_Backup= 0;
int Seg4_Mode= 0;
int OnLine=0;
//----------------------Uart變數宣告-------------------------------
char UartReceiveBuf[10];
char Time_h[2];
char Time_m[2];
char Time_s[2];
String inputString = "";
boolean stringComplete = false;
char inChar;
//----------------七段顯示器---------------------------
int Seg_Mode=0;
int Seg4_LoopOn =0;
const byte SegCode[10]={B00111111,B00000110,B01011011,B01001111,B01100110,
                        B01101101,B01111101,B00000111,B01111111,B01101111}; //0~9直接用在 SegCode[]
int Seg4_Data[]={0,0,0,0,0};
//--------------74595副程式-----------------------
//void Led_Show595(int LedData) //255 全亮
//---------------74595以下都不用改----------------------
/*{
   digitalWrite(latchPin, LOW);
   shiftOut(dataPin, clockPin, MSBFIRST, LedData);  //將東西放入 LedData裡面
   digitalWrite(latchPin, HIGH);
}*/
//--------------244副程式----------------------------
/*void LED_Show244(int LedData244)
{   
     
    for (int i=0;i<8;i++){
        if(bitRead(LedData244,i)) digitalWrite(IC244_DataPin[i], HIGH);
        else digitalWrite(IC244_DataPin[i], LOW);
    }
    digitalWrite (IC240_EnPin, LOW);
    

}*/
//------------------240副程式--------------------------------------
void LED_Show240(int LedData240)

{   
    for (int i=0;i<8;i++){
        if(bitRead(LedData240,i)) digitalWrite(IC240_DataPin[i], LOW);
        else digitalWrite(IC240_DataPin[i], HIGH);
    }
    digitalWrite (IC240_EnPin, LOW);
    

}
//------------四位數七段顯示器輸出---------------------------------
void Seg4_DataOut(int Seg4_Data0,int Seg4_Data1,int Seg4_Data2,int Seg4_Data3,int Seg4_Data4)
{   
    Seg4_Data[0]=Seg4_Data0;
    Seg4_Data[1]=Seg4_Data1;
    Seg4_Data[2]=Seg4_Data2;
    Seg4_Data[3]=Seg4_Data3;
    Seg4_Data[4]=Seg4_Data4;
}
//-----------------四位數七段顯示器畫面切換(三秒鐘切一次)----------------------------
/*void Seg4_Show1()

{
     if(Timer_3s == 0){
        Timer_3s = 300;
        switch (Seg4_Mode) {
            case 1:
                Seg4_DataOut(8,B00111001,2,9,8);//8.4.2.1
                break;
            case 2:
                Seg4_DataOut(8,B00111001,1,0,7);//8.4.2.1
                Seg4_Mode =3;
              break;
            case 3:
                Seg4_DataOut(0,1,1,9,8);//8.4.2.1
           break;
     }
        } 
}*/
//--------------------------------------------------------------------------------
void serialEvent() {
    while (Serial.available()) {
          inChar = (char)Serial.read();
          inputString += inChar;
    }      
    if (inChar == '\n') {
      inputString.toCharArray(UartReceiveBuf, 10);    
      inputString = "";      
      switch(UartReceiveBuf[0]){
        case 'K':        
          OnLine=1;
          Serial.println('K');
          break;
        case 'T':          
          if(UartReceiveBuf[1]=='1'){
           // Serial.println('1');
            Function=3;
            Time_h[1]=UartReceiveBuf[2]-'0';
            Time_h[0]=UartReceiveBuf[3]-'0';
            Time_m[1]=UartReceiveBuf[4]-'0';
            Time_m[0]=UartReceiveBuf[5]-'0';
            Serial.print(UartReceiveBuf[6]);
            Serial.print(UartReceiveBuf[7]);
            Time_s[1]=UartReceiveBuf[6]-'0';
            Time_s[0]=UartReceiveBuf[7]-'0';
          }        
        break; 
      }  
      stringComplete = true;       
    }    
   
}
//------------四位數七段顯示器的副程式--------------------------------
void Seg4_Show()
{
    if(Timer_50ms == 0){
        Timer_50ms = 5;
        if(Seg4_LoopOn==1){
            Led_LoopOn_Backup=Led_LoopOn;
            Led_LoopOn=0;
            digitalWrite (IC240_EnPin, HIGH); //關閉 LED
            if(bitRead(Seg4_Data[0],0)){
                for(int i=0;i<8;i++){
                    if(bitRead(Seg4_Data[4],i)) digitalWrite(Seg4_DataPin[i], HIGH);
                    else digitalWrite(Seg4_DataPin[i], LOW);
                }
            }
            else{
                for(int i=0;i<8;i++){
                    if(bitRead(SegCode[Seg4_Data[4]],i)) digitalWrite(Seg4_DataPin[i], HIGH);
                    else digitalWrite(Seg4_DataPin[i], LOW);
                }
            }   
            digitalWrite(IC273_ClkPin, LOW);
            delay(1);
            digitalWrite(IC273_ClkPin, HIGH);
            digitalWrite(Seg4_ScanPin[0], LOW);
            delay(1);
            digitalWrite(Seg4_ScanPin[0], HIGH);
            //===============================================================
             if(bitRead(Seg4_Data[0],1)){
                for(int i=0;i<8;i++){
                    if(bitRead(Seg4_Data[3],i)) digitalWrite(Seg4_DataPin[i], HIGH);
                    else digitalWrite(Seg4_DataPin[i], LOW);
                }
              }
              else{
                for(int i=0;i<8;i++){
                 if(bitRead(SegCode[Seg4_Data[3]],i)) digitalWrite(Seg4_DataPin[i], HIGH);
                    else digitalWrite(Seg4_DataPin[i], LOW);
                 }
              }
            digitalWrite(IC273_ClkPin, LOW);
            delay(1);
            digitalWrite(IC273_ClkPin, HIGH);
            digitalWrite(Seg4_ScanPin[1], LOW);
            delay(1);
            digitalWrite(Seg4_ScanPin[1], HIGH);
            //------------------------------------------------------------------
             if(bitRead(Seg4_Data[0],2)){
                for(int i=0;i<8;i++){
                    if(bitRead(Seg4_Data[2],i)) digitalWrite(Seg4_DataPin[i], HIGH);
                    else digitalWrite(Seg4_DataPin[i], LOW);
                }
              }
            else{
                for(int i=0;i<8;i++){
                    if(bitRead(SegCode[Seg4_Data[2]],i)) digitalWrite(Seg4_DataPin[i], HIGH);
                    else digitalWrite(Seg4_DataPin[i], LOW);
                } 
            }
            digitalWrite(IC273_ClkPin, LOW);
            delay(1);
            digitalWrite(IC273_ClkPin, HIGH);
            digitalWrite(Seg4_ScanPin[2], LOW);
            delay(1);
            digitalWrite(Seg4_ScanPin[2], HIGH);
            //-------------------------------------------------------------------
             if(bitRead(Seg4_Data[0],3)){
                for(int i=0;i<8;i++){
                    if(bitRead(Seg4_Data[1],i)) digitalWrite(Seg4_DataPin[i], HIGH);
                    else digitalWrite(Seg4_DataPin[i], LOW);
                }
              }
            else{
                for(int i=0;i<8;i++){
                    if(bitRead(SegCode[Seg4_Data[1]],i)) digitalWrite(Seg4_DataPin[i], HIGH);
                    else digitalWrite(Seg4_DataPin[i], LOW);
                }
            }
            digitalWrite(IC273_ClkPin, LOW);
            delay(1);
            digitalWrite(IC273_ClkPin, HIGH);
            digitalWrite(Seg4_ScanPin[3], LOW);
            delay(1);
            digitalWrite(Seg4_ScanPin[3], HIGH);

            //---------------還原LED迴圈------------------
                Led_LoopOn=Led_LoopOn_Backup;
                if(Function==0 && Led_Mode==1){
                    LED_Show240(Led_State[Led_State_Cun]);
                    digitalWrite (IC240_EnPin, LOW); 
                }
        }
    }
}

//-------------------74273副程式-----------------------------------
/*void LED_Show273(int LedData273)
{   
    digitalWrite (Ic_273_Mr_Pin, HIGH);
    digitalWrite (Ic_273_Clk_Pin, LOW);
    if((LedData273) & (0b00000001)) digitalWrite(D0_273Pin, HIGH);
    else digitalWrite(D0_273Pin, LOW);

    if((LedData273) & (0b00000010)) digitalWrite(D1_273Pin, HIGH);
    else digitalWrite(D1_273Pin, LOW);

    if((LedData273) & (0b00000100)) digitalWrite(D2_273Pin, HIGH);
    else digitalWrite(D2_273Pin, LOW);

    if((LedData273) & (0b00001000)) digitalWrite(D3_273Pin, HIGH);
    else digitalWrite(D3_273Pin, LOW);

    if((LedData273) & (0b00010000)) digitalWrite(D4_273Pin, HIGH);
    else digitalWrite(D4_273Pin, LOW);

    if((LedData273) & (0b00100000)) digitalWrite(D5_273Pin, HIGH);
    else digitalWrite(D5_273Pin, LOW);

    if((LedData273) & (0b01000000)) digitalWrite(D6_273Pin, HIGH);
    else digitalWrite(D6_273Pin, LOW);

    if((LedData273) & (0b10000000)) digitalWrite(D7_273Pin, HIGH);
    else digitalWrite(D7_273Pin, LOW);

   digitalWrite (Ic_273_Clk_Pin, HIGH);

   delay(10);


}*/
//---------------------------------------------------
void MyTimeInterrupt() { 
   if(Timer_1s > 0)                     //1秒鐘計時
     Timer_1s--;                       //累減
//---------------------------
   if(Timer_20ms > 0)                 //20ms計時
     Timer_20ms--;                    //累減
//---------------------------
   if(Timer_30ms > 0)                //30ms計時
     Timer_30ms--;                   //累減
//---------------------------
   if(Timer_50ms > 0)                //100ms計時
     Timer_50ms--;                   //累減
//---------------------------
   if(Timer_3s > 0)                //300ms計時
     Timer_3s--;                   //累減
}
//==============按鍵的副程式===========================
void KeyScan(){
  if(Timer_20ms == 0){                    //20ms偵測一次鍵盤 
      Timer_20ms = 2;                     //Timer初值
      Sw1 = digitalRead(Sw1_Pin);          //偵測按鍵1狀態
      if((Sw1_last == 1)&&(Sw1 == 0)){    //負緣觸發
        Led_Mode=1;                       //Led第一種模式          
        Led_State_Cun=0;                  //Led計數Cunter
        Seg4_LoopOn=0;                     //七段顯示器功能關閉
        delay(50);                        
        Led_LoopOn=1;                     //Led功能打開 
      }            
      //--------------------------------------
      Sw2 = digitalRead(Sw2_Pin);          //偵測按鍵2狀態
      if((Sw2_last == 1)&&(Sw2 == 0)){    //負緣觸發  
         Function=2;   
         Led_Mode=2;  
         Seg4_LoopOn=0; 
         delay(50); 
         Led_LoopOn=0;    
         digitalWrite(IC240_EnPin,HIGH);
         Seg_Mode=2;
         Seg4_LoopOn=1;          
      }
      //------------------------------------------
      Sw3 = digitalRead(Sw3_Pin);          //偵測按鍵3狀態
      if((Sw3_last == 1)&&(Sw3 == 0)){    //負緣觸發                
      }
      Sw1_last = Sw1;                     //保留上次按鍵1狀態
      Sw2_last = Sw2;                     //保留上次按鍵2狀態
      Sw3_last = Sw3;                     //保留上次按鍵3狀態
   }  
}

    //---------LED的副程式--------------
void LedShow(){   
   if(Timer_1s == 0){                      //1sLED顯示一次
      Timer_1s = 100;                     //Timer初值
   
      if(Led_LoopOn==1){
         //Seg_LoopOn=0;
         switch(Led_Mode){
         case 1:                      
            LED_Show240(Led_State[Led_State_Cun]);
            digitalWrite(IC240_EnPin,LOW);
            delay(5);
            Led_State_Cun++;                             //Led狀態指標加1
            if(Led_State_Cun>8){                         //判斷是否右移完
            Led_State_Cun=0;                          //指標指向左移第一個
            }
                     
            break;
         case 2:  
             
            break; 
        }   
      }   
   }  
   Seg4_LoopOn=1; 
      //-------------------------------------------------------------------------
  // if(Function==2 && Seg_Mode==3){
  //    Seg_4Out(0,1,1,9,8);
  //    Seg_Mode=4;      
  // }
   //-----------------------------------------------------------------------
   if(Function==2 && Seg_Mode==2){
      Seg4_DataOut(8,B00111001,1,0,7);        
   }
 }

         /* if(Flag_Led==0){                               //Led右移
            //Led_Show595(Led_State[Led_State_Cun]);        //Led資料輸出
            //LED_Show244(Led_State[Led_State_Cun]); 
            LED_Show273(Led_State[Led_State_Cun]);
            Led_State_Cun++;                             //Led狀態指標加1
            if(Led_State_Cun>7){                         //判斷是否右移完
               Flag_Led=!(Flag_Led);                     //Led指標反向改左移
               Led_State_Cun=7;                          //指標指向左移第一個
            }           
          }            
          else{
           //Led_Show595(Led_State[Led_State_Cun]);         //Led左移
           //LED_Show244(Led_State[Led_State_Cun]);
           LED_Show273(Led_State[Led_State_Cun]);
           Led_State_Cun--;                              //Led指標加1
           if(Led_State_Cun<0){                          //判斷左移是否移完
             Flag_Led=!(Flag_Led);                       //Led指標反向改右移
             Led_State_Cun=0;                            //指標指向右移第一個
           }           
          }
       
       }*/
          
         //break;


    /*if(Timer_1s == 0){
        Timer_1s = 50; //1s=100 0.5s=50
        switch (Led_Mode){
            case 1: //按鈕一
            //----------------按鈕按一下-------------------
                if(Sw1_Cut==1) //按第一下
               // Led_Show595(255);
                if(Loop_Cun>0){ //執行次數小於0不執行
                
                for (int i=7;i>0;i--){
                    Led_State_Cun=i;
                    Led_Show595(Led_State[Led_State_Cun]);
                }
                   /* Led_State_Cun--; //LED 右至左減少
                    if(Led_State_Cun<0){ 
                        Led_State_Cun=7; //預設為7~0
                       Loop_Cun--;//執行次數
                    }*/
                    /*Led_State_Cun++; //LED 左至右增加
                    if(Led_State_Cun>7){  
                        Led_State_Cun=0; //預設為0~7
                        Loop_Cun--;
                    }*/
               // } */
                  
               /*Led_Show595(Led_State[Led_State_Cun]);
                    Led_State_Cun++; //LED 左至右增加
                    if(Led_State_Cun>7){
                        Led_State_Cun=0; //預設為0~7
                        Loop_Cun--;
                        Timer_1s = 50;
                        }*/
                    /*if(Flag_Led==false){
                        Led_Show595(255);
                        Flag_Led=!(Flag_Led);
                    } 
                    else {
                        Led_Show595(0);
                        Flag_Led=!(Flag_Led);
                        
                    }
                    Loop_Cun--;*/
                   //  //Led_Show595(0);
              
                
            //-----------------按鈕按兩下-------------------
               /*if(Sw1_Cut==3) //按第二下 1+2 
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
                break;*/
//   }

//    }
//   }
// }
//-------------74595LED--VB部分--------------------------
    void Led_ShowVB()
    {
        int inByte = Serial.read();
        switch (inByte) { //1,3,7,15,31,63,127,255
            case '1': //LED全觀
                LED_Show240(0);
            break;
            case '2': //LED 1 開
                LED_Show240(1);
            break;
            case '4': //LED 2 開
                LED_Show240(2);
            break;
            case '6': 
                LED_Show240(4);
            break;
            case '8':
                LED_Show240(8);
            break;
            case '10':
                LED_Show240(16);
            break;
            case '12':
                LED_Show240(32);
            break;
            case '14':
                LED_Show240(64);
            break;
            case '16':
                LED_Show240(128);
            break;
            case '11':
                LED_Show240(0);
        }
    }
//---------------------VB到Seg4---------------------
void SegTime(){
    int Time = Serial.read();
    Seg4_LoopOn=1;
            Seg4_DataOut(0,0,0,Time,Time);//8.4.2.1
            Seg4_Show();
}
//--------------程式初始化-------------------------
void setup() {
  //-------------uart的宣告------------------
  Serial.begin(9600);
  //-------------74595的宣告-------------------
  /*pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);*/
  //------------74244的宣告---------------------
  /*for(int i=0;i<8;i++){
    pinMode(IC244_DataPin[i], OUTPUT);
  }
  pinMode(IC244_EnPin, OUTPUT);*/
  //------------74240的宣告------------------------
  for(int i=0;i<8;i++){
    pinMode(IC240_DataPin[i], OUTPUT);
  }
  pinMode(IC240_EnPin, OUTPUT);
  //digitalWrite (Ic_240_En_Pin, HIGH);
//--------------74273宣告------------------------
    /*for(int i=0;i<8;i++){
    pinMode(IC273_DataPin[i], OUTPUT);
  }
  pinMode(IC273_EnPin, OUTPUT);
  pinMode(Ic273_ClkPin, OUTPUT);*/
  //-----------------Timer的宣告--------------------
  Timer1.attachInterrupt(MyTimeInterrupt); //設定中斷副程式
  Timer1.initialize(10000); //設定10ms時間中
  //-----------------按鍵的宣告--------------------------
  pinMode(Sw1_Pin, INPUT_PULLUP);
  pinMode(Sw2_Pin, INPUT_PULLUP);
  pinMode(Sw3_Pin, INPUT_PULLUP);
  //----------------四位數七段顯示器宣告-------------------
  for(int i=0;i<8;i++){
       pinMode(Seg4_DataPin[i], OUTPUT);
  }

  for(int i=0;i<4;i++){
      pinMode(Seg4_ScanPin[i], OUTPUT);
      digitalWrite(Seg4_ScanPin[i], HIGH);
  }
  pinMode(IC273_ClkPin, OUTPUT);
}
//---------------主程式---------------------------
void loop() {
    KeyScan();    
    LedShow();
    Seg4_Show(); 
//---------------功能1----------------------------
  if(OnLine==0){
    if(Function==1){    
        Seg4_LoopOn=1;
        Seg_Mode=1;
        Seg4_DataOut(8,B00111001,2,9,8);
    }
  }
 else{
    if(Function==3){    
      if(stringComplete==true){
        Seg4_DataOut(9,B01000000,Time_s[1],Time_s[0],B01000000);  
        stringComplete = false;  
     }
   }
}
 
}

