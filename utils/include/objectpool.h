#include "types.h"
#include "definitions.h"

template <class T> class ObjectPool;

template <class T>
class Poolable
{
protected:
    Poolable() : _next(nullptr)
    {
    }

private:
    friend class ObjectPool<T>;

    T* GetNext()
    {
        return _next;
    }

    void SetNext(T* next)
    {
        _next = next;
    };

    T* _next;
};


template <class T>
class ObjectPool
{
public:
    ObjectPool(unsigned size)
        : _objects(size) 
        , _firstAvailable(_objects.data())
    {
        static_assert(std::is_base_of<Poolable<T>, T>::value, "Object in pool must implement Poolable<T>");
        for(unsigned i = 0; i < (size - 1); ++i)
            _objects[i].Poolable<T>::SetNext(&_objects[i+1]);
    }

    T* GetObject()
    {
        ASSERT(_firstAvailable != nullptr);
        T* tmp = _firstAvailable;
        _firstAvailable = tmp->Poolable<T>::GetNext();
        //std::cout << "Popping object 0x" << std::hex << GetAddress(tmp) << '\n';
        return tmp;
    }

    template <class... Args>
    T* ConstructObject(Args&&... args)
    {
        ASSERT(_firstAvailable != nullptr);
        T* tmp = _firstAvailable;
        _firstAvailable = tmp->Poolable<T>::GetNext();
        //std::cout << "Popping object 0x" << std::hex << GetAddress(tmp) << " constructed with " << sizeof...(Args) << " arguments\n";
        return new (tmp) T(std::forward<Args>(args)...);
    }

    void FreeObject(T* object)
    {
        ASSERT(object != nullptr);
        ASSERT(ObjectOwnedByPool(object));
        object->Poolable<T>::SetNext(_firstAvailable);
        //std::cout << "Pushing object 0x" << std::hex << GetAddress(object) << '\n';
        _firstAvailable = object;
    }

    bool ObjectOwnedByPool(T* object)
    {
        return GetAddress(object) >= GetAddress(&_objects.front())
            && GetAddress(object) <= GetAddress(&_objects.back());
    }

private:
    static uintptr_t GetAddress(T* object)
    {
        return reinterpret_cast<uintptr_t>(object);
    }

    std::vector<T> _objects;
    T* _firstAvailable; 
};
