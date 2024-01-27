#include "stdafx.h"
#include "Hrebet.h"
#include <xrPhysics/IPHWorld.h>

Hrebet::Hrebet(CCar* CLcar, IKinematics* K)
{
    car = CLcar;

    world_gravity   = physics_world()->Gravity();
    total_mass      = car->m_pPhysicsShell->getMass();
    total_torque    = total_mass + world_gravity;

    b_shift = false;

    xr_vector<LPCSTR> boneVec = {
        "root.001",
        "root.002",
        "root.003",
        "root.004",
        "root.005",
        "root.006",
        "root.007",
        "root.008"
    };

    SHrebet hrebet;

    for (LPCSTR& name : boneVec)
    {
        hrebet = SHrebet(car, K->LL_BoneID(name), name);

        if (hrebet.joint && hrebet.joint->bActive && hrebet.joint->PSecond_element())
        {
            hrebet.joint->GetLimits(hrebet.saved_lostop, hrebet.saved_histop, 0);
            hrebet.joint->PSecond_element()->SetAirResistance(0, 0);

            hrebet.joint->GetJointSDfactors(hrebet.saved_sf, hrebet.saved_df);

            hrebet.joint->GetAxisSDfactors(hrebet.saved_sf0, hrebet.saved_df0, 0);

            hrebet.total_mass = hrebet.joint->PFirst_element()->getMass();
            for (u16 i = hrebet.bone_id; i < K->LL_BoneCount(); ++i)
            {
                const CBoneData& bone_data = K->LL_GetData(i);
                if (!bone_data.shape.flags.test(SBoneShape::sfNoPhysics))
                    hrebet.total_mass += bone_data.mass;
            }
        }

        m_hrebet_vec.push_back(hrebet);
    }

    //K->LL_SetBoneVisible(K->LL_BoneID("kardan1"), 0, 0);
    //K->LL_SetBoneVisible(K->LL_BoneID("kardan2"), 0, 0);
}

void Hrebet::PhUpdate(float phStep)
{
    for (SHrebet& hrebet : m_hrebet_vec)
    {
        if (hrebet.joint && hrebet.joint->bActive && hrebet.joint->PSecond_element())
        {
            //hrebet.joint->SetJointSDfactors(hrebet.saved_sf, hrebet.saved_df);

            //hrebet.joint->SetAxisSDfactors(hrebet.saved_sf0, hrebet.saved_df0, 0);

            //bool b_shift = pInput->iGetAsyncKeyState(SDL_SCANCODE_RSHIFT) && !hrebet.i_am_locked;
            bool b_rctrl = false;//pInput->iGetAsyncKeyState(SDL_SCANCODE_RCTRL) && !hrebet.i_am_unlocked;

            hrebet.joint->GetLimits(curr_lo_limit, curr_hi_limit, 0);

            float curr_angle = hrebet.joint->GetAxisAngle(0);
            clamp(curr_angle, hrebet.saved_lostop, hrebet.saved_histop);

            float angles_diff = angle_difference_signed(b_shift ? curr_lo_limit : 0.0f, curr_angle);
            float target_vel = (angles_diff * (total_mass + world_gravity) * phStep);

            if (!b_rctrl && !b_shift) // заблокировать в 0
            {
                if (curr_angle == hrebet.saved_histop) // заблокировано
                {
                    hrebet.joint->SetLimits(hrebet.saved_histop, hrebet.saved_histop, 0);
                    hrebet.joint->SetForceAndVelocity(0,0,0);
                    hrebet.i_am_unlocked = true;hrebet.i_am_locked = false;
                }
                else//пытаемся блокировать
                    hrebet.joint->SetForceAndVelocity(total_torque, target_vel, 0);
            }

            if (b_rctrl && !hrebet.i_am_unlocked) // расправить
            {
                hrebet.joint->SetLimits(hrebet.saved_lostop, hrebet.saved_histop, 0);
                hrebet.joint->SetForceAndVelocity(total_torque/50, target_vel/20, 0);
            }
            if (b_shift) // сжать
            {
                if (_abs(curr_angle) >= _abs(hrebet.saved_lostop) - 0.05f)//зажали и заблокировали
                {
                    hrebet.joint->SetLimits(hrebet.saved_lostop, hrebet.saved_lostop, 0);
                    hrebet.joint->SetForceAndVelocity(0, 0, 0);
                    hrebet.i_am_locked = true;hrebet.i_am_unlocked = false;
                }
                else//пытаемся сжать
                {
                    hrebet.joint->SetLimits(hrebet.saved_lostop, hrebet.saved_histop, 0);
                    hrebet.joint->SetForceAndVelocity(total_torque, target_vel/2, 0);
                }
            }
        }
    }
}
