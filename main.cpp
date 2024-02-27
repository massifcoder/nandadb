// To run file, run : g++ main.cpp src/memotable.cpp src/utility.cpp src/console.cpp
// src/loading.cpp -I include -lncurses
#include <bits/stdc++.h>
#include "console.h"
using namespace std;

int main()
{
    bool passed_post = performPOST();

    if(!passed_post){
        cout<<"System Crashed!"<<endl;
        return 0;
    }

    int console_network_both = 0;

    cout<<"If you want to use only console, press 1."<<endl;
    cout<<"If you want to use only network, press 2."<<endl;
    cout<<"If you want to use both, press 3.\n"<<endl;

    cin>>console_network_both;

    if(console_network_both == 1){
        cout << "\033[2J\033[H";
        Console();
    }
    else if(console_network_both == 2){
        // network();
    }
    else if(console_network_both == 3){
        // both();
    }
    else{
        cout<<"Invalid input."<<endl;
    }


    return 0;
}