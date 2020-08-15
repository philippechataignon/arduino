class Button {
    int btnPin;      // the number of the LED pin
    long OnTime;     // milliseconds of on-time
     
    // These maintain the current state
    int btnState;             		// btnState used to set the LED
    unsigned long previousMillis;  	// will store last time LED was updated
     
  // Constructor - creates a Flasher 
  // and initializes the member variables and state
  public:
  Button(int pin, long on) {
    btnPin = pin;
    pinMode(btnPin, INPUT_PULLUP);     
    	  
    OnTime = on;
    btnState = LOW; 
    previousMillis = 0;
  }
     
  void Update() {
    // check to see if it's time to change the state of the LED
    unsigned long currentMillis = millis();
    if(digitalRead(btnPin) == LOW) {
        btnState = HIGH; 
        previousMillis = currentMillis;  // Remember the time
    } 
         
    if((btnState == HIGH) && (currentMillis - previousMillis >= OnTime)) {
        btnState = LOW; 
        previousMillis = currentMillis;  // Remember the time
    } 
  }

  int getState() {
    return btnState;
  }
};


class Flasher {
    // Class Member Variables
    // These are initialized at startup
    int ledPin;      // the number of the LED pin
    long OnTime;     // milliseconds of on-time
    long OffTime;    // milliseconds of off-time
     
    // These maintain the current state
    int ledState;             		// ledState used to set the LED
    unsigned long previousMillis;  	// will store last time LED was updated
     
  // Constructor - creates a Flasher 
  // and initializes the member variables and state
  public:
  Flasher(int pin, long on, long off) {
    ledPin = pin;
    pinMode(ledPin, OUTPUT);     
    	  
    OnTime = on;
    OffTime = off;
    	
    ledState = LOW; 
    previousMillis = 0;
  }
     
  void Update() {
    // check to see if it's time to change the state of the LED
    unsigned long currentMillis = millis();
         
    if((ledState == HIGH) && (currentMillis - previousMillis >= OnTime)) {
        ledState = LOW;  // Turn it off
      previousMillis = currentMillis;  // Remember the time
      digitalWrite(ledPin, ledState);  // Update the actual LED
    } else if ((ledState == LOW) && (currentMillis - previousMillis >= OffTime)) {
      ledState = HIGH;  // turn it on
      previousMillis = currentMillis;   // Remember the time
      digitalWrite(ledPin, ledState);	  // Update the actual LED
    }
  }
};
     
Flasher led1(13, 350, 350);
// Flasher led2(12, 100, 400);
Button btn1(10, 2000);
     
void setup() {
    pinMode(12, OUTPUT);     
}
     
void loop() {
    led1.Update();
    digitalWrite(12, btn1.getState());
//    led2.Update();
    btn1.Update();
}
