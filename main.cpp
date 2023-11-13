#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "extrafile.h"
using namespace std;

void addShips(const string &fileName, vector<Ship> &ships)
{
    ifstream file(fileName, ios::binary);
    int numShips;
    file.read(reinterpret_cast<char *>(&numShips), sizeof(numShips));
    for (int i = 0; i < numShips; i++)
    {
        Ship ship;

        int nameLength;
        file.read(reinterpret_cast<char *>(&nameLength), sizeof(nameLength));
        char *nameBuffer = new char[nameLength];
        file.read(nameBuffer, nameLength);
        ship.name = nameBuffer;
        delete[] nameBuffer;
        nameBuffer = nullptr;

        int classLength;
        file.read(reinterpret_cast<char *>(&classLength), sizeof(classLength));
        char *classBuffer = new char[classLength];
        file.read(classBuffer, classLength);
        ship.shipClass = classBuffer;
        delete[] classBuffer;
        classBuffer = nullptr;

        file.read(reinterpret_cast<char *>(&ship.length), sizeof(ship.length));
        file.read(reinterpret_cast<char *>(&ship.shieldCapacity), sizeof(ship.shieldCapacity));
        file.read(reinterpret_cast<char *>(&ship.warpSpeed), sizeof(ship.warpSpeed));
        file.read(reinterpret_cast<char *>(&ship.numWeapons), sizeof(ship.numWeapons));

        for (int j = 0; j < ship.numWeapons; j++)
        {
            Weapon weapon;
            int nameLength;
            file.read(reinterpret_cast<char *>(&nameLength), sizeof(nameLength));
            char *nameBuffer = new char[nameLength];
            file.read(nameBuffer, nameLength);
            weapon.name = nameBuffer;
            delete[] nameBuffer;
            nameBuffer = nullptr;

            file.read(reinterpret_cast<char *>(&weapon.powerRating), sizeof(weapon.powerRating));
            ship.totalPower += weapon.powerRating;
            file.read(reinterpret_cast<char *>(&weapon.powerConsumption), sizeof(weapon.powerConsumption));
            ship.weapons.push_back(weapon);
        }

        if (!ship.weapons.empty())
        {
            ship.strongestWeapon = &ship.weapons[0];
            for (Weapon &weapon : ship.weapons)
            {
                if (weapon.powerRating > ship.strongestWeapon->powerRating)
                {
                    ship.strongestWeapon = &weapon;
                }
            }
        }

        ships.push_back(ship);
    }
}

void printShip(const Ship &ship)
{
    cout << "Name: " << ship.name << '\n';
    cout << "Class: " << ship.shipClass << '\n';
    cout << "Length: " << ship.length << '\n';
    cout << "Shield capacity: " << ship.shieldCapacity << '\n';
    cout << "Maximum Warp: " << ship.warpSpeed << '\n';
    cout << "Armaments:" << '\n';
    if (ship.numWeapons == 0)
    {
        cout << "Unarmed" << '\n';
    }
    else
    {
        for (Weapon weapon : ship.weapons)
        {
            cout << weapon.name << ", " << weapon.powerRating << ", " << weapon.powerConsumption << '\n';
        }
        cout << "Total firepower: " << ship.totalPower << '\n';
    }
    cout << '\n';
}

void printShips(vector<Ship> &ships)
{
    for (Ship &ship : ships)
    {
        printShip(ship);
    }
}

int main()
{
    cout << "Which file(s) to open?\n";
    cout << "1. friendlyships.shp" << '\n';
    cout << "2. enemyships.shp" << '\n';
    cout << "3. Both files" << '\n';
    int option;
    cin >> option;
    vector<Ship> ships;
    if (option == 1)
    {
        addShips("friendlyships.shp", ships);
    }
    else if (option == 2)
    {
        addShips("enemyships.shp", ships);
    }
    else if (option == 3)
    {
        addShips("friendlyships.shp", ships);
        addShips("enemyships.shp", ships);
    }

    cout << "1. Print all ships" << endl;
    cout << "2. Starship with the strongest weapon" << endl;
    cout << "3. Strongest starship overall" << endl;
    cout << "4. Weakest ship (ignoring unarmed)" << endl;
    cout << "5. Unarmed ships" << endl;

    Ship *strongestWeaponShip = nullptr;
    Ship *strongestShip = nullptr;
    Ship *weakestShip = nullptr;

    for (Ship &ship : ships)
    {
        if (!ship.weapons.empty() && (strongestWeaponShip == nullptr || strongestWeaponShip != nullptr && strongestWeaponShip->strongestWeapon->powerRating < ship.strongestWeapon->powerRating))
        {
            strongestWeaponShip = &ship;
        }
        if (strongestShip == nullptr || (strongestShip != nullptr && strongestShip->totalPower < ship.totalPower))
        {
            strongestShip = &ship;
        }
        if (weakestShip == nullptr || (weakestShip != nullptr && ship.numWeapons > 0 && weakestShip->totalPower > ship.totalPower))
        {
            weakestShip = &ship;
        }
    }

    cin >> option;

    if (option == 1)
    {
        printShips(ships);
    }
    else if (option == 2 && strongestWeaponShip != nullptr)
    {
        printShip(*strongestWeaponShip);
    }
    else if (option == 3)
    {
        printShip(*strongestShip);
    }
    else if (option == 4)
    {
        printShip(*weakestShip);
    }
    else if (option == 5)
    {
        for (Ship &ship : ships)
        {
            if (ship.numWeapons == 0)
            {
                printShip(ship);
            }
        }
    }

    return 0;
}