int buzz = 2;
int LED1 = 14;
int LED2 = 12;
int LED3 = 13;
int LED4 = 15;
void setup() {

// initialize GPIO 5 as an output

pinMode(LED1, OUTPUT);
pinMode(LED2, OUTPUT);
pinMode(LED3, OUTPUT);
pinMode(LED4, OUTPUT);
pinMode(buzz, OUTPUT);

}

// the loop function runs over and over again forever

void loop() {

digitalWrite(LED1, HIGH);
digitalWrite(LED2, HIGH);
digitalWrite(LED3, HIGH);
digitalWrite(LED4, HIGH);
// turn the LED on
digitalWrite(buzz, HIGH);
delay(1000); // wait for a second
digitalWrite(LED1, LOW); // turn the LED off
digitalWrite(LED2, LOW);
digitalWrite(LED3, LOW); // turn the LED off
digitalWrite(LED4, LOW);
digitalWrite(buzz, LOW);
delay(1000); // wait for a second

}
