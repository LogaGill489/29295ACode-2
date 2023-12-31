#include "main.h"
// #include "variables.hpp"

/////
// For instalattion, upgrading, documentations and tutorials, check out website!
// https://ez-robotics.github.io/EZ-Template/
/////

const int DRIVE_SPEED = 110; // This is 110/127 (around 87% of max speed).  We don't suggest making this 127.
                             // If this is 127 and the robot tries to heading correct, it's only correcting by
                             // making one side slower.  When this is 87%, it's correcting by making one side
                             // faster and one side slower, giving better heading correction.
const int TURN_SPEED = 90;
const int SWING_SPEED = 90;

///
// Constants
///

// It's best practice to tune constants when the robot is empty and with heavier game objects, or with lifts up vs down.
// If the objects are light or the cog doesn't change much, then there isn't a concern here.

void default_constants()
{
  chassis.set_slew_min_power(80, 80);
  chassis.set_slew_distance(7, 7);
  chassis.set_pid_constants(&chassis.headingPID, 11, 0, 20, 0);
  chassis.set_pid_constants(&chassis.forward_drivePID, 0.35, 0, 5, 0);
  chassis.set_pid_constants(&chassis.backward_drivePID, 0.35, 0, 5, 0);
  chassis.set_pid_constants(&chassis.turnPID, 5, 0.003, 35, 15);
  chassis.set_pid_constants(&chassis.swingPID, 7, 0, 45, 0);
}

void one_mogo_constants()
{
  chassis.set_slew_min_power(80, 80);
  chassis.set_slew_distance(7, 7);
  chassis.set_pid_constants(&chassis.headingPID, 11, 0, 20, 0);
  chassis.set_pid_constants(&chassis.forward_drivePID, 0.45, 0, 5, 0);
  chassis.set_pid_constants(&chassis.backward_drivePID, 0.45, 0, 5, 0);
  chassis.set_pid_constants(&chassis.turnPID, 5, 0.003, 35, 15);
  chassis.set_pid_constants(&chassis.swingPID, 7, 0, 45, 0);
}

void two_mogo_constants()
{
  chassis.set_slew_min_power(80, 80);
  chassis.set_slew_distance(7, 7);
  chassis.set_pid_constants(&chassis.headingPID, 11, 0, 20, 0);
  chassis.set_pid_constants(&chassis.forward_drivePID, 0.45, 0, 5, 0);
  chassis.set_pid_constants(&chassis.backward_drivePID, 0.45, 0, 5, 0);
  chassis.set_pid_constants(&chassis.turnPID, 5, 0.003, 35, 15);
  chassis.set_pid_constants(&chassis.swingPID, 7, 0, 45, 0);
}

void exit_condition_defaults()
{
  chassis.set_exit_condition(chassis.turn_exit, 100, 3, 500, 7, 500, 500);
  chassis.set_exit_condition(chassis.swing_exit, 100, 3, 500, 7, 500, 500);
  chassis.set_exit_condition(chassis.drive_exit, 80, 50, 300, 150, 500, 500);
}

void modified_exit_condition()
{
  chassis.set_exit_condition(chassis.turn_exit, 100, 3, 500, 7, 500, 500);
  chassis.set_exit_condition(chassis.swing_exit, 100, 3, 500, 7, 500, 500);
  chassis.set_exit_condition(chassis.drive_exit, 80, 50, 300, 150, 500, 500);
}

///
// Drive Example
///
void drive_example()
{
  // The first parameter is target inches
  // The second parameter is max speed the robot will drive at
  // The third parameter is a boolean (true or false) for enabling/disabling a slew at the start of drive motions
  // for slew, only enable it when the drive distance is greater then the slew distance + a few inches

  chassis.set_drive_pid(24, DRIVE_SPEED, true);
  chassis.wait_drive();

  chassis.set_drive_pid(-12, DRIVE_SPEED);
  chassis.wait_drive();

  chassis.set_drive_pid(-12, DRIVE_SPEED);
  chassis.wait_drive();
}

///
// Turn Example
///
void turn_example()
{
  // The first parameter is target degrees
  // The second parameter is max speed the robot will drive at

  chassis.set_turn_pid(90, TURN_SPEED);
  chassis.wait_drive();

  chassis.set_turn_pid(45, TURN_SPEED);
  chassis.wait_drive();

  chassis.set_turn_pid(0, TURN_SPEED);
  chassis.wait_drive();
}

///
// Wait Until and Changing Max Speed
///
void wait_until_change_speed()
{
  // wait_until will wait until the robot gets to a desired position

  // When the robot gets to 6 inches, the robot will travel the remaining distance at a max speed of 40
  chassis.set_drive_pid(24, DRIVE_SPEED, true);
  chassis.wait_until(6);
  chassis.set_max_speed(40); // After driving 6 inches at DRIVE_SPEED, the robot will go the remaining distance at 40 speed
  chassis.wait_drive();

  chassis.set_turn_pid(45, TURN_SPEED);
  chassis.wait_drive();

  chassis.set_turn_pid(-45, TURN_SPEED);
  chassis.wait_drive();

  chassis.set_turn_pid(0, TURN_SPEED);
  chassis.wait_drive();

  // When the robot gets to -6 inches, the robot will travel the remaining distance at a max speed of 40
  chassis.set_drive_pid(-24, DRIVE_SPEED, true);
  chassis.wait_until(-6);
  chassis.set_max_speed(40); // After driving 6 inches at DRIVE_SPEED, the robot will go the remaining distance at 40 speed
  chassis.wait_drive();
}

///
// Swing Example
///
void swing_example()
{
  // The first parameter is ez::LEFT_SWING or ez::RIGHT_SWING
  // The second parameter is target degrees
  // The third parameter is speed of the moving side of the drive

  chassis.set_swing_pid(ez::LEFT_SWING, 45, SWING_SPEED);
  chassis.wait_drive();

  chassis.set_drive_pid(24, DRIVE_SPEED, true);
  chassis.wait_until(12);

  chassis.set_swing_pid(ez::RIGHT_SWING, 0, SWING_SPEED);
  chassis.wait_drive();
}

///
// Interference example
///
void tug(int attempts)
{
  for (int i = 0; i < attempts - 1; i++)
  {
    // Attempt to drive backwards
    printf("i - %i", i);
    chassis.set_drive_pid(-12, 127);
    chassis.wait_drive();

    // If failsafed...
    if (chassis.interfered)
    {
      chassis.reset_drive_sensor();
      chassis.set_drive_pid(-2, 20);
      pros::delay(1000);
    }
    // If robot successfully drove back, return
    else
    {
      return;
    }
  }
}

// If there is no interference, robot will drive forward and turn 90 degrees.
// If interfered, robot will drive forward and then attempt to drive backwards.
void interfered_example()
{
  chassis.set_drive_pid(24, DRIVE_SPEED, true);
  chassis.wait_drive();

  if (chassis.interfered)
  {
    tug(3);
    return;
  }

  chassis.set_turn_pid(90, TURN_SPEED);
  chassis.wait_drive();
}

///
///
///
///

void defGame()
{
  pros::Motor intake(8, pros::E_MOTOR_GEARSET_06);
  pros::ADIDigitalOut Wings('G');

  static pros::Motor cataleft(12, pros::E_MOTOR_GEARSET_36);
  static pros::Motor cataright(-19, pros::E_MOTOR_GEARSET_36); // comment out with prog
  // static pros::Motor cataright(9000, pros::E_MOTOR_GEARSET_36); // comment out with real bort

  static pros::Motor_Group cata({cataleft, cataright});

  int intakeTime = 190;

  chassis.set_drive_pid(100, 127);
  chassis.wait_until(33);

  chassis.set_drive_pid(-140, 127);
  chassis.wait_until(-110);

  chassis.set_turn_pid(45, 127);
  chassis.wait_drive();

  // Wings.set_value(true);

  chassis.set_drive_pid(-140, 127);
  chassis.wait_until(-65);

  chassis.set_turn_pid(90, 127);
  chassis.wait_drive();

  intake.move(-127);
  pros::delay(300);
  intake.move(0);

  chassis.set_turn_pid(-90, 127);
  chassis.wait_drive();

  chassis.set_drive_pid(100, 127);
  chassis.wait_until(50);

  // scored

  chassis.set_drive_pid(-100, 127);
  chassis.wait_until(-40);

  chassis.set_turn_pid(130, 127);
  chassis.wait_drive();

  chassis.set_drive_pid(-100, 127);
  chassis.wait_until(-80);

  intake.move(127);
  pros::delay(230);
  intake.move(0);

  // Wings.set_value(true);

  chassis.set_turn_pid(180, 127);
  chassis.wait_drive();

  intake.move(-127);
  pros::delay(230);
  intake.move(0);

  chassis.set_drive_pid(-100, 127);
  chassis.wait_until(-70);
}

/*
 * Autonomous Routines designed in EZ Template for 29295A's Team Bot
 * Do Not Reveal This Code to Outside Sources
 * LemLib is Currently Being Tested And Could Replace Current Code
 * First Autonomous Function is For the In-Game 15 Second Autonmous
 * Second is For the Skills Run
 * Remember, We Ball
 */

/*
  Psuedo-Code (In-Game)


*/

// runs 15 autonomous routine for the game.
void gameAuton()
{
  pros::Motor intake(8, pros::E_MOTOR_GEARSET_06);
  pros::ADIDigitalOut Wings('G');

  double rC = -1; // conversion between prog bot and team bot
  int intakeTime = 190;

  chassis.set_drive_pid(-40 * rC, 127);
  chassis.wait_until(-20 * rC);

  chassis.set_drive_pid(90 * rC, 127);
  chassis.wait_until(38 * rC);
  // chassis.reset_pid_targets();

  intake.move(127);
  pros::delay(intakeTime * 1.3);
  intake.move(32);

  chassis.set_drive_pid(-200 * rC, 127);
  chassis.wait_until(-110 * rC);

  // chassis.set_turn_pid(40, TURN_SPEED);
  // chassis.wait_drive();

  chassis.set_turn_pid(135, 127);
  chassis.wait_drive();

  // intake output
  intake.move(-90);
  pros::delay(intakeTime * 2);
  intake.move(0);

  chassis.set_turn_pid(-37, 127);
  chassis.wait_drive();

  // left arm extends

  // Wings.set_value(true);
  // pros::delay(200);

  // 360

  // chassis.set_turn_pid(-400, 127);
  // chassis.wait_drive();

  chassis.set_drive_pid(-120 * rC, 127);
  chassis.wait_until(-81 * rC);

  chassis.set_swing_pid(ez::RIGHT_SWING, -90, 127);
  chassis.wait_drive();

  // Wings.set_value(false);

  chassis.set_drive_pid(-90 * rC, 127);
  chassis.wait_until(-34 * rC);

  chassis.set_drive_pid(60 * rC, 127);
  chassis.wait_until(38 * rC);

  chassis.set_turn_pid(18, 127);
  chassis.wait_drive();

  chassis.set_drive_pid(240 * rC, 127);
  chassis.wait_until(170 * rC);

  intake.move(127);
  pros::delay(intakeTime);
  intake.move(0);

  chassis.set_turn_pid(150, 127);
  chassis.wait_drive();

  intake.move(-127);
  chassis.set_drive_pid(120 * rC, 127);
  chassis.wait_until(20 * rC);
  intake.move(0);

  chassis.set_turn_pid(56, 127);
  chassis.wait_drive();

  chassis.set_drive_pid(120 * rC, 127);
  chassis.wait_until(56 * rC);

  intake.move(127);
  pros::delay(intakeTime * 1.45);
  intake.move(32);

  chassis.set_drive_pid(-120 * rC, 127);
  chassis.wait_until(-16 * rC);

  chassis.set_turn_pid(-25, 127);
  chassis.wait_drive();

  Wings.set_value(true);

  chassis.set_drive_pid(-180 * rC, 127);
  chassis.wait_until(-126 * rC);

  chassis.set_drive_pid(90 * rC, 127);
  chassis.wait_until(40 * rC);

  Wings.set_value(false);

  chassis.set_turn_pid(180, 127);
  chassis.wait_drive();

  intake.move(-127);
  pros::delay(intakeTime * 1.35);
  intake.move(0);

  // extend both arms

  chassis.set_drive_pid(60 * rC, 127);
  chassis.wait_until(35 * rC);

  chassis.set_drive_pid(-40, 127);
  chassis.wait_drive();
  // Wings.set_value(false);

  /*

  chassis.set_turn_pid(180, 127);
  chassis.wait_drive();

  intake.move(-127);
  pros::delay(intakeTime * 1.2);
  intake.move(0);

  chassis.set_drive_pid(90 * rC, 127);
  chassis.wait_until(45 * rC);

  */

  for (int i = 0; i < 2; i++)
  {
    chassis.left_motors[i].move(0);
    chassis.right_motors[i].move(0);
  }
  while (1)
  {
    // intake.move(0);
    // cata.move(0);
    pros::delay(50);
  } // while closer (comment here if necessary)
}
/*
  Psuedo-Code (Skills)
  - Allign With Location For Match Loads
  - Match Load Certain Ammount Using a Delay (Or Millis if Necessary) With Cata Going
  - Allign and Drive Forward on an Arc To Allign Perp With Middle Bar
  - Extend Pnuematic Arm and Grab Corner Ball
  - Close Arm and Ram in On Arc
  - Back Out and Arc To Far Side
  - Both Arms Extend And Push On A Slight Angle
  - Back Up and Arc to Final Position
  - Push Other Side In To Finish
*/

// skills autonomous routine //no while loop is needed as it is preset to 1 minute
void skillsAuton()
{
  static pros::Motor cataleft(12, pros::E_MOTOR_GEARSET_36);
  static pros::Motor cataright(-19, pros::E_MOTOR_GEARSET_36); // comment out with prog
  // static pros::Motor cataright(9000, pros::E_MOTOR_GEARSET_36); // comment out with real bort

  static pros::Motor_Group cata({cataleft, cataright});
pros::Rotation rot(2);

  pros::ADIDigitalOut Wings('G');

  rot.set_position(0);

  int turnChange = 23;

    chassis.set_drive_pid(90, 127);
  chassis.wait_until(15);

    chassis.set_drive_pid(-22, 127);
  chassis.wait_drive();

    cata.move(127);
  pros::delay(41300);

    //pros::delay(2600);
  cata.move(0);

  chassis.set_turn_pid(-110, 127);
  chassis.wait_drive();

  chassis.set_drive_pid(90, 127);
  chassis.wait_until(65);

  chassis.set_turn_pid(-80, 127);
  chassis.wait_drive();

      chassis.set_drive_pid(130, 127);
  chassis.wait_until(45);

      chassis.set_drive_pid(-130, 127);
  chassis.wait_until(-25);

  chassis.set_turn_pid(60, 127);
  chassis.wait_drive();

        chassis.set_drive_pid(130, 127);
  chassis.wait_until(110);

    chassis.set_turn_pid(23, 127);
  chassis.wait_drive();

  chassis.set_drive_pid(330, 127);
  chassis.wait_until(290);

  // Wings.set_value(false);

  chassis.set_turn_pid(turnChange + -50, 90);
  // chassis.set_drive_pid(120, 127);
  chassis.wait_drive();

   //Wings.set_value(true);

  chassis.set_drive_pid(220, 127);
  chassis.wait_until(150);

    //Wings.set_value(false);

      chassis.set_drive_pid(-220, 127);
  chassis.wait_until(-20);

  chassis.set_turn_pid(turnChange + -160, 90);
  // chassis.set_drive_pid(120, 127);
  chassis.wait_drive();

  chassis.set_drive_pid(220, 127);
  chassis.wait_until(120);

  chassis.set_turn_pid(turnChange + -40, 127);
  chassis.wait_drive();

  Wings.set_value(true);

  chassis.set_drive_pid(200, 127);
  chassis.wait_until(123);

  Wings.set_value(false);

  chassis.set_turn_pid(turnChange + 0, 127);
  chassis.wait_drive();

  chassis.set_drive_pid(-150, 127);
  chassis.wait_until(-100);

  chassis.set_turn_pid(turnChange + 90, 127);
  chassis.wait_drive();

  chassis.set_drive_pid(-150, 127);
  chassis.wait_until(-100);

  chassis.set_turn_pid(turnChange + 20, 127);
  chassis.wait_drive();

  Wings.set_value(true);

  chassis.set_drive_pid(160, 127);
  chassis.wait_until(120);

  Wings.set_value(false);

  chassis.set_turn_pid(turnChange + -20, 127);
  chassis.wait_drive();

  chassis.set_drive_pid(-160, 127);
  chassis.wait_until(-100);

  chassis.set_turn_pid(turnChange + 0, 127);
  chassis.wait_drive();

  Wings.set_value(true);

  chassis.set_drive_pid(160, 127);
  chassis.wait_until(120);

  while (1)
  {
    pros::delay(100);
  }
}