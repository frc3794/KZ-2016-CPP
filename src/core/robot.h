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

#pragma once

#include "common.h"
#include "subsystems/hands.h"
#include "subsystems/lifter.h"
#include "subsystems/intake.h"
#include "subsystems/shooter.h"
#include "subsystems/powertrain.h"

class Robot : public IterativeRobot {
  public:
    void RobotInit();
    void AutonomousInit();
    void TeleopPeriodic();
    void AutonomousPeriodic();

  private:
    void resetAutoValues();
    void getDashboardValues();
    void putDashboardValues();

  private:
    unique_ptr<Joystick> m_driveJoystick;
    unique_ptr<Joystick> m_secndJoystick;

    unique_ptr<Hands> m_subsystemHands;
    unique_ptr<Lifter> m_subsystemLifter;
    unique_ptr<Intake> m_subsystemIntake;
    unique_ptr<Shooter> m_subsystemShooter;
    unique_ptr<Powertrain> m_subsystemPowertrain;

    unique_ptr<SendableChooser> m_auto_chooser;

    float m_auto_hands;
    float m_auto_intake;
    float m_auto_lifter;
    float m_auto_drive_x;
    float m_auto_drive_y;
    float m_auto_shooter;
    float m_auto_shoot_allign;

    float m_auto_hands_et;
    float m_auto_lifter_et;
    float m_auto_intake_et;
    float m_auto_drivex_et;
    float m_auto_drivey_et;
    float m_auto_shooter_et;
    float m_auto_shoot_allign_et;

    float m_auto_hands_st;
    float m_auto_lifter_st;
    float m_auto_intake_st;
    float m_auto_drivex_st;
    float m_auto_drivey_st;
    float m_auto_shooter_st;
    float m_auto_shoot_allign_st;
};


