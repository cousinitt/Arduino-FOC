#include "lowpass_filter.h"

ExponentialMovingAverage::ExponentialMovingAverage(float time_constant)
    : Tf(time_constant)
    , y_prev(0.0f)
{
    timestamp_prev = _micros();
}


float
ExponentialMovingAverage::operator() (float x)
{
    unsigned long timestamp = _micros();
    float dt = (timestamp - timestamp_prev)*1e-6f;

    if (dt < 0.0f || dt > 0.5f)
        dt = 1e-3f;

    float alpha = Tf/(Tf + dt);
    float y = alpha*y_prev + (1.0f - alpha)*x;

    y_prev = y;
    timestamp_prev = timestamp;
    return y;
}