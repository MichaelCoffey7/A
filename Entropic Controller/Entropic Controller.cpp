// Entropic Controller.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<fstream>
#include<vector>
using namespace std;



class particle {
public:
    //Define the kinematic values for particles
    long long int position[3];
    long long int velocity[3];
    long long int acceleration[3];
    short type; //The type of particle: -1 electron, 0 neutron, 1 proton
};

string filename = "File not loaded";
vector<particle> particles;

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
    cout << "5. List particle data\n";
    cout << "6. Add a particle\n";
    cout << "7. Remove a particle\n";
    cout << "8. Compute a solution\n";
    cout << "9. Read a solution file\n";
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

void addparticle() {
    short charge;
    long long int p[3];
    long long int v[3];
    long long int a[3];
    cout << "Enter the particle type by charge:\n";
    cout << "-1. Electron\n";
    cout << "0. Neutron\n";
    cout << "1. Proton\n";
    cin >> charge;
    cout << "Enter the x position: ";
    cin >> p[0];
    cout << "Enter the y position: ";
    cin >> p[1];
    cout << "Enter the z position: ";
    cin >> p[2];
    cout << "Enter the x velocity: ";
    cin >> v[0];
    cout << "Enter the y velocity: ";
    cin >> v[1];
    cout << "Enter the z velcity: ";
    cin >> v[2];
    cout << "Enter the x acceleration: ";
    cin >> a[0];
    cout << "Enter the y acceleration: ";
    cin >> a[1];
    cout << "Enter the z acceleration: ";
    cin >> a[2];
    particle particle;
    particle.type = charge;
    particle.position[0] = p[0];
    particle.position[1] = p[1];
    particle.position[2] = p[2];
    particle.velocity[0] = v[0];
    particle.velocity[1] = v[1];
    particle.velocity[2] = v[2];
    particle.acceleration[0] = a[0];
    particle.acceleration[1] = a[1];
    particle.acceleration[2] = a[2];
    particles.push_back(particle);
}

void listdata() {
    for (long long int i = 0; i < particles.size(); i++) {
        cout << "Particle " << i << ":" << endl;
        if (particles[i].type == -1) {
            cout << "   Electron" << endl;
        }
        if (particles[i].type == 0) {
            cout << "   Neutron" << endl;
        }
        if (particles[i].type == 1) {
            cout << "   Proton" << endl;
        }
        cout << "   Position: (" << particles[i].position[0] << ", " << particles[i].position[1] << ", " << particles[i].position[2] << ")" << endl;
        cout << "   Velcity: (" << particles[i].velocity[0] << ", " << particles[i].velocity[1] << ", " << particles[i].velocity[2] << ")" << endl;
        cout << "   Acceleration: (" << particles[i].acceleration[0] << ", " << particles[i].acceleration[1] << ", " << particles[i].acceleration[2] << ")" << endl;
    }
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
        if (input == 5) {
            listdata();
        }
        if (input == 6) {
            addparticle();
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
