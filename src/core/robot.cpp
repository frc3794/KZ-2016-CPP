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
// Robot::RobotInit
//===============================================================================

void Robot::RobotInit() {
    m_timer               = new Timer();
    m_subsystemHands      = new Hands();
    m_subsystemLifter     = new Lifter();
    m_subsystemIntake     = new Intake();
    m_subsystemShooter    = new Shooter();
    m_subsystemPowertrain = new Powertrain();
    m_driveJoystick       = new Joystick (0);
    m_secndJoystick       = new Joystick (1);

    CameraServer::GetInstance()->StartAutomaticCapture ("cam0");
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
    m_timer->Reset();
    m_timer->Start();
}

//===============================================================================
// Robot::TeleopPeriodic
//===============================================================================

void Robot::TeleopPeriodic() {
    m_subsystemHands->move       (*m_secndJoystick);
    m_subsystemLifter->move      (*m_secndJoystick);
    m_subsystemIntake->move      (*m_secndJoystick);
    m_subsystemShooter->shoot    (*m_secndJoystick);
    m_subsystemPowertrain->drive (m_driveJoystick, m_secndJoystick);
}

//===============================================================================
// Robot::AutonomousPeriodic
//===============================================================================

void Robot::AutonomousPeriodic() {
    if (m_timer->Get() < 7)
        m_subsystemPowertrain->drive (0, 0.75, 0, true);
}