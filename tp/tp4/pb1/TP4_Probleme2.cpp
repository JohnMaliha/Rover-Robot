/*
  Nom: Nawras Mohammed Amin - 1962832
  Nom: Fadi Nourredine - 1960345
*/

#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h> 

/*
Variables necessaires aux operations
*/
volatile uint8_t minuterieExpiree;
volatile uint8_t boutonPoussoir; 

/*
@Brief: Cette fonction regarde si le boutton poussoir est bien pousse. 
On verifie cela grace a une double verification avec la conditionnelle if qui verifie le state de PIND & 0x04, qui est le PIN relie au bouton poussoir interrupt par
le cavalier IntEn. On laisse un delay entre la premiere verification et la deuxieme.  
@Param: Void;
@Return: Bool. Si la fonction retourne vrai, c'est que le bouton est entrain de se faire pousse. Sinon, c'est que le bouton n'est pas entrainde se faire pousse;
*/
bool debounce(){
  if(PIND & 0x04){
    _delay_ms(10);
    if(PIND & 0x04) {
      return true;
    }
  }
  return false;

}

/*
@Brief: Cette fonction permet d'allumer la DEL avec la couleur ambre
(Une combinaison rapide entre rouge et verte qui fait croire a l'oeil que c'est de l'ambree).
Cette fonction est passee dans la boucle infinie ce qui permet le changement constant entre rouge et vert.
La couleur rouge est soit de 0b01 ou bien de 0b10 dependamment de la facon que le fils est insere dans le PORTB.
@Param: Void;
@Return: Void;
*/
void ambre(){
  PORTB = 0b01; //Rouge/Vert -> Rouge/Vert
  _delay_ms(15); //On donne un delay de 10ms avant de changer de couleur a vert
  PORTB = 0b10; //Rouge/Vert -> Vert/Rouge
  _delay_ms(15); //On donne un delay de 10ms avant de terminer la fonction.
}


ISR (TIMER0_COMPA_vect) {
  minuterieExpiree = 1;
}


ISR (INT0_vect) {
  boutonPoussoir = 1;
  // anti-rebond
  debounce();

}

void partirMinuterie ( uint16_t duree ) {
minuterieExpiree = 0;

  // mode CTC du timer 1 avec horloge divisée par 1024
  // interruption après la durée spécifiée
  TCNT1 = 0; 
  OCR1A = duree;
  TCCR1A |= (1 << COM1A1) || (1 << ) ;
  TCCR1B = 'modifier ici' ;
  TCCR1C = 0;
  TIMSK1 = 'modifier ici' ;

}


int main()
{
  DDRB = 0xff; // PORT B est en mode sortie
  DDRD = 0x00; // PORTD est en entre
  unsigned int currentState = INIT; 

  return 0; 
}

