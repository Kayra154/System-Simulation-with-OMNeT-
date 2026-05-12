//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "NumGenerator.h"
#include <cmath>
#include <cstdlib>

using namespace std;
NumGenerator::NumGenerator() {
    // TODO Auto-generated constructor stub

}

NumGenerator::~NumGenerator() {
    // TODO Auto-generated destructor stub
}

NumGenerator::NumGenerator(const NumGenerator &other) {
    // TODO Auto-generated constructor stub

}
double NumGenerator::exponential(double lambda){
    double u = (double)rand()/(RAND_MAX+1);
    return -log(1-u)/lambda;
}

