// To run file, run : g++ main.cpp src/memotable.cpp src/utility.cpp -I include
#include "utility.h"

int main() {
    
    //  Creation of an demo avl tree.
    creationAVL();

    // Printing values.
    printInOrder();
    flush_to_sst();
    printInOrder();

    return 0;
}
