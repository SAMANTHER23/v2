using namespace vex;

void intake();
void stopRollers();
void reverseIntake();
void scoreMiddle();
void scoreLong();
void scoreBalls();
void ejectBalls();
bool sortBalls();

void toggleMatchLoad();
void toggleHorn();

// ------------------------------------------------------------------------
//               Only change code below this line when necessary
// ------------------------------------------------------------------------

// A global instance of the brain used for printing to the V5 Brain screen.
extern brain Brain;
extern controller controller1;
// Forward declaration of the Drive class.
class Drive;
// A global instance of the Drive class.
extern Drive chassis;
extern int DRIVE_MODE;
extern int colorSortMode;
extern bool macroMode;
extern const int NUMBER_OF_MOTORS;

void changeDriveMode();
void changeColorSortMode();
void setChassisDefaults();
void additionalSetup();
void usercontrol();

void setupButtonMapping();

extern optical teamOptical;
extern optical colorSortOptical;

void scoreBalls(int durationMsec);
void getMatchLoads(int durationMsec);
float getDistance();