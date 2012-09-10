#pragma once

template<typename A>
struct ListOfSelf
{
    ListOfSelf(): prev(NULL), next(NULL)
    {
        if(last){ 
            last->next = static_cast<A*>(this); 
            prev = last; 
        } 
        last = static_cast<A*>(this); 
        if(! first){ 
            first = static_cast<A*>(this); 
        }
    }
    ~ListOfSelf()
    {
        if(prev){ 
            prev->next = next; 
        }else{ 
            first = next; 
        } 
        if(next) { 
            next->prev = prev; 
        }else{ 
            last = prev; 
        }
    }
    A *prev, *next;
    static A* first, *last;
};
