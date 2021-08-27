#pragma once
#include "inventory_space.h"
#include "GameObject.h"

class CInventoryBox : public CGameObject
{
    typedef CGameObject inherited;

public:
    xr_vector<u16> m_items;

protected:
    bool m_in_use;
    bool m_can_take;
    bool m_closed;
	
	//-> i-love-kfc: Ограничение объема из Unexplored Land
	u8 box_cells_x;
	u8 box_cells_y;

public:
    CInventoryBox();
    virtual ~CInventoryBox();

    virtual void OnEvent(NET_Packet& P, u16 type);
    virtual bool net_Spawn(CSE_Abstract* DC);
    virtual void net_Destroy();
    virtual void net_Relcase(IGameObject* O);
    void AddAvailableItems(TIItemContainer& items_container) const;
    IC bool IsEmpty() const { return m_items.empty(); }
    virtual void UpdateCL();

    IC void set_in_use(bool status) { m_in_use = status; }
    IC bool in_use() const { return m_in_use; }
    void set_can_take(bool status);
    IC bool can_take() const { return m_can_take; }
    void set_closed(bool status, LPCSTR reason);
    IC bool closed() const { return m_closed; }
	
	//-> i-love-kfc: Ограничение объема из Unexplored Land
	IC			u8		get_box_capacity_x				() const { return box_cells_x; }
	IC			u8		get_box_capacity_y				() const { return box_cells_y; }
protected:
    void SE_update_status();
};
