#include "vex.h"
using namespace vex;
brain Brain;
controller controller1 = controller(primary);

// ------------------------------------------------------------------------
//              Drivetrain definition
// ------------------------------------------------------------------------
//If you only have 4  motors (or mecanum drive), assign leftMotor3, rightMotor3 to unused ports.
motor leftMotor1 = motor(PORT11, ratio18_1, true);
motor leftMotor2 = motor(PORT12, ratio6_1, true);
motor leftMotor3 = motor(PORT13, ratio6_1, true);

motor rightMotor1 = motor(PORT1, ratio18_1, false);
motor rightMotor2 = motor(PORT2, ratio6_1, false);
motor rightMotor3 = motor(PORT3, ratio6_1, false);

motor matchLoad = motor(PORT14, ratio18_1, true);
motor hornMotor = motor(PORT5, ratio18_1, false);

motor rollerTop = motor(PORT18, ratio6_1, false);
motor rollerBottom = motor(PORT17, ratio6_1, false);
// inertial sensor for auton turning and heading
// If you do not have an inertial sensor, assign it to an unused port. Ignore the warning at the start of the program.
inertial inertial1 = inertial(PORT16);

optical teamOptical = optical(-1); // assign to an unused port if not used
optical colorSortOptical = optical(PORT19); // assign to an unused port if not used


// 0: double arcade drive, 1: single aracde, 2: tank drive
// -1: disable drive
int DRIVE_MODE = 2;

bool macroMode = false;

// ------------------------------------------------------------------------
//        Other subsystems: motors, sensors and helper functions definition
// ------------------------------------------------------------------------

//motor rollerIntake = motor(PORT10, ratio6_1, false);
//motor rollerScore = motor(PORT14, ratio6_1, true);

// total number of motors, including drivetrain
const int NUMBER_OF_MOTORS = 9;

bool hornUp = false;
void toggleHornPosition() {
  hornUp = !hornUp;
  if (hornUp) {
    hornMotor.spin(forward, 10, volt);
    wait(100, msec);
    waitUntil(hornMotor.torque()>0.3);
    hornMotor.stop(brake);
    chassis.stop(coast);
  } else {
    hornMotor.setVelocity(75, percent);
    hornMotor.setTimeout(300, msec);
    hornMotor.spinFor(reverse, 145, degrees);
    hornMotor.stop(brake);
    chassis.stop(hold);
  }
}

void intake()
{
  rollerBottom.spin(forward, 12, volt);
  rollerTop.stop(hold);
}

void reverseIntake()
{
  rollerBottom.spin(reverse, 12, volt);
  rollerTop.stop(coast);
  chassis.stop(hold);
}

void stopRollers()
{
  // Stops the roller motors.
  rollerBottom.stop(brake);
  rollerTop.stop(brake);
  chassis.stop(coast);
  if (colorSortOptical.installed()) {
    colorSortOptical.setLight(ledState::off);
  }
}


void scoreLong()
{
  rollerBottom.spin(forward, 12, volt);
  rollerTop.spin(forward, 12, volt);
  chassis.stop(hold);
}

void scoreMiddle()
{
  rollerBottom.spin(forward, 12, volt);
  rollerTop.spin(forward, 15, pct);
  chassis.stop(hold);
}

void ejectBalls(){
  rollerBottom.spin(forward, 12, volt);
  rollerTop.spin(reverse, 70, pct);
}

bool sortBalls(){
  if (colorSortOptical.installed()) {
    colorSortOptical.setLight(ledState::on);
    colorSortOptical.setLightPower(100, percent);
  }
  if (colorSortMode == 0 || !colorSortOptical.installed()|| !colorSortOptical.isNearObject()) {
    return true; // color sorting disabled
  }

  // read color from optical sensor
  color r = colorSortOptical.color();
  // assume red is 0-60 and blue is 180-240
  if ( (r == color::red && colorSortMode == 2) ||  (r == color::blue && colorSortMode == 1))  {
    return false; // wrong color
  }
  else {
    return true; // everything else
  }
}
void scoreBalls(int durationMsec)
{
  int count = durationMsec / 100;;
  for (int i = 0; i < count; i++) {
    bool matching_balls = sortBalls();
    if (!matching_balls) 
    {
      ejectBalls();
    }
    else 
    {
      scoreLong();
    }
    wait(100, msec);
  }
  stopRollers();
}

bool match_load_down = false;
void toggleMatchLoad(){
  match_load_down= !match_load_down;
  if (match_load_down)
  {
    matchLoad.spinFor(reverse, 300, degrees);
    matchLoad.stop(coast);
  }
  else
  {
    matchLoad.spinFor(forward, 300, degrees);
    matchLoad.stop(coast);
    wait(20, msec);
    matchLoad.stop(hold);
 }
}

distance frontDistanceSensor = distance(PORT4);

float getDistance()
{
  if (frontDistanceSensor.installed()) {
    return frontDistanceSensor.objectDistance(inches);
  }
  else {
    return -1.0;
  }
}

void getMatchLoads(int durationMsec)
{
  if (!match_load_down) 
  {
    toggleMatchLoad();
    wait(1000, msec);
  }
  intake();

  float d = getDistance();
  float h = chassis.getHeading();


  chassis.driveDistance(d -16, 10, h, 6);
  chassis.driveWithVoltage(10, 10);
  wait(100, msec);
  chassis.stop(coast);
  wait(100, msec);
  chassis.driveWithVoltage(10, 10);
  wait(60, msec);
  chassis.stop(hold);
  wait(durationMsec, msec);
}


// ------------------------------------------------------------------------
//              Button controls
// ------------------------------------------------------------------------


// This function is called when the L1 button is pressed.
void buttonL1Action() {

  intake();
  // Wait until the button is released to stop the rollers.
  while(controller1.ButtonL1.pressing()) {
    wait (20, msec);
  }
  stopRollers();
}


void buttonL2Action() {
  
  while(controller1.ButtonL2.pressing()) 
  {
    bool matching_balls = sortBalls();
    if (!matching_balls) 
    {
      ejectBalls();
    }
    else 
    {
      scoreLong();
    }
    wait (100, msec);
  }
  stopRollers();
}

void buttonBAction()
{
  // brake the drivetrain until the button is released.
  chassis.stop(hold);
  controller1.rumble(".");
  waitUntil(!controller1.ButtonB.pressing());
  chassis.checkStatus();
  chassis.stop(coast);
}

void buttonR1Action()
{
  toggleMatchLoad();
}

void buttonR2Action()
{
  toggleHornPosition();
}


void buttonLeftAction()
{
  // disable button if in auton test mode
  if (autonTestMode) return; 

  changeColorSortMode();
  wait(400, msec);
  if (controller1.ButtonLeft.pressing()) {
    ejectBalls();
    waitUntil(!controller1.ButtonLeft.pressing());
    stopRollers();
  }
}

void deScore()
{
  float currentHeading = chassis.inertialSensor.heading();
  if(hornUp) {
    chassis.driveDistance(10, 10, currentHeading, 6);
    chassis.turnToHeading(currentHeading-90);
    chassis.driveDistance(13);
    toggleHornPosition();
    chassis.turnToHeading(currentHeading);
    chassis.driveDistance(-24);
  }
  else {
    chassis.driveDistance(-24, 10, currentHeading, 6);
  }
}

void buttonXAction()
{
  changeDriveMode();
}

void buttonDownAction()
{
  reverseIntake();
  waitUntil(!controller1.ButtonDown.pressing());
  stopRollers();
}

void buttonUpAction()
{
  scoreMiddle();
  waitUntil(!controller1.ButtonUp.pressing());
  stopRollers();
}

void testButton()
{
  // disable button if in auton test mode, config mode, or in compeitition mode
  if (autonTestMode  || endGameTimerEnabled) return; 
  
  double t1 = Brain.Timer.time(sec);

  chassis.setHeading(180);
  chassis.driveDistance(-35, 10, 198, 6);

  double t2 = Brain.Timer.time(sec);
  char timeMsg[30];
  sprintf(timeMsg, "run time: %.1f", t2-t1);
  printControllerScreen(timeMsg);
  chassis.stop(coast);
}

void setupButtonMapping()
{
  controller1.ButtonL1.pressed(buttonL1Action);
  controller1.ButtonL2.pressed(buttonL2Action);
  controller1.ButtonR2.pressed(buttonR2Action);
  controller1.ButtonR1.pressed(buttonR1Action);
  controller1.ButtonA.pressed(testButton);
  controller1.ButtonLeft.pressed(buttonLeftAction);
  controller1.ButtonX.pressed(buttonXAction);
  controller1.ButtonB.pressed(buttonBAction);
  controller1.ButtonUp.pressed(buttonUpAction);
  controller1.ButtonDown.pressed(buttonDownAction);
}

void additionalSetup()
{
  // setup matchload motor here if any
  matchLoad.stop(hold);
  matchLoad.setVelocity(100, percent);
  matchLoad.setTimeout(500, msec);
}



// ------------------------------------------------------------------------
//               Only change code below this line when necessary
// ------------------------------------------------------------------------


Drive chassis(
  //Left Motors:
  motor_group(leftMotor2, leftMotor1, leftMotor3),
  //Right Motors:
  motor_group(rightMotor2, rightMotor1, rightMotor3),
  //Inertial Sensor:
  inertial1,
  //wheel diameter:
  2.75,
  //Gear ratio of motor to wheel: if your motor has an 36-tooth gear and your wheel has a 48-tooth gear, this value will be 0.75.
  0.75
);

// Resets the chassis constants.
void setChassisDefaults() {
  // Sets the heading of the chassis to the current heading of the inertial sensor.
  chassis.setHeading(chassis.inertialSensor.heading());

  chassis.setMaxVoltage(10, 10, 6);
  // Sets the drive PID constants for the chassis.
  // These constants are used to control the acceleration and deceleration of the chassis.
  chassis.setDrivePID(1.5, 0, 10, 0);
  // Sets the turn PID constants for the chassis.
  // These constants are used to control the turning of the chassis.
  chassis.setTurnPID(0.2, .015, 1.5, 7.5);
  // Sets the heading PID constants for the chassis.
  // These constants are used to control the heading adjustment of the chassis.
  chassis.setHeadingPID(0.4, 1);
  // Sets the exit conditions for the drive functions.
  // These conditions are used to determine when the drive function should exit.
  chassis.setDriveExitConditions(1, 300, 2000);
  // Sets the exit conditions for the turn functions.
  // These conditions are used to determine when the turn function should exit.
  chassis.setTurnExitConditions(1.5, 300, 1500);

  // Sets the arcade drive constants for the chassis.
  // These constants are used to control the arcade drive of the chassis.
  chassis.setArcadeConstants(0.5, 0.5, 0.85);
}

void changeDriveMode()
{
  controller1.rumble("-");
  DRIVE_MODE = (DRIVE_MODE +1)%3;
    switch (DRIVE_MODE) {
    case 0:
      printControllerScreen("Double Arcade");
      break;
    case 1:
      printControllerScreen("Single Arcade");
      break;
    case 2:
      printControllerScreen("Tank Drive");
      break;
    case 3:
      printControllerScreen("Mecanum Drive");
      break;
    }
}

int colorSortMode = 0; // 0: off, 1: red, 2: blue
void changeColorSortMode()
{
  controller1.rumble("-");
  colorSortMode = (colorSortMode + 1) % 3;
  if (colorSortMode == 0) printControllerScreen("No color sort");
  else if (colorSortMode == 1) printControllerScreen("Sort red");
  else if (colorSortMode == 2) printControllerScreen("Sort blue");
}


// This is the user control function.
// It is called when the driver control period starts.
void usercontrol(void)
{
  // Exits the autonomous menu.
  exitAuton();
  additionalSetup();

  // This loop runs forever, controlling the robot during the driver control period.
  while (1) {
    switch (DRIVE_MODE) {
    case 0: // double arcade
      chassis.controlArcade(controller1.Axis2.position(), controller1.Axis4.position());
      break;
    case 1: // single arcade
      chassis.controlArcade(controller1.Axis3.position(), controller1.Axis4.position());
      break;
    case 2: // tank drive
      chassis.controlTank(controller1.Axis3.position(), controller1.Axis2.position());    
      break;
    }

    // This wait prevents the loop from using too much CPU time.
    wait(20, msec);
   } 
}

