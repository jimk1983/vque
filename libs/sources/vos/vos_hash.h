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
#ifndef _CORE_HASH_H_
#define _CORE_HASH_H_

/*用户的地址数据信息*/
typedef struct tagVOSHashData
{   
    /*用户所属连接唯一节点ID*/
    DULONG            dulRandID;
    /*记录该用户的公网地址*/
    UINT32_T          uiClientAddr;
    /*记录该用户的公网端口*/
    UINT32_T          uiClientPort;
}VOS_HASHDATA_S, *PVOS_HASHDATA_S;


/*提供的普遍型hash节点*/
typedef struct tagVOSHashNode
{
    /*既可以当头节点的冲突链表，也可以当挂链节点*/
    VOS_DLIST_S             stBlkList;
    /*冲突链表数目*/
    volatile UINT32_T       ulBlkNum;
    /*Hash节点Key字符串值*/
    CHAR                    acKeyStr[SYS_MAXID_LEN];
    /*Hash节点保存的数据*/
    PVOID                   pvKeyData;
}VOS_HASHNODE_S, *PVOS_HASHNODE_S;


/*普遍型的比较函数*/
typedef INT32_T (*pfcore_hash_comp_cb)(const void *pvArgv1, const void *pvArgv2);

/*普遍型的Hash值计算函数*/
typedef UINT32_T (*pfcore_hash_calc_cb)(const void *pvData);


/*提供的普遍型hash表*/
typedef struct tagVosHashTable
{
    /*Hash表的数组*/
    VOS_HASHNODE_S**            ppstArrayHashTbl;
    /*选取的合适的素数大小*/
    UINT32_T                    ulprime;
    /*空闲节点链表*/
    VOS_DLIST_S                 stIdleList;
    /*Hash表的比较函数*/
    pfcore_hash_comp_cb         pfHashKeyCmp;
    /*Hash表的值计算函数*/
    pfcore_hash_calc_cb         pfHashKeyCalc;
}CORE_HASH_TABLE_S, *PCORE_HASH_TABLE_S;


CORE_HASH_TABLE_S *VOS_HashTbl_Create(IN UINT32_T ulSize, IN pfcore_hash_comp_cb pfHashCmp, IN pfcore_hash_calc_cb pfHashCalc);

VOID        VOS_HashTbl_Release(INOUT PCORE_HASH_TABLE_S* ppstHashTbl);

INT32_T     VOS_HashTbl_Insert(IN PCORE_HASH_TABLE_S pstHashTbl, IN      const CHAR *pcKeyStr, IN VOID *pvData);

VOID        VOS_HashTbl_Remove(IN PCORE_HASH_TABLE_S pstHashTbl, IN      const CHAR *pcKeyStr);

PVOID       VOS_HashTbl_Find(IN PCORE_HASH_TABLE_S pstHashTbl, IN const CHAR *pcKeyStr);

UINT32_T    VOS_HashStr_Dbj(IN const CHAR *str);

UINT32_T    VOS_HashStr_Normal(IN const CHAR *pcString);

UINT32_T    VOS_HashVal_key(IN const UCHAR *puckey, INT32_T iLen);

#endif


