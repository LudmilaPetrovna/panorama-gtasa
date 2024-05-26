#include <stdint.h>
#include "ApiGtaSA.h"
#include "SpawnObject.h"
#include "Panorama.h"


void spawnBox(){
static int trashbox[]={1220,1221,1230,926,928};
CVector *pos=getPlayerVector();
spawnObject(trashbox[(int)(drand()*5.0)],pos->x,pos->y,pos->z+3.0);
MessageJumpQ("box spawned", 1000, 0, false);
}

void spawnObject(int id, float x, float y, float z){
/*
0247(583)                REQUEST_MODEL iModel
038B(907)                LOAD_ALL_MODELS_NOW
0107(263)                CREATE_OBJECT iModel x y z (hObj)
0249(585)                MARK_MODEL_AS_NO_LONGER_NEEDED iModel
08D2(2258)               SET_OBJECT_SCALE hObj fScale
*/
CStreaming__RequestModel(id, 12);
CTimer__Suspend();
CStreaming__LoadAllRequestedModels(0);
CTimer__Resume();
CObject *h=CObject__Create(id);
CEntity *e=&h->PARENT.PARENT;
CVector pos;
pos.x=x;
pos.y=y;
pos.z=z;

h->PARENT.m_fMass=20.0;
h->PARENT.m_vecForce.x=drand()-.5;
h->PARENT.m_vecForce.y=drand()-.5;
h->PARENT.m_vecForce.z=3.0;

h->PARENT.PARENT.m_bUsesCollision=1;
h->PARENT.PARENT.m_bIsStatic=0;
h->m_fScale=3.0;

CPlaceable__setPosition(&e->PARENT,&pos);
//      CPlaceable::SetOrientation(&v21->physical.entity.placeable, 0.0, 0.0, 0.0);
//Entity__UpdateRW_(&h->physical.entity);
//CEntity__UpdateRwFrame(&h->physical.entity);
//???  CMissionCleanup::AddEntityToList(&CTheScripts::MissionCleanUp, handle, 3);
CWorld__Add(e);
CStreaming__SetMissionDoesntRequireModel(id);
//CPool_CObject___AtHandle(h)->m_fScale=3.0;
}


/*
void deleteObject(int h){
//0108(264)    DELETE_OBJECT hObj
  v23 = CPool<CObject>::AtHandle(handle);
      p_entity = &v23->physical.entity;
      if ( v23 )
      {
        CWorld::Remove(&v23->physical.entity);
        CWorld::RemoveReferencesToDeletedObject(p_entity);
        p_entity->placeable.vtbl->Destructor(p_entity, 1);
      }
}
*/