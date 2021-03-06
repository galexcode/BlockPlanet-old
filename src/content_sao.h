/*
Minetest-c55
Copyright (C) 2010-2011 celeron55, Perttu Ahola <celeron55@gmail.com>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#ifndef CONTENT_SAO_HEADER
#define CONTENT_SAO_HEADER

#include "serverobject.h"
#include "content_object.h"
#include "itemgroup.h"
#include "player.h"
#include "object_properties.h"

ServerActiveObject* createItemSAO(ServerEnvironment *env, v3f pos,
		const std::string itemstring);

/*
	LuaEntitySAO needs some internals exposed.
*/

class LuaEntitySAO : public ServerActiveObject
{
public:
	LuaEntitySAO(ServerEnvironment *env, v3f pos,
			const std::string &name, const std::string &state);
	~LuaEntitySAO();
	u8 getType() const
	{ return ACTIVEOBJECT_TYPE_LUAENTITY; }
	u8 getSendType() const
	{ return ACTIVEOBJECT_TYPE_GENERIC; }
	virtual void addedToEnvironment();
	static ServerActiveObject* create(ServerEnvironment *env, v3f pos,
			const std::string &data);
	void step(float dtime, bool send_recommended);
	std::string getClientInitializationData();
	std::string getStaticData();
	int punch(v3f dir,
			const ToolCapabilities *toolcap=NULL,
			ServerActiveObject *puncher=NULL,
			float time_from_last_punch=1000000);
	void rightClick(ServerActiveObject *clicker);
	void setPos(v3f pos);
	void moveTo(v3f pos, bool continuous);
	float getMinimumSavedMovement();
	std::string getDescription();
	void setHP(s16 hp);
	s16 getHP() const;
	void setArmorGroups(const ItemGroupList &armor_groups);
	ObjectProperties* accessObjectProperties();
	void notifyObjectPropertiesModified();
	/* LuaEntitySAO-specific */
	void setVelocity(v3f velocity);
	v3f getVelocity();
	void setAcceleration(v3f acceleration);
	v3f getAcceleration();
	void setPitch(float pitch);
	float getPitch();
	void setYaw(float yaw);
	float getYaw();
	void setTextureMod(const std::string &mod);
	void setSprite(v2s16 p, int num_frames, float framelength,
			bool select_horiz_by_yawpitch);
	std::string getName();
private:
	std::string getPropertyPacket();
	void sendPosition(bool do_interpolate, bool is_movement_end);

	std::string m_init_name;
	std::string m_init_state;
	bool m_registered;
	struct ObjectProperties m_prop;
	
	s16 m_hp;
	v3f m_velocity;
	v3f m_acceleration;
	float m_pitch;
	float m_yaw;
	ItemGroupList m_armor_groups;
	
	bool m_properties_sent;
	float m_last_sent_yaw;
	v3f m_last_sent_position;
	v3f m_last_sent_velocity;
	float m_last_sent_position_timer;
	float m_last_sent_move_precision;
	bool m_armor_groups_sent;
};

/*
	PlayerSAO needs some internals exposed.
*/

class PlayerSAO : public ServerActiveObject
{
public:
	PlayerSAO(ServerEnvironment *env_, Player *player_, u16 peer_id_,
			const std::set<std::string> &privs, bool is_singleplayer);
	~PlayerSAO();
	u8 getType() const
	{ return ACTIVEOBJECT_TYPE_PLAYER; }
	u8 getSendType() const
	{ return ACTIVEOBJECT_TYPE_GENERIC; }
	std::string getDescription();

	/*
		Active object <-> environment interface
	*/

	void addedToEnvironment();
	void removingFromEnvironment();
	bool isStaticAllowed() const;
	bool unlimitedTransferDistance() const;
	std::string getClientInitializationData();
	std::string getStaticData();
	void step(float dtime, bool send_recommended);
	void setBasePosition(const v3f &position);
	void setPos(v3f pos);
	void moveTo(v3f pos, bool continuous);

	/*
		Interaction interface
	*/

	int punch(v3f dir,
		const ToolCapabilities *toolcap,
		ServerActiveObject *puncher,
		float time_from_last_punch);
	void rightClick(ServerActiveObject *clicker);
	s16 getHP() const;
	s16 getHunger() const;
	s16 getOxygen() const;
	f32 getExhaustion() const;
	void setExhaustion(f32 ht);
	void setHP(s16 hp);
	void setHunger(s16 hunger);
	void setOxygen(s16 oxygen);
	f32 getHungerTimer() const;
	void setHungerTimer(f32 ht);
	f32 getHungerHurtHealTimer() const;
	void setHungerHurtHealTimer(f32 ht);
	f32 getOxygenTimer() const;
	void setOxygenTimer(f32 ht);
	f32 getOxygenHurtTimer() const;
	void setOxygenHurtTimer(f32 ht);

	bool in_water();

	void setArmorGroups(const ItemGroupList &armor_groups);
	ObjectProperties* accessObjectProperties();
	void notifyObjectPropertiesModified();

	/*
		Inventory interface
	*/

	Inventory* getInventory();
	const Inventory* getInventory() const;
	InventoryLocation getInventoryLocation() const;
	void setInventoryModified();
	std::string getWieldList() const;
	int getWieldIndex() const;
	void setWieldIndex(int i);

	/*
		PlayerSAO-specific
	*/

	void disconnected();

	void createCreativeInventory();

	Player* getPlayer()
	{
		return m_player;
	}
	u16 getPeerID() const
	{
		return m_peer_id;
	}
	v3f getLastGoodPosition() const
	{
		return m_last_good_position;
	}
	float resetTimeFromLastPunch()
	{
		float r = m_time_from_last_punch;
		m_time_from_last_punch = 0.0;
		return r;
	}
	void updatePrivileges(const std::set<std::string> &privs,
			bool is_singleplayer)
	{
		m_privs = privs;
		m_is_singleplayer = is_singleplayer;
	}

private:
	std::string getPropertyPacket();
	
	Player *m_player;
	u16 m_peer_id;
	Inventory *m_inventory;
	v3f m_last_good_position;
	float m_last_good_position_age;
	float m_time_from_last_punch;
	int m_wield_index;
	bool m_position_not_sent;
	bool m_textures_not_sent;
	ItemGroupList m_armor_groups;
	bool m_armor_groups_sent;
	bool m_properties_sent;
	struct ObjectProperties m_prop;
	// Cached privileges for enforcement
	std::set<std::string> m_privs;
	bool m_is_singleplayer;
	std::string m_textures_mod;

public:
	// Some flags used by Server
	bool m_teleported;
	bool m_inventory_not_sent;
	bool m_hp_not_sent;
	bool m_hunger_not_sent;
	bool m_oxygen_not_sent;
	bool m_wielded_item_not_sent;
};

#endif

