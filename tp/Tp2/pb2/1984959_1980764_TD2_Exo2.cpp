/*
 * Nom: compteur 32 bits
 * Copyright (C) 2005 Matthew Khouzam
 * License http://www.gnu.org/copyleft/gpl.html GNU/GPL
 * Description: Ceci est un exemple simple de programme 
 * Version: 1.1
 */


#include <avr/io.h> 
#define F_CPU 8000000UL //8 mhz
#include <util/delay.h> 

using namespace std; 

bool Boutton () {
	int petit_delai = 10;
	if(PIND & 0x04){
		_delay_ms(petit_delai); 
		if(PIND & 0x04){
			return true; 
		}
		return false; 
	}
	return false; 
}

enum etats {init,etat1,etat2,etat3,etat4,etat5};

int main() {
	int mode_sortie;
	int mode_entree;
	DDRB = mode_sortie;
	DDRD = mode_entree; 
	int ROUGE = 0x01;
	int VERT = 0x02;
	int NOIR = 0x00;
	etats etat = init;
	int petit_delai = 10;
	for( ;; ){
		switch (etat){ 
			case init:
				PORTB = ROUGE; 
				if(Boutton()){
					etat=etat1;
				}
				else {
					etat=init;
				}        
			break;
      
			case etat1:
				if(Boutton()) {
					PORTB = VERT;
					_delay_ms(petit_delai);
					PORTB = ROUGE;
					_delay_ms(petit_delai);
				}
				else{
					etat=etat2;
				}
			break;
   
			case etat2:
				PORTB=VERT;
				if(Boutton()){
					etat=etat3;
				}
				else{
					etat=etat2;
				}
			break;

			case etat3:
				PORTB=ROUGE;
				if(!Boutton()){
					etat=etat4;
				}
				else{
					etat=etat3;
				}
			break;

			case etat4:
				PORTB=NOIR;
				if(!Boutton()){
					etat=etat4;
				}
				else{
					etat=etat5;
				}
			break;

			case etat5:
				PORTB=VERT;
				if(Boutton()){
					etat=etat5;
				}
				else{
					etat=init;
				}    
		}
	}
	return 0; 
}