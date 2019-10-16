/*
  Nom: Nawras Mohammed Amin - 1962832
  Nom: Fadi Nourredine - 1960345

 Probleme 2 du TP2 - Groupe 02 de laboratoire 

INIT 0: Quand la carte mère démarre, la DEL libre doit s'allumer en rouge. 
INIT 1: Si le bouton-poussoir noir est pesé, la DEL affiche la couleur ambre.
S1 0: Quand le bouton-poussoir est relâché, la DEL devient verte. 
S1 1: Si le bouton est de nouveau pesé, la DEL prend la couleur rouge encore. 
S2 0: Quand il est relâché, la DEL s'éteint. 
S2 1: Si le bouton est de nouveau pesé, la DEL affiche la couleur verte. 
S3 0: Quand il est relâché, la DEL tourne au rouge 
S3 1: ce qui fait que la carte mère est de retour à son état initial et tout peut recommencer.

 PROBLEME 1 - Tableau d'ETAT: 
**********************************************************
**   STATE   **   INPUT  **   NEXT STATE   **     Z     **
**********************************************************
**   INIT    **     0    **      INIT      **   ROUGE   **
**   INIT    **     1    **       S1       **   AMBRE   **
**    S1     **     0    **       S1       **   VERT    **
**    S1     **     1    **       S2       **   ROUGE   **
**    S2     **     0    **       S2       **   ETEINT  **
**    S2     **     1    **       S3       **   VERT    **
**    S3     **     0    **       S3       **   ROUGE   **
**    S3     **     1    **       INIT     **   ROUGE   **
**********************************************************

 */

#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h> 

#define INIT 0
#define S1 1
#define S2 2
#define S3 3


/*
Variables necessaires aux operations
*/
volatile bool click = false;

/*
@Brief: Cette fonction regarde si le boutton poussoir est bien pousse. 
On verifie cela grace a une double verification avec la conditionnelle if qui verifie le state de PIND & 0x04, qui est le PIN relie au bouton poussoir interrupt par
le cavalier IntEn. On laisse un delay entre la premiere verification et la deuxieme.  
@Param: Void;
@Return: Bool. Si la fonction retourne vrai, c'est que le bouton est entrain de se faire pousse. Sinon, c'est que le bouton n'est pas entrainde se faire pousse;
*/
bool debounce(){
  if(PIND & 0x04){
    _delay_ms(100);
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


int main()
{
  DDRB = 0xff; // PORT B est en mode sortie
  DDRD = 0x00; // PORTD est en entre
  unsigned int currentState = INIT; 

  for(;;)  // boucle sans fin
  {
    PORTB = 0b00; //Initiallement pas allumee

    switch(currentState){
      case INIT:
        PORTB = 0b10;   //INIT couleur initialise a rouge

        if(debounce()){
          currentState = S1;    //Lorsqu'on click le bouton poussoir, le state passe a S1.
          click = true;         //Ce qui active notre bool click (voulant dire que le boutton est pesee)
          
          while(click){         
            
            ambre();            //Pendant que le boutton est pesee la DEL devient ambre. On fait appel a la fonction ambre().
            if(!debounce()){    //Des que le bouton n'est plus poussee, on brise la boucle qui garde la couleur et on passe au prochain state
              click = false;
            }

          } 
        }
        break;

      case S1:
        PORTB = 0b01;   //S1 couleur initialise a vert

        if(debounce()){
          currentState = S2;    //Lorsqu'on click le bouton poussoir, le state passe a S2;
          click = true;         //Ce qui active notre bool click (voulant dire que le boutton est pesee)

          while(click){         
            PORTB = 0b10;       //Devient rouge lorsqu'on click
            if(!debounce()){    //Des que le bouton n'est plus poussee, on brise la boucle qui garde la couleur et on passe au prochain state
              click = false;
            }
          } 
        }
        break;

      case S2:
        PORTB = 0b00;   //S2 couleur initialise a eteint

        if(debounce()){
          currentState = S3;    //Lorsqu'on click le bouton poussoir, le state passe a S3.
          click = true;         //Ce qui active notre bool click (voulant dire que le boutton est pesee)
          
          while(click){         //Pendant que le boutton est pesee
            PORTB = 0b01;       //Devient vert 
            if(!debounce()){    //Des que le bouton n'est plus poussee, on brise la boucle qui garde la couleur et on passe au prochain state
              click = false;
            }
          } 
        }
        break;

      case S3:
        PORTB = 0b10;   //S3 couleur initialise a rouge
        if(debounce()){
          currentState = INIT;    //Lorsqu'on click le bouton poussoir, le state passe a INIT.
        }
        break;
    }
  }

  return 0; 
}

