#pragma once

template<class TrigonAdaptor>
void octaher(TrigonAdaptor& trigon, const Matrix& M, float L, float R, float B, float T, float K, float F)
{
    Vector l = M.origin + M.axes.x * L;
    Vector r = M.origin + M.axes.x * R;
    Vector b = M.origin + M.axes.y * B;
    Vector t = M.origin + M.axes.y * T;
    Vector k = M.origin + M.axes.z * K;
    Vector f = M.origin + M.axes.z * F;
    trigon(t, k, l);
    trigon(t, r, k);
    trigon(t, f, r);
    trigon(t, l, f);
    trigon(l, k, b);
    trigon(k, r, b);
    trigon(r, f, b);
    trigon(f, l, b);
}

template<class TrigonAdaptor>
void spike(TrigonAdaptor& trigon, const Matrix& M, float L, float R, float T, float K, float F)
{
    Vector l = M.origin + M.axes.x * L;
    Vector r = M.origin + M.axes.x * R;
    Vector t = M.origin + M.axes.y * T;
    Vector k = M.origin + M.axes.z * K;
    Vector f = M.origin + M.axes.z * F;
    trigon(t, k, l);
    trigon(t, r, k);
    trigon(t, f, r);
    trigon(t, l, f);
}
