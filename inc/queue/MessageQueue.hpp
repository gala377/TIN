#ifndef TIN_MESSAGEQUEUE_H
#define TIN_MESSAGEQUEUE_H

#include <queue/Storage.h>
#include <queue/FileStorage.h>

namespace Queue {

    template<typename T>
    struct storage_trait {
        static const bool value = false;  
    };
    template<>
    struct storage_trait<Storage> {
        static const bool value = true;
    };
    template<>
    struct storage_trait<FileStorage> {
        static const bool value = true;
    };
    template<class T>
    constexpr bool is_storage = storage_trait<T>::value;

    template<class T, bool storage = is_storage<T>>
    class MessageQueue {};

    template<class T>
    class MessageQueue<T, true> {
    public:
        MessageQueue();

        void push(Message mess);
        void pushWithoutSave(Message mess);

        Message &pop();

        void remove(std::uint32_t id);

        bool empty();
        bool pending();

        std::uint32_t size();
        std::uint32_t pending_size();
    };

    template<>
    class MessageQueue<FileStorage, is_storage<FileStorage>> {
    public:
        MessageQueue();

        void push(Message mess);
        void pushWithoutSave(Message mess);
        
        Message& pop();

        void remove(std::uint32_t id);

        bool empty();
        bool pending();

        std::uint32_t size();
        std::uint32_t pending_size();
    protected:
        FileStorage _storage;

        std::uint32_t _size;
        std::uint32_t _pending_size; 
    };
}
#endif