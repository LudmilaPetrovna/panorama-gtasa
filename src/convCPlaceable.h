/* based on:
Plugin-SDK (Grand Theft Auto San Andreas) header file
https://github.com/DK22Pac/plugin-sdk
Converted to C struct with perl regular expressions and autism
*/

typedef struct _CPlaceable{
/* 0000 vtable) */ void *vtable;
/*(0x0004 (+16) 1,type=CSimpleTransform) */     CSimpleTransform m_placement;
/*(0x0014 (+4) 1,type=CMatrixLink) */     CMatrix *m_matrix;
/*(0x0018 (+0) 1,type=) */ 
}CPlaceable;
