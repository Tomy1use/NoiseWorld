#pragma once

#include <Math/Vector.h>
#include <Human.h>

struct HumanWalkClip
{
    HumanWalkClip(Human& h, float t, bool handsInPockets)
    {
        Vector& pelvis = h.pelvis, &neck = h.neck, &head = h.head;
        Vector* hip = h.hip, *knee = h.knee, *foot = h.foot;
        Vector* sho = h.sho, *elb = h.elb, *hand = h.hand;
        const float Stride = .9f;
        const float StrideTime = 1.f;
        const float FootRise = .2f;
        int strideIndex = int(t / StrideTime);
        int airFoot = strideIndex % 2;
        for(int i=0; i<2; i++){
            float side = i ? +1.f : -1.f;
            int footStrideCount = i ? int((t-StrideTime)/StrideTime/2) :  int(t/StrideTime/2);
            float footStrideFrac = i 
                ? (airFoot ? 0 : frac(t / StrideTime))
                : (airFoot ? frac(t / StrideTime) : 0); 
            float footStrideAmp = 1 - 2 * abs(footStrideFrac - .5f);
            foot[i].z = (i ? +Stride/2 : -Stride/2) + 2*Stride*footStrideCount + 2 * footStrideFrac * Stride;
            foot[i].x = side * .1f;
            foot[i].y = footStrideAmp * FootRise;
            hip[i] = foot[i] + UnitVectorY * HumanLegLength;
        }
        pelvis = (hip[0] + hip[1]) / 2;
        pelvis.x = -sinf(2*Pi*t/StrideTime/2)*.1f;
        pelvis.y = .75f - cosf(2*Pi*t/StrideTime) * .05f;
        pelvis.z += .1f;
        for(int i=0; i<2; i++){
            float side = i ? +1.f : -1.f;
            hip[i].x = pelvis.x + side * HumanHipSide;
            hip[i].z = pelvis.z;
            hip[i].y = pelvis.y;
            knee[i] = (hip[i] + foot[i]) / 2;
            knee[i] = h.ik(hip[i], foot[i], HumanLegLength, knee[i] + UnitVectorZ*6);
        }
        Vector torsoUp = UnitVectorY + UnitVectorZ * (.1f + cosf(2*Pi*t/StrideTime) * .05f);
        torsoUp.x = sinf(Pi*t/StrideTime) * .01f;
        neck = pelvis + unit(torsoUp) * HumanTorsoHeight;
        for(int i=0; i<2; i++){
            float side = i ? +1.f : -1.f;
            sho[i] = neck + UnitVectorX * side * HumanShoulderSide;
            sho[i].y -= HumanShoulderDown;
            hand[i] = hip[i] + UnitVectorX * side * .05f + UnitVectorY * .05f;
            if(handsInPockets){
                //do nothing
            }else{//hands waving
                hand[i].z += sinf(Pi/2*side + Pi*t/StrideTime) * .3f + .1f;
                hand[i].y += sinf(Pi/2*side + Pi*t/StrideTime) * .05f - .05f;
                hand[i].x += (-sinf(Pi/2*side + Pi*t/StrideTime) * .1f + .1f) * side;
            }
            elb[i] = h.ik(sho[i], hand[i], HumanArmLength, pelvis + UnitVectorX*side - UnitVectorZ);
        }
        head = neck + UnitVectorY * HumanHeadSize;
    }
    float frac(float f) {return f - floorf(f);}
};

