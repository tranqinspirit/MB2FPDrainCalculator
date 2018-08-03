#define JEDI_REVAMP_V1
#define IDR_RANGE_FAR 500
#define IDR_RANGE_CLOSE 100
#define IDR_RANGE_FAR_BETA 375
#define IDR_RANGE_CLOSE_BETA 160

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class Weapon {
public:
	std::string name; // Name for picking
	int damage; // Base damage of the weapon
	float FPDrains[2]; // Should be 2 values: Open Beta, Public/Live
	bool bCanBreakThreshold; // If there's a need to worry about the 55 damage threshold

	Weapon(std::string Name, int baseDamage, float betaFPDrain, float liveFPDrain)
	{
		name = Name;
		damage = baseDamage;
		FPDrains[0] = betaFPDrain;
		FPDrains[1] = liveFPDrain;

		if (damage >= 55)
		{
			bCanBreakThreshold = true;
		}
	}
};

class Player {
	std::string playerClass;
	Weapon playerWeapon;

};

typedef enum {
	MOD_DISRUPTOR_SNIPER, // 28
	MOD_DISRUPTOR, // 24 non-charged
	MOD_DISRUPTOR_SPLASH,
	MOD_BOWCASTER, // 30
	MOD_BOWCASTER_CHARGED,  // varies
	MOD_VEHICLE, // 25
	MOD_REPEATER, // 20
	MOD_BRYAR_PISTOL, // 26 (< level 3) | 40 (level 3) 
	MOD_BRYAR_PISTOL_ALT, // varies
	MOD_BRYAR_OLD, // 28
	MOD_BRYAR_OLD_ALT, // varies
	MOD_CONC, // 26
	MOD_DEMP2, // 28
	MOD_FLECHETTE, // 28 (24 for ARC), 100 (EE3 sniper), 47 (M5 sniper)
	MOD_FLECHETTE_ALT_SPLASH,
	MOD_T21, // 60 
	MOD_T21ALT, // 26
	MOD_SHOTGUN,
	MOD_BLASTER, // 28 (primary)
	MOD_BLASTER_ALT, //24 (altfire)
	MOD_LASER // 50
} weapon;


void initializeWeaponInfo(std::vector<Weapon>);

// Change all of these as you want
// Shooter options
bool IsInSpecialMove = false;
bool PM_InGetUpAnimation = false;

weapon OurGun = MOD_VEHICLE; // Weapon to use multipliers for 

							 // Target options
int CustomDistance = 0; // If you want a specific distance
bool bInJump = false; // Jumping/in the air
bool bAttacking = false; // Swinging
int DefLevel = 2; // Saber defense level


int main()
{
	// Create list of weapons
	std::vector<Weapon> WeaponList;

	// Fill in all of the info from the text file
	initializeWeaponInfo(WeaponList);

	std::string MBClasses[] = {
		"Soldier", "Commander",// "Jedi",
		"Hero", "Wookiee", "Clone", "ARC", // Rebels
		"Mandalorian",  "Droideka", "SBD",  "Bounty Hunter" // Imperials
	};


	int weaponCount = 0;
	int weaponLevel = 1;
	int chargeTicks = 0;
	float overrideMultiplier = 1.0f; // For testing replacements to the Live drains
	float nForcePowerNeededToBlock = 0;

	int overrideDamage = 0;
	bool OnlyOneSetOfDrains = false; // Print values for both sets of drains
	bool publicFPDrains = true; // True for public, false for beta
	bool publicFPDrainsTweaked = true; // Display results of tweaked modifiers

	bool printAll = false;
	// Go through all of the weapons
	if (printAll)
	{
		for (std::vector<Weapon>::iterator it = WeaponList.begin(); it != WeaponList.end(); ++it)
		{

			if (it->bCanBreakThreshold && it->damage >= 55)
			{
				// Override the multiplier
				switch (OurGun)
				{
				MOD_BOWCASTER_CHARGED:
					nForcePowerNeededToBlock = (int)(it->damage / 10.0f*1.5f + 0.5f);
					break;
				MOD_T21:
					nForcePowerNeededToBlock = (int)(it->damage / 10.0f*1.5f + 0.5f);
					break;
				MOD_DISRUPTOR_SNIPER:
				MOD_DISRUPTOR:
				MOD_DISRUPTOR_SPLASH:
					nForcePowerNeededToBlock = (int)(it->damage / 12.0f + 0.5f);
					break;
				default:
					nForcePowerNeededToBlock = (int)(it->damage / 4.0f + 0.5f);
					break;
				}
			}
		}
	}
	else
	{
		// Ask for which weapon to math for

		// Look and see if it's in the list of weapons read in

		// Handle if it's not

		// Otherwise, do the math and whatnot as usual


	}

	return 0;
}

void initializeWeaponInfo(std::vector<Weapon> WeaponList)
{
	std::fstream weaponFile;

	weaponFile.open("WeaponStats.txt");
	while (!weaponFile.eof())
	{
		std::string Name, baseDamage, betaFPDrain, liveFPDrain;

		// Name
		std::getline(weaponFile, Name, ',');
		
		// Base damage that will be used against the various multipliers
		std::getline(weaponFile, baseDamage, ',');

		// FP multiplier from the pre-1.5 open beta
		std::getline(weaponFile, betaFPDrain, ',');

		// FP multiplier currently used (1.5.2 as of this)
		std::getline(weaponFile, liveFPDrain, ',');

		// Create a weapon with the appropriate multipliers and such
		Weapon NewWeapon(Name, std::stoi(baseDamage), std::stof(betaFPDrain), std::stof(liveFPDrain));

		// Push it into the vector to be passed back to main
		WeaponList.push_back(NewWeapon); // Is this going to stay valid out of scope?
	}
}