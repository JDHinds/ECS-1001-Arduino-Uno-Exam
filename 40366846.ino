int value;
int timePassed = 0;

void setup() {
  
  pinMode(A0, INPUT); //potentiometer
  pinMode(12, OUTPUT); //Red LED
  pinMode(13, OUTPUT); //Blue LED

  Serial.begin(9600); //Open serial port
  Serial.print("Please enter a new target value in the range (10-90)?");
  
  while (value < 10 || value > 90)                                     //While loop is required because Serial.available doesn't execute, it reads, so it needs to keep reading until we are ready to start loop.
                                                                       //This code isn't in loop because once we leave, we don't need to revisit it; so it belongs in setup, it just needs to loop for a wee bit to work.
  {
    if(Serial.available() > 0)                                         //Only read a value when there is a value to read
    {
      String input = Serial.readString();                              //Read the value
      input.trim();                                                    //Just good practice to remove any leading or trailing spaces
      //Serial.print("\nString recieved: " + input);                   //This line was for debugging, can be uncommented if need be
      value = input.toInt();                                           //Convert the read value from a string to an int - to use in the sums later
      //Serial.print("\nString converted: " + String(value));          //Again, for debugging
    
      if (value < 10 || value > 90)                                    //Checking if the value is in the specified range
      {
        digitalWrite(12, HIGH);                                        //Flash LED
        delay(300);                                                    //Probably should have gone in a method, but since it only executes once, here, it's fine
        digitalWrite(12, LOW);
        delay(300);
        digitalWrite(12, HIGH);
        delay(300);
        digitalWrite(12, LOW);
      }
      else
      {
        Serial.print("\nPlease adjust the potentiometer to the new target value (" + String(value) + ")"); //The message to signal to the user that the loop function has started.
      }
    }
  }  
}

void loop() {
  int potentiometerValue = map(analogRead(A0), 0, 1023, 0, 100);       //Read the potentiometer value in
  
  if (potentiometerValue < value)                                      //If it's less than it has to be, turn Red off and Blue on, and reset the 3-second counter
  {
    digitalWrite(12, LOW);
    digitalWrite(13, HIGH);
    timePassed = 0;
  }
  else if (potentiometerValue > value)                                 //If it's more than it has to be, turn Red on and Blue off, and reset the 3-second counter
  {
    digitalWrite(12, HIGH);
    digitalWrite(13, LOW);
    timePassed = 0;
  }
  else if (timePassed == 3000)                                        //Only if **exactly** 3 seconds have passed, print the target found message to the Serial port.
  {
    Serial.print("\n\n ****Target found ****");
    timePassed += 1;                                                  //Time passed still needs to be added to, otherwise this block will keep executing until the cows come home
                                                                      //By adding one, we ensure it doesn't execute again unless 3 is passed (by setting timePassed to 0 again, and finding the target again)
  }      
  else                                                                //If the target is one, we add one to time passed, and delay one, to simulate keeping track of a millisecond.
  {
    digitalWrite(12, LOW);                                            // Both LEDs are set off in this state
    digitalWrite(13, LOW);
    timePassed += 1;                                                  //There's probably better (and more accurate) ways to time stuff in arduino c (this timer assumes code takes 0 time to run, which it does not).
                                                                      //On the other hand, there's half an hour left, I'm tired, and this works close enough, unless you count mississippily.
  }
  delay(1);  
}
