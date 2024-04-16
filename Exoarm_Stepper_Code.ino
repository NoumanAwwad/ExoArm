#include <AccelStepper.h>

// Defines the pins for stepper motor control
const int dirPin = 2;  // Direction pin
const int stepPin = 3; // Steps pin

// Setup for the stepper motor using a driver that requires a step and a direction pin
#define motorInterfaceType 1
#define flexSensorPin A3 // Analog pin connected to the flex sensor

// Creates an instance of the AccelStepper class to control the stepper motor
AccelStepper myStepper(motorInterfaceType, stepPin, dirPin);

// Flex sensor reading boundaries, these are subject to change depending on your flex sensors make and model
//It is recommended you run the program once with your flex sensor and replace these values depending on your bent and unbent values.
const int flexMin = 965;  // Minimum reading (fully bent)
const int flexMax = 980; // Maximum reading (fully unbent)

// MultiStepper allows multiple stepper motors to be synchronized
//This function is not used in this code, but if you wish to add more stepper motors for different joints you will need this library (otherwise you can remove it)
MultiStepper steppersControl;

void setup() {
  // Initialize the stepper motor's maximum speed
  myStepper.setMaxSpeed(400); // Maximum steps per second

  // Add the stepper to the MultiStepper control, only one stepper is used here
  steppersControl.addStepper(myStepper);
}

void loop() {
  // Reads the current value from the flex sensor
  int flexValue = analogRead(flexSensorPin);

  // Array to hold target position for the stepper motor
  long movetoposition[1];
  
  // Checks if the flex sensor value is bent
  if (flexValue > flexMin && flexValue > flexMax){
    delay(2000); // Delay for 2 seconds to debounce/stabilize sensor reading (ensuring the action was deliberate)
    // Recheck the condition to confirm the reading was a true/deliberate input
    if (flexValue > flexMin && flexValue > flexMax){
      movetoposition[0] = 1100;  // Sets the target position to 1100 steps
      steppersControl.moveTo(movetoposition); // Move to the specified position
      steppersControl.runSpeedToPosition(); // Execute the movement at previously set speed in line 25
    }
  // Checks if the flex sensor is unbent/straight  
  } else if (flexValue < flexMin && flexValue < flexMax){ 
    delay(2000); // Delay for stabilization once again
    // Confirm the condition as shown previously
    if (flexValue < flexMin && flexValue < flexMax){
      movetoposition[0] = 0; // Set target position to 0 steps (reset position)
      steppersControl.moveTo(movetoposition); // Move to the specified position
      steppersControl.runSpeedToPosition(); // Execute the movement
    }
  }

  // Print the current flex sensor value and the stepper motor's position to the Serial Monitor 
  //This will be where you get your flex sensor values from to change flexMin and flexMax
  Serial.print("Flex Sensor Value: ");
  Serial.print(flexValue);
  Serial.print(" - Motor Position: ");
  Serial.println(myStepper.currentPosition());
}