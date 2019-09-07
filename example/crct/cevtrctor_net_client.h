#ifndef _CEVTRCTOR_NET_CLIENT_H_
#define _CEVTRCTOR_NET_CLIENT_H_


#include <memory>
#include <thread>
#include <chrono>

class CEvtrctNetClient : public std::enable_shared_from_this<CEvtrctNetClient>
{ 
public:
        CEvtrctNetClient(const CEvtrctNetClient&) = delete;
        CEvtrctNetClient& operator=(const CEvtrctNetClient&) = delete;
        CEvtrctNetClient();
        ~CEvtrctNetClient();
};



#endif

