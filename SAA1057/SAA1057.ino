void clockOut() {
  digitalWrite(2, HIGH);
  delay(1);
  digitalWrite(2, LOW);
  delay(3);    
}

void initSaa(){
  for(int x=0; x<10; x++){
    clockOut();
  }  
}

void initbitsSaa(){
  digitalWrite(3, HIGH);
  delay(1);
  for(int x=0; x<16; x++){
    clockOut();  
  }
  digitalWrite(3, LOW);
}

void sendStartbits() {
  delay(10);
  digitalWrite(3, HIGH);
  delay(10);
  digitalWrite(2, 5 &  B01001000 );
  digitalWrite(3, LOW);
}

void setup() {
  // put your setup code here, to run once:
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  
  initSaa();
  initbitsSaa(); 
  //sendStartbits();
}

void loop() {
  // put your main code here, to run repeatedly:
  
}
