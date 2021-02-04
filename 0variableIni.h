void variableSetup (struct variables*);

struct variables {
    int distD1_AIM;
    int circle_X;
};

void variableSetup (struct variables* vr) {
    vr->distD1_AIM = 22; //Distance to sense for in cm
    vr->circle_X = 1;
}
