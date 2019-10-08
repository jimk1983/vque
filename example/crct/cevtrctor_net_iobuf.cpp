
#include <vos/vos_pub.h>
#include <vrct/vrct_api.h>

#include "cevtrctor_def.h"
#include "cevtrctor_net_iobuf.h"


CVosIobuf::CVosIobuf(int32_t buf_size)
        :m_size(buf_size)
{
    if (buf_size > VOS_IOBUF_MAXSIZE)
    {
        m_pstIobuf = VOS_IOBuf_mallocMax(0);
    }
    else
    {
        m_pstIobuf = VOS_IOBuf_malloc(0);
    }
    //printf("CVosIobuf entry, new this=%p\n", this);
}

CVosIobuf::~CVosIobuf()
{
    //printf("~CVosIobuf entry, delete this=%p\n", this);
    if (NULL != m_pstIobuf )
    {
        VOS_IOBuf_free(m_pstIobuf);
        m_pstIobuf = NULL;
    }
}


