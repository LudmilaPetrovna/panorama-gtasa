/* based on:
Plugin-SDK (Grand Theft Auto San Andreas) header file
https://github.com/DK22Pac/plugin-sdk
Converted to C struct with perl regular expressions and autism
*/

typedef struct _CMatrix{
/*(0x0000 (+0) 1,type=) */     
/*(0x0000 (+12) 1,type=CVector) */     CVector      right;
/*(0x000c (+4) 1,type=int) */     unsigned int flags;
/*(0x0010 (+12) 1,type=CVector) */     CVector      up;
/*(0x001c (+4) 1,type=int) */     unsigned int pad1;
/*(0x0020 (+12) 1,type=CVector) */     CVector      at;
/*(0x002c (+4) 1,type=int) */     unsigned int pad2;
/*(0x0030 (+12) 1,type=CVector) */     CVector      pos;
/*(0x003c (+4) 1,type=int) */     unsigned int pad3;
/*(0x0040 (+0) 1,type=) */     
/*(0x0040 (+4) 1,type=RwMatrix) */ 	void *m_pAttachMatrix;
/*(0x0044 (+1) 1,type=char) */ 	char m_bOwnsAttachedMatrix; 
/*(0x0045 (+0) 1,type=) */ 
char PADDING_END[3]; /* must be 72 as size of CMatrix */ 
}CMatrix;
