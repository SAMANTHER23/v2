#pragma once
#include "rgb-template/drive.h"

void pre_auton();
void autonomous();
void exitAuton();

bool continueAutonStep();
void registerAutonTestButtons();

extern int currentAutonSelection;
extern bool autonTestMode;
extern bool endGameTimerEnabled;