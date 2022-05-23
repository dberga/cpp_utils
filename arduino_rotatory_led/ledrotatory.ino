/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
 
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 3;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  Serial.begin(9600);
  pinMode(A0, INPUT);   
  pinMode(led, OUTPUT);   
Serial.println("SETUP FINISHED");  
}

// the loop routine runs over and over again forever:
void loop() {
  
  int sensorvalue = analogRead(A0);
  Serial.println("Sensor value =");
  Serial.println(sensorvalue);
  int outval = map(sensorvalue,0,1023,0,255);
  analogWrite(led, outval);   // turn the LED on (HIGH is the voltage level)

  //digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  //delay(1000);               // wait for a second
  //Serial.println("OFF"); 
}
