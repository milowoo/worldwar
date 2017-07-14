/******************************************************
 *			Copyright 2015, by linglong.
 *				All right reserved.
 *功能：国战国家实例
 *日期：2015-7-16 15:05
 *作者：milo_woo
 ******************************************************/
#ifndef _GWM_WAR_NATION_H_
#define _GWM_WAR_NATION_H_

#include <framework/daserver.h>
#include <framework/endpoint.h>
#include <util/refshare.h>
#include <common/util/singleton.h>
#include <gatewaymgr/gwmworldwar.h>
#include <gateway/worldwar.h>

class CWarNation : public CRefShare
{
public:
	CWarNation();

	virtual ~CWarNation();

	friend class CRefObject<CWarNation>;

public:
	void playerOnline(OBJID idPlayer);
	void playerOffline(OBJID idPlayer);
	void onServerStop();

	/**
	 * @功能：定时
	 */
	void onTime();
   
	//设置国家
	void setNation(uint8 idNation){m_nNationID = idNation;}
	uint8 getNation(){return m_nNationID;}

	/**
	* @功能：获取疆域管辖奖励
	*/
	void getAllDomainReward(std::vector<CDomainReward> &vecDomainReward);

	/**
	* @功能：获取某小时疆域管辖奖励
	*/
	void getHourDomainReward(uint16 nHour, std::vector<CRewardInfo> &vecReward);

	void clearDomainReward();

	//获取城战玩家信息
	CPlayerCityWarInfPtr getJoinPlayer(OBJID idPlayer);

	/**
	* @功能：增加玩家防守队伍
	*/
	void addPlayerCityInf(OBJID idPlayer, CPlayerCityWarInfPtr pPlayerCityIntPtr);

	/**
	* @功能：获取队伍ID
	*/
	uint32 getTeamID();
	void setTeamID(uint32 idTeam){m_nTeamID = idTeam;};

	/**
	* @功能：增加新驻兵队伍
	*/
	void addNewEtapeTeam(CEtapeTeam &cEtapeTeam);
    
	//增加队伍ID
	void addNewTeamID(uint32 idTeam);

	/**
	* @功能：删除驻兵队伍
	*/
	void delEtapeTeam(uint32 idTeam);

	/**
	* @功能：获取驻兵队伍列表
	*/
	void getEtapeTeamList(std::vector<CEtapeTeam> &vecEtapeTeam);

	/**
	* @功能：获取驻兵队伍数量
	*/
	uint16 getEtapeTeamNum();

	/**
	* @功能：获取禁军队伍列表
	*/
	void getEtapeNpcTeamList(std::vector<CEtapeTeam> &vecEtapeTeam);
   
	/**
	* @功能：获取禁军队伍数量
	*/
	uint16 getNpcTeamNum();


	/**
	* @功能：清空队伍信息
	*/
	void clearEtapeTeam();

	/**
	* @功能：获取session
	*/
	void incSessionID(){m_nSessionID++;};
	uint32 getSessionID(){return m_nSessionID;}
    
	/**
	* @功能：宣战次数
	*/
	uint8 getDecNum(){return m_nDecNum;}
	void setDenNum(uint8 nNum){m_nDecNum = nNum;}
    
	/**
	* @功能：校验奖励
	*/
	bool checkDomainReward(uint16 nHour);
	void erasePlayerTeam(CPlayerCityWarInfPtr pPlayerCityWarPtr, uint32 idTeam);
	OBJID eraseEtapeTeam(uint32 idTeam);




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
	uint8 m_nNationID;   //国家ID
	uint32 m_nTeamID;    //队伍ID
	uint32 m_nSessionID; //兵站session
	uint8 m_nDecNum;     //宣战次数
	std::string m_strName; //国家名称
	std::vector<CDomainReward> m_vecDomainReward;
	std::vector<CEtapeTeam>  m_vecEtapeTeam;  //兵站信息
	std::vector<CEtapeTeam> m_vecEtapeNpcTeam; //兵站禁军数量
	std::map<OBJID, CPlayerCityWarInfPtr > m_mapPlayerInf; //玩家驻兵信息

};
typedef CRefObject<CWarNation> CWarNationPtr;

#endif
