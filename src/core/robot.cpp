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
    /* Create subsystems */
    m_subsystemLifter     = make_unique<Lifter>();
    m_subsystemIntake     = make_unique<Intake>();
    m_subsystemShooter    = make_unique<Shooter>();
    m_subsystemPowertrain = make_unique<Powertrain>();

    /* Create joysticks */
    m_joystickIntake      = make_unique<Joystick> (OI::kLifterJoystick);
    m_joystickIntake      = make_unique<Joystick> (OI::kIntakeJoystick);
    m_joystickShooter     = make_unique<Joystick> (OI::kShooterJoystick);
    m_joystickPowertrainA = make_unique<Joystick> (OI::kPowertrainJoystickA);
    m_joystickPowertrainB = make_unique<Joystick> (OI::kPowertrainJoystickB);

    /* Autonomous actuator values */
    m_auto_intake         = 0;
    m_auto_lifter         = 0;
    m_auto_drive_x        = 0;
    m_auto_drive_y        = 0;
    m_auto_shooter        = 0;

    /* Autonomous actuator times */
    m_auto_lifter_time    = 0;
    m_auto_intake_time    = 0;
    m_auto_shooter_time   = 0;
    m_auto_drive_x_time   = 0;
    m_auto_drive_y_time   = 0;
}

//===============================================================================
// Robot::TeleopPeriodic
//===============================================================================

void Robot::AutonomousInit() {

}

//===============================================================================
// Robot::TeleopPeriodic
//===============================================================================

void Robot::TeleopPeriodic() {
    m_subsystemLifter->move      (*m_joystickLifter.get());
    m_subsystemIntake->move      (*m_joystickIntake.get());
    m_subsystemShooter->shoot    (*m_joystickShooter.get());
    m_subsystemPowertrain->drive (*m_joystickPowertrainA.get(),
                                  *m_joystickPowertrainB.get());
}

//===============================================================================
// Robot::AutonomousPeriodic
//===============================================================================

void Robot::AutonomousPeriodic() {
}

