void tickObjD1();
void intEcho_startD1();
void intEcho_stopD1();
void tickObjA2();
void intEcho_startA2();
void intEcho_stopA2();
void tickLineSense();

void tickObjD1() {          // Ticker function; For looking for obstacle at
    global_tickerD1 = true; // constant intervals
    global_echoMeasureD1 = true;
}

void intEcho_startD1() { // Interrupt for echo; start timer to measure high
    global_echoStartD1 = true;
}
void intEcho_stopD1() { // Interrupt for echo; stop timer to measure high
    global_echoStopD1 = true;
}

void tickObjA2() { // Ticker function; for travelling around obstacle
    global_tickerA2 = true;
    global_echoMeasureA2 = true;
}

void intEcho_startA2() { // Interrupt for echo; start timer to measure high
    global_echoStartA2 = true;
}
void intEcho_stopA2() { // Interrupt for echo; stop timer to measure high
    global_echoStopA2 = true;
}
void tickLineSense() {         // Ticker function; For detecting the line at
    global_lineSenseGo = true; // constant intervals
}
