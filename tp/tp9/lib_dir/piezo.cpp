#include "piezo.h"

Piezo::Piezo(){
    init();
}

Piezo::~Piezo(){
    //
}

void Piezo::init(){
    TCNT2 = 0; //Set timer2 to 0   
    //On set le timer a fastPWM en mode CTC.
    TCCR2A = _BV(WGM21) | _BV(COM2A0)                   //CTC mode. Toggle on Compare. 
    TCCR2B = _BV(CS21);                                 //Prescaler de 8 (Pour quon genere un clock frq de 1MHz)
}

void Piezo::play(uint8_t duree, uint8_t freq){

    /*
    Methodologie: On sait que la periode et la freq sont relier par la l'eq suivante: T = 1/F
    Or, on sait aussi que le wavelength est toujours de 50%. (Le duty cycle est de 1/2 periode).
    */

    uint8_t a, b, cycle;

    b = (1/freq) * 1000; 
    cycle = duree/b;
    a = b/2;

}

void Piezo::calculPourcentage(uint8_t duree, uint8_t freq){
    /*
    Methodologie: On sait que la periode et la freq sont relier par la l'eq suivante: T = 1/F
    Or, on sait aussi que le wavelength est toujours de 50%. (Le duty cycle est de 1/2 periode).
    Donc on peut set le duty cycle avec la formule si-dessous.
    */

   uint8_t dutyCycle = (F_CPU/8)*((1/frequence)/2); //1.Frequence = Periode. 
}
        
    
void Piezo::stop(){

    OCR2A = 0; //On set la duree 0 ;
    TCNT2 = 0; //On set le timer a 0;
    TCCR2B = 0; //On arrete le timer;
}