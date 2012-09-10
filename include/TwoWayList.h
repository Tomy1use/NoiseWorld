#pragma once


template<class Object>
struct TwoWayList
{
    struct Item{
        public:
            Object object;
            Item *prev, *next;
        public:
            Item(): object(NULL), prev(NULL), next(NULL) {}
            Item(Object o): object(o), prev(NULL), next(NULL) {}
    };
    
    Item *first, *last;
    
    TwoWayList(): first(NULL), last(NULL) {}
    
    Item* addFirst(Object o)
    {
        Item* item = new Item(o);
        if(first != NULL){
            first->prev = item;
            item->next = first;
        }
        first = item;
        if(last == NULL){
            last = item;
        }
        return item;
    }
    
    Item* addLast(Object o)
    {
        Item* item = new Item(o);
        if(last != NULL){
            last->next = item;
            item->prev = last;
        }
        last = item;
        if(first == NULL){
            first = item;
        }
        return item;
    }
    
    bool hasItems(){
        return (first != NULL);
    }
    
    bool hasOneItem(){
        return (hasItems() && (first == last));
    }
    
    Item *remove(Item *item)
    {
        Item *next = item->next;
        if(item->prev != NULL){
            item->prev->next = item->next;
        }else{
            first = item->next;
        }
        if(item->next != NULL){
            item->next->prev = item->prev;
        }else{
            last = item->prev;
        }
        delete item;
        return next;
    }
    
    Item* find(Object o)
    {
        for(Item* i=first; i!=NULL; i=i->next){
            if(i->object == o){
                return i;
            }
        }
        return NULL;
    }
    
    Item* destroy(Item *item){
        delete item->object;
        return remove(item);
    }
    
    void destroyItems()
    {
        Item *i = first;
        while(i != NULL){
            Item *next = i->next;
            delete i->object;
            delete i;
            i = next;
        }
        first = NULL;
        last = NULL;
    }

    void clear()
    {
        Item *i = first;
        while(i != NULL){
            Item *next = i->next;
            delete i;
            i = next;
        }
        first = NULL;
        last = NULL;
    }
    int count()
    {
        int k = 0;
        for(Item* i=first; i!=NULL; i=i->next){
            k ++;
        }
        return k;
    }
    Item* itemByIndex(int k)
    {
        for(Item* i=first; i!=NULL; i=i->next){
            if(k <= 0){
                return i;
            }
            k --;
        }
        return NULL;
    }
};


