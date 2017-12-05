/*
 * Create By You Gang Kuo, Thunder Tiger
 * 
*/
#include <LRemote.h>
#include <Servo.h>

#define PWM_PIN 2 
#define SERVO_PIN 3

LRemoteCircleButton btnAcc;
LRemoteCircleButton btnBackward ;
LRemoteCircleButton btnDrift ;

LRemoteSlider slWheel;
LRemoteLabel label;
LRemoteSwitch switchButton;

Servo servoWheel;
Servo acc ;

LRemoteButton btnAccArray[5] ;
LRemoteButton btnBackArray[5] ;

void setup() {
  Serial.begin(9600);
  
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  
  servoWheel.attach(SERVO_PIN) ;
  acc.attach( PWM_PIN ) ;
  
  Serial.println("Start Thunder Tiger Linkit 7697 Car");

  // Initialize GPIO
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, 0);

  // Setup the Remote Control's UI canvas
  LRemote.setName("Thunder Tiger BLE Car");
  LRemote.setOrientation(RC_PORTRAIT);
  LRemote.setGrid(5, 4);

  
  // Add a simple text label
  label.setText("BLE RC Car By Linkit 7697");
  label.setPos(0, 0);
  label.setSize(5, 1);
  label.setColor(RC_GREY);
  LRemote.addControl(label);


  // Add a slider
  slWheel.setText("Servo");
  slWheel.setPos(0, 1);
  slWheel.setSize(2, 1);
  slWheel.setColor(RC_ORANGE);
  slWheel.setValueRange(1000, 2000, 1500);
  LRemote.addControl(slWheel);

  // Add Acc
  btnAcc.setText("Acc");
  btnAcc.setPos(2, 1);
  btnAcc.setSize(1, 1);
  btnAcc.setColor(RC_GREEN);
  LRemote.addControl(btnAcc);


  btnBackward.setText("Back");
  btnBackward.setPos(3, 1);
  btnBackward.setSize(1, 1);
  btnBackward.setColor(RC_PINK);
  LRemote.addControl(btnBackward);

  btnDrift.setText("Drift");
  btnDrift.setPos(4, 1);
  btnDrift.setSize(1, 1);
  btnDrift.setColor(RC_BLUE);
  LRemote.addControl(btnDrift);

  String accTitle[] = { "←", "↖", "↑", "↗", "→" };
  String backTitle[] = { "←", "↙", "↓", "↘", "→" } ;

  for ( int i = 0 ; i < 5 ; i++ ) {
      btnAccArray[i].setText( accTitle[i] );
      btnAccArray[i].setPos(i, 2);
      btnAccArray[i].setSize(1, 1);
      btnAccArray[i].setColor(RC_GREEN);
      LRemote.addControl(btnAccArray[i]);

      btnBackArray[i].setText( backTitle[i] );
      btnBackArray[i].setPos(i, 3);
      btnBackArray[i].setSize(1, 1);
      btnBackArray[i].setColor(RC_PINK);
      LRemote.addControl(btnBackArray[i]);
  }
  
  LRemote.begin();
  Serial.println("begin() returned");

  setupESC() ;
}

void setupESC() {
    // setup ESC
  acc.writeMicroseconds(0) ;
  delay(500) ;
  acc.writeMicroseconds(2000) ;
  delay(100) ;
  acc.writeMicroseconds(1000) ;
  delay(100) ;
  acc.writeMicroseconds(1500) ;
  delay(100) ;
}

void runDrift() {
  
  servoWheel.writeMicroseconds(1500) ;
  acc.writeMicroseconds(1300) ;
  delay(600) ;
  acc.writeMicroseconds(1500) ;
  delay(100) ;
  servoWheel.writeMicroseconds(1050) ;
  acc.writeMicroseconds(2000) ;
  delay(1000) ;
  acc.writeMicroseconds(1400) ;
  delay(200) ;
}


int temp = 0;
boolean isPress = false ;
  
void loop() {

  isPress = false ;

  // check if we are connect by some 
  // BLE central device, e.g. an mobile app
  if(!LRemote.connected()) {
    Serial.println("waiting for connection");
    delay(1000);
    acc.writeMicroseconds(1500) ; // if dissconnect stop motor
    return ; 
  } else {
    delay(50);
  }
  
  LRemote.process();

  if ( btnAcc.getValue() == 1 ) {
    acc.writeMicroseconds(1750) ;
    isPress = true ;
  }
  else if ( btnBackward.getValue() == 1) {
    acc.writeMicroseconds(1250) ;
    isPress = true ;
  }
  else if ( btnDrift.getValue() == 1 ) {
    // process drift ;
    runDrift() ;
  }
  else {

    // check is array @@
    for ( int i = 0 ; i < 5; i++ ) {
      if ( btnAccArray[i].getValue() == 1 ) {
        temp = (i - 2) * 200 ;
        servoWheel.writeMicroseconds( 1500 + temp ) ;
        acc.writeMicroseconds(1750) ;
        isPress = true ;
        break ;
      }
      else if ( btnBackArray[i].getValue() == 1 ) {
        temp = (i - 2) * 200 ;
        servoWheel.writeMicroseconds( 1500 + temp ) ;
        acc.writeMicroseconds(1250) ;
        isPress = true ;
        break ;
      }
    }


    // show led if press
    if ( isPress == false ) {
      acc.writeMicroseconds(1500) ;
      digitalWrite(7, true);
    }
    else {
      digitalWrite(7, false);
    }
  }
  
  if(slWheel.isValueChanged()){
    servoWheel.writeMicroseconds( slWheel.getValue() ) ;
  }
}
