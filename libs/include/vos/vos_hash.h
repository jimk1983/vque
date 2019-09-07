/******************************************************************************

                  ��Ȩ���� (C), 2018-2028, �������޹�˾

 ******************************************************************************
  �� �� ��   : vos_hash.c
  �� �� ��   : ����
  ��    ��   : ����
  ��������   : 2016��2��15��
  ����޸�   :
  ��������   : �򵥵�string��Hash���ʵ��
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��2��15��
    ��    ��   : ����
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef _CORE_HASH_H_
#define _CORE_HASH_H_

/*�û��ĵ�ַ������Ϣ*/
typedef struct tagVOSHashData
{   
    /*�û���������Ψһ�ڵ�ID*/
    DULONG            dulRandID;
    /*��¼���û��Ĺ�����ַ*/
    UINT32_T          uiClientAddr;
    /*��¼���û��Ĺ����˿�*/
    UINT32_T          uiClientPort;
}VOS_HASHDATA_S, *PVOS_HASHDATA_S;


/*�ṩ���ձ���hash�ڵ�*/
typedef struct tagVOSHashNode
{
    /*�ȿ��Ե�ͷ�ڵ�ĳ�ͻ����Ҳ���Ե������ڵ�*/
    VOS_DLIST_S             stBlkList;
    /*��ͻ������Ŀ*/
    volatile UINT32_T       ulBlkNum;
    /*Hash�ڵ�Key�ַ���ֵ*/
    CHAR                    acKeyStr[SYS_MAXID_LEN];
    /*Hash�ڵ㱣�������*/
    PVOID                   pvKeyData;
}VOS_HASHNODE_S, *PVOS_HASHNODE_S;


/*�ձ��͵ıȽϺ���*/
typedef INT32_T (*pfcore_hash_comp_cb)(const void *pvArgv1, const void *pvArgv2);

/*�ձ��͵�Hashֵ���㺯��*/
typedef UINT32_T (*pfcore_hash_calc_cb)(const void *pvData);


/*�ṩ���ձ���hash��*/
typedef struct tagVosHashTable
{
    /*Hash�������*/
    VOS_HASHNODE_S**            ppstArrayHashTbl;
    /*ѡȡ�ĺ��ʵ�������С*/
    UINT32_T                    ulprime;
    /*���нڵ�����*/
    VOS_DLIST_S                 stIdleList;
    /*Hash��ıȽϺ���*/
    pfcore_hash_comp_cb         pfHashKeyCmp;
    /*Hash���ֵ���㺯��*/
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


