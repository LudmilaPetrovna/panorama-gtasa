typedef struct _FxManager_c{
/*(0x0000 (+12) 1,type=TList_c) */ 	char m_fxSystemBPList[12];
/*(0x000c (+12) 1,type=TList_c) */  char m_fxSystemList[12];
/*(0x0018 (+4) 1,type=FxEmitterPrt_c) */ 	void *        m_pFxEmitters;
/*(0x001c (+12) 1,type=TList_c) */ 	char m_fxEmitterPrtList[12];
/*(0x0028 (+4) 1,type=int) */ 	int                     m_nFxTxdIndex;
/*(0x002c (+4) 1,type=RwV3d) */ 	void *                 m_pWindDir;
/*(0x0030 (+4) 1,type=float) */ 	float                  m_pfWindSpeed;
/*(0x0034 (+84) 1,type=FxFrustumInfo_c) */ 	char m_frustum[84];
/*(0x0088 (+4) 1,type=int) */ 	unsigned int            m_nCurrentMatrix;
/*(0x008c (+32) 1,type=RwMatrix) */ 	void *              m_apMatrices[8];
/*(0x00ac (+12) 1,type=FxMemoryPool_c) */ 	char m_pool[12];
/*(0x00b8 (+0) 1,type=) */ 
}FxManager_c;
