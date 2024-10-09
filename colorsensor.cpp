#include "colorsensor.hpp"

ColorSensor::ColorSensor(u8 out, u8 oe, u8 s0, u8 s1, u8 s2, u8 s3) : out(out), oe(oe), s0(s0), s1(s1), s2(s2), s3(s3) {
    pinMode(this->s0, OUTPUT);
    pinMode(this->s1, OUTPUT);
    pinMode(this->s2, OUTPUT);
    pinMode(this->s3, OUTPUT);
    pinMode(this->oe, OUTPUT);

    pinMode(this->out, INPUT);
}

ColorP ColorSensor::measure(ColorSensor::FrequencyScale scale) {
    // step 1 select f_scale
    // step 2 select color
    // step 3 read pulse output sensor
    // repeat
    
    digitalWrite(this->s0, scale & 1);
    digitalWrite(this->s1, (scale & 2) >> 1);

    unsigned long col_buf[4];

    digitalWrite(oe, 0);

    for (u8 c = 0; c < 4; ++c) {
        digitalWrite(this->s2, c & 1);
        digitalWrite(this->s3, (c & 2) >> 1);

        delayMicroseconds(1); // give sensor time to update

        col_buf[c] = pulseIn(out, 0); // time in micro seconds

        if (!col_buf[c]) {
            // handle error, no signal detected!
            // possible hardware error -> abort
            abort();
        }
    }

    unsigned long abs_freq = col_buf[2];

    col_buf[2] = col_buf[1];

    col_buf[1] = col_buf[3];

    static_assert(sizeof(ColorP) == 3, "use the packed attribute when using ColorP");

    ColorP color;

    for (u8 c = 0; c < 3; ++c) { 
        reinterpret_cast<u8*>(&color)[c] = static_cast<u8>(((float)abs_freq / (float)col_buf[c]) * 100);
    }

    //color.normalize();
    
    return color;
}
