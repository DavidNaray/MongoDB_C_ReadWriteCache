#ifndef WriteUser_H
#define WriteUser_H   // these form a guard

#include "UserBreakdown.h"
#include <bson/bson.h>

bson_t* user_to_bson(const User* u);


void PDetails_to_bson(const PDetails* p, bson_t* parent);


void resource_to_bson(bson_t* parent, const char* key, const Resource* r);
void manpower_to_bson(bson_t* parent, const char* key, const ResourceManPower* m);
void resources_to_bson(const Resources* r, bson_t* parent); 


void tech_to_bson(bson_t* parent, const char* key, const Tech* t);
void technologies_to_bson(const Technologies* t, bson_t* parent);


void unit_training_to_bson(bson_t* parent, const UnitTraining* ut);
void regimen_training_to_bson(const RegimenTraining* rt, bson_t* parent);
void regimen_training_list_to_bson(const RegimenTrainingList* list, bson_t* parent);


void unit_to_bson(const Unit* u, bson_t* parent);
void unit_block_to_bson(const UnitBlock* ub, bson_t* parent);
void regimen_to_bson(const Regimen* r, bson_t* parent);
void regimen_queue_to_bson(const RegimenQueue* q, bson_t* parent);

#endif