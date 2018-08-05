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


void initializeWeaponInfo(std::vector<Weapon> &list);
// Distance/blocking (or not) modifiers
int FPDrainsIDR(bool publicFPDrains, float currentFPDrain, int distance, int damage, bool blocking, int DefLevel);

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

	int SaberDefenseLevel = 2;
	int weaponLevel = 1;
	int chargeTicks = 0; // Mainly for specific charging weapons/firing modes
	int baseDamage = 0; // For base damage combined with any stuff like charge ticks..
	float nForcePowerNeededToBlock = 0;
	bool SaberIsBlocking = true;

	int CustomDistance = 0; // For specific distances
	bool OnlyOneSetOfDrains = false; // Print values for both sets of drains
	bool publicFPDrains = true; // True for public, false for beta
	bool publicFPDrainsTweaked = true; // Display results of tweaked modifiers

	bool printAll = true;

	(publicFPDrains) ? std::cout << "Live (1.5.1) Drains\n" : std::cout << "Beta Drains\n";

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
					baseDamage = it->damage + ((int)(chargeTicks * ((48.0f - 26.0f) / 5.0f) + 0.5f));
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
				if (overrideMultiplier && publicFPDrains)
					nForcePowerNeededToBlock = (int)(baseDamage / 10.0f * overrideMultiplier + 0.5f);
				else
					nForcePowerNeededToBlock = (int)(baseDamage / 10.0f * it->FPDrains[publicFPDrains] + 0.5f);
			}
			else if (it->name == "T21 (Primary)")
			{
				if (overrideMultiplier && publicFPDrains)
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
					if (overrideMultiplier && publicFPDrains)
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


		if (SaberIsBlocking)
		{
			if (it->name == "T21 (Primary)" ||
				it->name == "Bowcaster (Charged)" ||
				it->name == "Wrist Laser" ||
				((it->name == "Bryar Pistol(Primary - Level 1)" ||
				it->name == "Bryar Pistol(Primary - Level 2)" ||
				it->name == "Bryar Pistol(Primary - Level 3)") && baseDamage > 30) ||
				(it->name == "Bryar Pistol(Charged)" && baseDamage > 30))
					nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*2.0f + 0.5f);
		}
		else
		{
			if (it->name == "T21 (Primary)" ||
				it->name == "Bowcaster (Charged)" ||
				it->name == "Wrist Laser" ||
				((it->name == "Bryar Pistol(Primary - Level 1)" ||
				it->name == "Bryar Pistol(Primary - Level 2)" ||
				it->name == "Bryar Pistol(Primary - Level 3)") && baseDamage > 30) ||
				(it->name == "Bryar Pistol(Charged)" && baseDamage > 30))
					nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*2.8 + 0.5f);
		}

		// Less duplicate code
		//(publicFPDrains) ? std::cout << "Live (1.5.1) Drains\n" : std::cout << "Beta Drains\n";
		std::cout << "Weapon: " << it->name << std::endl;
		std::cout << "Damage: " << baseDamage << std::endl;

		if (publicFPDrains)
		{
			// Longest range drains
			std::cout << "(Farthest - 500) Blocking: " << FPDrainsIDR(publicFPDrains, nForcePowerNeededToBlock, IDR_RANGE_FAR, baseDamage, true, SaberDefenseLevel)
				<< " | Not Blocking: " << FPDrainsIDR(publicFPDrains, nForcePowerNeededToBlock, IDR_RANGE_FAR, baseDamage, false, SaberDefenseLevel) << std::endl;
			// Medium range drains
			std::cout << "(Midpoint - 300) Blocking: " << FPDrainsIDR(publicFPDrains, nForcePowerNeededToBlock, IDR_MIDPOINT, baseDamage, true, SaberDefenseLevel)
				<< " | Not Blocking: " << FPDrainsIDR(publicFPDrains, nForcePowerNeededToBlock, IDR_MIDPOINT, baseDamage, false, SaberDefenseLevel) << std::endl;
			// Closest range drains
			std::cout << "(Closest - 100) Blocking: " << FPDrainsIDR(publicFPDrains, nForcePowerNeededToBlock, IDR_RANGE_CLOSE, baseDamage, true, SaberDefenseLevel)
				<< " | Not Blocking: " << FPDrainsIDR(publicFPDrains, nForcePowerNeededToBlock, IDR_RANGE_CLOSE, baseDamage, false, SaberDefenseLevel) << std::endl;
		}
		else
		{
			// Longest range drains
			std::cout << "(Farthest - 375) Blocking: " << FPDrainsIDR(publicFPDrains, nForcePowerNeededToBlock, IDR_RANGE_FAR_BETA, baseDamage, true, SaberDefenseLevel)
				<< " | Not Blocking: " << FPDrainsIDR(publicFPDrains, nForcePowerNeededToBlock, IDR_RANGE_FAR_BETA, baseDamage, false, SaberDefenseLevel) << std::endl;
			// Medium range drains
			std::cout << "(Midpoint - 267) Blocking: " << FPDrainsIDR(publicFPDrains, nForcePowerNeededToBlock, IDR_MIDPOINT_BETA, baseDamage, true, SaberDefenseLevel)
				<< " | Not Blocking: " << FPDrainsIDR(publicFPDrains, nForcePowerNeededToBlock, IDR_MIDPOINT_BETA, baseDamage, false, SaberDefenseLevel) << std::endl;
			// Closest range drains
			std::cout << "(Closest - 160) Blocking: " << FPDrainsIDR(publicFPDrains, nForcePowerNeededToBlock, IDR_RANGE_CLOSE_BETA, baseDamage, true, SaberDefenseLevel)
				<< " | Not Blocking: " << FPDrainsIDR(publicFPDrains, nForcePowerNeededToBlock, IDR_RANGE_CLOSE_BETA, baseDamage, false, SaberDefenseLevel) << std::endl;
		}

		if (CustomDistance)
		{
			std::cout << "(Distance Custom) Blocking: " << FPDrainsIDR(publicFPDrains, nForcePowerNeededToBlock, CustomDistance, baseDamage, true, SaberDefenseLevel)
				<< " | Not Blocking: " << FPDrainsIDR(publicFPDrains, nForcePowerNeededToBlock, CustomDistance, baseDamage, false, SaberDefenseLevel) << std::endl;
		}

		std::cout << std::endl;
	}


	// Gotta think about calculating stuff like draining from full to zero FP based on firing times and FP regen and such but that's for after the baseline is all done
	// Probably even a completely separate thing from all of the above (would need to calculate distance/FP drain > movement + FP regen to zero..
	system("pause");

	return 0;
}

enum {
	FORCE_LEVEL_0,
	FORCE_LEVEL_1,
	FORCE_LEVEL_2,
	FORCE_LEVEL_3
};

int FPDrainsIDR(bool publicFPDrains, float nForcePowerNeededToBlock, int distance, int damage, bool blocking, int DefLevel)
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

		if (blocking)
		{
			if (bOutsideIDR)
			{
				//nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*0.8f + 0.5f); // 0.8f; // No flat value.
				nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*1.2f + 0.5f);
			}
			else if (bInsideMediumIDR)
			{ // Smooth (linear) transition of drains from long to close.
				float a = 0;
				float b = 0;
				float fDrainBasedOnDist = 0;

				a = (1.2f - 2.0f) / ((float)(IDR_RANGE_FAR - IDR_RANGE_CLOSE));
				b = (IDR_RANGE_FAR * 2.0f - IDR_RANGE_CLOSE * 1.2f) / ((float)(IDR_RANGE_FAR - IDR_RANGE_CLOSE));
				fDrainBasedOnDist = a*((float)distance) + b;

				nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*fDrainBasedOnDist + 0.5f); // 1.5f;
			}
			else if (bInsideCloseIDR)
			{

				nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*2.0f + 0.5f);
			}
			
		}
		else
		{

			if (bOutsideIDR)
			{
				//nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*2.0f + 0.5f); // 1.3f;
				nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*2.0f + 0.5f);
			}
			else if (bInsideMediumIDR)
			{ // Smooth (linear) transition of drains from long to close.
				float a = 0;
				float b = 0;
				float fDrainBasedOnDist = 0;

				a = (2.0f - 2.8f) / ((float)(IDR_RANGE_FAR - IDR_RANGE_CLOSE));
				b = (IDR_RANGE_FAR * 2.8f - IDR_RANGE_CLOSE * 2.0f) / ((float)(IDR_RANGE_FAR - IDR_RANGE_CLOSE));
				fDrainBasedOnDist = a*((float)distance) + b;

				nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*fDrainBasedOnDist + 0.5f); // 2.2f;

			}
			else if (bInsideCloseIDR)
			{
				//nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*3.0f + 0.5f); //3.2f;
				nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*2.8f + 0.5f);
			}		
		}
		if (bInJump)
		{
			nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*1.5f + 0.5f);
		}

		if (DefLevel < FORCE_LEVEL_1)
			nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*3.0f + 0.5f);
		else if (DefLevel < FORCE_LEVEL_2)
			nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*1.6f + 0.5f);

		// Decrease FP drains while doing special moves, so that they're less crippled
		if (IsInSpecialMove)
			nForcePowerNeededToBlock = (int)(0.5f*nForcePowerNeededToBlock + 0.5f);

		// Cap FP drains no matter what.
		if (DefLevel > FORCE_LEVEL_1)
		{
			if (blocking)
			{
				if (nForcePowerNeededToBlock > 40)
				{
					nForcePowerNeededToBlock = 40;
				}
			}
			else
			{
				if (nForcePowerNeededToBlock > 80)
				{
					nForcePowerNeededToBlock = 80;
				}
			}
		}
		else
		{
			if (blocking)
			{
				if (nForcePowerNeededToBlock > 60)
				{
					nForcePowerNeededToBlock = 60;
				}
			}
			else
			{
				if (nForcePowerNeededToBlock > 120)
				{
					nForcePowerNeededToBlock = 120;
				}
			}
		}
	}
	else
	{
		// For IDR
		float a, b, fDrainBasedOnDist;

		bOutsideIDR = (distance > IDR_RANGE_FAR_BETA);
		bInsideMediumIDR = ((distance > IDR_RANGE_CLOSE_BETA) && (distance <= IDR_RANGE_FAR_BETA)); // No need to distinguish for Q3
		bInsideCloseIDR = (distance <= IDR_RANGE_CLOSE_BETA);

		// Distance based FP drains
		// No differing modifiers, just all one set of math
		a = 0;
		b = 0;
		fDrainBasedOnDist = 0;

		a = (2.0f - 3.0f) / ((float)(IDR_RANGE_FAR_BETA - IDR_RANGE_CLOSE_BETA));
		b = (IDR_RANGE_FAR * 3.0f - IDR_RANGE_CLOSE_BETA * 2.0f) / ((float)(IDR_RANGE_FAR_BETA - IDR_RANGE_CLOSE_BETA));

		// Cap the distance so there's a point to shooting saberists at longer ranges
		// Separate blocking/not blocking so that weapons can be further tweaked as necessary
		if (blocking)
		{
			if (bOutsideIDR)
			{
				fDrainBasedOnDist = a * ((float)IDR_RANGE_FAR_BETA) + b;
			}
			else if (bInsideMediumIDR)
			{
				fDrainBasedOnDist = a*((float)distance) + b;
			}
			else if (bInsideCloseIDR)
			{
				fDrainBasedOnDist = a * ((float)IDR_RANGE_CLOSE_BETA) + b;
			}
		}
		else
		{
			if (bOutsideIDR)
			{
				fDrainBasedOnDist = a * ((float)IDR_RANGE_FAR_BETA) + b;
			}
			else if (bInsideMediumIDR)
			{
				fDrainBasedOnDist = a*((float)distance) + b;
			}
			else if (bInsideCloseIDR)
			{
				fDrainBasedOnDist = a * ((float)IDR_RANGE_CLOSE_BETA) + b;
			}
		}

		// Apply the multiplier
		nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*fDrainBasedOnDist + 0.5f);

		if ((DefLevel < FORCE_LEVEL_3) && bAttacking && (blocking))
		{ // Swingblock has additional FP drain if you don't have saber defense lvl 3. If you have saber defense lvl 3, you get same drains for swingblocking as for blocking.
			nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*2.0f + 0.5f);
		}


		if (bClientT3Enabled && (DefLevel < FORCE_LEVEL_3) && ((bAttacking && !(blocking)) || PM_InGetUpAnimation))// Q3 gets a 2.0 FP drain multiplier for attack-defending if player doesn't have saber defense level 3. Otherwise no additional drain.
			nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*2.0f + 0.5f);


		// Just going to assume on the ground for the sake of this calculator
		//if (bInJump)
		//{
		//	nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*1.7f + 0.5f);
		//}

		// FP modifiers based on saber defense levels
		if (DefLevel < FORCE_LEVEL_1)
			nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*3.0f + 0.5f);
		else if (DefLevel < FORCE_LEVEL_2)
			nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*1.6f + 0.5f);

		// Decrease FP drains while doing special moves, so that they're less crippled
		if (IsInSpecialMove)
			nForcePowerNeededToBlock = (int)(0.5f*nForcePowerNeededToBlock + 0.5f);

		// Blocking / non-blocking modifiers
		if (blocking)
		{
			nForcePowerNeededToBlock = (int)(0.65f * nForcePowerNeededToBlock + 0.5f);

			// Cap FP drains if the saberist is actually blocking..
			if (nForcePowerNeededToBlock > 50.0f)
			{
				nForcePowerNeededToBlock = 50.0f;
			}
		}
		else
		{
			nForcePowerNeededToBlock = (int)(0.95f*nForcePowerNeededToBlock + 0.5f);
		}

		// Also cap weapons that might have huge FP drains by default (why did this get removed?)
		if (bOutsideIDR && nForcePowerNeededToBlock > 40.0f)
		{
			nForcePowerNeededToBlock = 40.0f;
		}
	}

	return (int)nForcePowerNeededToBlock;
}

void initializeWeaponInfo(std::vector<Weapon> &WeaponList)
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
		std::getline(weaponFile, liveFPDrain);

		// Create a weapon with the appropriate multipliers and such
		Weapon NewWeapon(Name, std::stoi(baseDamage), std::stof(betaFPDrain), std::stof(liveFPDrain));

		// Push it into the vector to be passed back to main
		WeaponList.push_back(NewWeapon); // Is this going to stay valid out of scope?
	}
}
