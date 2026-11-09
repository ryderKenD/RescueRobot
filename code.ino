#include <QTRSensors.h>
QTRSensors qtr;
const uint8_t SensorCount = 4;
uint16_t sensorValues[SensorCount];
const int S0 = A2;
const int S1 = A1;
const int S2 = A4;
const int S3 = A3;
const int OE = A0;
const int sensorOut = A5;
const int In1 = 7;
const int In2 = 13;
const int In3 = 12;
const int In4 = 4;
const int EnA = 6;
const int EnB = 5;
int blueMin = 20;
int redMin = 20;
int greenMin = 20;
int redMax = 250;
int greenMax = 250;
int blueMax = 250;
int redValue, greenValue, blueValue;
int lineValue;

void setup() {
  // put your setup code here, to run once:
  qtr.setTypeRC();
  qtr.setSensorPins((const uint8_t[]){1, 2, 3, 9}, SensorCount);
  for (int i = 0; i <250; i++){
    qtr.calibrate();
  }
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(OE, OUTPUT);
  pinMode(sensorOut, INPUT);
  pinMode(In1, OUTPUT);
  pinMode(In2, OUTPUT);
  pinMode(In3, OUTPUT);
  pinMode(In4, OUTPUT);
  // Setting frequency-scaling to 20%
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
  digitalWrite(OE, LOW);
  Serial.begin(9600);
}

void loop() {
  
  // Colour Sensor
  colourCheck();
  // this is where I WOULD have a function to check colour frequencies with the colour frequency values tested off of the colour swatch 
  // Line Sensor Array
  qtr.read(sensorValues);
  for (uint8_t i = 0; i < SensorCount; i++)
  {
    Serial.print(sensorValues[i]);
    Serial.print('\t');
  }
  lineValue = qtr.readLineBlack(sensorValues);
  Serial.print(lineValue);
  Serial.println();
  // Motors
  if (lineValue >= 1000 && lineValue <= 2000)
  {
    leftGo(255);
    rightGo(255);
  }
  else if (lineValue < 1000)
  {
    leftStop();
    rightGo(int map(lineValue, 0, 3000, 255, 0));
  }
  else 
  {
    rightStop();
    leftGo(map(lineValue, 0, 3000, 0, 255));
  }
  
}

void colourCheck()
{
  // red reading
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  int redFrequency = pulseIn(sensorOut, LOW);
  redValue = map(redFrequency, redMin, redMax, 255, 0);
  redValue = constrain(redValue, 0, 255);
  // green reading
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  int greenFrequency = pulseIn(sensorOut, LOW);
  greenValue = map(greenFrequency, greenMin, greenMax, 255, 0);
  greenValue = constrain(greenValue, 0, 255);
  // blue reading
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  int blueFrequency = pulseIn(sensorOut, LOW);
  blueValue = map(blueFrequency, blueMin, blueMax, 255, 0);
  blueValue = constrain(blueValue, 0, 255);
  // output
  Serial.print("r - ");
  Serial.print(redValue);
  Serial.print("   ");
  Serial.print("g - ");
  Serial.print(greenValue);
  Serial.print("   ");
  Serial.print("b - ");
  Serial.print(blueValue);
  Serial.print("   ");
}

void leftGo(int x)
{
  analogWrite(EnA, x);
  digitalWrite(In1, HIGH);
  digitalWrite(In2, LOW);
}

void leftStop()
{
  digitalWrite(In1, LOW);
  digitalWrite(In2, LOW);
}

void rightGo(int x)
{
  analogWrite(EnB, x);
  digitalWrite(In3, LOW); 
  digitalWrite(In4, HIGH);
}

void rightStop()
{
  digitalWrite(In3, LOW);
  digitalWrite(In4, LOW);
}
