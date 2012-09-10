#pragma once


struct RayThroughCursor
{
    Vector ray;
    RayThroughCursor(HWND window, const CursorInput& cursor, const Matrix& camera, float fov)
    {
        RECT client;
        GetClientRect(window, &client);
        float aspect = float(client.right)/float(client.bottom);
        float x = (float(cursor.x) / float(client.right) * 2 - 1) * tan(fov);
        float y = -(float(cursor.y) / float(client.bottom) * 2 - 1) * tan(fov) / aspect;
        ray = -camera.axes.z + camera.axes.x * x + camera.axes.y * y;
    }
    operator const Vector& () {return ray;}
};
