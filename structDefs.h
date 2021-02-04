void timeContSetup (struct timeControl*);
void roverContSetup (struct roverControl*);
void angularContSetup (struct angularControl*);
void lineSenseSetup (struct lineSense*);
void objectSenseSetup (struct objectSense*);

struct timeControl {                                                            
    float period; // Sets period of pwm for motors
    int cycleTime; // Legacy Code, does nothing
};

struct roverControl {
    float leftMotor;
    float rightMotor;
    int rightDir;
    
    int enAInt;
    int enBInt;
    int forAInt;
    int forBInt;
};

struct angularControl { // Variables for angular motion
    float throttle;          
    int theta;
    int thetaStore;    
    
    float vA;
    float vB;         
};

struct objectSense {
    int actualTime1;
    int actualTime2;
    int actualTimeT;
    int distD1;
    int distA2;
    int distCheckA2;
    int angleStrength;

    bool angleUpdate;
    bool circleLeft;
    bool circleRight;
    bool avoidSetup;
};

struct lineSense {
    int strCount;

    bool thetaRight;
    bool thetaLeft;
    bool thetaZero;
};

void lineSenseSetup (struct lineSense* ls) {
    ls->strCount = 1;

    ls->thetaRight = false;
    ls->thetaLeft = false;
    ls->thetaZero = true;
}

void objectSenseSetup (struct objectSense* os) {
    //stuff
    os->actualTime1 = 0;
    os->actualTime2 = 0;
    os->actualTimeT = 0;
    os->distD1 = 1000;
    os->distA2 = 0;
    os->distCheckA2 = 0;
    os->angleStrength = 1;

    os->angleUpdate = false;
    os->circleLeft = false;
    os->circleRight = false;
    os->avoidSetup = false;
}
void timeContSetup (struct timeControl* tc) {
    tc->period = 1; // Set period of pwm (ms)
    tc->cycleTime = 10; // Legacy?
};

void roverContSetup (struct roverControl* rc) {
    rc->leftMotor = 0; // leftMotor = Duty cycle for left-side
    rc->rightMotor = 0; // rightMotor = Dusty cycle for right-side
    rc->rightDir = 0; // Legacy?
    
    rc->enAInt = 1; // 1 = enable
    rc->enBInt = 1; // 1 = enable
    rc->forAInt = 0; // 0 = for, 1 = rev
    rc->forBInt = 0; // 0 = for, 1 = rev
};

void angularContSetup (struct angularControl* ac) {
    ac->throttle = 0; // throttle = controls pwm values for motor
    ac->theta = 0; // theta = Controls pwm values for motor
    ac->thetaStore = 0; // I don't think this does anything
    
    ac->vA = 0; // Value for calcuating theta
    ac->vB = 0; // Value for calcuating theta
};
