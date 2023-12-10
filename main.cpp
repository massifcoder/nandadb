// To run file, run : g++ main.cpp src/memotable.cpp src/utility.cpp -I include
#include "utility.h"

int main() {

    creationAVL();
    flush_to_sst();
    load_from_sst();

    return 0;
}