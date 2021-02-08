#include <Arduino.h>
#include <pwm_lib.h>
using namespace arduino_due::pwm_lib;

#define MOTOR_EN1 48
#define MOTOR_EN2 49
#define MOTOR_CS A5
#define ENCODER_1 50
#define ENCODER_2 51
#define OPTO 52
pwm<pwm_pin::PWML3_PC8> MOTOR_PWM;


long int numberOfClicksOne = 0;
long int numberOfClicksTwo = 0;

void interruptHandlerOne(void)
{
  noInterrupts();
  numberOfClicksOne++;
  interrupts();
}

void interruptHandlerTwo(void)
{
  noInterrupts();
  numberOfClicksTwo++;
  interrupts();
}


void setup()
{
  Serial.begin(115200);
  Serial.println ( "Hello World!" );
  pinMode ( MOTOR_EN1, OUTPUT );
  pinMode ( MOTOR_EN2, OUTPUT );
  pinMode ( MOTOR_CS, INPUT );
  pinMode ( OPTO, INPUT );

  MOTOR_PWM.start ( 4096, 0 );
 
  digitalWrite ( MOTOR_EN1, 0 );
  digitalWrite ( MOTOR_EN2, 1 );

  attachInterrupt ( digitalPinToInterrupt(ENCODER_1), interruptHandlerOne, CHANGE );
  attachInterrupt ( digitalPinToInterrupt(ENCODER_2), interruptHandlerTwo, CHANGE );
}

int loopCount = 0;

void loop()
{
  delay(1000);

  noInterrupts();
  long int one = numberOfClicksOne;
  long int two = numberOfClicksTwo;
  interrupts();

  Serial.print ( "Beep: " );
  int current = analogRead ( MOTOR_CS );
  Serial.print ( current );

  if ( loopCount >= 16 ) {
    MOTOR_PWM.set_duty( 0 );
    Serial.print( ", " );
    Serial.print ( 0 );
  } else { 
    MOTOR_PWM.set_duty( loopCount*256 );
    Serial.print( ", " );
    Serial.print ( loopCount*16 );
  }

  Serial.print ( ", " );
  Serial.print ( one );
  Serial.print ( ", " );
  Serial.print ( two );
  Serial.print ( ", " );
  Serial.print ( one - two );
  Serial.print ( ", " );
  Serial.print ( digitalRead ( OPTO ) );
  Serial.println();
  
  
  if ( loopCount++ >= 15 )
    MOTOR_PWM.set_duty( 0 );
}

