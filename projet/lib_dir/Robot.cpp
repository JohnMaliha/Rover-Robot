#include "Robot.h"

Robot::Robot(){
    DDRA |= _BV(PA6);
    DDRB = 0xFF; 
    DDRD = 0xF0;
    moteur.startEngine();
}

Robot::~Robot(){
    //
}


void Robot::detect(){
    if(PINA & CAPTEUR1)
        C1 = true;
    else if(!(PINA & CAPTEUR1))
        C1 = false;
        
    if(PINA & CAPTEUR2)
        C2 = true;
    else if(!(PINA & CAPTEUR2))
        C2 = false;
        
    if(PINA & CAPTEUR3)
        C3 = true;
    else if(!(PINA & CAPTEUR3))
        C3 = false;
    
    if(PINA & CAPTEUR4)
        C4 = true;
    else if(!(PINA & CAPTEUR4))
        C4 = false;
    
    if(PINA & CAPTEUR5)
        C5 = true;
    else if(!(PINA & CAPTEUR5))
        C5 = false;
}

void Robot::initEndSequence(){
    del.rouge();
    _delay_ms(ONE_SECOND);
    del.eteindre();
    _delay_ms(ONE_SECOND);
    del.rouge();
    _delay_ms(ONE_SECOND);
    del.eteindre();
    _delay_ms(ONE_SECOND);
}

void Robot::followLine(){
    bool detected = true;
    while(detected){
        detect();
        if(!C1 && !C2 && !C3 && !C4 && !C5){
            _delay_ms(DEBOUNCE_DELAY);
            if(!C1 && !C2 && !C3 && !C4 && !C5){
                detected = false;
            }
        }
        else if(C3 || (C2 && C3 && C4)){         
        moteur.changeSpeed(AVGSPEED, AVGSPEED);
        }
        else if(C2 || (C1 && C2)){              
            moteur.changeSpeed(SUBSPEED, AVGSPEED);
        }
        else if(C4 || (C4 && C5)){              
            moteur.changeSpeed(AVGSPEED, SUBSPEED);
        }
        else if(C1){                            
            moteur.changeSpeed(NOSPEED, AVGSPEED);
        }
        else if(C5){                            
            moteur.changeSpeed(AVGSPEED, NOSPEED);  
        }
    }
}

void Robot::basicFollowLine(){
    detect();

    if(C3 || (C2 && C3 && C4)){
        moteur.changeSpeed(AVGSPEED, AVGSPEED);
    }
    else if(C1 || (C1 && C2)){
        moteur.changeSpeed(NOSPEED, AVGSPEED);
    }
    else if(C4 || (C4 && C5)){
        moteur.changeSpeed(AVGSPEED, NOSPEED);
    }
}

void Robot::stop(){
    moteur.changeSpeed(NOSPEED, NOSPEED);
}


void Robot::turnSequence(const char direction){
    if(direction == 'r'){
        uint8_t turnBool = 1;
        while(turnBool){
            detect();
            if(C3 || C4){
                _delay_ms(100);
                detect();
                if(C3 || C4){
                    turnBool = false;
                }
            }
            else{
                del.rouge();
                moteur.turnRight(SUBSPEED);
                detect();
            }
        }
        // rightTurnCounter--;
    }
    else if(direction == 'l'){
        uint8_t turnBool = 1;
        while(turnBool){
            detect();
            if(C2 || C3){
                _delay_ms(100);
                detect();
                if(C2 || C3){
                    turnBool = false;
                }
            }
            else{
                del.vert();
                moteur.turnLeft(SUBSPEED);
                detect();
            }
        }
    }
    else {              //Si le input de l'utilisateur n'est pas L ou R. 
        return;
    }
}

void Robot::basicTurnSequence(const char direction){
    if(direction == 'r'){
        bool turnRight = true;
        while(turnRight){
            detect();
            if(C3 || C4 || C5){
                _delay_ms(DEBOUNCE_DELAY);
                detect();
                if(C3 || C4 || C5){
                    turnRight = false;
                }
            }
            else {
                moteur.changeSpeed(AVGSPEED, NOSPEED);
            }
        }
    }
    else if(direction == 'l'){
        bool turnLeft = true;
        while(turnLeft){
            detect();
            if(C1 || C2 || C3){
                _delay_ms(DEBOUNCE_DELAY);
                detect();
                if(C1 || C2 || C3){
                    turnLeft = false;
                }
            }
            else {
                moteur.changeSpeed(NOSPEED, AVGSPEED);
            }
        }
    }
    else{   //Direction == NULL
        return;
    }
}
void Robot::sonarSendPulse(){
    PORTA |= _BV(PA6);   
    _delay_us(15);
    PORTA &= ~_BV(PA6);
}

void Robot::sonarReadOutput(){
    while(!(PINA & _BV(PA7))){}
    uint16_t counter = 0;
    while((PINA & _BV(PA7))){
        _delay_us(0.5);
        counter++;
    };
    distance = (counter/58);
}

char Robot::boucles_ctr_char_converter(const uint8_t num){
    switch(num){
        case 0: return '0';
        case 1: return '1';
        case 2: return '2';
        case 3: return '3';
        case 4: return '4';
        case 5: return '5';
        case 6: return '6';
        case 7: return '7';
        case 8: return '8';
        case 9: return '9';
        case 10: return '10';
        case 11: return '11';
        case 12: return '12';
        case 13: return '13';
    }
}  

/////////////////////////////////////////////////////////////////////////////////////
//Parcours:
void Robot::avancerJusquaCouloir(){
    bool detected = true;
    while(detected){
        if(!C1 && !C2 && !C3 && !C4 && !C5){
            _delay_ms(DEBOUNCE_DELAY);
            detect();
            if(!C1 && !C2 && !C3 && !C4 && !C5){
                del.vert();
                turnSequence('l');
                detected = false;
            }
        }
        else{
            followLine();
        }
    }
    bool preCouloir = true;

    while(preCouloir){
        if(!C1 && !C2 && !C3 && !C4 && !C5){
            _delay_ms(DEBOUNCE_DELAY);
            detect();
            if(!C1 && !C2 && !C3 && !C4 && !C5){
                preCouloir = false;
            }
        }
        else {
            followLine();
        }
    }
}

void Robot::couloir(){
    bool couloir = true;
    bool couloirLeanRight;
    bool couloirLeanLeft;
    uint32_t counter = 0;
    uint32_t previousCounter = 0;
    
    bool tempFL = true;
    while(tempFL){
        detect();
        if(!C1 && !C2 && !C3 && !C4 && !C5){
            tempFL = false;
        }
        else{
            followLine();
        }
    }
    moteur.changeSpeed(AVGSPEED, NOSPEED);
    while(couloir){
        detect();
        
        if(previousCounter != 0 && counter < previousCounter/2){
            couloir = false;
            break;
        }
        else{
            previousCounter = counter;
            counter = 0;
        }

        if(C1 || (C1 && C2)){
            couloirLeanLeft = false;
            couloirLeanRight = true;
        }
        else if(C5 || (C4 && C5)){
            couloirLeanLeft = true;
            couloirLeanRight = false;
        }

        while(couloirLeanLeft){
            detect();
            if(C1 || (C1 && C2)){
                couloirLeanLeft = false;
            }
            moteur.changeSpeed(NOSPEED, AVGSPEED);
            counter++;
        }
        while(couloirLeanRight){
            detect();
            if(C5 || (C4 && C5)){
                couloirLeanRight = false;
            }
            moteur.changeSpeed(AVGSPEED, NOSPEED);
            counter++;
        }
    }
}

void Robot::avancerCouloirAMur(){
    detect();
    bool detected = true;
    while(detected){
        detect();
        if(!C1 && !C2 && !C3 && !C4 && !C5){
            _delay_ms(DEBOUNCE_DELAY);
            detect();
            if(!C1 && !C2 && !C3 && !C4 && !C5){
                turnSequence('l');
                detected = false;
            }
        }
        else{
            followLine();
        }
    }

    bool preMur = true;
        while(preMur){
            detect();
            if(!C1 && !C2 && !C3 && !C4 && !C5){
                _delay_ms(DEBOUNCE_DELAY);
                detect();
                if(!C1 && !C2 && !C3 && !C4 && !C5){
                    preMur = false;
                }
            }
            else {
                detect();
                followLine();
            }
        }
    }

void Robot::mur(){
    bool mur = true;
    while(mur){
        detect();
        if(C1 || C2 || C3 || C4 || C5){
            mur = false;
        }

        sonarSendPulse();
        sonarReadOutput();
        _delay_ms(50);

        if(distance >=13  && distance <= 17){
            moteur.changeSpeed(AVGSPEED, AVGSPEED);
        }
        else if(distance < 13){
            moteur.changeSpeed(SUBSPEED,0);
        }
        else if(distance > 17){
            moteur.changeSpeed(0, SUBSPEED);
        }
    }
}


void Robot::avancerMurABoucles(){
    bool murAboucle = true;
    while(murAboucle){
        detect();
        if(!C1 && !C2 && !C3 && !C4 && !C5){
            _delay_ms(DEBOUNCE_DELAY);
            detect();
            if(!C1 && !C2 && !C3 && !C4 && !C5){
                turnSequence('l');
                murAboucle = false;
            }
        }
        else{
            basicFollowLine();
        }
    }
}

void Robot::boucleFL(){
    detect();
    if(C2 && C3){
        moteur.changeSpeed(SUBSPEED, AVGSPEED);
    }
    else if (C4 && C3){
        moteur.changeSpeed(AVGSPEED, SUBSPEED);
    }
    else if(C3 || (C2 && C3 && C4)){
        moteur.changeSpeed(SUBSPEED, SUBSPEED);
    }
    else if(C1 || (C2 && C1)){
        moteur.changeSpeed(NOSPEED, SUBSPEED);
    }
    else if(C4 || (C5 && C4)){
        moteur.changeSpeed(SUBSPEED, NOSPEED);
    }
}

void Robot::boucleCheck(){
    bool bcl = true;
    while(bcl){
        if((C1 && C2 && C3 )|| (C1 && C2 && C3 && C4) || (!C1 && !C2 && !C3 && !C4 && !C5) || (C1 && C2 && C3 && C4 && C5)){
            _delay_ms(DEBOUNCE_DELAY);
            detect();
            if((C1 && C2 && C3 ) || (C1 && C2 && C3 && C4) || (!C1 && !C2 && !C3 && !C4 && !C5) || (C1 && C2 && C3 && C4 && C5)){
                bcl = false;
            }
        }
        else {
            boucleFL();
        }
    }
}
void Robot::hardCodeBoucles(){
    uint8_t bclCounter = 0; 
    bool bcl = true;
    boucleCheck();  //0 -> 1
    detect();
    del.vert();
    boucleCheck();  //1 -> 2
    detect();
    del.rouge();
    boucleCheck();  //2 -> 3
    detect();
    del.eteindre();
    turnSequence('l'); //A 3 on turn left
    detect();
    del.vert();
    boucleCheck();      //3 -> 4
    detect();
    turnSequence('l'); //A 4 on turn left
    del.rouge();
    detect();
    boucleCheck();     //4-> 5
    detect();
    turnSequence('l'); //A 4 on turn left
    del.vert();
    detect();
    boucleCheck();     //5-> 6
    detect();
    del.rouge();
    turnSequence('l'); //A 5 on turn left
    detect();
    boucleCheck();     //6->7
    del.vert();
    detect();
    turnSequence('l'); //A 7 on turn left
    detect();
    boucleCheck();     //7-> 8
    del.rouge();
    detect();
    turnSequence('l'); //A 8 on turn left
    del.vert();
    detect();
    boucleCheck();     //8->9
    del.rouge();
    detect();
    turnSequence('l'); //A 9 on turn left;
    detect();
    boucleCheck();     //9->10;
    del.vert();
    detect();
    turnSequence('l'); //A 10 on turn left;
}

void Robot::boucleGotoNextCorner(uint8_t& counter){
    bool detected = true;
    while(detected){
        detect();
        if((C1 && C2 && C3) || (!C1 && !C2 && !C3 && !C4 && !C5) || (C1 && C2 && C3 && C4)){
            _delay_ms(DEBOUNCE_DELAY);
            detect();
            if((C1 && C2 && C3) || (!C1 && !C2 && !C3 && !C4 && !C5) || (C1 && C2 && C3 && C4)){
                detected = false;
                counter+=1;
            }
        }
        else{
            boucleFL();
        }
    }
}

void Robot::boucles(){
    uint8_t boucleCounter = 0;
    while(boucleCounter <= 12){
        detect();
        boucleGotoNextCorner(boucleCounter);

        switch(boucleCounter){
            case 0: break;
            case 1: break;
            case 2: break;
            case 3: turnSequence('l'); break;
            case 4: turnSequence('l'); break;
            case 5: turnSequence('l'); break;
            case 6: turnSequence('l'); break;
            case 7: turnSequence('l'); break;
            case 8: turnSequence('l'); break;
            case 9: turnSequence('l'); break;
            case 10: turnSequence('l'); break;
            case 11: break;
            case 12: break;
        }
    }
}

void Robot::avancerBouclesACoupure(){
    bool boucleACoupure = true;
    while(boucleACoupure){
        detect();
        if(!C1 && !C2 && !C3 && !C4 && !C5){
            _delay_ms(DEBOUNCE_DELAY);
            detect();
            if(!C1 && !C2 && !C3 && !C4 && !C5){
                turnSequence('l');
                boucleACoupure = false;
            }
        }
        else{
            basicFollowLine();
        }
    }
}

void Robot::avancerJusquaProchaineCoupure(){
    bool coupureBool = true;
    while(coupureBool){
        detect();
        if(!C1 && !C2 && !C3 && !C4 && !C5){
            _delay_ms(DEBOUNCE_DELAY);
            detect();
            if(!C1 && !C2 && !C3 && !C4 && !C5){
                coupureBool = false;
            }
        }
        else{
            basicFollowLine();
        }
    }
}
void Robot::coupure(){
    avancerJusquaProchaineCoupure();
    piezo.play(80); //Son aigue
    basicTurnSequence('r');
    piezo.stop();
    avancerJusquaProchaineCoupure();
    piezo.play(50); //Son grave
    basicTurnSequence('l');
    piezo.stop();
    avancerJusquaProchaineCoupure();
    piezo.play(80); //Son aigue
    basicTurnSequence('r');
    piezo.stop();
    avancerJusquaProchaineCoupure();
    piezo.play(50); //Son grave
    basicTurnSequence('l');
    piezo.stop();
}
void Robot::avancerCoupureACouloir(){
    bool coupureACouloir = true;
    while(coupureACouloir){
        detect();
        if(!C1 && !C2 && !C3 && !C4 && !C5){
            _delay_ms(DEBOUNCE_DELAY);
            detect();
            if(!C1 && !C2 && !C3 && !C4 && !C5){
                coupureACouloir = false;
            }
        }
        else{
           basicFollowLine();
        }
    }
}