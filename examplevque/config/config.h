

typedef struct tagExampleClient
{
    UINT32_T    TypeCode;
    UINT32_T    HeadOffset;
    UINT32_T    BodySize;
    CHAR        acServerAddr[SYS_IPV4LEN];
    UINT32_T    ServerPort;
    UINT32_T    PthNums;
    UINT32_T    PthPerConns;
}TEST_CLNT_CFG_S,*PTEST_CLNT_CFG_S;


typedef struct tagExampleServer
{
    UINT32_T    TypeCode;
    UINT32_T    HeadOffset;
    CHAR        acServerAddr[SYS_IPV4LEN];
    UINT32_T    ServerPort;
    UINT32_T    DirectEcho;

    UINT32_T    ForwardEnable;
    CHAR        acForwardAddr[SYS_IPV4LEN];
    UINT32_T    ForwardPort;
    
}TEST_SEVR_CFG_S,*PTEST_SEVR_CFG_S;


typedef struct tagMessageQueue
{
    UINT32_T    TypeCode;
    UINT32_T    ProducerPths;
    UINT32_T    ConsumerPths;
    UINT32_T    PthFreqUs;
    UINT32_T    PthBatchs;
}TEST_MSQ_CFG_S, *PTEST_MSQ_CFG_S;


typedef struct tagExampleConfig
{
    CHAR        acEthName[SYS_MAXID_LEN];
    UINT32_T    CurrTypeCode;
    
    /**1. Client mode*/
    TEST_CLNT_CFG_S     stClntCfg;
    /**2. Server mode*/
    TEST_SEVR_CFG_S     stServCfg;
    /**3. Message Queue mode*/
    TEST_MSQ_CFG_S      stMsqCfg;
    
}TEST_CONF_S,*PTEST_CONF_S;

VOID config_init(PTEST_CONF_S      pstCfg);

