/* based on:
Plugin-SDK (Grand Theft Auto San Andreas) header file
https://github.com/DK22Pac/plugin-sdk
Converted to C struct with perl regular expressions and autism
*/

typedef struct _CEntity{
CPlaceable PARENT;
/*(0x0018 (+4) 1,type=int) */         void *m_pRwObject;
/*(0x001c (+0.125) 0.125,type=int) */     unsigned int m_bUsesCollision : 1;           
/*(0x001c (+0.125) 0.125,type=int) */     unsigned int m_bCollisionProcessed : 1;  
/*(0x001c (+0.125) 0.125,type=int) */     unsigned int m_bIsStatic : 1;                
/*(0x001c (+0.125) 0.125,type=int) */     unsigned int m_bHasContacted : 1;            
/*(0x001c (+0.125) 0.125,type=int) */     unsigned int m_bIsStuck : 1;             
/*(0x001c (+0.125) 0.125,type=int) */     unsigned int m_bIsInSafePosition : 1;        
/*(0x001c (+0.125) 0.125,type=int) */     unsigned int m_bWasPostponed : 1;            
/*(0x001c (+0.125) 0.125,type=int) */     unsigned int m_bIsVisible : 1;               
/*(0x001d (+0) 1,type=) */  
/*(0x001d (+0.125) 0.125,type=int) */     unsigned int m_bIsBIGBuilding : 1;           
/*(0x001d (+0.125) 0.125,type=int) */     unsigned int m_bRenderDamaged : 1;           
/*(0x001d (+0.125) 0.125,type=int) */     unsigned int m_bStreamingDontDelete : 1; 
/*(0x001d (+0.125) 0.125,type=int) */     unsigned int m_bRemoveFromWorld : 1;     
/*(0x001d (+0.125) 0.125,type=int) */     unsigned int m_bHasHitWall : 1;              
/*(0x001d (+0.125) 0.125,type=int) */     unsigned int m_bImBeingRendered : 1;     
/*(0x001d (+0.125) 0.125,type=int) */     unsigned int m_bDrawLast :1;             
/*(0x001d (+0.125) 0.125,type=int) */     unsigned int m_bDistanceFade :1;         
/*(0x001e (+0) 1,type=) */  
/*(0x001e (+0.125) 0.125,type=int) */     unsigned int m_bDontCastShadowsOn : 1;       
/*(0x001e (+0.125) 0.125,type=int) */     unsigned int m_bOffscreen : 1;               
/*(0x001e (+0.125) 0.125,type=int) */     unsigned int m_bIsStaticWaitingForCollision : 1; 
/*(0x001e (+0.125) 0.125,type=int) */     unsigned int m_bDontStream : 1;              
/*(0x001e (+0.125) 0.125,type=int) */     unsigned int m_bUnderwater : 1;              
/*(0x001e (+0.125) 0.125,type=int) */     unsigned int m_bHasPreRenderEffects : 1; 
/*(0x001e (+0.125) 0.125,type=int) */     unsigned int m_bIsTempBuilding : 1;          
/*(0x001e (+0.125) 0.125,type=int) */     unsigned int m_bDontUpdateHierarchy : 1; 
/*(0x001f (+0) 1,type=) */  
/*(0x001f (+0.125) 0.125,type=int) */     unsigned int m_bHasRoadsignText : 1;     
/*(0x001f (+0.125) 0.125,type=int) */     unsigned int m_bDisplayedSuperLowLOD : 1;
/*(0x001f (+0.125) 0.125,type=int) */     unsigned int m_bIsProcObject : 1;            
/*(0x001f (+0.125) 0.125,type=int) */     unsigned int m_bBackfaceCulled : 1;          
/*(0x001f (+0.125) 0.125,type=int) */     unsigned int m_bLightObject : 1;         
/*(0x001f (+0.125) 0.125,type=int) */     unsigned int m_bUnimportantStream : 1;       
/*(0x001f (+0.125) 0.125,type=int) */     unsigned int m_bTunnel : 1;          
/*(0x001f (+0.125) 0.125,type=int) */     unsigned int m_bTunnelTransition : 1;        
/*(0x0020 (+0) 1,type=) */     
/*(0x0020 (+2) 1,type=short) */     unsigned short m_nRandomSeed;
/*(0x0022 (+2) 1,type=short) */     unsigned short m_nModelIndex;
/*(0x0024 (+4) 1,type=CReference) */     void *m_pReferences;
/*(0x0028 (+4) 1,type=void) */     void *m_pStreamingLink;
/*(0x002c (+2) 1,type=short) */     short m_nScanCode;
/*(0x002e (+1) 1,type=char) */     char m_nIplIndex;
/*(0x002f (+1) 1,type=char) */     unsigned char m_nAreaCode;
/*(0x0030 (+4) 1,type=CEntity) */         CEntity *m_pLod;
/*(0x0034 (+1) 1,type=char) */     unsigned char m_nNumLodChildren;
/*(0x0035 (+1) 1,type=char) */     unsigned char m_nNumLodChildrenRendered;
/*(0x0036 (+0.375) 0.375,type=char) */     unsigned char m_nType : 3; 
/*(0x0036 (+0.625) 0.625,type=char) */     unsigned char m_nStatus : 5; 
/*(0x0037 (+0) 1,type=) */     
/*(0x0037 (+0) 1,type=) */     
char PADDING_END[1]; /* must be 56 as size of CEntity */ 
}CEntity;
