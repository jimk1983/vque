/******************************************************************************

                  版权所有 (C), 2018-2028, 杭州友机技术有限公司

 ******************************************************************************
  文 件 名   : core_hash.h
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

#ifndef _CORE_HASH_H_
#define _CORE_HASH_H_

/*用户的数据信息*/
typedef struct tagCoreHashData
{   
    /*用户所属节点*/
    //节约空间，友机系统中直接利用KeyStr作为SID即可*/
    //char            acSID[SYS_MAXID_LEN];
    /*用户所属连接唯一节点ID*/
    dulong          dulRandID;               
    /*记录该用户的公网地址*/
    uint32_t          uiClientAddr;
    /*记录该用户的公网端口*/
    uint32_t          uiClientPort;
}CORE_HASHDATA_S, *PCORE_HASHDATA_S;


/*提供的普遍型hash节点*/
typedef struct tagCoreHashNode
{
    /*既可以当头节点的冲突链表，也可以当挂链节点*/
    vos_dlist_node_s            stBlkList;
    /*冲突链表数目*/
    uint32_t                       ulBlkNum;
    /*Hash节点Key字符串值*/
    char                        acKeyStr[SYS_MAXID_LEN];
    /*Hash节点保存的数据*/
    void*                       pvKeyData;
}CORE_HASHNODE_S, *PCORE_HASHNODE_S;


/*普遍型的比较函数*/
typedef int32_t (*pfcore_hash_comp_cb)(const void *pvArgv1, const void *pvArgv2);

/*普遍型的Hash值计算函数*/
typedef uint32_t (*pfcore_hash_calc_cb)(const void *pvData);


/*提供的普遍型hash表*/
typedef struct tagCoreHashTable
{
    /*Hash表的数组*/
    CORE_HASHNODE_S**           ppstArrayHashTbl;
    /*Hash表的比较函数*/
    pfcore_hash_comp_cb         pfHashKeyCmp;
    /*Hash表的值计算函数*/
    pfcore_hash_calc_cb         pfHashKeyCalc;
    /*选取的合适的素数大小*/
    uint32_t                       ulprime;
    /*空闲节点链表*/
    vos_dlist_node_s            stIdleList;
}CORE_HASH_TABLE_S, *PCORE_HASH_TABLE_S;


CORE_HASH_TABLE_S *core_hashtbl_create(IN uint32_t ulSize, IN pfcore_hash_comp_cb pfHashCmp, IN pfcore_hash_calc_cb pfHashCalc);

void    core_hashtbl_release(INOUT PCORE_HASH_TABLE_S* ppstHashTbl);

int32_t    core_hashtbl_insert(PCORE_HASH_TABLE_S pstHashTbl, char *pcKeyStr, void *pvData);

void    core_hashtbl_remove(PCORE_HASH_TABLE_S pstHashTbl, char *pcKeyStr);

void*   core_hashtbl_find(PCORE_HASH_TABLE_S pstHashTbl, char *pcKeyStr);

uint32_t   core_hashstr_dbj(IN const char *str);

uint32_t   core_hashstr_normal(IN const char *pcString);

uint32_t  core_hash_key(uint8_t *pucData, int32_t iLen);

#endif


