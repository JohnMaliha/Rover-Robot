/*
Nom: Nawras Mohammed Amin - 1962832
Nom: Fadi Nourredine - 1960345

Probleme 2 TP6:

Nous avons calcule les valeurs suivante grace au multimetre dans le local L3818.
En posant les sondes du multimetre aux bons cote du photoresistor, nous pouvions determinee le courant precis 
qui passait a travers le circuit, en jouant sur la lumiere qui affectait le resistor. 

CAN valeurs:

Seuil lumiere ambiante: 4.49A
Seuil rouge: 4.96A  
Seuil vert: 2.96A
 
Seuil vert en binaire: (2.96/5) * 255 = 151
Seuil rouge en binaire: (4.96/5) * 255 = 253

*/


#define F_CPU 8000000
#include <util/delay.h> 
#include <avr/interrupt.h>
#include <can.h>

uint8_t templateRouge = 253;
uint8_t templateVert = 151;

can capteur;

////////////////////////////////////////////////////
//Ces methodes permettent un API plus user-friendly.

//Eteint la del
void eteinte() {
  PORTB = 0b00;
}

//Allume la del en vert
void vert() {
  PORTB = 0b01;
}

//Allume la del en rouge
void rouge() {
  PORTB = 0b10;
}

//Allume la del en ambree (alterne rapidement entre rouge et vert)
void ambre(){
  PORTB = 0b01; //Blanc -> Rouge
  _delay_ms(10); //On donne un delay de 10ms avant de changer de couleur a vert
  PORTB = 0b10; //Rouge -> Vert
  _delay_ms(10); //On donne un delay de 10ms avant de terminer la fonction.
}


/////////////////////////////////////////////////////////////////////////////
////////////////////////////////////MAIN/////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

int main()
{
  DDRB = 0xFF; //PORTB En sortie
  DDRA = 0x00; //PORTA En entree

  for(;;) 
  {

    uint8_t valeurLue = (capteur.lecture(0)) >> 2; //Nous faisons un bitshift de 2, car la valeur qui est retournee est de 10 bits. Nous nous interressons qu'au 8 premier bits.

    //Avec la valeur lu grace au capteur, on s'interresse a savoir si le capteur est en dessous du seuil attribuee
    //a la valeur du seuil vert (templateVert) on allume la del en vert.
    //Si elle est plus haute que le seuil vert, mais plus bas que le seuil rouge, on l'allume en ambree, sinon en rouge. 
    if(valeurLue <= templateVert){
      vert();
    }
    else if(valeurLue >= templateVert && valeurLue <= templateRouge){
      ambre();
    }
    else {
      rouge();
    }

  }
  return 0; 
}

