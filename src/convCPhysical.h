typedef struct _CPhysical{
CEntity PARENT;
/*(0x0038 (+4) 1,type=int) */     int field_38;
/*(0x003c (+4) 1,type=int) */     unsigned int m_nLastCollisionTime;
/*(0x0040 (+4) 1,type=int) */ int flags;
/*(0x0044 (+12) 1,type=CVector) */     CVector          m_vecMoveSpeed;
/*(0x0050 (+12) 1,type=CVector) */     CVector          m_vecTurnSpeed;
/*(0x005c (+12) 1,type=CVector) */     CVector          m_vecFrictionMoveSpeed;
/*(0x0068 (+12) 1,type=CVector) */     CVector          m_vecFrictionTurnSpeed;
/*(0x0074 (+12) 1,type=CVector) */     CVector          m_vecForce;
/*(0x0080 (+12) 1,type=CVector) */     CVector          m_vecTorque;
/*(0x008c (+4) 1,type=float) */     float            m_fMass;
/*(0x0090 (+4) 1,type=float) */     float            m_fTurnMass;
/*(0x0094 (+4) 1,type=float) */     float            m_fVelocityFrequency;
/*(0x0098 (+4) 1,type=float) */     float            m_fAirResistance;
/*(0x009c (+4) 1,type=float) */     float            m_fElasticity;
/*(0x00a0 (+4) 1,type=float) */     float            m_fBuoyancyConstant;
/*(0x00a4 (+12) 1,type=CVector) */     CVector          m_vecCentreOfMass;
/*(0x00b0 (+4) 1,type=void) */     void            *m_pCollisionList;
/*(0x00b4 (+4) 1,type=void) */     void            *m_pMovingList;
/*(0x00b8 (+1) 1,type=char) */     char field_B8;
/*(0x00b9 (+1) 1,type=char) */     unsigned char    m_nNumEntitiesCollided;
/*(0x00ba (+1) 1,type=char) */     unsigned char    m_nContactSurface;
/*(0x00bb (+1) 1,type=char) */     char field_BB;
/*(0x00bc (+24) 1,type=CEntity) */     CEntity         *m_apCollidedEntities[6];
/*(0x00d4 (+4) 1,type=float) */     float            m_fMovingSpeed; 
/*(0x00d8 (+4) 1,type=float) */     float            m_fDamageIntensity;
/*(0x00dc (+4) 1,type=CEntity) */     CEntity         *m_pDamageEntity;
/*(0x00e0 (+12) 1,type=CVector) */     CVector          m_vecLastCollisionImpactVelocity;
/*(0x00ec (+12) 1,type=CVector) */     CVector          m_vecLastCollisionPosn;
/*(0x00f8 (+2) 1,type=short) */     unsigned short   m_nPieceType;
/*(0x00fa (+2) 1,type=short) */     short field_FA;
/*(0x00fc (+4) 1,type=class) */     CPhysical *m_pAttachedTo;
/*(0x0100 (+12) 1,type=CVector) */     CVector          m_vecAttachOffset;
/*(0x010c (+12) 1,type=CVector) */     CVector          m_vecAttachedEntityPosn;
/*(0x0118 (+16) 1,type=CQuaternion) */     CQuaternion      m_qAttachedEntityRotation;
/*(0x0128 (+4) 1,type=CEntity) */     CEntity         *m_pEntityIgnoredCollision;
/*(0x012c (+4) 1,type=float) */     float            m_fContactSurfaceBrightness;
/*(0x0130 (+4) 1,type=float) */     float            m_fDynamicLighting;
/*(0x0134 (+4) 1,type=CRealTimeShadow) */     void *m_pShadowData;
/*(0x0138 (+0) 1,type=) */     
/*(0x0138 (+0) 1,type=) */     
}CPhysical;