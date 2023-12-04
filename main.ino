# Source: https://howtomechatronics.com/projects/arduino-ant-hexapod-robot/

#include <Servo.h>
#include <SoftwareSerial.h>

#define trigPin 7
#define echoPin 6
#define ledB 10

SoftwareSerial Bluetooth(12, 9); // Arduino(RX, TX) - HC-05 Bluetooth (TX, RX)

// Create servo object
Servo s24;
Servo s23;
Servo s22;

void setup(){
  Serial.begin(38400);
  Bluetooth.begin(38400); //Default baud rate of the Bluetooth module
  Bluetooth.setTimeout(1);
  delay(20);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(ledB, OUTPUT);
  // Head
  s15.attach(36, 600, 2400);
  s14.attach(35, 600, 2400);
  s13.attach(34, 600, 2400); //grip
  // Tail
  s5.attach(26, 600, 2400); // Tail
  // Leg 4
  s10.attach(31, 600, 2400);
  s11.attach(32, 600, 2400);
  s12.attach(33, 600, 2400); //rot
  // Leg 5
  s7.attach(28, 600, 2400);
  s8.attach(29, 600, 2400);
  s9.attach(30, 600, 2400); //rot
  // Leg 6
  s1.attach(22, 600, 2400);
  s2.attach(23, 600, 2400);
  s3.attach(24, 600, 2400); //rot
  // Leg 1
  s18.attach(39, 600, 2400);
  s17.attach(38, 600, 2400);
  s16.attach(37, 600, 2400); //rot
  // Leg 2
  s21.attach(42, 600, 2400);
  s20.attach(41, 600, 2400);
  s19.attach(40, 600, 2400); //rot
  // Leg 3
  s24.attach(45, 600, 2400);
  s23.attach(44, 600, 2400);
  s22.attach(43, 600, 2400); //rot


  // == Move to initial position
  // Head
  s15.write(72);
  s14.write(50);
  s13.write(90); // Grip
  
  s5.write(65); // Tail
  
  // Leg 4
  s10.write(65);
  s11.write(35);
  s12.write(40);
  // Leg 5
  s7.write(80);
  s8.write(50);
  s9.write(25);
  // Leg 6
  s1.write(90);
  s2.write(45);
  s3.write(60);

  // Leg 1
  s18.write(60);
  s17.write(90);
  s16.write(100);
  // Leg 2
  s21.write(50);
  s20.write(85);
  s19.write(75);
  // Leg 3
  s24.write(50);
  s23.write(80);
  s22.write(80);
}

void moveLeg1() {
  // Swign phase - move leg through air - from initial to final position
  // Rise the leg
  if (i1L1 <= 10) {
    s18.write(60 - i1L1 * 2);
    s17.write(90 - i1L1 * 3);
    i1L1++;
  }
  // Rotate the leg
  if (i2L1 <= 30) {
    s16.write(100 - i2L1);
    i2L1++;

  }
  // Move back to touch the ground
  if (i2L1 > 20 & i3L1 <= 10) {
    s18.write(40 + i3L1 * 2);
    s17.write(60 + i3L1 * 3);
    i3L1++;
  }
  // Stance phase - move leg while touching the ground
  // Rotate back to initial position
  if (i2L1 >= 30) {
    s16.write(70 + i4L1);
    i4L1++;
    l1status = HIGH;
  }
  // Reset the counters for repeating the process
  if (i4L1 >= 30) {
    i1L1 = 0;
    i2L1 = 0;
    i3L1 = 0;
    i4L1 = 0;
    i5L1 = 0;
  }
  // Each iteration or step is executed in the main loop section where there is also a delay time for controlling the speed of movement
}


// Move forward
  if (m == 2) {
    moveLeg1();
    moveLeg3();
    moveLeg5();
    if (l1status == HIGH) {
      moveLeg2();
      moveLeg4();
      moveLeg6();
    }
  }


// Check for incoming data
  if (Bluetooth.available() > 0) {
    dataIn = Bluetooth.read();  // Read the data


// Monitor the battery voltage
    int sensorValue = analogRead(A3);
    float voltage = sensorValue * (5.00 / 1023.00) * 2.9; // Convert the reading values from 5v to suitable 12V i
    Serial.println(voltage);
    // If voltage is below 11V turn on the LED
    if (voltage < 11) {
      digitalWrite(ledB, HIGH);
    }
    else {
      digitalWrite(ledB, LOW);
    }


// Get the distance from the ultrasonic sensor
    if (getDistance() > 40) {
      att = 0;
    }
    if (getDistance() <= 40) {
      att = 1;
      dataIn = 99;
    }


// If there is an object in front of the sensor prepare for attack
  if (att == 1) {
    prepareAttack();
    if (aStatus == HIGH) {
      while (a == 0) {
        delay(2000);
        a = 1;
      }
      if (getDistance() > 30) {
        att = 2;
        a = 0;
        aStatus = LOW;
        initialPosHead();
      }
      if (getDistance() < 30) {
        att = 3;
        a = 0;
        aStatus = LOW;
        initialPosHead();
      }
    }
  }
  // If there is no longer object in front, dismiss the attack
  if (att == 2) {
    dismissAttack();
    if (aStatus == HIGH) {
      dataIn = 0;
      att = 0;
    }
  }
  // If there is closer to the sensor attack
  if (att == 3) {
    attack();
    if (attStatus == HIGH) {
      while (aa == 0) {
        delay(2000);
        aa = 1;
      } attStatus = LOW;
    }
    if (aStatus == HIGH) {
      while (a == 0) {
        delay(2000);
        a = 1;
      }
      dataIn = 0;
      att = 0;
      initialPosHead();
    }
  }
