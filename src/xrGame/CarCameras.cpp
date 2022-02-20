#include "StdAfx.h"
#pragma hdrstop
#ifdef DEBUG

#include "PHDebug.h"
#include "xrPhysics/IPHWorld.h"
#endif
#include "alife_space.h"
#include "Hit.h"
#include "PHDestroyable.h"
#include "Car.h"
#include "Actor.h"
#include "CameraLook.h"
#include "CameraFirstEye.h"
#include "Level.h"
#include "xrEngine/CameraManager.h"

bool CCar::HUDView() const { return active_camera->tag == ectFirst; }
void CCar::cam_Update(float dt, float fov)
{
    VERIFY(!physics_world()->Processing());
    Fvector P, Da;
    Da.set(0, 0, 0);
    // bool							owner = !!Owner();

    XFORM().transform_tiny(P, m_camera_current_position);

    if (active_camera->tag == ectFirst)
    {
        // rotate head
        if (OwnerActor())
        {
            OwnerActor()->Orientation().yaw = -active_camera->yaw;
            OwnerActor()->Orientation().pitch = -active_camera->pitch;
        }
            
    }
    active_camera->f_fov = fov;
    active_camera->Update(P, Da);
    Level().Cameras().UpdateFromCamera(active_camera);
}

void CCar::OnCameraChange(int type)
{
    if (Owner())
    {
        if (type == ectFirst)
        {
            Owner()->setVisible(FALSE);
            m_camera_current_position = m_camera_position;
        }
            
        else if (active_camera && active_camera->tag == ectFirst)
        {
            Owner()->setVisible(TRUE);
            m_camera_current_position = m_camera_position2;
        }
            
    }

    if (!active_camera || active_camera->tag != type)
    {
        active_camera = camera[type];
        if (ectFree == type)
        {
            Fvector xyz;
            XFORM().getXYZi(xyz);
            active_camera->yaw = xyz.y;
        }
    }
}
