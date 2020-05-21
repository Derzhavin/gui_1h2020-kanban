#ifndef SINGLETON_H
#define SINGLETON_H

#include <QtGlobal>

template <class T>
class Singleton {
public:
    Singleton(T& rObject) {

        Q_ASSERT_X(!s_pInstance, "constructor", "Only one instance of this class is permitted.");
        s_pInstance = &rObject;
    }
    ~Singleton() {
        Q_ASSERT_X(s_pInstance, "destructor", "The singleton instance is invalid.");
        s_pInstance = 0;
    }
    static T& instance() {
        if(!s_pInstance) {
                   s_pInstance = new T();
        }
        Q_ASSERT_X(s_pInstance, "instancing", "The singleton has not yet been created.");
        return (*s_pInstance);
    }
private:
    static T* s_pInstance;
    Singleton(const Singleton& Src);
    Singleton& operator=(const Singleton& Src);
};
template <class T> T* Singleton<T>::s_pInstance = 0;

#endif // SINGLETON_H
