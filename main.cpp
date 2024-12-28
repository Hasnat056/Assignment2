#include <iostream>
#include <vector>
#include <sstream>
#include <memory>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

vector<string> species;

int tick(int n) {
    for (long long int i = 0; i < (10000000000 / n); i++) {
    }
    return 1;
}

enum Roomtype { Resting, queen, warrior, drone, working };

class AntRoom {
protected:
    Roomtype type;
    int numberofants;
    int capacity;
    int buildingticks;
    int ticksdone;

public:
    explicit AntRoom(Roomtype type) {
        this->type = type;
        numberofants = 0;
        if (this->type == Resting) {
            this->capacity = 50;
            this->buildingticks = 100;
        } else if (this->type == queen) {
            this->capacity = 1;
            this->buildingticks = 200;
        } else if (this->type == warrior) {
            this->capacity = 70;
            this->buildingticks = 150;
        } else if (this->type == drone) {
            this->capacity = 100;
            this->buildingticks = 80;
        } else if (this->type == working) {
            this->capacity = 150;
            this->buildingticks = 300;
        } else {
            cout << "Invalid room type" << endl;
        }
        ticksdone = 0;
    }

    bool buildroom(int ants_working) {
        while (this->ticksdone < this->buildingticks) {
            this->ticksdone += tick(ants_working);
        }
        return true;
    }

    bool addAnts(int n) {
        if (this->numberofants + n <= this->capacity) {
            this->numberofants += n;
            return true;
        }
        return false;
    }

    bool restAnts(int count) {
        if (count <= numberofants) {
            numberofants -= count; // Remove ants from the room
            return true; // Resting successful
        }
        return false; // Not enough ants to rest
    }

    int getCapacity() const { return capacity; }
    int getCurrentAnts() const { return numberofants; }
    Roomtype getType() const { return type; }
};

class Ant {
public:
    virtual string getType() = 0;
    virtual ~Ant() = default;
    virtual void performAction() = 0; // Each ant will perform its own action
};

class DroneAnt : public Ant {
public:
    int number;
    string getType() override {
        return "Drone";
    }

    void performAction() override {
        cout << "Drone is searching for food." << endl;
        // Implement food search logic here
    }
};

class WarriorAnt : public Ant {
public:
    int number;
    string getType() override {
        return "Warrior";
    }

    void performAction() override {
        cout << "Warrior is hunting." << endl;
        // Implement hunting logic here
    }
};

class WorkerAnt : public Ant {
public:
    int number;
    string getType() override {
        return "Worker";
    }

    void performAction() override {
        cout << "Worker is gathering resources." << endl;
        // Implement resource gathering logic here
    }
};

class QueenAnt : public Ant {
public:
    bool alive;
    string getType() override {
        return "Queen";
    }

    void performAction() override {
        cout << "Queen is laying eggs." << endl;
        // Implement egg-laying logic here
    }
};

class AntFactory {
public:
    static QueenAnt* createQueen() {
        return new QueenAnt();
    }

    static DroneAnt* createDrone() {
        return new DroneAnt();
    }

    static WarriorAnt* createWarrior() {
        return new WarriorAnt();
    }
};

template <typename AntSpecies>
class AntFarm {
    int uniqueID;
    int status;
    string species;
    vector<AntRoom*> rooms;
    QueenAnt* queen;
    WorkerAnt* worker;
    WarriorAnt* warrior;
    DroneAnt* drone;
    int AntKills;
    vector<int> ColonyKills;
    int x, y;
    int food, water;

public:
    AntFarm(int x, int y, string species, int ID)
        : x(x), y(y), species(std::move(species)), uniqueID(ID),
          status(0), AntKills(0), queen(nullptr), worker(nullptr), warrior(nullptr), drone(nullptr),
          food(0), water(0) {}

    void addRoom(Roomtype type) {
        rooms.push_back(new AntRoom(type));
        int workers = (worker) ? worker->number : 0;
        rooms.back()->buildroom(workers);
    }

    void addResources(int a, int b, int c, int d, int e) {
        food += a;
        water += b;
        if (!worker) worker = new WorkerAnt();
        worker->number += c;
        if (!warrior) warrior = new WarriorAnt();
        warrior->number += d;
        if (!drone) drone = new DroneAnt();
        drone->number += e;
    }

    void display() {
        cout << "Species: " << species << endl;
        cout << "Workers: " << (worker ? worker->number : 0) << endl;
        cout << "Warriors: " << (warrior ? warrior->number : 0) << endl;
        cout << "Drones: " << (drone ? drone->number : 0) << endl;
        cout << "Ant Kills: " << AntKills << endl;
        cout << "Colony Kills: " << ColonyKills.size() << endl;
        if (!ColonyKills.empty()) {
            for (size_t i = 0; i < ColonyKills.size(); i++) {
                cout << "(" << ColonyKills[i] << ":" << species[i] << ") ";
            }
            cout << endl;
        }
        cout << "Status: " << (status == 0 ? "Alive" : "Killed") << endl;
    }

    void performActions() {
        if (queen) queen->performAction();
        if (worker) worker->performAction();
        if (warrior) warrior->performAction();
        if (drone) drone->performAction();
    }

    void battle(Ant* attacker, Ant* defender) {
        // Simulate a battle
        cout << attacker->getType() << " attacks " << defender->getType() << endl;
        if (rand() % 2) {
            cout << attacker->getType() << " wins!" << endl;
            AntKills++;
            // Transfer attributes logic here
        } else {
            cout << defender->getType() << " wins!" << endl;
            // Transfer attributes logic here
        }
    }
};

class Meadow {
    static Meadow* instance;
    vector<AntFarm<Ant>*> AntFarms;

    Meadow() = default;

public:
    static Meadow* getInstance() {
        if (!instance) {
            instance = new Meadow();
        }
        return instance;
    }

    void spawn() {
        string x;
        cin >> x;
        istringstream ss(x);
        int num1, num2;
        string name;
        ss >> num1 >> num2 >> name;
        AntFarms.push_back(new AntFarm<Ant>(num1, num2, name, AntFarms.size()));
        species.push_back(name);
        cout << "Spawned colony: " << name << " at (" << num1 << ", " << num2 << ")" << endl;
    }

    void give(int n) {
        string x;
        cin >> x;
        istringstream ss(x);
        int food, water, drone, warrior, worker;
        ss >> food >> water >> worker >> drone >> warrior;
        AntFarms.at(n)->addResources(food, water, worker, drone, warrior);
    }

    void tick() {
        for (auto& farm : AntFarms) {
            farm->performActions(); // Each ant performs its action
        }
    }

    void summary(int n) {
        if (n > AntFarms.size()) {
            cout << "Colony Doesn't exist" << endl;
        } else {
            AntFarms[n - 1]->display();
        }
    }
};

Meadow* Meadow::instance = nullptr;

int main() {
    srand(static_cast<unsigned int>(time(0))); // Seed for random number generation

    cout << "Welcome to the Ant Colony Simulation!" << endl;
    cout << "Commands:" << endl;
    cout << "1. spawn X Y Species - Spawn a new ant colony at (X, Y) of the given species." << endl;
    cout << "2. give ID food/water/worker/drone/warrior - Give resources to the specified colony." << endl;
    cout << "3. tick - Perform a tick for all colonies." << endl;
    cout << "4. summary ID - Get a summary of the specified colony." << endl;
    cout << "5. exit - Exit the simulation." << endl;

    string command;
    while (true) {
        cout << "> "; // Prompt for user input
        getline(cin, command);
        if (command == "exit") break;

        istringstream ss(command);
        string cmd;
        ss >> cmd;

        if (cmd == "spawn") {
            Meadow::getInstance()->spawn();
        } else if (cmd == "give") {
            int id;
            ss >> id;
            Meadow::getInstance()->give(id);
        } else if (cmd == "tick") {
            Meadow::getInstance()->tick();
        } else if (cmd == "summary") {
            int id;
            ss >> id;
            Meadow::getInstance()->summary(id);
        } else {
            cout << "Unknown command. Please try again." << endl;
        }
    }

    cout << "Exiting the simulation. Thank you for playing!" << endl;
    return 0;
}