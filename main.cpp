// note, this code only works if the tower ID can also be used to index it in an array.

#include <iostream>
#include <cmath>
#include <fstream>
using namespace std;

double distance(int x1, int y1, int x2, int y2);
double strength(double r, int R);
string is_closest(double distance, double closest);

class Phone { // Phone object.
public:
    int id, x, y, towerID;
    double strength, distance, closest;
    double tower_ids[1000][3]; // [0] = ID, [1] = distance, [2] = strength
};

class Tower { // Tower object.
public:
    int id, x, y, R, capacity; //ID = tower ID, R = radius, x&y are coordinates
};

int main(int argc, const char * argv[]) {
    //reads in from towers and phones files.
    //Calculates tower with strongest signal for each phone.
    //Outputs list of phones matched with towers and their data
    
    //initialize files
    ifstream phone_file("phones.txt");
    ifstream tower_file("towers.txt");
    
    //intake tower capacity
    int tower_capacity;
    cout << "Enter tower capacity:" << endl;
    cin >> tower_capacity;
    
    //intake total number of towers and phones
    int phone_number, tower_number;
    phone_file >> phone_number;
    tower_file >> tower_number;
    
    //Initialize Tower and phone object arrays
    Tower towers[tower_number];
    Phone phones[phone_number];
    
    //intake data into tower array
    for (int i = 0; i < tower_number; i++) {
        tower_file >> towers[i].id >> towers[i].x >> towers[i].y >> towers[i].R;
        towers[i].capacity = tower_capacity;
    }
    
    //intake data into Phone array. Mark extra variables
    for (int i = 0; i < phone_number; i++) {
        phone_file >> phones[i].id >> phones[i].x >> phones[i].y;
        phones[i].towerID = -1; // used as initial comparison value
        phones[i].closest = 10000; // used as initial comparison value
        phones[i].strength = -10000; // used as initial comparison value
    }
    
    //close files
    phone_file.close();
    tower_file.close();
    
    //create list of tower id's, strengths, and distances in each phone
    for (int i = 0; i < phone_number; i++) {
        for (int j = 0; j < tower_number; j++) {
            phones[i].tower_ids[j][0] = towers[j].id;
            phones[i].tower_ids[j][1] = distance(phones[i].x, phones[i].y, towers[j].x, towers[j].y);
            phones[i].tower_ids[j][2] = strength(phones[i].tower_ids[j][1], towers[j].R);
        }
    }
    
    // Maps phones to towers and finds closest tower for each phone
    for (int i = 0; i < phone_number; i++) { // i indexes phones
        for (int j = 0; j < tower_number; j++) { // j indexes towers
            // find closest tower
            if(phones[i].tower_ids[j][1] < phones[i].closest) phones[i].closest = phones[i].tower_ids[j][1];
            
            // Compares strength and checks tower capacity before mapping phone to tower
            if((phones[i].strength <= phones[i].tower_ids[j][2]) and (towers[j].capacity != 0)){
                if(phones[i].towerID != -1){
                    towers[phones[i].towerID].capacity +=1; // increase capacity of old tower if switching away
                }
                towers[j].capacity -= 1; // decrease capacity
                phones[i].towerID = towers[j].id; // change towerID to new tower.
                phones[i].distance = phones[i].tower_ids[j][1]; // set distance
                phones[i].strength = phones[i].tower_ids[j][2]; // set strength
            }
        }
    }
    
    // FINAL OUTPUT LOOP
    cout << "Phone_ID  Tower_ID  Distance  Signal_strength  Closest_tower" << endl;
    for (int i = 0; i < phone_number; i++) { // loops through phones[i] array
        cout << phones[i].id << "         " << phones[i].towerID << "         "  << phones[i].distance
        << "   " << phones[i].strength << "          " << is_closest(phones[i].distance, phones[i].closest) << endl;
    }
    return 0;
}
double distance(int x1, int y1, int x2, int y2){
    // use pythagorean's thm to find distance
    return(sqrt(pow(x2-x1, 2) + pow(y2-y1, 2)));
}
double strength(double r, int R){
    // r is distance, R is Tower radius, output is signal strength by given formula
    return (-113.0-40.0*log10(r/R));
}
string is_closest(double distance, double closest){
    //returns string "yes" or "no" if tower with best service is closest.
    if(closest == distance){
        return ("yes");
    }
    return("no");
}
