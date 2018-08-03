#define JEDI_REVAMP_V1
#define IDR_RANGE_FAR 500
#define IDR_RANGE_CLOSE 100
#define IDR_RANGE_FAR_BETA 375
#define IDR_RANGE_CLOSE_BETA 160

#include <stdio.h>

typedef enum { false, true } bool;

int publicDrains(int, int, bool);
int publicDrainsTweaked(int, int, bool);
int betaDrains(int, int, bool);

int FORCE_LEVEL_1 = 1;
int FORCE_LEVEL_2 = 2;
int FORCE_LEVEL_3 = 3;

typedef enum {
	MB_CLASS_SOLDIER,
	MB_CLASS_JEDI,
	MB_CLASS_HERO,
	MB_CLASS_MANDALORIAN,
	MB_CLASS_WOOKIE,
	MB_CLASS_DROIDEKA,
	MB_CLASS_CLONETROOPER,
	MB_CLASS_ELITETROOPER,
	MB_CLASS_SBD,
	MB_CLASS_ARCTROOPER,
	MB_CLASS_BOUNTY_HUNTER
} Class;

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

// Change all of these as you want
// Shooter options
bool IsInSpecialMove = false;
bool PM_InGetUpAnimation = false;

Class OurClass = MB_CLASS_DROIDEKA; // Class to use (for some weapon specific multipliers)
weapon OurGun = MOD_VEHICLE; // Weapon to use multipliers for 

							 // Target options
int CustomDistance = 0; // If you want a specific distance
bool bInJump = false; // Jumping/in the air
bool bAttacking = false; // Swinging
int DefLevel = 2; // Saber defense level

int main()
{
	int damage = 0;
	int overrideDamage = 0;
	bool OnlyOneSetOfDrains = false; // Print values for both sets of drains
	bool publicFPDrains = true; // True for public, false for beta
	bool publicFPDrainsTweaked = true; // Display results of tweaked modifiers

	switch (OurGun)
	{
	 MOD_DISRUPTOR_SNIPER:
		damage = 28; break;
	 MOD_DISRUPTOR:
		damage = 24; break;
	  MOD_DISRUPTOR_SPLASH:
		damage = 28; break; //dunno
	  MOD_BOWCASTER:
		damage = 30; break;
	  MOD_BOWCASTER_CHARGED:  // varies
		damage = 30; break;
	  MOD_VEHICLE: // Deka
		damage = 25; break;
	  MOD_REPEATER: // 20
		damage = 20; break;
	  MOD_BRYAR_PISTOL: // 26 (< level 3) | 40 (level 3) 
		damage = 26; break;
	  MOD_BRYAR_PISTOL_ALT: // varies
		damage = 28; break;
	  MOD_BRYAR_OLD: // 28
		damage = 28; break;
	  MOD_BRYAR_OLD_ALT: // varies
		damage = 28; break;
	  MOD_CONC: // 26
		damage = 26; break;
	 MOD_DEMP2: // 28
		damage = 28; break;
	 MOD_FLECHETTE: // 28 (24 for ARC), 100 (EE3 sniper), 47 (M5 sniper)
		damage = 28; break;
	 MOD_FLECHETTE_ALT_SPLASH:
		damage = 28; break; // dunno
	 MOD_T21: // 60 
		damage = 60; break;
	 MOD_T21ALT: // 26
		damage = 26; break;
	 MOD_SHOTGUN:
		damage = 0; break;
	 MOD_BLASTER: // 28 (primary) 
		damage = 28; break;
	 MOD_BLASTER_ALT: // not real in mb2 but for the sake of ease
		damage = 24; break;
	 MOD_LASER: // 50
		damage = 50; break;
	default: 0;
	}

	// Really just for looking at charged shot stuff
	if (overrideDamage)
		damage = overrideDamage;

	// Target Options
	printf("Defense Level: %i\n", DefLevel);

	if (bAttacking)
		printf("Swinging\n");
	else
		printf("Not Swinging\n");

	//printf("============================\n");

	if (OnlyOneSetOfDrains)
	{
		if (publicFPDrains)
		{
			printf("Live (1.5.1) Drains\n");
			printf("Damage: %i\n", damage);
			printf("(Distance 500) Blocking: %i | Not Blocking: %i\n", publicDrains(500, damage, true), publicDrains(500, damage, false));
			printf("(Distance 299) Blocking: %i | Not Blocking: %i\n", publicDrains(299, damage, true), publicDrains(299, damage, false));
			printf("(Distance 99)  Blocking: %i | Not Blocking: %i\n", publicDrains(99, damage, true), publicDrains(99, damage, false));
			if (CustomDistance)
				printf("(Distance Custom) Blocking: %i | Not Blocking: %i\n", (publicDrains(CustomDistance, damage, true)), (publicDrains(CustomDistance, damage, false)));
		}
		else
		{
			printf("Pre-1.5 Open Beta Drains\n");
			printf("Damage: %i\n", damage);
			printf("(Distance 500) Blocking: %i | Not Blocking: %i\n", betaDrains(500, damage, true), betaDrains(500, damage, false));
			printf("(Distance 299) Blocking: %i | Not Blocking: %i\n", betaDrains(299, damage, true), betaDrains(299, damage, false));
			printf("(Distance 99)  Blocking: %i | Not Blocking: %i\n", betaDrains(99, damage, true), betaDrains(99, damage, false));
			if (CustomDistance)
				printf("(Distance Custom) Blocking: %i | Not Blocking: %i\n", (betaDrains(CustomDistance, damage, true)), (betaDrains(CustomDistance, damage, false)));
			//printf("FP drained (%i distance): %i\n", CustomDistance, betaDrains(CustomDistance, damage, blocking));
		}
	}
	else
	{
		printf("Damage: %i\n", damage);
		printf("============================\n");
		printf("Pre-1.5 Open Beta Drains\n");
		printf("(Distance 500) Blocking: %i | Not Blocking: %i\n", betaDrains(500, damage, true), betaDrains(500, damage, false));
		printf("(Distance 299) Blocking: %i | Not Blocking: %i\n", betaDrains(299, damage, true), betaDrains(299, damage, false));
		printf("(Distance 99)  Blocking: %i | Not Blocking: %i\n", betaDrains(99, damage, true), betaDrains(99, damage, false));
		if (CustomDistance)
			printf("(Distance Custom) Blocking: %i | Not Blocking: %i\n", (betaDrains(CustomDistance, damage, true)), (betaDrains(CustomDistance, damage, false)));
		printf("Live (1.5.1) Drains\n");
		printf("(Distance 500) Blocking: %i | Not Blocking: %i\n", publicDrains(500, damage, true), publicDrains(500, damage, false));
		printf("(Distance 299) Blocking: %i | Not Blocking: %i\n", publicDrains(299, damage, true), publicDrains(299, damage, false));
		printf("(Distance 99)  Blocking: %i | Not Blocking: %i\n", publicDrains(99, damage, true), publicDrains(99, damage, false));
		if (CustomDistance)
			printf("(Distance Custom) Blocking: %i | Not Blocking: %i\n", (publicDrains(CustomDistance, damage, true)), (publicDrains(CustomDistance, damage, false)));
		if (publicFPDrainsTweaked)
		{
			printf("============================\n");
			printf("Live (1.5.1) Drains (manual tweak)\n");
			printf("(Distance 500) Blocking: %i | Not Blocking: %i\n", publicDrainsTweaked(500, damage, true), publicDrainsTweaked(500, damage, false));
			printf("(Distance 299) Blocking: %i | Not Blocking: %i\n", publicDrainsTweaked(299, damage, true), publicDrainsTweaked(299, damage, false));
			printf("(Distance 99)  Blocking: %i | Not Blocking: %i\n", publicDrainsTweaked(99, damage, true), publicDrainsTweaked(99, damage, false));
			if (CustomDistance)
				printf("(Distance Custom) Blocking: %i | Not Blocking: %i\n", (publicDrainsTweaked(CustomDistance, damage, true)), (publicDrainsTweaked(CustomDistance, damage, false)));
		}
	}


	return 0;
}

// To tweak 1.5.1 multipliers and keeping the originals intact in the other function
int publicDrainsTweaked(int distance, int damage, bool blocking)
{
	float nForcePowerNeededToBlock = 0;
	bool bBelowFixedBoundary = (damage < 55); // Projectile damage.
	bool bOutsideIDR;
	bool bInsideMediumIDR;
	bool bInsideCloseIDR;
	bool bClientT3Enabled = false;
	//bool bReallyClose;

	if (bBelowFixedBoundary)
	{
		nForcePowerNeededToBlock = (int)(damage / 10.0f + 0.5f);

		switch (OurGun) {
		 MOD_DISRUPTOR_SNIPER:
		 MOD_DISRUPTOR:
		 MOD_DISRUPTOR_SPLASH:
			nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*3.0f + 0.5f);
			break;
		 MOD_BOWCASTER:
			nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*2.0f + 0.5f);
			break;
		 MOD_BOWCASTER_CHARGED:
			nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*1.5f + 0.5f);
			break;
		 MOD_VEHICLE:
			nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*1.175f + 0.5f);
			break;
		 MOD_REPEATER:
			nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*1.52f + 0.5f);
			break;
		 MOD_BRYAR_PISTOL:
		 MOD_BRYAR_PISTOL_ALT:
			nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*1.5f + 0.5f);
			break;
		 MOD_BRYAR_OLD:
		 MOD_BRYAR_OLD_ALT:
			nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*1.5f + 0.5f);
			break;
		 MOD_CONC:
			nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*1.11f + 0.5f);
			break;
		 MOD_DEMP2:
			nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*1.5f + 0.5f);
			break;
		 MOD_FLECHETTE:
			if (OurClass == MB_CLASS_ARCTROOPER)
				nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*1.8f + 0.5f);
			else
				nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*1.5f + 0.5f);
			break;
		 MOD_FLECHETTE_ALT_SPLASH:
			if (OurClass == MB_CLASS_ARCTROOPER)
				nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*1.6f + 0.5f);
			else
				nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*1.5f + 0.5f);
			break;
		 MOD_T21:
			nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*1.6f + 0.5f);
			break;
		 MOD_T21ALT:
			nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*1.25f + 0.5f);
			break;
		 MOD_SHOTGUN:
			nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*1.44f + 0.5f);
			break;
		 MOD_BLASTER:
		{
			nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*1.516f + 0.5f);
		} break;
		 MOD_BLASTER_ALT:
			nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*1.8f + 0.5f);
			break;
		 MOD_LASER:
			nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*2.2f + 0.5f);
			break;
		default:
			nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*2.0f + 0.5f);
			break;
		}
	}
	else
	{
		switch (OurGun) {
		 MOD_BOWCASTER_CHARGED:
			nForcePowerNeededToBlock = (int)(damage / 10.0f*1.5f + 0.5f);
			break;
		 MOD_T21:
			nForcePowerNeededToBlock = (int)(damage / 10.0f*1.5f + 0.5f);
			break;
		 MOD_DISRUPTOR_SNIPER:
		 MOD_DISRUPTOR:
		 MOD_DISRUPTOR_SPLASH:
			nForcePowerNeededToBlock = (int)(damage / 12.0f + 0.5f);
			break;
		default:
			nForcePowerNeededToBlock = (int)(damage / 4.0f + 0.5f);
			break;
		}

		if (OurGun == MOD_FLECHETTE_ALT_SPLASH && OurClass == MB_CLASS_MANDALORIAN)
			nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*0.8f + 0.5f); // Slightly nerf ee3 sniper
	}


	bOutsideIDR = (distance > IDR_RANGE_FAR);
	bInsideMediumIDR = ((distance > IDR_RANGE_CLOSE) && (distance <= IDR_RANGE_FAR)); // No need to distinguish for Q3
	bInsideCloseIDR = (distance <= IDR_RANGE_CLOSE);

	if (blocking)
	{
		if ((OurGun == MOD_T21) || (OurGun == MOD_BOWCASTER_CHARGED) || (OurGun == MOD_LASER) // || (OurGun == MOD_VEHICLE) // No longer Deka, and now applies at all ranges (including medium range, instead of just long range)
			|| (OurGun == MOD_BRYAR_PISTOL && damage > 30) || (OurGun == MOD_BRYAR_PISTOL_ALT && damage > 30)) // Let certain weapons(Dekas/Vehicles mainly, but other stuff too.) drain at long range through block. -- Azuvector
			nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*2.0f + 0.5f);
		//nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*1.8f + 0.5f);
		else
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

				nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*2.2f + 0.5f);
			}
		}
	}
	else
	{
		if ((OurGun == MOD_T21) || (OurGun == MOD_BOWCASTER_CHARGED) || (OurGun == MOD_LASER) // || (OurGun == MOD_VEHICLE) // No longer Deka, and now applies at all ranges (including medium range, instead of just long range)
			|| (OurGun == MOD_BRYAR_PISTOL && damage > 30) || (OurGun == MOD_BRYAR_PISTOL_ALT && damage > 30)) // Let certain weapons(Dekas/Vehicles mainly, but other stuff too.) drain at long range through running too now.
			nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*2.8 + 0.5f);
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
	}


	if (bInJump)
	{
#ifdef JEDI_REVAMP_V1
		nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*1.5f + 0.5f);
#else
		if (bOutsideIDR)
			nForcePowerNeededToBlock *= 1.7f;
		else
			nForcePowerNeededToBlock *= 1.7f;
#endif
	}

	if (DefLevel < FORCE_LEVEL_1)
		nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*3.0f + 0.5f);
	else if (DefLevel < FORCE_LEVEL_2)
		nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*1.6f + 0.5f);

#ifdef JEDI_REVAMP_V1
	// Decrease FP drains while doing special moves, so that they're less crippled
	if (IsInSpecialMove)
		nForcePowerNeededToBlock = (int)(0.5f*nForcePowerNeededToBlock + 0.5f);

#endif
#if !defined (JEDI_REVAMP_V15)
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
#endif

	return (int)nForcePowerNeededToBlock;
}

// IDR calculations from the pre-1.5 beta, weapon multipliers updated from 1.5+
int betaDrains(int distance, int damage, bool blocking)
{
	float nForcePowerNeededToBlock = 0;
	bool bBelowFixedBoundary = (damage < 55); // Projectile damage.
	bool bOutsideIDR;
	bool bInsideMediumIDR;
	bool bInsideCloseIDR;
	bool bClientT3Enabled = false;
	//bool bReallyClose;
	bool bInJump;

	// For IDR
	float a, b, fDrainBasedOnDist;

	if (bBelowFixedBoundary)
	{
		nForcePowerNeededToBlock = (int)(damage / 10.0f + 0.5f);

		switch (OurGun)
		{
		 MOD_DISRUPTOR_SNIPER:
		 MOD_DISRUPTOR:
		 MOD_DISRUPTOR_SPLASH:
			nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*3.0f + 0.5f);
			break;
		 MOD_BOWCASTER:
		 MOD_BOWCASTER_CHARGED:
			nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*1.5f + 0.5f);
			break;
		 MOD_VEHICLE:
			nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*1.05f + 0.5f);
			break;
		 MOD_REPEATER:
			nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*1.52f + 0.5f);
			break;
		 MOD_BRYAR_PISTOL:
		 MOD_BRYAR_PISTOL_ALT:
			nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*1.5f + 0.5f);
			break;
		 MOD_BRYAR_OLD:
		 MOD_BRYAR_OLD_ALT:
			nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*1.5f + 0.5f);
			break;
		 MOD_CONC:
			nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*1.11f + 0.5f);
			break;
		 MOD_DEMP2:
			nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*1.5f + 0.5f);
			break;
		 MOD_FLECHETTE:
			if (OurClass == MB_CLASS_ARCTROOPER)
				nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*2.4f + 0.5f);
			else
				nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*1.5f + 0.5f);
			break;
		 MOD_FLECHETTE_ALT_SPLASH:
			if (OurClass == MB_CLASS_ARCTROOPER)
				nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*1.5f + 0.5f);
			else
				nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*1.5f + 0.5f);
			break;
		 MOD_T21:
			nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*1.5f + 0.5f);
			break;
		 MOD_T21ALT:
			nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*1.25f + 0.5f);
			break;
		 MOD_SHOTGUN:
			nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*1.44f + 0.5f);
			break;
		 MOD_BLASTER:
		{
			nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*1.516f + 0.5f);
		} break;
		 MOD_BLASTER_ALT:
			nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*2.0f + 0.5f);
			break;
		 MOD_LASER:
			nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*1.9f + 0.5f);
			break;
		default:
			nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*2.0f + 0.5f);
			break;
		}
	}
	else
	{
		switch (OurGun)
		{
		 MOD_BOWCASTER_CHARGED:
			nForcePowerNeededToBlock = (int)(damage / 10.0f*1.5f + 0.5f);
			break;
		 MOD_T21:
			nForcePowerNeededToBlock = (int)(damage / 10.0f*1.5f + 0.5f);
			break;
		 MOD_DISRUPTOR_SNIPER:
		 MOD_DISRUPTOR:
		 MOD_DISRUPTOR_SPLASH:
			nForcePowerNeededToBlock = (int)(damage / 12.0f + 0.5f);
			break;
		default:
			nForcePowerNeededToBlock = (int)(damage / 4.0f + 0.5f);
			break;
		}
	}

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

#ifdef JEDI_REVAMP_V1
	if (bClientT3Enabled && (DefLevel < FORCE_LEVEL_3) && ((bAttacking && !(blocking)) || PM_InGetUpAnimation))// Q3 gets a 2.0 FP drain multiplier for attack-defending if player doesn't have saber defense level 3. Otherwise no additional drain.
		nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*2.0f + 0.5f);
#endif

	if (bInJump)
	{
		nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*1.7f + 0.5f);
	}

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

	return (int)nForcePowerNeededToBlock;
}

// 1.5.1 FP drain math
int publicDrains(int distance, int damage, bool blocking)
{
	float nForcePowerNeededToBlock = 0;
	bool bBelowFixedBoundary = (damage < 55); // Projectile damage.
	bool bOutsideIDR;
	bool bInsideMediumIDR;
	bool bInsideCloseIDR;
	bool bClientT3Enabled = false;
	//bool bReallyClose;

	if (bBelowFixedBoundary)
	{
		nForcePowerNeededToBlock = (int)(damage / 10.0f + 0.5f);

		switch (OurGun) {
		 MOD_DISRUPTOR_SNIPER:
		 MOD_DISRUPTOR:
		 MOD_DISRUPTOR_SPLASH:
			nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*3.0f + 0.5f);
			break;
		 MOD_BOWCASTER:
		 MOD_BOWCASTER_CHARGED:
			nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*1.5f + 0.5f);
			break;
		 MOD_VEHICLE:
			nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*1.05f + 0.5f);
			break;
		 MOD_REPEATER:
			nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*1.52f + 0.5f);
			break;
		 MOD_BRYAR_PISTOL:
		 MOD_BRYAR_PISTOL_ALT:
			nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*1.5f + 0.5f);
			break;
		 MOD_BRYAR_OLD:
		 MOD_BRYAR_OLD_ALT:
			nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*1.5f + 0.5f);
			break;
		 MOD_CONC:
			nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*1.11f + 0.5f);
			break;
		 MOD_DEMP2:
			nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*1.5f + 0.5f);
			break;
		 MOD_FLECHETTE:
			if (OurClass == MB_CLASS_ARCTROOPER)
				nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*1.8f + 0.5f);
			else
				nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*1.5f + 0.5f);
			break;
		 MOD_FLECHETTE_ALT_SPLASH:
			if (OurClass == MB_CLASS_ARCTROOPER)
				nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*1.6f + 0.5f);
			else
				nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*1.5f + 0.5f);
			break;
		 MOD_T21:
			nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*1.6f + 0.5f);
			break;
		 MOD_T21ALT:
			nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*1.25f + 0.5f);
			break;
		 MOD_SHOTGUN:
			nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*1.44f + 0.5f);
			break;
		 MOD_BLASTER:
		{
			nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*1.516f + 0.5f);
		} break;
		 MOD_BLASTER_ALT:
			nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*1.7f + 0.5f);
			break;
		 MOD_LASER:
			nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*2.2f + 0.5f);
			break;
		default:
			nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*2.0f + 0.5f);
			break;
		}
	}
	else
	{
		switch (OurGun) {
		 MOD_BOWCASTER_CHARGED:
			nForcePowerNeededToBlock = (int)(damage / 10.0f*1.5f + 0.5f);
			break;
		 MOD_T21:
			nForcePowerNeededToBlock = (int)(damage / 10.0f*1.5f + 0.5f);
			break;
		 MOD_DISRUPTOR_SNIPER:
		 MOD_DISRUPTOR:
		 MOD_DISRUPTOR_SPLASH:
			nForcePowerNeededToBlock = (int)(damage / 12.0f + 0.5f);
			break;
		default:
			nForcePowerNeededToBlock = (int)(damage / 4.0f + 0.5f);
			break;
		}

		if (OurGun == MOD_FLECHETTE_ALT_SPLASH && OurClass == MB_CLASS_MANDALORIAN)
			nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*0.8f + 0.5f); // Slightly nerf ee3 sniper
	}

#ifdef JEDI_REVAMP_V1
	bOutsideIDR = (distance > IDR_RANGE_FAR);
	bInsideMediumIDR = ((distance > IDR_RANGE_CLOSE) && (distance <= IDR_RANGE_FAR)); // No need to distinguish for Q3
	bInsideCloseIDR = (distance <= IDR_RANGE_CLOSE);
#else
	bOutsideIDR = ((distance > 410 && !bClientT3Enabled) || (distance > 410 && bClientT3Enabled));
	bInsideCloseIDR = (distance <= 205);
	bInsideMediumIDR = (distance > 205 && ((distance <= 410 && !bClientT3Enabled) || (DistanceToAttacker <= 410/*g_T3MediumIDRRange.integer*/ && bClientT3Enabled)));
	//bReallyClose = (DistanceToAttacker < 210); // Used on top of IDR calculations.
#endif

#ifdef JEDI_REVAMP_V1
	if (blocking)
	{
		if ((OurGun == MOD_T21) || (OurGun == MOD_BOWCASTER_CHARGED) || (OurGun == MOD_LASER) // || (OurGun == MOD_VEHICLE) // No longer Deka, and now applies at all ranges (including medium range, instead of just long range)
			|| (OurGun == MOD_BRYAR_PISTOL && damage > 30) || (OurGun == MOD_BRYAR_PISTOL_ALT && damage > 30)) // Let certain weapons(Dekas/Vehicles mainly, but other stuff too.) drain at long range through block. -- Azuvector
			nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*2.0f + 0.5f);
		//nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*1.8f + 0.5f);
		else
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
	}
	else
	{
		if ((OurGun == MOD_T21) || (OurGun == MOD_BOWCASTER_CHARGED) || (OurGun == MOD_LASER) // || (OurGun == MOD_VEHICLE) // No longer Deka, and now applies at all ranges (including medium range, instead of just long range)
			|| (OurGun == MOD_BRYAR_PISTOL && damage > 30) || (OurGun == MOD_BRYAR_PISTOL_ALT && damage > 30)) // Let certain weapons(Dekas/Vehicles mainly, but other stuff too.) drain at long range through running too now.
			nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*2.8 + 0.5f);
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
	}
#else
	if (bBelowFixedBoundary || OurGun == MOD_T21 || OurGun == MOD_BOWCASTER_CHARGED || OurGun == MOD_LASER ||
		((OurGun == MOD_BRYAR_PISTOL || OurGun == MOD_BRYAR_PISTOL_ALT) && atk && atk->client &&  atk->client->cClassAttributes[MB_ATT_PISTOL] >= FORCE_LEVEL_3)) //pistol 3
	{
		if (blocking)
		{
			if (bOutsideIDR)
			{
				if (OurGun == MOD_VEHICLE || OurGun == MOD_T21 || OurGun == MOD_BOWCASTER_CHARGED || OurGun == MOD_LASER || OurGun == MOD_BRYAR_PISTOL || OurGun == MOD_BRYAR_PISTOL_ALT)
					nForcePowerNeededToBlock *= 1.8f; // Let certain weapons(Dekas/Vehicles mainly, but other stuff too.) drain at long range through block. -- Azuvector
				else
					nForcePowerNeededToBlock = 4.0f;//2.5f;//2.5; // Flat value at long range.
			}
			else if (bInsideCloseIDR)
				nForcePowerNeededToBlock *= 2.3f;
			else if (bInsideMediumIDR)
				nForcePowerNeededToBlock *= 2.3f;
		}
		else
		{
			if (bOutsideIDR)
				nForcePowerNeededToBlock *= 2.0f;
			else if (bInsideCloseIDR)
				nForcePowerNeededToBlock *= 3.0f;
			else if (bInsideMediumIDR)
				nForcePowerNeededToBlock *= 3.0f;
		}
	}
	else
	{
		if (blocking)
		{
			if (bInsideCloseIDR)
				nForcePowerNeededToBlock *= 2.3f;
		}
		else
		{
			if (bInsideCloseIDR)
				nForcePowerNeededToBlock *= 3.0f;
		}
	}
#endif

	if (bInJump)
	{
#ifdef JEDI_REVAMP_V1
		nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*1.5f + 0.5f);
#else
		if (bOutsideIDR)
			nForcePowerNeededToBlock *= 1.7f;
		else
			nForcePowerNeededToBlock *= 1.7f;
#endif
	}

	if (DefLevel < FORCE_LEVEL_1)
		nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*3.0f + 0.5f);
	else if (DefLevel < FORCE_LEVEL_2)
		nForcePowerNeededToBlock = (int)(nForcePowerNeededToBlock*1.6f + 0.5f);

#ifdef JEDI_REVAMP_V1
	// Decrease FP drains while doing special moves, so that they're less crippled
	if (IsInSpecialMove)
		nForcePowerNeededToBlock = (int)(0.5f*nForcePowerNeededToBlock + 0.5f);

#endif
#if !defined (JEDI_REVAMP_V15)
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
#endif

	return (int)nForcePowerNeededToBlock;
}