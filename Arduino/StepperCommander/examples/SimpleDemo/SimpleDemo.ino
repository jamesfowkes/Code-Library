#include <steppercommander.h>

static void run(int value);
static void stop(int value);
static void setSpeed(int value);
static void setRate(int value);
static void step(int value);

static void run(int value) { 
  (void)value; 
  Serial.println("Run"); 
}
static void stop(int value) { 
  (void)value; 
  Serial.println("Stop"); 
}
static void setSpeed(int value){ 
  Serial.print("Set Speed "); 
  Serial.println(value); 
}
static void setRate(int value){ 
  Serial.print("Set Rate "); 
  Serial.println(value); 
}
static void step(int value){ 
  Serial.print("Step "); 
  Serial.println(value); 
}

static COMMAND_HANDLER s_handlers[] = {
  stop,
  run,
  setRate,
  setSpeed,
  step
};

static  StepperCommander StepperCommander(s_handlers);

char inChars[10];         // an array to hold incoming data
boolean stringComplete = false;  // whether the string is complete

void setup(void)
{
  Serial.begin(115200);
}

void loop(void)
{
  if (stringComplete) {
    stringComplete = false;
    StepperCommander.HandlePacket(inChars);
  }
}

void serialEvent() {
  static int i = 0;
  while (Serial.available()) {
    // get the new byte:
    inChars[i] = (char)Serial.read(); 
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChars[i] == '\n') {
      stringComplete = true;
      i = 0;
    }
    else
    {
      if (++i == 7) { 
        i == 0; 
      }
    }
  }
}




