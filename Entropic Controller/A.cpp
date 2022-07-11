// A.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

//1 entropic length = 0.2am (attometers)
//The entropic controller can control entropy within a 3 meter by 3 meter cube
//1 entropic tick = 0.2as (attoseconds)
//The entropic controller can simulate time 30 seconds
//Every entropic tick (0.2as) the minimum movement of a particle is one entropic length (0.2am); the minimum speed of a particle is 0.2am/0.2as = 1m/s
//The time for a single electron to orbit a hydrogen atom is 24as or 120 entropic ticks and the orbit velocity is 2.18*10^6 m/s
//Vector max size: 209622091746699450

//TO DO:
//Implement simulate time
//Implement generate solution

class particle {
public:
    short type; //The type of particle: -1 electron, 0 neutron, 1 proton
    //Define the kinematic values for particles
    long long int position[3];
    long long int velocity[3];
    long long int acceleration[3];
    unsigned long long int injectiontime; //For input particles, the time the particle appears at the edge of the cube
    unsigned long long int number; //The particle number
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
    if (abs(particle.position[0]) > boundary or abs(particle.position[1]) > boundary or abs(particle.position[2]) > boundary or particle.position[0] == LLONG_MIN or particle.position[1] == LLONG_MIN or particle.position[2] == LLONG_MIN) {
        return false;
    }
    else {
        return true;
    }
}

void validateboundary() {
    //Sequentially checks all input particles if they are within the boundary and erases them if they are not
    for (unsigned long long int i = 0; i < particles.size(); i++) { //Loop through all particles
        if (!withinbound(particles[i])) { //If the particle is not within the boundary, erase it
            particles.erase(particles.begin() + i);
        }
    }
}

//Calculate acceleration by dividing force, which is equal to strong force (calculated from Kq1q2/r^2) plus electromagnetic force (calculated from Kq1q2/r^2 plus gravity force (calculated from Kq1q2/r^2, by mass of the particle as dictated by particle type
long long int calculateacceleration(particle particle, short dimension) {
    //If it's the only particle in the system, we return its current acceleration
    if (particles.size() == 1) {
		return particle.acceleration[dimension];
	}
    //Otherwise, we calculate the multibody interaction of all other particles in the system on this particle
    double distance;
    //Set the particle mass according to the particle type
    //Set particle charge according to the particle type
    //Set the strong force constant according to the particle type
    double mass = 0;
    double charge = 0;
    double mass2 = 0;
    double charge2 = 0;
    if (particle.type == -1) { //Electron
        mass = 9.109383632E-31;
        charge = -1;
    }
    if (particle.type == 0) { //Neutron
        mass = 1.674927485E-27;
        charge = 0;
    }
    if (particle.type == 1) { //Proton
        mass = 1.672621911E-27;
        charge = 1;
    }
    //Set the K values for the calculations
    double kgravity = 6.674E-11;
    double kem = 8.98755179E9;
    //Calculate the force on this particle by summing the force on all other particles in the system
    double force = 0;
    //Loop through all particles in the system
    for (unsigned long long int i = 0; i < particles.size(); i++) {
        if (i != particle.number) { //If the particle is the same as the input particle, we skip it
            //Calculate the distance between the two particles
            distance = 2E-19 * abs(particle.position[dimension] - particles[i].position[dimension]); //Multiply the calculated unit distance by 0.2 attometers
            //Assign the constant values depending on particle type
            if (particles[i].type == -1) { //Electron
                mass2 = 9.109383632E-31;
                charge2 = -1;
            }
            if (particles[i].type == 0) { //Neutron
                mass2 = 1.674927485E-27;
                charge2 = 0;
            }
            if (particles[i].type == 1) { //Proton
                mass2 = 1.672621911E-27;
                charge2 = 1;
            }
            //Perform the calculation of force by adding it to the force variable
			force += (kem * charge * charge2) / (distance * distance);
			force += (kgravity * mass * mass2) / (distance * distance);
        }
    }
    //Convert force to acceleration by dividing by mass and return the acceleration.
    return (force / mass);
}

void updatetick() {  //Run one tick of the simulation for the vector of particles
    //Update acceleration for the particle based on force
    for (unsigned long long int i = 0; i < particles.size(); i++) { //Loop through all particles
        particles[i].acceleration[0] = calculateacceleration(particles[i], 0);
        particles[i].acceleration[1] = calculateacceleration(particles[i], 1);
        particles[i].acceleration[2] = calculateacceleration(particles[i], 2);
    }
    //Update the kinematic values for the particle
    for (unsigned long long int i = 0; i < particles.size(); i++) { //Loop through all particles
        particles[i].velocity[0] += particles[i].acceleration[0];
        particles[i].velocity[1] += particles[i].acceleration[1];
        particles[i].velocity[2] += particles[i].acceleration[2];
    }
    for (unsigned long long int i = 0; i < particles.size(); i++) { //Loop through all particles
        particles[i].position[0] += particles[i].velocity[0];
        particles[i].position[1] += particles[i].velocity[1];
        particles[i].position[2] += particles[i].velocity[2];
    }
    validateboundary();
}

int menu() {
    validateboundary();
    short input = 0;
    cout << "A by arc\n";
    cout << "File Loaded: " << filename << "\n";
    cout << "Select an option:\n";
    cout << "0. Exit\n";
    cout << "1. About\n";
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
    return input;
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
    cout << "Selection: ";
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
	particle.number = particles.size();
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
        cout << "   Velocity: (" << particles[i].velocity[0] << ", " << particles[i].velocity[1] << ", " << particles[i].velocity[2] << ")" << endl;
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
    cout << endl;
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
                    particle.number = particles.size();
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
    //Computational code goes here. Write the result to solutionfile.
    cout << endl;
}

void simulatetime() {
    unsigned long long int ticks = 0;
    cout << "For how many ticks: ";
    cin >> ticks;
    for (unsigned long long int i = 1; i <= ticks; i++) {
        cout << "Processing tick: " << i << endl;
        updatetick();
    }
    cout << endl;
}

void readsolution() {
    cout << "Enter the solution file to read: ";
	cin >> solutionfile;
    solutionfile = solutionfile + ".solution";
    ifstream file(solutionfile);
    string line;
    while (getline(file, line)) {
        cout << line << endl;
    }
    cout << endl;
}

void about() {
    cout << "A by arc" << endl;
    cout << "Version: 1.0" << endl;
    cout << "Description: A program to simulate the motion of particles in a 3D space" << endl;
    cout << "Technical Specifications:" << endl;
    cout << "Space Percision: 0.2 attometers" << endl;
    cout << "Time Percision: 0.2 attoseconds" << endl;
    cout << "Maximum Simulation Volume: 3 meters x 3 meters x 3 meters" << endl;
    cout << "Maximum Simulation Time: 3 seconds" << endl;
    cout << "Particle Limit: 209622091746699450" << endl;
    cout << endl;
}

int main() {
    short input = 100;
    while (input != 0) {
        input = menu();
        if (input == 1) {
            about();
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