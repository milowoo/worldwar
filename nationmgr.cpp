/*********************************************************************
*  Copyright 2014, by linglong.
*  All right reserved.
*
*  功能：国家系统2.0
*
*  Edit History:
*
*    2015/08/20 - RaymondCheung 创建
******************************************************/

#include <net/netdefines.h>
#include <business/functiondefines.h>
#include <business/langdefines.h>
#include <business/chat.h>
#include "module.h"
#include "nationmgr.h"
#include "worldwarmgr.h"

#include <gateway/gwnationinfonotice.h>

#include <gatewaymgr/gwmaddnationexp.h>
#include <gatewaymgr/gwmgmsetnationinf.h>

#include <dbmgr/dbupdnationinf.h>

/******************************************** 文件开始 ****************************************************/
CNationMgr::CNationMgr()
{
	m_vecGateWaySession.clear();
}

CNationMgr::~CNationMgr()
{
	this->onServerStop();
}

void CNationMgr::relolutionTable(bool bUpdate)
{
	if (NULL == g_pConsummer)
	{
		return;
	}
}

void  CNationMgr::playerOnline(OBJID idPlayer)
{
	//log_debug("CNationMgr::playerOnline("I64_FMT")", idPlayer);
}


void CNationMgr::playerOffline(OBJID idPlayer)
{
	//log_debug("player("I64_FMT") offline|CNationMgr::playerOffline", idPlayer);
	return;
}

int CNationMgr::onTimer()
{	
	return 0;
}

void CNationMgr::onServerStop()
{
	m_vecGateWaySession.clear();
	return;
}

//国家信息通知
IResponsePtr CNationMgr::executeAddNationExp(IRequestPtr Request,CCurrentPtr Current)
{
	CGWMAddNationExpRequestPtr pRequest = new CGWMAddNationExpRequest(Request->getSerialStream());

	uint8 nNation = pRequest->m_nNation;
	bool isAddDailyExp = !(pRequest->m_nIsForceAddExp);
	//检验阵营是否合法
	if (NATIONAL_NONE == nNation || nNation >= NATIONAL_ALL)
	{
		log_debug("not valid nation(%d), addExp(%d)|CNationMgr::executeAddNationExp", pRequest->m_nNation, pRequest->m_nAddExp);
		return NULL;
	}

	//获取国家信息
	CNationInfoPtr pNationInf = g_pConsummer->getNationInf(nNation);
	if (NULL == pNationInf)
	{
		log_debug("no nation(%d) inf, addExp(%d)|CNationMgr::executeAddNationExp", pRequest->m_nNation, pRequest->m_nAddExp);
		return NULL;
	}

	//获取国家配置(当前国家等级)
	CNationCfgPtr pNationCfg = g_pConsummer->getNationCfg(pNationInf->m_nCurLevel);
	if (NULL == pNationCfg)
	{
		log_debug("no nation(%d) cfg, curLevel(%d)|CNationMgr::executeAddNationExp", pNationInf->m_nNation, pNationInf->m_nAccLevel);
		return NULL;
	}
	//检验建设度上限是否为0
	if (!pNationCfg->m_nDailyExpLmt || !pNationCfg->m_nTotalExpLmt)
	{
		return NULL;
	}

	//计算实际增加的建设度
	uint32 nRealAddExp = getRealAddExp(nNation, pRequest->m_nAddExp, isAddDailyExp);
	if (!nRealAddExp)
	{
		return NULL;
	}

	std::vector<uint32> vecKey;
	if (isAddDailyExp)
	{
		//增加当日建设度
		pNationInf->m_nTodayExp += nRealAddExp;
		vecKey.push_back(NATION_UPD_TODAY_EXP);
	}

	//增加总建设度
	pNationInf->m_nTotalExp += nRealAddExp;
	vecKey.push_back(NATION_UPD_TOTAL_EXP);

	uint8 nPreLevel = pNationInf->m_nCurLevel;
	//更新国家等级
	while (pNationInf->m_nTotalExp >= pNationCfg->m_nTotalExpLmt)
	{
		++ pNationInf->m_nCurLevel;
		pNationInf->m_nTotalExp -= pNationCfg->m_nTotalExpLmt;

		pNationCfg = g_pConsummer->getNationCfg(pNationInf->m_nCurLevel);
		if (NULL == pNationCfg)
		{
			pNationInf->m_nTotalExp = 0;
			break;
		}

		if (pNationCfg->m_nIsAnnounce)
		{
			//发送公告
			sendLvUpAnnouce(nNation, pNationCfg->m_nLevel);
		}
	}

	if (nPreLevel != pNationInf->m_nCurLevel)
	{
		vecKey.push_back(NATION_UPD_CUR_LEVEL);
	}

	//通知db更新
	updNationInfoDB(nNation, vecKey);
	//下发gateway
	issuNationInfo(nNation, vecKey);

	return NULL;
}

//gm设置国家信息
IResponsePtr CNationMgr::executeGmSetNationInf(IRequestPtr Request,CCurrentPtr Current)
{
	CGWMGmSetNationInfoRequestPtr pRequest = new CGWMGmSetNationInfoRequest(Request->getSerialStream());
	
	uint8& nNation = pRequest->m_nNation;
	CUpdValues& nUpd = pRequest->m_nUpdValue;
	log_debug("nation(%d), key(%d), nValue(%d), strValue(%s)|CNationMgr::executeGmSetNationInf", 
		nNation, nUpd.m_nKey, nUpd.m_nValues, nUpd.m_strValues.c_str());

	//获取国家信息
	CNationInfoPtr pNationInf = g_pConsummer->getNationInf(nNation);
	if (NULL == pNationInf)
	{
		return NULL;
	}

	std::vector<uint32> vecKey;
	switch(nUpd.m_nKey)
	{
	case NATION_UPD_ACC_LEVEL:	//清算时国家等级
		{
			//检验清算等级是否合法
			if (nUpd.m_nValues > pNationInf->m_nCurLevel)
			{
				break;
			}

			pNationInf->m_nAccLevel = nUpd.m_nValues;
			pNationInf->m_nTodayExp = 0;

			vecKey.push_back(nUpd.m_nKey);
			vecKey.push_back(NATION_UPD_TODAY_EXP);
		}
		break;
	case NATION_UPD_CUR_LEVEL:	//当前国家等级
		{
			//获取国家配置
			CNationCfgPtr pNationCfg = g_pConsummer->getNationCfg(nUpd.m_nValues);
			if (NULL == pNationCfg)
			{
				break;
			}

			pNationInf->m_nCurLevel = nUpd.m_nValues;
			pNationInf->m_nTotalExp = 0;
			pNationInf->m_nTodayExp = 0;

			vecKey.push_back(nUpd.m_nKey);
			vecKey.push_back(NATION_UPD_TOTAL_EXP);
			vecKey.push_back(NATION_UPD_TODAY_EXP);

			if (pNationCfg->m_nIsAnnounce)
			{
				sendLvUpAnnouce(nNation, pNationInf->m_nCurLevel);
			}
		}
		break;
	case NATION_UPD_TODAY_EXP:	//当前建设度
		{
			//获取国家配置
			CNationCfgPtr pNationCfg = g_pConsummer->getNationCfg(pNationInf->m_nAccLevel);
			if (NULL == pNationCfg)
			{
				break;
			}

			if (pNationInf->m_nTodayExp >= pNationCfg->m_nDailyExpLmt)
			{
				break;
			}

			pNationInf->m_nTodayExp = nUpd.m_nValues;
			if (pNationInf->m_nTodayExp > pNationCfg->m_nDailyExpLmt)
			{
				pNationInf->m_nTodayExp = pNationCfg->m_nDailyExpLmt;
			}

			vecKey.push_back(nUpd.m_nKey);
		}
		break;
	case NATION_UPD_TOTAL_EXP:	//总建设度
		{
			//获取国家配置
			CNationCfgPtr pNationCfg = g_pConsummer->getNationCfg(pNationInf->m_nCurLevel);
			if (NULL == pNationCfg)
			{
				break;
			}

			if (pNationInf->m_nTotalExp >= pNationCfg->m_nTotalExpLmt)
			{
				break;
			}

			pNationInf->m_nTotalExp = nUpd.m_nValues;
			if (pNationInf->m_nTotalExp >= pNationCfg->m_nTotalExpLmt)
			{
				pNationInf->m_nTotalExp = pNationCfg->m_nTotalExpLmt - 1;
			}

			vecKey.push_back(nUpd.m_nKey);
		}
		break;
	default:
		break;
	}

	if (!vecKey.empty())
	{
		this->updNationInfoDB(nNation, vecKey);
		this->issuNationInfo(nNation, vecKey);
	}
	return NULL;
}

//每日清算
void CNationMgr::dailyAccount()
{
	std::vector<uint32> vecKey;
	vecKey.push_back(NATION_UPD_ACC_LEVEL);
	vecKey.push_back(NATION_UPD_TODAY_EXP);
	vecKey.push_back(NATION_UPD_ACC_DAY);

	std::vector<uint8> vecNation;
	vecNation.push_back(NATIONAL_WEI);
	vecNation.push_back(NATIONAL_SHU);
	vecNation.push_back(NATIONAL_WU);

	CNationInfoPtr pNationInf = NULL;
	for (size_t i = 0; i <vecNation.size(); i ++)
	{
		uint8& nNation = vecNation[i];
		//获取国家信息
		pNationInf = g_pConsummer->getNationInf(nNation);
		if (NULL == pNationInf)
		{
			continue;
		}

		pNationInf->m_nAccLevel = pNationInf->m_nCurLevel;
		pNationInf->m_nTodayExp = 0;
		this->updNationInfoDB(nNation, vecKey);
		this->issuNationInfo(nNation, vecKey);
	}
}

//计算实际增加的建设度
uint32 CNationMgr::getRealAddExp(uint8 nNation, uint32 nAddExp, bool isAddDailyExp)
{
	//检验阵营是否合法
	if (NATIONAL_NONE == nNation || nNation >= NATIONAL_ALL)
	{
		log_debug("not valid nation(%d), addExp(%d)|CNationMgr::executeAddNationExp", nNation, nAddExp);
		return 0;
	}

	//获取国家信息
	CNationInfoPtr pNationInf = g_pConsummer->getNationInf(nNation);
	if (NULL == pNationInf)
	{
		log_debug("no nation(%d) inf, addExp(%d)|CNationMgr::executeAddNationExp", nNation, nAddExp);
		return 0;
	}

	//获取国家配置(当前国家等级)
	CNationCfgPtr pNationCfg = g_pConsummer->getNationCfg(pNationInf->m_nCurLevel);
	if (NULL == pNationCfg)
	{
		log_debug("no nation(%d) cfg, curLevel(%d)|CNationMgr::executeAddNationExp", pNationInf->m_nNation, pNationInf->m_nAccLevel);
		return 0;
	}
	//检验建设度上限是否为0
	if (!pNationCfg->m_nDailyExpLmt || !pNationCfg->m_nTotalExpLmt)
	{
		return 0;
	}

	uint32 nRealAddExp = nAddExp;
	if (isAddDailyExp)
	{
		//获取国家配置(清算时国家等级)
		pNationCfg = g_pConsummer->getNationCfg(pNationInf->m_nAccLevel);
		if (NULL == pNationCfg)
		{
			log_debug("no nation(%d) cfg, accLevel(%d)|CNationMgr::executeAddNationExp", pNationInf->m_nNation, pNationInf->m_nAccLevel);
			return 0;
		}

		//检验是否达到当日建设度上限
		if (pNationInf->m_nTodayExp >= pNationCfg->m_nDailyExpLmt)
		{
			log_debug("dailyExpLmt, nation(%d) cfg, accLevel(%d), todayExp(%d)|CNationMgr::executeAddNationExp", pNationInf->m_nNation, pNationInf->m_nAccLevel, pNationInf->m_nTodayExp);
			return 0;
		}

		//计算可增加的当日建设度
		if (pNationInf->m_nTodayExp + nAddExp > pNationCfg->m_nDailyExpLmt)
		{
			nRealAddExp = pNationCfg->m_nDailyExpLmt - pNationInf->m_nTodayExp;
		}
	}

	return nRealAddExp;
}

//下发国家数据
void CNationMgr::issuNationInfo(uint8 nNation, const std::vector<uint32>& vecKey)
{
	if (vecKey.empty())
	{
		return;
	}

	CNationInfoPtr pNationInf = g_pConsummer->getNationInf(nNation);
	if (NULL == pNationInf)
	{
		return;
	}
 
 	//通知GATEWAY
 	CGWNationInfoNoticeRequestPtr gwreq = new CGWNationInfoNoticeRequest();
 	gwreq->m_nNation = nNation;
 
 	CUpdValues cInf;
 	for (size_t i = 0; i < vecKey.size(); i ++)
 	{
 		const uint32& nKey = vecKey[i];
		cInf.m_nKey = nKey;
 		switch(nKey)
 		{
 		case NATION_UPD_ACC_LEVEL:	//清算时国家等级
 			{
 				cInf.m_nValues = pNationInf->m_nAccLevel;
 				gwreq->m_vecUpdInf.push_back(cInf);
 			}
 			break;
		case NATION_UPD_CUR_LEVEL:	//当前国家等级
			{
				cInf.m_nValues = pNationInf->m_nCurLevel;
				gwreq->m_vecUpdInf.push_back(cInf);
			}
			break;
		case NATION_UPD_TODAY_EXP:	//当前建设度
			{
				cInf.m_nValues = pNationInf->m_nTodayExp;
				gwreq->m_vecUpdInf.push_back(cInf);
			}
			break;
		case NATION_UPD_TOTAL_EXP:	//总建设度
			{
				cInf.m_nValues = pNationInf->m_nTotalExp;
				gwreq->m_vecUpdInf.push_back(cInf);
			}
			break;
		case NATION_UPD_ACC_DAY:	//清算日期
			{
				cInf.m_nValues = g_pConsummer->getFunAccDay();
				gwreq->m_vecUpdInf.push_back(cInf);
			}
			break;
 		default:
 			break;
 		}
 	}
 	
 	if (gwreq->m_vecUpdInf.empty())
 	{
 		return;
 	}
 
 	m_vecGateWaySession.clear();
 	g_pConsummer->getGateWaySession(m_vecGateWaySession);
 	for( size_t i = 0; i < m_vecGateWaySession.size(); i++ )
 	{
 		g_DaServer->send(m_vecGateWaySession[i], GW_NATION_INF_NOTICE, gwreq.get());
 	}
}

//通知DB更新国家信息
void CNationMgr::updNationInfoDB(uint8 nNation, const std::vector<uint32>& vecKey)
{
	if (vecKey.empty())
	{
		return;
	}

	CNationInfoPtr pNationInf = g_pConsummer->getNationInf(nNation);
	if (NULL == pNationInf)
	{
		return;
	}

	//通知DB
	CDBUpdNationInfoRequestPtr dbreq = new CDBUpdNationInfoRequest();
	dbreq->m_nNation = nNation;

	CUpdValues cInf;
	for (size_t i = 0; i < vecKey.size(); i ++)
	{
		const uint32& nKey = vecKey[i];
		cInf.m_nKey = nKey;
		switch(nKey)
		{
		case NATION_UPD_ACC_LEVEL:	//清算时国家等级
			{
				cInf.m_nValues = pNationInf->m_nAccLevel;
				dbreq->m_vecUpdValue.push_back(cInf);
			}
			break;
		case NATION_UPD_CUR_LEVEL:	//当前国家等级
			{
				cInf.m_nValues = pNationInf->m_nCurLevel;
				dbreq->m_vecUpdValue.push_back(cInf);
			}
			break;
		case NATION_UPD_TODAY_EXP:	//当前建设度
			{
				cInf.m_nValues = pNationInf->m_nTodayExp;
				dbreq->m_vecUpdValue.push_back(cInf);
			}
			break;
		case NATION_UPD_TOTAL_EXP:	//总建设度
			{
				cInf.m_nValues = pNationInf->m_nTotalExp;
				dbreq->m_vecUpdValue.push_back(cInf);
			}
			break;
		case NATION_UPD_ACC_DAY:	//清算日期
			{
				cInf.m_nValues = g_pConsummer->getFunAccDay();
				dbreq->m_vecUpdValue.push_back(cInf);
			}
			break;
		default:
			break;
		}
	}

	if (dbreq->m_vecUpdValue.empty())
	{
		return;
	}

	g_DaServer->callServant(DB_UPD_NATION_INF, dbreq.get(), NULL);
}

//发送升级公告
void CNationMgr::sendLvUpAnnouce(uint8 nNation, uint8 nLevel)
{
	CBytesBufferPtr buf = CBytesBufferPtr::createInstance();
	pushChatNationInfo(buf, nNation);
	std::string strName = g_pConsummer->formatLangString(LANGTYPE_NATION_NAME, nNation);
	std::string strmsg = g_pConsummer->formatLangString(LANGTYPE_NATION, E_LANG_NATION_LEVEL_UP, nLevel);
	sendSystemNotice(CHANNEL_SYSTEM, strmsg, buf);
	sendSystemNotice(CHANNEL_SYSTEM2, strmsg, buf);
}

/******************************************** 文件结束 ****************************************************/

