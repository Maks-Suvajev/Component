#ifndef COMPONENT_MANAGER
#define COMPONENT_MANAGER

// STL
#include <iostream>
#include <vector>
#include <optional>

// Parent class
#include "IComponentPool.h"

#include "Entity.h"

constexpr uint32_t initialSparseCapacity = 10000U;

template<typename T>
class ComponentManager : public IComponentPool
{
    public:
        ComponentManager();
        bool                  addComponent(Entity entity, T&& newData);
        bool                  hasEntity(Entity entity) const override;
        void                  destroyEntity(Entity entity) override;
        T*                    getComponentData(Entity entity);
        Entity                getEntityID(size_t position);
        auto                  begin();
        auto                  end();

    private:
        void                  initSparseArray(size_t size);
        void                  resizeSparse(Entity entity);


        std::vector<T> dense;
        std::vector<Entity> denseMap; // Maps denseMap structure denseMap[0] tells you what entity is at pos 0 in dense
        std::vector<Entity> sparse; // Maps entityID to position in dense i.e. sparse[2] tells you where entity 2 component is

};

template<typename T>
Entity ComponentManager<T>::getEntityID(size_t position)
{
    return denseMap[position];
}

template<typename T>
auto ComponentManager<T>::begin()
{
    return dense.begin();
}

template<typename T>
auto ComponentManager<T>::end()
{
    return dense.end();
}

template<typename T>
bool ComponentManager<T>::hasEntity(Entity entity) const
{
    if (sparse[entity] == maxEntityValue)
    {
        return false;
    } 

    return true;
}

template<typename T>
void ComponentManager<T>::destroyEntity(Entity entity)
{
    uint32_t indexToRemove = sparse[entity]; // index in dense to remove
    uint32_t entityAtBack = denseMap.back(); // get entity ID that's at the back
    
    // Swap positions so deleted entity is replaced by back one
    dense[indexToRemove] = dense.back(); 
    denseMap[indexToRemove] = entityAtBack; 
    sparse[entityAtBack] = indexToRemove;
    
    // Remove superfluous entity
    dense.pop_back();
    denseMap.pop_back();

    // Mark entity as deleted
    sparse[entity] = maxEntityValue;
}

template<typename T>
void ComponentManager<T>::initSparseArray(size_t size)
{
    sparse.assign(size, maxEntityValue);
}

template<typename T>
ComponentManager<T>::ComponentManager()
{
    initSparseArray(initialSparseCapacity);
}

template<typename T>
void ComponentManager<T>::resizeSparse(Entity entity)
{
    // Double the size of the sparse vector unless entityID is higher
    size_t newSize = std::max(static_cast<size_t>(entity + 1), static_cast<size_t>(sparse.size()) * 2);

    sparse.resize(newSize, maxEntityValue);
}


template<typename T>
bool ComponentManager<T>::addComponent(Entity entity, T&& newData)
{
    if (entity >= sparse.size())
    {
        resizeSparse(entity);
    }

    if (sparse[entity] == maxEntityValue)
    {
        dense.push_back(std::move(newData));

        denseMap.push_back(entity);

        sparse[entity] = static_cast<Entity>(dense.size() - 1);

        return true;
    }

    #ifdef ENABLE_DEBUG_MESSAGES
        std::cout << "ERROR::Entity already exists - doing nothing." << std::endl;
    #endif

    return false;
}

template<typename T>
T* ComponentManager<T>::getComponentData(Entity entity)
{
    if (sparse[entity] == maxEntityValue)
    {
        #ifdef ENABLE_DEBUG_MESSAGES
            std::cout << "ERROR::Entity doesn't have this component." << std::endl;
        #endif

        return nullptr;    
    }

    return &dense[sparse[entity]];
}

#endif