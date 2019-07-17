/******************************************************************************

                  ��Ȩ���� (C), 2018-2028, �����ѻ��������޹�˾

 ******************************************************************************
  �� �� ��   : core_hash.h
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

#ifndef _CORE_HASH_H_
#define _CORE_HASH_H_

/*�û���������Ϣ*/
typedef struct tagCoreHashData
{   
    /*�û������ڵ�*/
    //��Լ�ռ䣬�ѻ�ϵͳ��ֱ������KeyStr��ΪSID����*/
    //char            acSID[SYS_MAXID_LEN];
    /*�û���������Ψһ�ڵ�ID*/
    dulong          dulRandID;               
    /*��¼���û��Ĺ�����ַ*/
    uint32_t          uiClientAddr;
    /*��¼���û��Ĺ����˿�*/
    uint32_t          uiClientPort;
}CORE_HASHDATA_S, *PCORE_HASHDATA_S;


/*�ṩ���ձ���hash�ڵ�*/
typedef struct tagCoreHashNode
{
    /*�ȿ��Ե�ͷ�ڵ�ĳ�ͻ����Ҳ���Ե������ڵ�*/
    vos_dlist_node_s            stBlkList;
    /*��ͻ������Ŀ*/
    uint32_t                       ulBlkNum;
    /*Hash�ڵ�Key�ַ���ֵ*/
    char                        acKeyStr[SYS_MAXID_LEN];
    /*Hash�ڵ㱣�������*/
    void*                       pvKeyData;
}CORE_HASHNODE_S, *PCORE_HASHNODE_S;


/*�ձ��͵ıȽϺ���*/
typedef int32_t (*pfcore_hash_comp_cb)(const void *pvArgv1, const void *pvArgv2);

/*�ձ��͵�Hashֵ���㺯��*/
typedef uint32_t (*pfcore_hash_calc_cb)(const void *pvData);


/*�ṩ���ձ���hash��*/
typedef struct tagCoreHashTable
{
    /*Hash�������*/
    CORE_HASHNODE_S**           ppstArrayHashTbl;
    /*Hash��ıȽϺ���*/
    pfcore_hash_comp_cb         pfHashKeyCmp;
    /*Hash���ֵ���㺯��*/
    pfcore_hash_calc_cb         pfHashKeyCalc;
    /*ѡȡ�ĺ��ʵ�������С*/
    uint32_t                       ulprime;
    /*���нڵ�����*/
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


