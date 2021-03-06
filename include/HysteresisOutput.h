/*
 Name:		HysteresisOutput.h
 Author:	Tim Long (TimTek)
*/

#pragma once
#include <Timer.h>
#include <functional>

/// <summary>
/// Class HysteresisOutput.
/// Accepts an input state and produces an output action which guarantees minimum stable on and off
/// times. That is, once the output changes state, that state is latched for a minimum period.
/// Example application: Given a rain detector that generates a pulse whenever a raindrop is
/// detected, we might wish to produce an output that is latched on for several minutes from the
/// last rain drop detection.
/// Note - the latch time runs from the moment the output state changes,
/// not from when the change was requested.
/// </summary>
class HysteresisOutput
    {
  public:
    /// <summary>
    /// Initializes a new instance of the <see cref="HysteresisOutput"/> class.
    /// </summary>
    /// <param name="MinimumTimeOn">The minimum time that the output can be on before turning
    /// off.</param> <param name="MinimumTimeOff">The minimum time that the output can be off before
    /// turning on.</param> <param name="stateChanger">An action method that will be called to
    /// change the output state.</param>
    HysteresisOutput(Duration MinimumTimeOn, Duration MinimumTimeOff,
                     std::function<void(bool)> stateChanger)
        : minimumTimeOn(MinimumTimeOn), minimumTimeOff(MinimumTimeOff),
          invokeUserStateChange(stateChanger)
        {
        timeSinceOff.stop();
        timeSinceOn.stop();
        }

    /// <summary>
    /// Turns the output on. May be delayed by hysteresis.
    /// Equivalent to <c>setInputState(true)</c>.
    /// </summary>
    void turnOn();

    /// <summary>
    /// Turns the output off. May be delayed by hysteresis.
    /// Equivalent to <c>setInputState(false)</c>.
    /// </summary>
    void turnOff();

    /// <summary>
    /// Sets the state of input to the boolean value.
    /// Note: the output state may not immediately change as it may be delayed by hysteresis.
    /// </summary>
    void setInputState(bool requestedState);

    /// <summary>
    /// Gets the current state of the output.
    /// </summary>
    bool getOutputState();

    // void forceOn();
    // void forceOff();
    /// <summary>
    /// Must be called periodically, typically from the Arduino main loop.
    /// Timing resolution is dictated by how frequently this method is called, but is 1 ms at
    /// best.
    /// </summary>
    void loop();

    /// <summary>
    /// Sets the latch times for the output.
    /// Can be used to reconfigure an active output.
    /// </summary>
    void setLatchTimes(Duration minimumTimeOn, Duration minimumTimeOff);

  private:
    void setOutputState();
    Duration getHysteresisTime();
    Duration minimumTimeOn;
    Duration minimumTimeOff;
    std::function<void(bool)> invokeUserStateChange;
    bool targetState;
    Timer hysteresis;
    Timer timeSinceOn;
    Timer timeSinceOff;
    bool currentState;
    };
