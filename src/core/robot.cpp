/*
 * Copyright (c) 2016 WinT 3794 <http://wint3794.org>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "robot.h"

//===============================================================================
// Autonomous options
//===============================================================================

#define AUTO_DEFAULT_TIME   5
#define SD_AUTO_CHOOSER    "Autonomous modes"
#define SD_HANDS           "Hands"
#define SD_INTAKE          "Intake"
#define SD_LIFTER          "Lifter"
#define SD_DRIVEX          "Drive X"
#define SD_DRIVEY          "Drive Y"
#define SD_SHOOTER         "Shooter"
#define SD_SHOOT_ALLIGN    "Shoot Alligner"
#define SD_HANDS_ET        "Hands End (secs)"
#define SD_LIFTER_ET       "Lifter End (secs)"
#define SD_INTAKE_ET       "Intake End (secs)"
#define SD_DRIVEX_ET       "Drive X End (secs)"
#define SD_DRIVEY_ET       "Drive Y End (secs)"
#define SD_SHOOTER_ET      "Shooter End (secs)"
#define SD_SHOOT_ALLIGN_ET "Shoot Allign End (secs)"
#define SD_HANDS_ST        "Hands Start (secs)"
#define SD_LIFTER_ST       "Lifter Start (secs)"
#define SD_INTAKE_ST       "Intake Start (secs)"
#define SD_DRIVEX_ST       "Drive X Start (secs)"
#define SD_DRIVEY_ST       "Drive Y Start (secs)"
#define SD_SHOOTER_ST      "Shooter Start (secs)"
#define SD_SHOOT_ALLIGN_ST "Shoot Allign Start (secs)"

//===============================================================================
// Autonomous commands/modes list
//===============================================================================

void* CMD_NONE;
void* CMD_CUSTOM;
void* CMD_DRIVE_FORWARD;
void* CMD_DRIVE_REVERSE;
void* CMD_SHOOT_NO_DRIVE;
void* CMD_JUST_TAKE_THAT_BALL;
void* CMD_TAKE_BALL_AND_SHOOT;
void* CMD_DRIVE_FWD_AND_SHOOT;
void* CMD_DRIVE_REV_AND_SHOOT;
void* CMD_TAKE_BALL_AND_DRIVE_FWD;
void* CMD_TAKE_BALL_AND_DRIVE_REV;

//===============================================================================
// Robot::RobotInit
//===============================================================================

void Robot::RobotInit() {
    /* Create subsystems */
    m_subsystemHands      = make_unique<Hands>();
    m_subsystemLifter     = make_unique<Lifter>();
    m_subsystemIntake     = make_unique<Intake>();
    m_subsystemShooter    = make_unique<Shooter>();
    m_subsystemPowertrain = make_unique<Powertrain>();

    /* Create joysticks */
    m_driveJoystick = make_unique<Joystick> (0);
    m_secndJoystick = make_unique<Joystick> (1);

    /* Register autonomous commands */
    m_auto_chooser = make_unique<SendableChooser>();
    m_auto_chooser->AddDefault ("None",                  CMD_NONE);
    m_auto_chooser->AddObject  ("Custom",                CMD_CUSTOM);
    m_auto_chooser->AddObject  ("Drive Forward",         CMD_DRIVE_FORWARD);
    m_auto_chooser->AddObject  ("Drive Reverse",         CMD_DRIVE_REVERSE);
    m_auto_chooser->AddObject  ("Shoot (no drive)",      CMD_SHOOT_NO_DRIVE);
    m_auto_chooser->AddObject  ("Shoot (drive fwd)",     CMD_DRIVE_FWD_AND_SHOOT);
    m_auto_chooser->AddObject  ("Shoot (drive rev)",     CMD_DRIVE_REV_AND_SHOOT);
    m_auto_chooser->AddObject  ("Take ball",             CMD_JUST_TAKE_THAT_BALL);
    m_auto_chooser->AddObject  ("Take ball & shoot",     CMD_TAKE_BALL_AND_SHOOT);
    m_auto_chooser->AddObject  ("Take ball & drive fwd", CMD_TAKE_BALL_AND_DRIVE_FWD);
    m_auto_chooser->AddObject  ("Take ball & drive rev", CMD_TAKE_BALL_AND_DRIVE_REV);

    /* Publish robot options to the Dashboard */
    resetAutoValues();
    putDashboardValues();
}

//===============================================================================
// Robot::AutonomousInit
//===============================================================================

void Robot::AutonomousInit() {
    void* selected = m_auto_chooser->GetSelected();

    /* Do nothing */
    if (selected == CMD_NONE)
        resetAutoValues();

    /* Use the specified values by the drivers */
    else if (selected == CMD_CUSTOM)
        getDashboardValues();

    /* Drive forward */
    else if (selected == CMD_DRIVE_FORWARD)
        resetAutoValues();

    /* Drive backward */
    else if (selected == CMD_DRIVE_REVERSE)
        resetAutoValues();

    /* Shoot baby! */
    else if (selected == CMD_SHOOT_NO_DRIVE)
        resetAutoValues();

    /* Rotate the intake forward to obtain the ball */
    else if (selected == CMD_JUST_TAKE_THAT_BALL)
        resetAutoValues();

    /* Obtain the ball and shoot */
    else if (selected == CMD_TAKE_BALL_AND_SHOOT)
        resetAutoValues();

    /* Drive forward and shoot */
    else if (selected == CMD_DRIVE_FWD_AND_SHOOT)
        resetAutoValues();

    /* Drive backward and shoot */
    else if (selected == CMD_DRIVE_REV_AND_SHOOT)
        resetAutoValues();

    /* Obtain the ball and drive forward */
    else if (selected == CMD_TAKE_BALL_AND_DRIVE_FWD)
        resetAutoValues();

    /* Obtain the ball and drive backward */
    else if (selected == CMD_TAKE_BALL_AND_DRIVE_REV)
        resetAutoValues();

    /* Do nothing if something is wrong */
    else
        resetAutoValues();

    /* Update the displayed values on the SmartDashboard */
    putDashboardValues();
}

//===============================================================================
// Robot::TeleopPeriodic
//===============================================================================

void Robot::TeleopPeriodic() {
    m_subsystemHands->move       (*m_secndJoystick.get());
    m_subsystemLifter->move      (*m_driveJoystick.get());
    m_subsystemIntake->move      (*m_driveJoystick.get());
    m_subsystemShooter->shoot    (*m_secndJoystick.get());
    m_subsystemPowertrain->drive (*m_driveJoystick.get(), *m_secndJoystick.get());
}

//===============================================================================
// Robot::AutonomousPeriodic
//===============================================================================

void Robot::AutonomousPeriodic() {
}

//===============================================================================
// Robot::resetAutoValues
//===============================================================================

void Robot::resetAutoValues() {
    m_auto_hands           = 0;
    m_auto_intake          = 0;
    m_auto_lifter          = 0;
    m_auto_drive_x         = 0;
    m_auto_drive_y         = 0;
    m_auto_shooter         = 0;
    m_auto_shoot_allign    = 0;
    m_auto_hands_st        = 0;
    m_auto_lifter_st       = 0;
    m_auto_intake_st       = 0;
    m_auto_drivex_st       = 0;
    m_auto_drivey_st       = 0;
    m_auto_shooter_st      = 0;
    m_auto_shoot_allign_st = 0;
    m_auto_hands_et        = 0;
    m_auto_lifter_et       = 0;
    m_auto_intake_et       = 0;
    m_auto_drivex_et       = 0;
    m_auto_drivey_et       = 0;
    m_auto_shooter_et      = 0;
    m_auto_shoot_allign_et = 0;
}

//===============================================================================
// Robot::readDashboardValues
//===============================================================================

void Robot::getDashboardValues() {
    /* Update actuator values */
    m_auto_hands           = SD::GetNumber (SD_HANDS,           0);
    m_auto_intake          = SD::GetNumber (SD_INTAKE,          0);
    m_auto_lifter          = SD::GetNumber (SD_LIFTER,          0);
    m_auto_drive_x         = SD::GetNumber (SD_DRIVEX,          0);
    m_auto_drive_y         = SD::GetNumber (SD_DRIVEY,          0);
    m_auto_shooter         = SD::GetNumber (SD_SHOOTER,         0);
    m_auto_shoot_allign    = SD::GetNumber (SD_SHOOT_ALLIGN,    0);

    /* Update end times */
    m_auto_hands_et        = SD::GetNumber (SD_HANDS_ET,        0);
    m_auto_lifter_et       = SD::GetNumber (SD_LIFTER_ET,       0);
    m_auto_intake_et       = SD::GetNumber (SD_INTAKE_ET,       0);
    m_auto_drivex_et       = SD::GetNumber (SD_DRIVEX_ET,       0);
    m_auto_drivey_et       = SD::GetNumber (SD_DRIVEY_ET,       0);
    m_auto_shooter_et      = SD::GetNumber (SD_SHOOTER_ET,      0);
    m_auto_shoot_allign_et = SD::GetNumber (SD_SHOOT_ALLIGN_ET, 0);

    /* Update start times */
    m_auto_hands_st        = SD::GetNumber (SD_HANDS_ST,        0);
    m_auto_lifter_st       = SD::GetNumber (SD_LIFTER_ST,       0);
    m_auto_intake_st       = SD::GetNumber (SD_INTAKE_ST,       0);
    m_auto_drivex_st       = SD::GetNumber (SD_DRIVEX_ST,       0);
    m_auto_drivey_st       = SD::GetNumber (SD_DRIVEY_ST,       0);
    m_auto_shooter_st      = SD::GetNumber (SD_SHOOTER_ST,      0);
    m_auto_shoot_allign_st = SD::GetNumber (SD_SHOOT_ALLIGN_ST, 0);
}

//===============================================================================
// Robot::putDashboardValues
//===============================================================================

void Robot::putDashboardValues() {
    /* Publish autonomous chooser */
    SD::PutData (SD_AUTO_CHOOSER, m_auto_chooser.get());

    /* Publish autonomous output values */
    SD::PutNumber (SD_HANDS,           m_auto_hands);
    SD::PutNumber (SD_INTAKE,          m_auto_intake);
    SD::PutNumber (SD_LIFTER,          m_auto_lifter);
    SD::PutNumber (SD_DRIVEX,          m_auto_drive_x);
    SD::PutNumber (SD_DRIVEY,          m_auto_drive_y);
    SD::PutNumber (SD_SHOOTER,         m_auto_shooter);
    SD::PutNumber (SD_SHOOT_ALLIGN,    m_auto_shoot_allign);

    /* Publish autonomous start times */
    SD::PutNumber (SD_HANDS_ST,        m_auto_hands_st);
    SD::PutNumber (SD_LIFTER_ST,       m_auto_lifter_st);
    SD::PutNumber (SD_INTAKE_ST,       m_auto_intake_st);
    SD::PutNumber (SD_DRIVEX_ST,       m_auto_drivex_st);
    SD::PutNumber (SD_DRIVEY_ST,       m_auto_drivey_st);
    SD::PutNumber (SD_SHOOTER_ST,      m_auto_shooter_st);
    SD::PutNumber (SD_SHOOT_ALLIGN_ST, m_auto_shoot_allign_st);

    /* Publish autonomous end times */
    SD::PutNumber (SD_HANDS_ET,        m_auto_hands_et);
    SD::PutNumber (SD_LIFTER_ET,       m_auto_lifter_et);
    SD::PutNumber (SD_INTAKE_ET,       m_auto_intake_et);
    SD::PutNumber (SD_DRIVEX_ET,       m_auto_drivex_et);
    SD::PutNumber (SD_DRIVEY_ET,       m_auto_drivey_et);
    SD::PutNumber (SD_SHOOTER_ET,      m_auto_shooter_et);
    SD::PutNumber (SD_SHOOT_ALLIGN_ET, m_auto_shoot_allign_et);
}
