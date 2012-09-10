#pragma once

static const float HumanHipSide = .15f;
static const float HumanShoulderDown = .05f;
static const float HumanShoulderSide = .2f;
static const float HumanHeadSize = .5f;
static const float HumanTorsoHeight = .7f;
static const float HumanLegLength = .8f;
static const float HumanArmLength = .8f;

struct Human
{
    Vector pelvis, neck, hip[2], knee[2], foot[2], sho[2], elb[2], hand[2], head;
    Vector ik(const Vector& A, const Vector& B, float Length, const Vector& Pole)
    {
        Vector v = B - A;
        float actualLength = length(v);
        Vector k = (A + B) / 2;
        if(square(actualLength) > square(Length)){
            return k;
        }
        Vector u = unit(cross(cross(v, unit(Pole-k)), v));
        return k + u * sqrtf(square(Length/2) - square(k-A));
    }
};
