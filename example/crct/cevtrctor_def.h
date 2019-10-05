#ifndef _CEVTRCTOR_DEF_H_
#define _CEVTRCTOR_DEF_H_


#include <iostream>
#include <string>
#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <set>
#include <map>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <typeinfo>
#include <algorithm>
#include <complex>
#include <limits>

#include <memory>
#include <thread>
#include <chrono>


typedef enum
{
    CONN_STATUS_INIT        = 0,
    CONN_STATUS_CONNECTING,
    CONN_STATUS_CONNECTED,
}CONN_STATUS_E;
    
class CEvtrctNetSlave;
class CEvtrctNetServer;
class CEvtrctNetProxy;
class CEvtrctNetConn;
    
typedef std::shared_ptr<CEvtrctNetConn>     cevt_net_conn_sptr;
typedef std::shared_ptr<CEvtrctNetSlave>    cevt_net_slave_sptr;
typedef std::shared_ptr<CEvtrctNetServer>   cevt_net_serv_sptr;  
typedef std::shared_ptr<CEvtrctNetProxy>    cevt_net_proxy_sptr;  

#endif
