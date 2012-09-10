#pragma once

struct ZoomDragMatrix
{
    Vector dragPoint;
    ZoomDragMatrix(): dragPoint(ZeroVector) {}
    Matrix update(const HWND window, float fov, const Plane& snapPlane, const Vector& pivot,
        const Matrix& camera, const CursorInput& cursor, float dt)
    {
        RECT client; 
        GetClientRect(window, &client);
        float aspect = float(client.right)/float(client.bottom);
        float x = (float(cursor.x) / float(client.right) * 2 - 1) * tan(fov);
        float y = -(float(cursor.y) / float(client.bottom) * 2 - 1) * tan(fov) / aspect;
        Vector hit, ray = -camera.axes.z + camera.axes.x * x + camera.axes.y * y;
        if(! planeHitRay(snapPlane, camera.origin, ray, hit)) return camera;
        Matrix result = camera;
        if(cursor.wheelDelta){
            float f = cursor.wheelDelta < 0 ? 1.2f : 1/1.2f;
            result.origin = (camera.origin - hit) * f + hit;
        }
        static Vector dragPoint(ZeroVector);
        if(cursor.rightButtonPress || cursor.leftButtonPress){
            dragPoint = hit;
        }else{
            if(cursor.rightButtonDown){
                result.origin += dragPoint - hit;
            }
            if(cursor.leftButtonDown){
                Matrix rotor(result.axes, pivot);
                result = rotor.in(result);
                rotor.axes = rotateY(rotor.axes.out(rotateX(UnitAxes, -cursor.deltaY*.01f)), cursor.deltaX*.01f);
                result = rotor.out(result);
            }
            result.axes.x.y = 0;
            result.axes.z = unit(cross(result.axes.x, result.axes.y));
            result.axes.y = unit(cross(result.axes.z, result.axes.x));
            result.axes.x = unit(cross(result.axes.y, result.axes.z));
        }
        return result;
    }
};

