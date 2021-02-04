////////////////////////////////////////////////////////////////////////////////
// Controls the rover in terms of angular motion
// So it can turn whilst moving
// Not sure if this will be useful in the grand scheme of things
// ... Spoiler, it was
////////////////////////////////////////////////////////////////////////////////

void angularMotionController (struct angularControl*, struct roverControl*);
void detvA (struct angularControl*);
void detvB (struct angularControl*);
void valPassBack (struct angularControl*, roverControl*);

void angularMotionController(struct angularControl* ac,struct roverControl* rc){
    if(ac->theta ==  180) {ac->theta = -180; } // This snippet ensures that if
    if(ac->theta != -180) {                    // theta != 180 or -180 degrees,
        if(ac->theta < -90) {ac->theta = -90; }// then it must be between -90
        if(ac->theta >  90) {ac->theta =  90; }// & 90 degrees. As this function
    }                                          // is not prepared for any other
                                               // values Should be unneccesary,
                                               // just in case
    detvA(ac); // Calcuates value vA
    detvB(ac); // Calcuates value vB
    valPassBack(ac, rc); // Calcuates pwm values
    rc->enAInt = 1;      // Makes sure to enable motors  || LEGACY CODE
    rc->enBInt = 1;      // On both sides                || LEGACY CODE
}

void detvA (struct angularControl* ac) { // Calcuates value vA*
    int x = 0;
    if(ac->theta == -180) { // Most of this mess due to how maths works
        x = -180;           // In C/C++ compared to Python
    } else if(ac->theta < -45) {
        x = ac->theta;
    } else if(ac->theta < 0) {
        x = -ac->theta;
    } else if(ac->theta < 45) {
        x = ac->theta;
    } else if(ac->theta <= 90) {
        x = -ac->theta;
    }
    ac->vA = ac->throttle * ((45 - x) % 90) / 90;
}

void detvB (struct angularControl* ac) {
    
    ac->vB = ac->throttle - ac->vA;
}

void valPassBack (struct angularControl* ac, struct roverControl* rc) {
    // Calcuates pwm values required for each side
    if (ac->theta == -180) { // 180 or -180 degrees
        rc->leftMotor  = 1 - (ac->vA / 100); //Fix this
        rc->rightMotor = 1 - (ac->vB / 100);
        rc->forAInt    = 1;
        rc->forBInt    = 1;
    } else if(ac->theta < -45) { // -45 -> -90 degrees
        rc->leftMotor  = 1 - (ac->vA / 100);
        rc->rightMotor =     (ac->vB / 100);
        rc->forAInt    = 1;
        rc->forBInt    = 0;
    } else if (ac->theta < 0) { //   0 -> -45 degrees
        rc->leftMotor  =     (ac->vA / 100);
        rc->rightMotor =     (ac->vB / 100);
        rc->forAInt    = 0;
        rc->forBInt    = 0;
    } else if (ac->theta < 45) { //  45 ->   0 degrees
        rc->leftMotor  =     (ac->vB / 100);
        rc->rightMotor =     (ac->vA / 100);
        rc->forAInt    = 0;
        rc->forBInt    = 0;
    } else if (ac->theta <= 90) { //  90 ->  45 degrees
        rc->leftMotor  =     (ac->vB / 100);
        rc->rightMotor = 1 - (ac->vA / 100);
        rc->forAInt    = 0;
        rc->forBInt    = 1;
    }
}
