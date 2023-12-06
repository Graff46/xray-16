#pragma once
#include "Car.h"

class Hrebet
{
public:
    Hrebet(CCar* CLcar, IKinematics* K);
    void PhUpdate(float phStep);

    struct SHrebet
    {
        u16 bone_id;
       // LPCSTR bone_name;
        CPhysicsJoint* joint;
        CCar* car;
        float total_mass, saved_lostop, saved_histop, saved_sf, saved_df, saved_sf0, saved_df0;
        bool i_am_locked, i_am_unlocked;
        SHrebet() {};
        SHrebet(CCar* acar, u16 boneId, LPCSTR bonename)
        {
            bone_id = boneId;
            //bone_name = bonename;
            car = acar;
            joint = car->m_pPhysicsShell->get_Joint(bonename);
            total_mass = 0.f;
            saved_lostop = 0.f;
            saved_histop = 0.f;
            saved_sf = 0.f;
            saved_df = 0.f;
            saved_sf0 = 0.f;
            saved_df0 = 0.f;
            i_am_unlocked = true;
            i_am_locked = false;
        }
    };

    xr_vector<SHrebet> m_hrebet_vec;

    private:
        CCar* car;

        float world_gravity;
        float total_mass   ;
        float total_torque ;
        float curr_lo_limit;
        float curr_hi_limit;
};


