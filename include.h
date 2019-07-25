#ifndef INCLUDE_H
#define INCLUDE_H

#include<allegro5/allegro.h>
#include<allegro5/allegro_image.h>
#include<allegro5/allegro_primitives.h>
#include<array>
#include<iostream>
#include<utility>
#include<vector>
#include<list>
#include<cstdlib>
#include<cmath>
#include<map>
#include<algorithm>
#include<cassert>
#include<climits>
using namespace std;

enum AXIS : int {none=0, X=1, Y=2, XY=3};
enum KEYS : int {still=0, UP=1, LEFT=2, DOWN=3, RIGHT=4, SPACE=5};
enum DIRS : int {NW=0, NE=1, SE=2, SW=3};

inline bool in_range(int first, int lower_bound, int upper_bound) { return first >= lower_bound and first <= upper_bound; } 

const bool debug_mode = false;

#endif