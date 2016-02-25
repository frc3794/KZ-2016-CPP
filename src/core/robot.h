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
#include "subsystems/shooter.h"
#include "subsystems/intake.h"
#include "subsystems/powertrain.h"

class Robot : public SampleRobot {
  public:
    void RobotInit();
    void Autonomous();
    void OperatorControl();

  private:
    unique_ptr<Joystick> m_joystickShooter;
    unique_ptr<Joystick> m_joystickIntake;
    unique_ptr<Joystick> m_joystickPowertrainA;
    unique_ptr<Joystick> m_joystickPowertrainB;

    unique_ptr<Shooter> m_subystemShooter;
    unique_ptr<Intake> m_subystemIntake;
    unique_ptr<Powertrain> m_subsystemPowertrain;
};


