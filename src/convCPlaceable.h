/* based on:
Plugin-SDK (Grand Theft Auto San Andreas) header file
https://github.com/DK22Pac/plugin-sdk
Converted to C struct with perl regular expressions and autism
*/

typedef struct _CPlaceable{
/*(0x0000 (+16) 1,type=CSimpleTransform) */     CSimpleTransform m_placement;
/*(0x0010 (+4) 1,type=CMatrixLink) */     void *m_matrix;
/*(0x0014 (+0) 1,type=) */ 
char PADDING_END[4]; /* must be 24 as size of CPlaceable */ 
}CPlaceable;
