/******************************************************
 *			Copyright 2015, by linglong.
 *				All right reserved.
 *功能：国战玩家实例
 *日期：2015-6-1 15:05
 *作者：milo_woo
 ******************************************************/
#ifndef _GWM_WAR_PLAYER_H_
#define _GWM_WAR_PLAYER_H_

#include <framework/daserver.h>
#include <framework/endpoint.h>
#include <util/refshare.h>
#include <common/util/singleton.h>
#include <gatewaymgr/gwmworldwar.h>
#include <gateway/worldwar.h>

class CWarJoinPlayer : public CRefShare
{
public:
	CWarJoinPlayer();

	virtual ~CWarJoinPlayer();

	friend class CRefObject<CWarJoinPlayer>;

public:
	void playerOnline(OBJID idPlayer);
	void playerOffline(OBJID idPlayer);
	void onServerStop();

	/**
	 * @功能：定时
	 */
	void onTime();

	/**
	* @功能：判断武将是否上阵
	*/
	bool isHaveDeployedBuddy(std::vector<CFormatUint> &vecUint);

	/**
	* @功能：增加城池上阵武将队伍
	*/
	void addDeployedBuddy(uint8 idCity, std::vector<CFormatUint> &vecUint);

	/**
	* @功能：获取所有上阵武将信息(除了某个城池外)
	*/
	void getNotInDeployedBuddy(uint8 idCity, std::vector<uint32> &vecBuddy);

	/**
	* @功能：获取城池所有上阵武将信息
	*/
	void getCityDeployedBuddy(uint8 idCity, std::vector<uint32> &vecBuddy);



public:
	void incRef()
	{
		CRefShare::incRef();
	}

	bool decRef()
	{
		return CRefShare::decRef();
	}

public:
	OBJID m_nPlayerID;     //玩家ID
	std::string m_strName; //姓名
	uint8 m_nSex;   //性别
	uint8 m_nLevel; //等级
	uint32 m_nBattEffect; //战斗力
	uint8 m_nNation;  //阵营
	uint8  m_nGoverPosts; //国家官职
	uint8  m_nCityID;    //当前城池
	std::vector<CPlayerCityWarInf> m_vecCityWarInf; //玩家城战信息
	std::map<uint8, std::map<uint32, uint8> > m_mapBuddy; //玩家城池驻兵信息
};
typedef CRefObject<CWarJoinPlayer> CWarJoinPlayerPtr;

#endif
