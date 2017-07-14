/******************************************************
*			Copyright 2013, by linglong.
*				All right reserved.
*功能：国战玩家实例
*日期：2015-5-12 15:05
*作者：milo_woo
******************************************************/
#include <net/netdefines.h>
#include "module.h"
#include "warnation.h"
#include <algorithm>
#include <business/language.h>
#include <business/langdefines.h>


CWarNation::CWarNation()
{
	m_nNationID = 0;   
	m_strName = "";
	m_vecDomainReward.clear();
	m_nSessionID = 1;
	m_nDecNum = 0;
	m_nTeamID = 1;
}

CWarNation::~CWarNation()
{

}

void CWarNation::playerOnline(OBJID idPlayer)
{

}

void CWarNation::playerOffline(OBJID idPlayer)
{

}


void CWarNation::onServerStop()
{

}

void CWarNation::onTime()
{	

}

/**
* @功能：获取疆域管辖奖励
*/
void CWarNation::getAllDomainReward(std::vector<CDomainReward> &vecDomainReward)
{
	vecDomainReward.assign(m_vecDomainReward.begin(), m_vecDomainReward.end());
	return;
}

/**
* @功能：获取某小时疆域管辖奖励
*/
void CWarNation::getHourDomainReward(uint16 nHour, std::vector<CRewardInfo> &vecReward)
{
	for (size_t i = 0; i < m_vecDomainReward.size(); i++)
	{
		CDomainReward &cDomaincReward = m_vecDomainReward[i];
		if (cDomaincReward.m_nTime == nHour)
		{
			vecReward.assign(cDomaincReward.m_vecReward.begin(), cDomaincReward.m_vecReward.end());
			return;
		}
	}

	return;
}

//获取参战成员信息
CPlayerCityWarInfPtr CWarNation::getJoinPlayer(OBJID idPlayer)
{
	std::map<OBJID, CPlayerCityWarInfPtr>::iterator iter = m_mapPlayerInf.find(idPlayer);
	if (iter == m_mapPlayerInf.end())
	{
		return NULL;
	}
	return iter->second;
}


/**
* @功能：获取队伍ID
*/
uint32  CWarNation::getTeamID()
{
	m_nTeamID++;
	if (m_nTeamID >= 99999)
		m_nTeamID = 1;

	uint32 nNation  = (uint32)m_nNationID;

	return (nNation * 100000 + m_nTeamID);
}

//增加队伍ID
void CWarNation::addNewTeamID(uint32 idTeam)
{
    uint32 idInnerTeam = idTeam %100000;
	if (idInnerTeam > m_nTeamID)
		m_nTeamID = idInnerTeam;
}

/**
* @功能：增加新驻兵队伍
*/
void CWarNation::addNewEtapeTeam(CEtapeTeam &cEtapeTeam)
{
	m_vecEtapeTeam.push_back(cEtapeTeam);
}

/**
* @功能：删除驻兵队伍
*/
OBJID CWarNation::eraseEtapeTeam(uint32 idTeam)
{
	OBJID idPlayer = 0;
	std::vector<CEtapeTeam> vecTeam;
	for (size_t i = 0; i < m_vecEtapeTeam.size(); i++)
	{
		CEtapeTeam &cTeam = m_vecEtapeTeam[i];
		if (cTeam.m_nTeamID == idTeam)
		{
			idPlayer = cTeam.m_nPlayerID;
			continue;
		}
		vecTeam.push_back(cTeam);
	}

	m_vecEtapeTeam.clear();
	for (size_t i = 0; i < vecTeam.size(); i++)
		m_vecEtapeTeam.push_back(vecTeam[i]);
	return idPlayer;
}

void CWarNation::delEtapeTeam(uint32 idTeam)
{
	OBJID idPlayer = this->eraseEtapeTeam(idTeam);
	std::map<OBJID, CPlayerCityWarInfPtr >::iterator iter_player = m_mapPlayerInf.find(idPlayer);
	if (iter_player != m_mapPlayerInf.end())
	{
		CPlayerCityWarInfPtr cPlayerWarPtr = iter_player->second;
		if (cPlayerWarPtr != NULL)
			this->erasePlayerTeam(cPlayerWarPtr, idTeam);
	}
	return;
}
void  CWarNation::erasePlayerTeam(CPlayerCityWarInfPtr pPlayerCityWarPtr, uint32 idTeam)
{
	if (pPlayerCityWarPtr == NULL)
		return;
	std::vector<CPlayerTeamInf> vecTeamInf;
	for (size_t i = 0; i < pPlayerCityWarPtr->m_vecTeamInf.size(); i++)
	{
		CPlayerTeamInf &cTeamInf = pPlayerCityWarPtr->m_vecTeamInf[i];
		if (cTeamInf.m_nTeamID == idTeam)
			continue;

		vecTeamInf.push_back(cTeamInf);
	}

	pPlayerCityWarPtr->m_vecTeamInf.clear();
	for (size_t i = 0; i < vecTeamInf.size(); i++)
	{
		pPlayerCityWarPtr->m_vecTeamInf.push_back(vecTeamInf[i]);
	}

	return;
}


/**
* @功能：获取禁军队伍列表
*/
void CWarNation::getEtapeNpcTeamList(std::vector<CEtapeTeam> &vecEtapeTeam)
{
	if (m_vecEtapeNpcTeam.size() == 0)
		return;

	vecEtapeTeam.assign(m_vecEtapeNpcTeam.begin(), m_vecEtapeNpcTeam.end());
	return;
}

/**
* @功能：获取驻兵队伍数量
*/
uint16 CWarNation::getEtapeTeamNum()
{
    return m_vecEtapeTeam.size() + m_vecEtapeNpcTeam.size();
}


/**
* @功能：获取禁军队伍数量
*/
uint16 CWarNation::getNpcTeamNum()
{
    return m_vecEtapeNpcTeam.size();
}

/**
* @功能：获取驻兵队伍列表
*/
void CWarNation::getEtapeTeamList(std::vector<CEtapeTeam> &vecEtapeTeam)
{
	if (m_vecEtapeTeam.size() == 0)
		return;

    vecEtapeTeam.assign(m_vecEtapeTeam.begin(), m_vecEtapeTeam.end());
	return;
}


/**
* @功能：增加玩家城战信息
*/
void  CWarNation::addPlayerCityInf(OBJID idPlayer, CPlayerCityWarInfPtr pPlayerCityIntPtr)
{
	m_mapPlayerInf.insert(make_pair(idPlayer, pPlayerCityIntPtr));

	return;
}

/**
* @功能：清空队伍信息
*/
void CWarNation::clearEtapeTeam()
{
	m_vecEtapeTeam.clear();
	std::map<OBJID, CPlayerCityWarInfPtr >::iterator iter =  m_mapPlayerInf.begin();
	for (; iter != m_mapPlayerInf.end(); ++iter)
	{
		CPlayerCityWarInfPtr cPlayerWarPtr = iter->second;
		if (cPlayerWarPtr == NULL)
			continue;

		cPlayerWarPtr->m_vecTeamInf.clear();
	}

	return;
}

//清理奖励信息
void CWarNation::clearDomainReward()
{
	m_vecDomainReward.clear();

	return;
}

/**
* @功能：校验奖励
*/
bool CWarNation::checkDomainReward(uint16 nHour)
{
	for (size_t i = 0; i < m_vecDomainReward.size(); i++)
	{
		CDomainReward &cReward = m_vecDomainReward[i];
		if (cReward.m_nTime == nHour)
			return true;

	}
	return false;
}

