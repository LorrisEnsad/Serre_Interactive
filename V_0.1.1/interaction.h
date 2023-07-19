//Header de interaction

#ifndef interaction_h
#define interaction_h
#include <Adafruit_NeoPixel.h>
#include "Arduino.h" 
#include <array>

class interaction {
  
public:
  interaction(int index, int capteur_pin, int LED_pin);

  bool get_completed() const;
  bool get_is_open() const;
  int get_index() const;
  std::array<int,3> get_pLED_colour_display() const;// Eviter les problemes et faire 3 getter de int a la place
  std::array<int,3> get_aLED_colour_display() const;//Eviter les problemes et faire 3 getter de int a la place

  void set_seuil (int seuil);
  void set_index (int index);
  void set_LED_pin(int pin);
  void set_pLED_colour(int R, int G, int B);
  void set_aLED_colour(int R, int G, int B);

  void pLED_breathing_start(int time, int target_colour[3]);
  void aLED_breathing_start(int time, int target_colour[3]);
  void breathing_stop(bool BpLED, bool BaLED);

  void update_state();
  void reset();


private:
  int _index;
  bool completed;
  int _capteur_pin;
  bool is_open;
  bool is_open_buffer;
  int open_compte;
  int _seuil;
  int _LED_pin;

  bool pLED_ON;
  int pLED_colourR;
  int pLED_colourG;
  int pLED_colourB;
  std::array<int,3> pLED_colour_display;
  bool pLED_breathing;
  bool pLED_breath_back;
  int pLED_breath_compte;
  int pLED_breath_time;
  int pLED_breath_colour_step[3];
  
  bool aLED_ON;
  int aLED_colourR;
  int aLED_colourG;
  int aLED_colourB;
  std::array<int,3> aLED_colour_display;
  bool aLED_breathing;
  bool aLED_breath_back;
  int aLED_breath_compte;
  int aLED_breath_time;
  int aLED_breath_colour_step[3];



};
#endif
