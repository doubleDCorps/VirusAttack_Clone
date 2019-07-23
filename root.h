#include"ScaledBitmap.h"

bool checkRange(int a, int u, int l);
bool checkRange(int a, int u, int l, bool left, bool right);

class Root {
    private:
        static int levelDimension;
        Root();
        
    public:
        static vector<int> makeNeighborhood(int);
        static vector<int> makeExtendedNeighborhood(int);
        static void setDim(int);
        static int getDim();
        static bool checkForBounds(int);
};