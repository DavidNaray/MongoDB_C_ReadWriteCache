#ifndef ReadUser_H
#define ReadUser_H   // these form a guard

#include "UserBreakdown.h"
#include <bson/bson.h>


void bson_to_user(const bson_t* doc, User* u);


void bson_to_PDetails(const bson_t* doc, PDetails* p);


void bson_to_resource(const bson_t* doc, Resource* r);
void bson_to_manpower(const bson_t* doc, ResourceManPower* m);
void bson_to_resources(const bson_t* doc, Resources* r);


void bson_to_tech(const bson_t* doc, Tech* t);
void bson_to_technologies(const bson_t* doc, Technologies* t);


void bson_to_unit_training(const bson_t* doc, UnitTraining* ut);
void bson_to_regimen_training(const bson_t* doc, RegimenTraining* rt);
void bson_to_regimen_training_list(const bson_t* doc, RegimenTrainingList* list);


void bson_to_unit(const bson_t* doc, Unit* u);
void bson_to_unit_block(const bson_t* doc, UnitBlock** ub_out);
void bson_to_regimen(const bson_t* doc, Regimen* r);
void bson_to_regimen_queue(const bson_t* doc, RegimenQueue* q);

#endif