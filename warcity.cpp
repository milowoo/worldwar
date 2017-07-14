/******************************************************
*			Copyright 2013, by linglong.
*				All right reserved.
*功能：国战城池实例
*日期：2015-5-12 15:05
*作者：milo_woo
******************************************************/
#include <net/netdefines.h>
#include "module.h"
#include "warcity.h"
#include <algorithm>
#include <business/language.h>
#include <business/langdefines.h>

static bool comparerank(const CWarCityRank& p1, const CWarCityRank& p2)
{
	if (p1.m_nKillNum > p2.m_nKillNum)
	{
		return true;
	}
	
	if (p1.m_nKillNum == p2.m_nKillNum && p1.m_nTimes < p2.m_nTimes)
	{
		return true;
	}

	return false;
}

CWarCity::CWarCity()
{
	m_nCityID = 0;   //城池ID
	m_nAccType = 0;
	m_strName = "";
	m_nNation = 0;   //国家
	m_nAttNaion = 0; //攻击国家
	m_nAccID = 0;    //事件ID
	m_nBeginTime = 0; //开始时间
	m_nEndTime = 0;    //结束时间
	m_nProtctTime = 0; //保护时间
	m_nTotBlowNpcTeam = 0;                     //防守NPC队伍总数
	m_nTotAttNpcTeam = 0;                      //攻击NPC队伍总数
	m_nTeamID = 0;                             //队伍ID
	m_nSessionID = 0;                          //序列号
	m_nLastWheelFighTime = 0;
	m_nStatus = 0;
	m_nClearPickTeamTime = 0;
	m_nTotPlayerBlowTeam = 0;
	m_nTotPlayerAttTeam = 0;
	m_nQueueFail = 0;
	m_nAutoPickTime = 0;
	m_nBattleID = 1;
	
	m_vecBlowNPCTeam.clear();     //防守NPC队伍排序
	m_vecAttNPCTeam.clear();      //进攻NPC队伍排序
    
	m_mapNpcTeam.clear();         //NPC队伍信息
	m_mapMonsterHP.clear();       //怪物血量

	m_vecBlowTeam.clear();         //防守队伍信息  (大本营中参与玩家的队列)
	m_vecAttTeam.clear();
	m_mapPlayerTeam.clear();       //玩家队伍信息

	m_mapPickTeam.clear();         //属于单挑或被单挑的队伍
	m_mapFightTeam.clear();        //战斗中的队伍

	m_vecAttQueue1Team.clear();                //战斗队列1攻击信息
	m_vecBlowQueue1Team.clear() ;               //战斗队列1防守信息
	m_strBattleReport1 = "";
	m_nQueueNextTime1 = 0;                       //队列1需要处理的时间
	m_nQueueSession1 = 0;                       //队列1session
	m_vecGrid1.clear();
	m_nTeamLastHP1 = 0;

	m_vecAttQueue2Team.clear();                //战斗队列2攻击信息
	m_vecBlowQueue2Team.clear();               //战斗队列2防守信息
	m_strBattleReport2 = "";
	m_nQueueNextTime2 = 0;                      //队列2需要处理的时间
	m_nQueueSession2 = 0;                       //队列2session
	m_vecGrid2.clear();
	m_nTeamLastHP2 = 0;

	m_vecAttQueue3Team.clear();                //战斗队列3攻击信息
	m_vecBlowQueue3Team.clear();               //战斗队列3防守信息
	m_strBattleReport3= "";
	m_nQueueNextTime3 = 0;                      //队列3需要处理的时间
	m_nQueueSession3 = 0;                       //队列3session
	m_vecGrid3.clear();
	m_nTeamLastHP3 = 0;

	m_nQueueStatus = 0;                          //队列信息 0正常 8个字节，后3个低位表示3个队列 0表示正常 1表示摧毁

	m_vecCityRank.clear();      //杀敌榜信息
	m_nRankSession = 0;

	m_vecDefSuccReward.clear();   //防守成功奖励
	m_vecDefFailRewad.clear();     //防守失败奖励
	m_vecAttSuccRewad.clear();   //进攻成功奖励
	m_vecAttFailReward.clear();  //进攻失败奖励

	m_mapEnterPlayer.clear();
	m_mapCaptainTeam.clear();
	m_mapItemRewardExp.clear();
	m_mapFightPlayer.clear();
	m_pConsummer = NULL;

	m_mapBattle.clear();

	//自爆兵信息
	bHaveAttBlewTeam = false;
	bHaveDefBlewTeam = false;
	m_mapBlewTeam.clear();

}

CWarCity::~CWarCity()
{

}

void CWarCity::playerOnline(OBJID idPlayer)
{

}

void CWarCity::playerOffline(OBJID idPlayer)
{

}


void CWarCity::onServerStop()
{

}

void CWarCity::onTime()
{	

}

//获取参战成员信息
CPlayerCityWarInfPtr CWarCity::getJoinPlayer(OBJID idPlayer)
{
	std::map<OBJID, CPlayerCityWarInfPtr>::iterator iter = m_mapPlayerTeam.find(idPlayer);
	if (iter == m_mapPlayerTeam.end())
	{
		return NULL;
	}
	return iter->second;
}

/**
* @功能：获取队伍ID
*/
uint32  CWarCity::getTeamID()
{
    m_nTeamID++;
	if (m_nTeamID >= 99999)
		m_nTeamID = 1;

	uint32 nCity  = (uint32)m_nCityID;

	return nCity * 100000 + m_nTeamID;
}


/**
* @功能：初始化城池事件
*/
void CWarCity::init(uint8 idAcc, uint8 idCity, uint32 beginTime)
{

	m_nAccID = idAcc;
	m_nCityID = idCity;
	m_nBeginTime = beginTime;
	m_nEndTime = beginTime + 2 * 60 * 60;
}


/**
* @功能：增加玩家城战信息
*/
void  CWarCity::addPlayerCityInf(OBJID idPlayer, CPlayerCityWarInfPtr pPlayerCityIntPtr)
{
	m_mapPlayerTeam.insert(make_pair(idPlayer, pPlayerCityIntPtr));
}


/**
* @功能：玩家增加防守队伍
*/
void CWarCity::addPlayerBlowTeam(OBJID idPlayer, CPlayerCityWarInfPtr pPlayerCityIntPtr)
{
	if (pPlayerCityIntPtr == NULL)
		return;

	std::map<OBJID, CPlayerCityWarInfPtr>::iterator iter =  m_mapPlayerTeam.find(idPlayer);
	if (iter == m_mapPlayerTeam.end())
	{
		m_mapPlayerTeam.insert(make_pair(idPlayer, pPlayerCityIntPtr));
	}
}

/**
* @功能：删除玩家防守队伍
*/
void CWarCity::delPlayerBlowTeam(CPlayerCityWarInfPtr pPlayerCityIntPtr)
{
	pPlayerCityIntPtr->m_vecTeamInf.clear();
}

/**
* @功能：修改玩家防守队伍
*/



/**
* @功能：删除队列1的前面一个防守方人员
*/
void CWarCity::eraseBlowQueue1Team()
{
    if (m_vecBlowQueue1Team.size() == 0)
		return;

	uint32 idTeam = 0;

#ifdef WIN32  
	std::vector<CCitySTeam> vecQueueTeam;
	for (size_t i = 0; i < m_vecBlowQueue1Team.size(); i++)
	{
		if (i == 0)
		{
			idTeam = m_vecBlowQueue1Team[0].m_nTeamID;
			continue;
		}
		vecQueueTeam.push_back(m_vecBlowQueue1Team[i]);
	}

	m_vecBlowQueue1Team.clear();
	for (size_t i = 0; i< vecQueueTeam.size(); i++)
		m_vecBlowQueue1Team.push_back(vecQueueTeam[i]);

#else
	std::vector<CCitySTeam>::iterator iter = m_vecBlowQueue1Team.begin();
	idTeam = iter->m_nTeamID;
	m_vecBlowQueue1Team.erase(iter);
#endif

	this->clearPlayerTeam(idTeam);
	m_mapFightTeam.erase(idTeam);
	return;
}

void CWarCity::getBlowQueue1FirstTeam(CCitySTeam &cSTeam)
{
	if (m_vecBlowQueue1Team.size() == 0)
		return;

	cSTeam = m_vecBlowQueue1Team[0];
}


/**
* @功能：删除队列1的前面一个攻击方人员
*/
void CWarCity::eraseAttQueue1Team()
{
	if (m_vecAttQueue1Team.size() == 0)
			return ;

	uint32 idTeam = 0;

#ifdef WIN32  
	std::vector<CCitySTeam> vecQueueTeam;
	for (size_t i = 0; i < m_vecAttQueue1Team.size(); i++)
	{
		if (i == 0)
		{
			idTeam = m_vecAttQueue1Team[0].m_nTeamID;
			continue;
		}
        
		vecQueueTeam.push_back(m_vecAttQueue1Team[i]);
	}

	m_vecAttQueue1Team.clear();
    for (size_t i = 0; i< vecQueueTeam.size(); i++)
		m_vecAttQueue1Team.push_back(vecQueueTeam[i]);
	    
#else
	std::vector<CCitySTeam>::iterator iter = m_vecAttQueue1Team.begin();
	idTeam = iter->m_nTeamID;
	m_vecAttQueue1Team.erase(iter);
#endif

	this->clearPlayerTeam(idTeam);
	m_mapFightTeam.erase(idTeam);
	return ;
}

void CWarCity::getAttQueue1FirstTeam(CCitySTeam &cSTeam)
{
	if (m_vecAttQueue1Team.size() == 0)
		return;

	cSTeam = m_vecAttQueue1Team[0];
}

/**
* @功能：删除队列2的前面一个防守方人员
*/
void CWarCity::eraseBlowQueue2Team()
{
	if (m_vecBlowQueue2Team.size() == 0)
		return;

	uint32 idTeam = 0;

#ifdef WIN32  
	std::vector<CCitySTeam> vecQueueTeam;
	for (size_t i = 0; i < m_vecBlowQueue2Team.size(); i++)
	{
		if (i == 0)
		{
			idTeam = m_vecBlowQueue2Team[0].m_nTeamID;
			continue;
		}

		vecQueueTeam.push_back(m_vecBlowQueue2Team[i]);
	}

	m_vecBlowQueue2Team.clear();
	for (size_t i = 0; i< vecQueueTeam.size(); i++)
		m_vecBlowQueue2Team.push_back(vecQueueTeam[i]);

#else
	std::vector<CCitySTeam>::iterator iter = m_vecBlowQueue2Team.begin();
	idTeam = iter->m_nTeamID;
	m_vecBlowQueue2Team.erase(iter);
#endif
    this->clearPlayerTeam(idTeam);
	m_mapFightTeam.erase(idTeam);

	return ;
}

/**
* @功能：清理玩家队伍
*/
void CWarCity::clearPlayerTeam(uint32 idTeam)
{
	std::map<uint32, CWarCityTeam>::iterator iter1 = m_mapFightTeam.find(idTeam);
	if (iter1 != m_mapFightTeam.end())
	{
		CWarCityTeam &cCityTeam = iter1->second;
		if (cCityTeam.m_nTeamType == TEAM_TYPE_PLAYER)
		{
			OBJID idPlayer = cCityTeam.m_nAvatarID;
			std::map<OBJID, CPlayerCityWarInfPtr>::iterator iter_player = m_mapPlayerTeam.find(idPlayer);
			if (iter_player != m_mapPlayerTeam.end())
			{
				CPlayerCityWarInfPtr pPlayerWarPtr = iter_player->second;
				if (pPlayerWarPtr != NULL)
				{
					this->erasePlayerTeam(pPlayerWarPtr, idTeam);
				}
			}
		}
	}

	return;
}

void CWarCity::erasePlayerTeam(CPlayerCityWarInfPtr pPlayerCityWarPtr, uint32 idTeam)
{
	if (pPlayerCityWarPtr == NULL)
		return;
	std::vector<CPlayerTeamInf> vecTeamInf;
	for (size_t i = 0; i < pPlayerCityWarPtr->m_vecTeamInf.size(); i++)
	{
		CPlayerTeamInf &cTeamInf = pPlayerCityWarPtr->m_vecTeamInf[i];
		if (cTeamInf.m_nTeamID == idTeam)
		{
			continue;
		}
		vecTeamInf.push_back(cTeamInf);
		}
	pPlayerCityWarPtr->m_vecTeamInf.clear();
	for (size_t i = 0; i < vecTeamInf.size(); i++)
	{
		pPlayerCityWarPtr->m_vecTeamInf.push_back(vecTeamInf[i]);
	}

	return;
}

void CWarCity::getBlowQueue2FirstTeam(CCitySTeam &cSTeam)
{
	if (m_vecBlowQueue2Team.size() == 0)
		return;

	cSTeam= m_vecBlowQueue2Team[0];
}


void CWarCity::getAttQueue2FirstTeam(CCitySTeam &cSTeam)
{
	if (m_vecAttQueue2Team.size() == 0)
		return;

	cSTeam = m_vecAttQueue2Team[0];
}


/**
* @功能：删除队列2的前面一个攻击方人员
*/
void CWarCity::eraseAttQueue2Team()
{
	if (m_vecAttQueue2Team.size() == 0)
		return ;

	uint32 idTeam = 0;

#ifdef WIN32  
	std::vector<CCitySTeam> vecQueueTeam;
	for (size_t i = 0; i < m_vecAttQueue2Team.size(); i++)
	{
		if (i == 0)
		{
			idTeam = m_vecAttQueue2Team[0].m_nTeamID;
			continue;
		}

		vecQueueTeam.push_back(m_vecAttQueue2Team[i]);
	}

	m_vecAttQueue2Team.clear();
	for (size_t i = 0; i< vecQueueTeam.size(); i++)
		m_vecAttQueue2Team.push_back(vecQueueTeam[i]);
#else
	std::vector<CCitySTeam>::iterator iter = m_vecAttQueue2Team.begin();
	idTeam = iter->m_nTeamID;
	m_vecAttQueue2Team.erase(iter);
#endif

    this->clearPlayerTeam(idTeam);
	m_mapFightTeam.erase(idTeam);

	return ;
}


/**
* @功能：删除队列3的前面一个防守方人员
*/
void CWarCity::eraseBlowQueue3Team()
{
	if (m_vecBlowQueue3Team.size() == 0)
		return ;

	uint32 idTeam = 0;

#ifdef WIN32  
	std::vector<CCitySTeam> vecQueueTeam;
	for (size_t i = 0; i < m_vecBlowQueue3Team.size(); i++)
	{
		if (i == 0)
		{
			idTeam = m_vecBlowQueue3Team[0].m_nTeamID;
			continue;
		}

		vecQueueTeam.push_back(m_vecBlowQueue3Team[i]);
	}

	m_vecBlowQueue3Team.clear();
	for (size_t i = 0; i< vecQueueTeam.size(); i++)
		m_vecBlowQueue3Team.push_back(vecQueueTeam[i]);

#else
	std::vector<CCitySTeam>::iterator iter = m_vecBlowQueue3Team.begin();
	idTeam = iter->m_nTeamID;
	m_vecBlowQueue3Team.erase(iter);
#endif
    
	this->clearPlayerTeam(idTeam);
	m_mapFightTeam.erase(idTeam);

	return ;
}

void CWarCity::getBlowQueue3FirstTeam(CCitySTeam &cSTeam)
{
	if (m_vecBlowQueue3Team.size() == 0)
		return;

	cSTeam = m_vecBlowQueue3Team[0];
}

void CWarCity::getAttQueueFirstTeam(uint8 nQueueNum, CCitySTeam &cSTeam)
{
	switch (nQueueNum)
	{
	case CITY_QUEUE_NUM_FIRST:
		this->getAttQueue1FirstTeam(cSTeam);
		return;
	case CITY_QUEUE_NUM_SECOND:
		this->getAttQueue2FirstTeam(cSTeam);
		return;
	case CITY_QUEUE_NUM_THIRTY:
		this->getAttQueue3FirstTeam(cSTeam);
		return;
	}
	this->getAttQueue1FirstTeam(cSTeam);
	return;
}
void CWarCity::setQueueNextTime(uint8 nQueueNum, uint32 queueTime)
{
	switch (nQueueNum)
	{
	case CITY_QUEUE_NUM_FIRST:
		{
			m_nQueueNextTime1 = queueTime;
			return;
		}
	case CITY_QUEUE_NUM_SECOND:
		{
			m_nQueueNextTime2 = queueTime;
			return;
		}
	case CITY_QUEUE_NUM_THIRTY:
		{
			m_nQueueNextTime3 = queueTime;
			return;
		}
	}
	return;
}
void CWarCity::getBlowQueueFirstTeam(uint8 nQueueNum, CCitySTeam &cSTeam)
{
	switch (nQueueNum)
	{
	case CITY_QUEUE_NUM_FIRST:
		this->getBlowQueue1FirstTeam(cSTeam);
		return;
	case CITY_QUEUE_NUM_SECOND:
		this->getBlowQueue2FirstTeam(cSTeam);
		return;
	case CITY_QUEUE_NUM_THIRTY:
		this->getBlowQueue3FirstTeam(cSTeam);
		return;
	}
	this->getBlowQueue1FirstTeam(cSTeam);
	return;
}

/**
* @功能：删除队列3的前面一个攻击方人员
*/
void CWarCity::eraseAttQueue3Team()
{
	if (m_vecAttQueue3Team.size() == 0)
		return ;

	uint32 idTeam = 0;
#ifdef WIN32  
	std::vector<CCitySTeam> vecQueueTeam;
	for (size_t i = 0; i < m_vecAttQueue3Team.size(); i++)
	{
		if (i == 0)
		{
			idTeam = m_vecAttQueue3Team[0].m_nTeamID;
			continue;
		}

		vecQueueTeam.push_back(m_vecAttQueue3Team[i]);
	}

	m_vecAttQueue3Team.clear();
	for (size_t i = 0; i< vecQueueTeam.size(); i++)
		m_vecAttQueue3Team.push_back(vecQueueTeam[i]);

#else
	std::vector<CCitySTeam>::iterator iter = m_vecAttQueue3Team.begin();
	idTeam = iter->m_nTeamID;
	m_vecAttQueue3Team.erase(iter);
#endif

	this->clearPlayerTeam(idTeam);
	m_mapFightTeam.erase(idTeam);

	return ;
}


void CWarCity::getAttQueue3FirstTeam(CCitySTeam &cSTeam)
{
	if (m_vecAttQueue3Team.size() == 0)
		return;

	cSTeam = m_vecAttQueue3Team[0];
}



/**
* @功能：获取攻击队伍数量
*/
uint32 CWarCity::getAttTeamNum()
{
	return m_vecAttNPCTeam.size() + m_vecAttTeam.size() + \
		   m_vecAttQueue1Team.size() + m_vecAttQueue2Team.size() + m_vecAttQueue3Team.size() + m_mapPickTeam.size()/2;
}

/**
* @功能：获取防守队伍数量
*/
uint32  CWarCity::getBlowTeamNum()
{
    return m_vecBlowTeam.size() + m_vecBlowNPCTeam.size() + m_vecBlowQueue1Team.size() + m_vecBlowQueue2Team.size() + m_vecBlowQueue3Team.size() + m_mapPickTeam.size()/2;
}

/**
* @功能：获取排行榜信息
*/
void CWarCity::getFightRank(std::vector<CKillRank> &vecKillRank)
{
	CKillRank cKillRank;
	for (size_t i = 0; i < m_vecCityRank.size(); i++)
	{
		if (i >= 10)
			break;

		CWarCityRank &cityRank = m_vecCityRank[i];

		cKillRank.m_nKillNum = cityRank.m_nKillNum;
		cKillRank.m_nNation = cityRank.m_nNation;
		cKillRank.m_strName = cityRank.m_strName;
		vecKillRank.push_back(cKillRank);
	}

	return;
}

/**
* @功能：是否已经拥有怪物队伍信息
*/
bool CWarCity::isHavaNPCTeam(uint32 idNPC)
{
	std::map<uint32, CWarNPCTeamPtr>::iterator iter = m_mapNpcTeam.find(idNPC);
	if (iter == m_mapNpcTeam.end())
		return false;

	return true;
}


/**
* @功能：增加怪物队伍信息
*/
void  CWarCity::addNPCTeam(CWarNPCTeamPtr pWarNpcTeamPtr)
{
    if (pWarNpcTeamPtr == NULL)
		return;

	m_mapNpcTeam.insert(make_pair(pWarNpcTeamPtr->m_nID, pWarNpcTeamPtr));
    return;
}

/**
* @功能：获取怪物队伍信息
*/
CWarNPCTeamPtr CWarCity::getNPCTeam(uint32 idNPC)
{
	std::map<uint32, CWarNPCTeamPtr>::iterator iter =  m_mapNpcTeam.find(idNPC);
	if (iter == m_mapNpcTeam.end())
		return NULL;

	return iter->second;
}

/**
* @功能：获取怪物的初始血量
*/
CMonsterInitHPPtr CWarCity::getMonsterInitHp(uint32 idMonster)
{
	std::map<uint32, CMonsterInitHPPtr>::iterator iter = m_mapMonsterHP.find(idMonster);
	if (iter == m_mapMonsterHP.end())
		return NULL;

	return iter->second;
}

/**
* @功能：增加怪物的初始血量
*/
void CWarCity::addMonsterInitHp(uint32 idMonster, CMonsterInitHPPtr pMonsterInitPtr)
{
	if (pMonsterInitPtr == NULL)
		return;

	m_mapMonsterHP.insert(make_pair(idMonster, pMonsterInitPtr));
	return;
}

/**
* @功能：增加作战队伍
*/
void CWarCity::addFightTeam(CWarCityTeam &cCityTeam, uint8 nType)
{
    if (nType == 0)
		m_vecBlowNPCTeam.push_back(cCityTeam);
	else
		m_vecAttNPCTeam.push_back(cCityTeam);
	return;
}

/**
* @功能：获取战斗中的队伍
*/
void CWarCity::getFightTeam(std::vector<CCityFightTeam> &vecFightTeam)
{
	CCityFightTeam cFightTeam;
	if (!(m_nQueueStatus & 1))
	{
	for (size_t i = 0; i < m_vecAttQueue1Team.size(); i++)
		cFightTeam.m_vecAttTeam.push_back(m_vecAttQueue1Team[i]);

	for (size_t i = 0; i < m_vecBlowQueue1Team.size(); i++)
		cFightTeam.m_vecBlowTeam.push_back(m_vecBlowQueue1Team[i]);
	}
    
	vecFightTeam.push_back(cFightTeam);

	CCityFightTeam cFightTeam1;
	if (!(m_nQueueStatus & 2))
	{
	for (size_t i = 0; i < m_vecAttQueue2Team.size(); i++)
		cFightTeam1.m_vecAttTeam.push_back(m_vecAttQueue2Team[i]);

	for (size_t i = 0; i < m_vecBlowQueue2Team.size(); i++)
		cFightTeam1.m_vecBlowTeam.push_back(m_vecBlowQueue2Team[i]);
	}
	vecFightTeam.push_back(cFightTeam1);

	CCityFightTeam cFightTeam2;
	if (!(m_nQueueStatus & 4))
	{
	for (size_t i = 0; i < m_vecAttQueue3Team.size(); i++)
		cFightTeam2.m_vecAttTeam.push_back(m_vecAttQueue3Team[i]);

	for (size_t i = 0; i < m_vecBlowQueue3Team.size(); i++)
		cFightTeam2.m_vecBlowTeam.push_back(m_vecBlowQueue3Team[i]);
	}

	vecFightTeam.push_back(cFightTeam2);
}

/**
* @功能：获取战斗简报
*/
void CWarCity::getFightSReport(std::vector<CTeamBatReport> &vecFightReport)
{
	if (!(m_nQueueStatus & 1))
	{
		if (m_strBattleReport1.size() > 5 )
			vecFightReport.push_back(cbatReport1);
	}

	if (!(m_nQueueStatus & 2))
	{
		if (m_strBattleReport2.size() > 5)
			vecFightReport.push_back(cbatReport2);
	}

	if (!(m_nQueueStatus & 4))
	{
		if (m_strBattleReport3.size() > 5)
			vecFightReport.push_back(cbatReport3);
	}

	return;
}

/**
* @功能：是否在预备战斗中
*/
bool CWarCity::isInPreBatTeam(uint32 idTeam)
{
	std::map<uint32, CWarCityTeam>::iterator iter =  m_mapFightTeam.find(idTeam);
	if (iter != m_mapFightTeam.end())
		return true;

	std::map<uint32, CCityPickTeam>::iterator iter_bat =  m_mapPickTeam.find(idTeam);
	if (iter_bat != m_mapPickTeam.end())
		return true;

	return false;
}

/**
* @功能：是否在单挑/被单挑队伍中
*/
bool CWarCity::isInPickBatTeam(uint32 idTeam)
{
	std::map<uint32, CCityPickTeam>::iterator iter = m_mapPickTeam.find(idTeam);
	if (iter != m_mapPickTeam.end())
		return true;

	return false;
}

bool CWarCity::eraseAttTeam(uint32 idTeam)
{
#ifdef WIN32  
	bool bfalse = false;
	std::vector<CWarCityTeam> vecCityTeam;
	for (size_t i = 0; i< m_vecAttTeam.size(); i++)
	{
		CWarCityTeam cCityTeam = m_vecAttTeam[i];
		if (bfalse)
		{
			vecCityTeam.push_back(cCityTeam);
			continue;
		}
		if (cCityTeam.m_nTeamID == idTeam)
		{
			bfalse = true;
		}
		else
		{
			vecCityTeam.push_back(cCityTeam);
		}
	}
	if (bfalse)
	{
		m_vecAttTeam.clear();
		for (size_t i = 0; i < vecCityTeam.size(); i++)
			m_vecAttTeam.push_back(vecCityTeam[i]);
	}
	return bfalse;
#else
	std::vector<CWarCityTeam>::iterator iter = m_vecAttTeam.begin();
	for(; iter != m_vecAttTeam.end(); ++iter)
	{
		if (iter->m_nTeamID == idTeam)
		{
			m_vecAttTeam.erase(iter);
			return true;
		}
	}
	return false;
#endif
}
/**
* @功能：删除防守大本营中的队伍
*/
bool  CWarCity::eraseBlowTeam(uint32 idTeam)
{
#ifdef WIN32  
	bool bfalse = false;
	std::vector<CWarCityTeam> vecCityTeam;
	for (size_t i = 0; i< m_vecBlowTeam.size(); i++)
	{
		CWarCityTeam cCityTeam = m_vecBlowTeam[i];

		if (bfalse)
		{
			vecCityTeam.push_back(cCityTeam);
			continue;
		}

		if (cCityTeam.m_nTeamID == idTeam)
		{
			bfalse = true;
		}
		else
		{
			vecCityTeam.push_back(cCityTeam);
		}
	}

	if (bfalse)
	{
		m_vecBlowTeam.clear();
		for (size_t i = 0; i < vecCityTeam.size(); i++)
			m_vecBlowTeam.push_back(vecCityTeam[i]);
	}

	return bfalse;

#else
	std::vector<CWarCityTeam>::iterator iter = m_vecBlowTeam.begin();
	for(; iter != m_vecBlowTeam.end(); ++iter)
	{
		if (iter->m_nTeamID == idTeam)
		{
			m_vecBlowTeam.erase(iter);
			return true;
		}
	}
	return false;
#endif

}

/**
* @功能：获取城战队伍列表
*/
void  CWarCity::getTeamList(std::vector<CTeamList> &vecAttTeamList, std::vector<CTeamList> &vecBlowTeamList)
{
	uint16 attnum = 0;
	
	//获取攻击方队伍列表
	this->getQueueTeamList(vecAttTeamList, m_vecAttQueue1Team);
	this->getQueueTeamList(vecAttTeamList, m_vecAttQueue2Team);
	this->getQueueTeamList(vecAttTeamList, m_vecAttQueue3Team);
	this->getCampTeamList(vecAttTeamList, m_vecAttTeam);
	if (vecAttTeamList.size() < this->getTeamListLmt())
	this->getCampTeamList(vecAttTeamList, m_vecAttNPCTeam);

	//获取防守方队伍列表
	this->getQueueTeamList(vecBlowTeamList, m_vecBlowQueue1Team);
	this->getQueueTeamList(vecBlowTeamList, m_vecBlowQueue2Team);
	this->getQueueTeamList(vecBlowTeamList, m_vecBlowQueue3Team);
	this->getCampTeamList(vecBlowTeamList, m_vecBlowTeam);
	if (vecBlowTeamList.size() < this->getTeamListLmt())
		this->getCampTeamList(vecBlowTeamList, m_vecBlowNPCTeam);
}

/**
* @功能：获取城战队伍列表
*/
void  CWarCity::getQueueTeamList(std::vector<CTeamList> &vecTeamList, std::vector<CCitySTeam> &vecQueueTeam)
{
	CTeamList cTeamList;
	for (size_t i = 0; i < vecQueueTeam.size(); i++)
	{
		CCitySTeam &cSTeam = vecQueueTeam[i];
		uint32 idTeam = cSTeam.m_nTeamID;
		std::map<uint32, CWarCityTeam>::iterator iter =  m_mapFightTeam.find(idTeam);
		if (iter == m_mapFightTeam.end())
			continue;

		CWarCityTeam &cCityTeam = iter->second;

		cTeamList.m_nLevel = cCityTeam.m_nLevel;
		cTeamList.m_nBatEffect = cCityTeam.m_nBattEffect;
		cTeamList.m_strName = cCityTeam.m_strName;
		cTeamList.m_nVisionFlag = cCityTeam.m_nVisionFlag;
		cTeamList.m_nStatus = 1;

		vecTeamList.push_back(cTeamList);
	}
}

/**
* @功能：获取城战队伍列表
*/
void  CWarCity::getCampTeamList(std::vector<CTeamList> &vecTeamList, std::vector<CWarCityTeam> &vecCityTeam)
{
	CTeamList cTeamList;
	uint16 teamnum = vecTeamList.size();
	for (size_t i = 0; i < vecCityTeam.size(); i++)
	{
		CWarCityTeam &cCityTeam = vecCityTeam[i];
		cTeamList.m_nLevel = cCityTeam.m_nLevel;
		cTeamList.m_nBatEffect = cCityTeam.m_nBattEffect;
		cTeamList.m_strName = cCityTeam.m_strName;
		cTeamList.m_nStatus = 0;
		cTeamList.m_nVisionFlag = cCityTeam.m_nVisionFlag;

		vecTeamList.push_back(cTeamList);

		teamnum++;
		if (teamnum >= this->getTeamListLmt())
			break;
	}

	return;
}


/**
* @功能：获取队伍信息
*/
uint16  CWarCity::getTeamInf(uint32 idTeam, uint8 &teamType, std::vector<CTeamDetail> &vecTeamDetail)
{
	uint16 nContinueKill = 0;
	std::map<uint32, CWarCityTeam>::iterator iter = m_mapFightTeam.find(idTeam);
	if (iter == m_mapFightTeam.end())
		return nContinueKill;

	CWarCityTeam &cCityTeam = iter->second;
	teamType = cCityTeam.m_nTeamType;

	CTeamDetail cTeamDetail;
    
	//怪物阵型
	if (teamType == TEAM_TYPE_NPC)
	{
		uint32 idNPC = cCityTeam.m_nNpcID;
		std::map<uint32, CWarNPCTeamPtr>::iterator iter1 = m_mapNpcTeam.find(idNPC);
		if (iter1 != m_mapNpcTeam.end())
		{
			CWarNPCTeamPtr pNpcTeamPtr = iter1->second;
			for (size_t i = 0; i < pNpcTeamPtr->m_vecGrid.size(); i++)
			{
				CWarNPCGrid &cGrid = pNpcTeamPtr->m_vecGrid[i];
				cTeamDetail.m_nID = cGrid.m_nMonsterID;
				cTeamDetail.m_nLevel = pNpcTeamPtr->m_nTeamLevel;
				cTeamDetail.m_nIsLive = 0;
				cTeamDetail.m_nClass = 0;
				for (size_t j = 0; j < cCityTeam.m_vecBatInf.size(); j++)
				{
					CTeamBatInf &batInf = cCityTeam.m_vecBatInf[j];
					if (batInf.m_nGrid == cGrid.m_nGrid)
					{
						if (batInf.m_nLastHP == 0)
						{
							cTeamDetail.m_nIsLive = 1;
							break;
						}
					}
				}

				vecTeamDetail.push_back(cTeamDetail);
			}
		}
	}
	else //玩家阵型
	{
		OBJID idPlayer = cCityTeam.m_nAvatarID;
		std::map<OBJID, CPlayerCityWarInfPtr>::iterator iter2 =  m_mapPlayerTeam.find(idPlayer);
		if (iter2 != m_mapPlayerTeam.end())
		{
			CPlayerCityWarInfPtr pPlayerWarInfPtr = iter2->second;
			if (pPlayerWarInfPtr->m_vecTeamInf.size() > 0)
			{
				for (size_t j = 0 ; j < pPlayerWarInfPtr->m_vecTeamInf.size(); j++)
				{
					CPlayerTeamInf &cTeamInf = pPlayerWarInfPtr->m_vecTeamInf[j];
					if (cTeamInf.m_nTeamID != idTeam)
						continue;
				for (size_t i = 0; i < cTeamInf.m_vecUint.size(); i++)
				{
					CFormatUint &cUint = cTeamInf.m_vecUint[i];
					cTeamDetail.m_nID = cUint.m_nConfId;
					cTeamDetail.m_nLevel = cUint.m_nLevel;
					cTeamDetail.m_nIsLive = 0;
					nContinueKill = cTeamInf.m_nContinKillNum;
					if (cUint.m_nCurBlood == 0)
						cTeamDetail.m_nIsLive = 1;
					cTeamDetail.m_nClass = cUint.m_nClass;
					vecTeamDetail.push_back(cTeamDetail);
					}
				}
			}
		}
	}

    return nContinueKill;
}

/**
* @功能：判断堆栈可以增加队伍
*/
bool  CWarCity::isAttQueueFull(uint8 nQueueNum)
{
	//判断堆栈状态是否都为摧毁
	if (m_nQueueStatus == 7)
		return true;

	switch (nQueueNum)
	{
	case CITY_QUEUE_NUM_FIRST:
		{
			if (m_vecAttQueue1Team.size() == 3)
				return true;

			if (m_nQueueStatus & 1)
				return true;

			return false;
		}
		break;
	case CITY_QUEUE_NUM_SECOND:
		{
			if (m_vecAttQueue2Team.size() == 3)
				return true;

			if (m_nQueueStatus & 2)
				return true;

			return false;
		}
		break;
	case CITY_QUEUE_NUM_THIRTY:
		{
			if (m_vecAttQueue3Team.size() == 3)
				return true;

			if (m_nQueueStatus & 4)
				return true;

			return false;
		}
		break;

	default:
		break;
	}

	return true;
}


/**
* @功能：从大本营增加作战队伍
*/
bool  CWarCity::addNewTeamToAttFightQueue()
{
	//如果是大本营已经为空了，则直接返回
	if (m_vecAttNPCTeam.size() == 0 && m_vecAttTeam.size() == 0)
		return false;

	if (this->isAttQueueFull(CITY_QUEUE_NUM_FIRST) && this->isAttQueueFull(CITY_QUEUE_NUM_SECOND) && this->isAttQueueFull(CITY_QUEUE_NUM_THIRTY))
		return false;
#ifdef WIN32  
	if  (m_vecAttTeam.size() >= 0)
	{
		std::vector<CWarCityTeam> vecNpcTeam;
		bool bfalse = false;
		for (size_t i = 0;  i < m_vecAttTeam.size(); i++)
		{
			CWarCityTeam &cNpcTeam = m_vecAttTeam[i];
			uint32 idTeam = cNpcTeam.m_nTeamID;
			if (!bfalse)
			{
				std::map<uint32, CCityPickTeam>::iterator iter_fight = m_mapPickTeam.find(idTeam);
				if (iter_fight == m_mapPickTeam.end())
				{
					m_mapFightTeam.insert(make_pair(cNpcTeam.m_nTeamID, cNpcTeam));
					this->addTeamToAttQueueOrder(cNpcTeam);
					bfalse = true;
				}
				else
				{
					vecNpcTeam.push_back(cNpcTeam);
				}
			}
			else
			{
				vecNpcTeam.push_back(cNpcTeam);
			}
		}
		if (bfalse)
		{
			m_vecAttTeam.clear();
			for (size_t i = 0;  i < vecNpcTeam.size(); i++)
				m_vecAttTeam.push_back(vecNpcTeam[i]);
			return true;
		}
	}
#else
	if (m_vecAttTeam.size() >= 0)
	{
		std::vector<CWarCityTeam>::iterator iter =  m_vecAttTeam.begin();
		for( ; iter != m_vecAttTeam.end(); ++iter)
		{
			uint32 idTeam = iter->m_nTeamID;
			std::map<uint32, CCityPickTeam>::iterator iter_fight =  m_mapPickTeam.find(idTeam);
			if (iter_fight == m_mapPickTeam.end())
			{
				CWarCityTeam cCityTeam = *iter;
				m_mapFightTeam.insert(make_pair(cCityTeam.m_nTeamID, cCityTeam));
				this->addTeamToAttQueueOrder(cCityTeam);
				m_vecAttTeam.erase(iter);
				return true;
			}
		}
	}
#endif
	return this->addNPCTeamToAttQueue();
}
bool CWarCity::addNPCTeamToAttQueue()
{
#ifdef WIN32  
	std::vector<CWarCityTeam> vecNpcTeam;
	bool bfalse = false;

	for (size_t i = 0;  i < m_vecAttNPCTeam.size(); i++)
	{
		CWarCityTeam &cNpcTeam = m_vecAttNPCTeam[i];
		uint32 idTeam = cNpcTeam.m_nTeamID;

		if (!bfalse)
		{
			//判断是否在战斗中
			std::map<uint32, CCityPickTeam>::iterator iter_fight = m_mapPickTeam.find(idTeam);
			if (iter_fight == m_mapPickTeam.end())
			{
				m_mapFightTeam.insert(make_pair(cNpcTeam.m_nTeamID, cNpcTeam));
				this->addTeamToAttQueueOrder(cNpcTeam);
				bfalse = true;
			}
			else
			{
				vecNpcTeam.push_back(cNpcTeam);
			}
		}
		else
		{
			vecNpcTeam.push_back(cNpcTeam);
		}
	}

	if (bfalse)
	{
		m_vecAttNPCTeam.clear();
		for (size_t i = 0; i < vecNpcTeam.size(); i++)
			m_vecAttNPCTeam.push_back(vecNpcTeam[i]);
	}

	return bfalse;
#else
	//获取可以进入战斗队列的队伍
	std::vector<CWarCityTeam>::iterator iter =  m_vecAttNPCTeam.begin();
	for( ; iter != m_vecAttNPCTeam.end(); iter++)
	{
		uint32 idTeam = iter->m_nTeamID;
		//判断是否在战斗中
		std::map<uint32, CCityPickTeam>::iterator iter_fight = m_mapPickTeam.find(idTeam);
		if (iter_fight == m_mapPickTeam.end())
		{
			CWarCityTeam cCityTeam = *iter;
			m_mapFightTeam.insert(make_pair(cCityTeam.m_nTeamID, cCityTeam));
			this->addTeamToAttQueueOrder(cCityTeam);
			m_vecAttNPCTeam.erase(iter);
			return true;
		}
	}

	return false;
#endif
}

/**
* @功能：判断防守堆栈可以增加队伍
*/
bool  CWarCity::isBlowQueueFull(uint8 nQueueNum)
{
	//判断堆栈状态是否都为摧毁
	if (m_nQueueStatus == 7)
		return true;

	switch (nQueueNum)
	{
	case CITY_QUEUE_NUM_FIRST:
		{
			if (m_vecBlowQueue1Team.size() == 3)
				return true;

			if (m_nQueueStatus & 1)
				return true;

			return false;
		}
		break;
	case CITY_QUEUE_NUM_SECOND:
		{
			if (m_vecBlowQueue2Team.size() == 3)
				return true;

			if (m_nQueueStatus & 2)
				return true;

			return false;
		}
		break;
	case CITY_QUEUE_NUM_THIRTY:
		{
			if (m_vecBlowQueue3Team.size() == 3)
				return true;

			if (m_nQueueStatus & 4)
				return true;

			return false;
		}
		break;

	default:
		break;
	}

	return true;
}


/**
* @功能：增加作战队伍到攻击堆栈
*/
void  CWarCity::addTeamToAttQueueOrder(CWarCityTeam &cCityTeam)
{
	uint8 queuenum = CITY_QUEUE_NUM_THIRTY;

	if (m_vecAttQueue2Team.size() <= m_vecAttQueue3Team.size())
	{
		if (!(m_nQueueStatus & 2))
			queuenum = CITY_QUEUE_NUM_SECOND;
	}

	if (queuenum == CITY_QUEUE_NUM_THIRTY)
	{
		if (m_vecAttQueue1Team.size() <= m_vecAttQueue3Team.size())
		{
			if (!(m_nQueueStatus & 1))
				queuenum = CITY_QUEUE_NUM_FIRST;
		}
	}
	else
	{
		if (m_vecAttQueue1Team.size() <= m_vecAttQueue2Team.size())
		{
			if (!(m_nQueueStatus & 1))
				queuenum = CITY_QUEUE_NUM_FIRST;
		}
	}

	CCitySTeam cSTeam;
	cSTeam.m_nSex = cCityTeam.m_nSex;
	cSTeam.m_nTeamID = cCityTeam.m_nTeamID;
	cSTeam.m_strName = cCityTeam.m_strName;
	cSTeam.m_nNpcId = cCityTeam.m_nNpcID;
	cSTeam.m_nTotHP = cCityTeam.m_nTotHP;
	cSTeam.m_nLastHP = cCityTeam.m_nLastHP;
	cSTeam.m_nLevel = cCityTeam.m_nLevel;
	cSTeam.m_nVisionFlag = cCityTeam.m_nVisionFlag;
	cSTeam.m_nNation = cCityTeam.m_nNation;
	cSTeam.m_nTitleID = cCityTeam.m_nTitleID;

	if (queuenum == CITY_QUEUE_NUM_FIRST)
		m_vecAttQueue1Team.push_back(cSTeam);
	else if (queuenum == CITY_QUEUE_NUM_SECOND)
		m_vecAttQueue2Team.push_back(cSTeam);
	else
		m_vecAttQueue3Team.push_back(cSTeam);
    
	return;
}

/**
* @功能：从大本营增加作战队伍
*/
bool  CWarCity::addNewTeamToBlowFightQueue()
{
	//如果是大本营已经为空了，则直接返回
	if (m_vecBlowNPCTeam.size() == 0 && m_vecBlowTeam.size() == 0)
		return false;

	if (this->isBlowQueueFull(CITY_QUEUE_NUM_FIRST) && this->isBlowQueueFull(CITY_QUEUE_NUM_SECOND) && this->isBlowQueueFull(CITY_QUEUE_NUM_THIRTY))
		return false;

#ifdef WIN32  
	if  (m_vecBlowTeam.size() >= 0)
	{
		std::vector<CWarCityTeam> vecNpcTeam;
		bool bfalse = false;

		for (size_t i = 0;  i < m_vecBlowTeam.size(); i++)
		{
			CWarCityTeam &cNpcTeam = m_vecBlowTeam[i];
			uint32 idTeam = cNpcTeam.m_nTeamID;

			if (!bfalse)
			{
				//判断是否在战斗中
				std::map<uint32, CCityPickTeam>::iterator iter_fight = m_mapPickTeam.find(idTeam);
				if (iter_fight == m_mapPickTeam.end())
				{
					m_mapFightTeam.insert(make_pair(cNpcTeam.m_nTeamID, cNpcTeam));
					this->addTeamToBlowQueueOrder(cNpcTeam);
					bfalse = true;
				}
				else
				{
					vecNpcTeam.push_back(cNpcTeam);
				}
			}
			else
			{
				vecNpcTeam.push_back(cNpcTeam);
			}
		}

		if (bfalse)
		{
			m_vecBlowTeam.clear();
			for (size_t i = 0;  i < vecNpcTeam.size(); i++)
				m_vecBlowTeam.push_back(vecNpcTeam[i]);
			return true;
		}
	}

#else
	//获取可以进入战斗队列的队伍
	if (m_vecBlowTeam.size() >= 0)
	{
		std::vector<CWarCityTeam>::iterator iter =  m_vecBlowTeam.begin();
		for( ; iter != m_vecBlowTeam.end(); ++iter)
		{
			uint32 idTeam = iter->m_nTeamID;
			//判断是否在战斗中
			std::map<uint32, CCityPickTeam>::iterator iter_fight =  m_mapPickTeam.find(idTeam);
			if (iter_fight == m_mapPickTeam.end())
			{
				CWarCityTeam cCityTeam = *iter;
				m_mapFightTeam.insert(make_pair(cCityTeam.m_nTeamID, cCityTeam));
				this->addTeamToBlowQueueOrder(cCityTeam);
				m_vecBlowTeam.erase(iter);
				return true;
			}
		}
	}

#endif

	return this->addNPCTeamToBlowQueue();
}

/**
* @功能：增加作战队伍到攻击堆栈
*/
bool  CWarCity::addNPCTeamToBlowQueue()
{
#ifdef WIN32  
	std::vector<CWarCityTeam> vecNpcTeam;
	bool bfalse = false;

	for (size_t i = 0;  i < m_vecBlowNPCTeam.size(); i++)
	{
		CWarCityTeam &cNpcTeam = m_vecBlowNPCTeam[i];
		uint32 idTeam = cNpcTeam.m_nTeamID;

		if (!bfalse)
		{
			//判断是否在战斗中
			std::map<uint32, CCityPickTeam>::iterator iter_fight = m_mapPickTeam.find(idTeam);
			if (iter_fight == m_mapPickTeam.end())
			{
				m_mapFightTeam.insert(make_pair(cNpcTeam.m_nTeamID, cNpcTeam));
				this->addTeamToBlowQueueOrder(cNpcTeam);
				bfalse = true;
			}
			else
			{
				vecNpcTeam.push_back(cNpcTeam);
			}
		}
		else
		{
			vecNpcTeam.push_back(cNpcTeam);
		}
	}

	if (bfalse)
	{
		m_vecBlowNPCTeam.clear();
		for (size_t i = 0; i < vecNpcTeam.size(); i++)
			m_vecBlowNPCTeam.push_back(vecNpcTeam[i]);
	}
	return bfalse;
#else

	std::vector<CWarCityTeam>::iterator iter =  m_vecBlowNPCTeam.begin();
	for( ; iter != m_vecBlowNPCTeam.end(); ++iter)
	{
		uint32 idTeam = iter->m_nTeamID;
		//判断是否在战斗中
		std::map<uint32, CCityPickTeam>::iterator iter_fight =  m_mapPickTeam.find(idTeam);
		if (iter_fight == m_mapPickTeam.end())
		{
			CWarCityTeam cCityTeam = *iter;
			m_mapFightTeam.insert(make_pair(cCityTeam.m_nTeamID, cCityTeam));
			this->addTeamToBlowQueueOrder(cCityTeam);
			m_vecBlowNPCTeam.erase(iter);
			return true;
		}
	}

	return false;
#endif
}

/**
* @功能：增加作战队伍到攻击堆栈
*/
void  CWarCity::addTeamToBlowQueueOrder(CWarCityTeam &cCityTeam)
{
	uint8 queuenum = CITY_QUEUE_NUM_THIRTY;
	if (m_vecBlowQueue2Team.size() <= m_vecBlowQueue3Team.size())
	{
		if (!(m_nQueueStatus & 2))
			queuenum = CITY_QUEUE_NUM_SECOND;
	}
	else if (m_nQueueStatus & 4)
	{
		queuenum = CITY_QUEUE_NUM_SECOND;
	}
	if (queuenum == CITY_QUEUE_NUM_THIRTY)
	{
		if (m_vecBlowQueue1Team.size() <= m_vecBlowQueue3Team.size())
		{
			if (!(m_nQueueStatus & 1))
				queuenum = CITY_QUEUE_NUM_FIRST;
		}
		else if (m_nQueueStatus & 4)
		{
			queuenum = CITY_QUEUE_NUM_FIRST;
		}
	}
	else
	{
		if (m_vecBlowQueue1Team.size() <= m_vecBlowQueue2Team.size())
		{
			if (!(m_nQueueStatus & 1))
				queuenum = CITY_QUEUE_NUM_FIRST;
		}
		else if (m_nQueueStatus & 2)
		{
				queuenum = CITY_QUEUE_NUM_FIRST;
		}
	}

	this->addTeamToBlowQueue(queuenum, cCityTeam);

	this->incQueueSessionID(queuenum);

	return;
}

/**
* @功能：增加作战队伍到防守堆栈
*/
void  CWarCity::addTeamToBlowQueue(uint8 nQueueNum, CWarCityTeam &cCityTeam)
{
	uint32 idTeam = cCityTeam.m_nTeamID;

	CCitySTeam cSteam;
	cSteam.m_nTeamID = idTeam;
	cSteam.m_nNpcId = cCityTeam.m_nNpcID;
	cSteam.m_strName = cCityTeam.m_strName;
	cSteam.m_nSex = cCityTeam.m_nSex;
	cSteam.m_nTotHP = cCityTeam.m_nTotHP;
	cSteam.m_nLastHP = cCityTeam.m_nLastHP;
	cSteam.m_nLevel = cCityTeam.m_nLevel;
	cSteam.m_nVisionFlag = cCityTeam.m_nVisionFlag;
	cSteam.m_nNation = cCityTeam.m_nNation;
	cSteam.m_nTitleID = cCityTeam.m_nTitleID;
	if (nQueueNum == CITY_QUEUE_NUM_FIRST)
		m_vecBlowQueue1Team.push_back(cSteam);
	else if (nQueueNum == CITY_QUEUE_NUM_SECOND)
		m_vecBlowQueue2Team.push_back(cSteam);
	else
		m_vecBlowQueue3Team.push_back(cSteam);

	return;
}


/**
* @功能：增加作战队伍到攻击堆栈
*/
void  CWarCity::addTeamToAttQueue(uint8 nQueueNum, CWarCityTeam &cCityTeam)
{
	uint32 idTeam = cCityTeam.m_nTeamID;

	CCitySTeam cSteam;
	cSteam.m_nTeamID = idTeam;
	cSteam.m_nNpcId = cCityTeam.m_nNpcID;
	cSteam.m_strName = cCityTeam.m_strName;
	cSteam.m_nSex = cCityTeam.m_nSex;
	cSteam.m_nLastHP = cCityTeam.m_nLastHP;
	cSteam.m_nTotHP = cCityTeam.m_nTotHP;
	cSteam.m_nLevel = cCityTeam.m_nLevel;
	cSteam.m_nVisionFlag = cCityTeam.m_nVisionFlag;
	cSteam.m_nNation = cCityTeam.m_nNation;
	cSteam.m_nTitleID = cCityTeam.m_nTitleID;
	if (nQueueNum == CITY_QUEUE_NUM_FIRST)
		m_vecAttQueue1Team.push_back(cSteam);
	else if (nQueueNum == CITY_QUEUE_NUM_SECOND)
		m_vecAttQueue2Team.push_back(cSteam);
	else
		m_vecAttQueue3Team.push_back(cSteam);

	return;
}

/**
* @功能：转化的队伍
*/
bool CWarCity::convCityTeamBatTeam(CWarCityTeam &cCityTeam, CWorldBatTeam &cBatTeam)
{
	cBatTeam.m_nTeamID = cCityTeam.m_nTeamID;
	cBatTeam.m_nType = cCityTeam.m_nTeamType;
	if (cBatTeam.m_nType == TEAM_TYPE_NPC)
	{
		CWarNPCFightTeam cNpcFightTeam;
		cNpcFightTeam.m_nID = cCityTeam.m_nNpcID;
		for (size_t i = 0; i < cCityTeam.m_vecBatInf.size(); i++)
			cNpcFightTeam.m_vecGrid.push_back(cCityTeam.m_vecBatInf[i]);
		cBatTeam.m_vecNpcInf.push_back(cNpcFightTeam);
	}
	else
	{
		OBJID idPlayer = cCityTeam.m_nAvatarID;
		//获取城战城池信息
		std::map<OBJID, CPlayerCityWarInfPtr>::iterator iter1  =  m_mapPlayerTeam.find(idPlayer);
		if (iter1 == m_mapPlayerTeam.end())
			return false;

		CPlayerCityWarInfPtr pPlayerWarInfPtr  = iter1->second;
		if (pPlayerWarInfPtr->m_vecTeamInf.size() == 0)
			return false;
		CPlayerTeamInf cTeamInf;
		for (size_t i = 0; i <  pPlayerWarInfPtr->m_vecTeamInf.size(); i++)
		{
			cTeamInf = pPlayerWarInfPtr->m_vecTeamInf[i];
			if (cTeamInf.m_nTeamID = cCityTeam.m_nTeamID)
				break;
		}
		CPlayerFightTeamInf cPlayerTeam;
		cPlayerTeam.m_nPlayerID = idPlayer;
		cPlayerTeam.m_nContinuKill = cTeamInf.m_nContinKillNum;
		cPlayerTeam.m_nBattEffect = cCityTeam.m_nBattEffect;
		cPlayerTeam.m_strName = pPlayerWarInfPtr->m_strName;
		cPlayerTeam.m_nLevel = pPlayerWarInfPtr->m_nLevel;
		cPlayerTeam.m_nSex = pPlayerWarInfPtr->m_nSex;
		for (size_t i = 0; i < cTeamInf.m_vecUint.size(); i++)
			cPlayerTeam.m_vecUint.push_back(cTeamInf.m_vecUint[i]);

		for (size_t i = 0; i < cTeamInf.m_vecBuddy.size(); i++)
			cPlayerTeam.m_vecBuddy.push_back(cTeamInf.m_vecBuddy[i]);

		for (size_t i = 0; i < cTeamInf.m_vecDragonSKill.size(); i++)
			cPlayerTeam.m_vecDragonSKill.push_back(cTeamInf.m_vecDragonSKill[i]);

		cPlayerTeam.m_nDragonLevel = cTeamInf.m_nDragonLevel;
		cBatTeam.m_vecPlayerInf.push_back(cPlayerTeam);
	}
    
	return true;
}


/**
* @功能：转化攻击方的队伍
*/
bool CWarCity::convQueueTeamBatTeam(CCitySTeam &cSTeam, CWorldBatTeam &cAttBatTeam)
{
	uint32 idTeam = cSTeam.m_nTeamID;
	cAttBatTeam.m_nTeamID = idTeam;

	//获取队伍信息
	std::map<uint32, CWarCityTeam>::iterator iter =  m_mapFightTeam.find(idTeam);
	if (iter == m_mapFightTeam.end())
		return false;

	CWarCityTeam &cCityTeam = iter->second;

	cAttBatTeam.m_nType = cCityTeam.m_nTeamType;
	if (cAttBatTeam.m_nType == TEAM_TYPE_NPC)
	{
		CWarNPCFightTeam cNpcFightTeam;
		cNpcFightTeam.m_nID = cCityTeam.m_nNpcID;
		for (size_t i = 0; i < cCityTeam.m_vecBatInf.size(); i++)
		    cNpcFightTeam.m_vecGrid.push_back(cCityTeam.m_vecBatInf[i]);
		cAttBatTeam.m_vecNpcInf.push_back(cNpcFightTeam);
	}
	else
	{
		OBJID idPlayer = cCityTeam.m_nAvatarID;
        //获取城战城池信息
		std::map<OBJID, CPlayerCityWarInfPtr>::iterator iter1  =  m_mapPlayerTeam.find(idPlayer);
		if (iter1 == m_mapPlayerTeam.end())
			return false;

		CPlayerCityWarInfPtr pPlayerWarInfPtr  = iter1->second;
		if (pPlayerWarInfPtr->m_vecTeamInf.size() == 0)
			return false;

		for (size_t j = 0; j < pPlayerWarInfPtr->m_vecTeamInf.size(); j++)
		{
			CPlayerTeamInf &cTeamInf = pPlayerWarInfPtr->m_vecTeamInf[j];
			if (cTeamInf.m_nTeamID != idTeam)
				continue;

			CPlayerFightTeamInf cPlayerTeam;
			cPlayerTeam.m_nPlayerID = idPlayer;
			cPlayerTeam.m_nContinuKill = cTeamInf.m_nContinKillNum;
			cPlayerTeam.m_nBattEffect = cCityTeam.m_nBattEffect;
			for (size_t i = 0; i < cTeamInf.m_vecUint.size(); i++)
				cPlayerTeam.m_vecUint.push_back(cTeamInf.m_vecUint[i]);

			for (size_t i = 0; i < cTeamInf.m_vecBuddy.size(); i++)
				cPlayerTeam.m_vecBuddy.push_back(cTeamInf.m_vecBuddy[i]);

			for (size_t i = 0; i < cTeamInf.m_vecDragonSKill.size(); i++)
				cPlayerTeam.m_vecDragonSKill.push_back(cTeamInf.m_vecDragonSKill[i]);

			cPlayerTeam.m_nDragonLevel = cTeamInf.m_nDragonLevel;
			cAttBatTeam.m_vecPlayerInf.push_back(cPlayerTeam);
		}	
	}

    return true; 
}

/**
* @功能：删除作战前面的第一个队伍
*/
void  CWarCity::eraseQueueFistTeam(uint8 nQueueNum, uint32 idTeam, uint8 nType)
{
	CCitySTeam cSTeam;

	//获取队伍信息
	std::map<uint32, CWarCityTeam>::iterator iter = m_mapFightTeam.find(idTeam);
	if (iter == m_mapFightTeam.end())
		return;

	CWarCityTeam &cCityTeam = iter->second;

	if (nQueueNum == 1)
	{
		//删除攻击方
		if (nType == 0)
		{
			this->getAttQueue1FirstTeam(cSTeam);
			this->eraseAttQueue1Team();
		}
		else  //删除防守方
		{
			this->getBlowQueue1FirstTeam(cSTeam);
			this->eraseBlowQueue1Team();
		}
	}
	else if (nQueueNum == 2)
	{
		//删除攻击方
		if (nType == 0)
		{
			this->getAttQueue2FirstTeam(cSTeam);
			this->eraseAttQueue2Team();
		}
		else  //删除防守方
		{
			this->getBlowQueue2FirstTeam(cSTeam);
			this->eraseBlowQueue2Team();
		}
	}
	else
	{
		//删除攻击方
		if (nType == 0)
		{
			this->getAttQueue3FirstTeam(cSTeam);
			this->eraseAttQueue3Team();
		}
		else  //删除防守方
		{
			this->getBlowQueue3FirstTeam(cSTeam);
			this->eraseBlowQueue3Team();
		}
	}

	//如果是玩家， 需要删除玩家的信息
	if (cCityTeam.m_nTeamType == TEAM_TYPE_PLAYER)
	{
		//找到玩家的信息
		CPlayerCityWarInfPtr pPlayerWarPtr = this->getJoinPlayer(cCityTeam.m_nAvatarID);
		if (pPlayerWarPtr != NULL)
		{
			std::vector<CPlayerTeamInf>::iterator iter_team = pPlayerWarPtr->m_vecTeamInf.begin();
			for(; iter_team != pPlayerWarPtr->m_vecTeamInf.end(); ++iter_team)
			{
				if(iter_team->m_nTeamID == idTeam)
				{
					pPlayerWarPtr->m_vecTeamInf.erase(iter_team);
					break;
				}
			}
		}
	}

	return;
}


/**
* @功能：保存战报
*/
void CWarCity::setQueueBatReport(uint8 nQueueNum, std::string strBatReport)
{
	if (nQueueNum == CITY_QUEUE_NUM_FIRST)
		m_strBattleReport1 = strBatReport;
	else if (nQueueNum == CITY_QUEUE_NUM_SECOND)
		m_strBattleReport2 = strBatReport;
	else
		m_strBattleReport3 = strBatReport;

	return;
}

/**
* @功能：保存战斗简报
*/
void  CWarCity::setQueueTeamBatReport(uint8 nQueueNum, CTeamBatReport &cTeamBatReport)
{
	if (nQueueNum == CITY_QUEUE_NUM_FIRST)
	{
		cbatReport1 = cTeamBatReport;
		m_nQueueNextTime1 = cTeamBatReport.m_nNextTime;
		this->getSession1();
	}
	else if (nQueueNum == CITY_QUEUE_NUM_SECOND)
	{
		cbatReport2 = cTeamBatReport;
		m_nQueueNextTime2 = cTeamBatReport.m_nNextTime;
		this->getSession2();
	}
	else
	{
		cbatReport3 = cTeamBatReport;
		m_nQueueNextTime3 = cTeamBatReport.m_nNextTime;
		this->getSession3();
	}

	return;
}

bool CWarCity::getQueueTeamBatReport(uint8 nQueueNum, CTeamBatReport &cTeamBatReport)
{
	if (nQueueNum == CITY_QUEUE_NUM_FIRST)
	{
		if (m_strBattleReport1.size() < 5)
			return false;
		cTeamBatReport = cbatReport1;
		return true;
	}
	else if (nQueueNum == CITY_QUEUE_NUM_SECOND)
	{
		if (m_strBattleReport2.size() < 5)
			return false;
		cTeamBatReport = cbatReport2;
		return true;
	}
	else
	{
		if (m_strBattleReport3.size() < 5)
			return false;
		cTeamBatReport = cbatReport3;
		return true;
	}

	return false;
}


/**
* @功能：修改队伍的战斗阵型信息
*/
void CWarCity::updSucTeamGrid(uint8 nQueueNum, uint32 idTeam)
{
	switch (nQueueNum)
	{
	case CITY_QUEUE_NUM_FIRST:
		{
			this->updTeamBatGrid(idTeam, m_vecGrid1);
		}
		break;
	case CITY_QUEUE_NUM_SECOND:
		{
			this->updTeamBatGrid(idTeam, m_vecGrid2);
		}
		break;
	case CITY_QUEUE_NUM_THIRTY:
		{
			this->updTeamBatGrid(idTeam, m_vecGrid3);
		}
		break;
	default:
		break;
	}

	return;
}

/**
* @功能：修改队伍的战斗阵型信息
*/
void  CWarCity::updTeamBatGrid(uint32 idTeam, std::vector<CTeamBatInf> &vecGrid)
{

	std::map<uint32, CWarCityTeam>::iterator iter = m_mapFightTeam.find(idTeam);
	if (iter != m_mapFightTeam.end())
	{

		CWarCityTeam &cCityTeam = iter->second;
		cCityTeam.m_vecBatInf.clear();
		for (size_t i = 0; i < vecGrid.size(); i++)
			cCityTeam.m_vecBatInf.push_back(vecGrid[i]);

		if (cCityTeam.m_nTeamType == TEAM_TYPE_PLAYER)
		{
			std::map<OBJID, CPlayerCityWarInfPtr>::iterator iter1 = m_mapPlayerTeam.find(cCityTeam.m_nAvatarID);
			if (iter1 == m_mapPlayerTeam.end())
				return;

			CPlayerCityWarInfPtr pPlayerWarPtr = iter1->second;
			if (pPlayerWarPtr == NULL)
				return;

			pPlayerWarPtr->m_nHaveFight  = 1;
			if (pPlayerWarPtr->m_vecTeamInf.size()  == 0)
				return;

			for (size_t j = 0; j < pPlayerWarPtr->m_vecTeamInf.size(); j++)
			{
				CPlayerTeamInf &cTeamInf = pPlayerWarPtr->m_vecTeamInf[j];
				if (cTeamInf.m_nTeamID != idTeam)
					continue;
			for (size_t i  = 0;i < cTeamInf.m_vecUint.size(); i++)
			{
				CFormatUint &cUint = cTeamInf.m_vecUint[i];
				for (size_t j = 0; j < vecGrid.size(); j++)
				{
					CTeamBatInf &cBatInf = vecGrid[j];
					if (cBatInf.m_nGrid == cUint.m_nGrid)
					{
						cUint.m_nCurBlood = cBatInf.m_nLastHP;
						cUint.m_nCurEn = cBatInf.m_nLastEn;
						continue;
						}
					}
				}
			}
		}

		return;
	}

	std::map<uint32, CCityPickTeam>::iterator iter_pick =  m_mapPickTeam.find(idTeam);
	if (iter_pick != m_mapPickTeam.end())
	{
		CCityPickTeam &cPickTeam = iter_pick->second;
		CWarCityTeam &cCityTeam = cPickTeam.m_cTeam;
		cCityTeam.m_vecBatInf.clear();
		for (size_t i = 0; i < vecGrid.size(); i++)
			cCityTeam.m_vecBatInf.push_back(vecGrid[i]);

		if (cCityTeam.m_nTeamType == TEAM_TYPE_PLAYER)
		{
			std::map<OBJID, CPlayerCityWarInfPtr>::iterator iter1 = m_mapPlayerTeam.find(cCityTeam.m_nAvatarID);
			if (iter1 == m_mapPlayerTeam.end())
				return;

			CPlayerCityWarInfPtr pPlayerWarPtr = iter1->second;
			if (pPlayerWarPtr == NULL)
				return;

			pPlayerWarPtr->m_nHaveFight  = 1;
			if (pPlayerWarPtr->m_vecTeamInf.size()  == 0)
				return;

			for (size_t j = 0; j < pPlayerWarPtr->m_vecTeamInf.size(); j++)
			{
				CPlayerTeamInf &cTeamInf = pPlayerWarPtr->m_vecTeamInf[j];
				if (cTeamInf.m_nTeamID != idTeam)
					continue;
				for (size_t i  = 0;i < cTeamInf.m_vecUint.size(); i++)
				{
					CFormatUint &cUint = cTeamInf.m_vecUint[i];
					for (size_t j = 0; j < vecGrid.size(); j++)
					{
						CTeamBatInf &cBatInf = vecGrid[j];
						if (cBatInf.m_nGrid == cUint.m_nGrid)
						{
							cUint.m_nCurBlood = cBatInf.m_nLastHP;
							cUint.m_nCurEn = cBatInf.m_nLastEn;
							continue;
						}
					}
				}
			}
		}

		return;
	}

	return;
}

/**
* @功能：清理堆栈双方失败的队伍
*/
void CWarCity::clearQueueAllFailTeam(uint8 nQueueNum)
{
	switch (nQueueNum)
	{
	case CITY_QUEUE_NUM_FIRST:
		{
			this->eraseAttQueue1Team();
			this->getCamTeamToAttQueue(nQueueNum);

			this->eraseBlowQueue1Team();
			this->getCamTeamToBlowQueue(nQueueNum);

			if (m_vecBlowNPCTeam.size() == 0 && m_vecBlowTeam.size() == 0 && m_vecBlowQueue1Team.size() == 0 && m_mapPickTeam.size() == 0)
			{
				m_nQueueStatus = m_nQueueStatus|1;
				m_nQueueFail = m_nQueueFail | 1; 
				m_strBattleReport1 = "";
				this->teamRebackCamp(TEAM_ATT_TYPE_ATT, m_vecAttQueue1Team);
			}

			if (m_vecAttNPCTeam.size() == 0 && m_vecAttTeam.size() == 0 && m_vecAttQueue1Team.size() == 0 && m_mapPickTeam.size() == 0)
			{
				m_nQueueStatus = m_nQueueStatus|1;
				m_strBattleReport1 = "";
				this->teamRebackCamp(TEAM_ATT_TYPE_DEF, m_vecBlowQueue1Team);
			}
		}
		break;
	case CITY_QUEUE_NUM_SECOND:
		{
			this->eraseAttQueue2Team();
			this->getCamTeamToAttQueue(nQueueNum);

			this->eraseBlowQueue2Team();
			this->getCamTeamToBlowQueue(nQueueNum);

			if (m_vecBlowNPCTeam.size() == 0 && m_vecBlowTeam.size() == 0 && m_vecBlowQueue2Team.size() == 0 && m_mapPickTeam.size() == 0)
			{
				m_nQueueStatus = m_nQueueStatus|2;
				m_nQueueFail = m_nQueueFail | 2; 
				m_strBattleReport2 = "";
				this->teamRebackCamp(TEAM_ATT_TYPE_ATT, m_vecAttQueue2Team);
			}

			if (m_vecAttNPCTeam.size() == 0 && m_vecAttTeam.size() == 0 && m_vecAttQueue2Team.size() == 0 && m_mapPickTeam.size() == 0)
			{
				m_nQueueStatus = m_nQueueStatus|2;
				m_strBattleReport2 = "";
				this->teamRebackCamp(TEAM_ATT_TYPE_DEF, m_vecBlowQueue2Team);
			}
		}
		break;
	default:
		{
			this->eraseAttQueue3Team();
			this->getCamTeamToAttQueue(nQueueNum);

			this->eraseBlowQueue3Team();
			this->getCamTeamToBlowQueue(nQueueNum);

			if (m_vecBlowNPCTeam.size() == 0 && m_vecBlowTeam.size() == 0 && m_vecBlowQueue3Team.size() == 0 && m_mapPickTeam.size() == 0)
			{
				m_nQueueStatus = m_nQueueStatus|4;
				m_nQueueFail = m_nQueueFail | 4; 
				m_strBattleReport3 = "";
				this->teamRebackCamp(TEAM_ATT_TYPE_ATT, m_vecAttQueue3Team);
			}

			if (m_vecAttNPCTeam.size() == 0 && m_vecAttTeam.size() == 0 && m_vecAttQueue3Team.size() == 0 && m_mapPickTeam.size() == 0)
			{
				m_nQueueStatus = m_nQueueStatus|4;
				m_strBattleReport3 = "";
				this->teamRebackCamp(TEAM_ATT_TYPE_DEF, m_vecBlowQueue3Team);
			}
		}
		break;
	}

	return;
    
}

/**
* @功能：清理堆栈失败的队伍
*/
void CWarCity::clearQueueFailTeam(uint8 nQueueNum)
{
	switch (nQueueNum)
	{
	case CITY_QUEUE_NUM_FIRST:
		{
			if (m_strBattleReport1.size() < 2)
				return;

			uint8 faildir = cbatReport1.m_nFailDir;
			//失败方 1攻击者 2防守者
			if (faildir == WAR_DIR_TYPE_ATT) 
			{
				this->eraseAttQueue1Team();
				this->getCamTeamToAttQueue(nQueueNum);

				if (m_vecAttNPCTeam.size() == 0 && m_vecAttTeam.size() == 0 && m_vecAttQueue1Team.size() == 0 && m_mapPickTeam.size() == 0)
				{
					m_nQueueStatus = m_nQueueStatus|1;
					m_strBattleReport1 = "";
					this->teamRebackCamp(TEAM_ATT_TYPE_DEF, m_vecBlowQueue1Team);
				}
			}
			else
			{
				this->eraseBlowQueue1Team();
				this->getCamTeamToBlowQueue(nQueueNum);

				if (m_vecBlowNPCTeam.size() == 0 && m_vecBlowTeam.size() == 0 && m_vecBlowQueue1Team.size() == 0 && m_mapPickTeam.size() == 0)
				{
					m_nQueueStatus = m_nQueueStatus|1;
					m_nQueueFail = m_nQueueFail | 1;
					m_strBattleReport1 = "";
					this->teamRebackCamp(TEAM_ATT_TYPE_ATT, m_vecAttQueue1Team);
				}
			}
		}
		break;
	case CITY_QUEUE_NUM_SECOND:
		{
			if (m_strBattleReport2.size() < 2)
				return;

			uint8 faildir = cbatReport2.m_nFailDir;
			//失败方 1攻击者 2防守者
			if (faildir == WAR_DIR_TYPE_ATT) 
			{
				this->eraseAttQueue2Team();
				this->getCamTeamToAttQueue(nQueueNum);

				if (m_vecAttNPCTeam.size() == 0 && m_vecAttTeam.size() == 0 && m_vecAttQueue2Team.size() == 0 && m_mapPickTeam.size() == 0)
				{
					m_nQueueStatus = m_nQueueStatus|2;
					m_strBattleReport2 = "";
					this->teamRebackCamp(TEAM_ATT_TYPE_DEF, m_vecBlowQueue2Team);
				}
			}
			else
			{
				this->eraseBlowQueue2Team();
				this->getCamTeamToBlowQueue(nQueueNum);

				if (m_vecBlowNPCTeam.size() == 0 && m_vecBlowTeam.size() == 0 && m_vecBlowQueue2Team.size() == 0 && m_mapPickTeam.size() == 0)
				{
					m_nQueueStatus = m_nQueueStatus|2;
					m_nQueueFail = m_nQueueFail | 2; 
					m_strBattleReport2 = "";
					this->teamRebackCamp(TEAM_ATT_TYPE_ATT, m_vecAttQueue2Team);
				}
			}
		}
		break;
	default:
		{
			if (m_strBattleReport3.size() < 2)
				return;

			uint8 faildir = cbatReport3.m_nFailDir;
			//失败方 1攻击者 2防守者
			if (faildir == WAR_DIR_TYPE_ATT) 
			{
				this->eraseAttQueue3Team();
				this->getCamTeamToAttQueue(nQueueNum);

				if (m_vecAttNPCTeam.size() == 0 && m_vecAttTeam.size() == 0 && m_vecAttQueue3Team.size() == 0 && m_mapPickTeam.size() == 0)
				{
					m_nQueueStatus = m_nQueueStatus|4;
					m_strBattleReport3 = "";
					this->teamRebackCamp(TEAM_ATT_TYPE_DEF, m_vecBlowQueue3Team);
				}
			}
			else
			{
				this->eraseBlowQueue3Team();
				this->getCamTeamToBlowQueue(nQueueNum);

				if (m_vecBlowNPCTeam.size() == 0 && m_vecBlowTeam.size() == 0 && m_vecBlowQueue3Team.size() == 0 && m_mapPickTeam.size() == 0)
				{
					m_nQueueStatus = m_nQueueStatus|4;
					m_nQueueFail = m_nQueueFail|4; 
					m_strBattleReport3 = "";
					this->teamRebackCamp(TEAM_ATT_TYPE_ATT, m_vecAttQueue3Team);
				}
			}
		}
		break;
	}

	return;
}

/**
* @功能：从大本营取队伍存入攻击队伍
*/
bool  CWarCity::getCamTeamToAttQueue(uint8 nQueueNum)
{
	if (this->isAttQueueFull(nQueueNum))
		return false;
//从大本营增加队伍
#ifdef WIN32 
	std::vector<CWarCityTeam> vecCityTeam;
	bool bfalse = false;
	for (size_t i = 0; i < m_vecAttTeam.size(); i++)
	{
		CWarCityTeam &cCityTeam = m_vecAttTeam[i];
		uint32 idTeam = cCityTeam.m_nTeamID;
		if (bfalse)
		{
			vecCityTeam.push_back(cCityTeam);
		}
		else
		{
			if (!this->isInPickBatTeam(idTeam))
			{
				this->addTeamToAttQueue(nQueueNum, cCityTeam);
				m_vecInFightPlayer.push_back(cCityTeam);
				m_mapFightTeam.insert(make_pair(cCityTeam.m_nTeamID, cCityTeam));
				bfalse = true;
			}
			else
			{
				vecCityTeam.push_back(cCityTeam);
			}
		}
	}
	if (bfalse)
	{
		m_vecAttTeam.clear();
		for (size_t i = 0; i < vecCityTeam.size(); i++)
			m_vecAttTeam.push_back(vecCityTeam[i]);
		return true;
	}

	vecCityTeam.clear();
	for (size_t i = 0; i < m_vecAttNPCTeam.size(); i++)
	{
		CWarCityTeam &cCityTeam = m_vecAttNPCTeam[i];
		uint32 idTeam = cCityTeam.m_nTeamID;
		if (bfalse)
		{
			vecCityTeam.push_back(cCityTeam);
		}
		else
		{
			if (!this->isInPickBatTeam(idTeam))
			{
				this->addTeamToAttQueue(nQueueNum, cCityTeam);
				m_mapFightTeam.insert(make_pair(cCityTeam.m_nTeamID, cCityTeam));
				bfalse = true;
			}
			else
			{
				vecCityTeam.push_back(cCityTeam);
			}
		}
	}

	if (bfalse)
	{
		m_vecAttNPCTeam.clear();
		for (size_t i = 0; i < vecCityTeam.size(); i++)
			m_vecAttNPCTeam.push_back(vecCityTeam[i]);
	}
#else
	if (m_vecAttTeam.size() > 0)
	{
		std::vector<CWarCityTeam>::iterator iter =  m_vecAttTeam.begin();
		for (; iter != m_vecAttTeam.end(); ++iter)
		{
			uint32 idTeam = iter->m_nTeamID;
			if (!this->isInPickBatTeam(idTeam))
			{
				CWarCityTeam &cWarCityTeam = *iter;
				m_vecInFightPlayer.push_back(*iter);
				this->addTeamToAttQueue(nQueueNum, *iter);
				m_mapFightTeam.insert(make_pair(cWarCityTeam.m_nTeamID, cWarCityTeam));
				m_vecAttTeam.erase(iter);
				return true;
			}
		}
	}
	
	if (m_vecAttNPCTeam.size() > 0)
	{
		std::vector<CWarCityTeam>::iterator iter =  m_vecAttNPCTeam.begin();
		for (; iter != m_vecAttNPCTeam.end(); ++iter)
		{
			uint32 idTeam = iter->m_nTeamID;
			if (!this->isInPickBatTeam(idTeam))
			{
				CWarCityTeam &cWarCityTeam = *iter;
				this->addTeamToAttQueue(nQueueNum, cWarCityTeam);
				m_mapFightTeam.insert(make_pair(cWarCityTeam.m_nTeamID, cWarCityTeam));
				m_vecAttNPCTeam.erase(iter);
				return true;
			}
		}
	}
#endif   

	return false;
}

/**
* @功能：从大本营取队伍存入防守队伍
*/
bool  CWarCity::getCamTeamToBlowQueue(uint8 nQueueNum)
{
	if (this->isBlowQueueFull(nQueueNum))
		return false;

//从大本营增加队伍 先找玩家队伍信息
#ifdef WIN32 
	std::vector<CWarCityTeam> vecCityTeam;
	bool bfalse = false;
	for (size_t i = 0; i < m_vecBlowTeam.size(); i++)
	{
		CWarCityTeam &cCityTeam = m_vecBlowTeam[i];
		uint32 idTeam = cCityTeam.m_nTeamID;
		if (bfalse)
		{
			vecCityTeam.push_back(cCityTeam);
		}
		else
		{
			if (!this->isInPickBatTeam(idTeam))
			{
				this->addTeamToBlowQueue(nQueueNum, cCityTeam);
				m_vecInFightPlayer.push_back(cCityTeam);
				m_mapFightTeam.insert(make_pair(cCityTeam.m_nTeamID, cCityTeam));
				bfalse = true;
			}
			else
			{
				vecCityTeam.push_back(cCityTeam);
			}
		}
	}

	if (bfalse)
	{
		m_vecBlowTeam.clear();
		for (size_t i = 0; i < vecCityTeam.size(); i++)
			m_vecBlowTeam.push_back(vecCityTeam[i]);

		return true;
	}

	vecCityTeam.clear();

	for (size_t i = 0; i < m_vecBlowNPCTeam.size(); i++)
	{
		CWarCityTeam &cCityTeam = m_vecBlowNPCTeam[i];
		uint32 idTeam = cCityTeam.m_nTeamID;
		if (bfalse)
		{
			vecCityTeam.push_back(cCityTeam);
		}
		else
		{
			if (!this->isInPickBatTeam(idTeam))
			{
				this->addTeamToBlowQueue(nQueueNum, cCityTeam);
				m_mapFightTeam.insert(make_pair(cCityTeam.m_nTeamID, cCityTeam));
				bfalse = true;
			}
			else
			{
				vecCityTeam.push_back(cCityTeam);
			}
		}
	}

	if (bfalse)
	{
		m_vecBlowNPCTeam.clear();
		for (size_t i = 0; i < vecCityTeam.size(); i++)
			m_vecBlowNPCTeam.push_back(vecCityTeam[i]);

		return true;
	}

#else
	
	if (m_vecBlowTeam.size() > 0)
	{
		std::vector<CWarCityTeam>::iterator iter =  m_vecBlowTeam.begin();
		for (; iter != m_vecBlowTeam.end(); ++iter)
		{
			uint32 idTeam = iter->m_nTeamID;
			if (!this->isInPickBatTeam(idTeam))
			{
				CWarCityTeam &cWarCityTeam = *iter;
				m_vecInFightPlayer.push_back(*iter);
				this->addTeamToBlowQueue(nQueueNum, *iter);
				m_mapFightTeam.insert(make_pair(cWarCityTeam.m_nTeamID, cWarCityTeam));
				m_vecBlowTeam.erase(iter);
				return true;
			}
		}
	}

	//从NPC队伍查找队伍
	if (m_vecBlowNPCTeam.size() > 0)
	{
		std::vector<CWarCityTeam>::iterator iter =  m_vecBlowNPCTeam.begin();
		for (; iter != m_vecBlowNPCTeam.end(); ++iter)
		{
			uint32 idTeam = iter->m_nTeamID;
			if (!this->isInPickBatTeam(idTeam))
			{
				CWarCityTeam &cWarCityTeam = *iter;
				this->addTeamToBlowQueue(nQueueNum, *iter);
				m_mapFightTeam.insert(make_pair(cWarCityTeam.m_nTeamID, cWarCityTeam));
				m_vecBlowNPCTeam.erase(iter);
				return true;
			}
		}
	}
#endif 
	return false;
}

/**
* @功能：返还驻兵道具
*/
void  CWarCity::accEndRebackDeployItem()
{
	std::string strItemExpTitle = m_pConsummer->formatLangString(LANGTYPE_WORLD_WAR, LANGTYPE_WORLD_WAR_ITEM_REWARD_EXP_TITLE);

	std::string strTitle = m_pConsummer->formatLangString(LANGTYPE_WORLD_WAR, LANGTYPE_WORLD_WAR_REBACK_ITEM_TILE);
	std::string  strContent = m_pConsummer->formatLangString(LANGTYPE_WORLD_WAR, LANGTYPE_WORLD_WAR_REBACK_ITEM_CONTENT);
	std::map<OBJID, CPlayerCityWarInfPtr>::iterator iter = m_mapPlayerTeam.begin();
	for (; iter != m_mapPlayerTeam.end(); ++iter)
	{
		CPlayerCityWarInfPtr pPlayerWarPtr = iter->second;
		if (pPlayerWarPtr == NULL)
			continue;

		OBJID idPlayer = iter->first;
        
		CRewardInfo cReward;
		uint16 nMulNum = 1;

		if (pPlayerWarPtr->m_nNation != this->getNation() && pPlayerWarPtr->m_nNation != this->getAttNation())
		{
			if ((this->getNation() >= NATIONAL_WEI && this->getNation() <= NATIONAL_WU) ||  
				(this->getAttNation() >= NATIONAL_WEI &&  this->getAttNation() <= NATIONAL_WU))
				nMulNum = 2;
		}
         
		uint16 nRetrunNum = 0;
		std::vector<CRewardInfo> vecPlayerReward;
		if (pPlayerWarPtr->m_vecTeamInf.size() > 0)
		{
			for (size_t i = 0; i < pPlayerWarPtr->m_vecTeamInf.size(); i++)
			{
				CPlayerTeamInf &cTeamInf = pPlayerWarPtr->m_vecTeamInf[i];
				//判断是否参加过战斗
				if (cTeamInf.m_nHaveFight == 0)
					nRetrunNum++;
			}

			cReward.m_nType = REWARDTYPE_ITEM;
			cReward.m_nValue = m_nDeployedItem;
			cReward.m_nNum = m_nDepoyedItemNum * nRetrunNum * nMulNum;
			vecPlayerReward.clear();
			vecPlayerReward.push_back(cReward);
			//发送邮件奖励
			m_pConsummer->sendMailToPlayer(idPlayer, strTitle, strContent, vecPlayerReward, REWARD_SOURCE_WORLD_END_REBACK_ITEM);
		}
        
		//进行道具消耗经验奖励
		uint16 nRewardExpNum = pPlayerWarPtr->m_nDeployNum;
		if (pPlayerWarPtr->m_nDeployNum >= nRetrunNum)
			nRewardExpNum = pPlayerWarPtr->m_nDeployNum - nRetrunNum;
		if (nRewardExpNum > 0)
		{
			uint32 nRewardExp = m_mapItemRewardExp[pPlayerWarPtr->m_nLevel] * nRewardExpNum * nMulNum;
			cReward.m_nType = REWARDTYPE_EXP;
			cReward.m_nValue = nRewardExp;
			cReward.m_nNum = 0;
			vecPlayerReward.clear();
			vecPlayerReward.push_back(cReward);
			std::string  strItemExpContent = m_pConsummer->formatLangString(LANGTYPE_WORLD_WAR, LANGTYPE_WORLD_WAR_ITEM_REWARD_EXP_CONTENT, nRewardExpNum, this->m_strName.data());
			m_pConsummer->sendMailToPlayer(idPlayer, strItemExpTitle, strItemExpContent, vecPlayerReward, REWARD_SOURCE_WORLD_END_REBACK_ITEM);
		}
	}

	return;
}

//获取杀敌奖励
void  CWarCity::getkillReward(CPlayerCityWarInfPtr pPlayerWarPtr, std::vector<CRewardInfo> &vecPlayerReward)
{
	if (pPlayerWarPtr == NULL)
		return;

	std::vector<CRewardInfo> vecKillReward;

	for (size_t i = 0; i < pPlayerWarPtr->m_vecReward.size(); i++)
		vecKillReward.push_back(pPlayerWarPtr->m_vecReward[i]);

	if (vecKillReward.size() == 0)
		return;

    m_pConsummer->arrangeAwardMoney(vecKillReward);
   
	uint16 nRetrunNum = 0;
	if (pPlayerWarPtr->m_vecTeamInf.size() > 0)
	{
		for (size_t i = 0; i < pPlayerWarPtr->m_vecTeamInf.size(); i++)
		{
			CPlayerTeamInf &cTeamInf = pPlayerWarPtr->m_vecTeamInf[i];
			//判断是否参加过战斗
			if (cTeamInf.m_nHaveFight == 0)
				nRetrunNum++;
		}
	}

	//进行道具消耗计算
	uint16 nRewardNum = pPlayerWarPtr->m_nDeployNum;
	if (pPlayerWarPtr->m_nDeployNum >= nRetrunNum)
		nRewardNum = pPlayerWarPtr->m_nDeployNum - nRetrunNum;

	for (size_t i = 0; i < vecKillReward.size(); i++)
	{
		CRewardInfo &cReward = vecKillReward[i];
		switch (cReward.m_nType)
		{
		case REWARDTYPE_GOLD:
			{
				if (cReward.m_nValue >= MAX_CCY_ONE_ITEM * nRewardNum)
					cReward.m_nValue = MAX_CCY_ONE_ITEM * nRewardNum;
			}
		    break;
		case REWARDTYPE_SILVER:
			{
				if (cReward.m_nValue >= MAX_GAME_CY_ONE_ITEM * nRewardNum)
					cReward.m_nValue = MAX_GAME_CY_ONE_ITEM * nRewardNum;
			}
			break;
		case REWARDTYPE_EXPLOIT:
			{
				if (cReward.m_nValue >= MAX_EXPLOIT_ONE_ITEM * nRewardNum)
					cReward.m_nValue = MAX_EXPLOIT_ONE_ITEM * nRewardNum;
			}
			break;
		case REWARDTYPE_ITEM:
			{
				if (cReward.m_nNum >= MAX_LIB_ONE_ITEM * nRewardNum)
					cReward.m_nNum = MAX_LIB_ONE_ITEM * nRewardNum;
			}
			break;
		default:
			break;
		}

		vecPlayerReward.push_back(cReward);
	}

	return;
}

/**
* @功能：城战结束玩家奖励
*/
void  CWarCity::cityPlayerRewardAcc()
{
	//获取奖励信息
	std::vector<CRewardInfo> vecSucReward;
	std::vector<CRewardInfo> vecFailReward;
	std::string strSuccTitle;
	std::string strSuccContent;
	std::string strFailTitle;
	std::string strFailContent;
	if (this->getAccType() == ACC_TYPE_NPC)
	{
		for (size_t i = 0; i < m_vecDefSuccReward.size(); i++)
			vecSucReward.push_back(m_vecDefSuccReward[i]);
		for (size_t i = 0; i < m_vecDefFailRewad.size(); i++)
			vecFailReward.push_back(m_vecDefFailRewad[i]);
	}
	else 
	{
		for (size_t i = 0; i < m_vecAttSuccRewad.size(); i++)
			vecSucReward.push_back(m_vecAttSuccRewad[i]);
		for (size_t i = 0; i < m_vecAttFailReward.size(); i++)
			vecFailReward.push_back(m_vecAttFailReward[i]);
	}
	strSuccTitle = m_pConsummer->formatLangString(LANGTYPE_WORLD_WAR, LANGTYPE_WORLD_WAR_SUC_TILE);
	strSuccContent = m_pConsummer->formatLangString(LANGTYPE_WORLD_WAR, LANGTYPE_WORLD_WAR_SUC_CONTENT, m_strName.data());
	strFailTitle = m_pConsummer->formatLangString(LANGTYPE_WORLD_WAR, LANGTYPE_WORLD_WAR_FAIL_TILE);
    strFailContent = m_pConsummer->formatLangString(LANGTYPE_WORLD_WAR, LANGTYPE_WORLD_WAR_FAIL_CONTENT, m_strName.data());

	std::map<OBJID, CPlayerCityWarInfPtr>::iterator iter =  m_mapPlayerTeam.begin();
	for (; iter != m_mapPlayerTeam.end(); ++iter)
	{
		OBJID idPlayer = iter->first;
		CPlayerCityWarInfPtr pPlayerWarPtr = iter->second;
		if (pPlayerWarPtr == NULL)
			continue;
		if (pPlayerWarPtr->m_nDeployNum == 0)
			continue;
		if (pPlayerWarPtr->m_nDir == WAR_DIR_TYPE_NO)
			continue;
        
		//玩家自己的奖励
		std::vector<CRewardInfo> vecPlayerReward;
		this->getkillReward(pPlayerWarPtr, vecPlayerReward);
		/*for (size_t i = 0; i < pPlayerWarPtr->m_vecReward.size(); i++)
			vecPlayerReward.push_back(pPlayerWarPtr->m_vecReward[i]);*/

		//城战结束奖励
		if (this->getStatus() ==  2) //防守成功
			{
				if (pPlayerWarPtr->m_nDir == WAR_DIR_TYPE_BLOW)
				{
					for (size_t i = 0; i < vecSucReward.size(); i++)
						vecPlayerReward.push_back(vecSucReward[i]);
					m_pConsummer->arrangeAwardMoney(vecPlayerReward);
					m_pConsummer->sendMailToPlayer(idPlayer, strSuccTitle, strSuccContent, vecPlayerReward, REWARD_SOURCE_CITY_END);	
				}
				else
				{
					for (size_t i = 0; i < vecFailReward.size(); i++)
						vecPlayerReward.push_back(vecFailReward[i]);
					m_pConsummer->arrangeAwardMoney(vecPlayerReward);
					m_pConsummer->sendMailToPlayer(idPlayer, strFailTitle, strFailContent, vecPlayerReward, REWARD_SOURCE_CITY_END);
				}
			}
		else
		{
			if (pPlayerWarPtr->m_nDir == WAR_DIR_TYPE_ATT)
			{
				for (size_t i = 0; i < vecSucReward.size(); i++)
					vecPlayerReward.push_back(vecSucReward[i]);
		
				//合并相同类型的奖励类型
				m_pConsummer->arrangeAwardMoney(vecPlayerReward);
				//发送邮件奖励
				m_pConsummer->sendMailToPlayer(idPlayer, strSuccTitle, strSuccContent, vecPlayerReward, REWARD_SOURCE_CITY_END);	
			}
		    else
			{
				for (size_t i = 0; i < vecFailReward.size(); i++)
					vecPlayerReward.push_back(vecFailReward[i]);
				m_pConsummer->arrangeAwardMoney(vecPlayerReward);
				m_pConsummer->sendMailToPlayer(idPlayer, strFailTitle, strFailContent, vecPlayerReward, REWARD_SOURCE_CITY_END);
			}
		}
	}

	return;
}



/**
* @功能：判断是否还有可以战斗的队伍
*/
bool  CWarCity::isHavaCanFightTeam(bool battak)
{
	if (!battak)
	{
		if (m_vecAttNPCTeam.size() == 0 && m_vecAttTeam.size() == 0)
			return false;
		for (size_t i = 0; i < m_vecAttTeam.size(); i++)
		{
			CWarCityTeam &cCityTeam = m_vecAttTeam[i];
			uint32 idTeam = cCityTeam.m_nTeamID;
			std::map<uint32, CWarCityTeam>::iterator iter = m_mapFightTeam.find(idTeam);
			if (iter == m_mapFightTeam.end())
				return true;
		}

		for (size_t i = 0; i < m_vecAttNPCTeam.size() ; i++)
		{
			CWarCityTeam &cCityTeam = m_vecAttNPCTeam[i];
			uint32 idTeam = cCityTeam.m_nTeamID;

			std::map<uint32, CWarCityTeam>::iterator iter = m_mapFightTeam.find(idTeam);
			if (iter == m_mapFightTeam.end())
				return true;
		}
	}
	else
	{
		if (m_vecBlowNPCTeam.size() == 0 && m_vecBlowTeam.size() == 0) 
			return false;
		
		for (size_t i = 0; i < m_vecBlowTeam.size(); i++)
		{
			CWarCityTeam &cCityTeam = m_vecBlowTeam[i];
			uint32 idTeam = cCityTeam.m_nTeamID;
			std::map<uint32, CWarCityTeam>::iterator iter = m_mapFightTeam.find(idTeam);
			if (iter == m_mapFightTeam.end())
				return true;
		}

		for (size_t i = 0; i < m_vecBlowNPCTeam.size(); i++)
		{
			CWarCityTeam &cCityTeam = m_vecBlowNPCTeam[i];
			uint32 idTeam = cCityTeam.m_nTeamID;
			std::map<uint32, CWarCityTeam>::iterator iter = m_mapFightTeam.find(idTeam);
			if (iter == m_mapFightTeam.end())
				return true;
		}
	}

	return false;
}

/**
* @功能：获取玩家虚拟战斗信息
*/
void CWarCity::getPlayerVirtualTeam(OBJID idPlayer, CPlayerFightTeamInf &cPlayerTeam)
{
	//获取玩家的信息
	std::map<OBJID, CPlayerCityWarInfPtr>::iterator iter_p = m_mapPlayerTeam.find(idPlayer);
	if (iter_p == m_mapPlayerTeam.end())
		return;
	CPlayerCityWarInfPtr pPlayerIntPtr = iter_p->second;
	if (pPlayerIntPtr == NULL)
		return;
    
	if (pPlayerIntPtr->m_vecTeamInf.size() == 0)
		return;

	CPlayerTeamInf &cTeamInf = pPlayerIntPtr->m_vecTeamInf[0];

	cPlayerTeam.m_nBattEffect = pPlayerIntPtr->m_nBattEffect;
	cPlayerTeam.m_nContinuKill = cTeamInf.m_nContinKillNum;
	cPlayerTeam.m_nDragonLevel = cTeamInf.m_nDragonLevel;
	cPlayerTeam.m_nFormationID = cTeamInf.m_nFormationID;
	cPlayerTeam.m_nLevel = pPlayerIntPtr->m_nLevel;
	cPlayerTeam.m_nSex = pPlayerIntPtr->m_nSex;
	cPlayerTeam.m_strName = pPlayerIntPtr->m_strName;
	for (size_t i = 0; i < cTeamInf.m_vecBuddy.size(); i++)
		cPlayerTeam.m_vecBuddy.push_back(cTeamInf.m_vecBuddy[i]);
	for (size_t i = 0; i < cTeamInf.m_vecDragonSKill.size(); i++)
		cPlayerTeam.m_vecDragonSKill.push_back(cTeamInf.m_vecDragonSKill[i]);
	for (size_t i = 0; i < cTeamInf.m_vecUint.size(); i++)
		cPlayerTeam.m_vecUint.push_back(cTeamInf.m_vecUint[i]);
    
	return;
}

/**
* @功能：获取自爆兵的被单挑队伍 
*/
bool CWarCity::getPickBlewTeam(bool battType, CWorldBatTeam &cBlowBatTeam, std::string &strName)
{
	CDateTime now;
	if (!battType)
	{
		if (!bHaveAttBlewTeam)
			return false;
		//从最头的队伍找出一个可以单挑的队伍
		std::vector<CWarCityTeam>::iterator iter =  m_vecAttNPCTeam.begin();
		for (; iter != m_vecAttNPCTeam.end(); ++iter)
		{
			CWarCityTeam &cCityTeam = *iter;
			uint32 idTeam = cCityTeam.m_nTeamID;
			//判断队伍是否在战斗队伍中
			if (this->isInPickBatTeam(idTeam))
				continue;

			//判断是否是自爆兵
			if (!this->isBlewTeam(idTeam))
				continue;

			//把队伍存放到挑战/被挑战的队伍
			CCityPickTeam cPickTeam;
			strName = cCityTeam.m_strName;
			cPickTeam.m_nTeamType  = TEAM_ATT_TYPE_ATT;
			cPickTeam.m_nTime = now.getTotalSecs();
			cPickTeam.m_cTeam = cCityTeam;
			m_mapPickTeam.insert(make_pair(idTeam, cPickTeam));
			m_mapFightTeam.insert(make_pair(idTeam, cCityTeam));
			cBlowBatTeam.m_nTeamID = idTeam;
			cBlowBatTeam.m_nType = TEAM_TYPE_NPC;
			CWarNPCFightTeam cNpcTeam;
			cNpcTeam.m_nID = cCityTeam.m_nNpcID;
			cNpcTeam.m_vecGrid.assign(cCityTeam.m_vecBatInf.begin(), cCityTeam.m_vecBatInf.end());
			cBlowBatTeam.m_vecNpcInf.push_back(cNpcTeam);
			m_vecAttNPCTeam.erase(iter);
			return true;
		}

		bHaveAttBlewTeam = false;
	}
	else
	{
		if (!bHaveDefBlewTeam)
			return false;

		std::vector<CWarCityTeam>::iterator iter =  m_vecBlowNPCTeam.begin();  
		for (; iter != m_vecBlowNPCTeam.end(); ++iter)
		{
			CWarCityTeam &cCityTeam = *iter;
			uint32 idTeam = cCityTeam.m_nTeamID;
			//判断队伍是否在战斗队伍中
			if (this->isInPickBatTeam(idTeam))
				continue;

			//判断是否是自爆兵
			if (!this->isBlewTeam(idTeam))
				continue;

			//把队伍存放到挑战/被挑战的队伍
			strName = cCityTeam.m_strName;
			CCityPickTeam cPickTeam;
			cPickTeam.m_nTeamType  = TEAM_ATT_TYPE_DEF;
			cPickTeam.m_nTime = now.getTotalSecs();
			cPickTeam.m_cTeam = cCityTeam;
			m_mapPickTeam.insert(make_pair(idTeam, cPickTeam));
			m_mapFightTeam.insert(make_pair(idTeam, cCityTeam));
			cBlowBatTeam.m_nTeamID = idTeam;
			cBlowBatTeam.m_nType = TEAM_TYPE_NPC;
			CWarNPCFightTeam cNpcTeam;
			cNpcTeam.m_nID = cCityTeam.m_nNpcID;
			cNpcTeam.m_vecGrid.assign(cCityTeam.m_vecBatInf.begin(), cCityTeam.m_vecBatInf.end());
			cBlowBatTeam.m_vecNpcInf.push_back(cNpcTeam);
			m_vecBlowNPCTeam.erase(iter);
			return true;
		}

		bHaveDefBlewTeam = false;
	}
   
	return false;
}

/**
* @功能：获取挑战被单挑的队伍
*/
void CWarCity::getPickTeam(bool battType, CWorldBatTeam &cBlowBatTeam, std::string &strName)
{
	CDateTime now;

	//优先挑选自爆兵
	if (this->getPickBlewTeam(battType, cBlowBatTeam, strName))
	    return;

	if (!battType)
	{
		bool bplayer = false;
		std::vector<CWarCityTeam>::iterator iter =  m_vecAttTeam.begin();  
		for (; iter != m_vecAttTeam.end(); ++iter)
		{
			CWarCityTeam &cCityTeam = *iter;
			uint32 idTeam = cCityTeam.m_nTeamID;
			if (this->isInPickBatTeam(idTeam))
				continue;
			CCityPickTeam cPickTeam;
			strName = cCityTeam.m_strName;
			cPickTeam.m_nTeamType  = TEAM_ATT_TYPE_ATT;
			cPickTeam.m_nTime = now.getTotalSecs();
			cPickTeam.m_cTeam = cCityTeam;
			m_mapPickTeam.insert(make_pair(idTeam, cPickTeam));
			m_mapFightTeam.insert(make_pair(idTeam, cCityTeam));
			cBlowBatTeam.m_nTeamID = idTeam;
			cBlowBatTeam.m_nType = TEAM_TYPE_PLAYER;
			CPlayerFightTeamInf cPlayerTeam;
			this->getPlayerVirtualTeam(cCityTeam.m_nAvatarID, cPlayerTeam);
			cBlowBatTeam.m_vecPlayerInf.push_back(cPlayerTeam);
			m_vecAttTeam.erase(iter);
			bplayer = true;
			return;
		}
		if (!bplayer)
		{
			//从最头的队伍找出一个可以单挑的队伍
			std::vector<CWarCityTeam>::iterator iter =  m_vecAttNPCTeam.begin();
			for (; iter != m_vecAttNPCTeam.end(); ++iter)
			{
				CWarCityTeam &cCityTeam = *iter;
				uint32 idTeam = cCityTeam.m_nTeamID;
				//判断队伍是否在战斗队伍中
				if (this->isInPickBatTeam(idTeam))
					continue;

				//把队伍存放到挑战/被挑战的队伍
				CCityPickTeam cPickTeam;
				strName = cCityTeam.m_strName;
				cPickTeam.m_nTeamType  = TEAM_ATT_TYPE_ATT;
				cPickTeam.m_nTime = now.getTotalSecs();
				cPickTeam.m_cTeam = cCityTeam;
				m_mapPickTeam.insert(make_pair(idTeam, cPickTeam));
				m_mapFightTeam.insert(make_pair(idTeam, cCityTeam));
				cBlowBatTeam.m_nTeamID = idTeam;
				cBlowBatTeam.m_nType = TEAM_TYPE_NPC;
				CWarNPCFightTeam cNpcTeam;
				cNpcTeam.m_nID = cCityTeam.m_nNpcID;
				cNpcTeam.m_vecGrid.assign(cCityTeam.m_vecBatInf.begin(), cCityTeam.m_vecBatInf.end());
				cBlowBatTeam.m_vecNpcInf.push_back(cNpcTeam);
				m_vecAttNPCTeam.erase(iter);
				return;
			}
		}
	}
	else
	{
		//优先从玩家防守阵型获取队伍
		bool bplayer = false;
		std::vector<CWarCityTeam>::iterator iter =  m_vecBlowTeam.begin();  
		for (; iter != m_vecBlowTeam.end(); ++iter)
		{
			CWarCityTeam &cCityTeam = *iter;
			uint32 idTeam = cCityTeam.m_nTeamID;
			//判断队伍是否在战斗队伍中
			if (this->isInPickBatTeam(idTeam))
				continue;

			//把队伍存放到挑战/被挑战的队伍
			CCityPickTeam cPickTeam;
			strName = cCityTeam.m_strName;
			cPickTeam.m_nTeamType  = TEAM_ATT_TYPE_DEF;
			cPickTeam.m_nTime = now.getTotalSecs();
			cPickTeam.m_cTeam = cCityTeam;
			m_mapPickTeam.insert(make_pair(idTeam, cPickTeam));
			m_mapFightTeam.insert(make_pair(idTeam, cCityTeam));
			cBlowBatTeam.m_nTeamID = idTeam;
			cBlowBatTeam.m_nType = TEAM_TYPE_PLAYER;
			CPlayerFightTeamInf cPlayerTeam;
			this->getPlayerVirtualTeam(cCityTeam.m_nAvatarID, cPlayerTeam);
			cBlowBatTeam.m_vecPlayerInf.push_back(cPlayerTeam);

			m_vecBlowTeam.erase(iter);
			bplayer = true;
			return;
		}

		if (!bplayer)
		{
			std::vector<CWarCityTeam>::iterator iter =  m_vecBlowNPCTeam.begin();  
			for (; iter != m_vecBlowNPCTeam.end(); ++iter)
			{
				CWarCityTeam &cCityTeam = *iter;
				uint32 idTeam = cCityTeam.m_nTeamID;
				//判断队伍是否在战斗队伍中
				if (this->isInPickBatTeam(idTeam))
					continue;

				//把队伍存放到挑战/被挑战的队伍
				strName = cCityTeam.m_strName;
				CCityPickTeam cPickTeam;
				cPickTeam.m_nTeamType  = TEAM_ATT_TYPE_DEF;
				cPickTeam.m_nTime = now.getTotalSecs();
				cPickTeam.m_cTeam = cCityTeam;
				m_mapPickTeam.insert(make_pair(idTeam, cPickTeam));
				m_mapFightTeam.insert(make_pair(idTeam, cCityTeam));
				cBlowBatTeam.m_nTeamID = idTeam;
				cBlowBatTeam.m_nType = TEAM_TYPE_NPC;
				CWarNPCFightTeam cNpcTeam;
				cNpcTeam.m_nID = cCityTeam.m_nNpcID;
				cNpcTeam.m_vecGrid.assign(cCityTeam.m_vecBatInf.begin(), cCityTeam.m_vecBatInf.end());
				cBlowBatTeam.m_vecNpcInf.push_back(cNpcTeam);
				m_vecBlowNPCTeam.erase(iter);
				return;
			}
		}
	}
	

	return;
}

/**
* @功能：把玩家的队伍放到挑战队列中
*/
void CWarCity::putPlayerTeamToPickTeam(OBJID idPlayer, uint32 idTeam)
{
	std::map<OBJID, CPlayerCityWarInfPtr>::iterator iter = m_mapPlayerTeam.find(idPlayer);
	if (iter == m_mapPlayerTeam.end())
		return;

	CPlayerCityWarInfPtr pPlayerInfPtr = iter->second;
	if (pPlayerInfPtr == NULL)
		return;

	CDateTime now;
	std::vector<CWarCityTeam> vecTeam;
	if (pPlayerInfPtr->m_nDir == WAR_DIR_TYPE_ATT)
		vecTeam.assign(m_vecAttTeam.begin(), m_vecAttTeam.end());
	else
        vecTeam.assign(m_vecBlowTeam.begin(), m_vecBlowTeam.end());

	if (vecTeam.size() == 0)
		return;
     
	bool bfalse = false;
	std::vector<CWarCityTeam> vecCityTeam;

	for (int i = vecTeam.size() - 1; i >= 0; i--)
	{
		CWarCityTeam &cTeam = vecTeam[i];
		if (bfalse)
		{
			vecCityTeam.push_back(cTeam);
			continue;
		}

		if (cTeam.m_nAvatarID == idPlayer && cTeam.m_nTeamID == idTeam) 
		{
			CCityPickTeam cPickTeam;
			if (pPlayerInfPtr->m_nDir == WAR_DIR_TYPE_BLOW)
			    cPickTeam.m_nTeamType = TEAM_ATT_TYPE_DEF;
			else
                cPickTeam.m_nTeamType = WAR_DIR_TYPE_ATT;
			cPickTeam.m_nTime = now.getTotalSecs();
			cPickTeam.m_cTeam = cTeam;
			m_mapPickTeam.insert(make_pair(idTeam, cPickTeam));
			m_mapFightTeam.insert(make_pair(idTeam, cTeam));
			bfalse = true;
		}
		else
		{
			vecCityTeam.push_back(cTeam);
		}
	}

	if (bfalse)
	{
		if (pPlayerInfPtr->m_nDir == WAR_DIR_TYPE_ATT)
			m_vecAttTeam.clear();
		else
		m_vecBlowTeam.clear();
		for (size_t i = 0; i < vecCityTeam.size(); i++)
		{
			if (pPlayerInfPtr->m_nDir == WAR_DIR_TYPE_ATT)
				m_vecAttTeam.push_back(vecCityTeam[i]);
			else
			m_vecBlowTeam.push_back(vecCityTeam[i]);
		}
	}

	return;
}

/**
* @功能：获取队伍的玩家ID 
*/
OBJID CWarCity::getTeamPlayerID(uint32 idTeam)
{
	std::map<uint32, CCityPickTeam>::iterator iter1 = m_mapPickTeam.find(idTeam);
	if (iter1 != m_mapPickTeam.end())
	{	
		CCityPickTeam &cBlowPickTeam = iter1->second;
		CWarCityTeam &cCityTeam = cBlowPickTeam.m_cTeam;
		if (cCityTeam.m_nTeamType == TEAM_TYPE_NPC)
			return 0;
		else
			return cCityTeam.m_nAvatarID;
	}

	std::map<uint32, CWarCityTeam>::iterator iter =  m_mapFightTeam.find(idTeam);
	if (iter != m_mapFightTeam.end())
	{
		CWarCityTeam &cCityTeam = iter->second;
		if (cCityTeam.m_nTeamType == TEAM_TYPE_NPC)
			return 0;
		else
			return cCityTeam.m_nAvatarID;
	}
    
	return 0;
}

/**
* @功能：玩家单挑成功处理
*/
void CWarCity::pickTeamReback(uint32 idTeam)
{
	//获取挑战放队伍阵型
	std::map<uint32, CCityPickTeam>::iterator iter = m_mapPickTeam.find(idTeam);
	if (iter == m_mapPickTeam.end())
		return;

	//把队伍存放到阵营中
	CCityPickTeam &cPickTeam = iter->second;
	CWarCityTeam &cCityTeam = cPickTeam.m_cTeam;

	//0属于防守方 1属于攻击方
    if (cPickTeam.m_nTeamType == TEAM_ATT_TYPE_DEF)
	{
		if (cCityTeam.m_nTeamType == TEAM_TYPE_NPC)
			m_vecBlowNPCTeam.push_back(cPickTeam.m_cTeam);  
		else
			m_vecBlowTeam.push_back(cPickTeam.m_cTeam);
	}
	else
	{
		if (cCityTeam.m_nTeamType == TEAM_TYPE_NPC)
			m_vecAttNPCTeam.push_back(cPickTeam.m_cTeam);
		else
			m_vecAttTeam.push_back(cPickTeam.m_cTeam);
	}

	m_mapPickTeam.erase(idTeam);
	m_mapFightTeam.erase(idTeam);

	this->getRankSessionID();

	return;
}

/**
* @功能：单挑队伍删除处理
*/
void CWarCity::pickTeamErase(uint32 idTeam)
{
	this->clearPlayerTeam(idTeam);
	m_mapPickTeam.erase(idTeam);
	m_mapFightTeam.erase(idTeam);

	return;
}

/**
* @功能：单挑失败处理
*/
uint32 CWarCity::playerPickFail(uint32 idTeam, uint32 idBlowTeam)
{
	//获取玩家挑战放队伍阵型
	std::map<uint32, CCityPickTeam>::iterator iter = m_mapPickTeam.find(idTeam);
	if (iter == m_mapPickTeam.end())
		return 0;

	//删除玩家队伍
	m_mapPickTeam.erase(idTeam);

	//获取被挑战的队伍
	std::map<uint32, CCityPickTeam>::iterator iter1 = m_mapPickTeam.find(idBlowTeam);
	if (iter1 == m_mapPickTeam.end())
		return 0;

	CCityPickTeam &cBlowPickTeam = iter1->second;
	CWarCityTeam &cCityTeam = cBlowPickTeam.m_cTeam;
	if (cBlowPickTeam.m_nTeamType == TEAM_TYPE_PLAYER)
	{
		OBJID idPlayer = cCityTeam.m_nAvatarID;
		//获取玩家信息
		CPlayerCityWarInfPtr pPlayerWarPtr = this->getJoinPlayer(idPlayer);
		if (pPlayerWarPtr != NULL)
		{
			for (size_t i = 0; i < pPlayerWarPtr->m_vecTeamInf.size(); i++)
			{
				CPlayerTeamInf &cTeamInf = pPlayerWarPtr->m_vecTeamInf[i];
				if (cTeamInf.m_nTeamID == idBlowTeam)
				{
					cTeamInf.m_nContinKillNum++;
					cTeamInf.m_nHaveFight = 1;
					break;
				}
			}
			pPlayerWarPtr->m_nKillNum++;

			//更新排行榜
			this->updCityKillRank(idPlayer, pPlayerWarPtr);
		}
        m_vecAttTeam.push_back(cCityTeam);
	}
	else
	{
		m_vecAttNPCTeam.push_back(cCityTeam);
	}

    //删除挑战队伍信息
	m_mapPickTeam.erase(idBlowTeam);

	return 0;
}

//更新城池杀敌榜
void CWarCity::updCityKillRank(OBJID idPlayer, CPlayerCityWarInfPtr pPlayerCityWarPtr)
{
	CDateTime now;
	if (pPlayerCityWarPtr->m_nKillNum == 1)
	{
		CWarCityRank cCityRank;
		cCityRank.m_nKillNum = pPlayerCityWarPtr->m_nKillNum;
		cCityRank.m_nNation = 0;
		cCityRank.m_nPlayerID = idPlayer;
		cCityRank.m_nTimes = now.getTotalSecs();
		cCityRank.m_strName = pPlayerCityWarPtr->m_strName;
		m_vecCityRank.push_back(cCityRank);
	}
	else
	{
		for (size_t i = 0; i < m_vecCityRank.size(); i++)
		{
			CWarCityRank &cCityRank = m_vecCityRank[i];
			if (cCityRank.m_nPlayerID == idPlayer)
			{
				cCityRank.m_nKillNum = pPlayerCityWarPtr->m_nKillNum;
				cCityRank.m_nTimes = now.getTotalSecs();
			}
		}
	}
    
	sort(m_vecCityRank.begin(), m_vecCityRank.end(), comparerank);
	this->getRankSessionID();
	return;
}


/**
* @功能：获取队栈的战报
*/
std::string CWarCity::getQueueBattleRepor(uint8 nQueueNum)
{
	switch (nQueueNum)
	{
	case CITY_QUEUE_NUM_FIRST:
		return m_strBattleReport1;
	case CITY_QUEUE_NUM_SECOND:
		return m_strBattleReport2;
	case CITY_QUEUE_NUM_THIRTY:
		return m_strBattleReport3;
	default:
		return " ";
	}

	return " ";
}

/**
* @功能：设置战斗队伍剩余血量 
*/
void CWarCity::setFightQueueTeamLastHp(uint8 queueNum)
{
	switch (queueNum)
	{
	case CITY_QUEUE_NUM_FIRST:
		{
			if (m_strBattleReport1.size() < 2)
				return;

			if (m_vecGrid1.size() == 0)
                return;

			if (m_nQueueStatus & 1)
				return;

			//失败者，1攻击方 2失败方
			uint32 idTeam = 0;
			if (cbatReport1.m_nFailDir == WAR_DIR_TYPE_ATT)
			{
				if (m_vecBlowQueue1Team.size() == 0)
					return;

				CCitySTeam &cSteam = m_vecBlowQueue1Team[0];
				idTeam = cSteam.m_nTeamID;
			}
			else
			{
				if (m_vecAttQueue1Team.size() == 0)
					return;

				CCitySTeam &cSteam = m_vecAttQueue1Team[0];
				idTeam = cSteam.m_nTeamID;
			}
            
			if (idTeam > 0)
			    this->updTeamBatGrid(idTeam, m_vecGrid1);
		}
		return;
	case CITY_QUEUE_NUM_SECOND:
		{
			if (m_strBattleReport2.size() < 2)
				return;

			if (m_vecGrid2.size() == 0)
				return;

			if (m_nQueueStatus & 2)
				return;

			//失败者，1攻击方 2失败方
			uint32 idTeam = 0;
			if (cbatReport2.m_nFailDir == WAR_DIR_TYPE_ATT)
			{
				if (m_vecBlowQueue2Team.size() == 0)
					return;

				CCitySTeam &cSteam = m_vecBlowQueue2Team[0];
				idTeam = cSteam.m_nTeamID;
			}
			else
			{
				if (m_vecAttQueue2Team.size() == 0)
					return;

				CCitySTeam &cSteam = m_vecAttQueue2Team[0];
				idTeam = cSteam.m_nTeamID;
			}

			if (idTeam > 0)
			    this->updTeamBatGrid(idTeam, m_vecGrid2);
		}
		return;
	case CITY_QUEUE_NUM_THIRTY:
		{
			if (m_strBattleReport3.size() < 2)
				return;

			if (m_vecGrid3.size() == 0)
				return;

			if (m_nQueueStatus & 4)
				return;

			//失败者，1攻击方 2失败方
			uint32 idTeam = 0;
			if (cbatReport3.m_nFailDir == WAR_DIR_TYPE_ATT)
			{
				if (m_vecBlowQueue3Team.size() == 0)
					return;

				CCitySTeam &cSteam = m_vecBlowQueue3Team[0];
				idTeam = cSteam.m_nTeamID;
			}
			else
			{
				if (m_vecAttQueue3Team.size() == 0)
					return;

				CCitySTeam &cSteam = m_vecAttQueue3Team[0];
				idTeam = cSteam.m_nTeamID;
			}

			if (idTeam > 0)
                this->updTeamBatGrid(idTeam, m_vecGrid3);
		}
		return;
	default:
		return;
	}

	return;

}

/**
* @功能：设置战斗队伍剩余血量 
*/
void CWarCity::setFightQueueGrid(uint8 queueNum, std::vector<CTeamBatInf> &vecGrid)
{
	switch (queueNum)
	{
	case CITY_QUEUE_NUM_FIRST:
		{
			m_vecGrid1.clear();
			for (size_t i = 0; i < vecGrid.size(); i++)
				m_vecGrid1.push_back(vecGrid[i]);
		}
		return;
	case CITY_QUEUE_NUM_SECOND:
		{
			m_vecGrid2.clear();
			for (size_t i = 0; i < vecGrid.size(); i++)
				m_vecGrid2.push_back(vecGrid[i]);
		}
		return;
	case CITY_QUEUE_NUM_THIRTY:
		{
			m_vecGrid3.clear();
			for (size_t i = 0; i < vecGrid.size(); i++)
				m_vecGrid3.push_back(vecGrid[i]);
		}
		return;
	default:
		return;
	}

	return;
}

/**
* @功能：获取城战杀敌排行榜
*/
void CWarCity::getCityKillRank(std::vector<CKillRank> &vecCityRank)
{
	CKillRank cRank;
	for (size_t i = 0; i < m_vecCityRank.size(); i++)
	{
		if (i >= 10)
			break;

		CWarCityRank &cCityRank = m_vecCityRank[i];

		cRank.m_nKillNum = cCityRank.m_nKillNum;
		cRank.m_nNation = cCityRank.m_nNation;
		cRank.m_strName = cCityRank.m_strName;

		vecCityRank.push_back(cRank);
	}
}


/**
* @功能：设置车轮战的时间
*/


/**
* @功能：清理超时的单挑被单挑的队伍
*/
void CWarCity::clearPickTeam()
{
	CDateTime now;
	std::vector<uint32> vecTimeOutTeam;
	std::map<uint32, CCityPickTeam>::iterator iter = m_mapPickTeam.begin();
    for (; iter != m_mapPickTeam.end(); ++iter)
    {
		//20秒没有返回，算超时
		CCityPickTeam &cPickTeam = iter->second;
		if (now.getTotalSecs() > cPickTeam.m_nTime + 20)
		{
			CWarCityTeam &cTeam = cPickTeam.m_cTeam;
			uint32  idTeam = iter->first;
			vecTimeOutTeam.push_back(idTeam);
			if (cPickTeam.m_nTeamType == TEAM_ATT_TYPE_DEF)
			{
				if (cTeam.m_nTeamType == TEAM_TYPE_NPC)
					m_vecBlowNPCTeam.push_back(cTeam);
				else
					m_vecBlowTeam.push_back(cTeam);
			}
			else
			{
				if (cTeam.m_nTeamType == TEAM_TYPE_NPC)
				m_vecAttNPCTeam.push_back(cTeam);
				else
					m_vecAttTeam.push_back(cTeam);
			}
		}
    }

	for (size_t i = 0; i < vecTimeOutTeam.size(); i++)
	{
		uint32 idTeam = vecTimeOutTeam[i];
		m_mapPickTeam.erase(idTeam);

	}
}


/**
* @功能：获取战斗堆栈的序列号ID
*/
uint32 CWarCity::incQueueSessionID(uint8 queueNum)
{
	switch (queueNum)
	{
	case CITY_QUEUE_NUM_FIRST:
		{
			return this->getSession1();
		}
		break;
	case CITY_QUEUE_NUM_SECOND:
		{
			return this->getSession2();
		}
		break;
	case CITY_QUEUE_NUM_THIRTY:
		{
			return this->getSession3();
		}
		break;
	default:
		break;
	}

	return 0;
}


/**
* @功能：获取战斗对战战斗队伍
*/
void CWarCity::getFightQueueTeam(uint8 nQueueNum, std::vector<CCityFightTeam> &vecFightTeam)
{
	CCityFightTeam cFightTeam;
	switch (nQueueNum)
	{
	case CITY_QUEUE_NUM_FIRST:
		{
			if (!(m_nQueueStatus & 1))
			{
				for (size_t j = 0; j < m_vecAttQueue1Team.size(); j++)
					cFightTeam.m_vecAttTeam.push_back(m_vecAttQueue1Team[j]);

				for (size_t j = 0; j < m_vecBlowQueue1Team.size(); j++)
					cFightTeam.m_vecBlowTeam.push_back(m_vecBlowQueue1Team[j]);

				vecFightTeam.push_back(cFightTeam);
			}
		}
		break;
	case CITY_QUEUE_NUM_SECOND:
		{
			if (!(m_nQueueStatus & 2))
			{
				for (size_t j = 0; j < m_vecAttQueue2Team.size(); j++)
					cFightTeam.m_vecAttTeam.push_back(m_vecAttQueue2Team[j]);

				for (size_t j = 0; j < m_vecBlowQueue2Team.size(); j++)
					cFightTeam.m_vecBlowTeam.push_back(m_vecBlowQueue2Team[j]);

				vecFightTeam.push_back(cFightTeam);
			}
		}
		break;
	case CITY_QUEUE_NUM_THIRTY:
		{
			if (!(m_nQueueStatus & 4))
			{
				for (size_t j = 0; j < m_vecAttQueue3Team.size(); j++)
					cFightTeam.m_vecAttTeam.push_back(m_vecAttQueue3Team[j]);

				for (size_t j = 0; j < m_vecBlowQueue3Team.size(); j++)
					cFightTeam.m_vecBlowTeam.push_back(m_vecBlowQueue3Team[j]);

				vecFightTeam.push_back(cFightTeam);
			}
		}
		break;
	default:
		break;
	}

	return;
}

/**
* @功能：设置队伍的剩余血量 
*/
void CWarCity::setTeamLastHP(uint32 idTeam, uint32 lastHP)
{
	std::map<uint32, CWarCityTeam>::iterator iter = m_mapFightTeam.find(idTeam);
	if (iter != m_mapFightTeam.end())
	{
		CWarCityTeam &cCityTeam = iter->second;
		cCityTeam.m_nLastHP = lastHP;
	}

	std::map<uint32, CCityPickTeam>::iterator iter1 = m_mapPickTeam.find(idTeam);
	if (iter1 != m_mapPickTeam.end())
	{
		CWarCityTeam &cCityTeam = iter1->second.m_cTeam;
		cCityTeam.m_nLastHP = lastHP;
	}

	return;
}

/**
* @功能：获取战斗队伍的血量
*/
uint32  CWarCity::getFightTeamHp(uint32 idTeam)
{
	std::map<uint32, CWarCityTeam>::iterator iter = m_mapFightTeam.find(idTeam);
	if (iter != m_mapFightTeam.end())
	{
		CWarCityTeam &cCityTeam = iter->second;
		return cCityTeam.m_nLastHP;
	}

	return 0;
}


/**
* @功能：获取队列的战斗队伍 
*/
void CWarCity::getQueueFightTeam(uint8 queueNum, CCityFightTeam &cFightTeam, CTeamBatReport &cbatReport)
{
	switch (queueNum)
	{
	case CITY_QUEUE_NUM_FIRST:
		{
			if (m_strBattleReport1.size() < 2)
				return;

			cbatReport = cbatReport1;
			if (m_vecAttQueue1Team.size() > 0)
				cFightTeam.m_vecAttTeam.push_back(m_vecAttQueue1Team[0]);

			if (m_vecBlowQueue1Team.size() > 0)
				cFightTeam.m_vecBlowTeam.push_back(m_vecBlowQueue1Team[0]);
		}
		break;
	case CITY_QUEUE_NUM_SECOND:
		{
			if (m_strBattleReport2.size() < 2)
				return;

			cbatReport = cbatReport2;
			if (m_vecAttQueue2Team.size() > 0)
				cFightTeam.m_vecAttTeam.push_back(m_vecAttQueue2Team[0]);

			if (m_vecBlowQueue2Team.size() > 0)
				cFightTeam.m_vecBlowTeam.push_back(m_vecBlowQueue2Team[0]);
		}
		break;
	case CITY_QUEUE_NUM_THIRTY:
		{
			if (m_strBattleReport3.size() < 2)
				return;

			cbatReport = cbatReport3;
			if (m_vecAttQueue3Team.size() > 0)
				cFightTeam.m_vecAttTeam.push_back(m_vecAttQueue3Team[0]);

			if (m_vecBlowQueue3Team.size() > 0)
				cFightTeam.m_vecBlowTeam.push_back(m_vecBlowQueue3Team[0]);
		}
		break;
	default:
		break;
	}

	return;
}


/**
* @功能：获取战斗中的队伍
*/
void CWarCity::getInFightTeam(uint32 idTeam, CWarCityTeam &cCityTeam)
{
	std::map<uint32, CWarCityTeam>::iterator iter = m_mapFightTeam.find(idTeam);
	if (iter == m_mapFightTeam.end())
		return;

	cCityTeam = iter->second;

	return;
}

/**
* @功能：获取刚进入战斗的玩家
*/
void CWarCity::getInFightPlayer(OBJID &idPlayer, uint32 &idTeam)
{
	if (m_vecInFightPlayer.size() == 0)
		return;

	CWarCityTeam &cCityTeam = m_vecInFightPlayer[0];
	idPlayer = cCityTeam.m_nAvatarID;
    idTeam = cCityTeam.m_nTeamID;
	m_vecInFightPlayer.clear();
	return;
}

/**
* @功能：获取刚进入战斗的玩家
*/
void CWarCity::updSteamLastHP(uint8 nQueueNum, uint32 nLastHP, uint8 nFailDir)
{
	switch (nQueueNum)
	{
	case CITY_QUEUE_NUM_FIRST:
		{
			if (nFailDir == WAR_DIR_TYPE_ATT)//失败方 1攻击者 2防守者
			{
				if(m_vecBlowQueue1Team.size() > 0)
				{
					CCitySTeam &cSteam = m_vecBlowQueue1Team[0];
					cSteam.m_nLastHP = nLastHP;
				}
			}
			else
			{
				if(m_vecAttQueue1Team.size() > 0)
				{
					CCitySTeam &cSteam = m_vecAttQueue1Team[0];
					cSteam.m_nLastHP = nLastHP;
				}
			}
		}
		break;
	case CITY_QUEUE_NUM_SECOND:
		{
			if (nFailDir == WAR_DIR_TYPE_ATT)//失败方 1攻击者 2防守者
			{
				if(m_vecBlowQueue2Team.size() > 0)
				{
					CCitySTeam &cSteam = m_vecBlowQueue2Team[0];
					cSteam.m_nLastHP = nLastHP;
				}
			}
			else
			{
				if(m_vecAttQueue2Team.size() > 0)
				{
					CCitySTeam &cSteam = m_vecAttQueue2Team[0];
					cSteam.m_nLastHP = nLastHP;
				}
			}
		}
		break;
	case CITY_QUEUE_NUM_THIRTY:
		{
			if (nFailDir == WAR_DIR_TYPE_ATT)//失败方 1攻击者 2防守者
			{
				if(m_vecBlowQueue3Team.size() > 0)
				{
					CCitySTeam &cSteam = m_vecBlowQueue3Team[0];
					cSteam.m_nLastHP = nLastHP;
				}
			}
			else
			{
				if(m_vecAttQueue3Team.size() > 0)
				{
					CCitySTeam &cSteam = m_vecAttQueue3Team[0];
					cSteam.m_nLastHP = nLastHP;
				}
			}
		}
		break;
	default:
		break;
	}

	return;
}

/**
* @功能：设置队列剩余血量
*/
void CWarCity::setQueueLastHP(uint8 nQueueNum, uint32 lastHP)
{
	switch (nQueueNum)
	{
	case CITY_QUEUE_NUM_FIRST:
		{
			m_nTeamLastHP1 = lastHP;
		}
		break;
	case CITY_QUEUE_NUM_SECOND:
		{
			m_nTeamLastHP2 = lastHP;
		}
		break;
	case CITY_QUEUE_NUM_THIRTY:
		{
			m_nTeamLastHP3 = lastHP;
		}
		break;
	default:
		break;
	}

	return;
}

uint32 CWarCity::getQueueLastHP(uint8 nQueueNum)
{
	switch (nQueueNum)
	{
	case CITY_QUEUE_NUM_FIRST:
		{
			return m_nTeamLastHP1;
		}
		break;
	case CITY_QUEUE_NUM_SECOND:
		{
			return m_nTeamLastHP2;
		}
		break;
	case CITY_QUEUE_NUM_THIRTY:
		{
			return m_nTeamLastHP3;
		}
		break;
	default:
		break;
	}

	return 0;
}

/**
* @功能：获取城战的所有玩家
*/
void CWarCity::getCityPlayer(std::vector<OBJID> &vecPlayer)
{
	std::map<OBJID, uint8>::iterator iter =  m_mapEnterPlayer.begin();
	for (; iter != m_mapEnterPlayer.end(); ++iter)
	{
		vecPlayer.push_back(iter->first);
	}
}

/**
* @功能：增加防守大本营队伍
*/
bool CWarCity::addBlowTeam(CWarCityTeam &cCityTeam)
{
	m_vecBlowTeam.push_back(cCityTeam);

	return false;

	/* if (m_nStatus == 0)	    
		return false;

	if (m_vecBlowTeam.size() != 1)
		return false;

	if (this->isBlowQueueFull(CITY_QUEUE_NUM_FIRST) && this->isBlowQueueFull(CITY_QUEUE_NUM_SECOND) && this->isBlowQueueFull(CITY_QUEUE_NUM_THIRTY))
		return false;

	m_mapFightTeam.insert(make_pair(cCityTeam.m_nTeamID, cCityTeam));
	this->addTeamToBlowQueueOrder(cCityTeam);
	m_vecBlowTeam.clear();

	return true; */
}

/**
* @功能：增加攻击大本营队伍
*/
bool CWarCity::addAttTeam(CWarCityTeam &cCityTeam)
{
	m_vecAttTeam.push_back(cCityTeam);

	return false;
	/* if (m_nStatus == 0)	    
		return false;

	if (m_vecAttTeam.size() != 1)
		return false;

	if (this->isAttQueueFull(CITY_QUEUE_NUM_FIRST) && this->isAttQueueFull(CITY_QUEUE_NUM_SECOND) && this->isAttQueueFull(CITY_QUEUE_NUM_THIRTY))
		return false;

	m_mapFightTeam.insert(make_pair(cCityTeam.m_nTeamID, cCityTeam));
	this->addTeamToAttQueueOrder(cCityTeam);
	m_vecAttTeam.clear();
	return true; */
}
void CWarCity::addJoinPlayer(OBJID idPlayer)
{
	m_mapEnterPlayer[idPlayer] = 0;
	m_mapFightPlayer[idPlayer] = 0;
}
void CWarCity::eraseJoinPlayer(OBJID idPlayer)
{
	m_mapEnterPlayer.erase(idPlayer);
}

/**
* @功能：获取城战的所有玩家
*/
void CWarCity::getAllDefPlayer(uint8 idNation, uint8 nDir, std::vector<OBJID> &vecPlayer)
{
	std::map<OBJID, CPlayerCityWarInfPtr>::iterator iter = m_mapPlayerTeam.begin(); 
	for (; iter != m_mapPlayerTeam.end(); ++iter)
	{
		CPlayerCityWarInfPtr pPlayerWarPtr = iter->second;
		if (pPlayerWarPtr == NULL)
			continue;

		if (pPlayerWarPtr->m_nNation != idNation)
			continue;

		if (pPlayerWarPtr->m_nDir != nDir)
			continue;

		if (pPlayerWarPtr->m_vecTeamInf.size() == 0)
			continue;
		vecPlayer.push_back(iter->first);
	}
    
	return;
}

/**
* @功能：获取单挑的NPC统帅队伍(防守兵)
*/
bool CWarCity::getBlowPickCaptainTeam(CWarCityTeam &cNpcWarTeam, CWarCityTeam &cPlayerWarTeam)
{
	if (m_vecAttTeam.size() == 0 || m_vecBlowNPCTeam.size() == 0)
		return false;

	bool bnpcteam = false;
	bool bplayerteam =  false;

	for (size_t i = 0; i < m_vecBlowNPCTeam.size(); i++)
	{
		CWarCityTeam &cWarTeam = m_vecBlowNPCTeam[i];
		uint32 idTeam = cWarTeam.m_nTeamID;

		//判断队伍是否是在战斗中
		if (this->isInPreBatTeam(idTeam))
			continue;

		if (!this->isCaptainTeam(idTeam))
			continue;

		cNpcWarTeam = m_vecBlowNPCTeam[i];
		bnpcteam = true;
		break;
	}

	for (size_t i = 0; i < m_vecAttTeam.size(); i++)
	{
		CWarCityTeam &cWarTeam = m_vecAttTeam[i];
		uint32 idTeam = cWarTeam.m_nTeamID;

		//判断队伍是否是在战斗中
		if (this->isInPreBatTeam(idTeam))
			continue;

		cPlayerWarTeam = m_vecAttTeam[i];
		bplayerteam = true;
		break;
	}

	if (bnpcteam  &&  bplayerteam)
	{
		CDateTime now;
		m_mapFightTeam.insert(make_pair(cNpcWarTeam.m_nTeamID, cNpcWarTeam));
		m_mapFightTeam.insert(make_pair(cPlayerWarTeam.m_nTeamID, cPlayerWarTeam));
		CCityPickTeam cPickTeam;
		cPickTeam.m_cTeam = cNpcWarTeam;
		cPickTeam.m_nTeamType = TEAM_ATT_TYPE_DEF;
		cPickTeam.m_nTime = now.getTotalSecs();
		m_mapPickTeam.insert(make_pair(cNpcWarTeam.m_nTeamID, cPickTeam));

		cPickTeam.m_nTeamType = TEAM_ATT_TYPE_ATT;
		cPickTeam.m_cTeam = cPlayerWarTeam;
		m_mapPickTeam.insert(make_pair(cPlayerWarTeam.m_nTeamID, cPickTeam));

		std::vector<CWarCityTeam>::iterator iter = m_vecBlowNPCTeam.begin();
		for (; iter != m_vecBlowNPCTeam.end(); ++iter)
		{
			uint32 idTeam = iter->m_nTeamID;
			if (idTeam == cNpcWarTeam.m_nTeamID)
			{
				m_vecBlowNPCTeam.erase(iter);
				break;
			}
		}

		std::vector<CWarCityTeam>::iterator iter1 = m_vecAttTeam.begin();
		for (; iter1 != m_vecAttTeam.end(); ++iter1)
		{
			uint32 idTeam = iter1->m_nTeamID;
			if (idTeam == cPlayerWarTeam.m_nTeamID)
			{
				m_vecAttTeam.erase(iter1);
				break;
			}
		}
		return true;
	}

	return false;

}

/**
* @功能：获取单挑的NPC统帅队伍
*/
bool CWarCity::getAttPickCaptainTeam(CWarCityTeam &cNpcWarTeam, CWarCityTeam &cPlayerWarTeam)
{
	if (m_vecBlowTeam.size() == 0 || m_vecAttNPCTeam.size() == 0)
		return false;

	bool bnpcteam = false;
	bool bplayerteam =  false;

	for (size_t i = 0; i < m_vecAttNPCTeam.size(); i++)
	{
		CWarCityTeam &cWarTeam = m_vecAttNPCTeam[i];
		uint32 idTeam = cWarTeam.m_nTeamID;

		//判断队伍是否是在战斗中
		if (this->isInPreBatTeam(idTeam))
			continue;
        
		if (!this->isCaptainTeam(idTeam))
			continue;
		cNpcWarTeam = m_vecAttNPCTeam[i];
		bnpcteam = true;
		break;
	}
	
	for (size_t i = 0; i < m_vecBlowTeam.size(); i++)
	{
		CWarCityTeam &cWarTeam = m_vecBlowTeam[i];
		uint32 idTeam = cWarTeam.m_nTeamID;

		//判断队伍是否是在战斗中
		if (this->isInPreBatTeam(idTeam))
			continue;

		cPlayerWarTeam = m_vecBlowTeam[i];
		bplayerteam = true;
		break;
	}

	if (bnpcteam  &&  bplayerteam)
	{
		CDateTime now;
		m_mapFightTeam.insert(make_pair(cNpcWarTeam.m_nTeamID, cNpcWarTeam));
		m_mapFightTeam.insert(make_pair(cPlayerWarTeam.m_nTeamID, cPlayerWarTeam));
		CCityPickTeam cPickTeam;
		cPickTeam.m_cTeam = cNpcWarTeam;
		cPickTeam.m_nTeamType = TEAM_ATT_TYPE_ATT;
		cPickTeam.m_nTime = now.getTotalSecs();
		m_mapPickTeam.insert(make_pair(cNpcWarTeam.m_nTeamID, cPickTeam));

		cPickTeam.m_nTeamType = TEAM_ATT_TYPE_DEF;
		cPickTeam.m_cTeam = cPlayerWarTeam;
		m_mapPickTeam.insert(make_pair(cPlayerWarTeam.m_nTeamID, cPickTeam));

		std::vector<CWarCityTeam>::iterator iter = m_vecAttNPCTeam.begin();
		for (; iter != m_vecAttNPCTeam.end(); ++iter)
		{
			uint32 idTeam = iter->m_nTeamID;
			if (idTeam == cNpcWarTeam.m_nTeamID)
			{
				m_vecAttNPCTeam.erase(iter);
				break;
			}
		}

		std::vector<CWarCityTeam>::iterator iter1 = m_vecBlowTeam.begin();
		for (; iter1 != m_vecBlowTeam.end(); ++iter1)
		{
			uint32 idTeam = iter1->m_nTeamID;
			if (idTeam == cPlayerWarTeam.m_nTeamID)
			{
				m_vecBlowTeam.erase(iter1);
				break;
			}
		}
		return true;
	}
    
	return false;
}
bool CWarCity::isCaptainTeam(uint32 idTeam)
{
	std::map<uint32, uint8>::iterator iter = m_mapCaptainTeam.find(idTeam);
	if (iter == m_mapCaptainTeam.end())
		return false;
	return true;
}

void CWarCity::addBlewTeam(uint32 idTeam, uint8 nType)
{
	m_mapBlewTeam[idTeam] = 0;
	if (nType == 0)
		bHaveDefBlewTeam = true;
	else
		bHaveAttBlewTeam = true;

}

bool CWarCity::isBlewTeam(uint32 idTeam)
{
	std::map<uint32, uint8>::iterator iter = m_mapBlewTeam.find(idTeam);
	if (iter == m_mapBlewTeam.end())
		return false;
	return true;
}

//设置道具奖励经验
void CWarCity::setItemRewardExp(std::map<uint8, uint32> &mapItemRewardExp)
{
	m_mapItemRewardExp.clear();

	std::map<uint8, uint32>::iterator iter = mapItemRewardExp.begin();
	for (; iter != mapItemRewardExp.end(); ++iter)
	{
		m_mapItemRewardExp[iter->first] = iter->second;
	}

    return;
}


//获取战报ID
uint32 CWarCity::getBattleID()
{
	m_nBattleID++;
	if (m_nBattleID >= 99999)
		m_nBattleID = 1;

	uint32 nCity  = (uint32)m_nCityID;

	return nCity * 100000 + m_nBattleID;
}

//保存战报
void CWarCity::saveBattle(uint32 idBattle, std::string &strBattle)
{
	m_mapBattle[idBattle] = strBattle;
}

//查询战报
std::string CWarCity::queryBattle(uint32 idBattle)
{
	std::map<uint32, std::string>::iterator iter = m_mapBattle.find(idBattle);
	if (iter != m_mapBattle.end())
		return iter->second;

	return "1";
}

uint32 CWarCity::getQueueNextTime(uint8 nQueueNum)
{
	switch (nQueueNum)
	{
	case CITY_QUEUE_NUM_FIRST:
		return m_nQueueNextTime1;
	case CITY_QUEUE_NUM_SECOND:
		return m_nQueueNextTime2;
	case CITY_QUEUE_NUM_THIRTY:
		return m_nQueueNextTime3;
	}

	return 0;
}
uint32 CWarCity::getQueueSession(uint8 nQueueNum)
{
	switch (nQueueNum)
	{
	case CITY_QUEUE_NUM_FIRST:
		return this->getSession1();
	case CITY_QUEUE_NUM_SECOND:
		return this->getSession2();
	case CITY_QUEUE_NUM_THIRTY:
		return this->getSession3();
	}
	return this->getSession1();
}

void CWarCity::checkQueueStatuts(uint8 nQueueNum)
{
	if (this->getStatus() != 1)
		return;

	switch (nQueueNum)
	{
	case CITY_QUEUE_NUM_FIRST:
		{
			if (m_vecAttNPCTeam.size() == 0 && m_vecAttTeam.size() == 0 && m_vecAttQueue1Team.size() == 0 && m_mapPickTeam.size() == 0)
			{
				m_nQueueStatus = m_nQueueStatus|1;
				this->teamRebackCamp(TEAM_ATT_TYPE_DEF, m_vecBlowQueue1Team);
				return;
			}

			if (m_vecBlowNPCTeam.size() == 0 && m_vecBlowTeam.size() == 0 && m_vecBlowQueue1Team.size() == 0 && m_mapPickTeam.size() == 0)
			{
				m_nQueueStatus = m_nQueueStatus|1;
				m_nQueueFail = m_nQueueFail | 1;
				this->teamRebackCamp(TEAM_ATT_TYPE_ATT, m_vecAttQueue1Team);
				return;
			}
		}
		break;
	case CITY_QUEUE_NUM_SECOND:
		{
			if (m_vecAttNPCTeam.size() == 0 && m_vecAttTeam.size() == 0 && m_vecAttQueue2Team.size() == 0 && m_mapPickTeam.size() == 0)
			{
				m_nQueueStatus = m_nQueueStatus|2;
				this->teamRebackCamp(TEAM_ATT_TYPE_DEF, m_vecBlowQueue2Team);
				return;
			}

			if (m_vecBlowNPCTeam.size() == 0 && m_vecBlowTeam.size() == 0 && m_vecBlowQueue2Team.size() == 0 && m_mapPickTeam.size() == 0)
			{
				m_nQueueStatus = m_nQueueStatus|2;
				m_nQueueFail = m_nQueueFail | 2;
				this->teamRebackCamp(TEAM_ATT_TYPE_ATT, m_vecAttQueue2Team);
				return;
			}
		}
		break;
	case CITY_QUEUE_NUM_THIRTY:
		{
			if (m_vecAttNPCTeam.size() == 0 && m_vecAttTeam.size() == 0 && m_vecAttQueue3Team.size() == 0 && m_mapPickTeam.size() == 0)
			{
				m_nQueueStatus = m_nQueueStatus|4;
				this->teamRebackCamp(TEAM_ATT_TYPE_DEF, m_vecBlowQueue3Team);
				return;
			}

			if (m_vecBlowNPCTeam.size() == 0 && m_vecBlowTeam.size() == 0 && m_vecBlowQueue3Team.size() == 0 && m_mapPickTeam.size() == 0)
			{
				m_nQueueStatus = m_nQueueStatus|4;
				m_nQueueFail = m_nQueueFail |4;
				this->teamRebackCamp(TEAM_ATT_TYPE_ATT, m_vecAttQueue3Team);
				return;
			}
		}
		break;
	}
}
void CWarCity::teamRebackCamp(uint8 attType,  std::vector<CCitySTeam> &vecQueueTeam)
{
	if (vecQueueTeam.size() == 0)
		return;

	for (size_t i  = 0; i < vecQueueTeam.size(); i++)
	{
		CCitySTeam &cSTeam = vecQueueTeam[i];
		bool btrue = false;
		std::map<uint32, CWarCityTeam>::iterator iter = m_mapFightTeam.find(cSTeam.m_nTeamID);
		if (iter != m_mapFightTeam.end())
		{
			btrue = true;
			CWarCityTeam &cCityTeam = iter->second;
			if (attType == TEAM_ATT_TYPE_DEF)
			{
				if (cCityTeam.m_nTeamType == TEAM_TYPE_PLAYER)
					m_vecBlowTeam.push_back(cCityTeam);
				else
					m_vecBlowNPCTeam.push_back(cCityTeam);
			}
			else
			{
				if (cCityTeam.m_nTeamType == TEAM_TYPE_PLAYER)
					m_vecAttTeam.push_back(cCityTeam);
				else
					m_vecAttNPCTeam.push_back(cCityTeam);
			}
		}

		m_mapFightTeam.erase(cSTeam.m_nTeamID);
	}

	vecQueueTeam.clear();
	return;
}

//清理队伍
void  CWarCity::clearDirtyFightTeam()
{
	if (this->getBlowTeamNum() == 0 || this->getAttTeamNum() == 0)
		return;

	if (this->getBlowTeamNum() >= this->getAttTeamNum())
	{
		m_vecAttNPCTeam.clear();
		m_vecAttTeam.clear();
		m_vecAttQueue1Team.clear(); 
		m_vecAttQueue2Team.clear();
		m_vecAttQueue3Team.clear();
		m_mapPickTeam.clear();
	}
	else if (this->getBlowTeamNum() < this->getAttTeamNum())
	{
		m_vecBlowTeam.clear();
		m_vecBlowNPCTeam.clear();
		m_vecBlowQueue1Team.clear(); 
		m_vecBlowQueue2Team.clear();
		m_vecBlowQueue3Team.clear();
		m_mapPickTeam.clear();
	}
	return;
}

/**
* @功能：返还道具
*/
void CWarCity::returnDeployedItem()
{
	std::string strTitle = m_pConsummer->formatLangString(LANGTYPE_WORLD_WAR, LANGTYPE_WORLD_WAR_REBACK_ITEM_TILE);
	std::string  strContent = m_pConsummer->formatLangString(LANGTYPE_WORLD_WAR, LANGTYPE_WORLD_WAR_REBACK_ITEM_CONTENT, m_strName.data());

	std::map<OBJID, CPlayerCityWarInfPtr>::iterator iter = m_mapPlayerTeam.begin();
	for (; iter != m_mapPlayerTeam.end(); ++iter)
	{
		OBJID idPlayer = iter->first;
		CPlayerCityWarInfPtr pPlayerWarInfPtr = iter->second;
		if (pPlayerWarInfPtr == NULL)
			continue;

		if (pPlayerWarInfPtr->m_nDeployNum == 0)
			continue;

		uint16 nReturnNum = pPlayerWarInfPtr->m_nDeployNum;

		if (pPlayerWarInfPtr->m_nNation != this->getNation() && pPlayerWarInfPtr->m_nNation != this->getAttNation())
		{
			if ((this->getNation() >= NATIONAL_WEI && this->getNation() <= NATIONAL_WU) ||  
				(this->getAttNation() >= NATIONAL_WEI &&  this->getAttNation() <= NATIONAL_WU))
				nReturnNum = nReturnNum * 2;
		}

		std::vector<CRewardInfo> vecPlayerReward;
		CRewardInfo cReward;
		cReward.m_nType = REWARDTYPE_ITEM;
		cReward.m_nValue = m_nDeployedItem;
		cReward.m_nNum = nReturnNum;
		vecPlayerReward.push_back(cReward);

		m_pConsummer->sendMailToPlayer(idPlayer, strTitle, strContent, vecPlayerReward, REWARD_SOURCE_WORLD_END_REBACK_ITEM);
	}
}