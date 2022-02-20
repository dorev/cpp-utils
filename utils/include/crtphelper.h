#pragma once 

// CRTP class helper
// Example:
// 
// template <class T>
// class Singleton : CrtpHelper<T, Singleton>
// {
// };

template <class CrtpChildClass, template<class> class CrtpClass>
class CrtpHelper
{
protected:
    // Convenience function to retrieve a reference to the child class
    CrtpChildClass& GetCrtpChild()
    {
        return static_cast<CrtpChildClass&>(*this);
    }

    const CrtpChildClass& GetCrtpChildConst()
    {
        return static_cast<const CrtpChildClass&>(*this);
    }

private:
    // Private constructor resolves the ambiguity if more than once class implement the same CrtpClass
    // Solves the diamond problem if a class inherits from multiple CrtpClass
    friend CrtpClass<CrtpChildClass>;
    CrtpHelper()
    {
    }
};
