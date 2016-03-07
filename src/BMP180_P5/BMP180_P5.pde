//the example code for processing to graph a println
// in 1.6.5 for graph doesnt work. this those i found it in the arduino.cc forums
import processing.serial.*;

Serial myPort;        // The serial port
int xPos = 1;         // horizontal position of the graph

float fValue;
boolean newVal = false;

float minPressure = 9999;
float maxPressure = 0;
boolean erase;

void setup () {
  size(600, 400);

  // List all the available serial ports
  println(Serial.list());
  myPort = new Serial(this, Serial.list()[0], 9600);
  // don't generate a serialEvent() unless you get a newline character:
  myPort.bufferUntil('\n');
  background(40);
  stroke(127, 34, 255);
}

void draw () {
  if (newVal) {
    line(xPos, height, xPos, height - fValue);
    if (erase || ++xPos >= width) {
      xPos = 0;
      background(40);
      println("min="+minPressure+ " max="+maxPressure+" delta="+ (maxPressure - minPressure));
      erase=false;
    }
    newVal = false;
  }
}

void serialEvent (Serial myPort) {
  String inString = myPort.readStringUntil('\n');
  if (inString != null) {
    inString = trim(inString);
    fValue = float(inString);
    //println("val= "+fValue);  
    if (fValue>0) { //to deal with that Nan not  number error
    
      if (fValue < minPressure) {
        minPressure = fValue;
        erase = true;
      }else if (fValue > maxPressure) {
        maxPressure = fValue;
        erase = true;
      }
      fValue = map(fValue, minPressure, maxPressure, 0, height);
    }
    newVal = true;
  }
}