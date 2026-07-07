#include "Schema/UserBreakdown.h"


void makePDetails(User *u){
    u->ProdInfo.TotalMil=0;
    u->ProdInfo.FreeMil=0;

    u->ProdInfo.TotalCiv=0;
    u->ProdInfo.FreeCiv=0;
}

void makeResource(User *u){

    strcpy(u->Resources.lastUpdated, "");

    u->Resources.Gold.Total=0;
    u->Resources.Gold.Rate=0;

    u->Resources.Stone.Total=0;
    u->Resources.Stone.Rate=0;

    u->Resources.Wood.Total=0;
    u->Resources.Wood.Rate=0;

    u->Resources.Political.Total=0;
    u->Resources.Political.Rate=0;

    u->Resources.Stability.Total=0;
    u->Resources.Stability.Rate=0;

    u->Resources.WarSupport.Total=0;
    u->Resources.WarSupport.Rate=0;

    u->Resources.ManPower.TotalManPower=0;
    u->Resources.ManPower.TotalPopulation=0;
    u->Resources.ManPower.Rate=0;
    u->Resources.ManPower.RecruitableFactor=0;
    u->Resources.ManPower.MaxPopulation=0;
    u->Resources.ManPower.FreePopulation=0;

}


void applyTech(Tech* tech, bool unlocked, char* desc){
    tech->unlocked = unlocked;
    strncpy(tech->Description, desc, sizeof(tech->Description) - 1);
    tech->Description[sizeof(tech->Description) - 1] = '\0';
}

void makeTechnologies(User *u){
    char defaultDesc[128]="Default description: This is technology";

    applyTech(&u->Technologies.Bows, true, defaultDesc);
    applyTech(&u->Technologies.Swords, true, defaultDesc);
    applyTech(&u->Technologies.Shields, true, defaultDesc);
    applyTech(&u->Technologies.Spears, true, defaultDesc);
    applyTech(&u->Technologies.LeatherArmour, true, defaultDesc);
    applyTech(&u->Technologies.BatteringRam, true, defaultDesc);
    applyTech(&u->Technologies.WagonFort, true, defaultDesc);
    applyTech(&u->Technologies.WoodWall, true, defaultDesc);
    applyTech(&u->Technologies.StoneWall, true, defaultDesc);
    applyTech(&u->Technologies.WoodGate, true, defaultDesc);
    applyTech(&u->Technologies.StoneGate, true, defaultDesc);
    applyTech(&u->Technologies.WoodenTower, true, defaultDesc);
    applyTech(&u->Technologies.StoneTower, true, defaultDesc);
    applyTech(&u->Technologies.WoodenKeep, true, defaultDesc);
    applyTech(&u->Technologies.StoneKeep, true, defaultDesc);
    applyTech(&u->Technologies.WoodHouse, true, defaultDesc);
    applyTech(&u->Technologies.StoneHouse, true, defaultDesc);
    applyTech(&u->Technologies.Pavement, true, defaultDesc);

    applyTech(&u->Technologies.CivilianFactory, true, defaultDesc);
    applyTech(&u->Technologies.MilitaryFactory, true, defaultDesc);
    applyTech(&u->Technologies.Farm, true, defaultDesc);
    applyTech(&u->Technologies.Quarry, true, defaultDesc);
    applyTech(&u->Technologies.LumberMill, true, defaultDesc);
    applyTech(&u->Technologies.Barracks, true, defaultDesc);
    applyTech(&u->Technologies.Market, true, defaultDesc);
    applyTech(&u->Technologies.TownHall, true, defaultDesc);
    applyTech(&u->Technologies.Warehouse, true, defaultDesc);

    applyTech(&u->Technologies.ChainArmour, true, defaultDesc);
    applyTech(&u->Technologies.PlateArmour, true, defaultDesc);
    applyTech(&u->Technologies.Crossbows, true, defaultDesc);
    applyTech(&u->Technologies.Trebuchet, true, defaultDesc);
    applyTech(&u->Technologies.Catapult, true, defaultDesc);
    applyTech(&u->Technologies.Ballista, true, defaultDesc);

    applyTech(&u->Technologies.StandardisedParts, true, defaultDesc);
    applyTech(&u->Technologies.RobustSupplyChains, true, defaultDesc);
    applyTech(&u->Technologies.WorkerShifts, true, defaultDesc);
    applyTech(&u->Technologies.FortifiedSettlements, true, defaultDesc);
    applyTech(&u->Technologies.CropRotation, true, defaultDesc);
    
}


void user_init(User *u, const char *username, const char *passwordHash){

    memset(u, 0, sizeof(*u));
    
    strncpy(u->username, username, sizeof(u->username) - 1);
    strncpy(u->passwordHash, passwordHash, sizeof(u->passwordHash) - 1);

    u->originTile[0] = 0;
    u->originTile[1] = 0;

    strcpy(u->lastClaimDate, "");

    makePDetails(u);
    makeResource(u);
    makeTechnologies(u);
}