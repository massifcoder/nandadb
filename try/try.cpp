#include <iostream>
#include <filesystem>
using namespace std;
namespace fs = std::filesystem;

int main() {
    fs::path currentPath = fs::current_path();
    string path = string(currentPath) + "/database";
    cout<<path<<endl;

    if(!fs::exists(path)){
        fs::create_directory(path);
        cout<<"File created successfully."<<endl;
    }

    return 0;
}
