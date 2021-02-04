////////////////////////////////////////////////////////////////////////////////
// This should hopefully be the final program for TDP3
// As a result, minimal code will be added to this "main.cpp" file
// Most of the functions will be in appropiately named header files
// This is for ease of use, and to ame it easier for multiple people to work on
// this program at the same time.
// 
// AS A RESULT; most variables will be in structs, so easy to add/change/remove
// as required.
//
// Good f*cking luck to anyone who reads this.
// God can't save us now.
//
// Let A2 be on left side
////////////////////////////////////////////////////////////////////////////////

#include "mbed.h"

#define LOW  1
#define HIGH 0


PwmOut motorL(PTC9), motorR(PTA12); // A = Left B = Right PTC8 PTA4
DigitalOut led(LED1), enA (PTA5), enB (PTD4), backL (PTC8), backR (PTA4);
DigitalOut trigD1 (PTC13), trigA2 (PTC12);
InterruptIn echoD1 (PTD7), echoA2 (PTD6);
Ticker strider, ranger, archer;

DigitalIn S5(PTA13),S4(PTD5),S3(PTD0),S2(PTD2),S1(PTD3);
// Input from Line sensing sensors,S1 is the left most one

Serial pc(USBTX, USBRX);
bool global_avoid = false, global_switchTicker = false;
bool global_lineTicker = false, global_updateMotor = false;
bool global_tickerD1 = false, global_echoStartD1 = false;
bool global_echoStopD1 = false, global_echoMeasureD1 = false;
bool global_tickerA2 = false, global_echoStartA2 = false;
bool global_echoStopA2 = false, global_echoMeasureA2 = false;
bool global_lineSenseGo = false;

#include "0variableIni.h"
#include "structDefs.h"
#include "angularMotion.h"
#include "objectDetect_D1.h"
#include "objectAvoid_A2.h"
#include "interruptStorage.h"
#include "lineSensing.h"

////////////////////////////////////////////////////////////////////////////////
// Interrupt Setup Start
////////////////////////////////////////////////////////////////////////////////

// Interrupt functions moved to storage
// Take up a lot of space

////////////////////////////////////////////////////////////////////////////////
// Interrupt Setup End
////////////////////////////////////////////////////////////////////////////////

int main() {
    
    ////////////////////////////////////////////////////////////////////////////
    // Setup Start
    ////////////////////////////////////////////////////////////////////////////
    
    struct timeControl tc; // Used structs to reduce shite in the main functions                                                    
    struct roverControl rc; // Also, so if I need to add a variable 10 functions                                                 
    struct angularControl ac; // in, I only have to add it to the struct itself.   
    struct objectSense os;
    struct variables vr;
    struct lineSense ls;
    
    Timer cycleTime;
    Timer disTime;
    
    timeContSetup(&tc); // These functions are in the "structDefs.h" header file
    roverContSetup(&rc); // This sets up the variables inside the structs
    angularContSetup(&ac);
    objectSenseSetup(&os);
    lineSenseSetup(&ls);
    variableSetup(&vr);

    enA = rc.enAInt;
    enB = rc.enBInt;
    backL = rc.forAInt;
    backR = rc.forBInt;
    
    motorR.period_ms(tc.period); // Sets up both motors for a
    motorL.period_ms(tc.period); // period of tc.period
    motorR.write(0); // Set the duty cycle for both as 0 initially
    motorL.write(0); // Just to stop anything stupid from happening
    
    disTime.start();
    strider.attach(&tickObjD1, 0.025);
    archer.attach(&tickLineSense, 0.005);
    echoD1.rise(&intEcho_startD1);
    echoD1.fall(&intEcho_stopD1);
    echoA2.rise(&intEcho_startA2);
    echoA2.fall(&intEcho_stopA2);


    cycleTime.start(); // Starts the timer managing the program loop
    cycleTime.reset(); // Resets just in case || Probably Legacy code

    ac.throttle = 100;
    ac.theta = 0;
    ////////////////////////////////////////////////////////////////////////////
    // Setup End
    ////////////////////////////////////////////////////////////////////////////
    
    while(1) {
        
    ////////////////////////////////////////////////////////////////////////////
    // Loop Start
    ////////////////////////////////////////////////////////////////////////////
        
        ////////////////////////////////////////////////////////////////////////
        // Movement Start
        ////////////////////////////////////////////////////////////////////////
        
        if( global_avoid == false) { // How to move if no object detected
            if(global_lineSenseGo == true) {
                global_lineSenseGo = false;
                global_updateMotor = true;
                lineSenseMain(&ac, &ls);
            }
        } else if(global_avoid == true) { // How to move if object is detected
            if(global_lineSenseGo == true) { // Move anticlockwise
                global_lineSenseGo = false;  // to avoid object
                global_updateMotor = true;
                static int stopCount = 0;
    
                    if(global_switchTicker == false) { // Backs up Rover to
                                                       // avoid hitting object
                    strider.detach(); // Pause Front-Side Sensor
                    ranger.attach(&tickObjA2, 0.025); // Start Left-Side Sensor
                    global_switchTicker = true;
                    motorL.write(0); // THIS IS DIRTY
                    motorR.write(0); // DON'T LOOK
                    backL = 1;       // NOONE DESERVES TO SEE THIS
                    backR = 1;       // AGHGHGHGHGH
                    wait_ms(100);    // WTF IS THIS WAIT AGHGHGG
                }
                if(os.avoidSetup == false) { // Rotate Rover clockwise
                    ac.theta = 90;     // avoidSetup = true when rover has
                    ac.throttle = 100; // turned 90 degrees
                }
                if(os.angleUpdate == true) {            // Makes rover travel
                    func_angleUpdate_A2(&os, &ac, &vr); // around the object
                    ac.throttle = 100;
                    // When the rover has successfully travelled around the
                    // object. It will detect the line
                    // The code for that would have probably gone in here
                }
            }
        }

        angularMotionController(&ac, &rc); // Calcuates output pwm signals
                                           // required
        if(global_updateMotor == true) {
            global_updateMotor = false;
            motorL.write(rc.leftMotor); // Updates both motor duty cycles
            motorR.write(rc.rightMotor); // Values taken from the test functions
            enA = rc.enAInt;
            enB = rc.enBInt;
            backL = rc.forAInt;
            backR = rc.forBInt;
        }
        ////////////////////////////////////////////////////////////////////////
        // Movement End
        ////////////////////////////////////////////////////////////////////////
        
        ////////////////////////////////////////////////////////////////////////
        // Object Sensing Start
        ////////////////////////////////////////////////////////////////////////

        while(global_echoMeasureD1 == true) {func_start_D1(&os, &ac, &disTime);}

        if( os.distD1 <= vr.distD1_AIM ) { 
            global_avoid = true;
        } else if( os.distD1 > vr.distD1_AIM ) {
            global_avoid = false;
            // Lol, this if statement actually doesn't do anything
        }
        
        ////////////////////////////////////////////////////////////////////////
        // Object Sensing End
        ////////////////////////////////////////////////////////////////////////
        
        ////////////////////////////////////////////////////////////////////////
        // Object Avoiding Start
        ////////////////////////////////////////////////////////////////////////

        while(global_echoMeasureA2 == true) {func_start_A2(&os, &vr, &disTime);}
        
        ////////////////////////////////////////////////////////////////////////
        // Object Avoiding End
        ////////////////////////////////////////////////////////////////////////
        
    ////////////////////////////////////////////////////////////////////////////
    // Loop End
    ////////////////////////////////////////////////////////////////////////////
    }
}
