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

#include "intake.h"

//===============================================================================
// Intake::Intake
//===============================================================================

Intake::Intake() {
    m_motor = new WinT_Motor (Motors::kIntakeMotor);
}

//===============================================================================
// Intake::move
//===============================================================================

void Intake::move (float intake) {
    m_motor->Set (ADJUST_INPUT (intake, 0));
}

//===============================================================================
// Intake::move
//===============================================================================

void Intake::move (const Joystick& joystick) {
    float left = joystick.GetRawAxis (OI::kIntakeTake);
    float right = joystick.GetRawAxis (OI::kIntakeGive);

    if (left > right)
        move (left);

    else if (right > left)
        move (right * -1);

    else
        move (0);
}

//===============================================================================
// Intake::setSafetyEnabled
//===============================================================================

void Intake::setSafetyEnabled (bool enabled) {
    m_motor->SetSafetyEnabled (enabled);
}
