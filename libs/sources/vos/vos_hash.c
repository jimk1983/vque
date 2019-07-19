/******************************************************************************

                  版权所有 (C), 2018-2028, 技术有限公司

 ******************************************************************************
  文 件 名   : vos_hash.c
  版 本 号   : 初稿
  作    者   : 蒋康
  生成日期   : 2016年2月15日
  最近修改   :
  功能描述   : 简单的string类Hash相关实现
  函数列表   :
  修改历史   :
  1.日    期   : 2016年2月15日
    作    者   : 蒋康
    修改内容   : 创建文件

******************************************************************************/
#include <vos/vos_pub.h>

#define vos_hash(key, c)   ((UINT32_T) key * 31 + c)

/*素数空间数组*/
static UINT32_T g_ulCorePrimeArry[]={
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
#define CORE_TBLPRIME_SIZE    (sizeof(g_ulCorePrimeArry) / sizeof(UINT32_T))

/**
 * @brief choice a correct prime number
 * @param size [in] the hash table size
 * @retval return the prime number.
 */
UINT32_T VOS_HashTbl_Prime(IN UINT32_T ulSize)
{
    UINT32_T ulCount = CORE_TBLPRIME_SIZE;
    UINT32_T ulIndex = 0;
    UINT32_T ulPrime = g_ulCorePrimeArry[0];
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

/**
 * @brief gain a hash node
 * @param pstHashTbl [in] the hash tables
 * @retval return the gain or new create hash node.
 */
VOS_HASHNODE_S *VOS_HashNode_Gain( IN PCORE_HASH_TABLE_S pstHashTbl)
{
    VOS_HASHNODE_S*     pstHashNode = NULL;
    VOS_DLIST_S*        pstListNode = NULL;

    /*直接获取空闲节点*/
    if ( VOS_TRUE != VOS_DList_IsEmpty(&pstHashTbl->stIdleList) )
    {
        VOS_DList_HeadGet(&pstHashTbl->stIdleList, &pstListNode);
        
        pstHashNode = VOS_DLIST_ENTRY(pstListNode, VOS_HASHNODE_S, stBlkList);
        
        if ( NULL != pstHashNode->pvKeyData )
        {
            PError("gain hash data error");
            free(pstHashNode);
            return NULL;
        }
        
        pstHashNode->ulBlkNum = 0;
        memset(pstHashNode->acKeyStr,0, SYS_MAXID_LEN);
        
        return pstHashNode;
    }
    
    /*否则 新建一个Hash节点*/
    pstHashNode = (VOS_HASHNODE_S *)malloc(sizeof(VOS_HASHNODE_S));
    if ( NULL == pstHashNode )
    {
        PError("malloc error");
        return pstHashNode;
    }
    
    memset(pstHashNode,0, sizeof(VOS_HASHNODE_S));
    VOS_DLIST_INIT(&pstHashNode->stBlkList);
    pstHashNode->pvKeyData = NULL;
    
    return pstHashNode;
}

/**
 * @brief back a hash node
 * @param pstHashTbl [in] the hash tables
 * @retval return none
 */
VOID VOS_HashNode_Back(PCORE_HASH_TABLE_S pstHashTbl, VOS_HASHNODE_S *pstNode)
{
    if ( NULL != pstNode->pvKeyData )
    {
        free(pstNode->pvKeyData);
        pstNode->pvKeyData = NULL;
    }
    
    VOS_DLIST_ADD_TAIL(&pstHashTbl->stIdleList, &pstNode->stBlkList);
    memset(pstNode->acKeyStr, 0, SYS_MAXID_LEN);
}

/**
 * @brief release a hash node
 * @param ppstNode [in] the hash node
 * @retval return none
 */
VOID VOS_HashNode_Release(INOUT PVOS_HASHNODE_S *ppstNode)
{
    PVOS_HASHNODE_S     pstNode = NULL;
    
    if ( NULL != ppstNode )
    {
        pstNode = *ppstNode;
        free(pstNode);
        *ppstNode  =NULL;
    }
}


/**
 * @brief caculate a value with a string
 * @param pcString [in] string
 * @retval return the hash value
 */
UINT32_T VOS_HashStr_Normal(IN const CHAR *pcString)
{
    UINT32_T  ret = 0;
    UINT32_T  v   = 0;
    INT32_T   n   = 0;
    INT32_T   r   = 0;
    
    n = 0x100;
    
    while (*pcString) 
    {
        v = n | (*pcString);
        n += 0x100;
        r = (INT32_T)((v >> 2) ^ v) & 0x0F;
        ret = (ret << r) | (ret >> (32 - r));
        ret &= 0xFFFFFFFFL;
        ret ^= v * v;
        pcString++;
    }
    
    return ((ret >> 16) ^ ret);
}
    
/**
 * @brief caculate a value with a string, method is the DBJ
 * @param pcString [in] string
 * @retval return the hash value
 */
UINT32_T VOS_HashStr_Dbj( IN const CHAR *pcString) 
{
    UINT32_T hash = 5381;
    
    while (*pcString) 
    {
        hash = ((hash << 5) + hash) + (*pcString++); /* times 33 */
    }
    
    /* strip the highest bit */
    hash &= ~(1 << 31);
    
    return hash;
}

/**
 * @brief compare the hash string type 
 * @param pvArgv1 [in] string
 * @param pvArgv2 [in] string
 * @retval return the hash value
 */
INT32_T VOS_HashStr_Cmp(IN const CHAR *pvArgv1,IN const CHAR *pvArgv2)
{
    if ( 0 == strcmp((char *)pvArgv1, (char *)pvArgv2) )
    {
        return VOS_OK;
    }
    
    return VOS_ERR;
}

/**
 * @brief create the hash table 
 * @param ulSize [in] hash table size
 * @param pfHashCmp [in] hash table compare function
 * @param pfHashCalc [in] hash table caculate function
 * @retval return the hash value
 */
CORE_HASH_TABLE_S *VOS_HashTbl_Create(IN UINT32_T ulSize, IN pfcore_hash_comp_cb pfHashCmp, IN pfcore_hash_calc_cb pfHashCalc)
{
    CORE_HASH_TABLE_S*      pstHashTbl  = NULL;
    UINT32_T                ulIndex     = 0;
    
    /*因为Hash表不会经常创建和释放，所以直接使用系统的, 确保内存使用安全*/
    pstHashTbl = (CORE_HASH_TABLE_S *)malloc(sizeof(CORE_HASH_TABLE_S));
    if ( NULL == pstHashTbl )
    {
        PError("malloc hash table error,%d:%s", errno, strerror(errno));
        return NULL;
    }
    
    /*内存申请之后基本上就不再释放*/
    memset(pstHashTbl, 0 , sizeof(CORE_HASH_TABLE_S));
    
    /*选取合适的素数*/
    pstHashTbl->ulprime = VOS_HashTbl_Prime(ulSize);
    if ( 0 == pstHashTbl->ulprime )
    {
        PError("choice the prime failed!");
        free(pstHashTbl);
        return NULL;
    }
    
    /*申请hash表中的Hash节点, 通过选取的合适的素数来创建数组大小*/
    pstHashTbl->ppstArrayHashTbl = (VOS_HASHNODE_S **)malloc(sizeof(VOS_HASHNODE_S *) * pstHashTbl->ulprime);
    if ( NULL == pstHashTbl->ppstArrayHashTbl )
    {
        PError("create the hashtbl failed!");
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
    pstHashTbl->pfHashKeyCmp = ((pfHashCmp == NULL) ? (pfcore_hash_comp_cb)VOS_HashStr_Cmp : pfHashCmp);
    pstHashTbl->pfHashKeyCalc = ((pfHashCalc == NULL) ? (pfcore_hash_calc_cb)VOS_HashStr_Normal : pfHashCalc);
    
    return pstHashTbl;
}


/**
 * @brief create the hash table 
 * @param ppstHashTbl [inout] hash table double ptr
 * @retval return the hash value
 */
void    VOS_HashTbl_Release(INOUT PCORE_HASH_TABLE_S* ppstHashTbl)
{
    PCORE_HASH_TABLE_S  pstHashTbl  = NULL;
    PVOS_HASHNODE_S     pstHashTmp  = NULL;
    PVOS_DLIST_S        pthisEntry  =NULL, pNextEntry=NULL, plistHead=NULL;
    
    if ( NULL == ppstHashTbl )
    {
        return;
    }
    
    pstHashTbl = *ppstHashTbl;
    
    if ( NULL != pstHashTbl )
    {
        if (NULL != pstHashTbl->ppstArrayHashTbl)
        {
            for (UINT32_T uiCount = 0; uiCount< pstHashTbl->ulprime; uiCount++)
            {
                if ( NULL != pstHashTbl->ppstArrayHashTbl[uiCount] )
                {
                    if ( VOS_TRUE != VOS_DList_IsEmpty( &pstHashTbl->ppstArrayHashTbl[uiCount]->stBlkList )  )
                    {
                        plistHead = &pstHashTbl->ppstArrayHashTbl[uiCount]->stBlkList;
                        for ( pthisEntry = plistHead->prev;
                            pthisEntry != plistHead;
                            pthisEntry = pNextEntry)
                        {
                            pNextEntry = pthisEntry->prev;
                            pstHashTmp = VOS_DLIST_ENTRY(pthisEntry, VOS_HASHNODE_S, stBlkList);
                            VOS_DLIST_DEL(&pstHashTmp->stBlkList);
                            free(pstHashTmp);
                            pstHashTmp=NULL;
                            /*must compare the list-head*/
                            pthisEntry = plistHead->prev;
                        }
                    }
                    free((char *)pstHashTbl->ppstArrayHashTbl[uiCount]);
                }
            }
            
            free(pstHashTbl->ppstArrayHashTbl);
        }
        
        free(pstHashTbl);
    }
    
    *ppstHashTbl = NULL;
}

/**
 * @brief create the hash table 
 * @param pstHashTbl [inout] hash table
 * @param pcKeyStr [inout] hash key string
 * @retval return the hash value
 */
PVOID VOS_HashTbl_Find(IN PCORE_HASH_TABLE_S pstHashTbl, IN const CHAR *pcKeyStr)
{
    UINT32_T        ulHashIndex = 0;
    UINT32_T        ulKeyValue  = 0;
    VOS_HASHNODE_S *pstHashHead = NULL;
    VOS_HASHNODE_S *pstHashTmp  = NULL;
    
    if ( NULL == pstHashTbl )
    {
        PError("param error!");
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
    
    pstHashHead = VOS_DLIST_ENTRY(&(pstHashTbl->ppstArrayHashTbl[ulHashIndex]->stBlkList), VOS_HASHNODE_S, stBlkList);
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
        for( pstHashTmp = VOS_DLIST_ENTRY(pstHashHead->stBlkList.next, VOS_HASHNODE_S, stBlkList);\
                &(pstHashTmp->stBlkList) != &(pstHashHead->stBlkList);\
               pstHashTmp= VOS_DLIST_ENTRY(pstHashTmp->stBlkList.next, VOS_HASHNODE_S, stBlkList))
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

/**
 * @brief create the hash table 
 * @param pstHashTbl [inout] hash table
 * @param pcKeyStr [inout] hash key string
 * @param pvData [in] hash key string
 * @retval return the hash value
 */
INT32_T    VOS_HashTbl_Insert(IN PCORE_HASH_TABLE_S pstHashTbl, IN      const CHAR *pcKeyStr, IN VOID *pvData)
{
    UINT32_T ulHashIndex  = 0;
    UINT32_T ulKeyValue   = 0;
    VOS_HASHNODE_S *pstHashNode = NULL;
    VOS_HASHNODE_S *pstHashHead = NULL;
    VOS_HASHNODE_S *pstHashTmp  = NULL;
    
    if ( NULL == pstHashTbl 
        || NULL == pcKeyStr
        || NULL == pvData )
    {
        PError("param error");
        return SYS_ERR;
    }
    
    pstHashNode = VOS_HashNode_Gain(pstHashTbl);
    if ( NULL == pstHashNode )
    {
        PError("system error");
        return SYS_ERR;
    }
    
    /*该Hash节点的ULONG值*/
    pstHashNode->pvKeyData = pvData;
    
    /*计算出Hash值*/
    ulKeyValue  = pstHashTbl->pfHashKeyCalc((void *)pcKeyStr);
    strcpy((char *)pstHashNode->acKeyStr, (char *)pcKeyStr);
    
    /*素数取余*/
    ulHashIndex = ulKeyValue % pstHashTbl->ulprime;
    
    if ( NULL == pstHashTbl->ppstArrayHashTbl[ulHashIndex] )
    {
        /*如果存在空闲，则直接插入即可*/
        pstHashTbl->ppstArrayHashTbl[ulHashIndex] = pstHashNode;
        return SYS_OK;
    }
    
    /*先比较第一个头节点，看看有无冲突*/
    pstHashHead = VOS_DLIST_ENTRY(&(pstHashTbl->ppstArrayHashTbl[ulHashIndex]->stBlkList), VOS_HASHNODE_S, stBlkList);
    if ( VOS_OK == pstHashTbl->pfHashKeyCmp((void *)pcKeyStr, (void *)pstHashHead->acKeyStr ) )
    {
        PError("The key has already exist! keyID=[%s], index=[%d]", pcKeyStr, ulHashIndex);
        
        /*如果冲突，则表示出错*/
        free(pstHashNode);
        return SYS_ERR_EXIST;
    }
    
    if ( 0 == pstHashHead->ulBlkNum )
    {
        //PEVENT("Hashtable insert empty list succ! hashStr=[%s],keyStr=[%s:%d]", 
        //                        pstHashHead->acKeyStr, pcKeyStr, ulHashIndex);
        VOS_DLIST_ADD_TAIL(&pstHashHead->stBlkList, &pstHashNode->stBlkList);
        pstHashHead->ulBlkNum++;
        return SYS_OK;
    }
    else
    {
        //PEVENT("[HSTBL] hashtable insert complict list start! hashStr=[%s],keyStr=[%s:%d]", 
        //                        pstHashHead->acKeyStr, pcKeyStr, ulHashIndex);
        
        for( pstHashTmp = VOS_DLIST_ENTRY(pstHashHead->stBlkList.next, VOS_HASHNODE_S, stBlkList);\
                &(pstHashTmp->stBlkList) != &(pstHashHead->stBlkList);\
               pstHashTmp= VOS_DLIST_ENTRY(pstHashTmp->stBlkList.next, VOS_HASHNODE_S, stBlkList))
        {
            if ( VOS_OK == pstHashTbl->pfHashKeyCmp((void *)pcKeyStr, (void *)pstHashTmp->acKeyStr ) )
            {
                PError("Hashtable insert has already exist! hashStr=[%s], keyStr=[%s:%d]", 
                                pstHashTmp->acKeyStr, pcKeyStr, ulHashIndex);
                
                VOS_HashNode_Back(pstHashTbl, pstHashNode);
                return SYS_ERR_EXIST;
            }
        }
               
        VOS_DLIST_ADD_TAIL(&pstHashHead->stBlkList, &pstHashNode->stBlkList);
        pstHashHead->ulBlkNum++;
        
        //PEVENT("[HSTBL] hashtable insert complict list succ! hashStr=[%s],keyStr=[%s:%d]", 
        //        pstHashTmp->acKeyStr, pcKeyStr, ulHashIndex);
    }
    
    return SYS_OK;
}


/**
 * @brief remove the hash node
 * @param pstHashTbl [inout] hash table
 * @param pcKeyStr [inout] hash key string
 * @retval return the hash value
 */
void    VOS_HashTbl_Remove(IN PCORE_HASH_TABLE_S pstHashTbl, IN      const CHAR *pcKeyStr)
{
    UINT32_T ulHashIndex   = 0;
    UINT32_T ulKeyValue    = 0;
    VOS_HASHNODE_S *pstHashHead = NULL;
    VOS_HASHNODE_S *pstHashTmp  = NULL;

    if ( NULL == pstHashTbl )
    {
        PError("param error");
        return;
    }
    
    /*计算出Hash值*/
    ulKeyValue  = pstHashTbl->pfHashKeyCalc((void *)pcKeyStr);
    /*素数取余*/
    ulHashIndex = ulKeyValue % pstHashTbl->ulprime;
    
    pstHashHead = pstHashTbl->ppstArrayHashTbl[ulHashIndex];
    if ( NULL == pstHashHead )
    {
        PError("Hash table index=[%d:%s] has empty!", ulHashIndex, pcKeyStr);
        
        return;
    }
    
    if ( 0 == pstHashHead->ulBlkNum )
    {
        if ( VOS_ERR == pstHashTbl->pfHashKeyCmp(pcKeyStr, pstHashHead->acKeyStr) )
        {
            PError("System error!hashStr=[%s],keyStr=[%s:%d],value has equal,but not removed!", 
                                pstHashHead->acKeyStr, pcKeyStr, ulHashIndex);
            return;
        }
        
        //PEVENT("[HSTBL] remove the hashNode,hashStr=[%s], keystr=[%s:%d], back it to pool!", 
        //                pstHashHead->acKeyStr, pcKeyStr, ulHashIndex);
        
        VOS_HashNode_Back(pstHashTbl, pstHashHead);
        
        pstHashTbl->ppstArrayHashTbl[ulHashIndex]  = NULL;

        return;
    }
    /*存在多个节点冲突的情况*/
    else if ( 0 < pstHashHead->ulBlkNum )
    {
        /*先比较第一个头节点，看看有无相同的KEY*/
        pstHashHead = VOS_DLIST_ENTRY(&(pstHashTbl->ppstArrayHashTbl[ulHashIndex]->stBlkList), VOS_HASHNODE_S, stBlkList);
        
        /*遍历看看该头节点是否即为该相同的KEY*/
        if ( VOS_OK == pstHashTbl->pfHashKeyCmp(pcKeyStr, pstHashHead->acKeyStr) )
        {
            /*先减掉一个*/
            pstHashHead->ulBlkNum--;
            /*得把下一个节点设置为头节点*/
            pstHashTmp = VOS_DLIST_ENTRY(pstHashHead->stBlkList.next, VOS_HASHNODE_S, stBlkList);
            /*将少了一个节点的值，赋值给新的头节点*/
            pstHashTmp->ulBlkNum = pstHashHead->ulBlkNum;
            /*设置后面的这个节点为新的头节点*/
            pstHashTbl->ppstArrayHashTbl[ulHashIndex] = pstHashTmp;
            /*将自己删除*/
            VOS_DLIST_DEL(&pstHashHead->stBlkList);
            
            //PrintTraceEvent("[HSTBL] remove the hashNode, hashStr=[%s], keystr=[%s:%d],first clt-list, BlkNums=%d", 
            //                    pstHashHead->acKeyStr, pcKeyStr, ulHashIndex, pstHashHead->ulBlkNum );
            
            VOS_HashNode_Back(pstHashTbl, pstHashHead);
            
            return;
        }
        
        for( pstHashTmp = VOS_DLIST_ENTRY(pstHashHead->stBlkList.next, VOS_HASHNODE_S, stBlkList);\
                &(pstHashTmp->stBlkList) != &(pstHashHead->stBlkList);\
               pstHashTmp= VOS_DLIST_ENTRY(pstHashTmp->stBlkList.next, VOS_HASHNODE_S, stBlkList))
        {
            /*遍历看看是否有相同的KEY*/
            if ( VOS_OK == pstHashTbl->pfHashKeyCmp(pcKeyStr, pstHashTmp->acKeyStr) )
            {
                /*因为数量都是头节点管理的，先减掉一个,
                 而且经过前面的判断，头节点没有相同，删除的不是头节点*/
                pstHashHead->ulBlkNum--;
                
                /*将自己删除*/
                VOS_DLIST_DEL(&pstHashTmp->stBlkList);
                
                //PEVENT("[HSTBL] remove the hashNode, hashStr=[%s], keystr=[%s:%d], second clt-list, BlkNums=%d", 
                //                pstHashTmp->acKeyStr, pcKeyStr, ulHashIndex, pstHashHead->ulBlkNum );
                
                VOS_HashNode_Back(pstHashTbl, pstHashTmp);
                return;
            }
        }
    }
        
}


UINT32_T VOS_HashVal_key(UCHAR *puckey, INT32_T iLen)
{
    int  i=0, key=0;

    for (i = 0; i < iLen; i++) {
        key = vos_hash(key, puckey[i]);
    }
    
    return key == 0 ? 1 : key;
}



