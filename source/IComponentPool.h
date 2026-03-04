#ifndef ICOMPONENT_POOL
#define ICOMPONENT_POOL

#include "Entity.h"

class IComponentPool
{
    public:
        virtual  ~IComponentPool() = default;

        virtual bool hasEntity(Entity entity) const = 0;
        virtual void destroyEntity(Entity entity) = 0;
};


#endif