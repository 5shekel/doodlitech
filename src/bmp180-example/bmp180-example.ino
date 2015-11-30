//based on https://github.com/telavivmakers/BMP180_Breakout_Arduino_Library/tree/master/examples/SFE_BMP180_example

#include <SFE_BMP180.h>
#include <Wire.h>

// You will need to create an SFE_BMP180 object, here called "pressure":

SFE_BMP180 pressure;

#define ALTITUDE 25.0 // Altitude of SparkFun's HQ in Boulder, CO. in meters

void setup()
{
  Serial.begin(9600);
  Serial.println("REBOOT");
  if (pressure.begin())
    Serial.println("BMP180 init success");
  else
  {
    // If you want sea-level-compensated pressure, as used in weather reports,
    // you will need to know the altitude at which your measurements are taken.
    // We're using a constant called ALTITUDE in this sketch:

    Serial.println();
    Serial.print("provided altitude: ");
    Serial.print(ALTITUDE, 0);
    Serial.print(" meters, ");
    Serial.print(ALTITUDE * 3.28084, 0);
    Serial.println(" feet");

    Serial.println("BMP180 init fail\n\n");
    while (1); // Pause forever.
  }
}

void loop()
{
  char status;
  double T, P, p0, a;

  // If you want to measure altitude, and not pressure, you will instead need
  // to provide a known baseline pressure. This is shown at the end of the sketch.

  // You must first get a temperature measurement to perform a pressure reading.

  // Start a temperature measurement:
  // If request is successful, the number of ms to wait is returned.
  // If request is unsuccessful, 0 is returned.

  status = pressure.startTemperature();
  if (status != 0)
  {
    // Wait for the measurement to complete:
    delay(status);

    // Retrieve the completed temperature measurement:
    // Note that the measurement is stored in the variable T.
    // Function returns 1 if successful, 0 if failure.

    status = pressure.getTemperature(T);
    if (status != 0)
    {
      // Print out the measurement:
      //Serial.print("temperature: ");
      //Serial.print(T, 2);
      //Serial.print(" deg C, ");

      // Start a pressure measurement:
      // The parameter is the oversampling setting, from 0 to 3 (highest res, longest wait).
      // If request is successful, the number of ms to wait is returned.
      // If request is unsuccessful, 0 is returned.

      status = pressure.startPressure(3);
      if (status != 0)
      {
        // Wait for the measurement to complete:
        delay(status);

        // Retrieve the completed pressure measurement:
        // Note that the measurement is stored in the variable P.
        // Note also that the function requires the previous temperature measurement (T).
        // (If temperature is stable, you can do one temperature measurement for a number of pressure measurements.)
        // Function returns 1 if successful, 0 if failure.

        status = pressure.getPressure(P, T);
        if (status != 0)
        {
          // Print out the measurement:
          //Serial.print("absolute pressure: ");
          Serial.println((P-1000)*100., 2);
          //Serial.print(" mb, ");
        //  Serial.print(P * 0.0295333727, 2);
         // Serial.println(" inHg");

          /*
            // The pressure sensor returns abolute pressure, which varies with altitude.
            // To remove the effects of altitude, use the sealevel function and your current altitude.
            // This number is commonly used in weather reports.
            // Parameters: P = absolute pressure in mb, ALTITUDE = current altitude in m.
            // Result: p0 = sea-level compensated pressure in mb

            p0 = pressure.sealevel(P, ALTITUDE); // we're at 1655 meters (Boulder, CO)
            Serial.print("relative (sea-level) pressure: ");
            Serial.print(p0, 2);
            Serial.print(" mb, ");
            Serial.print(p0 * 0.0295333727, 2);
            Serial.println(" inHg");

            // On the other hand, if you want to determine your altitude from the pressure reading,
            // use the altitude function along with a baseline pressure (sea-level or other).
            // Parameters: P = absolute pressure in mb, p0 = baseline pressure in mb.
            // Result: a = altitude in m.

            a = pressure.altitude(P, p0);
            Serial.print("computed altitude: ");
            Serial.print(a, 0);
            Serial.print(" meters, ");
            Serial.print(a * 3.28084, 0);
            Serial.println(" feet");
          */
        }
        else Serial.println("error retrieving pressure measurement\n");
      }
      else Serial.println("error starting pressure measurement\n");
    }
    else Serial.println("error retrieving temperature measurement\n");
  }
  else Serial.println("error starting temperature measurement\n");

  delay(100);  // Pause for 5 seconds.
}


/*
//the example code for processing to graph a println
// in 1.6.5 for graph doesnt work. this those i found it in the arduino.cc forums
 import processing.serial.*;

Serial myPort;        // The serial port
int xPos = 1;         // horizontal position of the graph

float fValue;
boolean newVal = false;

void setup () {
  size(400, 300);

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
    if (++xPos >= width) {
      xPos = 0;
      background(0);
    }
    newVal = false;
  }
}

void serialEvent (Serial myPort) {
  String inString = myPort.readStringUntil('\n');
  if (inString != null) {
    inString = trim(inString);
    fValue = float(inString);
    fValue = map(fValue, 1500, 2000, 0, height);
    newVal = true;
  }
}
 */
