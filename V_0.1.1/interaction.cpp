//Code de la classe interaction
//Demande debuggage, mais a priori rien de compliqué quand même
// Manque les getter LOL ça risquait pas de marcher
// La structure de classe part du principe que chaque interaction a 1 LED de progression et une LED d d'ambiance.
// Je pense que ça vaut le coup de tout reprendre de 0 de manière un peu plus miniamle, puis ajouter les fonctions à la classe de manière progressive, avec des tests pallier.
// 2 options : Perseverer en OOP (object oriented programming) et faire de vrais sous classes bien belles et strcturées (classe 'LED', classe couleur, classe loquet, ect...), ce qui permettrait se pretterait bien a un developpement progressif.
//            Passer en fonctionnel, ce qui est plus simple et plus rapide à programmer, mais qui est un code plus dépendant de la configuration de la serre et donc plus sensible aux changements de ses paramètres (nombres d'interactions ect...).

#include <Adafruit_NeoPixel.h>
#include "Arduino.h";
#include "interaction.h";


interaction::interaction(int index, int capteur_pin, int LED_pin){
  _index = index;
  completed = false;
  is_open_buffer = false;
  _seuil = 50;
  open_compte = 0;

  _capteur_pin = capteur_pin;
  pinMode(_capteur_pin,INPUT_PULLUP);

  _LED_pin = LED_pin;

  pLED_ON = false;
  pLED_colourR = 255;
  pLED_colourG = 255;
  pLED_colourB = 255;
  pLED_breathing = false;

  aLED_ON = false;
  aLED_colourR = 255;
  aLED_colourG = 255;
  aLED_colourB = 255;
  aLED_breathing = false;

}


bool interaction::get_completed() const{
  return completed;
}

bool interaction::get_is_open() const{
  return is_open;
}

void interaction::set_seuil(int seuil){
  _seuil = seuil;
}

void interaction::set_index(int index){
  _index = index;
}

void interaction::set_pLED_colour(int R, int G, int B){
  pLED_colourR = R;
  pLED_colourG = G;
  pLED_colourB = B;
}

void interaction::set_LED_pin(int pin){
  _LED_pin = pin;
}

void interaction::set_aLED_colour(int R, int G, int B){
  aLED_colourR = R;
  aLED_colourG = G;
  aLED_colourB = B;
}

void interaction::pLED_breathing_start(int time, int target_colour[3]){
  pLED_breathing = true;
  pLED_breath_back = false;
  pLED_breath_time = time;
  pLED_breath_compte = 1;
  pLED_colour_display[0] = pLED_colourR;
  pLED_colour_display[1] = pLED_colourG;
  pLED_colour_display[2] = pLED_colourB;

  pLED_breath_colour_step[0] = (pLED_colourR - target_colour[0]) / time;
  pLED_breath_colour_step[1] = (pLED_colourG - target_colour[1]) / time;
  pLED_breath_colour_step[2] = (pLED_colourB - target_colour[2]) / time;
}

void interaction::aLED_breathing_start(int time, int target_colour[3]){
  aLED_breathing = true;
  aLED_breath_back = false;
  aLED_breath_time = time;
  aLED_breath_compte = 1;
  aLED_colour_display[0] = aLED_colourR;
  aLED_colour_display[1] = aLED_colourG;
  aLED_colour_display[2] = aLED_colourB;

  aLED_breath_colour_step[0] = (aLED_colourR - target_colour[0]) / time;
  aLED_breath_colour_step[1] = (aLED_colourG - target_colour[1]) / time;
  aLED_breath_colour_step[2] = (aLED_colourB - target_colour[2]) / time;
}

void interaction::breathing_stop(bool BpLED, bool BaLED){
  pLED_breathing = !BpLED;
  aLED_breathing = !BaLED;
}

void interaction::update_state(){

  if (!completed){ // Si interaction non complete --> on check l'etat du capteur

    is_open_buffer = is_open;
    is_open = digitalRead(_capteur_pin):

    if(is_open){
      open_compte = open_compte +1;
    }

    if(open_compte >= _seuil){
      completed = true;
      pLED_ON = true;
    }
  }
  else{ // Sinon on allume la pLED (eventuellement avec breathing)

    if (pLED_breathing) {
      if (!pLED_breath_back){
        pLED_colour_display[0] = pLED_colourR + pLED_breath_colour_step[0]*pLED_breath_compte;
        pLED_colour_display[1] = pLED_colourR + pLED_breath_colour_step[1]*pLED_breath_compte;
        pLED_colour_display[2] = pLED_colourR + pLED_breath_colour_step[2]*pLED_breath_compte;
        pLED_breath_compte = (pLED_breath_compte +1)%pLED_breath_time;
        if (pLED_breath_compte ==0){
          pLED_breath_back = true;
        }
      }
      else {
        pLED_colour_display[0] = pLED_colour_display[0] - pLED_breath_colour_step[0];
        pLED_colour_display[1] = pLED_colour_display[1] - pLED_breath_colour_step[1];
        pLED_colour_display[2] = pLED_colour_display[2] - pLED_breath_colour_step[2];
        pLED_breath_compte = (pLED_breath_compte +1)%pLED_breath_time;
        if (pLED_breath_compte ==0){
          pLED_breath_back = false;
        }
      }
    }
    else{
      pLED_colour_display[0]= pLED_colourR;
      pLED_colour_display[1]= pLED_colourG;
      pLED_colour_display[2]= pLED_colourB;
    }
  }

  if (aLED_ON){ //Par ailleurs, si aLED est allumée
    if (aLED_breathing) {
      if (!aLED_breath_back){
        aLED_colour_display[0] = aLED_colourR + aLED_breath_colour_step[0]*aLED_breath_compte;
        aLED_colour_display[1] = aLED_colourR + aLED_breath_colour_step[1]*aLED_breath_compte;
        aLED_colour_display[2] = aLED_colourR + aLED_breath_colour_step[2]*aLED_breath_compte;
        aLED_breath_compte = (aLED_breath_compte +1)%aLED_breath_time;
        if (aLED_breath_compte ==0){
          aLED_breath_back = true;
        }
      }
      else {
        aLED_colour_display[0] = aLED_colour_display[0] - aLED_breath_colour_step[0];
        aLED_colour_display[1] = aLED_colour_display[1] - aLED_breath_colour_step[1];
        aLED_colour_display[2] = aLED_colour_display[2] - aLED_breath_colour_step[2];
        aLED_breath_compte = (aLED_breath_compte +1)%aLED_breath_time;
        if (aLED_breath_compte ==0){
          aLED_breath_back = false;
        }
      }
    }
    else{
      aLED_colour_display[0] = aLED_colourR;
      aLED_colour_display[1] = aLED_colourG;
      aLED_colour_display[2] = aLED_colourB;
    }
  }

}

void interaction::reset(){
  open_compte = 0;
  pLED_breath_compte = 0;
  aLED_breath_compte = 0;
  completed = false;

}

