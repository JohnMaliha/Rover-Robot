/**********************************************************
* Titre : exercise 2
*  TD6 
* Auteur : Johnny Khoury et John Maliha

*********************************************************/

#include <avr/io.h> 
#define F_CPU 8000000UL //8 mhz
#include <util/delay.h> 
#include <can.h>

enum etats {init0,click1,compteur10sec};
uint8_t eteint=0x00;
//PORTB = (1 << PB0);
uint8_t rouge=0x02;
uint8_t vert=0x01;
// rouge quand on met une lumiere forte dessus. Pour trouver le seuil, on fait subir le montage a de la lumiere forte et on prend la valeur du voltmetre.
uint8_t seuilRouge = 253.69;//  cette valeur on doit la diviser par 5 et multiplie le tout par 255. On obtient une valeur de 4.974 v. 
uint8_t seuilVert =159.63 ;//meme calcul. Par contre, cette fois cest sans lumiere. on empeche la lumiere de rentrer sur le montage et on obtient un voltage de 3.13 v. 
uint8_t valeur;

void ambre (){
    
     PORTB=rouge; 
     _delay_ms(5);
     PORTB=vert;
     _delay_ms(10); 
}
 
int main(){
    DDRA=0x00; 
    DDRB = 0xff;
    DDRD = 0xff;

    can convert;


    for(;;){
        valeur = convert.lecture(PA0) >> 0x02;//les bits les moins significatif sont negligable. 
   
        if(valeur <= seuilVert){
             PORTB = vert;
        }
        else if ( valeur < seuilRouge && valeur > seuilVert){  //lumiere ambiante : entre le max (seuil rouge/lumiere forte) et le min (seuil vert/ lumiere blockee).
                ambre();
        }
        else{
            PORTB = rouge;
        }

    }   