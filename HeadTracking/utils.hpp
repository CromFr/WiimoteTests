#ifndef UTILS_HPP_INCLUDED
#define UTILS_HPP_INCLUDED

class Coord3d
{
    public:

    Coord3d(float x=0, float y=0, float z=0)
    {
        Coord3d::x=x;
        Coord3d::y=y;
        Coord3d::z=z;
    }

    void set(float x, float y, float z)
    {
        Coord3d::x=x;
        Coord3d::y=y;
        Coord3d::z=z;
    }


    float x;
    float y;
    float z;
};

#endif // UTILS_HPP_INCLUDED
