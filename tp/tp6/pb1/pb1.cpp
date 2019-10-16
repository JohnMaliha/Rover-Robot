/**********************************************************
* Titre : exercise 1
*  TD6 
* Auteur : Johnny Khoury et John Maliha

*********************************************************/

#define F_CPU 8000000 //8MHz
#include <avr/io.h> 
#include <util/delay.h>
#include <avr/interrupt.h>

using namespace std;

/**************************************************************************************************************/

//		Etat			-			Entrée			-			Sortie				-			État suivant  //

//------------------------------------------------------------------------------------------------------------//		

//		Init0			-				1			-			eteint				-			Compteur10sec //

//						-				0			-			eteint				-				Init0	  //

//	Compteur10sec		-				1			-			Aucun				-			Compteur10sec //

//						-				0			-			Aucun				-				relache1  //

//		relache1		-				1			-	Clignote vert, puis rouge	-				Click1    //

//						-				0			-	Clignote vert, puis rouge	-				Click1	  //

//		Click1			-				-			-			Vert				-				Init0	  //

/**************************************************************************************************************/

enum etats { init0, relache1, compteur10sec, click1 };
volatile etats etat = init0;//on commence 'a partir de l etat init0
uint8_t eteint = 0x00; //couleur led
uint8_t rouge = 0x02; //couleur led
uint8_t vert = 0x01;// couleur led
volatile uint8_t compte = 0;
volatile uint8_t bouton=0; 
volatile uint8_t timerDebut =0;
//timer:  (frequence / diviseurfrequence) / 10 = 780.25;

void initialisation (void) {
	cli (); //commencer a compter
	DDRD = 0x00; //port d en mode entree
	DDRB = 0xFF;//port b en mode sortie
	EIMSK |= (1 << INT0);
	EICRA |= (1 << ISC00); 
	sei ();//arreter le compteur
}

void timer(uint16_t compte) {
	TCNT1 = 0;
	OCR1A = compte;
	TCCR1A = 0;
	TCCR1C = 0;
	TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);
	TIMSK1 |= (1 << OCIE1A);
}

ISR(INT0_vect) { 

	_delay_ms(30); //pour l antirebond, assure que le bouton a vraiment ete clique
    if (!(PIND & 0x04)){ //si le bouton n a pas ete cliquer
		
        bouton = 1;
	}
	else{
		bouton=0;
	}
	EIFR |= (1 << INTF0);
}

ISR(TIMER1_COMPA_vect){
	compte++; 
}

int main() {
	initialisation(); //a l exterieur du for, car on initialise une seule fois
	for (;;) {
		
		switch (etat) {

			case init0:
			PORTB = eteint;
			timerDebut = 0;
			compte = 0;
				if (bouton == 1) {
					etat = compteur10sec;
				}
				else {
					etat = init0;
				}
				break;
			case compteur10sec:
			
			if(timerDebut == 0){
				timer(780.25);//voir ligne 45 pour le calcul effectuer
				timerDebut = 1;
			}
				if (bouton==0 || compte == 120) {
					//arreter le timer
					TIMSK1 = 0;
					etat = relache1;
					}
				break;
				
			
			case relache1:
				PORTB = vert;
				_delay_ms(250);//on veut que la led clignote, donc on divise 1 seconde par 4 pour la permettre de s allumer et s eteindre 2 fois
				PORTB = eteint;
				_delay_ms(2000); // delai de 2 secondes

				for (uint16_t i = 0; i < (compte / 2); i++) {
					PORTB = rouge;
					_delay_ms(250);//on veut que la led clignote, donc on divise 1 seconde par 4 pour la permettre de s allumer et s eteindre 2 fois
					PORTB = eteint;
					_delay_ms(250);//on veut que la led clignote, donc on divise 1 seconde par 4 pour la permettre de s allumer et s eteindre 2 fois
				}

				etat = click1;
				break;
			case click1:
				PORTB = vert;
				_delay_ms(1000);//delai d une seconde
				etat = init0;
				compte=0;  //on reinitialise le compteur a 0 pour permettre de recommencer le processus si desirer     
				break;
		}
	}
}