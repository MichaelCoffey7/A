// Entropic Controller.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<fstream>
using namespace std;

string filename = "File not loaded";

class electron {
public:
    //Define the kinematic values for the electron
    long long int position[3];
    long long int velocity[3];
    long long int acceleration[3];
};

class proton {
public:
    //Define the kinematic values for the electron
    long long int position[3];
    long long int velocity[3];
    long long int acceleration[3];
};

class neutron {
public:
    //Define the kinematic values for the electron
    long long int position[3];
    long long int velocity[3];
    long long int acceleration[3];
};

int menu() {
    int input = 0;
    cout << "Entropic Controller\n";
    cout << "File Loaded: " << filename << "\n";
    cout << "Select an option:\n";
    cout << "0. Exit\n";
    cout << "1. Create a new file\n";
    cout << "2. Select a file\n";
    cout << "3. Load from file\n";
    cout << "4. Save to file\n";
    cout << "5. Add a particle\n";
    cout << "6. Compute a solution\n";
    cin >> input;
    return input;
}

void createfile() {
    cout << "Enter a file name\n";
    cin >> filename;
    filename = filename + ".state";
    ofstream file;
    file.open(filename);
    file.close();
}

void selectfile() {
    cout << "Enter a file name\n";
    cin >> filename;
    filename = filename + ".state";
}

int main()
{
    int input = 10;
    while (input != 0) {
        input = menu();
        if (input == 1) {
            createfile();
        }
        if (input == 2) {
            selectfile();
        }
    }
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
