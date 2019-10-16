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

enum etats {init0,click0,click1,relacher0,click2,relacher1,click3,relacher2,click4,relacher3,click5,relacher4};

int main() {
	int mode_sortie = 0xff;
	int mode_entree = 0x00;
	DDRB = mode_sortie;
	DDRD = mode_entree; 
	int ROUGE = 0x01;
	int NOIR = 0x00;
	int VERT = 0x02;
	etats etat=init0;
	int delai_une_seconde = 1000;
  
	for( ;; ){
		switch (etat){ 
			case init0:
				PORTB=NOIR; 
				if(Boutton()){
					etat=click0;
				}
			break;
      
			case click0:
				if(!Boutton()){
					etat=relacher0;
				}
			break;

			case relacher0:
				if(Boutton()){
					etat=click1;
				}
			break;

			case click1:
				if(!Boutton()){
					etat=relacher1;
				}
				break;

			case relacher1:
				if(Boutton()){
					etat=click2;
				}
			break;

			case click2:
				if(!Boutton()){
					etat=relacher2;
				}
			break;

			case relacher2:
				if(Boutton()){
					etat=click3;
				}
			break;

			case click3:
				if(!Boutton()){
					etat=relacher3;
				}
			break;

			case relacher3:
				if(Boutton()){
					etat=click4;
				}  
			break;

			case click4:
				if(!Boutton()){
					etat=relacher4; 
				}
			break;

			case relacher4:
				if(Boutton()){
					etat=click5; 
				}
     
			case click5:
				if(!Boutton()){
					PORTB = ROUGE; 
					_delay_ms(delai_une_seconde);
					etat=init0;
				}
		}
	}
	return 0; 
}
