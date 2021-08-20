unsigned char uart_receive_buf[12],uart_send_buf[3];
void setup() {
  Serial.begin(9600);
  uart_send_buf[0] = 0x41; 
  uart_send_buf[1] = 0x42; 
  uart_send_buf[2] = 0x43; 
  
}

void loop() {
    Serial.write(uart_send_buf,3);
    Serial.println();
    delay(3000);

}
