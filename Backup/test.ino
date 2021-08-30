//--------------引入library---------------------
//--------------74595腳位-----------------------
int latchPin = 4;  // Latch pin (STCP腳位)
int clockPin = 3; // Clock pin (SHCP腳位)
int dataPin = 2;  // Data pin (DS腳位)
//--------------變數宣告-------------------------
unsigned char SerialReceiveCnt , SerialAnalyze;
char UartReceiveBuf[99];
char LedData;
char SegData[6];
const int LED_PIN =  12; 
byte leds = 0;    // 亮燈的LED數量
char f;
//--------------函數宣告-------------------------
/*void UartReceive() { //usb資料接收
    if (Serial.available()){   //如果有資料就開始接收
    SerialReceiveCnt=0; //把UartReceiveBuf(接收)指標指向0
    memset (UartReceiveBuf,0,99); //buf 大小是99 這個是清空buf的內容
    if(SerialAnalyze==1){
        while(Serial.available()>0){ //Serial.available接收直到沒有資料
            UartReceiveBuf[SerialReceiveCnt++]=char (Serial.read()); //接收多個字節放到UartReceiveBuf裡面
            delay(10);
        }   
    } 
    //Serial.print(UartReceiveBuf); //把UartReceiveBuf印出來 看有沒有收到
    switch (UartReceiveBuf[0])
    {
    case '1':
        LedData=UartReceiveBuf[1];
        Serial.println(LedData);
        break;
    case '2': //取得時間 
        SegData[0]=UartReceiveBuf[1];
        SegData[1]=UartReceiveBuf[2];
        SegData[2]=UartReceiveBuf[4];
        SegData[3]=UartReceiveBuf[5];
        SegData[4]=UartReceiveBuf[7];
        SegData[5]=UartReceiveBuf[8];
        Serial.println("HH:"+String(SegData[0])+String(SegData[1]));
        Serial.println("MM:"+String(SegData[2])+String(SegData[3]));
        Serial.println("SS:"+String(SegData[4])+String(SegData[5]));
    default:
        break;
    }
  }
}*/
//--------------74595副程式-----------------------
void updateShiftRegister() //由左至右副程式 MSBFIRST
{
   digitalWrite(latchPin, LOW);
   shiftOut(dataPin, clockPin, MSBFIRST, leds);
   digitalWrite(latchPin, HIGH);
}
void updateright() //由右至左副程式 LSBFIRST
{
   digitalWrite(latchPin, HIGH);
   shiftOut(dataPin, clockPin, LSBFIRST, leds);
   digitalWrite(latchPin, LOW);
}
void LEDWaterLamp() 
{
    int inByte = Serial.read();
    switch (inByte) {
    case '3':

    leds = 0; // 一開始燈全滅，所以設為0
    updateShiftRegister();  //進行送資料的副程式
    delay(500);
    for (int i = 0; i < 8; i++) //依序亮燈
    {
        bitSet(leds, i);    // 經由多少燈要亮的i，換算成10進位，並把結果交給leds
        updateShiftRegister(); //進行送資料的副程式
        delay(200);
       
    }break;
    case '4':
    leds = 0;
    updateright(); //進行送資料的副程式
    for (int k = 0; k < 9; k++) //依序亮燈
    {
        bitSet(leds, k);    // 經由多少燈要亮的k，換算成10進位，並把結果交給leds
        updateright(); //進行送資料的副程式
        delay(200);
  } break;
    case '5':
    digitalWrite(latchPin, LOW);  // 送資料前要先把 latchPin 設成低電位
    shiftOut(dataPin, clockPin, LSBFIRST, 0);  //送出資料，0就是2進位的00000000  
    digitalWrite(latchPin, HIGH); // 送完資料後要把 latchPin 設成高電位
  }
}

//--------------程式初始化-------------------------
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
SerialAnalyze=1;
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
}
//---------------主程式---------------------------
void loop() {
 //UartReceive();
 LEDWaterLamp(); //LED 流水燈

}