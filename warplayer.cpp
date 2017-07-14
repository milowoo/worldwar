/******************************************************
*			Copyright 2013, by linglong.
*				All right reserved.
*功能：国战玩家实例
*日期：2015-5-12 15:05
*作者：milo_woo
******************************************************/
#include <net/netdefines.h>
#include "module.h"
#include "warplayer.h"
#include <algorithm>
#include <business/language.h>
#include <business/langdefines.h>


CWarJoinPlayer::CWarJoinPlayer()
{
	m_nPlayerID = 0;   
	m_strName = "";
	m_nSex = 0;  
	m_nLevel = 0;  
	m_nBattEffect = 0; 
	m_nNation = 0;   
	m_nGoverPosts = 0;              
	m_vecCityWarInf.clear();
	m_mapBuddy.clear();
}

CWarJoinPlayer::~CWarJoinPlayer()
{

}

void CWarJoinPlayer::playerOnline(OBJID idPlayer)
{

}

void CWarJoinPlayer::playerOffline(OBJID idPlayer)
{

}


void CWarJoinPlayer::onServerStop()
{

}

void CWarJoinPlayer::onTime()
{	

}

/**
* @功能：判断武将是否上阵
*/
bool CWarJoinPlayer::isHaveDeployedBuddy(std::vector<CFormatUint> &vecUint)
{
	for (size_t i = 0; i < vecUint.size(); i++)
	{
		CFormatUint &cUint = vecUint[i];
		uint32 idBuddy = cUint.m_nConfId;
		std::map<uint8, std::map<uint32, uint8> >::iterator iter =  m_mapBuddy.begin();
		for (; iter != m_mapBuddy.end(); ++iter)
		{
			std::map<uint32, uint8>::iterator iter_buddy = iter->second.find(idBuddy);
			if (iter_buddy != iter->second.end())
				return true;
		}
	}

	return false;
}

/**
* @功能：增加城池上阵武将队伍
*/
void CWarJoinPlayer::addDeployedBuddy(uint8 idCity, std::vector<CFormatUint> &vecUint)
{
	m_mapBuddy.erase(idCity);
	std::map<uint32, uint8> mapBuddy;
	for (size_t i = 0; i < vecUint.size(); i++)
	{
		CFormatUint &cUint = vecUint[i];
		uint32 idBuddy = cUint.m_nConfId;
		mapBuddy[idBuddy] = 0;
	}

	if (mapBuddy.size() > 0)
		m_mapBuddy.insert(make_pair(idCity, mapBuddy));

	return;
}

/**
* @功能：获取所有上阵武将信息(除了某个城池外)
*/
void CWarJoinPlayer::getNotInDeployedBuddy(uint8 idCity, std::vector<uint32> &vecBuddy)
{
	std::map<uint8, std::map<uint32, uint8> >::iterator iter =  m_mapBuddy.begin();
	for (; iter != m_mapBuddy.end(); ++iter)
	{
		if (iter->first == idCity)
			continue;

		std::map<uint32, uint8>::iterator iter_buddy = iter->second.begin();
		for (; iter_buddy != iter->second.end(); ++iter_buddy)
		{
			vecBuddy.push_back(iter_buddy->first);
		}
	}
    
	return;
}

/**
* @功能：获取城池所有上阵武将信息
*/
void CWarJoinPlayer::getCityDeployedBuddy(uint8 idCity, std::vector<uint32> &vecBuddy)
{
	std::map<uint8, std::map<uint32, uint8> >::iterator iter =  m_mapBuddy.find(idCity);
	if (iter == m_mapBuddy.end())
		return;

	std::map<uint32, uint8>::iterator iter_buddy = iter->second.begin();
	for (; iter_buddy != iter->second.end(); ++iter_buddy)
	{
		vecBuddy.push_back(iter_buddy->first);
	}

	return;
}
