#define JEDI_REVAMP_V1
#define IDR_RANGE_FAR 500
#define IDR_RANGE_CLOSE 100
#define IDR_MIDPOINT ((IDR_RANGE_FAR + IDR_RANGE_CLOSE) / 2)
#define IDR_RANGE_FAR_BETA 375
#define IDR_RANGE_CLOSE_BETA 160
#define IDR_MIDPOINT_BETA ((IDR_RANGE_FAR_BETA + IDR_RANGE_CLOSE_BETA) / 2)

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

void initializeWeaponInfo(std::vector<Weapon>);
// Distance/blocking (or not) modifiers
int FPDrainsIDR(bool publicFPDrains, float currentFPDrain, int distance, int damage, bool blocking);

// Change all of these as you want
// Shooter options
bool IsInSpecialMove = false;
bool PM_InGetUpAnimation = false;

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
		"Soldier", 
		"Commander",// "Jedi",
		"Hero", 
		"Wookiee", 
		"Clone", 
		"ARC", 
		"Mandalorian",  
		"Droideka", 
		"SBD",  
		"Bounty Hunter"
	};

	std::string playerClass = MBClasses[6]; // Player class for some specific multipliers
	std::string desiredWeapon = "E11 (Primary)";
	/* Weapon list for reference to avoid having to swap back and forth between files
	Disruptor(Sniper)
	Disruptor
	Disruptor (Splash)
	Bowcaster
	Bowcaster (Charged)
	Droideka Blasters
	Clone Rifle
	Bryar Pistol (Primary - Level 1)
	Bryar Pistol (Primary - Level 2)
	Bryar Pistol (Primary - Level 3)
	Bryar Pistol (Charged)
	Westar Pistol(s)
	Westar Pistol(s) (Charged)
	SBD Arm Blaster
	Clone Pistol(s)
	M5 Rifle
	M5 Sniper
	EE3 (Primary)
	EE3 (Sniper)
	T21 (Primary)
	T21 (Secondary)
	E11 (Primary)
	E11 (Secondary)
	Wrist Laser
	Shotgun
	*/

	int weaponLevel = 1;
	int chargeTicks = 0; // Mainly for specific charging weapons/firing modes
	int baseDamage = 0; // For base damage combined with any stuff like charge ticks..
	float nForcePowerNeededToBlock = 0;

	int CustomDistance = 0; // For specific distances
	bool OnlyOneSetOfDrains = false; // Print values for both sets of drains
	bool publicFPDrains = true; // True for public, false for beta
	bool publicFPDrainsTweaked = true; // Display results of tweaked modifiers

	bool printAll = false;

	// Ask for which weapon to math for
	// Look and see if it's in the list of weapons read in

	for (std::vector<Weapon>::iterator it = WeaponList.begin(); it != WeaponList.end(); ++it)
	{	
		// Handle if it's not
		if (!printAll && it->name != desiredWeapon)
			continue;

		float overrideMultiplier = 0.0f; // For testing replacements to the Live drains

		// Calculate charge ticks for the appropriate weapons
		if (chargeTicks && 
			(it->name == "Westar Pistol(s) (Charged)" ||
			it->name == "Bryar Pistol (Charged)" ||
			it->name == "Bowcaster (Charged)" ||
			it->name == "Disruptor (Sniper)"))
		{
			if (it->name == "Westar Pistol(s) (Charged)")
			{
				baseDamage = it->damage + (chargeTicks * ((53.0f - 28.0f) / 5.0f) + 0.5f);
			}
			else if (it->name == "Bryar Pistol (Charged)")
			{
				if (weaponLevel > 2)
				{	// Level 3 gets higher base damage; an extra 2 so it's 28 instead of 26 
					baseDamage = it->damage + ((int)(chargeTicks * ((75.0f - 28.0f) / 5.0f) + 0.5f)) + 2;
				}
				else
				{
					baseDamage == it->damage + ((int)(chargeTicks * ((48.0f - 26.0f) / 5.0f) + 0.5f));
				}
			}
			else if (it->name == "Bowcaster (Charged)")
			{
				float chargeTimer = 10.0f;

				// Level 3 charges faster
				if (weaponLevel > 2)
					chargeTimer /= 2;

				baseDamage = it->damage + ((int)(chargeTicks * ((90.0f - 30.0f) / chargeTimer) + 0.5f));
			}
			else if (it->name == "Disruptor (Sniper)")
			{
				// Note that this deals in counts from 10 to 45
				baseDamage = it->damage + chargeTicks;
			}
		}
		// Otherwise just set the base damage to what it normally is
		else
			baseDamage = it->damage;

		// Now do the actual weapon multipliers
		if (it->bCanBreakThreshold && baseDamage >= 55)
		{
			// Override the multiplier
			if (it->name == "Bowcaster (Charged)")
			{
				if (overrideMultiplier)
					nForcePowerNeededToBlock = (int)(baseDamage / 10.0f * overrideMultiplier + 0.5f);
				else
					nForcePowerNeededToBlock = (int)(baseDamage / 10.0f * it->FPDrains[publicFPDrains] + 0.5f);
			}
			else if (it->name == "T21 (Primary)")
			{
				if (overrideMultiplier)
					nForcePowerNeededToBlock = (int)(baseDamage / 10.0f * overrideMultiplier + 0.5f);
				else
					nForcePowerNeededToBlock = (int)(baseDamage / 10.0f * it->FPDrains[publicFPDrains] + 0.5f);
			}
			else if (it->name == "Disruptor" || it->name == "Disruptor (Sniper)" || it->name == "Disruptor (Splash)")
			{
				nForcePowerNeededToBlock = (int)(baseDamage / 12.0f + 0.5f);
			}
			else
			{
				nForcePowerNeededToBlock = (int)(baseDamage / 4.0f + 0.5f);
			}

			// This is only in live drains..
			if (publicFPDrains)
			{
				if (it->name == "EE3 (Sniper)" && playerClass == MBClasses[6]) // Mandalorian
				{
					if (overrideMultiplier)
						nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock * overrideMultiplier + 0.5f);
					else
						nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock * it->FPDrains[publicFPDrains] + 0.5f); // Slightly nerf ee3 sniper
				}
			}
		}

		else
		{
			// Initial modification
			nForcePowerNeededToBlock = (int)(baseDamage / 10.0f + 0.5f);

			// Do multipliers for the current weapon
			nForcePowerNeededToBlock *= it->FPDrains[publicFPDrains];
		}

		// Less duplicate code
		(publicFPDrains) ? std::cout << "Live (1.5.1) Drains\n" : std::cout << "Beta Drains\n";;
		std::cout << "Weapon: \n" << it->name;
		std::cout << "Damage: \n" << baseDamage;

		if (publicFPDrains)
		{
			// Longest range drains
			std::cout << "(Farthest - 500) Blocking: " << FPDrainsIDR(publicFPDrains, nForcePowerNeededToBlock, IDR_RANGE_FAR, baseDamage, true)
				<< " | Not Blocking: \n" << FPDrainsIDR(publicFPDrains, nForcePowerNeededToBlock, IDR_RANGE_FAR, baseDamage, false);
			// Medium range drains
			std::cout << "(Midpoint - 300) Blocking: " << FPDrainsIDR(publicFPDrains, nForcePowerNeededToBlock, IDR_MIDPOINT, baseDamage, true)
				<< " | Not Blocking: \n" << FPDrainsIDR(publicFPDrains, nForcePowerNeededToBlock, IDR_MIDPOINT, baseDamage, false);
			// Closest range drains
			std::cout << "(Closest - 100) Blocking: " << FPDrainsIDR(publicFPDrains, nForcePowerNeededToBlock, IDR_RANGE_CLOSE, baseDamage, true)
				<< " | Not Blocking: \n" << FPDrainsIDR(publicFPDrains, nForcePowerNeededToBlock, IDR_RANGE_CLOSE, baseDamage, false);
		}
		else
		{
			// Longest range drains
			std::cout << "(Farthest - 375) Blocking: " << FPDrainsIDR(publicFPDrains, nForcePowerNeededToBlock, IDR_RANGE_FAR_BETA, baseDamage, true)
				<< " | Not Blocking: \n" << FPDrainsIDR(publicFPDrains, nForcePowerNeededToBlock, IDR_RANGE_FAR_BETA, baseDamage, false);
			// Medium range drains
			std::cout << "(Midpoint - 267) Blocking: " << FPDrainsIDR(publicFPDrains, nForcePowerNeededToBlock, IDR_MIDPOINT_BETA, baseDamage, true)
				<< " | Not Blocking: \n" << FPDrainsIDR(publicFPDrains, nForcePowerNeededToBlock, IDR_MIDPOINT_BETA, baseDamage, false);
			// Closest range drains
			std::cout << "(Closest - 160) Blocking: " << FPDrainsIDR(publicFPDrains, nForcePowerNeededToBlock, IDR_RANGE_CLOSE_BETA, baseDamage, true)
				<< " | Not Blocking: \n" << FPDrainsIDR(publicFPDrains, nForcePowerNeededToBlock, IDR_RANGE_CLOSE_BETA, baseDamage, false);
		}

		if (CustomDistance)
			std::cout << "(Distance Custom) Blocking: " << FPDrainsIDR(publicFPDrains, nForcePowerNeededToBlock, CustomDistance, baseDamage, true)
			<< " | Not Blocking: \n" << FPDrainsIDR(publicFPDrains, nForcePowerNeededToBlock, CustomDistance, baseDamage, false);
	}


	// Gotta think about calculating stuff like draining from full to zero FP based on firing times and FP regen and such but that's for after the baseline is all done
	// Probably even a completely separate thing from all of the above (would need to calculate distance/FP drain > movement + FP regen to zero..


	return 0;
}


int FPDrainsIDR(bool publicFPDrains, float currentFPDrain, int distance, int damage, bool blocking)
{
	// Distance modifiers
	bool bOutsideIDR;
	bool bInsideMediumIDR;
	bool bInsideCloseIDR;
	bool bClientT3Enabled = false;


	if (publicFPDrains)
	{
		bOutsideIDR = (distance > IDR_RANGE_FAR);
		bInsideMediumIDR = ((distance > IDR_RANGE_CLOSE) && (distance <= IDR_RANGE_FAR)); // No need to distinguish for Q3
		bInsideCloseIDR = (distance <= IDR_RANGE_CLOSE);
	}
	else
	{
		// For IDR
		float a, b, fDrainBasedOnDist;

		bOutsideIDR = (distance > IDR_RANGE_FAR_BETA);
		bInsideMediumIDR = ((distance > IDR_RANGE_CLOSE_BETA) && (distance <= IDR_RANGE_FAR_BETA)); // No need to distinguish for Q3
		bInsideCloseIDR = (distance <= IDR_RANGE_CLOSE_BETA);



	}

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