/////////74HC595腳位////////////////////////////
#define data_pin 2  // 74HC595序列輸入
#define clk_pin 3  // 74HC595序列時脈
#define latch_pin 4  // 74HC595暫存器時脈
/////////按鍵腳位///////////////////////////////
#define KEY_COUNT_PIN 10
#define KEY_PAUSE_PIN 11
#define KEY_CLEAR_PIN 12
//////////////////////////////////////////////
int value=0;

unsigned long previousMillis=0; //millis()取代delay
//===========================================================================
const byte SEG_PIN[2]={8,9}; // 7段顯示器的控制接腳
byte digits[2]={5,5}; //七段顯示器顯示數字
int seg_7[10]= //陣列 

{    //建立的0-9參數，改為16進位數值表示
  0xfc,    //0 11111100  
  0x60,    //1 01100000
  0xda,    //2 11011010
  0xf2,    //3 11110010
  0x66,    //4 01100110
  0xb6,    //5 10110110
  0xbe,    //6 10111110
  0xe0,    //7 11100000
  0xfe,    //8 11111110 
  0xf6     //9 11110110
};
//==========================================================================


bool getCountButton() {
  return not(digitalRead(KEY_COUNT_PIN));
}

bool getPauseButton() {
  return not(digitalRead(KEY_PAUSE_PIN));
}

bool getClearButton() {
  return not(digitalRead(KEY_CLEAR_PIN));
}

void display(int number){
  digits[1]=number % 10;    // 取得個位數字
  digits[0]=number / 10 % 10;    // 取得十位數字
  byte num;
  // 逐一設定每個七段顯示器
  for(int i=0;i<2;i++)  
  {
    digitalWrite(latch_pin,LOW);   // 送資料前要先把 latchPin 拉成低電位
    shiftOut(data_pin,clk_pin,LSBFIRST,seg_7[digits[i]]);
    digitalWrite(latch_pin,HIGH); // // 送完資料後要把 latchPin 拉回成高電位
    digitalWrite(SEG_PIN[i],LOW);   // 點亮此顯示器
    delay(5); // 暫停
    for(int i=0;i<2;i++){
    digitalWrite(SEG_PIN[i],HIGH);  // 關閉所有顯示器
    }    
  }
}

void setup() {
  Serial.begin(9600);
  // 將 latchPin, clockPin, dataPin 設置為輸出
  pinMode(2,OUTPUT);    //data pin // 74HC595序列輸入
  pinMode(3,OUTPUT);    //data clk // 74HC595序列時脈
  pinMode(4,OUTPUT);    //sto clk // 74HC595暫存器時脈
  
  pinMode(KEY_COUNT_PIN,INPUT);    
  pinMode(KEY_PAUSE_PIN,INPUT);    
  pinMode(KEY_CLEAR_PIN,INPUT);    
  
  
   for(int i=0;i<2;i++){
    pinMode(SEG_PIN[i],OUTPUT);
    digitalWrite(SEG_PIN[i],HIGH);
  }
}

void loop() {
  if(Serial.available() > 0) {
    display(11);
    value=Serial.parseInt();
    switch(value){
      case 1:
        startCounter();
      break;
    
      case 2:
        pauseCounter();
      break;

      case 3:  
        clearCounter();
      break;
    }
  }

  if(getCountButton()) { //開始
    startCounter();
  }
  if(getPauseButton()) { //暫停
    pauseCounter();
  }
  if(getClearButton()) { //清除
    clearCounter();
  }

  int count = getCounterValue();
  // Serial.println(count);
  // display(count);
}

/////////////////////////計時器/////////////////////////
const unsigned long counterInterval = 200; //計時器的+1速度(ms)
//////////////////////下面都不能改/////////////////////////////
bool counterCounting = false;
int counterCount = 0;
unsigned long counterStartTime = millis();
//開始計時器
void startCounter() {
  if(not(counterCounting)) {
    counterCounting = true;
    counterStartTime = millis();
  }
}
//暫停計時器
void pauseCounter() {
  counterCount = getCounterValue();
  counterCounting = false;
}
//清除計時器
void clearCounter() {
  pauseCounter();
  counterCount = 0;
}
//獲取計時器數值
int getCounterValue() {
  int count = 0;
  if(counterCounting) {
    count = (millis() - counterStartTime) / counterInterval;
    count += counterCount;
  }else{
    count = counterCount;
  }
  return count;
}
/////////////////////////計時器/////////////////////////
