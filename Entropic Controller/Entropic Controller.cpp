// Entropic Controller.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
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
vector<particle> particles; //Vector of particles for selected (initial) .state file
vector<particle> particles2; //Vector of particles for final .state file
long long int boundary = 8000000000000000000; //The side length of the entropic controller cube

int menu() {
    short input = 0;
    cout << "Entropic Controller\n";
    cout << "File Loaded: " << filename << "\n";
    cout << "Select an option:\n";
    cout << "0. Exit\n";
    cout << "1. Create a new file\n";
    cout << "2. Select a file\n";
    cout << "3. Save to file\n";
    cout << "4. Load from file\n";
    cout << "5. Set boundary value\n";
    cout << "6. List particle data\n";
    cout << "7. Add a particle\n";
    cout << "8. Remove a particle\n";
    cout << "9. Compute a solution\n";
    cout << "10. Read a solution file\n";
    cout << "Selection: ";
    cin >> input;
    cout << endl;
    return input;
}

void createfile() {
    cout << "Enter a file name: ";
    cin >> filename;
    filename = filename + ".state";
    cout << endl;
    ofstream file;
    file.open(filename);
file.close();
}

void selectfile() {
    cout << "Enter a file name: ";
    cin >> filename;
    filename = filename + ".state";
    cout << endl;
}

void addparticle() {
    short charge;
    long long int p[3];
    long long int v[3];
    long long int a[3];
    cout << "Enter the particle type:\n";
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
    cout << "Enter the z velocity: ";
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
    cout << endl;
}

void listdata() {
    cout << "Boundary: " << boundary << endl;
    for (unsigned long long int i = 0; i < particles.size(); i++) {
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
    cout << endl;
}
void removeparticle() {
    long long int particlenumber;
    cout << "Enter the particle number to remove: ";
    cin >> particlenumber;
    particles.erase(particles.begin() + particlenumber);
    cout << endl;
}

void savefile() {
    ofstream file;
    file.open(filename);
    file << "Boundary: " << boundary << endl;
    for (unsigned long long int i = 0; i < particles.size(); i++) {
        file << "Particle " << i << ":" << endl;
        if (particles[i].type == -1) {
            file << "   Electron" << endl;
        }
        if (particles[i].type == 0) {
            file << "   Neutron" << endl;
        }
        if (particles[i].type == 1) {
            file << "   Proton" << endl;
        }
        file << "   Position: (" << particles[i].position[0] << ", " << particles[i].position[1] << ", " << particles[i].position[2] << ")" << endl;
        file << "   Velcity: (" << particles[i].velocity[0] << ", " << particles[i].velocity[1] << ", " << particles[i].velocity[2] << ")" << endl;
        file << "   Acceleration: (" << particles[i].acceleration[0] << ", " << particles[i].acceleration[1] << ", " << particles[i].acceleration[2] << ")" << endl;
    }
    file.close();
}

void setboundary() {
    cout << "Enter a boundary value: ";
    cin >> boundary;
    cout << endl;
}

void loadfile() {
    ifstream file;
    string line;
    short counter = 0;
    short charge;
    long long int p[3];
    long long int v[3];
    long long int a[3];
    bool bound = false;
    char* endptr;
    file.open(filename);
    while (getline(file, line)) {
        if (bound == false) {
            bound = true;
            line.erase(0, 10);
            boundary = stoll(line);
            cout << "Boundary: " << boundary << endl;
        }
        else {
            switch (counter) {
                case 0: //Particle number is discarded
                    cout << line << endl;
                    counter++;
                    break;
                case 1: //Read the particle type
                    cout << line << endl;
                    line.erase(0, 3);
                    if (line == "Electron") {
                        charge = -1;
                    }
                    if (line == "Neutron") {
                        charge = 0;
                    }
                    if (line == "Proton") {
                        charge = 1;
                    }
                    counter++;
                    break;
                case 2: //Parse position
                    cout << line << endl;
                    line.erase(0, 14);
                    p[0] = stoll(line);
                    line.erase(0, line.find(" ")+1);
                    p[1] = stoll(line);
                    line.erase(0, line.find(" ") + 1);
                    p[2] = stoll(line);
                    counter++;
                    break;
                case 3: //Parse velocity
                    cout << line << endl;
                    line.erase(0, 13);
                    v[0] = stoll(line);
                    line.erase(0, line.find(" ") + 1);
                    v[1] = stoll(line);
                    line.erase(0, line.find(" ") + 1);
                    v[2] = stoll(line);
                    counter++;
                    break;
                case 4: //Parse acceleration and create the object
                    cout << line << endl;
                    line.erase(0, 18);
                    a[0] = stoll(line);
                    line.erase(0, line.find(" ") + 1);
                    a[1] = stoll(line);
                    line.erase(0, line.find(" ") + 1);
                    a[2] = stoll(line);
                    //Create the object
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
                    counter = 0;
                    break;
            }
        }
    }
    file.close();
}

int main()
{
    short input = 100;
    while (input != 0) {
        input = menu();
        if (input == 1) {
            createfile();
        }
        if (input == 2) {
            selectfile();
        }
        if (input == 3) {
            savefile();
        }
        if (input == 4) {
            loadfile();
        }
        if (input == 5) {
            setboundary();
        }
        if (input == 6) {
            listdata();
        }
        if (input == 7) {
            addparticle();
        }
        if (input == 8) {
            removeparticle();
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
