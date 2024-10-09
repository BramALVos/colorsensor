#pragma once

#include <Arduino.h>
#include "utils.hpp"

struct ColorP {
    u8 r, g, b;

    ColorP() {}
    ColorP(u8 r, u8 g, u8 b) : r(r), g(g), b(b) {}

    void normalize() {
        u8 min = this->r;
        u8 max = this->r;
        for (u8 i = 1; i < sizeof(*this); ++i) {
            min = (&this->r)[i] < min ? (&this->r)[i] : min;
            max = (&this->r)[i] > max ? (&this->r)[i] : max;
        }

        for (u8 i = 0; i < sizeof(*this); ++i) {
            (&this->r)[i] = ((float)((&this->r)[i] - min) / (float)(max - min)) * 100.f;
        }

    }

    void print() {
        Serial.print("ColorP [");
        Serial.print("r = ");
        Serial.print(this->r);
        Serial.print("%, g = ");
        Serial.print(this->g);
        Serial.print("%, b = %");
        Serial.print(this->b);
        Serial.print("]");

    }
};

class ColorSensor {
    u8 s0, s1, s2, s3;
    u8 oe;
    u8 out;

public:
    ColorSensor(u8 out, u8 oe, u8 s0, u8 s1, u8 s2, u8 s3);

    enum FrequencyScale : u8{
        POWER_DOWN = 0b00,
        P2 = 0b01,
        P20 = 0b10,
        P100 = 0b11,
    };

    ColorP measure(FrequencyScale scale = FrequencyScale::P20);
};
