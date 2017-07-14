/******************************************************
*			Copyright 2013, by linglong.
*				All right reserved.
*���ܣ���ս���ʵ��
*���ڣ�2015-5-12 15:05
*���ߣ�milo_woo
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
* @���ܣ���ȡ�����Ͻ����
*/
void CWarNation::getAllDomainReward(std::vector<CDomainReward> &vecDomainReward)
{
	vecDomainReward.assign(m_vecDomainReward.begin(), m_vecDomainReward.end());
	return;
}

/**
* @���ܣ���ȡĳСʱ�����Ͻ����
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

//��ȡ��ս��Ա��Ϣ
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
* @���ܣ���ȡ����ID
*/
uint32  CWarNation::getTeamID()
{
	m_nTeamID++;
	if (m_nTeamID >= 99999)
		m_nTeamID = 1;

	uint32 nNation  = (uint32)m_nNationID;

	return (nNation * 100000 + m_nTeamID);
}

//���Ӷ���ID
void CWarNation::addNewTeamID(uint32 idTeam)
{
    uint32 idInnerTeam = idTeam %100000;
	if (idInnerTeam > m_nTeamID)
		m_nTeamID = idInnerTeam;
}

/**
* @���ܣ�������פ������
*/
void CWarNation::addNewEtapeTeam(CEtapeTeam &cEtapeTeam)
{
	m_vecEtapeTeam.push_back(cEtapeTeam);
}

/**
* @���ܣ�ɾ��פ������
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
* @���ܣ���ȡ���������б�
*/
void CWarNation::getEtapeNpcTeamList(std::vector<CEtapeTeam> &vecEtapeTeam)
{
	if (m_vecEtapeNpcTeam.size() == 0)
		return;

	vecEtapeTeam.assign(m_vecEtapeNpcTeam.begin(), m_vecEtapeNpcTeam.end());
	return;
}

/**
* @���ܣ���ȡפ����������
*/
uint16 CWarNation::getEtapeTeamNum()
{
    return m_vecEtapeTeam.size() + m_vecEtapeNpcTeam.size();
}


/**
* @���ܣ���ȡ������������
*/
uint16 CWarNation::getNpcTeamNum()
{
    return m_vecEtapeNpcTeam.size();
}

/**
* @���ܣ���ȡפ�������б�
*/
void CWarNation::getEtapeTeamList(std::vector<CEtapeTeam> &vecEtapeTeam)
{
	if (m_vecEtapeTeam.size() == 0)
		return;

    vecEtapeTeam.assign(m_vecEtapeTeam.begin(), m_vecEtapeTeam.end());
	return;
}


/**
* @���ܣ�������ҳ�ս��Ϣ
*/
void  CWarNation::addPlayerCityInf(OBJID idPlayer, CPlayerCityWarInfPtr pPlayerCityIntPtr)
{
	m_mapPlayerInf.insert(make_pair(idPlayer, pPlayerCityIntPtr));

	return;
}

/**
* @���ܣ���ն�����Ϣ
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

//��������Ϣ
void CWarNation::clearDomainReward()
{
	m_vecDomainReward.clear();

	return;
}

/**
* @���ܣ�У�齱��
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

