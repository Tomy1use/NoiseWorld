#pragma once

Vector kneePos(const Vector& Hip, const Vector& Foot, float ThighLength, float ShinLength)
{
    Vector v = Foot - Hip;
    float MaxPossibleLength = ThighLength + ShinLength;
    float actualLength = length(v);
    Vector k = lerp(Hip, Foot, ThighLength / MaxPossibleLength);
    if(square(actualLength) > square(MaxPossibleLength)){
        return k;
    }
    Vector u = unit(cross(cross(v, UnitVectorY), v));
    return k + u * sqrtf(square(ThighLength) - square(k-Hip));
}

void glHuman(const Human& h, const Matrix& m, Color ShirtColor, Color PantsColor)
{
    const Color SkinColor(0xffFFffcc);
    glColor(ShirtColor);
    glCylinder(m.out(h.pelvis), m.out(h.neck), .2f, .2f, 12);
    for(int i=0; i<2; i++){
        glColor(ShirtColor);
        glCylinder(m.out(h.sho[i]), m.out(h.elb[i]), .08f, .06f, 12);
        glColor(PantsColor);
        glCylinder(m.out(h.hip[i]), m.out(h.knee[i]), .1f, .08f, 12);
        glCylinder(m.out(h.knee[i]), m.out(h.foot[i]), .08f, .05f, 12);
        glColor(SkinColor);
        glCylinder(m.out(h.elb[i]), m.out(h.hand[i]), .06f, .04f, 12);
    }
    glColor(SkinColor);
    //glCylinder(matrixFromOriginAxisY(m.out(h.neck), h.head-h.neck), 0.f, .3f, .08f, .08f, 12, 0);
    glCylinder(matrixFromOriginAxisY(m.out(h.neck), h.head-h.neck), 0.1f, .65f, .13f, .13f, 12);
}
