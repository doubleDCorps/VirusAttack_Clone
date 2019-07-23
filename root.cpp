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

vector<int> Root::makeNeighborhood(int val) {

    vector<int> ret;

    int R = val + 1;
    int L = val - 1;
    int D = val + getDim();
    int U = val - getDim();

    if(R/getDim() >= 0 and R/getDim() < getDim() and R%getDim() >= 0 and R%getDim() < getDim())
        ret.push_back(R);
    if(L/getDim() >= 0 and L/getDim() < getDim() and L%getDim() >= 0 and L%getDim() < getDim())
        ret.push_back(L);
    if(D/getDim() >= 0 and D/getDim() < getDim() and D%getDim() >= 0 and D%getDim() < getDim())
        ret.push_back(D);
    if(U/getDim() >= 0 and U/getDim() < getDim() and U%getDim() >= 0 and U%getDim() < getDim())
        ret.push_back(U);

    return ret;
}

vector<int> Root::makeExtendedNeighborhood(int val) {

    vector<int> ret = makeNeighborhood(val);

    int RD = val + 1 + getDim();
    int RU = val + 1 - getDim();
    int LD = val - 1 + getDim();
    int LU = val - 1 - getDim();

    if(RD/getDim() >= 0 and RD/getDim() < getDim() and RD%getDim() >= 0 and RD%getDim() < getDim())
        ret.push_back(RD);
    if(RU/getDim() >= 0  and RU/getDim() < getDim() and RU%getDim() >= 0 and RU%getDim() < getDim())
        ret.push_back(RU);
    if(LD/getDim() >= 0 and LD/getDim() < getDim() and LD%getDim() >= 0 and LD%getDim() < getDim())
        ret.push_back(LD);
    if(LU/getDim() >= 0 and LU/getDim() < getDim() and LU%getDim() >= 0 and LU%getDim() < getDim())
        ret.push_back(LU);

    return ret;
}