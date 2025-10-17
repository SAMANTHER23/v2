# steps to create a clean open repository
- new v5 project (named rgb_sample)
- add .gitignore file
- commit and pubblish in github

# code to add 
- 
In exitAuton, add immediate actions right at the start of the driver control.

# private feature
- chaining    
```cpp
// Turns the robot to a specific heading without stopping.
    // earlyExitFactor should between 1-5
  void turnToHeading(float heading, float turnMaxVoltage, float earlyExitFactor);
      // Drives the robot a specific distance without stopping.
  void driveDistance(float distance, float driveMaxVoltage, float heading, float headingMaxVoltage,  float earlyExitFactor);
```

# big bot
- cable management
- opponent ball basket
- own ball baskset (plastic, cart board)
- color sort test

# ruiguan
- matchload refine
- descore horn
- two-side piston
- auton

# public repo
- change timeout numbers for both cpp and python
- add readme for PID tuning

# to view
https://www.youtube.com/watch?v=2RbRCHhwIuA&t=4824s
floating score mech

https://www.youtube.com/watch?v=bPMflQ9f16Y&t=9s
motor use instead of piston

https://www.youtube.com/watch?v=6_gskVKkRzc
matchload
# to new students
1. how to check drivetrain
2. Standoff coupler
3. Battery cable
4. Strengthen and protection
5. License plate
6. Game manual
7. Auton
8. Chase drive practice

1. rubber band roller: shaft collar location
2. default auton number test
3. match load test
4. horn macro

2. auton code walkthrough (where to put the robot and ball)
3. drive pracice and descore practice
4. engineering notebook (pictures)
5. license plate
6. tournament documents
7. pack box




iteration (inspired): take inspiration, use a starting point, build own innovations.
1. 2nd stage (lift): medium flaps, how many flaps, strings as back
2. top roller: flat hood, double roller, floating roller
3. bottom intake: rubber band (different width), fixed flex wheel
4. drivetrain (5 motor): 8 wheels -> 6 wheels, crossbar: 30, 28, 27, why?
5. match load: none, standoffs, flexible plastics, motorized
6. auton: write our own code (fast vs slow)
7. horn: fixed, motorized (different gear, height, length)

---
to test
1. optical sensor for color sort
2. 


Imagine you have a robot car that you want to drive in a straight line, or stop exactly at a certain spot. The problem is, the car might wobble, overshoot, or not stop in the right place. That’s where PID control comes in—it’s like giving your robot car a smart “driver” that constantly makes small adjustments.

Here’s what the letters mean:

P = Proportional
Think of this like how far off the car is from where it should be.
If the car is a little off, the correction is small. If it’s way off, the correction is bigger.
Example: If you want the car to stop on a line, but it’s still far away, P tells it to go faster. If it’s close, P slows it down.

I = Integral
This looks at all the small errors that have built up over time.
Sometimes the car might always stop a little short or a little too far, even if P tries to fix it. I adds up these mistakes and nudges the car to cancel them out.
Example: If your car always ends up stopping just before the line, I will slowly increase the push so it actually reaches the line.

D = Derivative
This looks at how fast the car is changing. It’s like predicting the future a little bit.
If the car is heading quickly toward the line, D will say “Hey, slow down now or you’ll overshoot!”
Example: Like putting on brakes early when you see a stop sign coming up.

So in short:

P fixes the error right now.

I fixes leftover errors that keep happening.

D prevents the car from rushing past the target.

When you mix P, I, and D together, the robot car drives more smoothly and stops right where you want it.

---
team to watch out 1138B

--
todo: october 5

code:
1. add break for autonbuttoncheck while loop
2. uncomment loadparameters (skip drive_mode)
3. right side auton
4. remove "auton skill"
- descore function
- change project name
-     hornMotor.setVelocity(100, percent);
    hornMotor.spinFor(reverse, 300, degrees);
    hornMotor.stop(coast);
    hornMotor.stop(hold);

    Add this line to auton.h:
extern bool autonTestMode;

Change line in buttonAAction() function from
  if(macroMode) return;
To
  if(macroMode || autonTestMode) return;

add score bottom, score middle
void scoreMiddle() {
  chassis.driveWithVoltage(8, 8);
  wait(150, msec);
  chassis.stop(hold);
  rollerBottom.spin(forward, 12, volt);
  rollerTop.spin(forward, 6, volt);
}

void outTake() {
  chassis.driveWithVoltage(-8, -8);
  wait(150, msec);
  chassis.stop(brake);
  rollerBottom.spin(forward, -10, volt);
  rollerTop.stop(coast);
}

void buttonL1Action() {
  if (controller1.ButtonR2.pressing()) // score middle goal if R2 is also pressed
  {
    outTake();
  }
  else
  {
    inTake();
  }
  while(controller1.ButtonL1.pressing()) {
      wait(20, msec);
    }
  stopRollers();
}

build:
1. dissemble (not drivetrain) prototype
2. gather material and take pictures
3. build compoments and take pictures
4. add license plate to v1

notebook:
1. pictures (big)
2. tournament documents
3. pack box
4. buttons
5. inertia sensor errors
- change auton using screen
- manual update (clamp)
- touching => not scored
- fly out match load ball

test:
1. color sort (need fine-tune)
2. funnel (done)
3. match load auton test (need fine-tune)
- top score mech (need fine-tune)
- use distance sensor to drive to matchload (done)
- use distance sensor to detect if at long goal (for decore and match load)
- macro for score & descore middle goal
- use drivetrainNeedsStopped to check if it's at long goal (do macro)
- automatically raise matchload then score long goal
- auton score middle goals
- change menu color

library:
1. change left button to up button
2. test_auton_buttons.md (done)
- additionalSetup

middle school plan
Nov 15: 
- build: v2 without horn. 
- auton: matchload then push
- after auton: 
    - pick up middle 6 balls
    - score other long goal
    - match load other long goal
- mid game
    - match load opponent match load
    - score middle goal
- end game
    - control control zone

- setup: matchlod ball orientation


high school plan
Nov 15:
- build: v2 with pistons
- auton:
    - qual: matchload then middle 3 balls
    - elim: same?
- after auton
    - alliance: attack opp
    - we: drive back the long goal and do match load
