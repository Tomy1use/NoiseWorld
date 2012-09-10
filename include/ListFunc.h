#pragma once

template<class T>
void DeleteList(T* list)
{
    T* e = list;
    while(e){
        T* next = e->next;
        delete(e);
        e = next;
    }
}

template<class T>
int ListItemCount(const T* list)
{
    int count = 0;
    for(const T* e=list; e; e=e->next){
        count ++;
    }
    return count;
}

template<class T>
T* ListItemBefore(const T* item, T* list)
{
    if(list == item) return NULL;
    for(T* e=list; e; e=e->next){
        if(e->next == item){
            return e;
        }
    }
    return NULL;
}
