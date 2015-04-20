#ifndef _NOTIFICATION_DISPATCHER_H_
#define _NOTIFICATION_DISPATCHER_H_

#include "AdsNotification.h"
#include "AmsHeader.h"
#include "Semaphore.h"

#include <map>
#include <thread>

struct AmsProxy {
    virtual long DeleteNotification(const AmsAddr& amsAddr, uint32_t hNotify, uint32_t tmms, uint16_t port) = 0;
};

struct NotificationDispatcher {
    NotificationDispatcher(AmsProxy& __proxy, VirtualConnection __conn);
    ~NotificationDispatcher();
    bool operator<(const NotificationDispatcher& ref) const;
    void Emplace(uint32_t hNotify, Notification& notification);
    bool Erase(uint32_t hNotify, uint32_t tmms);
    inline void Notify() { sem.Post(); }
    void Run();

    const VirtualConnection conn;
    RingBuffer ring;
private:
    std::map<uint32_t, Notification> notifications;
    std::recursive_mutex mutex;
    AmsProxy& proxy;
    Semaphore sem;
    std::thread thread;
};

#endif /* #ifndef _NOTIFICATION_DISPATCHER_H_ */
