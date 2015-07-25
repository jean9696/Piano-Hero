
const int buttonPin = 2;     // the number of the pushbutton pin
const int buttonPin2 = 3;
const int buttonPin3 = 4;
const int buttonPin4 = 5;
const int buttonPin5 = 6;
const int buttonPin6 = 7;
const int buttonPin7 = 8;


const int ledPin =  13;      // the number of the LED pin
const int ledPin2 = 12;
const int ledPin3 = 11;
const int ledPin4 = 10;
const int ledPin5 = 9;



int buttonState = 0;         // variable for reading the pushbutton status
int buttonState2 = 0;
int buttonState3 =  0;
int buttonState4 =  0;
int buttonState5 =  0;
int buttonState6 =  0;
int buttonState7 =  0;

void setup() {

  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);  
  pinMode(buttonPin2, INPUT);
  pinMode(buttonPin3, INPUT);
  pinMode(buttonPin4, INPUT);
  pinMode(buttonPin5, INPUT);
    pinMode(buttonPin6, INPUT);
  pinMode(buttonPin7, INPUT);
  

Serial.begin(9600); // initialise la communication série
  // vérifier que le débit utiliser est le même dans le Terminal Série

 
   
}

void loop(){
  // read the state of the pushbutton value:
 buttonState = digitalRead(buttonPin);
 buttonState2 = digitalRead(buttonPin2);
 buttonState3 = digitalRead(buttonPin3);
 buttonState4 = digitalRead(buttonPin4);
 buttonState5 = digitalRead(buttonPin5);
  buttonState6 = digitalRead(buttonPin6);
 buttonState7 = digitalRead(buttonPin7);
 
 
 

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  
  if (buttonState == HIGH) {     

    Serial.print("1");
    
  } 

    if (buttonState2 == HIGH) {     

    Serial.print("2");
  } 

     if (buttonState3 == HIGH) {     

    Serial.print("3");
  } 
  else 
  {
   digitalWrite(ledPin3, LOW); 
  }
    if (buttonState4 == HIGH) {     

    Serial.print("4");
  } 

   if (buttonState5 == HIGH) {     

   Serial.print("5"); 
  } 

    if (buttonState6 == HIGH) {     

   Serial.print("6"); 
  } 

    if (buttonState5 == HIGH) {     
    
   Serial.print("7"); 
  } 
 
  

  Serial.println("\0");
 
  
   // delay(50);    
}
