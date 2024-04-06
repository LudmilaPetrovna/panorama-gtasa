/* based on:
Plugin-SDK (Grand Theft Auto San Andreas) header file
https://github.com/DK22Pac/plugin-sdk
Converted to C struct with perl regular expressions and autism
*/

typedef struct _CQueuedMode{
/*(0x0000 (+2) 1,type=short) */     unsigned short m_nMode;
/*(0x0002 (+2) 1,type=char) */ char pad[2];
/*(0x0004 (+4) 1,type=float) */     float m_fDuration;
/*(0x0008 (+2) 1,type=short) */     unsigned short m_nMinZoom;
/*(0x000a (+2) 1,type=short) */     unsigned short m_nMaxZoom;
}CQueuedMode;
