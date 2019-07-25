/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : vos_utils.h
  �� �� ��   : ����
  ��    ��   : jimk
  ��������   : 2016��1��2��
  ����޸�   :
  ��������   : ͨ�ù���ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��1��2��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/
/*----------------------------------------------*
 * ���õĺ궨��                                       *
 *----------------------------------------------*/
/*IPV4ȫ����ַ�ַ�����󳤶�*/
#define VOS_IPV4STR_MAXLEN      16
/*IPV4��ַ�����ַ�������*/
#define VOS_IPADDR_DOILEN       4

#define VOS_GUID_LEN            64
     
/*����32λ�����ַ�������*/
#define VOS_INT32STR_MAXLEN     12

/*һ�㳣���ļ����ĳ���*/
#define VOS_FILE_MAXLEN         256

/*��������*/
#define VOS_URL_MAXLEN          128

BOOL_T      VOS_CpuIsBigEndian();
UINT32_T    VOS_GetSelfTId();
UINT32_T    VOS_GetSelfPId();
UINT32_T    VOS_atoi(CHAR *pcStr);
VOID        VOS_itoa(CHAR *pcStr, UINT32_T iVal);
VOID        VOS_hextoa(CHAR *pcStr, INT8_T iVal);
UINT16_T    VOS_Random16();
UINT32_T    VOS_Random32();
VOID        VOS_GenRandom64(DULONG *pstRand64);
VOID        VOS_GenUUid(CHAR *pcUUid, INT32_T ulLen );
VOID        VOS_PrintHex(CHAR *pcBuf, UINT32_T BufLen, CHAR *pcData, UINT32_T DataLen);


