/*
Nom: Nawras Mohammed Amin - 1962832
Nom: Fadi Nourredine - 1960345

Probleme 1 du TP6 - Groupe 02 de laboratoire 

Machine a etat:

STATE & INPUT & OUTPUT                                                           & Z    
------&-------&------------------------------------------------------------------&------
INIT  & 0     & eteinte                                                          & INI
INIT  & 1     & eteinte                                                          & S0 
S0    & 0     & eteinte                                                          & S0   
S0    & 1     & eteinte                                                          & S1 
S1    & 0     & Clignote en vert et ensuite eteint                               & S1 
S1    & 1     & Clignote en vert et ensuite eteint                               & S2 
S2    & 0     & Eteinte pendant 2 secondes                                       & S2 
S2    & 1     & Eteinte pendant 2 secondes                                       & S3 
S3    & 0     & Allume en rouge et clignote en rouge pour une duree de counter/2 & S3 
S3    & 1     & Allume en rouge et clignote en rouge pour une duree de counter/2 & S4 
S4    & 0     & Vert                                                             & S4         
S4    & 1     & Vert                                                             & INIT
----------------------------------------------------------------------------------------


////////////////////////////////////////////////////////////////
d'ou vient 781 ?:
On utilise ici ces informations pour deduire la duree du OCR1A:
-F_CPU = 8000000
-Prescaler = 1024

On sait que le CPU execute 8 000 000 de ticks par secondes.
Notre prescaler est a 1024. 
Ainsi, si l'on divise 8MHz/1024 = 7812.5 ticks/secondes

Comme on veut que le notre variable counter augmente 10 fois par interruption de timer, on doit encore diviser par 10:

8MHz/1024/10 = 781.25 = duree que nous devons specifie a OCR1A.
////////////////////////////////////////////////////////////////
*/


#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h> 
#include <avr/interrupt.h>


//Variable d'etat 
enum Etat {INIT, S0, S1, S2, S3, S4};
Etat etat = INIT;

volatile uint8_t boutonToggle = 0; //Binaire pour nous dire si le bouton est clickee. (0 = bouton lachee, 1 = boutton clickee)
volatile uint8_t counter = 0; //Binaire counter pour le probleme 1
volatile uint8_t minuterieExpiree = 0;  //Bit pour savoir si la minuterie a expiree

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
Methode partir minuterie qui initialise le timer1 en posant la duree donnee en parametre dans le registre de OCR1A.
*/
void partirMinuterie ( uint16_t duree ) {
  TCNT1 = 0;                              //(1 << WGM12); //On set le timer au mode CTC avec TOP = OCR1A
  OCR1A = duree;                          //C'est la duree que nous lui avons donnee en parametre
  TIMSK1 = ( 1<< OCIE1A );                
  TCCR1A = 0;                             //Le registre TCCR1A n'est pas necessaire pour notre cas.
  TCCR1B = (1 << CS12) | (1 << CS10);     //Prescaler 1024
  TCCR1C = 0;                             //Le registre TCCR1C n'est pas necessaire pour notre cas.
}

/*
Methode ISR du timer1 en verifiant qui "trigger" des que TCNT1 atteint la valeur de OCR1A
*/
ISR (TIMER1_COMPA_vect) {
  minuterieExpiree = 1;
}

/*
Methode ISR du bouton d'interrupt. Ici on a "setup" le interrupt en toggle mode. Le falling edge et le rising edge vont trigger
le interrupt request.
*/
ISR (INT0_vect) {
  _delay_ms(30);
 
  if(debounce()){
    boutonToggle = 1;
  }
  else{
    boutonToggle = 0; 
  }
}

/*
Methode d'init du service de interrupt. 
*/
void initialisation ( void ) {
  // cli est une routine qui bloque toutes les interruptions.
  // Il serait bien mauvais d'être interrompu alors que
  // le microcontroleur n'est pas prêt...
  cli ();

  // configurer et choisir les ports pour les entrées
  // et les sorties. DDRx... Initialisez bien vos variables
  DDRB = 0xff;    //PORTB en sortie
  DDRD = 0x00;    //PORTD en entree

  // cette procédure ajuste le registre EIMSK
  // de l’ATmega324PA pour permettre les interruptions externes
  EIMSK |= (1 << INT0);

  // il faut sensibiliser les interruptions externes aux
  // changements de niveau du bouton-poussoir
  // en ajustant le registre EICRA
  EICRA |= (1 << ISC00); //Ici on est aux deux sens de l'interruption (TOGGLE)

  // sei permet de recevoir à nouveau des interruptions.
  sei ();
  
}

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
  initialisation(); //Appel de la methode d'init pour les interrupts

  for(;;) 
  {
      switch (etat) {
        case INIT:
          eteinte();

          if(boutonToggle = 1)
            etat = S0;
          else
            etat = INIT;
        break;
        
        case S0:
          if(minuterieExpiree == 0){
            partirMinuterie(781); //Voir l'entete pour savoir comment nous avons trouver cette valeur.
          }
          else if(minuterieExpiree == 1){
            counter++;
          }
          
          if(counter >= 12 || boutonToggle == 0){
            TCCR1B = 0; //TCCR1B, lorsque tout les reigstres sont a 0, arrete le timer. 
            etat = S1;
          }

        break;

        //////Le reste de ses etapes n'ont pas besoin de trop d'explications, ce n'est que la sequence d'etat decrit dans 
        //l'enonce du TP.
        case S1:
          vert();
          _delay_ms(500);
          eteinte();
          etat = S2;
        break;
 
        case S2:
          eteinte();
          _delay_ms(2000);
          etat = S3;
        break;
 
        case S3:
          rouge();
          for(uint8_t x = 0; x <= counter/2; x++){
            rouge();
            _delay_ms(500);
            eteinte();
            _delay_ms(500);
          }
          etat = S4;
        break;
 
        case S4:
          vert();
          _delay_ms(1000);
          etat = INIT;
          boutonToggle = 0;
          minuterieExpiree = 0;
        break;
        }
    
  }
  return 0; 
}

