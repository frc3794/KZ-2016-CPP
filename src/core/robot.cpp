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
    m_subystemShooter     = make_unique<Shooter>();
    m_subystemIntake   = make_unique<Intake>();
    m_subsystemPowertrain = make_unique<Powertrain>();
    m_joystickShooter     = make_unique<Joystick> (OI::kShooterJoystick);
    m_joystickIntake      = make_unique<Joystick> (OI::kIntakeJoystick);
    m_joystickPowertrainA = make_unique<Joystick> (OI::kPowertrainJoystickA);
    m_joystickPowertrainB = make_unique<Joystick> (OI::kPowertrainJoystickB);
}

//===============================================================================
// Robot::Autonomous
//===============================================================================

void Robot::Autonomous() {
    while (IsAutonomous())
        Wait (LOOP_TIME);
}

//===============================================================================
// Robot::OperatorControl
//===============================================================================

void Robot::OperatorControl() {
    while (IsOperatorControl()) {
        m_subystemIntake->move       (*m_joystickIntake.get());
        m_subystemShooter->shoot     (*m_joystickShooter.get());
        m_subsystemPowertrain->drive (*m_joystickPowertrainA.get(),
                                      *m_joystickPowertrainB.get());

        Wait (LOOP_TIME);
    }
}
