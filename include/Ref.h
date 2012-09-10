#pragma once

struct Ref
{
    int _ref;
    explicit Ref(): _ref(0) {}
    virtual ~Ref() {};
    void incRef()
    {
        _ref ++;
    }
    void decRef()
    {
        _ref --;
        if(_ref <= 0){
            delete this;
        }
    }
    int ref() {
        return _ref;
    }
};
