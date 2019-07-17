/******************************************************************************

                  ��Ȩ���� (C), 2018-2028, �����ѻ��������޹�˾

 ******************************************************************************
  �� �� ��   : core_hash.c
  �� �� ��   : ����
  ��    ��   : ����
  ��������   : 2019��3��21��
  ����޸�   :
  ��������   : �ʺ��ѻ�����򵥵�Hash���ʵ��
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2019��3��21��
    ��    ��   : ����
    �޸�����   : �����ļ�

******************************************************************************/

#include "share.h"


#define core_hash(key, c)   ((uint32_t) key * 31 + c)

/*�����ռ�����*/
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

/*�����������С*/
#define CORE_TBLPRIME_SIZE    (sizeof(g_ulCorePrimeArry) / sizeof(uint32_t))


/*****************************************************************************
 �� �� ��  : core_hashtbl_prime
 ��������  : ��������
 �������  : uint32_t ulSize  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2019��3��21��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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
 �� �� ��  : core_hashnode_new
 ��������  : ��ȡһ���ڵ���Ϣ
 �������  : ��
 �������  : ��
 �� �� ֵ  : CORE_HASHNODE_S
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2019��3��21��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
CORE_HASHNODE_S *core_hashnode_gain(PCORE_HASH_TABLE_S pstHashTbl)
{
    CORE_HASHNODE_S*    pstHashNode = NULL;
    vos_dlist_node_s*   pstListNode = NULL;

    /*ֱ�ӻ�ȡ���нڵ�*/
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
    
    /*���� �½�һ��Hash�ڵ�*/
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
 �� �� ��  : core_hashnode_delete
 ��������  : �黹һ���ڵ�
 �������  : CORE_HASHNODE_S *pstNode  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2019��3��21��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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
 �� �� ��  : core_hashnode_release
 ��������  : ��ȫ�ͷ�һ���ڵ�
 �������  : PCORE_HASHNODE_S *ppstNode  
 �������  : ��
 �� �� ֵ: 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2019��3��21��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
void core_hashnode_release(PCORE_HASHNODE_S *ppstNode)
{
    
    
    
}


/*****************************************************************************
 �� �� ��  : core_hashstr_nor
 ��������  : ����Hash,��������ֱ�Ӽ���Hashֵ
 �������  : const void *pcStr  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2019��3��21��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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
 �� �� ��: core_hashdbj_string
 ��������  : core_hashstr_dbj
 �������  : const char *str  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2019��3��21��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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
 �� �� ��  : core_hashstr_cmp
 ��������  : �Ƚϣ���������Ϊ0ʱ���ʾ��ͬ
 �������  : const char *pvArgv1  
             const void *pvArgv2  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2019��3��21��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
int32_t core_hashstr_cmp(IN const void *pvArgv1,IN const void *pvArgv2)
{
    return strcmp((char *)pvArgv1, (char *)pvArgv2);
}


/*****************************************************************************
 �� �� ��: core_hashtbl_create
 ��������  : ����Hash��
 �������  : uint32_t ulSize                         HashͰ��С
           pfcore_hash_comp_cb pfHashCmp      Hash�ȽϺ���
           pfcore_hash_calc_cb pfHashCalc     Hash���㺯��
 �������  : ��
 �� �� ֵ: CORE_HASH_TABLE_S
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2019��3��21��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
CORE_HASH_TABLE_S *core_hashtbl_create(IN uint32_t ulSize, IN pfcore_hash_comp_cb pfHashCmp, IN pfcore_hash_calc_cb pfHashCalc)
{
    CORE_HASH_TABLE_S*      pstHashTbl  = NULL;
    uint32_t                   ulIndex     = 0;
    
    /*��ΪHash���ᾭ���������ͷţ�����ֱ��ʹ��ϵͳ��, ȷ���ڴ�ʹ�ð�ȫ*/
    pstHashTbl = (CORE_HASH_TABLE_S *)malloc(sizeof(CORE_HASH_TABLE_S));
    if ( NULL == pstHashTbl )
    {
        PrintError("[HSTBL] malloc hash table error");
        return NULL;
    }
    
    /*�ڴ���Ϊ����֮������ϾͲ����ͷ�*/
    memset(pstHashTbl, 0 , sizeof(CORE_HASH_TABLE_S));
    
    /*ѡȡ���ʵ�����*/
    pstHashTbl->ulprime = core_hashtbl_prime(ulSize);
    if ( 0 == pstHashTbl->ulprime )
    {
        PrintError("[HSTBL] choice the prime failed!");
        free(pstHashTbl);
        return NULL;
    }
    
    /*����hash���е�Hash�ڵ�, ͨ��ѡȡ�ĺ��ʵ����������������С*/
    pstHashTbl->ppstArrayHashTbl = (CORE_HASHNODE_S **)malloc(sizeof(CORE_HASHNODE_S *) * pstHashTbl->ulprime);
    if ( NULL == pstHashTbl->ppstArrayHashTbl )
    {
        PrintError("[HSTBL] create the hashtbl failed!");
        free(pstHashTbl);
        return NULL;
    }
    
    /*��ʼ��ÿ��Hash�ڵ�*/
    for(ulIndex=0; ulIndex < pstHashTbl->ulprime; ulIndex++)
    {
        pstHashTbl->ppstArrayHashTbl[ulIndex] = NULL;
    }

    VOS_DLIST_INIT(&pstHashTbl->stIdleList);
    
    /*Ĭ�ϴ����ַ��͵�Hash��*/
    pstHashTbl->pfHashKeyCmp = ((pfHashCmp == NULL) ? (pfcore_hash_comp_cb)core_hashstr_cmp : pfHashCmp);
    pstHashTbl->pfHashKeyCalc = ((pfHashCalc == NULL) ? (pfcore_hash_calc_cb)core_hashstr_normal : pfHashCalc);
    
    return pstHashTbl;
}


/*****************************************************************************
 �� �� ��: core_hashtbl_release
 ��������  : �ͷŸ�Hash��
 �������  : PCORE_HASH_TABLE_S* pstHashTbl  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2019��3��21��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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
                    /*ֱ��ͨ���ڴ�,��ƽ̨��ʹ��VOS_Free()������*/
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
 �� �� ��  : core_hashtbl_find
 ��������  : ����Key�ҵ��û�����
 �������  : PCORE_HASH_TABLE_S pstHashTbl  
           char *pcKeyStr                 
 �������  : ��
 �� �� ֵ  : void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2019��3��21��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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
    
    /*�����Hashֵ*/
    ulKeyValue  = pstHashTbl->pfHashKeyCalc((void *)pcKeyStr);
    /*����ȡ��*/
    ulHashIndex = ulKeyValue % pstHashTbl->ulprime;
    
    /*û���ҵ�*/
    if ( NULL == pstHashTbl->ppstArrayHashTbl[ulHashIndex] )
    {
        return NULL;
    }
    
    pstHashHead = VOS_DLIST_ENTRY(&(pstHashTbl->ppstArrayHashTbl[ulHashIndex]->stBlkList), CORE_HASHNODE_S, stBlkList);
    /*ֻ��һ���ڵ�*/
    if ( 0 == pstHashHead->ulBlkNum )
    {
        /*�Ƚϲ���ͬ������£���ȡʧ��*/
        if ( VOS_OK != pstHashTbl->pfHashKeyCmp((void *)pcKeyStr, (void *)pstHashHead->acKeyStr) )
        {
            return NULL;
        }
        return pstHashHead->pvKeyData;
    }
    /*���ڶ���ڵ��ͻ�����*/
    else if ( 0 < pstHashHead->ulBlkNum )
    {
        /*�����Ƿ� ��ǰ��һ������ͬ��KEY*/
        if ( VOS_OK == pstHashTbl->pfHashKeyCmp((void *)pcKeyStr, (void *)pstHashHead->acKeyStr) )
        {
            return pstHashHead->pvKeyData;
        }
        /*ǰ���һ���ڵ��Ѿ��Ƚϣ���һ��next�ڵ㿪ʼ*/
        for( pstHashTmp = VOS_DLIST_ENTRY(pstHashHead->stBlkList.next, CORE_HASHNODE_S, stBlkList);\
                &(pstHashTmp->stBlkList) != &(pstHashHead->stBlkList);\
               pstHashTmp= VOS_DLIST_ENTRY(pstHashTmp->stBlkList.next, CORE_HASHNODE_S, stBlkList))
        {
             /*���������Ƿ�����ͬ��KEY*/
            if ( VOS_OK == pstHashTbl->pfHashKeyCmp((void *)pcKeyStr, (void *)pstHashTmp->acKeyStr) )
            {
                return pstHashTmp->pvKeyData;
            }
        }
    }
    
    return NULL;
}

/*****************************************************************************
 �� �� ��  : core_hashtbl_insert
 ��������  : �����û�����
 �������  : PCORE_HASH_TABLE_S pstHashTbl  
             char *pcKeyStr                 
             void *pvData                   ע����ڴ�һ��Ҫmalloc���룬��Ϊremove�����free�����ͷ�
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2019��3��21��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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
    
    /*��Hash�ڵ��ULONGֵ*/
    pstHashNode->pvKeyData = pvData;
    
    /*�����Hashֵ*/
    ulKeyValue  = pstHashTbl->pfHashKeyCalc((void *)pcKeyStr);
    strcpy(pstHashNode->acKeyStr, pcKeyStr);
    
    /*����ȡ��*/
    ulHashIndex = ulKeyValue % pstHashTbl->ulprime;
    
    if ( NULL == pstHashTbl->ppstArrayHashTbl[ulHashIndex] )
    {
        /*������ڿ��У���ֱ�Ӳ��뼴��*/
        pstHashTbl->ppstArrayHashTbl[ulHashIndex] = pstHashNode;
        return SYS_OK;
    }
    
    /*�ȱȽϵ�һ��ͷ�ڵ㣬�������޳�ͻ*/
    pstHashHead = VOS_DLIST_ENTRY(&(pstHashTbl->ppstArrayHashTbl[ulHashIndex]->stBlkList), CORE_HASHNODE_S, stBlkList);
    if ( VOS_OK == pstHashTbl->pfHashKeyCmp((void *)pcKeyStr, (void *)pstHashHead->acKeyStr ) )
    {
        PrintError("[HSTBL] The key has already exist! keyID=[%s], index=[%d]", pcKeyStr, ulHashIndex);
        
        /*�����ͻ�����ʾ����*/
        free(pstHashNode);
        return SYS_ERR_EXIST;
    }
        
    /*���Ŀǰֻ��һ���ڵ�, ��ô��ֱ�Ӽ����ͻ����*/
    if ( 0 == pstHashHead->ulBlkNum )
    {
        PrintTraceEvent("[HSTBL] hashtable insert empty list succ! hashStr=[%s],keyStr=[%s:%d]", 
                                pstHashHead->acKeyStr, pcKeyStr, ulHashIndex);
        /*ֱ�Ӳ������*/
        VOS_DLIST_ADD_TAIL(&pstHashHead->stBlkList, &pstHashNode->stBlkList);
        pstHashHead->ulBlkNum++;
        return SYS_OK;
    }
    /*���ڶ���ڵ��ͻ�����*/
    else
    {
        PrintTraceWarning("[HSTBL] hashtable insert complict list start! hashStr=[%s],keyStr=[%s:%d]", 
                                pstHashHead->acKeyStr, pcKeyStr, ulHashIndex);
        /*ǰ���һ���ڵ��Ѿ��Ƚϣ���һ��next�ڵ㿪ʼ*/
        for( pstHashTmp = VOS_DLIST_ENTRY(pstHashHead->stBlkList.next, CORE_HASHNODE_S, stBlkList);\
                &(pstHashTmp->stBlkList) != &(pstHashHead->stBlkList);\
               pstHashTmp= VOS_DLIST_ENTRY(pstHashTmp->stBlkList.next, CORE_HASHNODE_S, stBlkList))
        {
            /*���������Ƿ��г�ͻ*/
            if ( VOS_OK == pstHashTbl->pfHashKeyCmp((void *)pcKeyStr, (void *)pstHashTmp->acKeyStr ) )
            {
                PrintError("[HSTBL] hashtable insert has already exist! hashStr=[%s], keyStr=[%s:%d]", 
                                pstHashTmp->acKeyStr, pcKeyStr, ulHashIndex);
                
                /*�����ͻ�����ʾ����, ���ڵ�黹*/
                core_hashnode_back(pstHashTbl, pstHashNode);
                return SYS_ERR_EXIST;
            }
        }
        /*ֱ�Ӳ������*/
        VOS_DLIST_ADD_TAIL(&pstHashHead->stBlkList, &pstHashNode->stBlkList);
        pstHashHead->ulBlkNum++;
        PrintTraceWarning("[HSTBL] hashtable insert complict list succ! hashStr=[%s],keyStr=[%s:%d]", 
                pstHashTmp->acKeyStr, pcKeyStr, ulHashIndex);
    }
    
    return SYS_OK;
}


/*****************************************************************************
 �� �� ��  : core_hashtbl_remove
 ��������  : ɾ���û�����
 �������  : PCORE_HASH_TABLE_S pstHashTbl  
             char *pcKeyStr                 
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2019��3��21��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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
    
    /*�����Hashֵ*/
    ulKeyValue  = pstHashTbl->pfHashKeyCalc((void *)pcKeyStr);
    /*����ȡ��*/
    ulHashIndex = ulKeyValue % pstHashTbl->ulprime;
    
    pstHashHead = pstHashTbl->ppstArrayHashTbl[ulHashIndex];
    if ( NULL == pstHashHead )
    {
        PrintError("[HSTBL] hash table index=[%d:%s] has empty!", ulHashIndex, pcKeyStr);
        /*����*/
        return;
    }
    
    if ( 0 == pstHashHead->ulBlkNum )
    {
        /*�����Ƿ���ͬ*/
        if ( VOS_ERR == pstHashTbl->pfHashKeyCmp(pcKeyStr, pstHashHead->acKeyStr) )
        {
            /*��̫���ܷ�����*/
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
    /*���ڶ���ڵ��ͻ�����*/
    else if ( 0 < pstHashHead->ulBlkNum )
    {
        /*�ȱȽϵ�һ��ͷ�ڵ㣬����������ͬ��KEY*/
        pstHashHead = VOS_DLIST_ENTRY(&(pstHashTbl->ppstArrayHashTbl[ulHashIndex]->stBlkList), CORE_HASHNODE_S, stBlkList);
        
        /*����������ͷ�ڵ��Ƿ�Ϊ����ͬ��KEY*/
        if ( VOS_OK == pstHashTbl->pfHashKeyCmp(pcKeyStr, pstHashHead->acKeyStr) )
        {
            /*�ȼ���һ��*/
            pstHashHead->ulBlkNum--;
            /*�ð���һ���ڵ�����Ϊͷ�ڵ�*/
            pstHashTmp = VOS_DLIST_ENTRY(pstHashHead->stBlkList.next, CORE_HASHNODE_S, stBlkList);
            /*������һ���ڵ��ֵ����ֵ���µ�ͷ�ڵ�*/
            pstHashTmp->ulBlkNum = pstHashHead->ulBlkNum;
            /*���ú��������ڵ�Ϊ�µ�ͷ�ڵ�*/
            pstHashTbl->ppstArrayHashTbl[ulHashIndex] = pstHashTmp;
            /*���Լ�ɾ��*/
            VOS_DLIST_DEL(&pstHashHead->stBlkList);
            
            PrintTraceEvent("[HSTBL] remove the hashNode, hashStr=[%s], keystr=[%s:%d],first clt-list, BlkNums=%d", 
                                pstHashHead->acKeyStr, pcKeyStr, ulHashIndex, pstHashHead->ulBlkNum );
            
            core_hashnode_back(pstHashTbl, pstHashHead);
            
            return;
        }
        
        /*ǰ���һ���ڵ��Ѿ��Ƚϣ�����һ���ڵ㿪ʼ*/
        for( pstHashTmp = VOS_DLIST_ENTRY(pstHashHead->stBlkList.next, CORE_HASHNODE_S, stBlkList);\
                &(pstHashTmp->stBlkList) != &(pstHashHead->stBlkList);\
               pstHashTmp= VOS_DLIST_ENTRY(pstHashTmp->stBlkList.next, CORE_HASHNODE_S, stBlkList))
        {
            /*���������Ƿ�����ͬ��KEY*/
            if ( VOS_OK == pstHashTbl->pfHashKeyCmp(pcKeyStr, pstHashTmp->acKeyStr) )
            {
                /*��Ϊ��������ͷ�ڵ����ģ��ȼ���һ��,
                 ���Ҿ���ǰ����жϣ�ͷ�ڵ�û����ͬ��ɾ���Ĳ���ͷ�ڵ�*/
                pstHashHead->ulBlkNum--;
                
                /*���Լ�ɾ��*/
                VOS_DLIST_DEL(&pstHashTmp->stBlkList);
                
                PrintTraceEvent("[HSTBL] remove the hashNode, hashStr=[%s], keystr=[%s:%d], second clt-list, BlkNums=%d", 
                                pstHashTmp->acKeyStr, pcKeyStr, ulHashIndex, pstHashHead->ulBlkNum );
                
                core_hashnode_back(pstHashTbl, pstHashTmp);
                return;
            }
        }
    }
        
}


/***************************************��Էַ�������д��Hash������㷨********************************************/
/*****************************************************************************
 �� �� ��  : core_hash_key
 ��������  : ngnix��Hash�����
 �������  : uint8_t *pucData  
             int iLen        
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2019��3��26��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
uint32_t core_hash_key(uint8_t *pucData, int32_t iLen)
{
    int  i=0, key=0;

    for (i = 0; i < iLen; i++) {
        key = core_hash(key, pucData[i]);
    }
    
    return key == 0 ? 1 : key;
}



