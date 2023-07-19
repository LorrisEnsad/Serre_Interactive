// Code obsolete, dernier upload sur le micro controlleur avant la soutenance 
// Compile mais de fonctionne pas : probleme de branchement ? de code ? Gros debuggage necessaire
// Nécessite la bibliothque interacition.h et interaction.cpp pour fonctionner
// Les instructions Servo moteurs sont commentées. 
// Utilisation de la classe Array non native en C++ Arduino pose peut-être probleme. Changer pour les Arrays Natifs lorsqu'essentiel, sinon eviter l'utilisation de tableaux
// Faire une version fonctionelle et pas s'embêter ?


#include <Servo.h>
#include <Array.h>
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <DFRobot_DF1201S.h>
#include <SoftwareSerial.h>
#include <interaction.h>
#include <Uduino_Wifi.h>


#define LEDPIN 12
#define NUMPIXELS 6
#define NUMINTERACTIONS 6
#define VOLUME 20

#define BREATHINGTIME 120

#define SERVOPIN1 5
#define SERVOPIN2 5
#define SERVOPIN3 5
#define SERVOPIN4 5

//Declaration Unity communication
Uduino_Wifi uduino("uduinoBoard");

//Declaration Leds
Adafruit_NeoPixel pixels(NUMPIXELS, LEDPIN, NEO_GRB + NEO_KHZ800);

//Declaration soundchip
SoftwareSerial DF1201SSerial(3, 1);  //RX  TX
DFRobot_DF1201S DF1201S;

//Declaration Servo
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

//Declarations additionnelles
bool completed_array[NUMINTERACTIONS] = { false, false, false, false, false, false };

int completed_compte = 0;

interaction interactions_liste[NUMINTERACTIONS] = { interaction(0, 6, LEDPIN),
                                                    interaction(1, 7, LEDPIN),
                                                    interaction(2, 11, LEDPIN),
                                                    interaction(3, 8, LEDPIN),
                                                    interaction(4, 9, LEDPIN),
                                                    interaction(5, 10, LEDPIN) };  //change to arduino synthaxe : Array<tout pareil>

Array<int, 3> c;  //change arduino synthaxe ? semblerait que ça existe quand même

String audio_path = "";  //arduino change la syhtaxe String audio_path = '';

int breath_C_target[3] = { 150, 10, 150 };


// int servo_middles[4] = {88,90,90,90};
bool servo_lock = true;
// int servo_speed[4] = {80,80,80,80};

void lock_servo()
{
  if (!servo_lock)
  {
    ;
    // servo1.write(0);
    // servo2.write(0);
    // servo3.write(0);
    // servo4.write(0);
    //for continuous servo
    // servo1.write(servo_middles[0]+servo_speed[0]);
    // servo2.write(servo_middles[1]+servo_speed[1]);
    // servo3.write(servo_middles[2]+servo_speed[2]);
    // servo4.write(servo_middles[3]+servo_speed[3]);
    // delay(300);
    // servo1.write(servo_middles[0]);
    // servo2.write(servo_middles[1]);
    // servo3.write(servo_middles[2]);
    // servo4.write(servo_middles[3]);
  } 
}

void unlock_servo()
{
  if (servo_lock)
  {
    // servo1.write(180);
    // servo2.write(180);
    // servo3.write(180);
    // servo4.write(180);
    //for continuous servo
    // servo1.write(servo_middles[0]-servo_speed[0]);
    // servo2.write(servo_middles[1]-servo_speed[1]);
    // servo3.write(servo_middles[2]-servo_speed[2]);
    // servo4.write(servo_middles[3]-servo_speed[3]);
    // delay(300);
    // servo1.write(servo_middles[0]);
    // servo2.write(servo_middles[1]);
    // servo3.write(servo_middles[2]);
    // servo4.write(servo_middles[3]);
    ;
  }
}

void add_music_track(int new_index) {
  audio_path += String(new_index);  //arduino convert avec juste String(new_index)

  uint16_t time_cursor = DF1201S.getCurTime();
  DF1201S.playSpecFile(audio_path + ".mp3");
  DF1201S.setPlayTime(time_cursor);
  DF1201S.start();
}

void unlock_final() {
  //on ouvlre les loquets
  unlock_servo();

  for (auto& i : interactions_liste) {
    i.pLED_breathing_start(BREATHINGTIME, breath_C_target);
  }
}

void reset() 
{
  //arrêter la musique
  DF1201S.pause();
  audio_path = "";

  //reset les servos (refermer les loquets)
  lock_servo();

  //reset completed compte et completed_array
  completed_compte = 0;
  for (int iv = 0; iv < NUMINTERACTIONS; iv++) {
    completed_array[iv] = false;
  }

  //resets classes
  for (auto& i : interactions_liste) {
    i.reset();
  }

  //reset LEDs
  pixels.clear();
  pixels.show();
}

void update_unity() 
{
  String String_completed_interaction ="";

  for(int iv = 0; iv < NUMINTERACTIONS; iv++){
    if (completed_array[iv]){
        String_completed_interaction += '_' + String(iv);
    }
  }
  uduino.println(String_completed_interaction);
  
}

void setup() 
{

  //serial communication (voir pour internet)
  Serial.begin(9600);

  // DF player initialisation
  DF1201SSerial.begin(115200);
  while (!DF1201S.begin(DF1201SSerial)) {
    Serial.println("Init failed, please check the wire connection!");
    delay(1000);
  }
  /*Set volume to 20*/
  DF1201S.setVol(VOLUME);
  /*Enter music mode*/
  DF1201S.switchFunction(DF1201S.MUSIC);
  /*Wait for the end of the prompt tone */
  delay(2000);

  //Initialisation LEDs
  pixels.begin();

  //initialisation servos
  //servo1.attach(SERVOPIN1);
  //servo2.attach(SERVOPIN2);
  //servo3.attach(SERVOPIN3);
  //servo4.attach(SERVOPIN4);

  //servo1.write(0);
  //servo2.write(0);
  //servo3.write(0);
  //servo4.write(0);

  //Uduino Setup
  uduino.connectWifi("Antonin", "antodiplome");
}

void loop() 
{

  //Update Uduino
  uduino.update();
  if(uduino.isConnected()) {
    //uduino.println("Connected!!");
    ;
  }

  //update des interactions et consequences
  for (auto& i : interactions_liste) {
    i.update_state();
    if (i.get_just_completed()) {
      add_music_track(i.get_index());  //Ajout de la musique
      completed_compte = completed_compte + 1;
      completed_array[i.get_index()] = true;
      update_unity();  //Update info vers Unity
    }
  }

  if (completed_compte == NUMINTERACTIONS) {
    unlock_final();
  }

  //Affichage LEDs
  for (auto& i : interactions_liste) {
    c = i.get_pLED_colour_display();
    pixels.setPixelColor(i.get_index(), pixels.Color(c[0], c[1], c[2]));
    c = i.get_aLED_colour_display();
    pixels.setPixelColor(NUMINTERACTIONS + i.get_index(), pixels.Color(c[0], c[1], c[2]));
  }
  pixels.show();

  //delay
  delay(16);
}
