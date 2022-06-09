// Entropic Controller.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

//1 entropic length = 0.2am (attometers)
//The entropic controller can control entropy within a 3 meter by 3 meter cube
//1 entropic tick = 2as (attoseconds)
//The entropic controller can simulate time 30 seconds
//Every entropic tick (2as) the minimum movement of a particle is one entropic length (0.2am); the minimum speed of a particle is 0.2am/2as
//The time for a single electron to orbit a hydrogen atom is 24as or 12 entropic ticks and the orbit velocity is 2.18*10^6 m/s
//TODO: create a container array for the vector of particles or some other way to extend the possible size of particles to encompass the molecules in a 3m by 3m space
//Vector max size: 209622091746699450

class particle {
public:
    short type; //The type of particle: -1 electron, 0 neutron, 1 proton
    //Define the kinematic values for particles
    long long int position[3];
    long long int velocity[3];
    long long int acceleration[3];
    unsigned long long int injectiontime; //For input particles, the time the particle appears at the edge of the cube
};

string filename = "File not loaded";
string filename2 = "File not loaded";
string solutionfile = "File not loaded";
vector<particle> particles; //Vector of particles for selected (initial) .state file
vector<particle> particles2; //Vector of particles for final .state file
vector<particle> inputparticles; //Vector of particles to inject at edges of the cube

long long int boundary = 7500000000000000000; //The side length of the entropic controller cube; default value is 3 meter by 3 meter cube
long long int boundary2 = 7500000000000000000; //The side length of the entropic controller cube; default value is 3 meter by 3 meter cube

bool withinbound(particle particle) { //Checks if the input particle is within the boundary
    if (abs(particle.position[0]) > boundary) {
        return false;
    }
    if (abs(particle.position[1]) > boundary) {
        return false;
    }
    if (abs(particle.position[2]) > boundary) {
        return false;
    }
    return true;
}

void kinematictick(particle particle) { //Process a kinematic tick for the particle
    //Update the position based on the velocity
    particle.position[0] += particle.velocity[0];
    particle.position[1] += particle.velocity[1];
    particle.position[2] += particle.velocity[2];
    //Update the velocity based on the acceleration
    particle.velocity[0] += particle.acceleration[0];
    particle.velocity[1] += particle.acceleration[1];
    particle.velocity[2] += particle.acceleration[2];
}

void updatetick(vector<particle> particles) {  //Run one tick of the simulation for the vector of particles
    for (unsigned long long int i = 0; i < particles.size(); i++) { //Loop through all particles
        //Update the kinematic values for the particle
        kinematictick(particles[i]);
        //Sequentially checks all input particles if they are within the boundary and erases them if they are not
        if (!withinbound(particles[i])) { //If the particle is not within the boundary, erase it
            particles.erase(particles.begin() + i);
        }
    }
}

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
    cout << "9. Simulate time\n";
    cout << "10. Compute a solution\n";
    cout << "11. Read a solution file\n";
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
    unsigned long long int particlenumber;
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
        file << "   Velocity: (" << particles[i].velocity[0] << ", " << particles[i].velocity[1] << ", " << particles[i].velocity[2] << ")" << endl;
        file << "   Acceleration: (" << particles[i].acceleration[0] << ", " << particles[i].acceleration[1] << ", " << particles[i].acceleration[2] << ")" << endl;
    }
    file.close();
}

void setboundary() {
    cout << "Enter a boundary value: ";
    cin >> boundary;
    cout << endl;
}

void loadfile() { //Load the first file
    particles.clear();
    ifstream file;
    string line;
    particle particle;
    short counter = 0;
    short charge;
    long long int p[3];
    long long int v[3];
    long long int a[3];
    bool bound = false;
    file.open(filename);
    while (getline(file, line)) {
        if (bound == false) {
            cout << line << endl;
            line.erase(0, 10);
            boundary = stoll(line);
            bound = true;
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
                    line.erase(0, 14);
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
    cout << endl;
}

void loadfile2() { //Load the second file
    particles2.clear();
    ifstream file2;
    string line;
    particle particle;
    short counter = 0;
    short charge;
    long long int p[3];
    long long int v[3];
    long long int a[3];
    bool bound = false;
    file2.open(filename);
    while (getline(file2, line)) {
        if (bound == false) {
            line.erase(0, 10);
            boundary2 = stoll(line);
            bound = true;
        }
        else {
            switch (counter) {
            case 0: //Particle number is discarded
                counter++;
                break;
            case 1: //Read the particle type
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
                line.erase(0, 14);
                p[0] = stoll(line);
                line.erase(0, line.find(" ") + 1);
                p[1] = stoll(line);
                line.erase(0, line.find(" ") + 1);
                p[2] = stoll(line);
                counter++;
                break;
            case 3: //Parse velocity
                line.erase(0, 14);
                v[0] = stoll(line);
                line.erase(0, line.find(" ") + 1);
                v[1] = stoll(line);
                line.erase(0, line.find(" ") + 1);
                v[2] = stoll(line);
                counter++;
                break;
            case 4: //Parse acceleration and create the object
                line.erase(0, 18);
                a[0] = stoll(line);
                line.erase(0, line.find(" ") + 1);
                a[1] = stoll(line);
                line.erase(0, line.find(" ") + 1);
                a[2] = stoll(line);
                //Create the object
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
                particles2.push_back(particle);
                counter = 0;
                break;
            }
        }
    }
    file2.close();
}

void computesolution() {
    cout << "Enter the file name of the second file: ";
    cin >> filename2;
    filename2 = filename2 + ".state";
    cout << "Initial state: " << filename << endl;
    cout << "Final state: " << filename2 << endl;
    loadfile2();
    cout << "Enter name of solution file: ";
    cin >> solutionfile;
    solutionfile = solutionfile + ".solution";
    //Computational code goes here
    cout << endl;
}

void simulatetime() {

}

void readsolution() {

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
        if (input == 9) {
            simulatetime();
        }
        if (input == 10) {
            computesolution();
        }
        if (input == 11) {
            readsolution();
        }
    }
}