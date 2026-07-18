#ifndef UUtils_H
#define UUtils_H   // these form a guard

#include "Schema/UserBreakdown.h"

void makePDetails(User *u);

void makeResource(User *u);

void applyTech(Tech* tech, bool unlocked, char* desc);

void makeTechnologies(User *u);

void user_init(User *u, const char *username, const char *passwordHash);

#endif