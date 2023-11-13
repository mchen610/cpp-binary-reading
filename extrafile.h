#include <string>
#include <vector>
#pragma once
struct Weapon
{   
    std::string name;
    int powerRating = 0;
    float powerConsumption;
};

struct Ship
{
    std::string name;
    std::string shipClass;
    short length;
    int shieldCapacity;
    float warpSpeed;
    int numWeapons;
    std::vector<Weapon> weapons;
    Weapon *strongestWeapon;
    int totalPower = 0;
};