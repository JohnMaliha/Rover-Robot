/*
 Nom: Nawras Mohammed Amin - 1962832
 Nom: Fadi Nourredine - 1960345

 Probleme 1 du TP2 - Groupe 02 de laboratoire 


 PROBLEME 1 - Tableau d'ETAT: 
**********************************************************
**   STATE   **   INPUT  **   NEXT STATE   **     Z     **
**********************************************************
**   INIT    **     0    **      INIT      **   ETEINT  **
**   INIT    **     1    **       S0       **   ETEINT  **
**    S1     **     0    **       S1       **   ETEINT  **
**    S1     **     1    **       S2       **   ETEINT  **
**    S2     **     0    **       S2       **   ETEINT  **
**    S2     **     1    **       S3       **   ETEINT  **
**    S3     **     0    **       S3       **   ETEINT  **
**    S3     **     1    **       S4       **   ETEINT  **
**    S4     **     0    **       S4       **   ETEINT  **
**    S4     **     1    **      INIT      **   ROUGE   **
**********************************************************
 */

#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h> 

#define INIT 0
#define S1 1
#define S2 2
#define S3 3
#define S4 4

/*
Variables necessaires aux operations
*/

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
Cette fonction est passe dans la boucle infinie ce qui permet le changement constant entre rouge et vert.

La couleur rouge est soit de 0b01 ou bien de 0b10 dependamment de la facon que le fils est insere dans le PORTB.
@Param: Void;
@Return: Void;
*/
void ambre(){
  PORTB = 0b01; //Blanc -> Rouge
  _delay_ms(10); //On donne un delay de 10ms avant de changer de couleur a vert
  PORTB = 0b10; //Rouge -> Vert
  _delay_ms(10); //On donne un delay de 10ms avant de terminer la fonction.
}


int main()
{
  DDRB = 0xff; // PORT B est en mode sortie
  DDRD = 0x00; // PORTD est en entre
  unsigned int currentState = INIT; 

  for(;;) 
  {
    PORTB = 0b00; //Initiallement pas allumee

    switch(currentState){ //Machine a etat
      case INIT:          //State initial 
        if(debounce()){   //Si on click le bouton
          currentState = S1;  //State switch a State1 
        }
        break;

      //On repette le meme processus, jusqu'a state4 (S4)
      case S1:
        if(debounce()){
          currentState = S2;
        }
        break;

      case S2:
        if(debounce()){
          currentState = S3;
        }
        break;

      case S3:
        if(debounce()){
          currentState = S4;
        }
        break;

      case S4:          //Dans le cas ou l'on arrive a S4 (Voulant dire que nous avons clicker 4 fois le boutton)
        if(debounce()){
          currentState = INIT;    //Des que l'on reclick le boutton, (5 clicks total)
          PORTB = 0b10;           //La couleur passe a Rouge et le state retourn a INIT
          _delay_ms(1000);        //On donne un delay de 1000ms (1s) comme specifie dans l'ennonce
        }
        break;
    }
  }

  return 0; 
}

