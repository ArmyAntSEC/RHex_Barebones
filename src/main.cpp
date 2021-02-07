#include <Arduino.h>

#define MOTOR_PWM 6
#define MOTOR_EN1 5
#define MOTOR_EN2 4
#define MOTOR_CS A0
#define OPTO 7

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
  pinMode ( MOTOR_PWM, OUTPUT );
  pinMode ( OPTO, INPUT );

  digitalWrite ( MOTOR_EN1, 0 );
  digitalWrite ( MOTOR_EN2, 1 );
  analogWrite ( MOTOR_PWM, 0 );

  attachInterrupt ( digitalPinToInterrupt(2), interruptHandlerOne, CHANGE );
  attachInterrupt ( digitalPinToInterrupt(3), interruptHandlerTwo, CHANGE );
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
    analogWrite( MOTOR_PWM, 0 );
    Serial.print( ", " );
    Serial.print ( 0 );
  } else { 
    analogWrite ( MOTOR_PWM, loopCount*16 );
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
    analogWrite( MOTOR_PWM, 0 );
}

