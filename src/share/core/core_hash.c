/******************************************************************************

                  版权所有 (C), 2018-2028, 杭州友机技术有限公司

 ******************************************************************************
  文 件 名   : core_hash.c
  版 本 号   : 初稿
  作    者   : 蒋康
  生成日期   : 2019年3月21日
  最近修改   :
  功能描述   : 适合友机的最简单的Hash相关实现
  函数列表   :
  修改历史   :
  1.日    期   : 2019年3月21日
    作    者   : 蒋康
    修改内容   : 创建文件

******************************************************************************/

#include "share.h"


#define core_hash(key, c)   ((uint32_t) key * 31 + c)

/*素数空间数组*/
static uint32_t g_ulCorePrimeArry[]={
    53,
    97,
    193,   
    389,  //3
    769,  //4
    1543,  
    3079,
    6151,
    12289,
    24593,
    49157,
    98317,
    196613,
    393241,
    786433,
    1572869,
    3145739,
    6291469,
    12582917,
    25165843,
    50331653,
    100663319,
    201326611,
    402653189,
    805306457,
    1610612741,
};

/*素数的数组大小*/
#define CORE_TBLPRIME_SIZE    (sizeof(g_ulCorePrimeArry) / sizeof(uint32_t))


/*****************************************************************************
 函 数 名  : core_hashtbl_prime
 功能描述  : 计算素数
 输入参数  : uint32_t ulSize  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2019年3月21日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
uint32_t core_hashtbl_prime(IN uint32_t ulSize)
{
    uint32_t ulCount = CORE_TBLPRIME_SIZE;
    uint32_t ulIndex = 0;
    uint32_t ulPrime = g_ulCorePrimeArry[0];
    double dmin = fabs(log((double)ulSize) - log((double)g_ulCorePrimeArry[0]));
    double temp;
    
    for(ulIndex = 0; ulIndex < ulCount; ulIndex++)
    {
        temp = fabs(log((double)ulSize) - log((double)g_ulCorePrimeArry[ulIndex]));
        if ( temp > dmin )
        {
            continue;
        }
        dmin = temp;
        
        ulPrime = g_ulCorePrimeArry[ulIndex];
    }
    
    return ulPrime;
}

/*****************************************************************************
 函 数 名  : core_hashnode_new
 功能描述  : 获取一个节点信息
 输入参数  : 无
 输出参数  : 无
 返 回 值  : CORE_HASHNODE_S
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2019年3月21日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
CORE_HASHNODE_S *core_hashnode_gain(PCORE_HASH_TABLE_S pstHashTbl)
{
    CORE_HASHNODE_S*    pstHashNode = NULL;
    vos_dlist_node_s*   pstListNode = NULL;

    /*直接获取空闲节点*/
    if ( VOS_OK != VOS_Node_IsEmpty(&pstHashTbl->stIdleList) )
    {
        VOS_Node_HeadGet(&pstHashTbl->stIdleList, &pstListNode);
        
        pstHashNode = VOS_DLIST_ENTRY(pstListNode, CORE_HASHNODE_S, stBlkList);
        
        if ( NULL != pstHashNode->pvKeyData )
        {
            PrintError("[HSTBL] system gain hash data error");
            free(pstHashNode);
            return NULL;
        }
        
        pstHashNode->ulBlkNum = 0;
        memset(pstHashNode->acKeyStr,0, SYS_MAXID_LEN);
        
        return pstHashNode;
    }
    
    /*否则 新建一个Hash节点*/
    pstHashNode = (CORE_HASHNODE_S *)malloc(sizeof(CORE_HASHNODE_S));
    if ( NULL == pstHashNode )
    {
        PrintError("[HSTBL] malloc error");
        return pstHashNode;
    }
    
    memset(pstHashNode,0, sizeof(CORE_HASHNODE_S));
    VOS_DLIST_INIT(&pstHashNode->stBlkList);
    pstHashNode->pvKeyData = NULL;
    
    return pstHashNode;
}

/*****************************************************************************
 函 数 名  : core_hashnode_delete
 功能描述  : 归还一个节点
 输入参数  : CORE_HASHNODE_S *pstNode  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2019年3月21日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
void core_hashnode_back(PCORE_HASH_TABLE_S pstHashTbl, CORE_HASHNODE_S *pstNode)
{
    if ( NULL == pstNode )
    {
        return;
    }

    if ( NULL != pstNode->pvKeyData )
    {
        free(pstNode->pvKeyData);
        pstNode->pvKeyData = NULL;
    }
    
    VOS_DLIST_ADD_TAIL(&pstHashTbl->stIdleList, &pstNode->stBlkList);
    memset(pstNode->acKeyStr, 0, SYS_MAXID_LEN);
}

/*****************************************************************************
 函 数 名  : core_hashnode_release
 功能描述  : 完全释放一个节点
 输入参数  : PCORE_HASHNODE_S *ppstNode  
 输出参数  : 无
 返 回 值: 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2019年3月21日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
void core_hashnode_release(PCORE_HASHNODE_S *ppstNode)
{
    
    
    
}


/*****************************************************************************
 函 数 名  : core_hashstr_nor
 功能描述  : 正常Hash,不保护，直接计算Hash值
 输入参数  : const void *pcStr  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2019年3月21日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
uint32_t core_hashstr_normal(IN const char *pcString)
{
    uint32_t  ret = 0;
    int32_t   n   = 0;
    uint32_t  v   = 0;
    int32_t  r   = 0;
    
    n = 0x100;
    
    while (*pcString) 
    {
        v = n | (*pcString);
        n += 0x100;
        r = (int32_t)((v >> 2) ^ v) & 0x0F;
        ret = (ret << r) | (ret >> (32 - r));
        ret &= 0xFFFFFFFFL;
        ret ^= v * v;
        pcString++;
    }
    
    return ((ret >> 16) ^ ret);
}


/*****************************************************************************
 函 数 名: core_hashdbj_string
 功能描述  : core_hashstr_dbj
 输入参数  : const char *str  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2019年3月21日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
uint32_t core_hashstr_dbj(IN const char *str) 
{
    unsigned int hash = 5381;
    
    while (*str) 
    {
        hash = ((hash << 5) + hash) + (*str++); /* times 33 */
    }
    
    hash &= ~(1 << 31);                         /* strip the highest bit */
    return hash;
}

/*****************************************************************************
 函 数 名  : core_hashstr_cmp
 功能描述  : 比较，仅仅返回为0时候表示相同
 输入参数  : const char *pvArgv1  
             const void *pvArgv2  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2019年3月21日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
int32_t core_hashstr_cmp(IN const void *pvArgv1,IN const void *pvArgv2)
{
    return strcmp((char *)pvArgv1, (char *)pvArgv2);
}


/*****************************************************************************
 函 数 名: core_hashtbl_create
 功能描述  : 创建Hash表
 输入参数  : uint32_t ulSize                         Hash桶大小
           pfcore_hash_comp_cb pfHashCmp      Hash比较函数
           pfcore_hash_calc_cb pfHashCalc     Hash计算函数
 输出参数  : 无
 返 回 值: CORE_HASH_TABLE_S
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2019年3月21日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
CORE_HASH_TABLE_S *core_hashtbl_create(IN uint32_t ulSize, IN pfcore_hash_comp_cb pfHashCmp, IN pfcore_hash_calc_cb pfHashCalc)
{
    CORE_HASH_TABLE_S*      pstHashTbl  = NULL;
    uint32_t                   ulIndex     = 0;
    
    /*因为Hash表不会经常创建和释放，所以直接使用系统的, 确保内存使用安全*/
    pstHashTbl = (CORE_HASH_TABLE_S *)malloc(sizeof(CORE_HASH_TABLE_S));
    if ( NULL == pstHashTbl )
    {
        PrintError("[HSTBL] malloc hash table error");
        return NULL;
    }
    
    /*内存因为申请之后基本上就不再释放*/
    memset(pstHashTbl, 0 , sizeof(CORE_HASH_TABLE_S));
    
    /*选取合适的素数*/
    pstHashTbl->ulprime = core_hashtbl_prime(ulSize);
    if ( 0 == pstHashTbl->ulprime )
    {
        PrintError("[HSTBL] choice the prime failed!");
        free(pstHashTbl);
        return NULL;
    }
    
    /*申请hash表中的Hash节点, 通过选取的合适的素数来创建数组大小*/
    pstHashTbl->ppstArrayHashTbl = (CORE_HASHNODE_S **)malloc(sizeof(CORE_HASHNODE_S *) * pstHashTbl->ulprime);
    if ( NULL == pstHashTbl->ppstArrayHashTbl )
    {
        PrintError("[HSTBL] create the hashtbl failed!");
        free(pstHashTbl);
        return NULL;
    }
    
    /*初始化每个Hash节点*/
    for(ulIndex=0; ulIndex < pstHashTbl->ulprime; ulIndex++)
    {
        pstHashTbl->ppstArrayHashTbl[ulIndex] = NULL;
    }

    VOS_DLIST_INIT(&pstHashTbl->stIdleList);
    
    /*默认创建字符型的Hash表*/
    pstHashTbl->pfHashKeyCmp = ((pfHashCmp == NULL) ? (pfcore_hash_comp_cb)core_hashstr_cmp : pfHashCmp);
    pstHashTbl->pfHashKeyCalc = ((pfHashCalc == NULL) ? (pfcore_hash_calc_cb)core_hashstr_normal : pfHashCalc);
    
    return pstHashTbl;
}


/*****************************************************************************
 函 数 名: core_hashtbl_release
 功能描述  : 释放该Hash表
 输入参数  : PCORE_HASH_TABLE_S* pstHashTbl  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2019年3月21日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
void    core_hashtbl_release(INOUT PCORE_HASH_TABLE_S* ppstHashTbl)
{
    PCORE_HASH_TABLE_S  pstHashTbl = NULL;
    
    if ( NULL == ppstHashTbl )
    {
        return;
    }

    pstHashTbl = *ppstHashTbl;

    if ( NULL != pstHashTbl )
    {
        if (NULL != pstHashTbl->ppstArrayHashTbl)
        {
            for (uint32_t uiCount = 0; uiCount< pstHashTbl->ulprime; uiCount++)
            {
                if ( NULL != pstHashTbl->ppstArrayHashTbl[uiCount] )
                {
                    /*直接通过内存,云平台不使用VOS_Free()来管理*/
                    free((char *)pstHashTbl->ppstArrayHashTbl[uiCount]);
                }
            }
        
            free(pstHashTbl->ppstArrayHashTbl);
        }

        free(pstHashTbl);
    }
    
    *ppstHashTbl = NULL;
    
}



/*****************************************************************************
 函 数 名  : core_hashtbl_find
 功能描述  : 根据Key找到用户数据
 输入参数  : PCORE_HASH_TABLE_S pstHashTbl  
           char *pcKeyStr                 
 输出参数  : 无
 返 回 值  : void
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2019年3月21日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
void *core_hashtbl_find(PCORE_HASH_TABLE_S pstHashTbl, char *pcKeyStr)
{
    uint32_t ulHashIndex = 0;
    uint32_t ulKeyValue   = 0;
    CORE_HASHNODE_S *pstHashHead = NULL;
    CORE_HASHNODE_S *pstHashTmp  = NULL;
    
    if ( NULL == pstHashTbl )
    {
        PrintError("[HSTBL] param error!");
        return NULL;
    }
    
    /*计算出Hash值*/
    ulKeyValue  = pstHashTbl->pfHashKeyCalc((void *)pcKeyStr);
    /*素数取余*/
    ulHashIndex = ulKeyValue % pstHashTbl->ulprime;
    
    /*没有找到*/
    if ( NULL == pstHashTbl->ppstArrayHashTbl[ulHashIndex] )
    {
        return NULL;
    }
    
    pstHashHead = VOS_DLIST_ENTRY(&(pstHashTbl->ppstArrayHashTbl[ulHashIndex]->stBlkList), CORE_HASHNODE_S, stBlkList);
    /*只有一个节点*/
    if ( 0 == pstHashHead->ulBlkNum )
    {
        /*比较不相同的情况下，获取失败*/
        if ( VOS_OK != pstHashTbl->pfHashKeyCmp((void *)pcKeyStr, (void *)pstHashHead->acKeyStr) )
        {
            return NULL;
        }
        return pstHashHead->pvKeyData;
    }
    /*存在多个节点冲突的情况*/
    else if ( 0 < pstHashHead->ulBlkNum )
    {
        /*看看是否 当前第一个有相同的KEY*/
        if ( VOS_OK == pstHashTbl->pfHashKeyCmp((void *)pcKeyStr, (void *)pstHashHead->acKeyStr) )
        {
            return pstHashHead->pvKeyData;
        }
        /*前面第一个节点已经比较，下一个next节点开始*/
        for( pstHashTmp = VOS_DLIST_ENTRY(pstHashHead->stBlkList.next, CORE_HASHNODE_S, stBlkList);\
                &(pstHashTmp->stBlkList) != &(pstHashHead->stBlkList);\
               pstHashTmp= VOS_DLIST_ENTRY(pstHashTmp->stBlkList.next, CORE_HASHNODE_S, stBlkList))
        {
             /*遍历看看是否有相同的KEY*/
            if ( VOS_OK == pstHashTbl->pfHashKeyCmp((void *)pcKeyStr, (void *)pstHashTmp->acKeyStr) )
            {
                return pstHashTmp->pvKeyData;
            }
        }
    }
    
    return NULL;
}

/*****************************************************************************
 函 数 名  : core_hashtbl_insert
 功能描述  : 插入用户数据
 输入参数  : PCORE_HASH_TABLE_S pstHashTbl  
             char *pcKeyStr                 
             void *pvData                   注意该内存一定要malloc申请，因为remove会采用free进行释放
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2019年3月21日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
int32_t    core_hashtbl_insert(PCORE_HASH_TABLE_S pstHashTbl, char *pcKeyStr, void *pvData)
{
    uint32_t ulHashIndex  = 0;
    uint32_t ulKeyValue   = 0;
    CORE_HASHNODE_S *pstHashNode = NULL;
    CORE_HASHNODE_S *pstHashHead = NULL;
    CORE_HASHNODE_S *pstHashTmp  = NULL;
    
    if ( NULL == pstHashTbl 
        || NULL == pcKeyStr
        || NULL == pvData )
    {
        PrintError("[HSTBL] param error");
        return SYS_ERR;
    }
    
    pstHashNode = core_hashnode_gain(pstHashTbl);
    if ( NULL == pstHashNode )
    {
        PrintError("[HSTBL] system error");
        return SYS_ERR;
    }
    
    /*该Hash节点的ULONG值*/
    pstHashNode->pvKeyData = pvData;
    
    /*计算出Hash值*/
    ulKeyValue  = pstHashTbl->pfHashKeyCalc((void *)pcKeyStr);
    strcpy(pstHashNode->acKeyStr, pcKeyStr);
    
    /*素数取余*/
    ulHashIndex = ulKeyValue % pstHashTbl->ulprime;
    
    if ( NULL == pstHashTbl->ppstArrayHashTbl[ulHashIndex] )
    {
        /*如果存在空闲，则直接插入即可*/
        pstHashTbl->ppstArrayHashTbl[ulHashIndex] = pstHashNode;
        return SYS_OK;
    }
    
    /*先比较第一个头节点，看看有无冲突*/
    pstHashHead = VOS_DLIST_ENTRY(&(pstHashTbl->ppstArrayHashTbl[ulHashIndex]->stBlkList), CORE_HASHNODE_S, stBlkList);
    if ( VOS_OK == pstHashTbl->pfHashKeyCmp((void *)pcKeyStr, (void *)pstHashHead->acKeyStr ) )
    {
        PrintError("[HSTBL] The key has already exist! keyID=[%s], index=[%d]", pcKeyStr, ulHashIndex);
        
        /*如果冲突，则表示出错*/
        free(pstHashNode);
        return SYS_ERR_EXIST;
    }
        
    /*如果目前只有一个节点, 那么就直接加入冲突链表*/
    if ( 0 == pstHashHead->ulBlkNum )
    {
        PrintTraceEvent("[HSTBL] hashtable insert empty list succ! hashStr=[%s],keyStr=[%s:%d]", 
                                pstHashHead->acKeyStr, pcKeyStr, ulHashIndex);
        /*直接插入后面*/
        VOS_DLIST_ADD_TAIL(&pstHashHead->stBlkList, &pstHashNode->stBlkList);
        pstHashHead->ulBlkNum++;
        return SYS_OK;
    }
    /*存在多个节点冲突的情况*/
    else
    {
        PrintTraceWarning("[HSTBL] hashtable insert complict list start! hashStr=[%s],keyStr=[%s:%d]", 
                                pstHashHead->acKeyStr, pcKeyStr, ulHashIndex);
        /*前面第一个节点已经比较，下一个next节点开始*/
        for( pstHashTmp = VOS_DLIST_ENTRY(pstHashHead->stBlkList.next, CORE_HASHNODE_S, stBlkList);\
                &(pstHashTmp->stBlkList) != &(pstHashHead->stBlkList);\
               pstHashTmp= VOS_DLIST_ENTRY(pstHashTmp->stBlkList.next, CORE_HASHNODE_S, stBlkList))
        {
            /*遍历看看是否有冲突*/
            if ( VOS_OK == pstHashTbl->pfHashKeyCmp((void *)pcKeyStr, (void *)pstHashTmp->acKeyStr ) )
            {
                PrintError("[HSTBL] hashtable insert has already exist! hashStr=[%s], keyStr=[%s:%d]", 
                                pstHashTmp->acKeyStr, pcKeyStr, ulHashIndex);
                
                /*如果冲突，则表示出错, 将节点归还*/
                core_hashnode_back(pstHashTbl, pstHashNode);
                return SYS_ERR_EXIST;
            }
        }
        /*直接插入后面*/
        VOS_DLIST_ADD_TAIL(&pstHashHead->stBlkList, &pstHashNode->stBlkList);
        pstHashHead->ulBlkNum++;
        PrintTraceWarning("[HSTBL] hashtable insert complict list succ! hashStr=[%s],keyStr=[%s:%d]", 
                pstHashTmp->acKeyStr, pcKeyStr, ulHashIndex);
    }
    
    return SYS_OK;
}


/*****************************************************************************
 函 数 名  : core_hashtbl_remove
 功能描述  : 删除用户数据
 输入参数  : PCORE_HASH_TABLE_S pstHashTbl  
             char *pcKeyStr                 
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2019年3月21日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
void    core_hashtbl_remove(PCORE_HASH_TABLE_S pstHashTbl, char *pcKeyStr)
{
    uint32_t ulHashIndex   = 0;
    uint32_t ulKeyValue    = 0;
    CORE_HASHNODE_S *pstHashHead = NULL;
    CORE_HASHNODE_S *pstHashTmp  = NULL;

    if ( NULL == pstHashTbl )
    {
        PrintError("[HSTBL] param error");
        return;
    }
    
    /*计算出Hash值*/
    ulKeyValue  = pstHashTbl->pfHashKeyCalc((void *)pcKeyStr);
    /*素数取余*/
    ulHashIndex = ulKeyValue % pstHashTbl->ulprime;
    
    pstHashHead = pstHashTbl->ppstArrayHashTbl[ulHashIndex];
    if ( NULL == pstHashHead )
    {
        PrintError("[HSTBL] hash table index=[%d:%s] has empty!", ulHashIndex, pcKeyStr);
        /*错误*/
        return;
    }
    
    if ( 0 == pstHashHead->ulBlkNum )
    {
        /*看看是否相同*/
        if ( VOS_ERR == pstHashTbl->pfHashKeyCmp(pcKeyStr, pstHashHead->acKeyStr) )
        {
            /*不太可能发生的*/
            PrintError("[HSTBL] system error!hashStr=[%s],keyStr=[%s:%d],value has equal,but not removed!", 
                                pstHashHead->acKeyStr, pcKeyStr, ulHashIndex);
            return;
        }

        PrintTraceEvent("[HSTBL] remove the hashNode,hashStr=[%s], keystr=[%s:%d], back it to pool!", 
                        pstHashHead->acKeyStr, pcKeyStr, ulHashIndex);
        
        core_hashnode_back(pstHashTbl, pstHashHead);
        
        pstHashTbl->ppstArrayHashTbl[ulHashIndex]  = NULL;

        return;
    }
    /*存在多个节点冲突的情况*/
    else if ( 0 < pstHashHead->ulBlkNum )
    {
        /*先比较第一个头节点，看看有无相同的KEY*/
        pstHashHead = VOS_DLIST_ENTRY(&(pstHashTbl->ppstArrayHashTbl[ulHashIndex]->stBlkList), CORE_HASHNODE_S, stBlkList);
        
        /*遍历看看该头节点是否即为该相同的KEY*/
        if ( VOS_OK == pstHashTbl->pfHashKeyCmp(pcKeyStr, pstHashHead->acKeyStr) )
        {
            /*先减掉一个*/
            pstHashHead->ulBlkNum--;
            /*得把下一个节点设置为头节点*/
            pstHashTmp = VOS_DLIST_ENTRY(pstHashHead->stBlkList.next, CORE_HASHNODE_S, stBlkList);
            /*将少了一个节点的值，赋值给新的头节点*/
            pstHashTmp->ulBlkNum = pstHashHead->ulBlkNum;
            /*设置后面的这个节点为新的头节点*/
            pstHashTbl->ppstArrayHashTbl[ulHashIndex] = pstHashTmp;
            /*将自己删除*/
            VOS_DLIST_DEL(&pstHashHead->stBlkList);
            
            PrintTraceEvent("[HSTBL] remove the hashNode, hashStr=[%s], keystr=[%s:%d],first clt-list, BlkNums=%d", 
                                pstHashHead->acKeyStr, pcKeyStr, ulHashIndex, pstHashHead->ulBlkNum );
            
            core_hashnode_back(pstHashTbl, pstHashHead);
            
            return;
        }
        
        /*前面第一个节点已经比较，从下一个节点开始*/
        for( pstHashTmp = VOS_DLIST_ENTRY(pstHashHead->stBlkList.next, CORE_HASHNODE_S, stBlkList);\
                &(pstHashTmp->stBlkList) != &(pstHashHead->stBlkList);\
               pstHashTmp= VOS_DLIST_ENTRY(pstHashTmp->stBlkList.next, CORE_HASHNODE_S, stBlkList))
        {
            /*遍历看看是否有相同的KEY*/
            if ( VOS_OK == pstHashTbl->pfHashKeyCmp(pcKeyStr, pstHashTmp->acKeyStr) )
            {
                /*因为数量都是头节点管理的，先减掉一个,
                 而且经过前面的判断，头节点没有相同，删除的不是头节点*/
                pstHashHead->ulBlkNum--;
                
                /*将自己删除*/
                VOS_DLIST_DEL(&pstHashTmp->stBlkList);
                
                PrintTraceEvent("[HSTBL] remove the hashNode, hashStr=[%s], keystr=[%s:%d], second clt-list, BlkNums=%d", 
                                pstHashTmp->acKeyStr, pcKeyStr, ulHashIndex, pstHashHead->ulBlkNum );
                
                core_hashnode_back(pstHashTbl, pstHashTmp);
                return;
            }
        }
    }
        
}


/***************************************针对分发特性新写的Hash环随机算法********************************************/
/*****************************************************************************
 函 数 名  : core_hash_key
 功能描述  : ngnix的Hash最简函数
 输入参数  : uint8_t *pucData  
             int iLen        
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2019年3月26日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
uint32_t core_hash_key(uint8_t *pucData, int32_t iLen)
{
    int  i=0, key=0;

    for (i = 0; i < iLen; i++) {
        key = core_hash(key, pucData[i]);
    }
    
    return key == 0 ? 1 : key;
}



