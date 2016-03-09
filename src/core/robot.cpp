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

static void* CMD_NONE;
static void* CMD_CUSTOM;
static void* CMD_DRIVE_FORWARD;
static void* CMD_DRIVE_REVERSE;
static void* CMD_SHOOT_NO_DRIVE;
static void* CMD_DRIVE_DROP_HANDS;
static void* CMD_JUST_TAKE_THAT_BALL;
static void* CMD_TAKE_BALL_AND_SHOOT;
static void* CMD_DRIVE_FWD_AND_SHOOT;
static void* CMD_DRIVE_REV_AND_SHOOT;
static void* CMD_TAKE_BALL_AND_DRIVE;

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

    /* Configure timer */
    m_timer = make_unique<Timer>();

    /* Register autonomous commands */
    m_auto_chooser = make_unique<SendableChooser>();
    m_auto_chooser->AddDefault ("None",               CMD_NONE);
    m_auto_chooser->AddObject  ("Custom",             CMD_CUSTOM);
    m_auto_chooser->AddObject  ("Drive Forward",      CMD_DRIVE_FORWARD);
    m_auto_chooser->AddObject  ("Drive Reverse",      CMD_DRIVE_REVERSE);
    m_auto_chooser->AddObject  ("Drive & Drop Hands", CMD_DRIVE_DROP_HANDS);
    m_auto_chooser->AddObject  ("Shoot (no drive)",   CMD_SHOOT_NO_DRIVE);
    m_auto_chooser->AddObject  ("Shoot (drive fwd)",  CMD_DRIVE_FWD_AND_SHOOT);
    m_auto_chooser->AddObject  ("Shoot (drive rev)",  CMD_DRIVE_REV_AND_SHOOT);
    m_auto_chooser->AddObject  ("Take ball",          CMD_JUST_TAKE_THAT_BALL);
    m_auto_chooser->AddObject  ("Take ball & shoot",  CMD_TAKE_BALL_AND_SHOOT);
    m_auto_chooser->AddObject  ("Take ball & drive",  CMD_TAKE_BALL_AND_DRIVE);

    /* Publish robot options to the Dashboard */
    resetAutoOutputs();
    putDashboardValues();
}

//===============================================================================
// Robot::TeleopInit
//===============================================================================

void Robot::TeleopInit() {
    m_timer->Stop();
    putDashboardValues();
}

//===============================================================================
// Robot::DisabledInit
//===============================================================================

void Robot::DisabledInit() {
    m_timer->Stop();
    putDashboardValues();
}

//===============================================================================
// Robot::AutonomousInit
//===============================================================================

void Robot::AutonomousInit() {
    void* selected = m_auto_chooser->GetSelected();

    /* Do nothing */
    if (selected == CMD_NONE)
        resetAutoOutputs();

    /* Use the specified values by the drivers */
    else if (selected == CMD_CUSTOM)
        getDashboardValues();

    /* Drive forward */
    else if (selected == CMD_DRIVE_FORWARD) {
        resetAutoOutputs();

        m_auto_drive_y   = 0.8;
        m_auto_drivey_st = 1.0;
        m_auto_drivey_et = m_auto_drivey_st + 5.0;
    }

    /* Drive backward */
    else if (selected == CMD_DRIVE_REVERSE) {
        resetAutoOutputs();

        m_auto_drive_y   = 0.8 * -1;
        m_auto_drivey_st = 1.0;
        m_auto_drivey_et = m_auto_drivey_st + 5.0;
    }

    /* Drop hands & drive forward */
    else if (selected == CMD_DRIVE_DROP_HANDS) {
        resetAutoOutputs();

        m_auto_hands = 0.4;
        m_auto_hands_st = 1.0;
        m_auto_hands_et = 3 + m_auto_hands_st;

        m_auto_drive_y = 0.8;
        m_auto_drivey_st = m_auto_hands_et + 1.0;
        m_auto_drivey_et = m_auto_drivex_st + 5.0;
    }

    /* Shoot baby! */
    else if (selected == CMD_SHOOT_NO_DRIVE) {
        resetAutoOutputs();

        m_auto_shooter = 1.0;
        m_auto_shooter_st = 1.0;
        m_auto_shooter_et = m_auto_shooter_st + 5.0;

        m_auto_shoot_allign = 0.6;
        m_auto_shoot_allign_st = 3.0;
        m_auto_shoot_allign_et = m_auto_shoot_allign_st + 2.0;
    }

    /* Drive forward and rotate the intake */
    else if (selected == CMD_JUST_TAKE_THAT_BALL) {
        resetAutoOutputs();

        m_auto_intake = 1.0;
        m_auto_intake_st = 1.0;
        m_auto_intake_et = m_auto_intake_st + 8.0;

        m_auto_drive_y = 0.4;
        m_auto_drivey_st = m_auto_intake_st + 3.0;
        m_auto_drivey_et = m_auto_drivey_st + 3.0;
    }

    /* Obtain the ball and shoot */
    else if (selected == CMD_TAKE_BALL_AND_SHOOT) {
        resetAutoOutputs();

        m_auto_intake = 1.0;
        m_auto_intake_st = 1.0;
        m_auto_intake_et = m_auto_intake_st + 8.0;

        m_auto_drive_y = 0.4;
        m_auto_drivey_st = m_auto_intake_st + 3.0;
        m_auto_drivey_et = m_auto_drivey_st + 3.0;

        m_auto_shooter = 1.0;
        m_auto_shooter_st = m_auto_intake_et + 1.0;
        m_auto_shooter_et = m_auto_shooter_st + 5.0;

        m_auto_shoot_allign = 0.6;
        m_auto_shoot_allign_st = m_auto_intake_et + 3.0;
        m_auto_shoot_allign_et = m_auto_shoot_allign_st + 2.0;
    }

    /* Drive forward and shoot */
    else if (selected == CMD_DRIVE_FWD_AND_SHOOT) {
        resetAutoOutputs();

        m_auto_drive_y = 0.6;
        m_auto_drivey_st = 1.0;
        m_auto_drivey_et = m_auto_drivey_st + 5.0;

        m_auto_shooter = 1.0;
        m_auto_shooter_st = m_auto_drivey_et;
        m_auto_shooter_et = m_auto_shooter_st + 5.0;

        m_auto_shoot_allign = 0.6;
        m_auto_shoot_allign_st = 3.0;
        m_auto_shoot_allign_et = m_auto_shoot_allign_st + 2.0;
    }

    /* Drive backward and shoot */
    else if (selected == CMD_DRIVE_REV_AND_SHOOT) {
        resetAutoOutputs();

        m_auto_drive_y = 0.6 * -1;
        m_auto_drivey_st = 1.0;
        m_auto_drivey_et = m_auto_drivey_st + 5.0;

        m_auto_shooter = 1.0;
        m_auto_shooter_st = m_auto_drivey_et;
        m_auto_shooter_et = m_auto_shooter_st + 5.0;

        m_auto_shoot_allign = 0.6;
        m_auto_shoot_allign_st = 3.0;
        m_auto_shoot_allign_et = m_auto_shoot_allign_st + 2.0;

    }

    /* Obtain the ball, rotate chassis and drive forward */
    else if (selected == CMD_TAKE_BALL_AND_DRIVE) {
        resetAutoOutputs();

        m_auto_intake = 1.0;
        m_auto_intake_st = 1.0;
        m_auto_intake_et = m_auto_intake_st + 8.0;

        m_auto_drive_x = 0.6;
        m_auto_drivex_st = m_auto_intake_st + 3.0;
        m_auto_drivex_et = m_auto_drivex_st + 3.0;

        m_auto_drive_y = 0.8;
        m_auto_drivey_st = m_auto_drivex_et + 3.0;
        m_auto_drivey_et = m_auto_drivey_st + 5.0;
    }

    /* Do nothing if something is wrong */
    else
        resetAutoOutputs();

    /* Update the displayed values on the SmartDashboard */
    putDashboardValues();

    /* Reset autonomous states */
    m_auto_hands_started          = false;
    m_auto_intake_started         = false;
    m_auto_lifter_started         = false;
    m_auto_drive_x_started        = false;
    m_auto_drive_y_started        = false;
    m_auto_shooter_started        = false;
    m_auto_shooter_allign_started = false;
    m_auto_hands_ended            = false;
    m_auto_intake_ended           = false;
    m_auto_lifter_ended           = false;
    m_auto_drive_x_ended          = false;
    m_auto_drive_y_ended          = false;
    m_auto_shooter_ended          = false;
    m_auto_shooter_allign_ended   = false;
    m_auto_hands_enabled          = false;
    m_auto_intake_enabled         = false;
    m_auto_lifter_enabled         = false;
    m_auto_drive_x_enabled        = false;
    m_auto_drive_y_enabled        = false;
    m_auto_shooter_enabled        = false;
    m_auto_shooter_allign_enabled = false;

    /* Start autonomous timer */
    m_timer->Reset();
    m_timer->Start();
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
    /* Calculate outputs */
    float hands        = m_auto_hands_enabled ? m_auto_hands : 0;
    float intake       = m_auto_intake_enabled ? m_auto_intake : 0;
    float shooter      = m_auto_shooter_enabled ? m_auto_shooter : 0;
    float drive_x      = m_auto_drive_x_enabled ? m_auto_drive_x : 0;
    float drive_y      = m_auto_drive_y_enabled ? m_auto_drive_y : 0;
    float shoot_allign = m_auto_shooter_allign_enabled ? m_auto_shoot_allign : 0;
    SOL_VALUE lifter   = m_auto_lifter_enabled ? m_auto_lifter : DoubleSolenoid::kOff;

    /* Apply calculated outputs */
    m_subsystemHands->move                (hands);
    m_subsystemLifter->move               (lifter);
    m_subsystemIntake->move               (intake);
    m_subsystemShooter->shoot             (shooter);
    m_subsystemShooter->moveBallToShooter (shoot_allign);
    m_subsystemPowertrain->drive          (drive_x, drive_y, 0, false);

    /* Update enabled state of each system */
    updateAutonomousSystems();
}

//===============================================================================
// Robot::resetAutoOutputs
//===============================================================================

void Robot::resetAutoOutputs() {
    m_auto_hands                  = 0;
    m_auto_intake                 = 0;
    m_auto_drive_x                = 0;
    m_auto_drive_y                = 0;
    m_auto_shooter                = 0;
    m_auto_shoot_allign           = 0;
    m_auto_hands_st               = 0;
    m_auto_lifter_st              = 0;
    m_auto_intake_st              = 0;
    m_auto_drivex_st              = 0;
    m_auto_drivey_st              = 0;
    m_auto_shooter_st             = 0;
    m_auto_shoot_allign_st        = 0;
    m_auto_hands_et               = 0;
    m_auto_lifter_et              = 0;
    m_auto_intake_et              = 0;
    m_auto_drivex_et              = 0;
    m_auto_drivey_et              = 0;
    m_auto_shooter_et             = 0;
    m_auto_shoot_allign_et        = 0;
    m_auto_lifter                 = DoubleSolenoid::kOff;
}

//===============================================================================
// Robot::readDashboardValues
//===============================================================================

void Robot::getDashboardValues() {
    /* Update actuator values */
	m_auto_lifter          = DoubleSolenoid::kOff;
    m_auto_hands           = SD::GetNumber (SD_HANDS,           0);
    m_auto_intake          = SD::GetNumber (SD_INTAKE,          0);
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

//===============================================================================
// Robot::updateAutonomousSystems
//===============================================================================

void Robot::updateAutonomousSystems() {
    double time = m_timer->Get();

    /* Know which systems should be running */
    m_auto_hands_started          = (time >= m_auto_hands_st);
    m_auto_intake_started         = (time >= m_auto_intake_st);
    m_auto_lifter_started         = (time >= m_auto_lifter_st);
    m_auto_drive_x_started        = (time >= m_auto_drivex_st);
    m_auto_drive_y_started        = (time >= m_auto_drivey_st);
    m_auto_shooter_started        = (time >= m_auto_shooter_st);
    m_auto_shooter_allign_started = (time >= m_auto_shoot_allign_st);

    /* Know which systems should not be running */
    m_auto_hands_ended            = (time <= m_auto_hands_et);
    m_auto_intake_ended           = (time <= m_auto_intake_et);
    m_auto_lifter_ended           = (time <= m_auto_lifter_et);
    m_auto_drive_x_ended          = (time <= m_auto_drivex_et);
    m_auto_drive_y_ended          = (time <= m_auto_drivey_et);
    m_auto_shooter_ended          = (time <= m_auto_shooter_et);
    m_auto_shooter_allign_ended   = (time <= m_auto_shoot_allign_et);

    /* Calculate final results */
    m_auto_hands_enabled          = m_auto_hands_started   && !m_auto_hands_ended;
    m_auto_intake_enabled         = m_auto_intake_started  && !m_auto_intake_ended;
    m_auto_lifter_enabled         = m_auto_lifter_started  && !m_auto_lifter_ended;
    m_auto_drive_x_enabled        = m_auto_drive_x_started && !m_auto_drive_x_ended;
    m_auto_drive_y_enabled        = m_auto_drive_y_started && !m_auto_drive_y_ended;
    m_auto_shooter_enabled        = m_auto_shooter_started && !m_auto_shooter_ended;
    m_auto_shooter_allign_enabled = m_auto_shooter_allign_started && !m_auto_shooter_allign_ended;
}
