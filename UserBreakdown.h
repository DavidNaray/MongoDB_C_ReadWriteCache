#ifndef user_H
#define user_H   // these form a guard

#include <stdbool.h>//makes the boolean type

#define MAX_REGIMENS 64

typedef struct {
    int TotalMil;
    int FreeMil;

    int TotalCiv;
    int FreeMil;
} PDetails;

//-------------------------------------------------------------------

typedef struct {
    int Total;
    float Rate;
} Resource;

typedef struct {
    int TotalManPower;
    int TotalPopulation;
    float Rate;
    float RecruitableFactor;
    int MaxPopulation;
    int FreePopulation;
} ResourceManPower;

typedef struct {
    Resource Gold;
    Resource Stone;
    Resource Wood;
    Resource Political;
    Resource Stability;
    Resource WarSupport;

    ResourceManPower ManPower;

    char lastUpdated[64];
} Resources;

//-------------------------------------------------------------------

typedef struct {
    bool unlocked;
    char Description[128];
} Tech;

typedef struct {

    Tech Bows;
    Tech Swords;
    Tech Shields;
    Tech Spears;
    Tech LeatherArmour;
    Tech BatteringRam;
    Tech WagonFort;
    Tech WoodWall;
    Tech StoneWall;
    Tech WoodGate;
    Tech StoneGate;
    Tech WoodenTower;
    Tech StoneTower;
    Tech WoodenKeep;
    Tech StoneKeep;
    Tech WoodHouse;
    Tech StoneHouse;
    Tech Pavement;

    Tech CivilianFactory;
    Tech MilitaryFactory;
    Tech Farm;
    Tech Quarry;
    Tech LumberMill;
    Tech Barracks;
    Tech Market;
    Tech TownHall;
    Tech Warehouse;

    Tech ChainArmour;
    Tech PlateArmour;
    Tech Crossbows;
    Tech Trebuchet;
    Tech Catapult;
    Tech Ballista;

    Tech StandardisedParts;
    Tech RobustSupplyChains;
    Tech WorkerShifts;
    Tech FortifiedSettlements;
    Tech CropRotation;
} Technologies;


// ------------------------------------------------------------------

enum UnitType{
    UNIT_ARCHER, //0
    UNIT_SPEARMAN, //1
    UNIT_SWORDSMAN, //2
    UNIT_MAX // is 3, just how enum works ig, basically giving a name to an index
};

typedef struct {
    int count;//how many units of this type in regimen for training
    int progress;
    int finish;
} UnitTraining;

typedef struct {
    UnitTraining units[UNIT_MAX];
    int deployTile[2];
    int deployPixel[2];
    bool active;
} RegimenTraining;

typedef struct {
    RegimenTraining regimens[MAX_REGIMENS];
} RegimenTrainingList;
//users can only have MAX_REGIMENS training at once
//fixed list means fast lookup, less lag

typedef struct {

    int health;
    int tile[2];//x,y tile
    int position[2];//x,y pixel of tile
} Unit;

typedef struct {
    int count;
    //fixed array of Unit
    Unit individuals[];
} UnitBlock;

typedef struct {
    int id;
    UnitBlock* units[UNIT_MAX];//index corresponds to unit type
} Regimen;


typedef struct {
    //dynamic array rather than linked list, fast lookup, slow edit but there wont be edits, or rarely
    Regimen** regimens;
    int count;
    int capacity;
} RegimenQueue;
// ----------------------------------------------

typedef struct {
    char username[256]; 
    char passwordHash[128]; 
    int  originTile[2];    // [x,y]

    char lastClaimDate[64];


    PDetails ProdInfo;

    Resources Resources;

    Technologies Technologies;

    RegimenTrainingList regimenTrainingList;

    RegimenQueue regimens;
} User;


#endif