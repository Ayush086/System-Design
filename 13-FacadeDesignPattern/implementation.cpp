/* FACADE DESIGN PATTERN                                                                                    03/06/2026 */
#include <iostream>
using namespace std;

// subsystems
class PowerSupply{
public:
    void providePower() {
        cout << "Power Supply: providing power..." << endl;
    }
};

class CoolingSystem {
public:
    void startFans() {
        cout << "Cooling System: fans started..." << endl;
    }
};

class CPU {
public:
    void init() {
        cout << "CPU: Initialization started..." << endl;
    }
};

class Memory {
public:
    void selfTest() {
        cout << "Memory: self-test passed..." << endl;
    }
};

class HardDrive{
public:
    void spinUp() {
        cout << "Hard Drive: Spinning Up..." << endl;
    }
};

class BIOS{
public:
    void boot(CPU& cpu, Memory& m) {
        cout << "BIOS: Booting CPU and Memory checks..." << endl;
        cpu.init();
        m.selfTest();
    }
};

class OperatingSystem{
public:
    void load() {
        cout << "Operating System: Loading into memory..." << endl;
    }
};


// Facade
class ComputerFacade{
private:
    PowerSupply power;
    CoolingSystem coolingSystem;
    CPU cpu;
    Memory memory;
    HardDrive hardDrive;
    BIOS bios;
    OperatingSystem os;

public:
    void startComputer() {
        cout << "----- Starting Computer -----" << endl;
        power.providePower();
        coolingSystem.startFans();
        bios.boot(cpu, memory);
        hardDrive.spinUp();
        os.load();
        cout << "Computer Booted Successfully!" << endl;
    }
};


// client
int main() {
    ComputerFacade computer;
    computer.startComputer();

    return 0;
}