#include <Servo.h>

#include <SparkFun_TB6612.h>


// Pins for all inputs, keep in mind the PWM defines must be on PWM pins
// the default pins listed are the ones used on the Redbot (ROB-12097) with
// the exception of STBY which the Redbot controls with a physical switch
#define AIN1 A2
#define BIN1 A3
#define AIN2 A1
#define BIN2 A4
#define PWMA 5
#define PWMB 4
#define STBY A0

#define AIN3 A9
#define BIN3 A10
#define AIN4 A8
#define BIN4 A11
#define PWMA2 3
#define PWMB2 2
#define STBY2 A12

#define MTR_MOVE 19
#define EXPLODE 18
#define BULLET 17

#define servoPin1 9
#define servoPin2 10
#define servoPin3 11

#define servoPinA 7
#define servoPinB 6

Servo myservo1;
Servo myservo2;
Servo myservo3;

Servo myservoA;
Servo myservoB;

// these constants are used to allow you to make your motor configuration 
// line up with function names like forward.  Value can be 1 or -1
const int offsetA = 1;
const int offsetB = 1;

const int offsetA2 = -1;
const int offsetB2 = -1;

// Initializing motors.  The library will allow you to initialize as many
// motors as you have memory for.  If you are using functions like forward
// that take 2 motors as arguements you can either write new functions or
// call the function more than once.
Motor motor1 = Motor(AIN1, AIN2, PWMA, offsetA, STBY);
Motor motor2 = Motor(BIN1, BIN2, PWMB, offsetB, STBY);


Motor motor3 = Motor(AIN3, AIN4, PWMA2, offsetA2, STBY2);
Motor motor4 = Motor(BIN3, BIN4, PWMB2, offsetB2, STBY2);

void setup()
{
 //Nothing here
 Serial.begin(9600); // open the serial port at 9600 bps:
 pinMode(MTR_MOVE, INPUT);
 pinMode(EXPLODE, INPUT);
 pinMode(BULLET, INPUT);

 bulletHole(0);
 explode(0);

}

void explode(int reset){

  int finish = 0;
  int start = 90;

  if (reset){
    start = 0;
    finish = 90;
  }


  myservoA.attach(servoPinA); // attaches the servo on pin 18 to the servo object
  myservoB.attach(servoPinB); // attaches the servo on pin 18 to the servo object

  delay(500); 
  // int pos = 0;
  // for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
  //   // in steps of 1 degree
  //   myservoA.write(abs(finish-pos)); //90
  //   myservoB.write(abs(start - pos)); 
  //   delay(2);                       // waits 15ms for the servo to reach the position
  // }

  myservoA.write(start);
  myservoB.write(finish);

  delay(1500); 
  myservoA.detach();
  myservoB.detach();

}


void leftRight(){
  forward(motor1, motor2, 150);
  forward(motor3, motor4, -150);
  delay(2000);
  forward(motor1, motor2, -150);
  forward(motor3, motor4, 150);
  delay(2000);
  forward(motor1, motor2, 150);
  forward(motor3, motor4, -150);
  delay(2000);
  forward(motor1, motor2, -150);
  forward(motor3, motor4, 150);
  delay(2000);
  brake(motor1, motor2);
  brake(motor3, motor4);
  
}

void bulletHole(int reset){

  int finish = 0;

  if (reset){

    finish = 90;    
  }


  myservo1.attach(servoPin1); // attaches the servo on pin 18 to the servo object
  myservo2.attach(servoPin2); // attaches the servo on pin 18 to the servo object
  myservo3.attach(servoPin3); // attaches the servo on pin 18 to the servo object
  delay(500); 
  int pos = 0; 
  // for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
  //   // in steps of 1 degree
  //   int angle = abs(finish-pos);
  //   myservo1.write(angle); 
  //   myservo2.write(angle); 
  //   myservo3.write(angle);              // tell servo to go to position in variable 'pos'
  //   delay(2);                       // waits 15ms for the servo to reach the position
  // }
  myservo1.write(finish); 
  myservo2.write(finish); 
  myservo3.write(finish); 
  delay(2000); 
  myservo1.detach();
  myservo2.detach();
  myservo3.detach();
}


void loop()
{

  if (digitalRead(BULLET)){
    bulletHole(1);
  }

  if (digitalRead(MTR_MOVE)){
    leftRight();
  }

  if (digitalRead(EXPLODE)){
    explode(1);
  }

}
