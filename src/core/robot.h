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
    unique_ptr<Joystick> m_driveJoystick;
    unique_ptr<Joystick> m_secndJoystick;

    unique_ptr<Lifter> m_subsystemLifter;
    unique_ptr<Intake> m_subsystemIntake;
    unique_ptr<Shooter> m_subsystemShooter;
    unique_ptr<Powertrain> m_subsystemPowertrain;

    float m_auto_intake;
    float m_auto_lifter;
    float m_auto_drive_x;
    float m_auto_drive_y;
    float m_auto_shooter;

    float m_auto_lifter_time;
    float m_auto_intake_time;
    float m_auto_shooter_time;
    float m_auto_drive_x_time;
    float m_auto_drive_y_time;
};


