/*
 * Rev 2 of ATTINY code
 * 
 * Additions include more error checking and decreased delay on the Main Loop
 */

bool targetConnected;
bool targetPower;
bool relayPower;

int targetConnectedPin = 3;
int targetPowerPin = 4;
int relayPin = 0;

//Size of Checksum Array
int CheckSize = 50;

void PrintStatus(){
  //Serial.println("--------------------------------------------------");
  //Serial.print("Target Connected: ");
  //Serial.println(targetConnected);
  //Serial.print("Target Power: ");
  //Serial.println(targetPower);
  //Serial.print("Relay Power: ");
  //Serial.println(relayPower);
}

void setup() {
  targetConnected = false;
  targetPower = false;
  relayPower = false;

  //Serial.begin(9600);
  //Serial.println("Isolation Dongle Program --- Version 1.0 --- Justin Hynes-Bruell");  
  //PrintStatus();
  
  delay(60);
  
  //Setup Input(s)
  pinMode(targetConnectedPin, INPUT);
  pinMode(targetPowerPin, INPUT);
  
  //Setup output(s)
  pinMode(relayPin, OUTPUT);
}

void loop() {
    
  //Read States first time
  targetConnected = digitalRead(targetConnectedPin);
  targetPower = !digitalRead(targetPowerPin);


  int checksumConnected[CheckSize];
  int checksumPowered[CheckSize]; 

  //Gather Numbers for cheksum
  for(int i=0; i < CheckSize; i++){
    //Serial.println("Checksum!");
    delay(7);
    checksumConnected[i] = digitalRead(targetConnectedPin);
    checksumPowered[i] = !digitalRead(targetPowerPin);
  }

  //Preform Checksum
  int sumConnected = 0;
  int sumPowered = 0;
  for(int i=0; i < CheckSize; i++){
    sumConnected += checksumConnected[i];
    sumPowered += checksumPowered[i];
  }

  //Check for consistency
  if((sumConnected == CheckSize || sumConnected == 0) && (sumPowered == CheckSize || sumPowered == 0)){

   if(sumConnected == CheckSize){
    targetConnected = true;
   }
   else if(sumConnected == 0){
    targetConnected = false;
   }
   if(sumPowered == CheckSize){
    targetPower = true;
   }
   else if(sumPowered == 0){
    targetPower = false;
   }

    
    //Target is connected and has power
    if(targetConnected == true && targetPower == true) {
      //Serial.println("Target is connected and has power");
      //Do nothing
    }
    
    //Target is connected and has no power
    else if(targetConnected == true && targetPower == false) {
      //Serial.println("Target is connected and has no power");
      //Turn on Relay
      relayPower = true;
    }

    //Target is not connected and has power
    else if(targetConnected == false && targetPower == true) {
      //Serial.println("Target is not connected and has power");
      //TURN RELAY OFF!
      relayPower = false;
    }

    //Target is not connected and has no power
    else if(targetConnected == false && targetPower == false) {
      //Serial.println("Target is not connected and has no power");
      //TURN RELAY OFF!
      relayPower = false;
    }

    digitalWrite(relayPin, relayPower);
    PrintStatus();
  }
  else {
    
      //Serial.println("ERROR!!! State Not Found!");
  }
  

}
