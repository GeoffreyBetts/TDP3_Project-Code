////////////////////////////////////////////////////////////////////////////////
// The angularControl struct has 2 relevant structs:
// ac->throttle (value range: 0 - 100)
// ac->theta (value range: -90 - 90 && 180) Where -ve == left and +ve == right
////////////////////////////////////////////////////////////////////////////////

int read_sensor_values() {
    int position, sensor1, sensor2, sensor3, sensor4, sensor5;
    
    sensor4 = S4;
    sensor3 = S3;
    sensor2 = S2;
    sensor1 = S1;
    
// all PID error cases follow: each sensor has a set weight that increases
// linearly(-2,-1,0,1,2) as the turn is sharper
  if ((sensor1 == 0) && (sensor2 == 0) && (sensor3 == 0) && (sensor4 == 0)) {
        // white tape case i.e move forward
        position = 0; 
    } else if ((sensor1 == 0)&&(sensor2 == 1)&&(sensor3 == 1)&&(sensor4 == 0)) {
        position = 0;
    } else if ((sensor1 == 0)&&(sensor2 == 0)&&(sensor3 == 1)&&(sensor4 == 0)) {
        // weighted average is taken for the 2 sensors ((0+1)/2)
        position = 1;
    } else if ((sensor1 == 0)&&(sensor2 == 0)&&(sensor3 == 1)&&(sensor4 == 1)) {
        position = 2;
    } else if ((sensor1 == 0)&&(sensor2 == 0)&&(sensor3 == 0)&&(sensor4 == 1)) {
        position = 3;
    } else if ((sensor1 == 0)&&(sensor2 == 1)&&(sensor3 == 0)&&(sensor4 == 0)) {
        position = -1;
    } else if ((sensor1 == 1)&&(sensor2 == 1)&&(sensor3 == 0)&&(sensor4 == 0)) {
        position = -2;
    } else if ((sensor1 == 1)&&(sensor2 == 0)&&(sensor3 == 0)&&(sensor4 == 0)) {
        position = -3;
    } else if ((sensor1 == 0)&&(sensor2 == 1)&&(sensor3 == 1)&&(sensor4 == 1)) {
        position = 10; //special case i.e right angle detected
    } else if ((sensor1 == 1)&&(sensor2 == 1)&&(sensor3 == 1)&&(sensor4 == 0)) {
        position = -10; //special case i.e left angle detected
    } else {
        position = 0;
    }
    return position;
  
  
}

void calculate_pd(float error, struct angularControl* ac, struct lineSense* ls)
{
    float Kp = 10;
    float Kd = 0;
    static float lasterror;

    if(error < 0) { // All this schieBe is to manage to counter, strCount.
                    // Mimics oscillations
        ls->thetaLeft = true; // When rover needs to move left
        if(ls->thetaRight == true) {
            ls->thetaRight = false;
            ls->strCount -= 1;
        } else if(ls->thetaZero == true) {
            ls->thetaZero = false;
            ls->strCount = 80;
        } else {
            ls->strCount += 1;
        }
    } else if(error > 0) { // When rover needs to move right
        ls->thetaRight = true;
        if(ls->thetaLeft == true) {
            ls->thetaLeft = false;
            ls->strCount -= 1;
        } else if(ls->thetaZero == true) {
            ls->thetaZero = false;
            ls->strCount = 80;
        } else {
            ls->strCount += 1;
        }
    } else { // When rover needs to move straight
        ls->thetaZero = true;
        if(ls->thetaLeft == true || ls->thetaRight == true) {
            ls->thetaLeft  = false;
            ls->thetaRight = false;
            ls->strCount -= 1;
        } else {
            ls->strCount += 1;
        }
    }
    
    if(ls->strCount > 100) { ls->strCount = 100; } // Limits variable strCount
    if(ls->strCount <  10) { ls->strCount =  10; } // to sensisble values

    ac->theta = (ls->strCount / 10) * 3 * error; // Calcuates theta value
    if(ac->theta >  90) { ac->theta =  90; } // Limits theta value, unneseccary
    if(ac->theta < -90) { ac->theta = -90; }
    int temp_X;              // Ummm, all this looks useless
    if(ac->theta < 0) {      // Will keep this anyway
        temp_X = -ac->theta; // Since we can't test rover anymore
    } else {                 // - person who wrote it
        temp_X =  ac->theta;
    }
}

void lineSenseMain(struct angularControl* ac, struct lineSense* ls) {
    calculate_pd(read_sensor_values(), ac, ls);
}
