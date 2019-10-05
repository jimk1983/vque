#ifndef _CEVTRCTOR_NET_IOBUF_H_
#define _CEVTRCTOR_NET_IOBUF_H_

#include <crct/cevtrctor_def.h>
#include <crct/cevtrctor_cfg.h>
#include <crct/cevtrctor_net_iobuf.h>
#include <memory>
#include <thread>
#include <chrono>
#include <list>
#include <iostream>

class CVosIobuf : public std::enable_shared_from_this<CVosIobuf>
{
public:
    int32_t         m_size;
    VOS_IOBUF_S*    m_pstIobuf;
public:
    CVosIobuf(int32_t buf_size);
    ~CVosIobuf();
};

typedef std::shared_ptr<CVosIobuf>          vos_iobuf_sptr;


#endif

