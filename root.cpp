#include"root.h"

bool checkRange(int a, int u, int l) { 
    
    return a >= u and a <= l;
}

bool checkRange(int a, int u, int l, bool left, bool right) { 
    
    return (left  ? a >= u : a > u) 
       and (right ? a <= l : a < l); 
}

Root::Root() {}

int Root::levelDimension = 120;

void Root::setDim(int d) {

    levelDimension = d;
}
 
int Root::getDim() {
    
    return levelDimension;
}

bool Root::checkForBounds(int a) {
    
    return checkRange(a, 0, Root::getDim()*Root::getDim(), true, false);
}

vector<int> Root::makeNeighborhood(int val) {

    vector<int> ret;

    int R = val + 1;
    int L = val - 1;
    int D = val + getDim();
    int U = val - getDim();

    if(checkForBounds(R))   ret.push_back(R);
    if(checkForBounds(L))   ret.push_back(L);
    if(checkForBounds(D))   ret.push_back(D);
    if(checkForBounds(U))   ret.push_back(U);

    return ret;
}

vector<int> Root::makeExtendedNeighborhood(int val) {

    vector<int> ret = makeNeighborhood(val);

    int RD = val + 1 + getDim();
    int RU = val + 1 - getDim();
    int LD = val - 1 + getDim();
    int LU = val - 1 - getDim();

    if(checkForBounds(RD))  ret.push_back(RD);
    if(checkForBounds(RU))  ret.push_back(RU);
    if(checkForBounds(LD))  ret.push_back(LD);
    if(checkForBounds(LU))  ret.push_back(LU);

    return ret;
}