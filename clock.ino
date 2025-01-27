#define clkp 23
#define resp 22
#define cycp 12
int delt = 10;


void cpuStart(){
  digitalWrite(resp, HIGH);
  for(int i = 0; i < 4; i++)
    cycle();    
}

void cycle(){
  digitalWrite(clkp, HIGH);
  delay(delt);
  digitalWrite(clkp, LOW);
  delay(delt);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(clkp, OUTPUT);
  pinMode(resp, OUTPUT);
  pinMode(cycp, INPUT_PULLDOWN);
  Serial.println("Connected, Reseting CPU; Address == FFFF0");
  cpuStart();

}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(cycp == HIGH)){
    cycle();
    delay(10);
  }
}


