/*********************************************************************
*  Copyright 2014, by linglong.
*  All right reserved.
*
*  功能:国战模块协议业务处理逻辑
*
*  Edit History:
*
*    2014/07/22 - milowoo 创建
*************************************************************************/
#include <net/netdefines.h>
#include "module.h"
#include "worldwarmgr.h"
#include <business/email.h>
#include <business/bussinesscommon.h>
#include <business/staticdatadef.h>
#include <business/consummer/businessconsummer.h>
#include <business/staticdatamgr.h>

#include <gateway/worldwar.h>
#include <gateway/clworldwarnotice.h>
#include <gateway/gwworldwarnotice.h>
#include <gateway/gwworldcityfight.h>
#include <gateway/gwgetfihgtteaminf.h>
#include <gateway/gwwarplayernotice.h>
#include "gateway/gwgetcityfightteam.h"
#include "gateway/gwbeatretreat.h"

#include <gateway/gwquerycitysatrapbuddy.h>
#include <gateway/gwquerycitysatrap.h>
#include <gateway/gwgetprinceacc.h>
#include <gateway/gwgetallyinf.h>



#include <gatewaymgr/gwmquerycitysatrapbuddy.h>
#include <gatewaymgr/gmwquerycitysatrap.h>
#include <gatewaymgr/gwmadjustcitysatrap.h>
#include <gatewaymgr/gwmabandoncity.h>
#include <gatewaymgr/gwmquerycitysatrapfightinf.h>
#include <gatewaymgr/gwmcitysatrapfightresult.h>

#include <gateway/gwmiddleware.h>

#include <dbmgr/dbupdachieveinfo.h>
#include <dbmgr/dbregcityoccup.h>
#include <dbmgr/dbgetcityaccinf.h>
#include <dbmgr/dbregcityaccplayerteam.h>
#include <dbmgr/dbdelcityoccup.h>
#include <dbmgr/dbregcityacc.h>
#include <dbmgr/dbgetworldexploitlibrcd.h>
#include <dbmgr/dbupdworldexploitlibrcd.h>
#include <dbmgr/dbsetsystemvar.h>
#include <dbmgr/dbregetapedeploy.h>
#include <dbmgr/dbdeletatedeploy.h>
#include <dbmgr/dbgetworldwar.h>
#include <dbmgr/dbupdwarnation.h>
#include <dbmgr/dbregnationdomain.h>
#include <dbmgr/dbupdcitysatrap.h>
#include <dbmgr/dbquerycitysatrapinf.h>
#include <dbmgr/dbdelcitysatrap.h>
#include <dbmgr/dbdelaccinf.h>
#include <dbmgr/dbregprincekillrank.h>

#include <gatewaymgr/gwmworldwarnotice.h>
#include <gatewaymgr/gwmworldwarmove.h>
#include <gatewaymgr/gwmworldwar.h>
#include <gatewaymgr/gwmsystemnotice.h>
#include <gatewaymgr/gwmenterworldwar.h>
#include <gatewaymgr/gwmcityfightdeployed.h>
#include <gatewaymgr/gwmgetcityteamnum.h>
#include "gatewaymgr/gwmdeclarewar.h"
#include "gatewaymgr/gwmexitworldwar.h"

#include <gatewaymgr/gwmselectfightdirect.h>
#include <gatewaymgr/gwmcitydepart.h>
#include <gatewaymgr/gwmgetcityteamlist.h>
#include <gatewaymgr/gwmquerycitybattlereport.h>
#include <gatewaymgr/gwmcitypick.h>
#include <gatewaymgr/gwmquerycityfightrank.h>
#include <gatewaymgr/gwmenterfightcity.h>
#include <gatewaymgr/gwmcitypicknotice.h>
#include <gatewaymgr/gwmworldpraise.h>
#include <gatewaymgr/gwmrcdexploitlibnotice.h>
#include "gatewaymgr/gwmgetfightqueue.h"
#include <gatewaymgr/gwmworldlogin.h>
#include <gatewaymgr/gwmgroupsendemail.h>
#include <gatewaymgr/gwmetapedeployed.h>
#include "gatewaymgr/gwmgetetapeteamlist.h"
#include "gatewaymgr/gwmdeployforce.h"
#include "gatewaymgr/gwmsetvision.h"
#include "gatewaymgr/gwmbeatretreat.h"
#include "gatewaymgr/gwmquerydomainrewardinf.h"
#include "gatewaymgr/gwmgetetapeteamlist.h"
#include "gatewaymgr/gwmgetdeployteamlist.h"
#include "gatewaymgr/gwmexitfightcity.h"
#include "gatewaymgr/gwmqueryworldwarbattle.h"
#include "gatewaymgr/gwmgmupdcityoccup.h"
#include "gatewaymgr/gwmworldwaronline.h"
#include "gatewaymgr/gwmgetkillrank.h"
#include "gatewaymgr/gwmgetkillnumaward.h"




#include "../gatewaymgr/gatewaymgrglobal.h"

#include <net/db_code.h>
#include <dbauto/db_protocol.h>
#include <dbauto/db_client.h>

#include <business/chat.h>
#include <business/langdefines.h>
#include <business/language.h>
#include <business/nationpositiondef.h>
#include <business/functiondefines.h>

#define PRINCE_ACC_ID_BEGIN  120
#define PRINCE_ACC_ID_END  150 



static bool comparedomainreward(CNationDomainCity p1, CNationDomainCity p2)
{
	return p1.m_nHour < p2.m_nHour;
}
static bool comparekillrank(CDBPrinceKillRank p1, CDBPrinceKillRank p2)
{
	if  (p1.m_nKillNum > p2.m_nKillNum)
		return true;

	if (p1.m_nKillNum < p2.m_nKillNum)
		return false;

	return false;
}

CWorldWarMgr::CWorldWarMgr()
{
	m_nSessionID = 1;
	m_ndbopenflag = 0;
	m_ndbgetlogflag = 0;
	m_ndbsatrapflag = 0;
	m_nKillTeam6Rate = 0;
	m_nAutoPickCd = 20;
	m_mapWarCityPtr.clear();
	m_mapGroupPtr.clear();
	m_nDeclareAccID = 160;
	m_nDeclareTime = 0;
	m_nProcTime = 0;

	m_nLastLibRcdSaveTime = 0;
	m_mapExploitLibRecord.clear();
	m_vecKillTeam6Reward.clear();
	m_vecKillTeam4Reward.clear();
	m_mapCityOccup.clear();
	m_mapNationAdimNum.clear();
	m_mapWeakNationBuff.clear();
	m_mapEnterPlayer.clear();
	m_vecKillPlayerReward.clear();
	m_mapItemRewardExp1.clear();
	m_mapItemRewardExp2.clear();
	m_nFightTime = 2 * 60 * 60;
	m_nPrinceAccFlag = 0;

	//盟战信息
	m_nAllyNation = 0;
	m_nSignNaitonNum = 0;              
	m_nTotNationNum = 0;        
	m_nLifeGuardNum = 0;       
	m_nLifeGuardLmt = 0;
	m_nWeakNationCityNum = 0;
	m_mapViceNum.clear();    
	m_mapAllyAcc.clear();
}

CWorldWarMgr::~CWorldWarMgr()
{
    this->onServerStop();
}

//导入静态数据 
void CWorldWarMgr::relolutionTable(bool bUpdate)
{
	if (NULL == g_pConsummer)
		return;

    
	//导入集团军的配置
	this->reloadWorldGroup(bUpdate);

	//导入排名奖励的配置
	this->reloadKillRankReward(bUpdate);
    
	//导入城战奖励配置
	this->reloadCityEndReward(bUpdate);
    
	//特殊时间的疆域产出奖励
	this->reloadSpeDomainReward(bUpdate);
    
	//弱国保护BUFF
	this->loadWeakNationBuff();
    
	//导入消耗道具经验奖励
	this->loadItemRewardExp(bUpdate);
    //导入城池管辖信息
	this->loadNationAdminInf();
	//获取单挑的CD时间
	m_nPickCDTime = atoi(g_pConsummer->findSysParamInfo(SYSPARAM_WORLD_WAR, 14).data());

	//国战的时间
	m_nFightTime = atoi(g_pConsummer->findSysParamInfo(SYSPARAM_WORLD_WAR, 8).data());

	//获取单挑的CD时间
	m_nInitFightTime = atoi(g_pConsummer->findSysParamInfo(SYSPARAM_WORLD_WAR, 9).data());
    //每回合需要冷却的CD时间 按万分比的配置 比如1000 为1秒
	m_nCountFightTime = atoi(g_pConsummer->findSysParamInfo(SYSPARAM_WORLD_WAR, 10).data());

	//驻兵消耗道具信息
	uint32 nNum = 0;
	std::string strParam = g_pConsummer->findSysParamInfo(SYSPARAM_WORLD_WAR, 5);
	sscanf(strParam.c_str(), "%d:%d", &m_nDeployedItem, &nNum);
	m_nDepoyedItemNum = static_cast<uint16>(nNum);

	//城战最大队伍数量
	m_nTotTeamLmt = atoi(g_pConsummer->findSysParamInfo(SYSPARAM_WORLD_WAR, 4).data());

	//返回城战信息的队伍数量
	m_nTeamListLmt = atoi(g_pConsummer->findSysParamInfo(SYSPARAM_WORLD_WAR, 6).data());
    
	//自动单挑的CD时间
	m_nAutoPickCd  = atoi(g_pConsummer->findSysParamInfo(SYSPARAM_WORLD_WAR, 21).data());
    
	//宣战后开战的准备时间
	m_nDeclareTime = atoi(g_pConsummer->findSysParamInfo(SYSPARAM_WORLD_WAR, 33).data());
	m_nDeclareTime = m_nDeclareTime * 60;

	//驻兵消耗道具信息
	m_nDecDeployItem = atoi(g_pConsummer->findSysParamInfo(SYSPARAM_WORLD_WAR, 24).data());

	//城战结束后的保护时间
	m_nProcTime = atoi(g_pConsummer->findSysParamInfo(SYSPARAM_WORLD_WAR, 25).data());
	m_nProcTime = m_nProcTime * 60;;

	//疆域产出的时间
	uint32 begintime = 0;
	uint32 endtime = 0;
	strParam = g_pConsummer->findSysParamInfo(SYSPARAM_WORLD_WAR, 29);
	sscanf(strParam.c_str(), "%d-%d", &begintime, &endtime);
	m_nDomainBeginTime = static_cast<uint16>(begintime);
	m_nDomainEndTime =  static_cast<uint16>(endtime);

	//国家宣战次数限制
	m_nDeclareNumLmt = (uint8)atoi(g_pConsummer->findSysParamInfo(SYSPARAM_WORLD_WAR, 23).c_str());
    
	//获取特殊攻击城池
	m_vecSpeCity.clear();
	m_nMaxSpeNpcRate = 0;
	g_pConsummer->getSpeNationCity(m_vecSpeCity);
	if (m_vecSpeCity.size() > 0)
	{
		size_t vecSixe = m_vecSpeCity.size() -1;
		CNationNpcRate cNpcRate = m_vecSpeCity[vecSixe];
		m_nMaxSpeNpcRate = cNpcRate.m_nEndRate;
	}

	//争夺太守失败CD
	strParam = g_pConsummer->findSysParamInfo(SYSPARAM_CITY_SATRAP, 2);
	this->m_nCityChangeCd = atoi(strParam.c_str());

	strParam = g_pConsummer->findSysParamInfo(SYSPARAM_CITY_SATRAP, 3);
	this->m_nSatrapBlowFightBuff = atoi(strParam.c_str());

	m_mapKillNumReward.clear();
	std::map<uint32, std::string> mapInfo;
	g_pConsummer->findSysParamInfo(SYSPARAM_KILL_NUM_REWARD, mapInfo);
	std::map<uint32, std::string>::iterator iter0 = mapInfo.begin();
	for (; iter0 != mapInfo.end(); ++iter0)
	{
		std::string strreward = iter0->second;
		uint16 nKillNum = (uint16)iter0->first;
		std::vector<CRewardInfo> vecAward;
		g_pConsummer->readRewardCfg(strreward, vecAward, ";");
		m_mapKillNumReward.insert(make_pair(nKillNum, vecAward));
	}
	return;
}

//导入盟国的数据
void CWorldWarMgr::reloadAllyData(bool bUpdate)
{
	//盟战条件
	uint32 nSignNum = 0;
	uint32 nTotNum = 0;
	std::string strParam = g_pConsummer->findSysParamInfo(SYSPRAM_WAR_ALLY, 2);
	sscanf(strParam.c_str(), "%d:%d", &nSignNum, &nTotNum);
	m_nSignNaitonNum = static_cast<uint8>(nSignNum);
	m_nTotNationNum =  static_cast<uint8>(nTotNum);
    
	//首次结盟日期
	m_nAllyDay = atoi(g_pConsummer->findSysParamInfo(SYSPRAM_WAR_ALLY, 1).data());
    
	//结盟周期
	m_nAllyCycle = atoi(g_pConsummer->findSysParamInfo(SYSPRAM_WAR_ALLY, 6).data());

	//每日奖励禁卫军数量
	m_nLifeGuardNum = (uint8)atoi(g_pConsummer->findSysParamInfo(SYSPRAM_WAR_ALLY, 3).data());

	//禁卫军限制
	m_nLifeGuardLmt = (uint8)atoi(g_pConsummer->findSysParamInfo(SYSPRAM_WAR_ALLY, 4).data());
    
	//弱国管辖城池的数量
	m_nWeakNationCityNum = (uint8)atoi(g_pConsummer->findSysParamInfo(SYSPRAM_WAR_ALLY, 7).data());

    //小号限制
	m_mapViceNum.clear();
	strParam = g_pConsummer->findSysParamInfo(SYSPRAM_WAR_ALLY, 5);
	uint32 nDay, nLevel;
	std::vector<std::string> vecSplitIndent;
	splitString(strParam, ";", vecSplitIndent);
	for(size_t i = 0; i < vecSplitIndent.size(); ++ i)
	{
		if( 2 != sscanf(vecSplitIndent[i].c_str(), "%d:%d", &nDay, &nLevel) )
			continue;	

		m_mapViceNum[nDay] = (uint8)nLevel;
	}

	return;
}

//导入弱国保护的BUFF
void CWorldWarMgr::loadWeakNationBuff()
{
	std::string strParam;
	std::vector<std::string> vecParam;

	//防守成功奖励
	strParam = g_pConsummer->findSysParamInfo(SYSPARAM_WORLD_WAR, 28);
	splitString(strParam, ";", vecParam, true);
	char sBuf[1024]={0};
	for(size_t i = 0; i < vecParam.size(); ++ i)
	{
		m_mapWeakNationBuff.clear();
		std::string stNationBuff = vecParam[i];
		char sBuff[1024]={0};
		char sNum[32]={0};
		std::vector<std::string> vecHourReward;

		memset(sBuf, 0, sizeof(sBuf));
		strcpy(sBuf, stNationBuff.data());

		GetStrItem(sNum, sBuf, ':', 0); 
		uint8 nNum = (uint8)atoi(sNum);

		GetStrItem(sBuff, sBuf, ':', 1);
		strParam = std::string(sBuff);
		std::vector<std::string> vecBuff;
		splitString(strParam, ",", vecBuff, true);
		for(size_t j = 0; j < vecBuff.size(); ++j)
		{
			uint32 idBuff = atoi(vecBuff[j].data());
			m_mapWeakNationBuff[nNum].push_back(idBuff);
		}
	}

	return;
}

//登记城池被占领情况
void CWorldWarMgr::loadNationAdminInf()
{
	if (m_mapNationAdimNum.size() > 0)
		return;

	//国家的初始管辖城池数量
	uint32 nNum = atoi(g_pConsummer->findSysParamInfo(SYSPARAM_WORLD_WAR, 35).c_str());
	m_mapNationAdimNum[NATIONAL_WEI] = nNum;
	m_mapNationAdimNum[NATIONAL_SHU] = nNum;
	m_mapNationAdimNum[NATIONAL_WU] = nNum;

	return;
}

//导入城战结束奖励的配置
void CWorldWarMgr::reloadCityEndReward(bool bUpdate)
{
	std::string strParam;
	std::vector<std::string> vecParam;
	uint32 nType = 0;
	uint32 nValue = 0;
	uint32 nNum = 0;
	uint32 nRate = 0;
    
	//防守成功奖励
	strParam = g_pConsummer->findSysParamInfo(SYSPARAM_WORLD_WAR, 11);
	splitString(strParam, ";", vecParam, true);
	CRewardInfo cReward;
	for(size_t i = 0; i < vecParam.size(); ++ i)
	{
		strParam = vecParam[i];
		if (sscanf(strParam.c_str(), "%d:%d:%d", &nType, &nValue,&nNum) != 3)
			continue;

		cReward.m_nType = (uint8)nType;
		cReward.m_nValue = (uint32)nValue;
		cReward.m_nNum = (uint32)nNum;
		m_vecDefSuccReward.push_back(cReward);
	}

	vecParam.clear();

	//防守失败奖励
	strParam = g_pConsummer->findSysParamInfo(SYSPARAM_WORLD_WAR, 12);
	splitString(strParam, ";", vecParam, true);
	for(size_t i = 0; i < vecParam.size(); ++ i)
	{
		
		strParam = vecParam[i];
		if (sscanf(strParam.c_str(), "%d:%d:%d", &nType, &nValue,&nNum) != 3)
			continue;

		cReward.m_nType = (uint8)nType;
		cReward.m_nValue = (uint32)nValue;
		cReward.m_nNum = (uint32)nNum;
		m_vecDefFailRewad.push_back(cReward);
	}

	//击杀部队类型6的奖励
	strParam = g_pConsummer->findSysParamInfo(SYSPARAM_WORLD_WAR, 19);
	if (sscanf(strParam.c_str(), "%d:%d:%d:%d", &nType, &nValue,&nNum, &nRate) == 4)
	{
		cReward.m_nType = (uint8)nType;
		cReward.m_nValue = (uint32)nValue;
		cReward.m_nNum = (uint32)nNum;
		m_nKillTeam6Rate = (uint32)nRate;
		m_vecKillTeam6Reward.push_back(cReward);
	}

	//击杀部队类型5的奖励
	m_vecKillTeam5Reward.clear();
	strParam = g_pConsummer->findSysParamInfo(SYSPARAM_WORLD_WAR, 38);
	if (sscanf(strParam.c_str(), "%d:%d:%d:%d", &nType, &nValue,&nNum, &nRate) == 4)
	{
		cReward.m_nType = (uint8)nType;
		cReward.m_nValue = (uint32)nValue;
		cReward.m_nNum = (uint32)nNum;
		m_vecKillTeam5Reward.push_back(cReward);
	}

	//击杀部队类型4的奖励
	strParam = g_pConsummer->findSysParamInfo(SYSPARAM_WORLD_WAR, 20);
	if (sscanf(strParam.c_str(), "%d:%d:%d:%d", &nType, &nValue,&nNum, &nRate) == 4)
	{
		cReward.m_nType = (uint8)nType;
		cReward.m_nValue = (uint32)nValue;
		cReward.m_nNum = (uint32)nNum;
		m_nKillTeam4Rate = (uint32)nRate;
		m_vecKillTeam4Reward.push_back(cReward);
	}

	//进攻成功奖励
	m_vecAttSuccRewad.clear();
	strParam = g_pConsummer->findSysParamInfo(SYSPARAM_WORLD_WAR, 31);
	splitString(strParam, ";", vecParam, true);
	for(size_t i = 0; i < vecParam.size(); ++ i)
	{

		strParam = vecParam[i];
		if (sscanf(strParam.c_str(), "%d:%d:%d", &nType, &nValue,&nNum) != 3)
			continue;

		cReward.m_nType = (uint8)nType;
		cReward.m_nValue = (uint32)nValue;
		cReward.m_nNum = (uint32)nNum;
		m_vecAttSuccRewad.push_back(cReward);
	}

	//进攻失败奖励
	m_vecAttFailReward.clear();
	strParam = g_pConsummer->findSysParamInfo(SYSPARAM_WORLD_WAR, 32);
	splitString(strParam, ";", vecParam, true);
	for(size_t i = 0; i < vecParam.size(); ++ i)
	{

		strParam = vecParam[i];
		if (sscanf(strParam.c_str(), "%d:%d:%d", &nType, &nValue,&nNum) != 3)
			continue;

		cReward.m_nType = (uint8)nType;
		cReward.m_nValue = (uint32)nValue;
		cReward.m_nNum = (uint32)nNum;
		m_vecAttFailReward.push_back(cReward);
	}

	//击杀玩家的概率奖励
	strParam = g_pConsummer->findSysParamInfo(SYSPARAM_WORLD_WAR, 26);
	if (sscanf(strParam.c_str(), "%d:%d:%d:%d", &nType, &nValue,&nNum, &nRate) == 4)
	{
		cReward.m_nType = (uint8)nType;
		cReward.m_nValue = (uint32)nValue;
		cReward.m_nNum = (uint32)nNum;
		m_nKillPlayerRewardRate = (uint32)nRate;
		m_vecKillPlayerReward.push_back(cReward);
	}

	return;
}


//特定时间产出
void CWorldWarMgr::reloadSpeDomainReward(bool bUpdate)
{
	m_mapSpeTimeReward.clear();
	std::string strParam;
	std::vector<std::string> vecParam;
	strParam = g_pConsummer->findSysParamInfo(SYSPARAM_WORLD_WAR, 30);
	splitString(strParam, ";", vecParam, true);
	char sBuf[1024]={0};
	for(size_t i = 0; i < vecParam.size(); ++ i)
	{
		std::string strHourReward = vecParam[i];
		char sReward[1024]={0};
		char sHour[32]={0};
		std::vector<std::string> vecHourReward;

		memset(sBuf, 0, sizeof(sBuf));
		strcpy(sBuf, strHourReward.data());

		GetStrItem(sHour, sBuf, '-', 0);
		uint16 nHour = atoi(sHour);
		GetStrItem(sReward, sBuf, '-', 1);

		strHourReward = std::string(sReward);
        
		std::vector<CRewardInfo> vecReward;
		g_pConsummer->readRewardCfg(strHourReward, vecReward, ",");
		if (vecReward.size() > 0)
			m_mapSpeTimeReward.insert(make_pair(nHour, vecReward));
	}

	return;
}

//导入集团军的配置
void CWorldWarMgr::reloadWorldGroup(bool bUpdate)
{
	std::string strParam;
	std::vector<std::string> vecParam;

	CTableDataPtr ptabledata = g_pConsummer->getStaticData("tb_table_world_group", bUpdate);
	if (ptabledata != NULL)
	{
		for (Json::Value::iterator iter=ptabledata->m_Value.begin(); iter!=ptabledata->m_Value.end(); ++iter)
		{
			Json::Value& tmpvalue = (*iter);	
			CWarGroupPtr pWarGroupPtr = new CWarGroup();
			uint32 idGroup = tmpvalue["group_id"].asInt();
			uint8 worldlevel = tmpvalue["world_level"].asInt();

			pWarGroupPtr->m_nGroupID = idGroup;
			pWarGroupPtr->m_nWorldLevel = worldlevel;
			OBJID idKey = (OBJID)idGroup;
			idKey = idGroup * 1000 + worldlevel;
			pWarGroupPtr->m_nTotArms = tmpvalue["tot_arm"].asInt();
			std::string strArmInf = tmpvalue["arm_inf"].asString();
			vecParam.clear();
			splitString(strArmInf, ",", vecParam, true);
			for (size_t i = 0; i < vecParam.size(); ++i)
			{
				strParam = vecParam[i];
				uint32 idNpcTeam = atoi(strParam.data());
				pWarGroupPtr->m_vecArms.push_back(idNpcTeam);
			}


			m_mapGroupPtr.insert(make_pair(idKey, pWarGroupPtr));
		}
	}

	return;
}

//导入道具奖励经验表
void CWorldWarMgr::loadItemRewardExp(bool bUpdate)
{
	std::string strParam;
	std::vector<std::string> vecParam;

	CTableDataPtr ptabledata = g_pConsummer->getStaticData("tb_table_item_reward_exp", bUpdate);
	if (ptabledata != NULL)
	{
		m_mapItemRewardExp1.clear();
		m_mapItemRewardExp2.clear();
		for (Json::Value::iterator iter=ptabledata->m_Value.begin(); iter!=ptabledata->m_Value.end(); ++iter)
		{
			Json::Value& tmpvalue = (*iter);	
			uint8 nItemType = tmpvalue["item_type"].asUInt();
			if (nItemType != 1 && nItemType != 2)
				continue;

			uint8 nLevel = tmpvalue["lvl"].asUInt();
			uint32 nExp =  tmpvalue["exp"].asUInt();
			if (nItemType == 1)
			    m_mapItemRewardExp1[nLevel]  = nExp;
			else
                m_mapItemRewardExp2[nLevel]  = nExp;
		}
	}

	return;
}

//导入排名奖励的配置
void CWorldWarMgr::reloadKillRankReward(bool bUpdate)
{
	std::string strParam;
	std::vector<std::string> vecParam;

	CTableDataPtr ptabledata = g_pConsummer->getStaticData("tb_table_city_rank_reward", bUpdate);
	if (ptabledata != NULL)
	{
		m_vecRankReward.clear();
		m_vecRankReward1.clear();
		for (Json::Value::iterator iter=ptabledata->m_Value.begin(); iter!=ptabledata->m_Value.end(); ++iter)
		{
			Json::Value& tmpvalue = (*iter);	
			CCityRankReward cRankReward;
			cRankReward.m_nBeginRank = tmpvalue["begin_rank"].asUInt();
			cRankReward.m_nEndRank = tmpvalue["end_rank"].asUInt();
			strParam = tmpvalue["rank_reward"].asString();
			uint8 nRankType = tmpvalue["rank_type"].asUInt();
			splitString(strParam, ";", vecParam, true);

			CRewardInfo cReward;
			for(size_t i = 0; i < vecParam.size(); ++ i)
			{
				uint32 nType = 0;
				uint32 nValue = 0;
				uint32 nNum = 0;
				strParam = vecParam[i];
				if (sscanf(strParam.c_str(), "%d:%d:%d", &nType, &nValue,&nNum) != 3)
					continue;

				cReward.m_nType = (uint8)nType;
				cReward.m_nValue = (uint32)nValue;
				cReward.m_nNum = (uint32)nNum;
				cRankReward.m_vecReward.push_back(cReward);
			}
            if (nRankType == 1)
			    m_vecRankReward.push_back(cRankReward);
			else if (nRankType == 2)
				m_vecRankReward1.push_back(cRankReward);
			else
				m_vecPrinceRankReward.push_back(cRankReward);
		}
	}

	return;
}

//国王宣战
IResponsePtr CWorldWarMgr::executeGWMDeclareWar(IRequestPtr Request, CCurrentPtr Current)
{
	CGWMDeclareWarRequestPtr prequest = new CGWMDeclareWarRequest(Request->getSerialStream());
	CGWMDeclareWarResponsePtr presponse = new CGWMDeclareWarResponse();

	uint8 idCity = prequest->m_nCityID;
	uint8 idNation = prequest->m_nNationID;

	//获取城池信息
	CWorldCityPtr cWorldCityPtr  = g_pConsummer->getWorldCityPtr(prequest->m_nCityID);
	if (cWorldCityPtr == NULL)
	{
		presponse->m_nRetCode = NO_ACC_CITY;
		return presponse.get();
	}

	//判断城池是否在事件中
	CWarCityPtr pWarCityPtr = this->getWarCityPtr(idCity);
	if (pWarCityPtr != NULL)
	{
		presponse->m_nRetCode = HAVE_ACC_CITY;
		return presponse.get();
	}

	//判断是否在保护城池中
	CProjectCityPtr pProcCityPtr = this->getProCityPtr(idCity);
	if (pProcCityPtr != NULL)
	{
		presponse->m_nRetCode = CITY_IN_PROJECT_TIME;
		return presponse.get();
	}

	//判断城池是否是相邻管辖城池
	if (!this->checkConnectAdminCity(idNation, idCity))
	{
		presponse->m_nRetCode = HAVE_ACC_CITY;
		return presponse.get();
	}


	//判断是否是王城
	if (cWorldCityPtr->m_nCityType == CITY_TYPE_CAPITAL)
	{
		presponse->m_nRetCode = HAVE_ACC_CITY;
		return presponse.get();
	}

	//获取国家信息
	CWarNationPtr pWarNationPtr = this->getWarNationPtr(idNation);
	if (pWarNationPtr == NULL)
	{
		presponse->m_nRetCode = HAVE_ACC_CITY;
		return presponse.get();
	}
#ifdef WIN32
	prequest->m_nType = 1;
#else
	//判断是否还有宣战次数
	if (pWarNationPtr->getDecNum() >= m_nDeclareNumLmt)
	{
		presponse->m_nRetCode = DECLARE_NUM_LMT;
		return presponse.get();
	}
#endif

	//判断国家的管辖数量
	uint8 nAdminNum = this->getNaitonAdminCityNum(idNation);
	if (nAdminNum >= g_pConsummer->getNationCityLmt(idNation))
	{
		presponse->m_nRetCode = NATION_ADMIN_CITY_LMT;
		return presponse.get();
	}
    
	//加上事件中的城池
	std::map<uint8, CWarCityPtr>::iterator iter_acc = m_mapWarCityPtr.begin();
	for (; iter_acc != m_mapWarCityPtr.end(); ++iter_acc)
	{
		CWarCityPtr pWarCityPtr = iter_acc->second;
		if (pWarCityPtr == NULL)
			continue;
		if (pWarCityPtr->getAttNation() == idNation)
			nAdminNum++;
	}
    
	//防止事件中的城池成功后，国家管辖的城池超过限制
	if (nAdminNum >= g_pConsummer->getNationCityLmt(idNation))
	{
		presponse->m_nRetCode = NATION_ADMIN_CITY_LMT;
		return presponse.get();
	}


	//生成事件信息
    uint8 idAcc = this->getDeclareAccID();

	//事件初始化
	pWarCityPtr = new CWarCity();

	CDateTime now;
	uint32 begintime = now.getTotalSecs();
    
	uint8 idAdminNation = cWorldCityPtr->m_nNation;
	this->getCityOccupNation(idCity, idAdminNation);
   
	uint32 nBeginTime = begintime + m_nDeclareTime;

	if (prequest->m_nType != 0)
		nBeginTime = begintime + 60;

	uint32 endTime = nBeginTime + m_nFightTime;

	pWarCityPtr->init(idAcc, idCity, nBeginTime);
	pWarCityPtr->setEndTime(endTime);
	pWarCityPtr->setAccType(1);
	pWarCityPtr->setCityName(cWorldCityPtr->m_strName);

	pWarCityPtr->setAttSuccReward(m_vecAttSuccRewad);
	pWarCityPtr->setAttFailReward(m_vecAttFailReward);
	pWarCityPtr->setConsummer(g_pConsummer);
	pWarCityPtr->setDepoyedItem(m_nDecDeployItem);
	pWarCityPtr->setDepoyedItemNum(1);
	pWarCityPtr->setTeamListLmt(m_nTeamListLmt);
	pWarCityPtr->setNation(idAdminNation);
	pWarCityPtr->setAttNation(idNation);
	pWarCityPtr->setItemRewardExp(m_mapItemRewardExp2);

	//初始化队伍信息
	this->initCityDefTeam(pWarCityPtr);

	m_mapWarCityPtr.insert(make_pair(idCity, pWarCityPtr));

	//更改宣战次数
	pWarNationPtr->setDenNum(pWarNationPtr->getDecNum()+1);

	//更改数据库
	this->updWarNation(pWarNationPtr, WAR_NATION_DEC_NUM);

	//发送公告
	std::string strAttNation = g_pConsummer->formatLangString(LANGTYPE_NATION_NAME, idNation);
	std::string strAdminNation = g_pConsummer->formatLangString(LANGTYPE_NATION_NAME, idAdminNation);
	std::string strNotice = g_pConsummer->formatLangString(LANGTYPE_WORLD_WAR, LANGTYPE_WORLD_WAR_DECLARE_WAR, 
		                          strAttNation.data(), strAdminNation.data());

	CBytesBufferPtr buf = CBytesBufferPtr::createInstance();
	pushChatCityInfo(buf, cWorldCityPtr->m_nCityID);

	sendSystemNotice(CHANNEL_MARQUEE, strNotice, buf, 26);
	sendSystemNotice(CHANNEL_WORLD, strNotice, buf, 26);
   
	//宣战信息通告
	this->decWarNotice(pWarCityPtr);

	presponse->m_nNaitonID = idNation;
	presponse->m_nDeclareTime = pWarCityPtr->getBeginTime();
	presponse->m_nWarID = pWarCityPtr->getAccID(); 
	return presponse.get();
}

//宣战信息通知
void CWorldWarMgr::decWarNotice(CWarCityPtr pWarCityPtr)
{
	if (pWarCityPtr == NULL)
		return;

	uint8 idCity = pWarCityPtr->getCityID();
	uint8 idAttNation = pWarCityPtr->getAttNation();
    
	//通知gateway 城池开战通知
	CGWWorldWarNoticeRequestPtr gwreq = new CGWWorldWarNoticeRequest();
	gwreq->m_nType = WORLD_WAR_PRE_FIGHT;
	gwreq->m_nAccID = pWarCityPtr->getAccID();
	gwreq->m_nCityID = pWarCityPtr->getCityID();
	char sBuf[128] = {0};
	sprintf(sBuf, "%d:%d:%d", idCity, idAttNation, pWarCityPtr->getBeginTime());
	gwreq->m_strData = std::string(sBuf);

	m_vecGateWaySession.clear();
	g_pConsummer->getGateWaySession(m_vecGateWaySession);
	for( size_t i = 0; i < m_vecGateWaySession.size();i++ )
		g_DaServer->send(m_vecGateWaySession[i], GW_WORLD_WAR_NOTICE, gwreq.get());

	//宣战次数通知
	CWarNationPtr cNationPtr = this->getWarNationPtr(idAttNation);
	if (cNationPtr == NULL )
	    return;

	std::map<OBJID, CWarJoinPlayerPtr>::iterator iter = m_mapWarJoinPlayer.begin();
	for (; iter != m_mapWarJoinPlayer.end(); ++iter)
	{
		OBJID idPlayer = iter->first;
		CWarJoinPlayerPtr pWarPlayer = iter->second;
		if (pWarPlayer == NULL)
			continue;

		if (pWarPlayer->m_nNation != idAttNation)
			continue;

		this->playerWarNotice(idPlayer, WORLD_DECLARE_NUM, cNationPtr->getDecNum(), "");
	}

	return;
}

//更新国家国战信息
void CWorldWarMgr::updWarNation(CWarNationPtr pWarNationPtr, uint8 nKey)
{
	CDBUpdWarNationRequestPtr dbreq = new CDBUpdWarNationRequest();
	if (pWarNationPtr == NULL)
	{
		dbreq->m_nNation = 0;
	}
	else
	{
		dbreq->m_nNation = pWarNationPtr->getNation();
		switch (nKey)
		{
		case WAR_NATION_DEC_NUM:
			{
				CUpdValues cValues;
				cValues.m_nKey = nKey;
				cValues.m_nValues = pWarNationPtr->getDecNum();
				dbreq->m_vecInf.push_back(cValues);
			}
			break;
		default:
			break;
		}
	}

	if (dbreq->m_nNation != 0 && dbreq->m_vecInf.size() == 0)
		return;

	g_DaServer->callServant(DB_UPD_WAR_NATION, dbreq.get(), NULL, 0, NULL, NULL);	 
	return;
}


//进入国战
IResponsePtr CWorldWarMgr::executeGWMEnterWorldWar(IRequestPtr Request, CCurrentPtr Current)
{
	CGWMEnterWorldWarRequestPtr prequest = new CGWMEnterWorldWarRequest(Request->getSerialStream());
	CGWMEnterWorldWarResponsePtr presponse = new CGWMEnterWorldWarResponse();

	OBJID idPlayer = prequest->m_nPlayerID;

	//获取玩家国战信息
	CWarJoinPlayerPtr pWarJoinPlayerPtr = this->getWarJoinPlayer(idPlayer);
	if (pWarJoinPlayerPtr == NULL)
	{
		pWarJoinPlayerPtr = new CWarJoinPlayer();
		pWarJoinPlayerPtr->m_nPlayerID = idPlayer;
		pWarJoinPlayerPtr->m_nBattEffect = prequest->m_nBattEffect;
		pWarJoinPlayerPtr->m_nCityID = prequest->m_nCityID;
		pWarJoinPlayerPtr->m_nGoverPosts = prequest->m_nGoverPosts;
		pWarJoinPlayerPtr->m_nLevel = prequest->m_nLevel;
		pWarJoinPlayerPtr->m_nNation = prequest->m_nNation;
		pWarJoinPlayerPtr->m_strName = prequest->m_strName;
		m_mapWarJoinPlayer.insert(make_pair(idPlayer, pWarJoinPlayerPtr));
	}
	else
	{
		//更新玩家国战信息
		pWarJoinPlayerPtr->m_nLevel = prequest->m_nLevel;
		pWarJoinPlayerPtr->m_nBattEffect = prequest->m_nBattEffect;
		pWarJoinPlayerPtr->m_nGoverPosts = prequest->m_nGoverPosts;
		pWarJoinPlayerPtr->m_nCityID = prequest->m_nCityID;
	}

	//获取事件信息
	CDateTime now;
	CCityAccident cCityAcc;
	CDeclareWar cDeclareWar;
	std::map<uint8, CWarCityPtr>::iterator iter = m_mapWarCityPtr.begin();
	for (; iter != m_mapWarCityPtr.end(); ++iter)
	{
		CWarCityPtr pWarCityPtr = iter->second;
		if (pWarCityPtr == NULL)
			continue;

		//战火中的城池
		if (pWarCityPtr->getStatus()  ==  1)
			presponse->m_vecCity.push_back(pWarCityPtr->getCityID());

		if (pWarCityPtr->getAccType() == ACC_TYPE_NPC)
		{
			cCityAcc.m_nAccID = pWarCityPtr->getAccID();
			cCityAcc.m_nCityID = pWarCityPtr->getCityID();
			presponse->m_vecAccident.push_back(cCityAcc);
		}
		else
		{
			cDeclareWar.m_nCityID = pWarCityPtr->getCityID();
			cDeclareWar.m_nDeclareTime = pWarCityPtr->getBeginTime();
			cDeclareWar.m_nNationID = pWarCityPtr->getAttNation();
			cDeclareWar.m_nWarID = pWarCityPtr->getAccID();
			presponse->m_vecDeclareWar.push_back(cDeclareWar);
		}
	}
    
	//获取保护城池的信息
	CProjectCity cProcCity;
	std::map<uint8, CProjectCityPtr>::iterator iter_pro = m_mapProCity.begin();
	for (; iter_pro != m_mapProCity.end(); ++iter_pro)
	{
		CProjectCityPtr pCitPtr = iter_pro->second;
		if (pCitPtr == NULL)
			continue;

		cProcCity.m_nCityID = pCitPtr->m_nCityID;
		cProcCity.m_nProcTime = pCitPtr->m_nProcTime;
		presponse->m_vecProcCity.push_back(cProcCity);
	}
    
	presponse->m_nRetCode = SUCCESS;

	m_mapEnterPlayer[idPlayer] = 0;

	uint8 nationId = 0; 
	std::map<uint8, std::vector<CExploitLibInfo> > ::iterator itELI = m_mapExploitLibRecord.find(nationId);
	if( itELI != m_mapExploitLibRecord.end() )
		presponse->m_vecExploitLibRecord.assign(itELI->second.begin(), itELI->second.end());
	std::map<uint8, uint8>::iterator iter_occup =  m_mapCityOccup.begin();
	CCityOccup cCityOccup;
	for (; iter_occup != m_mapCityOccup.end(); ++iter_occup)
	{
		cCityOccup.m_nCityID = iter_occup->first;
		cCityOccup.m_nNationID = iter_occup->second;
		presponse->m_vecOccup.push_back(cCityOccup);
	}
    
	//获取玩家驻兵的事件(npc)
	this->getPlayerDeployAcc(pWarJoinPlayerPtr, presponse->m_vecDeployedAcc);

    Current->sendResponse(presponse.get());

	CWarNationPtr cNationPtr = this->getWarNationPtr(pWarJoinPlayerPtr->m_nNation);
	if (cNationPtr != NULL )
		this->playerWarNotice(idPlayer, WORLD_DECLARE_NUM, cNationPtr->getDecNum(), "");

	//判断通知有疆域产出奖励
	this->checkFixTimeReward(pWarJoinPlayerPtr, prequest->m_nFixRewardTime);

	//校验返回太守城池
	this->checkReturnSatrapCity(idPlayer);

	return NULL;
}

//获取玩家驻兵的事件
void CWorldWarMgr::getPlayerDeployAcc(CWarJoinPlayerPtr pWarJoinPlayerPtr, std::vector<uint8> &vecDeployedAcc)
{
	if (pWarJoinPlayerPtr == NULL)
		return;

	std::map<uint8, uint8> mapAcc;
	OBJID idPlayer = pWarJoinPlayerPtr->m_nPlayerID;

	std::map<uint8, CWarCityPtr>::iterator iter =  m_mapWarCityPtr.begin();
	for (; iter != m_mapWarCityPtr.end(); ++iter)
	{
		CWarCityPtr pWarCityPtr = iter->second;
		if (pWarCityPtr == NULL)
			continue;

		uint8 idAcc = pWarCityPtr->getAccID();
		CPlayerCityWarInfPtr pPlayerWarPtr = pWarCityPtr->getJoinPlayer(idPlayer);
		if (pPlayerWarPtr == NULL)
			continue;


		if (pPlayerWarPtr->m_nDeployNum > 0 || pPlayerWarPtr->m_vecTeamInf.size() > 0 )
		     vecDeployedAcc.push_back(idAcc);
	}

	return;
}

//校验是否有定时的奖励
void CWorldWarMgr::checkReturnSatrapCity(OBJID idPlayer)
{

	std::map<OBJID, uint8>::iterator iter =  m_mapPlayerSatrapCity.find(idPlayer);
	if (iter == m_mapPlayerSatrapCity.end())
		return;

	this->playerWarNotice(idPlayer, WORLD_SATRAP_CITY, iter->second, "");
}

//校验是否有定时的奖励
void CWorldWarMgr::checkFixTimeReward(CWarJoinPlayerPtr pWarPlayerPtr, uint16 nFixTime)
{
	if (pWarPlayerPtr == NULL)
		return;

	CDateTime now;
	uint16 nHour = (uint16)now.getHour();

	if (nHour < m_nDomainBeginTime)
		return;

	uint8 idNation = pWarPlayerPtr->m_nNation;
	CWarNationPtr  pWarNationPtr = this->getWarNationPtr(idNation);
	if (pWarNationPtr == NULL)
		return;

	for(size_t i = 0; i < pWarNationPtr->m_vecDomainReward.size(); i++)
	{
		CDomainReward &cReward = pWarNationPtr->m_vecDomainReward[i];
		if (cReward.m_nTime > nFixTime)
		{
			this->playerWarNotice(pWarPlayerPtr->m_nPlayerID, WORLD_DOMAIN_REWARD_NOTICE, 0, "");
			return;
		}
	}
    
	return;
}

//进入城战城池
IResponsePtr CWorldWarMgr::executeGWMEnterFightCity(IRequestPtr Request, CCurrentPtr Current)
{
	CGWMEnterFightCityRequestPtr prequest = new CGWMEnterFightCityRequest(Request->getSerialStream());
	CGWMEnterFightCityResponsePtr presponse = new CGWMEnterFightCityResponse();

	uint8 idCity = prequest->m_nCityID;
	OBJID idPlayer = prequest->m_nPlayerID;

	//获取城战事件信息
	CWarCityPtr pWarCityPtr = this->getWarCityPtr(idCity);
	if (pWarCityPtr == NULL)
	{
		presponse->m_nRetCode = NO_ACC_CITY;
		return presponse.get();
	}

	//获取玩家在事件中的信息
	CPlayerCityWarInfPtr pPlayerCityWarPtr =  pWarCityPtr->getJoinPlayer(idPlayer);
	if (pPlayerCityWarPtr != NULL)
		presponse->m_nFightDirt = pPlayerCityWarPtr->m_nDir;
    
	pWarCityPtr->addJoinPlayer(idPlayer);
	presponse->m_nAttackNum = pWarCityPtr->getAttTeamNum();
	presponse->m_nBlowNum = pWarCityPtr->getBlowTeamNum();
	presponse->m_nQueueStatus = pWarCityPtr->getQueueStatus();
	presponse->m_nQueueFail = pWarCityPtr->getQueueFail();
	pWarCityPtr->getFightTeam(presponse->m_vecFightTeam);
	pWarCityPtr->getFightSReport(presponse->m_vecBatReport);
	CDateTime now;
	for (size_t i = 0; i < presponse->m_vecBatReport.size(); i++)
	{
		CTeamBatReport &cBatReport = presponse->m_vecBatReport[i];
		if (cBatReport.m_nNextTime >= now.getTotalSecs())
			cBatReport.m_nNextTime = cBatReport.m_nNextTime -  now.getTotalSecs();
		else
			cBatReport.m_nNextTime = 0;
	}

    presponse->m_nRetCode = SUCCESS;
	return presponse.get();
}

//获取城战队伍数量
IResponsePtr CWorldWarMgr::executeGWMGetCityTeamNum(IRequestPtr Request, CCurrentPtr Current)
{
	CGWMGetCityTeamNumRequestPtr prequest = new CGWMGetCityTeamNumRequest(Request->getSerialStream());
	CGWMGetCityTeamNumResponsePtr presponse = new CGWMGetCityTeamNumResponse();

	uint8 idCity = prequest->m_nCityID;

	//获取城战事件信息
	CWarCityPtr pWarCityPtr = this->getWarCityPtr(idCity);
	if (pWarCityPtr == NULL)
	{
		presponse->m_nRetCode = NO_ACC_CITY;
		return presponse.get();
	}

	presponse->m_nAttackNum = pWarCityPtr->getAttTeamNum();
	presponse->m_nBlowNum = pWarCityPtr->getBlowTeamNum();

	presponse->m_nRetCode = SUCCESS;
	return presponse.get();
}

//选择作战阵营
IResponsePtr CWorldWarMgr::executeSelectFightDirect(IRequestPtr Request, CCurrentPtr Current)
{
	CGWMSelectFightDirectRequestPtr prequest = new CGWMSelectFightDirectRequest(Request->getSerialStream());
	CGWMSelectFightDirectResponsePtr presponse = new CGWMSelectFightDirectResponse();

	uint8 idCity = prequest->m_nCityID;

	//获取城战事件信息
	CWarCityPtr pWarCityPtr = this->getWarCityPtr(idCity);
	if (pWarCityPtr == NULL)
	{
		presponse->m_nRetCode = NO_ACC_CITY;
		return presponse.get();
	}

	if (pWarCityPtr->getAttNation() == prequest->m_nNation)
	{
		log_info("att nation= %d player nation %d m_nDirect %d ", pWarCityPtr->getAttNation(), prequest->m_nNation, prequest->m_nDirect);
		if (prequest->m_nDirect != WAR_DIR_TYPE_ATT)
		{
			presponse->m_nRetCode = SELECT_DIR_ATT_INVLAD;
			return presponse.get();
		}
	}
	if (pWarCityPtr->getNation() == prequest->m_nNation)
	{
		log_info("def nation= %d player nation %d m_nDirect %d ", pWarCityPtr->getNation(), prequest->m_nNation, prequest->m_nDirect);
		if (prequest->m_nDirect != WAR_DIR_TYPE_BLOW)
		{
			presponse->m_nRetCode = SELECT_DIR_DEF_INVLAD;
			return presponse.get();
		}
	}
	OBJID idPlayer = prequest->m_nPlayerID;

	//判断玩家是否之前选择过
	CPlayerCityWarInfPtr pPlayerCityWarPtr =  pWarCityPtr->getJoinPlayer(idPlayer);
	if (pPlayerCityWarPtr != NULL)
	{
		if (pPlayerCityWarPtr->m_nDir != WAR_DIR_TYPE_NO)
		{
			presponse->m_nRetCode = HAVE_SELECT_DIR;
			return presponse.get();
		}

		pPlayerCityWarPtr->m_nSex = prequest->m_nSex;
		pPlayerCityWarPtr->m_strName = prequest->m_strName;
		pPlayerCityWarPtr->m_nLevel = prequest->m_nLevel;
		pPlayerCityWarPtr->m_nNation = prequest->m_nNation;
		pPlayerCityWarPtr->m_nBattEffect = prequest->m_nBattEffect;
		pPlayerCityWarPtr->m_nDir = prequest->m_nDirect;
	}
	else
	{
		pPlayerCityWarPtr = new CPlayerCityWarInf();
		pPlayerCityWarPtr->m_nKillNum = 0;
		pPlayerCityWarPtr->m_nCityID = idCity;
		pPlayerCityWarPtr->m_nDir = prequest->m_nDirect;
		pPlayerCityWarPtr->m_nSex = prequest->m_nSex;
		pPlayerCityWarPtr->m_strName = prequest->m_strName;
		pPlayerCityWarPtr->m_nLevel = prequest->m_nLevel;
		pPlayerCityWarPtr->m_nNation = prequest->m_nNation;
		pPlayerCityWarPtr->m_nBattEffect = prequest->m_nBattEffect;
		pWarCityPtr->addPlayerCityInf(idPlayer, pPlayerCityWarPtr);
	}

	presponse->m_nRetCode = SUCCESS;
	presponse->m_nDirect = prequest->m_nDirect;
	return presponse.get();
}

//兵战驻兵
IResponsePtr CWorldWarMgr::executeEtapeDeployed(IRequestPtr Request,CCurrentPtr Current)
{
	CGWMEtapeDeployedRequestPtr prequest = new CGWMEtapeDeployedRequest(Request->getSerialStream());
	CGWMEtapeDeployedResponsePtr presponse = new CGWMEtapeDeployedResponse();

	CDeployTeamInf &cDeployTeam = prequest->m_cDeployTeam;
   
	//获取城战国家信息
	CWarNationPtr pWarNationPtr = this->getWarNationPtr(cDeployTeam.m_nNationID);
	if (pWarNationPtr == NULL)
	{
		pWarNationPtr = new CWarNation();
		pWarNationPtr->setNation(cDeployTeam.m_nNationID);
		m_mapWarNation.insert(make_pair(cDeployTeam.m_nNationID, pWarNationPtr));
	}

	//获取玩家的驻兵信息
	CPlayerCityWarInfPtr pPlayerWarPtr = pWarNationPtr->getJoinPlayer(cDeployTeam.m_nPlayerID);
	if (pPlayerWarPtr == NULL)
	{
		pPlayerWarPtr = new CPlayerCityWarInf();
		pPlayerWarPtr->m_nLevel = cDeployTeam.m_nLevel;
		pPlayerWarPtr->m_nNation = cDeployTeam.m_nNationID;
		pPlayerWarPtr->m_nSex = cDeployTeam.m_nSex;
		pPlayerWarPtr->m_strName = cDeployTeam.m_strName;
		pPlayerWarPtr->m_nBattEffect = cDeployTeam.m_nBattEffect;

        pWarNationPtr->addPlayerCityInf(cDeployTeam.m_nPlayerID, pPlayerWarPtr);
	}

	for (size_t i = 0; i < prequest->m_nTeamNum; i++)
	{
		//组装玩家队伍信息
		CPlayerTeamInf cTeamInf;
		cTeamInf.m_nTeamID = pWarNationPtr->getTeamID();
		cTeamInf.m_nDragonLevel = cDeployTeam.m_nDragonLevel;
		cTeamInf.m_nFormationID = cDeployTeam.m_nFormationID;
		cTeamInf.m_nVisionFlag = 1; //幻影标志
		cTeamInf.m_vecUint.assign(cDeployTeam.m_vecUint.begin(), cDeployTeam.m_vecUint.end());
		cTeamInf.m_vecDragonSKill.assign(cDeployTeam.m_vecDragonSKill.begin(), cDeployTeam.m_vecDragonSKill.end());
		cTeamInf.m_vecBuddy.assign(cDeployTeam.m_vecBuddy.begin(), cDeployTeam.m_vecBuddy.end());
		pPlayerWarPtr->m_vecTeamInf.push_back(cTeamInf);

		//组装驻兵队伍信息
		CEtapeTeam cEtapeTeam;
		cEtapeTeam.m_nPlayerID = cDeployTeam.m_nPlayerID;
		cEtapeTeam.m_nTeamID = cTeamInf.m_nTeamID;
		cEtapeTeam.m_nSex = cDeployTeam.m_nSex;
		cEtapeTeam.m_strName = cDeployTeam.m_strName;
		cEtapeTeam.m_nLevel = cDeployTeam.m_nLevel;
		cEtapeTeam.m_nBattleEff = cDeployTeam.m_nBattEffect;
		pWarNationPtr->addNewEtapeTeam(cEtapeTeam);

		pWarNationPtr->incSessionID();

		//记录数据库
		CDBRegEtapeDeployRequestPtr dbreq = new CDBRegEtapeDeployRequest();
		dbreq->m_nFormationID = cDeployTeam.m_nFormationID;
		dbreq->m_nDragonLevel = cDeployTeam.m_nDragonLevel;
		dbreq->m_nLevel = cDeployTeam.m_nLevel;
		dbreq->m_nPlayerID = cDeployTeam.m_nPlayerID;
		dbreq->m_nTeamID = cTeamInf.m_nTeamID;
		dbreq->m_nBattEff = cDeployTeam.m_nBattEffect;
		dbreq->m_nNationID = pPlayerWarPtr->m_nNation;
		dbreq->m_strFormation = this->conFormationUnit2Str(cTeamInf.m_vecUint);
		dbreq->m_strDragonSkill = g_pConsummer->conDragonSkill2Str(cDeployTeam.m_vecDragonSKill);
		CDBBatBuddy cDbBuddy;
		for (size_t i = 0; i < cTeamInf.m_vecBuddy.size(); i++)
		{
			CBattleBuddy &cBuddy = cTeamInf.m_vecBuddy[i];
			cDbBuddy.m_nBattleEffect = cBuddy.m_nBattleEffect;
			cDbBuddy.m_nBuddyID = cBuddy.m_nBuddyID;
			cDbBuddy.m_nBuddySort = cBuddy.m_nBuddySort;
			cDbBuddy.m_nClass = cBuddy.m_nClass;
			cDbBuddy.m_nLevel = cBuddy.m_nLevel;
			cDbBuddy.m_nProfessionLevel = cBuddy.m_nProfessionLevel;
			cDbBuddy.m_nRebornNum = cBuddy.m_nRebornNum;
			cDbBuddy.m_nStar = cBuddy.m_nStar;
			g_pConsummer->Attr_VecToStr(cBuddy.m_vecAttr, cDbBuddy.m_strAttr);
			dbreq->m_vecBuddy.push_back(cDbBuddy);
		}

		g_DaServer->callServant(DB_REG_ETAPE_DEPLOY, dbreq.get(), NULL, 0, NULL, NULL);
		presponse->m_vecTeamID.push_back(cTeamInf.m_nTeamID);
	}
	
	presponse->m_nRetCode = SUCCESS;
	return presponse.get();
}

//获取兵战驻兵列表
IResponsePtr CWorldWarMgr::executeGetEtapeTeamList(IRequestPtr Request,CCurrentPtr Current)
{
	CGWMGetEtapeTeamListRequestPtr prequest = new CGWMGetEtapeTeamListRequest(Request->getSerialStream());
	CGWMGetEtapeTeamListResponsePtr presponse = new CGWMGetEtapeTeamListResponse();

	//获取城战国家信息
	CWarNationPtr pWarNationPtr = this->getWarNationPtr(prequest->m_nNationID);
	if (pWarNationPtr == NULL)
	{
		presponse->m_nRetCode = SUCCESS;
		return presponse.get();
	}

	if (prequest->m_nSessionID == pWarNationPtr->getSessionID())
	{
		presponse->m_nRetCode = SUCCESS;
		return presponse.get();
	}

	pWarNationPtr->getEtapeTeamList(presponse->m_vecTeam);

	presponse->m_nRetCode = SUCCESS;
	return presponse.get();
}

//调兵
IResponsePtr CWorldWarMgr::executeDeployForce(IRequestPtr Request,CCurrentPtr Current)
{
    CGWMDeployForceRequestPtr prequest = new CGWMDeployForceRequest(Request->getSerialStream());
    CGWMDeployForceResponsePtr presponse = new CGWMDeployForceResponse();

	//获取城战事件信息
	CWarCityPtr pWarCityPtr = this->getWarCityPtr(prequest->m_nCityID);
	if (pWarCityPtr == NULL)
	{
		presponse->m_nRetCode = NO_ACC_CITY;
		return presponse.get();
	}

	//判断事件类型
	if (pWarCityPtr->getAccType() == ACC_TYPE_NPC)
	{
		presponse->m_nRetCode = CANNT_DEPLOY_FORCE_CITY;
		return presponse.get();
	}

	if (pWarCityPtr->getQueueStatus() == 7)
	{
		presponse->m_nRetCode = WAR_FIGHT_OVER;
		return presponse.get();
	}
	//判断国家是否属于攻击或防守阵营
	if (prequest->m_nNation != pWarCityPtr->getNation() && prequest->m_nNation != pWarCityPtr->getAttNation())
	{
		presponse->m_nRetCode = CANNT_DEPLOY_FORCE_CITY;
		return presponse.get();
	}

	//判断兵站是否有驻兵
	CWarNationPtr pWarNationPtr = this->getWarNationPtr(prequest->m_nNation);
	if (pWarNationPtr == NULL)
	{
		presponse->m_nRetCode = NO_ETAPE_TEAM;
		return presponse.get();
	}

	std::map<uint32, uint8> mapForceTeamID;
	for (size_t i = 0; i < prequest->m_vecTeamID.size(); i++)
		mapForceTeamID[prequest->m_vecTeamID[i]] = 0;

	//进行调兵处理
	std::vector<CEtapeTeam>  vecEtapeTeam;
	pWarNationPtr->getEtapeTeamList(vecEtapeTeam);
	if (vecEtapeTeam.size() == 0)
	{
		presponse->m_nRetCode = NO_ETAPE_TEAM;
		return presponse.get();
	}
    
	//获取驻兵阵营
	uint8 nDir = WAR_DIR_TYPE_ATT; //选择阵营 0 没有 1攻击 2防守
	if (prequest->m_nNation == pWarCityPtr->getNation())
		nDir = WAR_DIR_TYPE_BLOW;

	//循环处理驻兵队伍
	for (size_t i = 0; i < vecEtapeTeam.size(); i++)
	{
		CEtapeTeam &cETeam = vecEtapeTeam[i];
		uint32 idTeam = cETeam.m_nTeamID;

		//判断是否在调兵的处理中
		std::map<uint32, uint8>::iterator iter = mapForceTeamID.find(idTeam);
		if (iter == mapForceTeamID.end())
			continue;

		OBJID idPlayer = cETeam.m_nPlayerID;
		//获取玩家信息
		CPlayerCityWarInfPtr pPlayerWarPtr = pWarNationPtr->getJoinPlayer(idPlayer);
		if (pPlayerWarPtr == NULL)
			continue;

		//判断队伍是否存在
		CPlayerTeamInf cTeamInf;
		bool bTeamFlag = false;
		for (size_t j = 0; j < pPlayerWarPtr->m_vecTeamInf.size(); j++)
		{
			cTeamInf = pPlayerWarPtr->m_vecTeamInf[j];
			if (cTeamInf.m_nTeamID == idTeam)
			{
				bTeamFlag = true;
				break;
			}
		}

		if (!bTeamFlag)
			continue;
       
		//记录成功调兵的队伍
		presponse->m_vecTeamID.push_back(idTeam);

		//判断玩家是否之前选择过
		CPlayerCityWarInfPtr pPlayerCityWarPtr =  pWarCityPtr->getJoinPlayer(idPlayer);
		if (pPlayerCityWarPtr == NULL)
		{
			pPlayerCityWarPtr = new CPlayerCityWarInf();
			pPlayerCityWarPtr->m_nKillNum = 0;
			pPlayerCityWarPtr->m_nCityID = prequest->m_nCityID;
			pPlayerCityWarPtr->m_nDir = nDir;
			pPlayerCityWarPtr->m_nSex = pPlayerWarPtr->m_nSex;
			pPlayerCityWarPtr->m_strName = pPlayerWarPtr->m_strName;
			pPlayerCityWarPtr->m_nLevel = pPlayerWarPtr->m_nLevel;
			pPlayerCityWarPtr->m_nNation = pPlayerWarPtr->m_nNation;
			pPlayerCityWarPtr->m_nBattEffect = pPlayerWarPtr->m_nBattEffect;
			pWarCityPtr->addPlayerCityInf(idPlayer, pPlayerCityWarPtr);
		}
		else
		{
			pPlayerCityWarPtr->m_nDir = nDir;
		}
        
		//玩家队伍处理
		cTeamInf.m_nTeamID = pWarCityPtr->getTeamID();
		pPlayerCityWarPtr->m_vecTeamInf.push_back(cTeamInf);
		pPlayerCityWarPtr->m_nDeployNum++;

		//队伍存放到大本营
		CWarCityTeam cCityTeam;
		cCityTeam.m_nLastHP = this->getTotBuddyHP(cTeamInf.m_vecUint);
		cCityTeam.m_nLevel = pPlayerWarPtr->m_nLevel;
		cCityTeam.m_nAvatarID = idPlayer;
		cCityTeam.m_nBattEffect = cETeam.m_nBattleEff;
		cCityTeam.m_nSex = pPlayerWarPtr->m_nSex;
		cCityTeam.m_nTeamID = cTeamInf.m_nTeamID;
		cCityTeam.m_nTeamType = TEAM_TYPE_PLAYER;
		cCityTeam.m_nTotHP = cCityTeam.m_nLastHP;
		cCityTeam.m_strName = pPlayerWarPtr->m_strName;
		cCityTeam.m_nVisionFlag = 1;
		cCityTeam.m_nNation = pPlayerWarPtr->m_nNation;
		cCityTeam.m_nTitleID = pPlayerWarPtr->m_nTitleID;

		for (size_t j = 0; j < cTeamInf.m_vecUint.size(); j++)
		{
			CFormatUint &cUint = cTeamInf.m_vecUint[j];
			CTeamBatInf cBatInf;
			cBatInf.m_nGrid = cUint.m_nGrid;
			cBatInf.m_nLastEn = cUint.m_nCurEn;
			cBatInf.m_nLastHP = cUint.m_nMaxBlood;
			cCityTeam.m_vecBatInf.push_back(cBatInf);
		}

		if (nDir == WAR_DIR_TYPE_BLOW)
		{
		    pWarCityPtr->addBlowTeam(cCityTeam);
			pWarCityPtr->incPlayerBlowTeamNum();
		}
		else
		{
            pWarCityPtr->addAttTeam(cCityTeam);
			pWarCityPtr->incPlayerAttTeamNum();
		}
	}
   
	//删除调兵成功的记录
	if (presponse->m_vecTeamID.size() > 0)
	{
		pWarCityPtr->getSessionID();
		for (size_t i = 0; i < presponse->m_vecTeamID.size(); i++)
		{
			uint32 idTeam = presponse->m_vecTeamID[i];
			pWarNationPtr->delEtapeTeam(idTeam);
		}
        
		//删除数据库数据
		this->delEtapeTeam(presponse->m_vecTeamID);
	}
	
	presponse->m_nRetCode = SUCCESS;
	return presponse.get();
}

//获取武将的总血量
uint32 CWorldWarMgr::getTotBuddyHP(std::vector<CFormatUint> &vecUint)
{
	uint32 tothp = 0;
	for (size_t i = 0; i < vecUint.size(); i++)
	{
		CFormatUint &cUint = vecUint[i];
		tothp += cUint.m_nMaxBlood;
	}
    
	return tothp;
}

//城战驻兵
IResponsePtr CWorldWarMgr::executeCityFightDeployed(IRequestPtr Request,CCurrentPtr Current)
{
	CGWMCityFightDeployedRequestPtr prequest = new CGWMCityFightDeployedRequest(Request->getSerialStream());
	CGWMCityFightDeployedResponsePtr presponse = new CGWMCityFightDeployedResponse();

	uint8 idCity = prequest->m_nCityID;

	//获取城战事件信息
	CWarCityPtr pWarCityPtr = this->getWarCityPtr(idCity);
	if (pWarCityPtr == NULL)
	{
		presponse->m_nRetCode = NO_ACC_CITY;
		return presponse.get();
	}

	OBJID idPlayer = prequest->m_nPlayerID;
	//获取玩家事件信息
	CPlayerCityWarInfPtr pPlayerCityWarPtr =  pWarCityPtr->getJoinPlayer(idPlayer);
	if (pPlayerCityWarPtr == NULL)
	{
		presponse->m_nRetCode = NO_SELECT_DIR;
		return presponse.get();
	}

	//判断是否已经选择阵营
	if (pPlayerCityWarPtr->m_nDir == WAR_DIR_TYPE_NO) //选择阵营 0 没有 1攻击 2防守
	{
		presponse->m_nRetCode = NO_SELECT_DIR;
		return presponse.get();
	}

	//判断道具数量是否满足
	presponse->m_nItemNum = 1;
	if (pPlayerCityWarPtr->m_nNation != pWarCityPtr->getNation() && pPlayerCityWarPtr->m_nNation != pWarCityPtr->getAttNation())
	{
		if ((pWarCityPtr->getNation() >= NATIONAL_WEI && pWarCityPtr->getNation() <= NATIONAL_WU) ||  
			(pWarCityPtr->getAttNation() >= NATIONAL_WEI &&  pWarCityPtr->getAttNation() <= NATIONAL_WU))
		{
			if (prequest->m_nItemNum == 1)
			{
				presponse->m_nRetCode = NO_ITEM;
				return presponse.get();
			}
			presponse->m_nItemNum = 2;
		}
	}
    
#ifdef WIN32
	;
#else
	if (pWarCityPtr->getAccType() == ACC_TYPE_NPC)
	{
		if (prequest->m_nItemID != m_nDeployedItem)
		{
			presponse->m_nRetCode = INVALID_PARAM;
			return presponse.get();
		}
	}
	else
	{
		if (prequest->m_nItemID != m_nDecDeployItem)
		{
			presponse->m_nRetCode = INVALID_PARAM;
			return presponse.get();
		}
	}
#endif
	//判断选择阵营是否合法
	if (pWarCityPtr->getAttNation() == pPlayerCityWarPtr->m_nNation)
	{
		if (pPlayerCityWarPtr->m_nDir != WAR_DIR_TYPE_ATT)
		{
			presponse->m_nRetCode = SELECT_DIR_DEF_INVLAD;
			return presponse.get();
		}
	}

	if (pWarCityPtr->getNation() == pPlayerCityWarPtr->m_nNation)
	{
		if (pPlayerCityWarPtr->m_nDir != WAR_DIR_TYPE_BLOW)
		{
			presponse->m_nRetCode = SELECT_DIR_ATT_INVLAD;
			return presponse.get();
		}
	}

	//判断队伍是否超过限制
	uint16 nBlowNum = pWarCityPtr->getBlowTeamNum(); 
	if (pPlayerCityWarPtr->m_nDir == WAR_DIR_TYPE_ATT)
		nBlowNum = pWarCityPtr->getAttTeamNum(); 
	if (nBlowNum >= m_nTotTeamLmt)
	{
		presponse->m_nRetCode = FIGHT_TEAM_NUM_LME;
		return presponse.get();
	}
	if (pWarCityPtr->getQueueStatus() == 7)
	{
		presponse->m_nRetCode = WAR_FIGHT_OVER;
		return presponse.get();
	}
    
	//获取玩家国战信息
	CWarJoinPlayerPtr pWarJoinPlayerPtr = this->getWarJoinPlayer(idPlayer);
	if (pWarJoinPlayerPtr == NULL)
	{
		presponse->m_nRetCode = FIGHT_TEAM_NUM_LME;
		return presponse.get();
	}

	uint8 nVisionFlag = 0;
	for (size_t i = 0; i < pPlayerCityWarPtr->m_vecTeamInf.size(); i ++)
	{
		CPlayerTeamInf &cTeamInf = pPlayerCityWarPtr->m_vecTeamInf[i];
		if (cTeamInf.m_nVisionFlag == 0)
		{
			nVisionFlag = 1;
			break;
		}
	}
	//派驻队伍处理
	CPlayerTeamInf cTeamInf;
	cTeamInf.m_nTeamID = pWarCityPtr->getTeamID();
	cTeamInf.m_nDragonLevel = prequest->m_nDragonLevel;
	cTeamInf.m_nFormationID = prequest->m_nFormationID;
	cTeamInf.m_vecUint.assign(prequest->m_vecUint.begin(), prequest->m_vecUint.end());
	cTeamInf.m_vecDragonSKill.assign(prequest->m_vecDragonSKill.begin(), prequest->m_vecDragonSKill.end());
	cTeamInf.m_vecBuddy.assign(prequest->m_vecBuddy.begin(), prequest->m_vecBuddy.end());
	cTeamInf.m_nVisionFlag = nVisionFlag;
	pPlayerCityWarPtr->m_vecTeamInf.push_back(cTeamInf);
    
	//增加上阵
	pPlayerCityWarPtr->m_nDeployNum++;
	pPlayerCityWarPtr->m_nTitleID = prequest->m_nTitleID;

	//记录玩家队伍信息
	CWarCityTeam cCityTeam;
	cCityTeam.m_nLastHP = prequest->m_nMaxHP;
	cCityTeam.m_nLevel = pPlayerCityWarPtr->m_nLevel;
	cCityTeam.m_nAvatarID = idPlayer;
	cCityTeam.m_nBattEffect = prequest->m_nBattleEff;
	cCityTeam.m_nSex = pPlayerCityWarPtr->m_nSex;
	cCityTeam.m_nTeamID = cTeamInf.m_nTeamID;
	cCityTeam.m_nTeamType = TEAM_TYPE_PLAYER;
	cCityTeam.m_nTotHP = prequest->m_nMaxHP;
	cCityTeam.m_strName = pPlayerCityWarPtr->m_strName;
	cCityTeam.m_nVisionFlag = nVisionFlag;
	cCityTeam.m_nNation = pPlayerCityWarPtr->m_nNation;
	cCityTeam.m_nTitleID = pPlayerCityWarPtr->m_nTitleID;
	CTeamBatInf cBatInf;
	for (size_t i = 0; i < prequest->m_vecUint.size(); i++)
	{
		CFormatUint &cUint = prequest->m_vecUint[i];
		if (cUint.m_nGrid == 16)
			continue;
		cBatInf.m_nGrid = cUint.m_nGrid;
		cBatInf.m_nLastEn = cUint.m_nCurEn;
		cBatInf.m_nLastHP = cUint.m_nMaxBlood;
		cCityTeam.m_vecBatInf.push_back(cBatInf);
	}
    
	bool infightflag = false;
	if (pPlayerCityWarPtr->m_nDir == WAR_DIR_TYPE_BLOW)
	{
		infightflag = pWarCityPtr->addBlowTeam(cCityTeam);
		pWarCityPtr->incPlayerBlowTeamNum();
	}
	else
	{
		infightflag = pWarCityPtr->addAttTeam(cCityTeam);
		pWarCityPtr->incPlayerAttTeamNum();
	}

	pWarCityPtr->getSessionID();

	//通知玩家上阵
	if (infightflag)
	{
		char sBuf[128]={0};
		sprintf(sBuf, "%d", cCityTeam.m_nTeamID);
		std::string strTeam = std::string(sBuf);
		//通知玩家上阵
		this->playerWarNotice(idPlayer, WORLD_WAR_TEAM_FIGHT, pWarCityPtr->getCityID(), strTeam);
	}

    presponse->m_nRetCode = SUCCESS;
	return presponse.get();
}

//获取城池战斗队列
IResponsePtr CWorldWarMgr::executeGetCityFightTeam(IRequestPtr Request,CCurrentPtr Current)
{
	CGWGetCityFightTeamRequestPtr prequest = new CGWGetCityFightTeamRequest(Request->getSerialStream());
	CGWGetCityFightTeamResponsePtr presponse = new CGWGetCityFightTeamResponse();

	uint8 idCity = prequest->m_nCityID;

	//获取城战事件信息
	CWarCityPtr pWarCityPtr = this->getWarCityPtr(idCity);
	if (pWarCityPtr == NULL)
	{
		presponse->m_nRetCode = NO_ACC_CITY;
		return presponse.get();
	}
    
	//获取战斗队栈队伍
	pWarCityPtr->getFightQueueTeam(CITY_QUEUE_NUM_FIRST, presponse->m_vecFightTeam1);
	pWarCityPtr->getFightQueueTeam(CITY_QUEUE_NUM_SECOND, presponse->m_vecFightTeam2);
	pWarCityPtr->getFightQueueTeam(CITY_QUEUE_NUM_THIRTY, presponse->m_vecFightTeam3);

	presponse->m_nRetCode = SUCCESS;
	return presponse.get();
}

//获取战斗队列信息
IResponsePtr CWorldWarMgr::executeGetFightQueue(IRequestPtr Request,CCurrentPtr Current)
{
	CGWMGetFightQueueRequestPtr prequest = new CGWMGetFightQueueRequest(Request->getSerialStream());
	CGWMGetFightQueueResponsePtr presponse = new CGWMGetFightQueueResponse();

	uint8 idCity = prequest->m_nCityID;

	//获取城战事件信息
	CWarCityPtr pWarCityPtr = this->getWarCityPtr(idCity);
	if (pWarCityPtr == NULL)
	{
		presponse->m_nRetCode = NO_ACC_CITY;
		return presponse.get();
	}

	presponse->m_nAttackNum = pWarCityPtr->getAttTeamNum();
	presponse->m_nBlowNum = pWarCityPtr->getBlowTeamNum();
	presponse->m_nQueueStatus = pWarCityPtr->getQueueStatus();
	presponse->m_nQueueFail = pWarCityPtr->getQueueFail();
	presponse->m_nQueueID = prequest->m_nQueueID;

	//判断session 
	if (prequest->m_nSession == pWarCityPtr->getQueueSession(prequest->m_nQueueID))
	{
		presponse->m_nSesion = prequest->m_nSession ;
		presponse->m_nRetCode = SUCCESS;
		return presponse.get();
	}

	//获取战斗简报
	CTeamBatReport cbatReport;
	if (pWarCityPtr->getQueueTeamBatReport(prequest->m_nQueueID, cbatReport))
	{
		CDateTime now;
		if (cbatReport.m_nNextTime >= now.getTotalSecs())
			cbatReport.m_nNextTime = cbatReport.m_nNextTime - now.getTotalSecs();
		else
            cbatReport.m_nNextTime = 0;
		presponse->m_vecBatReport.push_back(cbatReport);
	}

	//获取战斗队栈队伍
	pWarCityPtr->getFightQueueTeam(prequest->m_nQueueID, presponse->m_vecFightTeam);

	presponse->m_nSesion = pWarCityPtr->getQueueSession(prequest->m_nQueueID);
    
	presponse->m_nRetCode = SUCCESS;
	return presponse.get();
}

//更新城池杀敌榜
std::string CWorldWarMgr::conFormationUnit2Str(std::vector<CFormatUint> &vecUint)
{
	char sBuf[255]={0};
	for (size_t i = 0; i < vecUint.size(); i++)
	{
		CFormatUint &cUint = vecUint[i];
		char sUint[128]={0};

		sprintf(sUint, "%d:%d:%d:%d:%d:%d:%d;", cUint.m_nGrid, cUint.m_nConfId, cUint.m_nMaxBlood,
			                                    cUint.m_nMaxEn, cUint.m_nCurSpeed, cUint.m_nLevel, cUint.m_nClass);

		uint32 ilen = sizeof(sUint);

		strcat(sBuf, sUint);
	}

	return std::string(sBuf);
}

//撤兵
IResponsePtr CWorldWarMgr::executeGWMCityDepart(IRequestPtr Request, CCurrentPtr Current)
{
	CGWMCityDepartRequestPtr prequest = new CGWMCityDepartRequest(Request->getSerialStream());
	CGWMCityDepartResponsePtr presponse = new CGWMCityDepartResponse();

	uint8 idCity = prequest->m_nCityID;
	uint8 idNation = prequest->m_nNation;

	OBJID idPlayer = prequest->m_nPlayerID;
	uint16 nDepartTeamNum = 0;
	uint32 nReturnItem = 0;

	CPlayerCityWarInfPtr pPlayerCityWarPtr  = NULL;
	CWarCityPtr pWarCityPtr = NULL;
	CWarNationPtr pWarNationptr = NULL;
    
	//兵站撤兵
	if (idCity == 0)
	{
		pWarNationptr = this->getWarNationPtr(idNation);
		if (pWarNationptr == NULL)
		{
			presponse->m_nRetCode = NO_ACC_CITY;
			return presponse.get();
		}

		pPlayerCityWarPtr = pWarNationptr->getJoinPlayer(idPlayer);
		if (pPlayerCityWarPtr == NULL)
		{
			presponse->m_nRetCode = INVALID_PARAM;
			return presponse.get();
		}

		//判断玩家之前是否驻兵
		if (pPlayerCityWarPtr->m_vecTeamInf.size() == 0)
		{
			presponse->m_nRetCode = NO_DEPART_TEAM;
			return presponse.get();
		}

		nReturnItem = m_nDecDeployItem;
	}
	else
	{
		//获取城战事件信息
		pWarCityPtr = this->getWarCityPtr(idCity);
		if (pWarCityPtr == NULL)
		{
			presponse->m_nRetCode = NO_ACC_CITY;
			return presponse.get();
		}

		//获取玩家数据
		pPlayerCityWarPtr =  pWarCityPtr->getJoinPlayer(idPlayer);
		
		nReturnItem = m_nDecDeployItem;
		if (pWarCityPtr->getAccType() == ACC_TYPE_NPC)
			nReturnItem = m_nDeployedItem;
	}

    //判断玩家驻兵信息
	if (pPlayerCityWarPtr == NULL)
	{
		presponse->m_nRetCode = INVALID_PARAM;
		return presponse.get();
	}

	//判断玩家之前是否驻兵
	if (pPlayerCityWarPtr->m_vecTeamInf.size() == 0)
	{
		presponse->m_nRetCode = NO_DEPART_TEAM;
		return presponse.get();
	}

	for (size_t i = 0; i < prequest->m_vecTeamID.size(); i++)
	{
		uint32 idTeam = prequest->m_vecTeamID[i];
		std::vector<CPlayerTeamInf>::iterator iter = pPlayerCityWarPtr->m_vecTeamInf.begin();
		for (; iter != pPlayerCityWarPtr->m_vecTeamInf.end(); ++iter)
		{
			if (iter->m_nTeamID == idTeam)
			{
				if (prequest->m_nCityID != 0)
				{
					//判断是否已经在预备队中
					if (pWarCityPtr->isInPreBatTeam(idTeam))
						continue;

					//判断是否战斗过
					if (iter->m_nHaveFight == 0)
						nDepartTeamNum++;

					//删除队伍信息
					if (pPlayerCityWarPtr->m_nDir == WAR_DIR_TYPE_BLOW)
					{
						pWarCityPtr->eraseBlowTeam(idTeam);
						pWarCityPtr->decPlayerBlowTeamNum();
					}
					else
					{
						pWarCityPtr->eraseAttTeam(idTeam);
						pWarCityPtr->decPlayerAttTeamNum();	
					}
				}
				else
				{
					pWarNationptr->delEtapeTeam(idTeam);
					pWarNationptr->incSessionID();
					nDepartTeamNum++;
				}

				presponse->m_vecTeamID.push_back(idTeam);
				break;
			}
		}
	}
    
	//城战撤兵
	if (prequest->m_nCityID != 0)
	{
		int16 nDepartNum = presponse->m_vecTeamID.size();
		if (pPlayerCityWarPtr != NULL)
		{
			if (pPlayerCityWarPtr->m_nDeployNum >= nDepartNum)
				pPlayerCityWarPtr->m_nDeployNum = pPlayerCityWarPtr->m_nDeployNum - nDepartNum;
			else
				pPlayerCityWarPtr->m_nDeployNum = 0;
		}

		//判断是否需要返还双倍虎符
		if (pPlayerCityWarPtr->m_nNation != pWarCityPtr->getNation() && pPlayerCityWarPtr->m_nNation != pWarCityPtr->getAttNation())
		{
			if ((pWarCityPtr->getNation() >= NATIONAL_WEI && pWarCityPtr->getNation() <= NATIONAL_WU) ||  
				(pWarCityPtr->getAttNation() >= NATIONAL_WEI &&  pWarCityPtr->getAttNation() <= NATIONAL_WU))
			{
				nDepartTeamNum = nDepartTeamNum * 2;
			}
		}
	}

	for (size_t i = 0; i < presponse->m_vecTeamID.size(); i++)
		this->erasePlayerTeam(pPlayerCityWarPtr, presponse->m_vecTeamID[i]);

	//邮件返回道具
	std::vector<CRewardInfo> vecReward;
	CRewardInfo cReward;
	cReward.m_nType = REWARDTYPE_ITEM;
	cReward.m_nValue = nReturnItem;
	cReward.m_nNum = nDepartTeamNum;
	vecReward.push_back(cReward);
	std::string strTitle = g_pConsummer->formatLangString(LANGTYPE_WORLD_WAR, LANGTYPE_WORLD_WAR_DEPART_TITLET);
	std::string sContent = g_pConsummer->formatLangString(LANGTYPE_WORLD_WAR, LANGTYPE_WORLD_WAR_DEPART_CONTENT);
	g_pConsummer->sendMailToPlayer(idPlayer, strTitle, sContent, vecReward, ITEMOPT_CITY_DEPART_RETURN);
    
	//如果是兵站撤兵，需要删除数据库数据
	if (prequest->m_nCityID == 0)
	{
		//数据库删除记录
		this->delEtapeTeam(presponse->m_vecTeamID);
	}

	presponse->m_nRetCode = SUCCESS;
	return presponse.get();
}
void  CWorldWarMgr::erasePlayerTeam(CPlayerCityWarInfPtr pPlayerCityWarPtr, uint32 idTeam)
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

IResponsePtr  CWorldWarMgr::executeGWMWorldWarOnline(IRequestPtr Request, CCurrentPtr Current)
{
	CGWMWorldWarOnlineRequestPtr prequest = new CGWMWorldWarOnlineRequest(Request->getSerialStream());

	OBJID idPlayer = prequest->m_nPlayerId;
    uint8 idNation =  prequest->m_nNation;

	//获取阵营管辖的城池数量
	uint8 nNum = this->getNaitonAdminCityNum(idNation);
	this->playerWarNotice(idPlayer, WORLD_ADMIN_CITY_NUM, nNum, "");
    
	//如果是群雄逐鹿的活动日期，需要发送已经领取的杀敌奖励
	if (g_pConsummer->isFuncAccDay(6) && m_nPrinceAccFlag == 1)
	{
		std::map<OBJID, uint32>::iterator iter = m_mapPlayerKillNum.find(idPlayer);
		if (iter != m_mapPlayerKillNum.end())
		{
			for (size_t i = 0; i < m_vecPrinceKillRank.size(); i++)
			{
				CDBPrinceKillRank &cKillRank = m_vecPrinceKillRank[i];
				if (cKillRank.m_nPlayerID == idPlayer)
				{
					this->playerWarNotice(idPlayer, WORLD_WAR_HAVE_GET_RANK_REWARD, cKillRank.m_nKillNum, cKillRank.m_strKillReward);
					break;
				}
			}
		}
	}

	return NULL;
}

//鸣金收兵
IResponsePtr CWorldWarMgr::executeBeatRetreat(IRequestPtr Request, CCurrentPtr Current)
{
	CGWMBeatRetreatRequestPtr prequest = new CGWMBeatRetreatRequest(Request->getSerialStream());
	CGWMBeatRetreatResponsePtr presponse = new CGWMBeatRetreatResponse();

	uint8 idCity = prequest->m_nCityID;

	//获取城战事件信息
	CWarCityPtr pWarCityPtr = this->getWarCityPtr(idCity);
	if (pWarCityPtr == NULL)
	{
		presponse->m_nRetCode = NO_ACC_CITY;
		return presponse.get();
	}

	OBJID idPlayer = prequest->m_nPlayerID;

	//获取玩家数据
	CPlayerCityWarInfPtr pPlayerCityWarPtr =  pWarCityPtr->getJoinPlayer(idPlayer);
	if (pPlayerCityWarPtr == NULL)
	{
		presponse->m_nRetCode = INVALID_PARAM;
		return presponse.get();
	}

	//判断玩家之前是否驻兵
	if (pPlayerCityWarPtr->m_vecTeamInf.size() == 0)
	{
		presponse->m_nRetCode = NO_DEPART_TEAM;
		return presponse.get();
	}

	uint16 nDepartTeamNum = 0;
	std::vector<uint32> vecTeamID;
	std::vector<CPlayerTeamInf>::iterator iter = pPlayerCityWarPtr->m_vecTeamInf.begin();
	for (; iter != pPlayerCityWarPtr->m_vecTeamInf.end(); ++iter)
	{
		uint32 idTeam = iter->m_nTeamID;
		//判断是否已经在预备队中
		if (pWarCityPtr->isInPreBatTeam(idTeam))
			continue;

		//判断是否战斗过
		if (iter->m_nHaveFight == 0)
			nDepartTeamNum++;

		//删除队伍信息
		vecTeamID.push_back(idTeam);
		if (pPlayerCityWarPtr->m_nDir == WAR_DIR_TYPE_ATT)
		{
			pWarCityPtr->eraseAttTeam(idTeam);
			pWarCityPtr->decPlayerAttTeamNum();
		}
		else
		{
			pWarCityPtr->decPlayerBlowTeamNum();
			pWarCityPtr->eraseBlowTeam(idTeam);
		}
	}
    
	//删除玩家队伍
	for (size_t i = 0; i < vecTeamID.size(); i++)
	{
		uint32 idTeam = vecTeamID[i];
		this->erasePlayerTeam(pPlayerCityWarPtr, idTeam);
	}

	uint32 nReturnItem = m_nDecDeployItem;
	if (pWarCityPtr->getAccType() == ACC_TYPE_NPC)
		nReturnItem = m_nDeployedItem;
   
	//计算驻兵数量
	int16 nDepartNum = (int16)nDepartTeamNum;
	pPlayerCityWarPtr->m_nDeployNum = pPlayerCityWarPtr->m_nDeployNum - nDepartNum;
	if (pPlayerCityWarPtr->m_nDeployNum < 0)
		pPlayerCityWarPtr->m_nDeployNum = 0;

	if (pWarCityPtr->getStatus() == 0)
	{
		pPlayerCityWarPtr->m_nDir = WAR_DIR_TYPE_NO;
	}

	//判断是否需要返还双倍虎符
	if (pPlayerCityWarPtr->m_nNation != pWarCityPtr->getNation() && pPlayerCityWarPtr->m_nNation != pWarCityPtr->getAttNation())
	{
		if ((pWarCityPtr->getNation() >= NATIONAL_WEI && pWarCityPtr->getNation() <= NATIONAL_WU) ||  
			(pWarCityPtr->getAttNation() >= NATIONAL_WEI &&  pWarCityPtr->getAttNation() <= NATIONAL_WU))
		{
			nDepartTeamNum = nDepartTeamNum * 2;
		}
	}

	presponse->m_nDir = pPlayerCityWarPtr->m_nDir;
	pWarCityPtr->getSessionID();
	//邮件返回道具
	std::vector<CRewardInfo> vecReward;
	CRewardInfo cReward;
	cReward.m_nType = REWARDTYPE_ITEM;
	cReward.m_nValue = nReturnItem;
	cReward.m_nNum = nDepartTeamNum;
	vecReward.push_back(cReward);
	std::string strTitle = g_pConsummer->formatLangString(LANGTYPE_WORLD_WAR, LANGTYPE_WORLD_WAR_BEAT_TITLET);
	std::string sContent = g_pConsummer->formatLangString(LANGTYPE_WORLD_WAR, LANGTYPE_WORLD_WAR_BEAT_CONTENT);
	g_pConsummer->sendMailToPlayer(idPlayer, strTitle, sContent, vecReward, ITEMOPT_CITY_DEPART_RETURN);

	presponse->m_nRetCode = SUCCESS;
	return presponse.get();

}

//设置幻象
IResponsePtr CWorldWarMgr::executeGWMSetVision(IRequestPtr Request, CCurrentPtr Current)
{
	CGWMSetVisionRequestPtr prequest = new CGWMSetVisionRequest(Request->getSerialStream());
	CGWMSetVisionResponsePtr presponse = new CGWMSetVisionResponse();
    
	CDeployTeamInf &cDeployTeam = prequest->m_cDeployTeam;
	OBJID idPlayer = cDeployTeam.m_nPlayerID;
	uint8 idCity = prequest->m_nCityID;

	//获取城战事件信息
	CWarCityPtr pWarCityPtr = this->getWarCityPtr(idCity);
	if (pWarCityPtr == NULL)
	{
		presponse->m_nRetCode = NO_ACC_CITY;
		return presponse.get();
	}

	//获取玩家事件信息
	CPlayerCityWarInfPtr pPlayerCityWarPtr =  pWarCityPtr->getJoinPlayer(idPlayer);
	if (pPlayerCityWarPtr == NULL)
	{
		presponse->m_nRetCode = NO_SELECT_DIR;
		return presponse.get();
	}

	//判断是否已经选择阵营
	if (pPlayerCityWarPtr->m_nDir == WAR_DIR_TYPE_NO)
	{
		presponse->m_nRetCode = NO_SELECT_DIR;
		return presponse.get();
	}

	//判断道具数量是否满足
	presponse->m_nItemNum = 1;
	if (pPlayerCityWarPtr->m_nNation != pWarCityPtr->getNation() && pPlayerCityWarPtr->m_nNation != pWarCityPtr->getAttNation())
	{
		if ((pWarCityPtr->getNation() >= NATIONAL_WEI && pWarCityPtr->getNation() <= NATIONAL_WU) ||  
			(pWarCityPtr->getAttNation() >= NATIONAL_WEI &&  pWarCityPtr->getAttNation() <= NATIONAL_WU))
		{
			if (prequest->m_nItemNum == 1)
			{
				presponse->m_nRetCode = NO_ITEM;
				return presponse.get();
			}
			presponse->m_nItemNum = 2;
		}
	}

#ifdef WIN32
	;
#else
	//判断事件类型 检查上传道具是否合法
	if (pWarCityPtr->getAccType() == ACC_TYPE_NPC)
	{
		if (prequest->m_nItemID != m_nDeployedItem)
		{
			presponse->m_nRetCode = NO_ITEM;
			return presponse.get();
		}
	}
	else
	{
		if (prequest->m_nItemID != m_nDecDeployItem)
		{
			presponse->m_nRetCode = NO_ITEM;
			return presponse.get();
		}
	}
#endif
    
	//判断队伍是否超过限制
	uint16 nTeamNum = pWarCityPtr->getBlowTeamNum();
	if (pPlayerCityWarPtr->m_nDir == WAR_DIR_TYPE_ATT)
	    nTeamNum = pWarCityPtr->getAttTeamNum();
	if (nTeamNum >= m_nTotTeamLmt)
	{
		presponse->m_nRetCode = FIGHT_TEAM_NUM_LME;
		return presponse.get();
	}
	if (pWarCityPtr->getQueueStatus() == 7)
	{
		presponse->m_nRetCode = WAR_FIGHT_OVER;
		return presponse.get();
	}

	//获取玩家国战信息
	CWarJoinPlayerPtr pWarJoinPlayerPtr = this->getWarJoinPlayer(idPlayer);
	if (pWarJoinPlayerPtr == NULL)
	{
		presponse->m_nRetCode = FIGHT_TEAM_NUM_LME;
		return presponse.get();
	}

	//派驻队伍处理
	CPlayerTeamInf cTeamInf;
	cTeamInf.m_nTeamID = pWarCityPtr->getTeamID();
	cTeamInf.m_nDragonLevel = cDeployTeam.m_nDragonLevel;
	cTeamInf.m_nFormationID = cDeployTeam.m_nFormationID;
	cTeamInf.m_nVisionFlag = 1;
	cTeamInf.m_vecUint.assign(cDeployTeam.m_vecUint.begin(), cDeployTeam.m_vecUint.end());
	cTeamInf.m_vecDragonSKill.assign(cDeployTeam.m_vecDragonSKill.begin(), cDeployTeam.m_vecDragonSKill.end());
	cTeamInf.m_vecBuddy.assign(cDeployTeam.m_vecBuddy.begin(), cDeployTeam.m_vecBuddy.end());
	pPlayerCityWarPtr->m_vecTeamInf.push_back(cTeamInf);

	//增加上阵
	pPlayerCityWarPtr->m_nDeployNum++;
	pPlayerCityWarPtr->m_nTitleID = cDeployTeam.m_nTitleID;
	
	//记录玩家队伍信息
	CWarCityTeam cCityTeam;
	cCityTeam.m_nLastHP = cDeployTeam.m_nMaxHP;
	cCityTeam.m_nLevel = pPlayerCityWarPtr->m_nLevel;
	cCityTeam.m_nAvatarID = idPlayer;
	cCityTeam.m_nBattEffect = pPlayerCityWarPtr->m_nBattEffect;
	cCityTeam.m_nSex = pPlayerCityWarPtr->m_nSex;
	cCityTeam.m_nTeamID = cTeamInf.m_nTeamID;
	cCityTeam.m_nTeamType = TEAM_TYPE_PLAYER;
	cCityTeam.m_nTotHP = cDeployTeam.m_nMaxHP;
	cCityTeam.m_strName = pPlayerCityWarPtr->m_strName;
	cCityTeam.m_nVisionFlag = 1;
	cCityTeam.m_nNation = pPlayerCityWarPtr->m_nNation;
	cCityTeam.m_nTitleID = pPlayerCityWarPtr->m_nTitleID;
	CTeamBatInf cBatInf;
	for (size_t i = 0; i < cDeployTeam.m_vecUint.size(); i++)
	{
		CFormatUint &cUint = cDeployTeam.m_vecUint[i];
		if (cUint.m_nGrid == 16)
			continue;
		cBatInf.m_nGrid = cUint.m_nGrid;
		cBatInf.m_nLastEn = cUint.m_nCurEn;
		cBatInf.m_nLastHP = cUint.m_nMaxBlood;
		cCityTeam.m_vecBatInf.push_back(cBatInf);
	}

    //选择阵营 0 没有 1攻击 2防守
	if (pPlayerCityWarPtr->m_nDir == WAR_DIR_TYPE_BLOW)
	{
		pWarCityPtr->addBlowTeam(cCityTeam);
		pWarCityPtr->incPlayerBlowTeamNum();
	}
	else
	{
		pWarCityPtr->addAttTeam(cCityTeam);
		pWarCityPtr->incPlayerAttTeamNum();
	}
	pWarCityPtr->getSessionID();

	presponse->m_nRetCode = SUCCESS;
	presponse->m_nUniqID = prequest->m_nUniqID;
	presponse->m_nPlayerID = idPlayer;
	return presponse.get();
}

//获取城池驻兵列表
IResponsePtr CWorldWarMgr::executeGetDeployTeamList(IRequestPtr Request,CCurrentPtr Current)
{
	CGWMGetDeployTeamListRequestPtr prequest = new CGWMGetDeployTeamListRequest(Request->getSerialStream());
	CGWMGetDeployTeamListResponsePtr presponse = new CGWMGetDeployTeamListResponse();

	OBJID idPlayer = prequest->m_nPlayerID;
	uint8 idCity = prequest->m_nCityID;

	//获取城战事件信息
	CWarCityPtr pWarCityPtr = this->getWarCityPtr(idCity);
	if (pWarCityPtr == NULL)
	{
		presponse->m_nRetCode = NO_ACC_CITY;
		return presponse.get();
	}

	//获取玩家数据
	CPlayerCityWarInfPtr pPlayerCityWarPtr =  pWarCityPtr->getJoinPlayer(idPlayer);
	if (pPlayerCityWarPtr == NULL)
	{
		presponse->m_nRetCode = SUCCESS;
		return presponse.get();
	}

	//判断玩家之前是否驻兵
	if (pPlayerCityWarPtr->m_vecTeamInf.size() == 0)
	{
		presponse->m_nRetCode = SUCCESS;
		return presponse.get();
	}

    CDeployTeam cDeployTeam;
	for (size_t i = 0; i < pPlayerCityWarPtr->m_vecTeamInf.size(); i++)
	{
        CPlayerTeamInf &cTeamInf = pPlayerCityWarPtr->m_vecTeamInf[i];
		cDeployTeam.m_nTeamID = cTeamInf.m_nTeamID;
		cDeployTeam.m_nVisionFlag = cTeamInf.m_nVisionFlag;
		cDeployTeam.m_nStatus = 0;
		if (pWarCityPtr->isInPreBatTeam(cDeployTeam.m_nTeamID))
			cDeployTeam.m_nStatus  = 1;

		presponse->m_vecTeam.push_back(cDeployTeam);
	}
    
	presponse->m_nRetCode = SUCCESS;
	return presponse.get();
}

//单挑
IResponsePtr CWorldWarMgr::executeGWMCityPick(IRequestPtr Request, CCurrentPtr Current)
{
	CGWMCityPickRequestPtr prequest = new CGWMCityPickRequest(Request->getSerialStream());
	CGWMCityPickResponsePtr presponse = new CGWMCityPickResponse();
    
	OBJID idPlayer = prequest->m_nPlayerID;
	uint8 idCity = prequest->m_nCityID;

	//获取城战事件信息
	CWarCityPtr pWarCityPtr = this->getWarCityPtr(idCity);
	if (pWarCityPtr == NULL)
	{
		presponse->m_nRetCode = NO_ACC_CITY;
		return presponse.get();
	}

	if (pWarCityPtr->getStatus() != 1)
	{
		presponse->m_nRetCode = NO_ACC_CITY;
		return presponse.get();
	}

	//获取玩家数据
	CPlayerCityWarInfPtr pPlayerCityWarPtr =  pWarCityPtr->getJoinPlayer(idPlayer);
	if (pPlayerCityWarPtr == NULL)
	{
		presponse->m_nRetCode = INVALID_PARAM;
		return presponse.get();
	}

	//判断玩家之前是否驻兵
	if (pPlayerCityWarPtr->m_vecTeamInf.size() == 0)
	{
		presponse->m_nRetCode = NO_DEPART_TEAM;
		return presponse.get();
	}

	CDateTime now;

	//判断CD时间
	if (now.getTotalSecs() < pPlayerCityWarPtr->m_nPickTime)
	{
		presponse->m_nRetCode = PICK_CD_LMT;
		return presponse.get();
	}

	//获取驻兵信息
	uint8 isVisionTeam = 0;
	CPlayerTeamInf cTeamInf;
	for (size_t i = 0; i < pPlayerCityWarPtr->m_vecTeamInf.size(); i++)
	{
		CPlayerTeamInf &cCityTeam = pPlayerCityWarPtr->m_vecTeamInf[i];
		if (cCityTeam.m_nVisionFlag == 0)
		{
			cTeamInf = pPlayerCityWarPtr->m_vecTeamInf[i];
			if (pWarCityPtr->isInPreBatTeam(cTeamInf.m_nTeamID))
			{
				if (isVisionTeam == 0)
				    isVisionTeam = 1;
				continue;
			}
			if (pWarCityPtr->isInPickBatTeam(cTeamInf.m_nTeamID))
			{
				if (isVisionTeam == 0)
				    isVisionTeam = 1;
			    continue;
			}

			isVisionTeam = 2;
		}
	}
   
	//判断是否是只有幻影部队
	if (isVisionTeam == 0)
	{
		presponse->m_nRetCode = VISION_TEAM_CANNT_PICK;
		return presponse.get();
	}
	else if (isVisionTeam == 1)
	{
		presponse->m_nRetCode = FIGHTING_CANNT_DEPART;
		return presponse.get();
	}

	uint32 idTeam = cTeamInf.m_nTeamID;

	//判断对方是否还有队伍
	bool batt = true;
	if (pPlayerCityWarPtr->m_nDir == WAR_DIR_TYPE_BLOW)
		batt =  false;
	if (!pWarCityPtr->isHavaCanFightTeam(batt))
	{
		presponse->m_nRetCode = HAVE_NO_FIGHT_TEAM;
		return presponse.get();
	}
    
	//修改玩家战斗标志
	pPlayerCityWarPtr->m_nHaveFight = 1;

	//获取应答信息
	presponse->m_nCDTime = m_nPickCDTime + now.getTotalSecs();
	pPlayerCityWarPtr->m_nPickTime = presponse->m_nCDTime;
	presponse->m_nCityID = idCity;
	presponse->m_nAttTeamID = cTeamInf.m_nTeamID;
	presponse->m_nDragonLevel = cTeamInf.m_nDragonLevel;
	presponse->m_nFormationID = cTeamInf.m_nFormationID;
	presponse->m_nLevel = pPlayerCityWarPtr->m_nLevel;
	presponse->m_vecBuddy.assign(cTeamInf.m_vecBuddy.begin(), cTeamInf.m_vecBuddy.end());
	presponse->m_vecDragonSKill.assign(cTeamInf.m_vecDragonSKill.begin(), cTeamInf.m_vecDragonSKill.end());
	presponse->m_vecUint.assign(cTeamInf.m_vecUint.begin(), cTeamInf.m_vecUint.end());

	//获取城池的阵营BUFF加成
	this->getCityNationBuff(idCity, pPlayerCityWarPtr->m_nNation, presponse->m_vecBuff);

	//获取弱国保护的BUFF
	this->getWeakNationBuf(pPlayerCityWarPtr->m_nNation, presponse->m_vecBuff);

	//随机产生一个队伍
	pWarCityPtr->getPickTeam(batt, presponse->m_cBlowBatTeam, presponse->m_strBlowName);
	presponse->m_nBlowTeamID = presponse->m_cBlowBatTeam.m_nTeamID;
	presponse->m_nKillNum = pPlayerCityWarPtr->m_nKillNum;
	presponse->m_nContinuKill = cTeamInf.m_nContinKillNum;
	//获取BUFF
	this->getPlayerWarBuff(idCity, presponse->m_cBlowBatTeam);


	//把自己的队伍放到单挑队列中
	pWarCityPtr->putPlayerTeamToPickTeam(idPlayer, cTeamInf.m_nTeamID);

    presponse->m_nRetCode = SUCCESS;
	presponse->m_nPlayerID = idPlayer;
	return presponse.get();
}

//获取玩家的国战BUFF
void CWorldWarMgr::getPlayerWarBuff(uint8 idCity, CWorldBatTeam &cBatTeam)
{
	if (cBatTeam.m_nType == TEAM_TYPE_NPC)
		return;

	if (cBatTeam.m_vecPlayerInf.size() == 0)
		return;

	CPlayerFightTeamInf &cPlayerTeamInf = cBatTeam.m_vecPlayerInf[0];

	OBJID idPlayer = cPlayerTeamInf.m_nPlayerID;
    
	CWarJoinPlayerPtr pJoinPlayerPtr = this->getWarJoinPlayer(idPlayer);
	if (pJoinPlayerPtr != NULL)
	{
		//获取弱国保护的BUFF
		this->getWeakNationBuf(pJoinPlayerPtr->m_nNation, cPlayerTeamInf.m_vecBuff);

		//获取城池的BUFF
		this->getCityNationBuff(idCity, pJoinPlayerPtr->m_nNation, cPlayerTeamInf.m_vecBuff);
	}
    
	return;
}

//获取杀怪奖励
void CWorldWarMgr::getKillMonsterReward(CWarCityPtr pWarCityPtr, CPlayerCityWarInfPtr pPlayerCityWarPtr, OBJID idPlayer, uint32 idNpc, std::vector<CRewardInfo> &vecReward)
{
	uint8 nTeamReward = 0;
	const Json::Value& cNpcConf = g_pConsummer->warNpcConf(idNpc);
	if (cNpcConf == Json::nullValue)
		return;

	std::vector<std::string> vecOption;
    
	//获取概率奖励
	std::string strRateReward = cNpcConf["rate_reward"].asString();
	splitString(strRateReward, ";", vecOption, true);
	for (size_t i = 0; i < vecOption.size(); i++)
	{
		std::string str = vecOption[i];
		if(str.length() < 2)
			continue;
        
		int type;
		int value = 0;
		int num = 0;
		int droprate = 0;

		if (sscanf(str.c_str(), "%d:%d:%d:%d", &type, &value, &num, &droprate) != 4)
			break;

		int nRand = CRandom::rand(10000);
		if (nRand < droprate)
		{
			CRewardInfo info;
			info.m_nType = (uint8)type;
			info.m_nValue = (uint32)value;
			info.m_nNum = (uint32)num;
			vecReward.push_back(info);
		}
	}

	std::string tmpstr = cNpcConf["kill_reward"].asString();
	vecOption.clear();
	splitString(tmpstr, ";", vecOption, true);
	CRewardInfo cReward;
	for(size_t i = 0; i < vecOption.size(); ++ i)
	{
		uint32 nType = 0;
		uint32 nValue = 0;
		uint32 nNum = 0;
		tmpstr = vecOption[i];
		if (sscanf(tmpstr.c_str(), "%d:%d:%d", &nType, &nValue,&nNum) != 3)
			continue;

		cReward.m_nType = (uint8)nType;
		cReward.m_nValue = (uint32)nValue;
		cReward.m_nNum = (uint32)nNum;
		vecReward.push_back(cReward);
	}
    
	//判断是否是触发击杀名将类型的NPC
	uint8 formation_type = (uint8)cNpcConf["formation_type"].asUInt();
	std::string strNpcName = cNpcConf["name"].asString();
	int nRand = CRandom::rand(10000);
	switch (formation_type)
	{
	case NPC_FORMATION_TYPE_HERO:
		{
			if (nRand <= m_nKillTeam6Rate)
			{
				this->rewardNationKillTeamReward(pWarCityPtr, pPlayerCityWarPtr, idPlayer, formation_type, strNpcName);
			}
		}
		break;
	case NPC_FORMATION_TYPE_CAPTAIN:
		{
			if (nRand <= m_nKillTeam4Rate)
			{
				this->rewardNationKillTeamReward(pWarCityPtr, pPlayerCityWarPtr,  idPlayer, formation_type, strNpcName);
			}
		}
		break;
	case NPC_FORMATION_TYPE_BLEW:
		{
			this->rewardNationKillTeamReward(pWarCityPtr, pPlayerCityWarPtr,  idPlayer,  formation_type, strNpcName);
		}
		break;
	default:
		break;
	}

	return;
}

//给玩家增加杀怪的奖励
void CWorldWarMgr::addPlayerKillMonsterReward(CWarCityPtr pWarCityPtr, CPlayerCityWarInfPtr pPlayerCityWarPtr, OBJID idPlayer,  uint32 idNpc)
{
	//给玩家增加杀怪奖励
	std::vector<CRewardInfo> vecReward;
	this->getKillMonsterReward(pWarCityPtr, pPlayerCityWarPtr, idPlayer, idNpc, vecReward);
	//给玩家增加奖励
	this->addPlayerNewReward(pPlayerCityWarPtr, vecReward);
   
	return;
}

//玩家增加新的奖励
void CWorldWarMgr::addPlayerNewReward(CPlayerCityWarInfPtr pPlayerCityWarPtr, std::vector<CRewardInfo> &vecReward)
{
	for (size_t i = 0; i < vecReward.size(); i++)
	{
		bool bhavabefore = false;
		CRewardInfo &cReward = vecReward[i];
		for (size_t j = 0; j < pPlayerCityWarPtr->m_vecReward.size(); j++)
		{
			CRewardInfo &cHaveReward = pPlayerCityWarPtr->m_vecReward[j];
			if (cReward.m_nType == cHaveReward.m_nType)
			{
				if (cReward.m_nType == REWARDTYPE_ITEM)
				{
					if (cReward.m_nValue == cHaveReward.m_nValue)
					{
						bhavabefore = true;
						cHaveReward.m_nNum += cReward.m_nNum;
					}
				}
				else
				{
					bhavabefore = true;
					cHaveReward.m_nValue += cReward.m_nValue;
				}
			}
		}
		if (!bhavabefore)
			pPlayerCityWarPtr->m_vecReward.push_back(cReward);
	}

	return;
}

//给击杀名将NPC的阵营发送奖励
void CWorldWarMgr::rewardNationKillTeamReward(CWarCityPtr pWarCityPtr, CPlayerCityWarInfPtr pPlayerCityWarPtr,  OBJID idPlayer,  uint8 nTeamReward, std::string &strNpcName)
{
	CGWMGroupSendEmailRequestPtr gwmreq = new CGWMGroupSendEmailRequest();
	
	uint32 nLanguaType =0;
	std::vector<CRewardInfo> vecReward;
	if (nTeamReward == NPC_FORMATION_TYPE_HERO)
	{
		pWarCityPtr->getAllDefPlayer(pPlayerCityWarPtr->m_nNation, pPlayerCityWarPtr->m_nDir, gwmreq->m_vecPlayerID);
		vecReward.assign(m_vecKillTeam6Reward.begin(), m_vecKillTeam6Reward.end());
		gwmreq->m_vecRewards.assign(m_vecKillTeam6Reward.begin(), m_vecKillTeam6Reward.end());
		nLanguaType = LANGTYPE_WORLD_WAR_KILL_HERO;
		gwmreq->m_strTitle = g_pConsummer->formatLangString(LANGTYPE_WORLD_WAR, LANGTYPE_WORLD_WAR_KILL_HERO_TITLE);
		gwmreq->m_strContent = g_pConsummer->formatLangString(LANGTYPE_WORLD_WAR, LANGTYPE_WORLD_WAR_KILL_HERO_CONTENT, 
			                        pPlayerCityWarPtr->m_strName.data(), strNpcName.data());
	}
	else if (nTeamReward == NPC_FORMATION_TYPE_CAPTAIN)
	{
		pWarCityPtr->getAllDefPlayer(pPlayerCityWarPtr->m_nNation, pPlayerCityWarPtr->m_nDir, gwmreq->m_vecPlayerID);
		vecReward.assign(m_vecKillTeam4Reward.begin(), m_vecKillTeam4Reward.end());
		gwmreq->m_vecRewards.assign(m_vecKillTeam4Reward.begin(), m_vecKillTeam4Reward.end());
		nLanguaType = LANGTYPE_WORLD_WAR_KILL_CAPTAIN;
		gwmreq->m_strTitle = g_pConsummer->formatLangString(LANGTYPE_WORLD_WAR, LANGTYPE_WORLD_WAR_KILL_CAPTAIN_TITLE);
		gwmreq->m_strContent = g_pConsummer->formatLangString(LANGTYPE_WORLD_WAR, LANGTYPE_WORLD_WAR_KILL_CAPTAIN_CONTENT, 
			pPlayerCityWarPtr->m_strName.data(), strNpcName.data());
	}
	else if (nTeamReward == NPC_FORMATION_TYPE_BLEW)
	{
        gwmreq->m_vecPlayerID.push_back(idPlayer);
		vecReward.assign(m_vecKillTeam5Reward.begin(), m_vecKillTeam5Reward.end());
		gwmreq->m_vecRewards.assign(m_vecKillTeam5Reward.begin(), m_vecKillTeam5Reward.end());
		nLanguaType = 9999;
		gwmreq->m_strTitle = g_pConsummer->formatLangString(LANGTYPE_WORLD_WAR, LANGTYPE_WORLD_WAR_KILL_BLEW_TITLE);
		gwmreq->m_strContent = g_pConsummer->formatLangString(LANGTYPE_WORLD_WAR, LANGTYPE_WORLD_WAR_KILL_BLEW_CONTENT);
	}

	//发送邮件
	if (gwmreq->m_vecRewards.size() > 0)
	{
		gwmreq->m_nType = 0;
		gwmreq->m_nSenderID = 0;
		gwmreq->m_strChannel = "0";
		g_DaServer->callServant(GWM_GROUP_SEND_EMAIL, gwmreq.get(), NULL, 0, NULL, NULL);	 
	}
    
	//击杀自爆兵不需要公告
	if (nTeamReward == NPC_FORMATION_TYPE_BLEW)
		return;

	//发送公告
	std::string strNotice = g_pConsummer->formatLangString(LANGTYPE_WORLD_WAR, nLanguaType, pPlayerCityWarPtr->m_strName.data());

	if (strNotice.size() < 3)
		return;

	//发送公告
	std::vector<OBJID> vecPlayer;
	pWarCityPtr->getCityPlayer(vecPlayer);
	sendSysNoticeToPlayer(CHANNEL_CITY, strNotice, NULL, vecPlayer, 1);

    return;
}

//玩家战斗击杀玩家奖励
void CWorldWarMgr::rewardPlayerKillPlayer(CWarCityPtr pWarCityPtr, OBJID idPlayer)
{
	if(pWarCityPtr == NULL || idPlayer == 0)
		return;

	//获取玩家数据
	CPlayerCityWarInfPtr pPlayerCityWarPtr =  pWarCityPtr->getJoinPlayer(idPlayer);
	if (pPlayerCityWarPtr == NULL)
		return;

	//给玩家增加击杀玩家奖励
	int nRand = CRandom::rand(10000+1);
	if (nRand <= m_nKillPlayerRewardRate)
	{
		//给玩家增加奖励
		this->addPlayerNewReward(pPlayerCityWarPtr, m_vecKillPlayerReward);
	}

	return;
}

//玩家战斗胜利怪物奖励
void CWorldWarMgr::rewardPlayerKillMonster(CWarCityPtr pWarCityPtr, OBJID idPlayer, uint32 idNpc)
{
	if(pWarCityPtr == NULL || idPlayer == 0 || idNpc == 0)
		return;

	//获取玩家数据
	CPlayerCityWarInfPtr pPlayerCityWarPtr =  pWarCityPtr->getJoinPlayer(idPlayer);
	if (pPlayerCityWarPtr == NULL)
		return;

	this->addPlayerKillMonsterReward(pWarCityPtr, pPlayerCityWarPtr, idPlayer, idNpc);        	

	return;
}

//玩家获胜处理(同时阵亡)
void CWorldWarMgr::playerDieSuccFight(CWarCityPtr pWarCityPtr, OBJID idPlayer, uint32 idTeam)
{
	if(pWarCityPtr == NULL || idPlayer == 0)
		return;

	//获取玩家数据
	CPlayerCityWarInfPtr pPlayerCityWarPtr =  pWarCityPtr->getJoinPlayer(idPlayer);
	if (pPlayerCityWarPtr == NULL)
		return;

	//单挑成功，连胜加一
	pPlayerCityWarPtr->m_nKillNum++;
	pPlayerCityWarPtr->m_nHaveFight = 1;   

	//更新排行榜
	pWarCityPtr->updCityKillRank(idPlayer, pPlayerCityWarPtr);

}

//玩家获胜处理
void CWorldWarMgr::playerSuccFight(CWarCityPtr pWarCityPtr, OBJID idPlayer, uint32 idTeam)
{
	if(pWarCityPtr == NULL || idPlayer == 0)
		return;

	//获取玩家数据
	CPlayerCityWarInfPtr pPlayerCityWarPtr =  pWarCityPtr->getJoinPlayer(idPlayer);
	if (pPlayerCityWarPtr == NULL)
		return;

	//单挑成功，连胜加一
	pPlayerCityWarPtr->m_nKillNum++;
	pPlayerCityWarPtr->m_nHaveFight = 1;   
	uint16 nContinueKillNum = 0;
	uint8 nVision = 0;
	for (size_t i = 0; i < pPlayerCityWarPtr->m_vecTeamInf.size(); i++)
	{
		CPlayerTeamInf &cTeamInf = pPlayerCityWarPtr->m_vecTeamInf[i];
		if (cTeamInf.m_nTeamID == idTeam)
		{
			cTeamInf.m_nHaveFight = 1;
			cTeamInf.m_nContinKillNum++;
			nContinueKillNum = cTeamInf.m_nContinKillNum;
			nVision = cTeamInf.m_nVisionFlag;
		}
	}

	//更新排行榜
	pWarCityPtr->updCityKillRank(idPlayer, pPlayerCityWarPtr);
    
	//发送连斩通告
	this->sendContinueKillNotice(pWarCityPtr, pPlayerCityWarPtr, nContinueKillNum,nVision);

	return;
}

//发送连斩通告
void CWorldWarMgr::sendContinueKillNotice(CWarCityPtr pWarCityPtr, CPlayerCityWarInfPtr pPlayerCityWarPtr, uint16 nContinueKillNum, uint8 nVision)
{
	if (pPlayerCityWarPtr == NULL || pWarCityPtr == NULL)
		return;

	//发送公告
	if (nContinueKillNum != 5 && nContinueKillNum != 10 && nContinueKillNum != 20)
	    return;

    std::string strNotice = "";
	if (nVision == 0)
	{
	if (nContinueKillNum == 5)
         strNotice = g_pConsummer->formatLangString(LANGTYPE_WORLD_WAR, LANGTYPE_WORLD_WAR_CONTINUE_KILL_5, pPlayerCityWarPtr->m_strName.data());
	if (nContinueKillNum == 10)
		strNotice = g_pConsummer->formatLangString(LANGTYPE_WORLD_WAR, LANGTYPE_WORLD_WAR_CONTINUE_KILL_10, pPlayerCityWarPtr->m_strName.data());
	if (nContinueKillNum == 20)
		strNotice = g_pConsummer->formatLangString(LANGTYPE_WORLD_WAR, LANGTYPE_WORLD_WAR_CONTINUE_KILL_20, pPlayerCityWarPtr->m_strName.data());
	}
	else
	{
		if (nContinueKillNum == 5)
			strNotice = g_pConsummer->formatLangString(LANGTYPE_WORLD_WAR, LANGTYPE_WORLD_WAR_CONTINUE_KILL_5_VISON, pPlayerCityWarPtr->m_strName.data());
		if (nContinueKillNum == 10)
			strNotice = g_pConsummer->formatLangString(LANGTYPE_WORLD_WAR, LANGTYPE_WORLD_WAR_CONTINUE_KILL_10_VISION, pPlayerCityWarPtr->m_strName.data());
		if (nContinueKillNum == 20)
			strNotice = g_pConsummer->formatLangString(LANGTYPE_WORLD_WAR, LANGTYPE_WORLD_WAR_CONTINUE_KILL_20_VISION, pPlayerCityWarPtr->m_strName.data());
	}

	if (strNotice.size() < 3)
		return;

	//发送公告
	std::vector<OBJID> vecPlayer;
	pWarCityPtr->getCityPlayer(vecPlayer);
	sendSysNoticeToPlayer(CHANNEL_CITY, strNotice, NULL, vecPlayer, 1);

	return;
}

//自动单挑应答处理
void CWorldWarMgr::onResponseAutoPickTeamFight(IResponsePtr pResponse, CCurrentPtr Current, CRefSharePtr Param)
{
	CGWWorldCityFightResponsePtr presponse = new CGWWorldCityFightResponse(pResponse->getSerialStream());
	if (presponse->m_nRetCode != SUCCESS)
		return;

	CDateTime now;

	uint8 idCity = presponse->m_nCityID;

	//获取城战事件信息
	CWarCityPtr pWarCityPtr = this->getWarCityPtr(idCity);
	if (pWarCityPtr == NULL)
		return;

	//获取攻击以及防守方队伍信息
	CWarCityTeam cAttCityTeam;
    CWarCityTeam cBlowCityTeam;
	pWarCityPtr->getInFightTeam(presponse->m_nAttTeamID, cAttCityTeam);
	pWarCityPtr->getInFightTeam(presponse->m_nBlowTeamID, cBlowCityTeam);

	OBJID idPlayer = cBlowCityTeam.m_nAvatarID;

	//获取玩家数据
	CPlayerCityWarInfPtr pPlayerCityWarPtr =  pWarCityPtr->getJoinPlayer(idPlayer);
	if (pPlayerCityWarPtr == NULL)
		return;

	//对战斗结果进行处理
	if (presponse->m_nFightRet == 1)
	{
		//战胜奖励
		this->playerSuccFight(pWarCityPtr, idPlayer, cBlowCityTeam.m_nTeamID);

		//更新队伍的血量
		pWarCityPtr->updTeamBatGrid(presponse->m_nBlowTeamID, presponse->m_vecGrid);
		pWarCityPtr->setTeamLastHP(presponse->m_nBlowTeamID, presponse->m_nLastBlood);

		//给玩家杀怪奖励
		if (cAttCityTeam.m_nTeamType == TEAM_TYPE_NPC)
			this->addPlayerKillMonsterReward(pWarCityPtr, pPlayerCityWarPtr, idPlayer, cAttCityTeam.m_nNpcID);   

		//玩家队伍回到大本营
		pWarCityPtr->pickTeamReback(presponse->m_nBlowTeamID); 

		//清理怪物队伍
		pWarCityPtr->pickTeamErase(presponse->m_nAttTeamID);

		//增加杀敌数
		this->addPlayerKillNum(idPlayer);
	}
	else if (presponse->m_nFightRet == 2 && cAttCityTeam.m_nTeamType == TEAM_TYPE_NPC)
	{
		//增加杀敌数
		this->addPlayerKillNum(idPlayer);

		//单挑失败清理队伍
		this->erasePlayerTeam(pPlayerCityWarPtr, presponse->m_nBlowTeamID);
		this->playerWarNotice(idPlayer, WORLD_WAR_TEAM_END, idCity, "");
		//清理玩家队伍
		pWarCityPtr->pickTeamErase(presponse->m_nBlowTeamID);

		//给玩家杀怪奖励
		this->addPlayerKillMonsterReward(pWarCityPtr, pPlayerCityWarPtr, idPlayer,  cAttCityTeam.m_nNpcID);   

		//清理怪物队伍
		pWarCityPtr->pickTeamErase(presponse->m_nAttTeamID);
	}
	else
	{
		//单挑失败清理队伍
		this->erasePlayerTeam(pPlayerCityWarPtr, presponse->m_nBlowTeamID);
		this->playerWarNotice(idPlayer, WORLD_WAR_TEAM_END, idCity, "");

		//更新队伍的血量
		pWarCityPtr->updTeamBatGrid(presponse->m_nAttTeamID, presponse->m_vecGrid);
		pWarCityPtr->setTeamLastHP(presponse->m_nAttTeamID, presponse->m_nLastBlood);

		//清理玩家队伍
		pWarCityPtr->pickTeamErase(presponse->m_nBlowTeamID);

		//被挑战队伍回到大本营
		pWarCityPtr->pickTeamReback(presponse->m_nAttTeamID); 
	}

	//判断玩家是否在线
	CUserSessionInfoPtr psession = g_pConsummer->getUserSession(idPlayer);
	if (psession != NULL)
	{
		//记录战报，并发通知
		uint32 idBattle = pWarCityPtr->getBattleID();
		pWarCityPtr->saveBattle(idBattle, presponse->m_strBatReport);
		//发送通知
		this->playerWarNotice(idPlayer, WORLD_PICK_NOTICE, idBattle, cAttCityTeam.m_strName);
	}

	return;
}

//单挑结果通知
IResponsePtr CWorldWarMgr::executeGWMCityPickNotice(IRequestPtr Request, CCurrentPtr Current)
{
	CGWMCityPickNoticeRequestPtr prequest = new CGWMCityPickNoticeRequest(Request->getSerialStream());
	CGWMCityPickNoticeResponsePtr presponse = new CGWMCityPickNoticeResponse();

	uint8 idCity = prequest->m_nCityID;

	//获取城战事件信息
	CWarCityPtr pWarCityPtr = this->getWarCityPtr(idCity);
	if (pWarCityPtr == NULL)
	{
		presponse->m_nRetCode = NO_ACC_CITY;
		return presponse.get();
	}

	OBJID idPlayer = prequest->m_nPlayerID;

	//获取玩家数据
	CPlayerCityWarInfPtr pPlayerCityWarPtr =  pWarCityPtr->getJoinPlayer(idPlayer);
	if (pPlayerCityWarPtr == NULL)
	{
		presponse->m_nRetCode = INVALID_PARAM;
		return presponse.get();
	}

	CWarCityTeam cCityTeam;
	pWarCityPtr->getInFightTeam(prequest->m_nBlowTeamID, cCityTeam);

	//对战斗结果进行处理
	if (prequest->m_nFightRet == 0)
	{
		//战胜奖励
		this->playerSuccFight(pWarCityPtr, idPlayer, prequest->m_nAttTeamID);

		//更新队伍的血量
		pWarCityPtr->updTeamBatGrid(prequest->m_nAttTeamID, prequest->m_vecGrid);
		pWarCityPtr->setTeamLastHP(prequest->m_nAttTeamID, prequest->m_nLastHP);
        
		//给玩家杀怪奖励
		if (cCityTeam.m_nTeamType == TEAM_TYPE_NPC)
			this->addPlayerKillMonsterReward(pWarCityPtr, pPlayerCityWarPtr, idPlayer, cCityTeam.m_nNpcID);   

		//玩家队伍回到大本营
		pWarCityPtr->pickTeamReback(prequest->m_nAttTeamID); 

		//清理怪物队伍
		pWarCityPtr->pickTeamErase(prequest->m_nBlowTeamID);

		//增加玩家杀敌数
		this->addPlayerKillNum(idPlayer);
	}
	else if (prequest->m_nFightRet == 1)
	{
		//单挑失败清理队伍
		this->erasePlayerTeam(pPlayerCityWarPtr, prequest->m_nAttTeamID);
       
		//玩家死亡通知
		this->playerWarNotice(idPlayer, WORLD_WAR_TEAM_END, idCity, "");

		//更新队伍的血量
		pWarCityPtr->updTeamBatGrid(prequest->m_nBlowTeamID, prequest->m_vecGrid);
		pWarCityPtr->setTeamLastHP(prequest->m_nBlowTeamID, prequest->m_nLastHP);
        
		//清理玩家队伍
	    pWarCityPtr->pickTeamErase(prequest->m_nAttTeamID);

		//被挑战队伍回到大本营
		pWarCityPtr->pickTeamReback(prequest->m_nBlowTeamID); 
	}
	else  //自爆兵的情况
	{
		//战胜奖励
		this->playerDieSuccFight(pWarCityPtr, idPlayer, prequest->m_nAttTeamID);

		//单挑失败清理队伍
		this->erasePlayerTeam(pPlayerCityWarPtr, prequest->m_nAttTeamID);

		//玩家死亡通知
		this->playerWarNotice(idPlayer, WORLD_WAR_TEAM_END, idCity, "");

		this->addPlayerKillMonsterReward(pWarCityPtr, pPlayerCityWarPtr, idPlayer, cCityTeam.m_nNpcID);   

		//清理玩家队伍
		pWarCityPtr->pickTeamErase(prequest->m_nAttTeamID);

		//清理怪物队伍
		pWarCityPtr->pickTeamErase(prequest->m_nBlowTeamID);
        
		//增加玩家杀敌数
		this->addPlayerKillNum(idPlayer);
	}

	return NULL;
}


/*
*@获取城战队伍列表
*/
IResponsePtr CWorldWarMgr::executeGetCityTeamList(IRequestPtr Request,CCurrentPtr Current)
{
	CGWMGetCityTeamListRequestPtr prequest = new CGWMGetCityTeamListRequest(Request->getSerialStream());
	CGWMGetCityTeamListResponsePtr presponse = new CGWMGetCityTeamListResponse();

	uint8 idCity = prequest->m_nCityID;

	//获取城战事件信息
	CWarCityPtr pWarCityPtr = this->getWarCityPtr(idCity);
	if (pWarCityPtr == NULL)
	{
		presponse->m_nRetCode = NO_ACC_CITY;
		return presponse.get();
	}

	//判断玩家session 
	if (prequest->m_nSessionID == pWarCityPtr->getCurSession() && prequest->m_nSessionID != 0)
	{
		presponse->m_nRetCode = SUCCESS;
		presponse->m_nSessionID = prequest->m_nSessionID;
		return presponse.get();
	}
    
	//获取队伍列表
	pWarCityPtr->getTeamList(presponse->m_vecAttTeamList, presponse->m_vecBlowTeamList);

	if (pWarCityPtr->getCurSession() == 0)
		pWarCityPtr->getSessionID();

	presponse->m_nRetCode = SUCCESS;
	presponse->m_nSessionID = pWarCityPtr->getCurSession();
	return presponse.get();
}

//查询队伍阵型
IResponsePtr  CWorldWarMgr::executeGetCityFightTeamInf(IRequestPtr Request,CCurrentPtr Current)
{
	CGWGetFightTeamInfRequestPtr prequest = new CGWGetFightTeamInfRequest(Request->getSerialStream());
	CGWGetFightTeamInfResponsePtr presponse = new CGWGetFightTeamInfResponse();

	uint32 idTeam = prequest->m_nTeamID;
	uint32 nCityID = idTeam/100000;
	uint8 idCity = (uint8)nCityID;

	//获取城战事件信息
	CWarCityPtr pWarCityPtr = this->getWarCityPtr(idCity);
	if (pWarCityPtr == NULL)
	{
		presponse->m_nRetCode = NO_ACC_CITY;
		return presponse.get();
	}

	//判断队伍是否是在预备战斗中
	if (!pWarCityPtr->isInPreBatTeam(idTeam))
	{
		presponse->m_nRetCode = TEAM_HAVE_BE_KILL;
		return presponse.get();
	}

	//获取队伍信息
	presponse->m_nContinueKill = pWarCityPtr->getTeamInf(idTeam, presponse->m_nType, presponse->m_vecTeam);
	if (presponse->m_vecTeam.size()  == 0)
		presponse->m_nIsLive = 1;
    
	presponse->m_nRetCode = SUCCESS;
	presponse->m_nTeamID = prequest->m_nTeamID;
	return presponse.get();
}

//查询国战战报
IResponsePtr CWorldWarMgr::executeQueryWorldWarBattle(IRequestPtr Request,CCurrentPtr Current)
{
	CGWMGetWorldWarBattleRequestPtr prequest = new CGWMGetWorldWarBattleRequest(Request->getSerialStream());
	CGWMGetWorldWarBattleResponsePtr presponse = new CGWMGetWorldWarBattleResponse();

	uint32 nCityID = prequest->m_nBattleID/100000;
	uint8 idCity = (uint8)nCityID;

	log_info("executeQueryWorldWarBattle m_nBattleID= %d cityid %d ", prequest->m_nBattleID, idCity);

	//获取城战事件信息
	CWarCityPtr pWarCityPtr = this->getWarCityPtr(idCity);
	if (pWarCityPtr == NULL)
	{
		presponse->m_nRetCode = WAR_ACC_END;
		return presponse.get();
	}

	presponse->m_nRetCode = SUCCESS;
	presponse->m_strBattle = pWarCityPtr->queryBattle(prequest->m_nBattleID);
	if (presponse->m_strBattle == "1")
		presponse->m_nRetCode = BATTLE_NOEXIST;
    return presponse.get();
}

IResponsePtr CWorldWarMgr::executeGMUpdCityOccup(IRequestPtr Request, CCurrentPtr Current)
{
	CGWMUpdCityOccupRequestPtr prequest = new CGWMUpdCityOccupRequest(Request->getSerialStream());
	CGWMUpdCityOccupResponsePtr presponse = new CGWMUpdCityOccupResponse();
	uint8 idCity = prequest->m_nCityID;
	uint8 idNation  = prequest->m_nNationID;
	CWorldCityPtr pCityPtr =  g_pConsummer->getWorldCityPtr(idCity);
	if (pCityPtr == NULL)
		return NULL;
	CDBRegCityOccupRequestPtr dbreq = new CDBRegCityOccupRequest();
	dbreq->m_nCityID = idCity;
	dbreq->m_nNatianID = idNation;
    g_DaServer->callServant(DB_REG_CITY_OCCUP, dbreq.get(), NULL, 0, NULL, NULL);
	return NULL;
}
//查询战报
IResponsePtr CWorldWarMgr::executeQueryCityBattleReport(IRequestPtr Request,CCurrentPtr Current)
{
	CGWMQueryCityBattReportRequestPtr prequest = new CGWMQueryCityBattReportRequest(Request->getSerialStream());
	CGWMQueryCityBattReportResponsePtr presponse = new CGWMQueryCityBattReportResponse();

	uint8 idCity = prequest->m_nCityID;
	//获取城战事件信息
	CWarCityPtr pWarCityPtr = this->getWarCityPtr(idCity);
	if (pWarCityPtr == NULL)
	{
		presponse->m_nRetCode = NO_ACC_CITY;
		return presponse.get();
	}

	//获取队伍战报
	std::string strBattle = pWarCityPtr->getQueueBattleRepor(prequest->m_nQueueID);
	if (strBattle.size() < 5)
	{
		presponse->m_nRetCode = CITY_QUEUE_NO_BAT_REPORT;
	}
	else
	{
		CBytesBufferPtr ptmpbuff = CBytesBufferPtr::createInstance();
		unSerialStr(strBattle, *(ptmpbuff.get()));
		uncompressBytesbuff(ptmpbuff);
		presponse->unserial(*(ptmpbuff.get()));
		if (presponse->m_cFightData.m_nAttackID == 0 && presponse->m_cFightData.m_nBlowBlood == 0 && presponse->m_cFightData.m_nAttackBlood ==0)
			presponse->m_nRetCode = CITY_QUEUE_NO_BAT_REPORT;
	}

	return presponse.get();
}

//获取疆域产出信息
IResponsePtr CWorldWarMgr::executeQueryDomainRewardInf(IRequestPtr Request,CCurrentPtr Current)
{
	CGWMQueryDomainRewardInfRequestPtr prequest = new CGWMQueryDomainRewardInfRequest(Request->getSerialStream());
	CGWMQueryDomainRewardInfResponsePtr presponse = new CGWMQueryDomainRewardInfResponse();

	//获取国家信息
	CWarNationPtr pWarNationPtr = this->getWarNationPtr(prequest->m_nNation);
	if (pWarNationPtr == NULL)
	{
		presponse->m_nRetCode = SUCCESS;
		return presponse.get();
	}
    
	//获取定点的奖励
	std::vector<CDomainReward> vecDomainReward;
	pWarNationPtr->getAllDomainReward(vecDomainReward);
	if (vecDomainReward.size() == 0)
	{   
		std::map<uint8, std::vector<uint8> > mapNationCity;
		std::vector<CCityOccup> vecCityOccup;
		this->getNationAdminCity(vecCityOccup, mapNationCity);
		std::map<uint8, std::vector<uint8> >::iterator iter_city = mapNationCity.begin();
		for (; iter_city != mapNationCity.end(); ++iter_city)
		{
			if (iter_city->first == prequest->m_nNation)
			{
			    this->nationCityReward(iter_city->first, iter_city->second, vecCityOccup, m_nDomainBeginTime);
				break;
			}
		}
		pWarNationPtr->getAllDomainReward(vecDomainReward);
		pWarNationPtr->clearDomainReward();
		if (vecDomainReward.size() == 0)
		{
			presponse->m_nRetCode = SYSTEM_ERROR;
			return presponse.get();
		}
	}
    
	//获取奖励
	if (prequest->m_nType == 0)
	{
		for (size_t i = 0; i < vecDomainReward.size(); i++)
		{
			CDomainReward &cReward = vecDomainReward[i];
			if (cReward.m_nTime >= prequest->m_nHour)
			{
				presponse->m_vecDomainReward.push_back(cReward);
				break;
			}
		}
		if (presponse->m_vecDomainReward.size() == 0)
		{
			size_t i =  vecDomainReward.size(); 
			if (i > 0)
			{
				CDomainReward &cReward = vecDomainReward[i-1];
				presponse->m_vecDomainReward.push_back(cReward);
			}
		}
	}
	else
	{
		for (size_t i  = 0; i < vecDomainReward.size(); i++)
		{
			CDomainReward &cReward = vecDomainReward[i];
			if (cReward.m_nTime >= prequest->m_nHour)
			{
				presponse->m_vecDomainReward.push_back(cReward);
				if (presponse->m_vecDomainReward.size() >= 2)
					break;
			}
		}
	}

	presponse->m_nRetCode = SUCCESS;
	presponse->m_nHour = prequest->m_nHour;
	return presponse.get();
}

//获取群雄逐鹿事件
IResponsePtr CWorldWarMgr::executeGetPrinceAcc(IRequestPtr Request,CCurrentPtr Current)
{
	CGWGetPrinceAccRequestPtr prequest = new CGWGetPrinceAccRequest(Request->getSerialStream());
	CGWGetPrinceAccResponsePtr presponse = new CGWGetPrinceAccResponse();
    
	if (m_vecPrinceAcc.size() > 0)
		presponse->m_vecPrinceAcc.assign(m_vecPrinceAcc.begin(), m_vecPrinceAcc.end());
    
	presponse->m_nRetCode = SUCCESS;
 	return presponse.get();
}

IResponsePtr CWorldWarMgr::executeGetKillNumAward(IRequestPtr Request,CCurrentPtr Current)
{
	CGWMGetKillNumAwardRequestPtr prequest = new CGWMGetKillNumAwardRequest(Request->getSerialStream());
	CGWMGetKillNumAwardResponsePtr presponse = new CGWMGetKillNumAwardResponse();

	OBJID idPlayer = prequest->m_nPlayerID;
	std::map<OBJID, uint32>::iterator iter = m_mapPlayerKillNum.find(idPlayer);
	if (iter == m_mapPlayerKillNum.end())
	{
		presponse->m_nRetCode = KILL_NUM_NO_ENOUHT;
		return presponse.get();
	}
	if (prequest->m_nKillNum > iter->second)
	{
		presponse->m_nRetCode = KILL_NUM_NO_ENOUHT;
		return presponse.get();
	}
	for (size_t i = 0; i < m_vecPrinceKillRank.size(); i++)
	{
		CDBPrinceKillRank &cKillRank = m_vecPrinceKillRank[i];
		if (cKillRank.m_nPlayerID == idPlayer)
		{
			if (this->isHaveGetReward(cKillRank.m_strKillReward, prequest->m_nKillNum))
			{
				presponse->m_nRetCode = HVAE_GET_REWARD;
				return presponse.get();
			}
			char sBuff[127+1]={0};
			sprintf(sBuff, "%d", prequest->m_nKillNum);
			std::string strBuf = std::string(sBuff);
			if (cKillRank.m_strKillReward.size() < 1)
			{
				cKillRank.m_strKillReward = strBuf;
			}
			else
			{
				cKillRank.m_strKillReward += ",";
				cKillRank.m_strKillReward += strBuf;
			}
			break;
		}
	}
	presponse->m_nRetCode = SUCCESS;
	presponse->m_nPlayerID = prequest->m_nPlayerID;
	presponse->m_nKillNum = prequest->m_nKillNum;
	return presponse.get();
}
bool  CWorldWarMgr::isHaveGetReward(std::string &strRewardInf, uint16 nKillNum)
{
	std::vector<std::string> vecParam;
	splitString(strRewardInf, ",", vecParam, true);
	for (size_t i = 0; i < vecParam.size(); ++i)
	{
		uint16 haveGetNum = (uint16)(atoi(vecParam[i].data()));
		if (haveGetNum == nKillNum)
			return true;
	}
	return false;
}

//获取杀敌排名
IResponsePtr CWorldWarMgr::executeGetKillRank(IRequestPtr Request,CCurrentPtr Current)
{
	CGWMGetKillRankRequestPtr prequest = new CGWMGetKillRankRequest(Request->getSerialStream());
	CGWMGetKillRankResponsePtr presponse = new CGWMGetKillRankResponse();
	OBJID idPlayer = prequest->m_nPlayerID;
	std::map<OBJID, uint32>::iterator iter = m_mapPlayerKillNum.find(idPlayer);
	if (iter != m_mapPlayerKillNum.end())
	{
		presponse->m_nKillNum = (uint16)iter->second;
	}
    presponse->m_nTotNum = m_vecPrinceKillRank.size();
	uint32 nPageNum = (uint32)prequest->m_nPageNum;
	if (nPageNum * 6 > m_vecPrinceKillRank.size())
		nPageNum = 0;
	uint32 beginNum = nPageNum * 6;
	uint32 endNum = (nPageNum + 1) * 6;
	CPrinceKillRank cResKillRank;
	for (uint32 i = beginNum; i < m_vecPrinceKillRank.size() && i < endNum; i++)
	{
		CDBPrinceKillRank &cKillRank  = m_vecPrinceKillRank[i];
        cResKillRank.m_nPlayerID = cKillRank.m_nPlayerID;
		cResKillRank.m_nKillNum = cKillRank.m_nKillNum;
		cResKillRank.m_nLevel = cKillRank.m_nLevel;
		cResKillRank.m_nNation = cKillRank.m_nNation;
		cResKillRank.m_strName = cKillRank.m_strName;
		presponse->m_vecKillRank.push_back(cResKillRank);
	}
    for (size_t i = 0; i < 120 && i < m_vecPrinceKillRank.size(); i++)
	{
		CDBPrinceKillRank &cKillRank = m_vecPrinceKillRank[i];
		if (cKillRank.m_nPlayerID == idPlayer)
		{
			presponse->m_nSelfRank = i+1;
			break;
		}
	}
	presponse->m_nRetCode = SUCCESS;
	presponse->m_nPageNum = prequest->m_nPageNum;
	return presponse.get();
}

//退出国战场景
IResponsePtr CWorldWarMgr::executeExitWorldWar(IRequestPtr Request, CCurrentPtr Current)
{
	CGWMExitWorldWarRequestPtr prequest = new CGWMExitWorldWarRequest(Request->getSerialStream());

	m_mapEnterPlayer.erase(prequest->m_nPlayerID);

	return NULL;
}

//获取参战的人员
void CWorldWarMgr::getJoinPlayer(std::vector<OBJID> &vecPlayer)
{
	std::map<OBJID, uint8>::iterator iter = m_mapEnterPlayer.begin();
	for (; iter != m_mapEnterPlayer.end(); ++iter)
	{
		vecPlayer.push_back(iter->first);
	}
	return;
}

//退出城战
IResponsePtr CWorldWarMgr::executeExitFightCity(IRequestPtr Request, CCurrentPtr Current)
{
	CGWMExitFightCityRequestPtr prequest = new CGWMExitFightCityRequest(Request->getSerialStream());
	CGWMExitFightCityResponsePtr presponse = new CGWMExitFightCityResponse();

	uint8 idCity = prequest->m_nCityID;
	OBJID idPlayer = prequest->m_nPlayerID;

	//获取城战事件信息
	CWarCityPtr pWarCityPtr = this->getWarCityPtr(idCity);
	if (pWarCityPtr == NULL)
	{
		presponse->m_nRetCode = NO_ACC_CITY;
		return presponse.get();
	}

	pWarCityPtr->eraseJoinPlayer( prequest->m_nPlayerID);

	return NULL;
}

//获取城战杀敌榜
IResponsePtr CWorldWarMgr::executeQueryCityFightRank(IRequestPtr Request,CCurrentPtr Current)
{
	CGWMQueryCityFightRankRequestPtr prequest = new CGWMQueryCityFightRankRequest(Request->getSerialStream());
	CGWMQueryCityFightRankResponsePtr presponse = new CGWMQueryCityFightRankResponse();

	uint8 idCity = prequest->m_nCityID;
	presponse->m_nSession = prequest->m_nSession;
	OBJID idPlayer = prequest->m_nPlayerID;

	//获取城战事件信息
	CWarCityPtr pWarCityPtr = this->getWarCityPtr(idCity);
	if (pWarCityPtr == NULL)
	{
		presponse->m_nRetCode = NO_ACC_CITY;
		return presponse.get();
	}

	//获取个人杀敌信息
	CPlayerCityWarInfPtr pPlayerWarInfPtr = pWarCityPtr->getJoinPlayer(idPlayer);
	if (pPlayerWarInfPtr == NULL)
		presponse->m_nKillNum = 0;
	else
		presponse->m_nKillNum = pPlayerWarInfPtr->m_nKillNum;

	//判断session
	if(pWarCityPtr->getCurRankSession() == prequest->m_nSession)
	{
		presponse->m_nRetCode = SUCCESS;
		presponse->m_nSession = pWarCityPtr->getCurRankSession();
		return presponse.get();
	}

	presponse->m_nRetCode = SUCCESS;
	presponse->m_nSession = pWarCityPtr->getCurRankSession();
	pWarCityPtr->getCityKillRank(presponse->m_vecRank);
	return presponse.get();
}

//随机产生一个事件城池
uint8 CWorldWarMgr::getRandSpeAccCity()
{
	uint8 idAccCity = 0;
	if (m_nMaxSpeNpcRate == 0)
		return idAccCity;

	for (size_t j = 0; j < 100; j++)
	{
		uint32 radomvalues = CRandom::rand(m_nMaxSpeNpcRate+1);
		for (size_t i = 0; i < m_vecSpeCity.size(); i++)
		{
			CNationNpcRate &cNpcRate = m_vecSpeCity[i];
			uint8 idCity = cNpcRate.m_nCityID;
			if (cNpcRate.m_nBeginRate <= radomvalues && radomvalues <= cNpcRate.m_nEndRate)
			{
				std::map<uint8, CWarCityPtr>::iterator iter =  m_mapWarCityPtr.find(idCity);
				std::map<uint8, CProjectCityPtr>::iterator iter_pro = m_mapProCity.find(idCity);
				if (iter == m_mapWarCityPtr.end() && iter_pro == m_mapProCity.end())
				{
					idAccCity = idCity;
					return idAccCity;
				}
			}
		}
	}

	return idAccCity;
}

//随机产生一个事件城池
uint8 CWorldWarMgr::getRandAccCity(uint8 idNation)
{
	std::vector<uint8> vecCity;
	g_pConsummer->getAllNationCity(idNation, vecCity);
	uint32 idSize = vecCity.size();
	if (idSize == 0)
		return 0;

	if (m_mapWarCityPtr.size() == vecCity.size())
		return 0;

	uint8 idAccCity = 0;

	if (m_mapWarCityPtr.size() + 5 >= vecCity.size())
	{
		for (size_t i = 0; i < vecCity.size(); i++)
		{
			uint8 idCity = vecCity[i];
			std::map<uint8, CWarCityPtr>::iterator iter =  m_mapWarCityPtr.find(idCity);
			if (iter == m_mapWarCityPtr.end())
			{
				idAccCity = idCity;
				break;
			}
		}
	}
	else
	{
		for (size_t j = 0; j < 100; j++)
		{
			uint32 radomvalues = CRandom::rand(idSize);
			if (radomvalues == idSize)
				radomvalues = 0;
			uint8 idCity = vecCity[radomvalues];
			std::map<uint8, CWarCityPtr>::iterator iter =  m_mapWarCityPtr.find(idCity);
			std::map<uint8, CProjectCityPtr>::iterator iter_pro = m_mapProCity.find(idCity);
			if (iter == m_mapWarCityPtr.end() && iter_pro == m_mapProCity.end())
			{
				idAccCity = idCity;
				break;
			}
		}
	}

	return idAccCity;
}

//国家疆域产出处理
void CWorldWarMgr::getNationAdminCity(std::vector<CCityOccup> &vecCityOccup, std::map<uint8, std::vector<uint8> > &mapNationCity)
{
	//获取疆域分布
	std::map<uint8, uint8>::iterator iter_occup =  m_mapCityOccup.begin();
	for (; iter_occup != m_mapCityOccup.end(); ++iter_occup)
	{
		CCityOccup cCityOccup;
		cCityOccup.m_nCityID = iter_occup->first;
		cCityOccup.m_nNationID = iter_occup->second;
		vecCityOccup.push_back(cCityOccup);
	}

	//遍历城池，计算所有国家的城池产出奖励
	std::vector<uint8> vecCity;
	g_pConsummer->getAllWolrdCity(vecCity);
	for (size_t i = 0; i < vecCity.size(); i++)
	{
		uint8 idCity = vecCity[i];
		//判断城池ID是否合法
		CWorldCityPtr pCityPtr =  g_pConsummer->getWorldCityPtr(idCity);
		if (pCityPtr == NULL)
		    continue;

		uint8 idNation = pCityPtr->m_nNation;

		//判断城池归属是否有变化
		std::map<uint8, uint8>::iterator iter = m_mapCityOccup.find(idCity);
		if (iter != m_mapCityOccup.end())
			idNation = iter->second;
         
		//判断城池归属
		if (idNation != NATIONAL_WEI && idNation != NATIONAL_SHU && idNation != NATIONAL_WU)
			continue;
       
		//记录国家管辖的城池
		mapNationCity[idNation].push_back(idCity);
	}
}
void CWorldWarMgr::nationDomainReward(uint8 nAccFlag)
{
	CDateTime now;
	uint16 nHour = now.getHour();
	if (nAccFlag != 1)
	{
		if (nHour < m_nDomainBeginTime || nHour > m_nDomainEndTime)
			return;
	}
	if (nHour < m_nDomainBeginTime)
		nHour = m_nDomainBeginTime;
	if (nAccFlag == 1)
		nHour = m_nDomainBeginTime;
	CWarNationPtr cNationPtr = this->getWarNationPtr(NATIONAL_SHU);
	if (cNationPtr == NULL)
		return;
	if (cNationPtr->checkDomainReward(nHour))
		return;
	std::map<uint8, std::vector<uint8> > mapNationCity;
	std::vector<CCityOccup> vecCityOccup;
	this->getNationAdminCity(vecCityOccup, mapNationCity);
	//国家管辖城池产出计算
	std::map<uint8, std::vector<uint8> >::iterator iter_city = mapNationCity.begin();
	for (; iter_city != mapNationCity.end(); ++iter_city)
	{
		this->nationCityReward(iter_city->first, iter_city->second, vecCityOccup, nHour);
	}

	//登记数据库
	this->regNationDomainCity(mapNationCity, nHour);
    
	return;
}

//国家管辖城池产出
void CWorldWarMgr::nationCityReward(uint8 idNation, std::vector<uint8> &vecCity, std::vector<CCityOccup> &vecCityOccup, uint16 nHour)
{
	//获取国家信息
	CWarNationPtr cNationPtr = this->getWarNationPtr(idNation);
	if (cNationPtr == NULL)
		return;

	CDomainReward cDomainReward;
	cDomainReward.m_nTime = nHour;
	cDomainReward.m_vecCityOccup.assign(vecCityOccup.begin(), vecCityOccup.end());

	std::vector<CRewardInfo> vecReward;

	for (size_t i = 0; i < vecCity.size(); i++)
	{
		uint8 idCity = vecCity[i];
		//判断城池ID是否合法
		CWorldCityPtr pCityPtr =  g_pConsummer->getWorldCityPtr(idCity);
		if (pCityPtr == NULL)
			continue;

		for(size_t j = 0; j < pCityPtr->m_vecFixTimeReward.size(); j++)
			vecReward.push_back(pCityPtr->m_vecFixTimeReward[j]);
	}
    
	//特殊奖励
	std::map<uint16, std::vector<CRewardInfo> >::iterator iter_spe = m_mapSpeTimeReward.find(nHour);
	if (iter_spe != m_mapSpeTimeReward.end())
	{
		std::vector<CRewardInfo> &vecSpeReward = iter_spe->second;
		for (size_t i = 0; i < vecSpeReward.size(); i++)
			vecReward.push_back(vecSpeReward[i]);
	}

	g_pConsummer->arrangeAwardMoney(vecReward);

    cDomainReward.m_vecReward.assign(vecReward.begin(), vecReward.end());

	cNationPtr->m_vecDomainReward.push_back(cDomainReward);
	return;
}

//获取被占领的城池信息
std::string CWorldWarMgr::getOccupCity()
{
	char sBuf[1024]={0};
	std::map<uint8, uint8>::iterator iter = m_mapCityOccup.begin();
	for (; iter != m_mapCityOccup.end(); ++iter)
	{
		char sTmp[128]={0};
		sprintf(sTmp, "%d:%d;", iter->first, iter->second);
		strcat(sBuf, sTmp);
	}
    
	return std::string(sBuf);
}

//获取被占领的城池信息
void CWorldWarMgr::convOccupCity(std::string strCity, std::vector<CCityOccup> &vecOccupCity)
{
	std::vector<std::string> vecParam;
	splitString(strCity, ";", vecParam, true);
	CCityOccup cOccupCity;
	for (size_t i = 0; i < vecParam.size(); i++)
	{
		uint32 idCity;
		uint32 idNation;
		std::string strOccup = vecParam[i];
		if (sscanf(strOccup.c_str(), "%d:%d", &idCity, &idNation) != 2)
			continue;

		cOccupCity.m_nCityID = (uint8)idCity;
		cOccupCity.m_nNationID = (uint8)idNation;

        vecOccupCity.push_back(cOccupCity);
	}
    
	return;
}

//登记国家管辖城池
void CWorldWarMgr::regNationDomainCity(std::map<uint8, std::vector<uint8> > &mapNaitonDomain, uint16 nHour)
{
	if (nHour == 5)
	    return;
    
	std::string strOccupCity = this->getOccupCity();
	CDBRegNationDomainRequestPtr dbreq = new CDBRegNationDomainRequest();
	std::map<uint8, std::vector<uint8> >::iterator iter = mapNaitonDomain.begin();
	for (; iter != mapNaitonDomain.end(); ++iter)
	{
		CNationDomainCity cCity;
		cCity.m_nNation = iter->first;
		char sBuff[1024]={0};
		std::vector<uint8> &vecCity = iter->second;
		for (size_t i = 0; i < vecCity.size(); i++)
		{
			char sCity[32]={0};
			sprintf(sCity, "%d", vecCity[i]);
			if (i == 0)
			{
				strcpy(sBuff, sCity);
			}
			else
			{
				strcat(sBuff, ",");
				strcat(sBuff, sCity);
			}
		}

		cCity.m_strCity = std::string(sBuff);
		cCity.m_nHour = nHour;
		cCity.m_strOccupCity = strOccupCity;
        
		dbreq->m_vecCity.push_back(cCity);
	}

    g_DaServer->callServant(DB_REG_NATION_DOMAIN, dbreq.get(), NULL, 0, NULL, NULL);

	return;
}

//获取城池被占领的国家
void  CWorldWarMgr::getCityOccupNation(uint8 idCity, uint8 &idNation)
{
	std::map<uint8, uint8>::iterator iter =  m_mapCityOccup.find(idCity);
	if (iter == m_mapCityOccup .end())
		return;

	idNation = iter->second;
	return;
}

//返还玩家道具
void CWorldWarMgr::returnItem()
{
	m_vecGateWaySession.clear();
	g_pConsummer->getGateWaySession(m_vecGateWaySession);
	std::map<uint8, CWarCityPtr>::iterator iter =  m_mapWarCityPtr.begin();
	for (; iter != m_mapWarCityPtr.end(); ++iter)
	{
		CWarCityPtr pWarCityPtr = iter->second;
		if (pWarCityPtr == NULL)
			continue;
        
		//返还玩家道具
		pWarCityPtr->returnDeployedItem();
		//通知gateway 城池结束通知
		CGWWorldWarNoticeRequestPtr gwreq = new CGWWorldWarNoticeRequest();
		gwreq->m_nType = WORLD_WAR_FIGHT_END;
		gwreq->m_nAccID = pWarCityPtr->getAccID();
		gwreq->m_nCityID = pWarCityPtr->getCityID();
		pWarCityPtr->setStatus(3);
		gwreq->m_strData = this->getCityWarEndInf(pWarCityPtr);
		for( size_t i = 0; i < m_vecGateWaySession.size();i++ )
			g_DaServer->send(m_vecGateWaySession[i], GW_WORLD_WAR_NOTICE, gwreq.get());
	}

	m_mapWarCityPtr.clear();
    
	return;
}

void CWorldWarMgr::getSpeNationPrinceAccCity(uint8 idNation, std::vector<uint8> &vecEliteCity, std::vector<uint8> &vecNorCity)
{
	for (size_t i = 0; i < m_vecSpeCity.size(); i++)
	{
		CNationNpcRate &cNpcRate = m_vecSpeCity[i];
		uint8 idCity = cNpcRate.m_nCityID;
		CWarCityPtr pWarCityPtr = this->getWarCityPtr(idCity);
		if (pWarCityPtr != NULL)
			continue;
		vecEliteCity.push_back(idCity);
	}
	return;
}

//获取阵营管辖的城池
void CWorldWarMgr::getNationOccupCity(uint8 idNation, std::vector<uint8> &vecCity)
{
     std::vector<uint8> vecAllCity;
	 g_pConsummer->getAllWolrdCity(vecAllCity);
	 for (size_t i = 0; i < vecAllCity.size(); i++)
	 {
		 uint8 idCity = vecAllCity[i];
		 CWorldCityPtr pWorldCityPtr  = g_pConsummer->getWorldCityPtr(idCity);
		 if (pWorldCityPtr ==  NULL)
			 continue;
		 //获取城池被占领的国家
		 uint8 idOccupNation = pWorldCityPtr->m_nNation;
		 this->getCityOccupNation(idCity, idOccupNation);
		 if (idOccupNation == idNation)
			 vecCity.push_back(idCity);
	 }

	 return;
}

void CWorldWarMgr::getNationPrinceAccCity(uint8 idNation, uint8 nCityNum,std::vector<uint8> &vecEliteCity, std::vector<uint8> &vecNorCity)
{
	//获取国家的管辖城池
	std::vector<uint8>  vecAdimCity;
	this->getNationOccupCity(idNation, vecAdimCity);

	for(size_t i = 0; i < vecAdimCity.size(); i++)
	{
		uint8 idCity = vecAdimCity[i];
		CWorldCityPtr pWorldCityPtr  = g_pConsummer->getWorldCityPtr(idCity);
		if (pWorldCityPtr ==  NULL)
			continue;

		if (pWorldCityPtr->m_nCityType == CITY_TYPE_CAPITAL)
			continue;
	
		bool isNorCity = true;
		for (size_t j = 0; j < pWorldCityPtr->m_vecCity.size(); j++)
		{
			isNorCity = false;
			uint8 idNearCity = pWorldCityPtr->m_vecCity[j];
			CWarCityPtr pWarCityPtr = this->getWarCityPtr(idNearCity);
			if (pWarCityPtr != NULL)
				continue;
			std::map<uint8, uint8>::iterator iter =  m_mapCityOccup.find(idNearCity);
			if (iter != m_mapCityOccup.end())
			{
				uint8 idNearNation = iter->second;
				if (idNearNation != idNation)
				{
					vecEliteCity.push_back(idCity);
					isNorCity = true;
					break;
				}
			}
			else
			{
				CWorldCityPtr pWorldCityPtr  = g_pConsummer->getWorldCityPtr(idNearCity);
				if (pWorldCityPtr ==  NULL)
					continue;
				if (pWorldCityPtr->m_nCityType == CITY_TYPE_CAPITAL)
					continue;
				if (pWorldCityPtr->m_nNation != idNation)
				{
					vecEliteCity.push_back(idCity);
					isNorCity = true;
					break;
				}
			}
		}
		if(!isNorCity)
			vecNorCity.push_back(idCity);
	}
	return;
}
uint8 CWorldWarMgr::getRandPrinceAccCity(std::vector<uint8> &vecBorCity, std::vector<uint8> &vecNorCity)
{
	size_t numCity = vecBorCity.size();
	for (size_t i = 0; i < numCity; i++)
	{
		size_t radomvalues = CRandom::rand(numCity);
		if (radomvalues == numCity)
			radomvalues = 0;
		uint8 idCity = vecBorCity[radomvalues];
		CWarCityPtr pWarCityPtr = this->getWarCityPtr(idCity);
		if (pWarCityPtr == NULL)
		{
			std::vector<uint8> vecCity;
            vecCity.assign(vecBorCity.begin(), vecBorCity.end());
			vecBorCity.clear();
			for (size_t j = 0; j < vecCity.size(); j++)
			{
				uint8 ideraseCity = vecCity[j];
				if (ideraseCity != idCity)
					vecBorCity.push_back(ideraseCity);
			}
			return idCity;
		}
	}
	numCity = vecNorCity.size();
	for (size_t i = 0; i < numCity; i++)
	{
		size_t radomvalues = CRandom::rand(numCity);
		if (radomvalues == numCity)
			radomvalues = 0;
		uint8 idCity = vecNorCity[i];
		CWarCityPtr pWarCityPtr = this->getWarCityPtr(idCity);
		if (pWarCityPtr == NULL)
		{
			std::vector<uint8> vecCity;
			vecCity.assign(vecNorCity.begin(), vecNorCity.end());
			vecNorCity.clear();
			for (size_t j = 0; j < vecCity.size(); j++)
			{
				uint8 ideraseCity = vecCity[j];
				if (ideraseCity != idCity)
					vecNorCity.push_back(ideraseCity);
			}
			return idCity;
		}
	}
	return 0;
}
void CWorldWarMgr::initPrinceAcc(uint8 idNation, uint8 accNum, bool isElite)
{
	uint8 idBlowNation = idNation;
	std::vector<uint8> vecBorCity; //边境城池
	std::vector<uint8> vecNorCity; //普通城池
	CPrinceCityAcc cCityAcc;
	if (NATIONAL_SPE == idNation)
		this->getSpeNationPrinceAccCity(idNation, vecBorCity, vecNorCity);
	else
	    this->getNationPrinceAccCity(idNation, accNum, vecBorCity, vecNorCity);
	if (vecBorCity.size() == 0 && vecNorCity.size() == 0)
		return;
	CDBRegCityAccRequestPtr dbreq = new CDBRegCityAccRequest();
	for (size_t i = 0; i < accNum; i++)
	{
		for (size_t j = PRINCE_ACC_ID_BEGIN; j < PRINCE_ACC_ID_END; j++)
		{
			uint8 idAcc = (uint8)j;
			const Json::Value& cCityAccConf = g_pConsummer->cityaccConf(idAcc);
			if (cCityAccConf == Json::nullValue)
				continue;

			//判断是否已经有该事件
			CWarCityPtr pWarPtr = this->getWarCityByAccID(idAcc);
			if (pWarPtr != NULL)
				continue;

			std::string strbeginTime = cCityAccConf["attack_time"].asString();
			uint8 idNpcNation = (uint8)cCityAccConf["npc_nation"].asUInt();
			uint8 nAccType = (uint8)cCityAccConf["acc_type"].asUInt();
			if (isElite)
			{
				if (nAccType != 2)
					continue;
			}
			else
			{
				if (nAccType != 1)
					continue;
			}

			uint8 idAccCity = 0;
			idAccCity =	this->getRandPrinceAccCity(vecBorCity, vecNorCity);
			if(idAccCity == 0)
				return;
			CWorldCityPtr pWorldCityPtr = g_pConsummer->getWorldCityPtr(idAccCity);
			if (pWorldCityPtr == NULL)
				return;
			CWarCityPtr pWarCityPtr = new CWarCity();
			CDateTime now;
			int nHour = 0;
			int nMinute = 0;
			int nYear = now.getYear();
			int nMonth = now.getMonth();
			int nDay = now.getDay();
			int nSecond = now.getSecond();
			sscanf(strbeginTime.c_str(), "%d:%d", &nHour, &nMinute) ;
			now.init(nYear, nMonth, nDay, nHour, nMinute, nSecond, 0);
			uint32 begintime = now.getTotalSecs();
			pWarCityPtr->init(idAcc, idAccCity, begintime);
			this->initAccCityNpcTeam(pWarCityPtr);
			pWarCityPtr->setCityName(cCityAccConf["name"].asString());
			pWarCityPtr->setDefSuccReward(m_vecDefSuccReward);
			pWarCityPtr->setDefFailReward(m_vecDefFailRewad);
			pWarCityPtr->setConsummer(g_pConsummer);
			pWarCityPtr->setDepoyedItem(m_nDeployedItem);
			pWarCityPtr->setDepoyedItemNum(m_nDepoyedItemNum);
			pWarCityPtr->setTeamListLmt(m_nTeamListLmt);
			if (NATIONAL_SPE == idNation)
			{
				idBlowNation = pWorldCityPtr->m_nNation;
				this->getCityOccupNation(idAccCity, idBlowNation);
			}

			log_debug(" idBlowNation  %d", idBlowNation);
			pWarCityPtr->setNation(idBlowNation);
			pWarCityPtr->setAttNation(idNpcNation); 
			pWarCityPtr->setItemRewardExp(m_mapItemRewardExp1);
			cCityAcc.m_nAccID = idAcc;
			cCityAcc.m_nCityID = idAccCity;
			m_vecPrinceAcc.push_back(cCityAcc);
			dbreq->m_nAccID = idAcc;
			dbreq->m_nCityID = idAccCity;
			g_DaServer->callServant(DB_REG_CITY_ACC, dbreq.get(), NULL, 0, NULL, NULL);
			m_mapWarCityPtr.insert(make_pair(idAccCity, pWarCityPtr));
			break;
		}
	}

	return;
}
void CWorldWarMgr::princeAccProc(uint8 nType)
{
	if( !g_pConsummer->isFunctionPermit(EFUNCTIONID_PRINCE_WAR) )  
		return;

	if (!g_pConsummer->isFuncAccDay(6))
		return;

	m_nPrinceAccFlag = 1;

	std::vector<CPrinceCityAcc> vecAcc;
	uint8 nNoticeType = WORLD_WAR_FIGHT_BEGIN;
	uint8 nPrinceNoticeType = WORLD_PRINCE_PRE_FIGHT;
	CGWWorldWarNoticeRequestPtr gwreq = new CGWWorldWarNoticeRequest();
	switch (nType)
	{
	case EWORLD_NOTICE_TYPE_PRINCE_ACC_PRE: //群雄逐鹿预告
		{
			nNoticeType = WORLD_WAR_PRE_FIGHT;
			nPrinceNoticeType = WORLD_PRINCE_PRE_FIGHT;
			m_vecPrinceAcc.clear();
			uint16 nCityNum  = (uint16)g_pConsummer->getWorldCityNum();
			if (nCityNum < 4)
				return;
			nCityNum = nCityNum - 4;
			uint16 nShuCityNum = (uint16)(this->getNaitonAdminCityNum(NATIONAL_SHU));
			uint16 nWeiCityNum = (uint16)(this->getNaitonAdminCityNum(NATIONAL_WEI));
			uint16 nWuCityNum = (uint16)(this->getNaitonAdminCityNum(NATIONAL_WU));

            log_debug(" nWuCityNum  %d nShuCityNum %d nWeiCityNum %d ", nWuCityNum, nShuCityNum, nWeiCityNum);
			uint8 nShuAttEliteNum = (uint8)(6 * (nShuCityNum-1)/nCityNum);
			uint8 nWeiAttEliteNum = (uint8)(6 * (nWeiCityNum-1)/nCityNum);
			uint8 nWuAttEliteNum =  (uint8)(6 * (nWuCityNum-1)/nCityNum);

			log_debug(" nWuAttEliteNum  %d nShuAttEliteNum %d nWeiAttEliteNum %d ", nWuAttEliteNum, nShuAttEliteNum, nWeiAttEliteNum);
			this->initPrinceAcc(NATIONAL_WEI, nWeiAttEliteNum, true);
			this->initPrinceAcc(NATIONAL_SHU, nShuAttEliteNum, true);
			this->initPrinceAcc(NATIONAL_WU,  nWuAttEliteNum, true);
			uint8 otherEliteNum = 6 - nShuAttEliteNum - nWeiAttEliteNum - nWuAttEliteNum;
			if (otherEliteNum != 0)
			{
				this->initPrinceAcc(NATIONAL_SPE,  otherEliteNum, true);
			}
			uint8 nShuAttNorNum = (uint8) (12 * (nShuCityNum-1)/nCityNum);
			uint8 nWeiAttNorNum = (uint8) (12 * (nWeiCityNum-1)/nCityNum);
			uint8 nWuAttNorNum = (uint8) (12 * (nWuCityNum-1)/nCityNum);

			log_debug(" nWuAttNorNum  %d nShuAttNorNum %d nWeiAttNorNum %d ", nWuAttNorNum, nShuAttNorNum, nWeiAttNorNum);
			this->initPrinceAcc(NATIONAL_WEI, nWeiAttNorNum, false);
			this->initPrinceAcc(NATIONAL_SHU, nShuAttNorNum, false);
			this->initPrinceAcc(NATIONAL_WU,  nWuAttNorNum,  false);
			uint8 otherNorNum = 12 - nShuAttNorNum - nWeiAttNorNum - nWuAttNorNum;
			if (otherNorNum != 0)
			{
				this->initPrinceAcc(NATIONAL_SPE,  otherNorNum, false);
			}
			vecAcc.assign(m_vecPrinceAcc.begin(), m_vecPrinceAcc.end());

			//发送公告
			std::string strNotice = g_pConsummer->formatLangString(LANGTYPE_PRINCE_WAR, E_LANGTYPE_PRINCE_WAR_PRE_FIGHT);
			sendSystemNotice(CHANNEL_MARQUEE, strNotice, NULL, 26);
		}
		break;
	case EWORLD_NOTICE_TYPE_PRINCE_ACC_BEGIN: //群雄逐鹿开始
		{
			nNoticeType = WORLD_WAR_FIGHT_BEGIN;
			nPrinceNoticeType = WORLD_PRINCE_FIGHT_BEGIN;
			for (size_t i = 0; i < m_vecPrinceAcc.size(); i++)
			{
				CPrinceCityAcc &cCityAcc = m_vecPrinceAcc[i];
				uint8 idAcc = cCityAcc.m_nAccID;
				CWarCityPtr pWarCityPtr = this->getWarCityByAccID(idAcc);
				if (pWarCityPtr == NULL)
					continue;
				pWarCityPtr->setStatus(1);
				while(pWarCityPtr->addNewTeamToAttFightQueue())
					;
				while (pWarCityPtr->addNewTeamToBlowFightQueue())
					;
				CDateTime now;
				pWarCityPtr->setWheelFightTime(now.getTotalSecs());
				vecAcc.push_back(cCityAcc);
			}

			//发送公告
			std::string strNotice = g_pConsummer->formatLangString(LANGTYPE_PRINCE_WAR, E_LANGTYPE_PRINCE_WAR_BEGIN_FIGHT);
			sendSystemNotice(CHANNEL_MARQUEE, strNotice, NULL, 26);
		}
		break;
	case EWORLD_NOTICE_TYPE_PRINCE_ACC_END: //群雄逐鹿结束
		{
			nNoticeType = WORLD_WAR_FIGHT_END;
			nPrinceNoticeType = WORLD_PRINCE_FIGHT_END;
			for (size_t i = 0; i < m_vecPrinceAcc.size(); i++)
			{
				CPrinceCityAcc &cCityAcc = m_vecPrinceAcc[i];
				cCityAcc.m_nStatus = 2;
				uint8 idAcc = cCityAcc.m_nAccID;
				uint8 idCity = cCityAcc.m_nCityID;
				CWarCityPtr pWarCityPtr = this->getWarCityByAccID(idAcc);
				if (pWarCityPtr == NULL)
					continue;
				pWarCityPtr->setStatus(2); //防守成功
				this->cityKillRankReward(pWarCityPtr);
				pWarCityPtr->cityPlayerRewardAcc();
				pWarCityPtr->accEndRebackDeployItem();
				vecAcc.push_back(cCityAcc);
			}
            
			//发送公告
			std::string strNotice = g_pConsummer->formatLangString(LANGTYPE_PRINCE_WAR, E_LANGTYPE_PRINCE_WAR_END_FIGHT);
			sendSystemNotice(CHANNEL_MARQUEE, strNotice, NULL, 26);
		}
		break;
	default:
		return ;
	}
	m_vecGateWaySession.clear();
	g_pConsummer->getGateWaySession(m_vecGateWaySession);
	CDBDelAccInfRequestPtr dbreq = new CDBDelAccInfRequest();
	for (size_t j = 0; j < vecAcc.size(); j++)
	{
		CPrinceCityAcc &cCityAcc = vecAcc[j];
		gwreq->m_nType = nNoticeType;
		gwreq->m_nAccID =  cCityAcc.m_nAccID;
		gwreq->m_nCityID = cCityAcc.m_nCityID;

		char sBuf[128] = {0};
		sprintf(sBuf, "%d", gwreq->m_nCityID);
		gwreq->m_strData = std::string(sBuf);

		if (nNoticeType == WORLD_WAR_FIGHT_END)
		{
			CWarCityPtr pWarCityPtr = this->getWarCityByAccID(gwreq->m_nAccID);
			if (pWarCityPtr == NULL)
				continue;
			gwreq->m_strData = this->getCityWarEndInf(pWarCityPtr);
			m_mapWarCityPtr.erase(cCityAcc.m_nCityID);
			this->clearWarPlayerTeam(cCityAcc.m_nCityID);
			dbreq->m_nAccID = gwreq->m_nAccID;
			g_DaServer->callServant(DB_DEL_ACC_INF, dbreq.get(), NULL, 0, NULL, NULL);
		}

		for( size_t i = 0; i < m_vecGateWaySession.size();i++ )
		    g_DaServer->send(m_vecGateWaySession[i], GW_WORLD_WAR_NOTICE, gwreq.get());
	}
	gwreq->m_nType = nPrinceNoticeType;
	gwreq->m_nAccID =  0;
	gwreq->m_nCityID = 0;
	for( size_t i = 0; i < m_vecGateWaySession.size();i++ )
		g_DaServer->send(m_vecGateWaySession[i], GW_WORLD_WAR_NOTICE, gwreq.get());
	return;
}

//兵站清理
void CWorldWarMgr::etapeClear(uint8 idNation)
{

	//获取城战国家信息
	CWarNationPtr pWarNationPtr = this->getWarNationPtr(idNation);
	if (pWarNationPtr == NULL)
	{
		return;
	}
    
	std::vector<CEtapeTeam> vecEtapeTeam;
	pWarNationPtr->getEtapeTeamList(vecEtapeTeam);

	if (vecEtapeTeam.size() == 0)
		return;


	OBJID  idPlayer = 0;
	std::map<OBJID, uint16> mapPlayerTeam;
	for (size_t i = 0; i < vecEtapeTeam.size(); i++)
	{
		CEtapeTeam &cEtapeTeam = vecEtapeTeam[i];
		idPlayer = cEtapeTeam.m_nPlayerID;
		mapPlayerTeam[idPlayer] = mapPlayerTeam[idPlayer] + 1;
	}

	pWarNationPtr->clearEtapeTeam();

	std::map<OBJID, uint16>::iterator iter =  mapPlayerTeam.begin();
	std::vector<CRewardInfo> vecReward;
	for (; iter != mapPlayerTeam.end(); ++iter)
	{
		idPlayer = iter->first;
		CRewardInfo cReward;
		cReward.m_nType = REWARDTYPE_ITEM;
		cReward.m_nValue = m_nDecDeployItem;
		cReward.m_nNum = iter->second;
		vecReward.clear();
		vecReward.push_back(cReward);
		//发送邮件奖励
		std::string strTitle = g_pConsummer->formatLangString(LANGTYPE_WORLD_WAR, LANGTYPE_WORLD_WAR_DEPART_TITLET);
		std::string sContent = g_pConsummer->formatLangString(LANGTYPE_WORLD_WAR, LANGTYPE_WORLD_WAR_DEPART_CONTENT);
		g_pConsummer->sendMailToPlayer(idPlayer, strTitle, sContent, vecReward, ITEMOPT_CITY_DEPART_RETURN);
	}

}


//校验是否是盟战时间
bool CWorldWarMgr::isAllyDay()
{
	uint32 nOpenGameDay = g_pConsummer->getOpenGameDate();

	CDateTime now;
	uint32 nCurDay = fmtDT2YYYYMMDD(now);
	uint32 iterDay = CountDayInterval(nOpenGameDay, nCurDay);
	if (iterDay < m_nAllyDay)
		return false;

	if (iterDay == m_nAllyDay)
		return true;

	uint32 nAllyCycle = CountDate(nOpenGameDay, m_nAllyDay);

	iterDay = CountDayInterval(nAllyCycle, nCurDay);
	if (iterDay == 0)
		return false;

	uint32 nOffDay = (iterDay+1)%m_nAllyCycle;
	if (nOffDay == 0)
		return true;

	return false;
}

//结盟处理
void CWorldWarMgr::allyProc()
{
	//判断是否是结盟时间
	if (!this->isAllyDay())
	    return;

	bool bSignNum = false;
	bool bTotNum = false;

	//判断是否满足结盟条件
	uint16 nWei = (uint16)(this->getNaitonAdminCityNum(NATIONAL_WEI));
	uint16 nShu = (uint16)(this->getNaitonAdminCityNum(NATIONAL_SHU));
	uint16 nWu = (uint16)(this->getNaitonAdminCityNum(NATIONAL_WU));
	if (nWei < 15 || nShu < 15 || nWu < 15)
		bSignNum = true;

	if (nWei + nShu + nWei >= 70)
		bTotNum = true;

	CGWWorldWarNoticeRequestPtr gwreq = new CGWWorldWarNoticeRequest();
	gwreq->m_nType = WORLD_ALLY_NATION;
	gwreq->m_nCityID = 0;
	m_vecGateWaySession.clear();
	g_pConsummer->getGateWaySession(m_vecGateWaySession);
	CDateTime now;
	uint32 nAllyEndTime = now.getTotalSecs() + m_nAllyCycle * 24 * 60 * 60;
	char sBuf[128] = {0};
	sprintf(sBuf, "%d", nAllyEndTime);

	if (bSignNum && bTotNum)
	{
		//获取最强国
		m_nAllyNation = NATIONAL_WEI;
		if (nShu > nWei)
		{
			m_nAllyNation = NATIONAL_SHU;
			if (nWu > nShu)
				m_nAllyNation = NATIONAL_WU;
		}
		else
		{
			if (nWu > nWei)
				m_nAllyNation = NATIONAL_WU;
		}

		//更新数据库

		//通知gateway
		gwreq->m_nAccID = m_nAllyNation;
		gwreq->m_strData = std::string(sBuf);

		for( size_t i = 0; i < m_vecGateWaySession.size();i++ )
			g_DaServer->send(m_vecGateWaySession[i], GW_WORLD_WAR_NOTICE, gwreq.get());
	}
	else
	{
		//判断之前是否有盟国
		if (m_nAllyNation != NATIONAL_NONE)
		{
			m_nAllyNation = NATIONAL_NONE;
			//更新数据库

			//通知gateway
			gwreq->m_nAccID = m_nAllyNation;
			for( size_t i = 0; i < m_vecGateWaySession.size();i++ )
				g_DaServer->send(m_vecGateWaySession[i], GW_WORLD_WAR_NOTICE, gwreq.get());
		}
	}
    
	return;
}


//国战通知
IResponsePtr CWorldWarMgr::executeGWMWorldWarNotice(IRequestPtr Request, CCurrentPtr Current)
{
	CGWMWorldWarNoticeRequestPtr prequest = new CGWMWorldWarNoticeRequest(Request->getSerialStream());

	log_debug(" executeGWMWorldWarNotice accid %d type %d", prequest->m_nAccID, prequest->m_nType);

	if( prequest->m_nType == EWORLD_NOTICE_TYPE_EVENT) //民生民情事件更新
	{
		CGWWorldWarNoticeRequestPtr gwreq = new CGWWorldWarNoticeRequest();
		gwreq->m_nType = WORLD_EVENT_NOTICE;

		//通知gateway
		m_vecGateWaySession.clear();
		g_pConsummer->getGateWaySession(m_vecGateWaySession);
		for( size_t i = 0; i < m_vecGateWaySession.size();i++ )
			g_DaServer->send(m_vecGateWaySession[i], GW_WORLD_WAR_NOTICE, gwreq.get());

		return NULL;
	}
    
	//兵站清理
	if (prequest->m_nType == EWORLD_NOTICE_TYPE_ETAPE_CLEAR)
	{
		this->etapeClear(NATIONAL_WEI);
		this->etapeClear(NATIONAL_SHU);
		this->etapeClear(NATIONAL_WU);

		return NULL;
	}
    
	//玩家攻城战事件
	if (prequest->m_nType == EWORLD_NOTICE_TYPE_CITY_FIGHT)
	{
		if( !g_pConsummer->isFunctionPermit(EFUNCTIONID_DECLARE_WAR) )
			return NULL;
        
		//进行国家疆域产出奖励处理
		this->nationDomainReward(0);
		return NULL;
	}
    
	//返还虎符
	if (prequest->m_nType == EWORLD_NOTICE_TYPE_RETURN_ITEM)
	{
		this->returnItem();
		this->regKillRankDB();
		return NULL;
	}

	if (prequest->m_nType == EWORLD_NOTICE_TYPE_PRINCE_ACC_PRE ||
		prequest->m_nType == EWORLD_NOTICE_TYPE_PRINCE_ACC_BEGIN || 
		prequest->m_nType == EWORLD_NOTICE_TYPE_PRINCE_ACC_END)
	{
		this->princeAccProc(prequest->m_nType);
		return NULL;
	}
    
	//结盟处理
	if (prequest->m_nType == EWORLD_NOTICE_TYPE_ALLY )
	{
		//结盟处理
		//this->allyProc();
        return NULL;
	}
    
	//npc攻城战后台开关
	if( !g_pConsummer->isFunctionPermit(EFUNCTIONID_NPC_WAR) )  
		return NULL;

	CGWWorldWarNoticeRequestPtr gwreq = new CGWWorldWarNoticeRequest();
	gwreq->m_nType = prequest->m_nType;
	gwreq->m_nAccID = prequest->m_nAccID;

	switch (prequest->m_nType)
	{
	case WORLD_WAR_PRE_FIGHT: //事件预告
		{
			//选择事件的城池
			const Json::Value& cCityAccConf = g_pConsummer->cityaccConf(prequest->m_nAccID);
			if (cCityAccConf == Json::nullValue)
				return NULL;

			uint8 AccType = (uint8)cCityAccConf["acc_type"].asUInt();
			if (AccType == 3)
			{
				if (!this->isAllyDay())
					return NULL;
			}

			std::string strbeginTime = cCityAccConf["attack_time"].asString();
			uint8 idNation = (uint8)cCityAccConf["attack_nation"].asUInt();
			
			
			//选择一个城池
			uint8 idAccCity = 0;
			if (idNation != NATIONAL_SPE)
			    idAccCity =	this->getRandAccCity(idNation);
			else
                idAccCity =	this->getRandSpeAccCity();
			if(idAccCity == 0)
				return NULL;
            
			//获取城池信息
			CWorldCityPtr pWorldCityPtr = g_pConsummer->getWorldCityPtr(idAccCity);
		    if (pWorldCityPtr == NULL)
				return NULL;

			
			//事件初始化
			CWarCityPtr pWarCityPtr = new CWarCity();
			
			CDateTime now;
			int nHour = 0;
			int nMinute = 0;
			
			int nYear = now.getYear();
			int nMonth = now.getMonth();
			int nDay = now.getDay();
			int nSecond = now.getSecond();

			sscanf(strbeginTime.c_str(), "%d:%d", &nHour, &nMinute) ;
			now.init(nYear, nMonth, nDay, nHour, nMinute, nSecond, 0);
			uint32 begintime = now.getTotalSecs();

			uint8 nNation = pWorldCityPtr->m_nNation;
			this->getCityOccupNation(idAccCity, nNation);

			pWarCityPtr->init(prequest->m_nAccID, idAccCity, begintime);
			this->initAccCityNpcTeam(pWarCityPtr);
			pWarCityPtr->setCityName(cCityAccConf["name"].asString());
			pWarCityPtr->setDefSuccReward(m_vecDefSuccReward);
			pWarCityPtr->setDefFailReward(m_vecDefFailRewad);
			pWarCityPtr->setConsummer(g_pConsummer);
			pWarCityPtr->setDepoyedItem(m_nDeployedItem);
			pWarCityPtr->setDepoyedItemNum(m_nDepoyedItemNum);
			pWarCityPtr->setTeamListLmt(m_nTeamListLmt);
			pWarCityPtr->setNation(nNation);
			pWarCityPtr->setAttNation(0); //代表群雄
			pWarCityPtr->setItemRewardExp(m_mapItemRewardExp1);
			if (AccType == 3)
			{
				pWarCityPtr->setAccType(ACC_TYPE_ALLY);
				m_mapAllyAcc[prequest->m_nAccID] = idAccCity;
			}

			m_mapWarCityPtr.insert(make_pair(idAccCity, pWarCityPtr));


			gwreq->m_nCityID = idAccCity;
			char sBuf[128] = {0};
			sprintf(sBuf, "%d", idAccCity);
			gwreq->m_strData = std::string(sBuf);
			std::string strAttNation = g_pConsummer->formatLangString(LANGTYPE_NATION_NAME, NATIONAL_NONE);
			std::string strAdminNation = g_pConsummer->formatLangString(LANGTYPE_NATION_NAME, nNation);
			std::string strNotice = g_pConsummer->formatLangString(LANGTYPE_WORLD_WAR, LANGTYPE_WORLD_WAR_NPC_WAR_PRE, 
				strAttNation.data(), strAdminNation.data());
			CBytesBufferPtr buf = CBytesBufferPtr::createInstance();
			pushChatCityInfo(buf, idAccCity);
			sendSystemNotice(CHANNEL_MARQUEE, strNotice, buf, 26);
			sendSystemNotice(CHANNEL_WORLD, strNotice, buf, 26);
		}
		break;
	case WORLD_WAR_FIGHT_BEGIN:  //事件开始
		{
			//判断事件是否存在
			CWarCityPtr pWarCityPtr = this->getWarCityByAccID(prequest->m_nAccID);
			if (pWarCityPtr == NULL)
				return NULL;

			gwreq->m_nCityID = pWarCityPtr->getCityID();
			pWarCityPtr->setStatus(1);

			//从大本营增加作战队伍
			while(pWarCityPtr->addNewTeamToAttFightQueue())
				;
			while (pWarCityPtr->addNewTeamToBlowFightQueue())
			   ;
            
			CDateTime now;
			pWarCityPtr->setWheelFightTime(now.getTotalSecs());
			std::string strAttNation = g_pConsummer->formatLangString(LANGTYPE_NATION_NAME, NATIONAL_NONE);
			std::string strAdminNation = g_pConsummer->formatLangString(LANGTYPE_NATION_NAME, pWarCityPtr->getNation());
			std::string strNotice = g_pConsummer->formatLangString(LANGTYPE_WORLD_WAR, LANGTYPE_WORLD_WAR_NPC_WAR_FIGHT, 
				strAttNation.data(), strAdminNation.data());
			CBytesBufferPtr buf = CBytesBufferPtr::createInstance();
			pushChatCityInfo(buf, pWarCityPtr->getCityID());
			sendSystemNotice(CHANNEL_MARQUEE, strNotice, buf, 26);
			sendSystemNotice(CHANNEL_WORLD, strNotice, buf, 26);
			if (pWarCityPtr->getNation() >= NATIONAL_WEI && pWarCityPtr->getNation()  <= NATIONAL_WU)
			     sendNationNotice(pWarCityPtr->getNation(), strNotice, buf);
		}
		break;
	case WORLD_WAR_FIGHT_END: //事件结束
		{
			//判断事件是否存在
			CWarCityPtr pWarCityPtr = this->getWarCityByAccID(prequest->m_nAccID);
			if (pWarCityPtr == NULL)
				return NULL;

			pWarCityPtr->setStatus(2); //防守成功
			gwreq->m_nCityID = pWarCityPtr->getCityID();

			//杀敌排名奖励
			this->cityKillRankReward(pWarCityPtr);

			//城战结束奖励
			pWarCityPtr->cityPlayerRewardAcc();
            
			//对驻兵进行道具返还
			pWarCityPtr->accEndRebackDeployItem();

			gwreq->m_strData = this->getCityWarEndInf(pWarCityPtr);

			//删除事件信息
			m_mapWarCityPtr.erase(gwreq->m_nCityID);
            
			//清理事件队伍信息
			this->clearWarPlayerTeam(gwreq->m_nCityID);
		}

		break;
	default:
		return NULL;
	}

	//通知gateway
	m_vecGateWaySession.clear();
	g_pConsummer->getGateWaySession(m_vecGateWaySession);
	for( size_t i = 0; i < m_vecGateWaySession.size();i++ )
		g_DaServer->send(m_vecGateWaySession[i], GW_WORLD_WAR_NOTICE, gwreq.get());
   
    return NULL;
}

//弱国增加禁卫军
void  CWorldWarMgr::weakNationAddGuard(uint8 idNation)
{
	//判断是否满足结盟条件
	uint8 adminNum = this->getNaitonAdminCityNum(idNation);
	if (adminNum > m_nWeakNationCityNum || adminNum == 0)
		return;

	//获取国家信息
	CWarNationPtr pWarNationPtr = this->getWarNationPtr(idNation);
	if (pWarNationPtr == NULL)
	    return;

	//判断兵站的队伍数量



	return;
}

//根据排名获取奖励信息
void CWorldWarMgr::getKillRankReward(uint8 accType, uint16 nRank, std::vector<CRewardInfo> &vecReward)
{
	if (accType == 0)
{
	for (size_t i = 0; i < m_vecRankReward.size(); i++)
	{
		CCityRankReward &cReward = m_vecRankReward[i];
		if (cReward.m_nBeginRank <= nRank && nRank <= cReward.m_nEndRank)
		{
			vecReward.assign(cReward.m_vecReward.begin(), cReward.m_vecReward.end());
			return;
			}
		}
	}
	else if (accType == 1)
	{
		for (size_t i = 0; i < m_vecRankReward1.size(); i++)
		{
			CCityRankReward &cReward = m_vecRankReward1[i];
			if (cReward.m_nBeginRank <= nRank && nRank <= cReward.m_nEndRank)
			{
				vecReward.assign(cReward.m_vecReward.begin(), cReward.m_vecReward.end());
				return;
			}
		}
	}
	else
	{
		for (size_t i = 0; i < m_vecPrinceRankReward.size(); i++)
		{
			CCityRankReward &cReward = m_vecPrinceRankReward[i];
			if (cReward.m_nBeginRank <= nRank && nRank <= cReward.m_nEndRank)
			{
				vecReward.assign(cReward.m_vecReward.begin(), cReward.m_vecReward.end());
				return;
			}
		}
	}

	return;
}


//城战结束杀敌排名奖励
void CWorldWarMgr::cityKillRankReward(CWarCityPtr pWarCityPtr)
{
	uint8 idCity = pWarCityPtr->getCityID();
	std::string strCityName = pWarCityPtr->getCityName();
	std::vector<CWarCityRank>  vecCityRank;
	pWarCityPtr->getKillRank(vecCityRank);
	for (size_t i = 0; i < vecCityRank.size(); i++)
	{
		CWarCityRank &cityRank = vecCityRank[i];
		uint16 nkillRank = i+1;
		//根据排名获取奖励信息
		std::vector<CRewardInfo> vecReward;
		this->getKillRankReward(pWarCityPtr->getAccType(), nkillRank, vecReward);

		if (vecReward.size() == 0)
			continue;

		//发送邮件奖励
		std::string strTitle = g_pConsummer->formatLangString(LANGTYPE_WORLD_WAR, LANGTYPE_WORLD_WAR_RANK_TILE);
		std::string sContent = g_pConsummer->formatLangString(LANGTYPE_WORLD_WAR, LANGTYPE_WORLD_WAR_RANK_CONTENT, strCityName.data(), nkillRank);
		g_pConsummer->sendMailToPlayer(cityRank.m_nPlayerID, strTitle, sContent, vecReward, REWARD_SOURCE_CITY_KILL_RANK);
	}
}


/*************************************************** 国战移动  begin *******************************************************/
IResponsePtr CWorldWarMgr::executeGWMWorldWarMove(IRequestPtr Request, CCurrentPtr Current)
{
	CGWMWorldWarMoveRequestPtr prequest = new CGWMWorldWarMoveRequest(Request->getSerialStream());
	CGWMWorldWarMoveResponsePtr presponse = new CGWMWorldWarMoveResponse();

	//通知gateway
	m_vecGateWaySession.clear();
	g_pConsummer->getGateWaySession(m_vecGateWaySession);
	for( size_t i = 0; i < m_vecGateWaySession.size();i++ )
		g_DaServer->send(m_vecGateWaySession[i], GW_WAR_MOVE_NOTICE, prequest.get());

	presponse->m_nRetCode = SUCCESS;
	return presponse.get();
}

void CWorldWarMgr::onDbmgrOpen()
{
	for (uint8 i = NATIONAL_WEI; i <= NATIONAL_WU; i++)
	{
		uint8 idNation = i; 
		CWarNationPtr cNationPtr = this->getWarNationPtr(idNation);
		if (cNationPtr == NULL)
		{
			cNationPtr = new CWarNation();
			cNationPtr->setNation(idNation);
			m_mapWarNation.insert(make_pair(idNation, cNationPtr));
		}
	}

	if (m_ndbopenflag == 0)
	{
		CDBGetWorldWarRequestPtr dbreq = new CDBGetWorldWarRequest();
		IResponseHandlerPtr handler = new TResponseHandler<CWorldWarMgr>(this,
			&CWorldWarMgr::onResponseGetWorldWar,
			&CWorldWarMgr::onTimeOut);
		g_DaServer->callServant(DB_GET_WORLD_WAR, dbreq.get(), handler, 0, NULL, NULL);
	}


	if( m_ndbgetlogflag == 0)
	{
		CDBGetWorldExploitLibRcdRequestPtr pDBGWELRRequest = new CDBGetWorldExploitLibRcdRequest();
		IResponseHandlerPtr handlerGWELR = new TResponseHandler<CWorldWarMgr>(this,
			&CWorldWarMgr::onResponseGetExploitLog,
			&CWorldWarMgr::onTimeOut);
		g_DaServer->callServant(DB_GET_WORLD_EXPLOIT_LIB_RCD, pDBGWELRRequest.get(), handlerGWELR, 0, NULL, NULL);

	}

	//加载城池系统信息
	if( m_ndbsatrapflag == 0)
	{
		CDBQuerySatrapCityInfRequestPtr pdbreq = new CDBQuerySatrapCityInfRequest();
		IResponseHandlerPtr handlerGWELR = new TResponseHandler<CWorldWarMgr>(this,
			&CWorldWarMgr::onResponseGetCitySatrapInfFromDB,
			&CWorldWarMgr::onTimeOut);
		g_DaServer->callServant(DB_QUERY_CITY_SATRAP, pdbreq.get(), handlerGWELR, 0, NULL, NULL);

	}

	return;
}

//获取玩家的太守城池ID
uint8 CWorldWarMgr::getPlayerSatrapCity(OBJID idPlayer)
{
	std::map<OBJID, uint8>::iterator iter = m_mapPlayerSatrapCity.find(idPlayer);
	if (iter == m_mapPlayerSatrapCity.end())
		return 0;

	return iter->second;
}

void CWorldWarMgr::onResponseGetCitySatrapInfFromDB(IResponsePtr pResponse, CCurrentPtr Current, CRefSharePtr Param) 
{
	log_debug("db_open_response (CWorldWarMgr::onResponseGetCitySatrapInfFromDB)");

	if (m_ndbsatrapflag == 1)
		return;

	CDBQuerySatrapCityInfResponsePtr pdbrsp = new CDBQuerySatrapCityInfResponse(pResponse->getSerialStream());
	if (SUCCESS != pdbrsp->m_nRetCode) 
	{
		log_debug("CWorldWarMgr::onResponseGetCitySatrapInfFromDB ret:(%d)", pdbrsp->m_nRetCode);
		return;
	}

	m_ndbsatrapflag = 1;

	this->m_mapSatrapCity.clear();
	this->m_mapPlayerSatrapCity.clear();
	this->m_mapCitySatrapDbFormation.clear();

	for (size_t i = 0; i < pdbrsp->m_vecSatrapCityInf.size(); ++ i) 
	{
		CSatrapCity& cityInf = pdbrsp->m_vecSatrapCityInf[i];
		if (0 == cityInf.m_nSatrapPlayer) 
		{
			continue;
		}

		CSatrapCityPtr pCityInf = new CSatrapCity(cityInf);
		this->m_mapSatrapCity.insert(std::make_pair(pCityInf->m_nCityId, pCityInf));
		this->m_mapPlayerSatrapCity.insert(std::make_pair(pCityInf->m_nSatrapPlayer, pCityInf->m_nCityId));

		std::string strDBFormation;
		this->converCitySatrap2Str(pCityInf->m_nCityId, strDBFormation);
		this->m_mapCitySatrapDbFormation.insert(std::make_pair(pCityInf->m_nCityId, strDBFormation));
	}
}

void CWorldWarMgr::onResponseGetExploitLog(IResponsePtr pResponse, CCurrentPtr Current, CRefSharePtr Param)
{
	log_debug("db_open_response (CWorldWarMgr::onResponseGetExploitLog)");

	if (m_ndbgetlogflag ==  1)
		return;

	m_ndbgetlogflag = 1;

	m_mapExploitLibRecord.clear();

	CDBGetWorldExploitLibRcdResponsePtr pDBGWELRResponse = new CDBGetWorldExploitLibRcdResponse(pResponse->getSerialStream());
	if( pDBGWELRResponse->m_nRetCode != SUCCESS )
	{
		log_debug("CWorldWarMgr::onResponseGetExploitLog ret:(%d)", pDBGWELRResponse->m_nRetCode);
		return;
	}

	m_nTotPraiseCnt = pDBGWELRResponse->m_nPraiseNum;

	for( size_t i = 0; i < pDBGWELRResponse->m_vecExploitLibInfo.size(); i ++ )
	{
		CNationExploitLibInfo &cNationLibInfo = pDBGWELRResponse->m_vecExploitLibInfo[i];
		if( cNationLibInfo.m_vecLibInfo.size() > 0 )
			m_mapExploitLibRecord.insert(make_pair(cNationLibInfo.m_nNationId, cNationLibInfo.m_vecLibInfo));
	}

	return;
}


void CWorldWarMgr::onResponseGetWorldWar(IResponsePtr pResponse, CCurrentPtr Current, CRefSharePtr Param)
{
	log_debug("db_open_response (CWorldWarMgr::onResponseGetWorldWar)");
	if (m_ndbopenflag ==  1)
		return;

	m_ndbopenflag = 1; 
	CDBGetWorldWarResponsePtr pdbresponse = new CDBGetWorldWarResponse(pResponse->getSerialStream());
	if (pdbresponse->m_nRetCode != SUCCESS)
	{
		log_debug("CWorldWarMgr::onResponseGetWorldWar ret:(%d)", pdbresponse->m_nRetCode);
		return;
	}

	//国家宣战信息
	for (size_t i = 0; i < pdbresponse->m_vecNation.size(); i++)
	{
		CWarNationInf &cNationInf = pdbresponse->m_vecNation[i];
		uint8 idNation = cNationInf.m_nNationID;
		CWarNationPtr cNationPtr = this->getWarNationPtr(idNation);
		if (cNationPtr == NULL)
			continue;

		cNationPtr->setDenNum(cNationInf.m_nDecNum);
	}

	//保存队伍信息
	std::map<OBJID, std::map<uint32, CDBBatBuddy> > mapEtapeBuddy;
	for (size_t i = 0; i < pdbresponse->m_vecBuddy.size(); i++)
	{
		CDBBatBuddy &cBatBuddy = pdbresponse->m_vecBuddy[i];
		OBJID idPlayer = cBatBuddy.m_nPlayerID;
		std::map<OBJID, std::map<uint32, CDBBatBuddy> >::iterator iter = mapEtapeBuddy.find(idPlayer);
		if (iter == mapEtapeBuddy.end())
		{
			std::map<uint32, CDBBatBuddy> mapTeam;
			mapTeam.insert(make_pair(cBatBuddy.m_nBuddySort, cBatBuddy));
			mapEtapeBuddy.insert(make_pair(idPlayer, mapTeam));
		}
		else
		{
			uint32 idBuddy = cBatBuddy.m_nBuddySort;
			std::map<uint32, CDBBatBuddy> &mapTeam = iter->second;
			std::map<uint32, CDBBatBuddy>::iterator iter_buddy = mapTeam.find(idBuddy);
			if (iter_buddy == mapTeam.end())
			{
				mapTeam.insert(make_pair(idBuddy, cBatBuddy));
			}
		}
	}

	//驻兵信息
	for (size_t i = 0; i < pdbresponse->m_vecEtapeTeam.size(); i++)
	{
		CDBEtapeTeam  &cEtapeTeam = pdbresponse->m_vecEtapeTeam[i];
		CWarNationPtr cNationPtr = this->getWarNationPtr(cEtapeTeam.m_nNation);
		if (cNationPtr == NULL)
			continue;

		CEtapeTeam cTeam;
		cTeam.m_nTeamID = cEtapeTeam.m_nTeamID;
		cTeam.m_nBattleEff = cEtapeTeam.m_nBattleEff;
		cTeam.m_nLevel = cEtapeTeam.m_nLevel;
		cTeam.m_nPlayerID = cEtapeTeam.m_nPlayerID;
		cTeam.m_nSex = cEtapeTeam.m_nSex;
		cTeam.m_strName = cEtapeTeam.m_strName;

		CPlayerTeamInf cPlayerTeam;
		cPlayerTeam.m_nFormationID = cEtapeTeam.m_nFormationID;
		cPlayerTeam.m_nVisionFlag = 1;

		//转换上阵武将信息
		CBattleBuddy cBatBuddy;
		uint32 idTeam = cTeam.m_nTeamID;
		OBJID idPlayer = cTeam.m_nPlayerID;

		std::map<OBJID, std::map<uint32, CDBBatBuddy> >::iterator iter_team = mapEtapeBuddy.find(idPlayer);
		if (iter_team == mapEtapeBuddy.end())
			continue;

		//转化灵龙信息
		cPlayerTeam.m_nDragonLevel = cEtapeTeam.m_nDragonLevel;
		cPlayerTeam.m_nTeamID = cTeam.m_nTeamID;
		this->convStrDragon(cEtapeTeam.m_strDragonSkill, cPlayerTeam.m_vecDragonSKill);
		//转化阵型信息
		this->conStr2FormationUnit(cEtapeTeam.m_strFormation, cPlayerTeam.m_vecUint);

		std::map<uint32, CDBBatBuddy>& mapBuddy = iter_team->second;
		for (size_t j = 0; j < cPlayerTeam.m_vecUint.size(); j++)
		{
			CFormatUint &cUnt = cPlayerTeam.m_vecUint[j];
			uint32 idBuddy = cUnt.m_nConfId;
			std::map<uint32, CDBBatBuddy>::iterator iter_buddy = mapBuddy.find(idBuddy);
			if (iter_buddy == mapBuddy.end())
				continue;

			CDBBatBuddy &cBuddy = iter_buddy->second;
			this->convDBBuddy(cBuddy, cBatBuddy);
			cPlayerTeam.m_vecBuddy.push_back(cBatBuddy);
		}

		this->getBuddyID(cPlayerTeam.m_vecUint, cPlayerTeam.m_vecBuddy);

		//获取城战玩家信息
		CPlayerCityWarInfPtr pPlayerWarPtr =  cNationPtr->getJoinPlayer(cTeam.m_nPlayerID);
		if (pPlayerWarPtr == NULL)
		{
			pPlayerWarPtr = new CPlayerCityWarInf();
			pPlayerWarPtr->m_nLevel = cEtapeTeam.m_nLevel;
			pPlayerWarPtr->m_strName = cEtapeTeam.m_strName;
			pPlayerWarPtr->m_nSex = cEtapeTeam.m_nSex;
			pPlayerWarPtr->m_nNation = cEtapeTeam.m_nNation;
			pPlayerWarPtr->m_nBattEffect = cEtapeTeam.m_nBattleEff;
			pPlayerWarPtr->m_nTitleID = cEtapeTeam.m_nTitleID;
			cNationPtr->addPlayerCityInf(cTeam.m_nPlayerID, pPlayerWarPtr);
		}

		pPlayerWarPtr->m_vecTeamInf.push_back(cPlayerTeam);
		cNationPtr->addNewEtapeTeam(cTeam);
        cNationPtr->addNewTeamID(cTeam.m_nTeamID);
	}

	//城池被占领信息
	for (size_t i = 0; i < pdbresponse->m_vecCityOccup.size(); i++)
	{
		CCityOccup &cCity = pdbresponse->m_vecCityOccup[i];
		uint8 idCity = cCity.m_nCityID;
		uint8 idNation = cCity.m_nNationID;
		CWorldCityPtr pWorldCityPtr  = g_pConsummer->getWorldCityPtr(idCity);
		if (pWorldCityPtr == NULL)
			continue;

		//判断国家是否变化
		if (pWorldCityPtr->m_nNation == idNation)
			continue;

		m_mapCityOccup[idCity]  = idNation;
	}

	//获取所有城池信息, 计算阵营的管辖城池数量
	std::vector<uint8> vecAllCity;
	g_pConsummer->getAllWolrdCity(vecAllCity);
	m_mapNationAdimNum.clear();
	for (size_t i = 0; i < vecAllCity.size(); i++)
	{
		uint8 idCity = vecAllCity[i];
		CWorldCityPtr pWorldCityPtr  = g_pConsummer->getWorldCityPtr(idCity);
		if (pWorldCityPtr == NULL)
			continue;

		uint8 idNation = pWorldCityPtr->m_nNation;
		this->getCityOccupNation(idCity, idNation);
		m_mapNationAdimNum[idNation] = m_mapNationAdimNum[idNation] + 1;
	}


	//记录国家管辖信息
	CNationDomainCity cPreDomainCity;
	std::map<uint8, std::vector<CNationDomainCity> > mapDBNationDomainCity;
	std::map<uint8, std::vector<CNationDomainCity> > mapNationDomainCity;
	for(size_t i = 0; i < pdbresponse->m_vecDomainCity.size(); i++)
	{
		CNationDomainCity &cDomainCity =  pdbresponse->m_vecDomainCity[i];
		mapDBNationDomainCity[cDomainCity.m_nNation].push_back(cDomainCity);
	}

	std::map<uint8, std::vector<CNationDomainCity> >::iterator iter_domain = mapDBNationDomainCity.begin();
	for (; iter_domain != mapDBNationDomainCity.end(); ++iter_domain)
	{
		//先进行排序

		//修补中间的奖励
        std::vector<CNationDomainCity> &vecNaionDomain = iter_domain->second;
		sort(vecNaionDomain.begin(), vecNaionDomain.end(), comparedomainreward);
		CNationDomainCity cPreDomain;
		for (size_t i = 0; i < vecNaionDomain.size(); i++)
		{
			CNationDomainCity &cCurDomain = vecNaionDomain[i];
			if (i == 0)
			{
			   mapNationDomainCity[cCurDomain.m_nNation].push_back(cCurDomain); 
			}
			else
			{
				for (uint16 nHour = cPreDomain.m_nHour+1; nHour <= cCurDomain.m_nHour; nHour++)
				{
					CNationDomainCity cNoDomainCity;
					cNoDomainCity.m_nHour = nHour;
					cNoDomainCity.m_nNation = cPreDomain.m_nNation;
					cNoDomainCity.m_strCity = cPreDomain.m_strCity;
					mapNationDomainCity[cNoDomainCity.m_nNation].push_back(cNoDomainCity); 
				}
			}

			cPreDomain = cCurDomain;
		}
        
		//补充后面的奖励
		CDateTime now;
		uint16 nCurHour = (uint16)now.getHour();
		for (size_t nHour = cPreDomain.m_nHour+1; nHour <= nCurHour; nHour++)
		{
			CNationDomainCity cNoDomainCity;
			cNoDomainCity.m_nHour = nHour;
			cNoDomainCity.m_nNation = cPreDomain.m_nNation;
			cNoDomainCity.m_strCity = cPreDomain.m_strCity;
			mapNationDomainCity[cNoDomainCity.m_nNation].push_back(cNoDomainCity); 
		}
	}

	iter_domain = mapNationDomainCity.begin();
	for (; iter_domain != mapNationDomainCity.end(); ++iter_domain)
	{
		uint8 idNation = iter_domain->first;
		//判断城池归属
		if (idNation != NATIONAL_WEI && idNation != NATIONAL_SHU && idNation != NATIONAL_WU)
			continue;

		std::vector<CNationDomainCity> &vecNaionDomain = iter_domain->second;

		for (size_t i = 0; i < vecNaionDomain.size(); i++)
		{
			CNationDomainCity &cDomainCity = vecNaionDomain[i];
			std::vector<uint8> vecCity;
			std::vector<CCityOccup> vecOccupCity;
			this->convStrCity(cDomainCity.m_strCity, vecCity);
			this->convOccupCity(cDomainCity.m_strOccupCity, vecOccupCity);
			//计算国家管辖城池产出
			this->nationCityReward(cDomainCity.m_nNation, vecCity, vecOccupCity, cDomainCity.m_nHour);
		}		
	}
	this->resetPrinceAcc(pdbresponse->m_vecAccInf);
	this->loadKillRank(pdbresponse->m_vecKillRank);
	return;
}
void CWorldWarMgr::loadKillRank(std::vector<CDBPrinceKillRank> &vecKillRank)
{
	m_vecPrinceKillRank.clear();
	m_mapPlayerKillNum.clear();
	m_vecPrinceKillRank.assign(vecKillRank.begin(), vecKillRank.end());
	for (size_t i = 0; i < m_vecPrinceKillRank.size();  i++)
	{
		CDBPrinceKillRank &cKillRank = m_vecPrinceKillRank[i];
		m_mapPlayerKillNum[cKillRank.m_nPlayerID] = cKillRank.m_nKillNum;
	}
	return;
}
void CWorldWarMgr::resetPrinceAcc(std::vector<CPrinceCityAcc> &vecAccInf)
{
	//加上时间的判断 
	CDateTime now;
	if (now.getHour() <= 4 || now.getHour() >= 19)
		return;

	if( vecAccInf.size() == 0 )
		return;

	m_nPrinceAccFlag = 1;

	m_vecPrinceAcc.clear();
	CPrinceCityAcc cPrinceAcc;
    for (size_t i = 0; i < vecAccInf.size(); i++)
	{
		CPrinceCityAcc &cCityAcc = vecAccInf[i];
		uint8 idAcc = cCityAcc.m_nAccID;
		uint8 idAccCity = cCityAcc.m_nCityID;
		cPrinceAcc.m_nCityID = idAccCity;
		cPrinceAcc.m_nAccID = idAcc;
		m_vecPrinceAcc.push_back(cPrinceAcc);
		if (cCityAcc.m_nStatus == 1)
			continue;
		const Json::Value& cCityAccConf = g_pConsummer->cityaccConf(idAcc);
		if (cCityAccConf == Json::nullValue)
			continue;
		CWorldCityPtr pWorldCityPtr = g_pConsummer->getWorldCityPtr(idAccCity);
		if (pWorldCityPtr == NULL)
			continue;
		CWarCityPtr pWarCityPtr = new CWarCity();
		std::string strbeginTime = cCityAccConf["attack_time"].asString();
		uint8 idNpcNation = (uint8)cCityAccConf["npc_nation"].asUInt();
		CDateTime now;
		int nHour = 0;
		int nMinute = 0;
		int nYear = now.getYear();
		int nMonth = now.getMonth();
		int nDay = now.getDay();
		int nSecond = now.getSecond();
		sscanf(strbeginTime.c_str(), "%d:%d", &nHour, &nMinute) ;
		now.init(nYear, nMonth, nDay, nHour, nMinute, nSecond, 0);
		uint32 begintime = now.getTotalSecs();
		uint8 idNation = pWorldCityPtr->m_nNation;
		this->getCityOccupNation(idAccCity, idNation);
		pWarCityPtr->init(idAcc, idAccCity, begintime);
		this->initAccCityNpcTeam(pWarCityPtr);
		pWarCityPtr->setCityName(cCityAccConf["name"].asString());
		pWarCityPtr->setDefSuccReward(m_vecDefSuccReward);
		pWarCityPtr->setDefFailReward(m_vecDefFailRewad);
		pWarCityPtr->setConsummer(g_pConsummer);
		pWarCityPtr->setDepoyedItem(m_nDeployedItem);
		pWarCityPtr->setDepoyedItemNum(m_nDepoyedItemNum);
		pWarCityPtr->setTeamListLmt(m_nTeamListLmt);
		pWarCityPtr->setNation(idNation);
		pWarCityPtr->setAttNation(idNpcNation); 
		pWarCityPtr->setItemRewardExp(m_mapItemRewardExp1);
		cCityAcc.m_nAccID = idAcc;
		cCityAcc.m_nCityID = idAccCity;
		m_vecPrinceAcc.push_back(cPrinceAcc);
		m_mapWarCityPtr.insert(make_pair(idAccCity, pWarCityPtr));
	}
	return;
}

//转换城池
void CWorldWarMgr::convStrCity(std::string strCity, std::vector<uint8> &vecCity)
{
	std::vector<std::string> vecParam;
	splitString(strCity, ",", vecParam, true);
	for (size_t i = 0; i < vecParam.size(); ++i)
	{
		uint8 cityid = (uint8)(atoi(vecParam[i].data()));
		vecCity.push_back(cityid);
	}
    
	return;
}

void CWorldWarMgr::onResponseGetPlayerTeam(IResponsePtr pResponse, CCurrentPtr Current, CRefSharePtr Param)
{
	if (m_ndbopenflag ==  1)
		return;

	m_ndbopenflag = 1;

	m_mapWarJoinPlayer.clear();

	CDBGetCityAccInfResponsePtr pdbresponse = new CDBGetCityAccInfResponse(pResponse->getSerialStream());
	if (pdbresponse->m_nRetCode != SUCCESS)
		return;

	//记录事件信息
	for (size_t i = 0; i < pdbresponse->m_vecCityAcc.size(); i++)
	{
		CDBCityAccident  &dbaccPtr = pdbresponse->m_vecCityAcc[i];

		const Json::Value& cCityAccConf = g_pConsummer->cityaccConf(dbaccPtr.m_nAccID);
		if (cCityAccConf == Json::nullValue)
			continue;

		CWarCityPtr pWarCityPtr = new CWarCity();
		pWarCityPtr->init(dbaccPtr.m_nAccID, dbaccPtr.m_nCityID, dbaccPtr.m_nBeginTime);
		this->initAccCityNpcTeam(pWarCityPtr);
		pWarCityPtr->setCityName(cCityAccConf["name"].asString());
		pWarCityPtr->setDefSuccReward(m_vecDefSuccReward);
		pWarCityPtr->setDefFailReward(m_vecDefFailRewad);
		pWarCityPtr->setConsummer(g_pConsummer);
		pWarCityPtr->setDepoyedItem(m_nDeployedItem);
		pWarCityPtr->setDepoyedItemNum(m_nDepoyedItemNum);
		pWarCityPtr->setTeamListLmt(m_nTeamListLmt);
		pWarCityPtr->setItemRewardExp(m_mapItemRewardExp1);

		m_mapWarCityPtr.insert(make_pair(dbaccPtr.m_nCityID, pWarCityPtr));
	}

	//记录玩家队伍信息
	for (size_t i = 0; i < pdbresponse->m_vecTeamInf.size(); i++)
	{
		CDBPlayerTeamInf &cTeamInf = pdbresponse->m_vecTeamInf[i];
		CWarJoinPlayerPtr pWarJoinPlayerPtr = this->getWarJoinPlayer(cTeamInf.m_nPlayerID);
		if (pWarJoinPlayerPtr == NULL)
		{
			pWarJoinPlayerPtr = new CWarJoinPlayer();
			pWarJoinPlayerPtr->m_nCityID = 0;
			pWarJoinPlayerPtr->m_nBattEffect = cTeamInf.m_nBattEffect;
			pWarJoinPlayerPtr->m_nGoverPosts = cTeamInf.m_nGoverPosts;
			pWarJoinPlayerPtr->m_nLevel = cTeamInf.m_nLevel;
			pWarJoinPlayerPtr->m_nNation = cTeamInf.m_nNation;
			pWarJoinPlayerPtr->m_nPlayerID = cTeamInf.m_nPlayerID;
			pWarJoinPlayerPtr->m_nSex = cTeamInf.m_nSex;
			pWarJoinPlayerPtr->m_strName = cTeamInf.m_strName;
			m_mapWarJoinPlayer.insert(make_pair(pWarJoinPlayerPtr->m_nPlayerID, pWarJoinPlayerPtr));
		}

		//增加玩家信息到城池事件中
		uint8 idCity = cTeamInf.m_nCityID;
		CWarCityPtr pWarCityPtr = this->getWarCityPtr(idCity);
		if (pWarCityPtr == NULL)
			continue;

		CPlayerCityWarInfPtr pPlayerCityWarInfPtr = new CPlayerCityWarInf();
		pPlayerCityWarInfPtr->m_nCityID = cTeamInf.m_nCityID;
		pPlayerCityWarInfPtr->m_nDir = cTeamInf.m_nDir;
		pPlayerCityWarInfPtr->m_nKillNum = 0;
		pPlayerCityWarInfPtr->m_nLevel = cTeamInf.m_nLevel;
		pPlayerCityWarInfPtr->m_nNation = cTeamInf.m_nNation;
		pPlayerCityWarInfPtr->m_nSex = cTeamInf.m_nSex;
		pPlayerCityWarInfPtr->m_strName = cTeamInf.m_strName;
		pPlayerCityWarInfPtr->m_nBattEffect = cTeamInf.m_nBattEffect;
		CPlayerTeamInf cPlayerTeam;
		cPlayerTeam.m_nFormationID = cTeamInf.m_nFormationID;

		//转化阵型信息
		this->conStr2FormationUnit(cTeamInf.m_strFormation, cPlayerTeam.m_vecUint);

		//转换上阵武将信息
		CBattleBuddy cBatBuddy;
		for (size_t j = 0; j < cTeamInf.m_vecBuddy.size(); j ++)
		{
			CDBBatBuddy &cBuddy = cTeamInf.m_vecBuddy[j];
			this->convDBBuddy(cBuddy, cBatBuddy);
			cPlayerTeam.m_vecBuddy.push_back(cBatBuddy);
		}

		this->getBuddyID(cPlayerTeam.m_vecUint, cPlayerTeam.m_vecBuddy);

		//转化灵龙信息
		cPlayerTeam.m_nDragonLevel = cTeamInf.m_nDragonLevel;
		cPlayerTeam.m_nTeamID = pWarCityPtr->getTeamID();
		this->convStrDragon(cTeamInf.m_strDragon, cPlayerTeam.m_vecDragonSKill);

		pPlayerCityWarInfPtr->m_vecTeamInf.push_back(cPlayerTeam);
		pWarCityPtr->addPlayerBlowTeam(cTeamInf.m_nPlayerID, pPlayerCityWarInfPtr);

		CWarCityTeam cCityTeam;
		cCityTeam.m_nTeamID = cPlayerTeam.m_nTeamID;
		cCityTeam.m_nAvatarID = cTeamInf.m_nPlayerID;
		cCityTeam.m_nTeamType = TEAM_TYPE_PLAYER;
		cCityTeam.m_strName = cTeamInf.m_strName;
		CTeamBatInf cTeamBatInf;
		for (size_t j = 0; j < cPlayerTeam.m_vecUint.size(); j++)
		{
			CFormatUint &cUint = cPlayerTeam.m_vecUint[j];
			if (cUint.m_nGrid == 16)
				continue;

			cTeamBatInf.m_nGrid = cUint.m_nGrid;
			cTeamBatInf.m_nLastEn = cUint.m_nCurEn;
			cTeamBatInf.m_nLastHP = cUint.m_nMaxBlood;
			cCityTeam.m_vecBatInf.push_back(cTeamBatInf);
		}

		pWarCityPtr->addBlowTeam(cCityTeam);
	}

	return;
}

//获取BUDDYid
void CWorldWarMgr::getBuddyID(std::vector<CFormatUint> &vecUint, std::vector<CBattleBuddy> &vecBuddy)
{
	for (size_t i = 0; i < vecUint.size(); i++)
	{
		CFormatUint &cUint = vecUint[i];
		for (size_t j = 0 ; j < vecBuddy.size(); j++)
		{
			CBattleBuddy &cBuddy = vecBuddy[j];
			if (cBuddy.m_nBuddySort == cUint.m_nConfId)
			{
				cUint.m_nFightId = cBuddy.m_nBuddyID;
				break;
			}
		}
	}

	return;
}

//转化阵型信息
void CWorldWarMgr::conStr2FormationUnit(std::string strFormation,std::vector<CFormatUint> &vecUint)
{
	std::vector<std::string> vecOption;
	splitString(strFormation, ";", vecOption, true);
	CFormatUint cUint;
	std::string strUint;
	for(size_t i = 0; i < vecOption.size(); ++ i)
	{
		uint32 nGrid = 0;
		uint32 confId = 0;
		uint32 nBlood = 0;
		uint32 nEn = 0;
		uint32 nSpeed = 0;
		uint32 nLevel = 0;
		uint32 nClass = 0;
		strUint = vecOption[i];

		if (sscanf(strUint.c_str(), "%d:%d:%d:%d:%d:%d:%d", &nGrid, &confId,&nBlood, &nEn, &nSpeed, &nLevel, &nClass) != 7)
			continue;

		cUint.m_nClass = (uint8)nClass;
		cUint.m_nConfId = confId;
		cUint.m_nCurBlood = nBlood;
		cUint.m_nCurEn = nEn;
		cUint.m_nCurSpeed = nSpeed;
		cUint.m_nGrid = (uint8)nGrid;
		cUint.m_nLevel = (uint8)nLevel;
		cUint.m_nType = 0;
		cUint.m_nMaxEn = nEn;
		cUint.m_nMaxBlood = nBlood;
		vecUint.push_back(cUint);
	}

	return;
}

//转化BUDDY信息
void CWorldWarMgr::convDBBuddy(CDBBatBuddy &cDbBuddy, CBattleBuddy &cBatBuddy)
{
	cBatBuddy.m_nBattleEffect = cDbBuddy.m_nBattleEffect;
	cBatBuddy.m_nBuddyID = cDbBuddy.m_nBuddyID;
	cBatBuddy.m_nBuddySort = cDbBuddy.m_nBuddySort;
	cBatBuddy.m_nClass = cDbBuddy.m_nClass;
	cBatBuddy.m_nLevel = cDbBuddy.m_nLevel;
	cBatBuddy.m_nProfessionLevel = cDbBuddy.m_nProfessionLevel;
	cBatBuddy.m_nRebornNum = cDbBuddy.m_nRebornNum;
	cBatBuddy.m_nStar = cDbBuddy.m_nStar;
	cBatBuddy.m_vecAttr.clear();
	g_pConsummer->Attr_StrToVec(cDbBuddy.m_strAttr, cBatBuddy.m_vecAttr);
	return;
}

//转化灵龙信息
void CWorldWarMgr::convStrDragon(std::string strDragon, std::vector<CLSkillData> &Skill)
{
	std::string tokenStr = ";";
	std::vector<std::string> tmpVec;
	std::vector<std::string> ::iterator tmpIter;

	splitString(strDragon, tokenStr, tmpVec, true);
	for( tmpIter=tmpVec.begin();tmpIter!=tmpVec.end();tmpIter++ )
	{
		CLSkillData cLSkillData;

		uint32 skillId = 0;
		uint32 skillLvl = 0;

		if( 2 != sscanf((*tmpIter).c_str(), "%d:%d", &skillId, &skillLvl) )
			continue;

		cLSkillData.m_nSkillID = skillId;
		cLSkillData.m_nSKillLevel = (uint8)skillLvl;

		Skill.push_back(cLSkillData);
	}

	return;
}


void CWorldWarMgr::playerOnline(OBJID idPlayer)
{


	 return;
}

void CWorldWarMgr::playerOffline(OBJID idPlayer)
{
	//更新太守调整的阵形信息
	std::map<OBJID, uint8>::iterator it = this->m_mapPlayerSatrapCity.find(idPlayer);
	if (it == this->m_mapPlayerSatrapCity.end()) {
		return;
	}

	uint8 nCity = it->second;
	if (0 == nCity) {
		this->m_mapPlayerSatrapCity.erase(idPlayer);
		return;
	}

	//检查守城玩家或者阵形是否有变更
	std::string strCurFormation;
	this->converCitySatrap2Str(nCity, strCurFormation);
	std::map<uint8, std::string>::iterator itdb = this->m_mapCitySatrapDbFormation.find(nCity);
	if (itdb != this->m_mapCitySatrapDbFormation.end()) {
		if (strCurFormation == itdb->second) {
			return;
		}
	}

	this->m_mapCitySatrapDbFormation[nCity] = strCurFormation;
	this->updateDBCitySatrapInf(nCity);
	return;
}

uint8 CWorldWarMgr::checkWarStatus(CWarCityPtr pWarCityPtr)
{
	uint16 blowTeam = pWarCityPtr->getBlowTeamNum();
	uint16 attTeam = pWarCityPtr->getAttTeamNum();
	if (blowTeam == 0 || attTeam == 0)
	{
		if (attTeam == 0)
		{
			pWarCityPtr->setStatus(2);
		}
		else
		{
			pWarCityPtr->setStatus(3);
		}
	}
    return pWarCityPtr->getStatus();
}
//定时服务
void CWorldWarMgr::onTimer(uint32 nInterval)
{
	//保存功勋宝箱获取记录
	CDateTime nowTime;
	if( (m_nLastLibRcdSaveTime == 0) || (nowTime.getTotalSecs() > m_nLastLibRcdSaveTime + EXPLOID_LIB_RECORD_SAVE_CD) )
	{
		m_nLastLibRcdSaveTime = nowTime.getTotalSecs();

		CDBUpdWorldExploitLibRcdRequestPtr pDBUWELRRequest = new CDBUpdWorldExploitLibRcdRequest();

		std::map<uint8, std::vector<CExploitLibInfo> > ::iterator itMELI = m_mapExploitLibRecord.begin();
		for( ; itMELI != m_mapExploitLibRecord.end(); itMELI ++ )
		{
			CNationExploitLibInfo cNationExploitLibInfo;
			cNationExploitLibInfo.m_nNationId = itMELI->first;
			cNationExploitLibInfo.m_vecLibInfo.assign(itMELI->second.begin(), itMELI->second.end());

			pDBUWELRRequest->m_vecExploitLibInfo.push_back(cNationExploitLibInfo);
		}

		if( pDBUWELRRequest->m_vecExploitLibInfo.size() > 0 )
		{
			g_DaServer->callServant(DB_UPD_WORLD_EXPLOIT_LIB_RCD, pDBUWELRRequest.get(), NULL, 0, NULL, NULL);
		}
	}

	std::vector<uint8> vecEndCityAcc;
	std::vector<uint8> vecEndAcc;

	//循环事件，进行车轮战
	std::map<uint8, CWarCityPtr>::iterator iter  =  m_mapWarCityPtr.begin();
	for(; iter != m_mapWarCityPtr.end(); ++iter)
	{
		CWarCityPtr pWarCityPtr = iter->second;
		if (pWarCityPtr == NULL)
			continue;

		//判断事件状态
		if (pWarCityPtr->getStatus() != 1)
			continue;

		//判断事件是否可以结束（玩家攻城战）
		this->checkAccTimeEnd(pWarCityPtr);

		//车轮战处理
		this->cityWheelWar(pWarCityPtr);

		//清理过期的单挑/被单挑堆栈
		CDateTime now;
		if (now.getTotalSecs() > pWarCityPtr->getClearPickTeamTime())
		{
			pWarCityPtr->clearPickTeam();
			pWarCityPtr->setClearPickTeamTime(now.getTotalSecs() + 15);
		}

		//定时的统帅队伍进行攻击处理
		this->npcAutoPick(pWarCityPtr);

		//判断国战是否结束
		uint8 nStatus = this->checkWarStatus(pWarCityPtr);
		if (nStatus == 1)
			continue;

		//城战结束奖励
		pWarCityPtr->cityPlayerRewardAcc();

		vecEndCityAcc.push_back(pWarCityPtr->getCityID());
		//杀敌排名奖励
		this->cityKillRankReward(pWarCityPtr);

		//对驻兵进行道具返还
		pWarCityPtr->accEndRebackDeployItem();

		//通知gateway 城池结束通知
		CGWWorldWarNoticeRequestPtr gwreq = new CGWWorldWarNoticeRequest();
		gwreq->m_nType = WORLD_WAR_FIGHT_END;
		gwreq->m_nAccID = pWarCityPtr->getAccID();
		gwreq->m_nCityID = pWarCityPtr->getCityID();
		gwreq->m_strData = this->getCityWarEndInf(pWarCityPtr);
		m_vecGateWaySession.clear();
		g_pConsummer->getGateWaySession(m_vecGateWaySession);
		for( size_t i = 0; i < m_vecGateWaySession.size();i++ )
			g_DaServer->send(m_vecGateWaySession[i], GW_WORLD_WAR_NOTICE, gwreq.get());

		vecEndAcc.push_back(gwreq->m_nCityID);
        
		//登记城池攻占情况
		this->regCityOccup(pWarCityPtr);

		std::string strNotice;
		std::string strCityName =  pWarCityPtr->getCityName();
		std::string strAttNation = g_pConsummer->formatLangString(LANGTYPE_NATION_NAME, pWarCityPtr->getAttNation());
		std::string strNation = g_pConsummer->formatLangString(LANGTYPE_NATION_NAME, pWarCityPtr->getNation());

		std::vector<OBJID> vecJoinPlayer;
		this->getJoinPlayer(vecJoinPlayer);

		//发送通告
		if (pWarCityPtr->getAccType() == ACC_TYPE_PLAYER)
		{   
			//攻击失败
			if (pWarCityPtr->getStatus() == 2)
			{
				strNotice = g_pConsummer->formatLangString(LANGTYPE_WORLD_WAR, LANGTYPE_WORLD_WAR_DECLARE_WAR_FAIL, 
					                                   strAttNation.data(), strNation.data(), strCityName.data());
			}
			else
			{
				strNotice = g_pConsummer->formatLangString(LANGTYPE_WORLD_WAR, LANGTYPE_WORLD_WAR_DECLARE_WAR_SUC, 
					                                     strAttNation.data(), strNation.data(), strCityName.data());
			}
			sendSysNoticeToPlayer(CHANNEL_MARQUEE, strNotice, NULL, vecJoinPlayer, 1);
		}
		else
		{
			if (pWarCityPtr->getStatus() != 2)
			{
				//npc攻城战公告发送
				CWorldCityPtr pDat = g_pConsummer->getWorldCityPtr(pWarCityPtr->getCityID());
				if (NULL != pDat) 
				strCityName = pDat->m_strName;

				strNotice = g_pConsummer->formatLangString(LANGTYPE_WORLD_WAR, LANGTYPE_WORLD_WAR_CITY_ADMIN_CHANGE, 
					strAttNation.data(), strNation.data(), strCityName.data());

				sendSysNoticeToPlayer(CHANNEL_MARQUEE, strNotice, NULL, vecJoinPlayer, 1);
			}
		}
	}

	//删除结束的国战事件
	for (size_t i = 0; i < vecEndAcc.size(); i++)
	{
		uint8 idCity = vecEndAcc[i];
		CWarCityPtr pWarCityPtr = getWarCityPtr(idCity);
		if (pWarCityPtr == NULL)
			continue;
        
		//玩家攻城战才需要进行城池保护
		if (pWarCityPtr->getAccType() == ACC_TYPE_PLAYER)
		{
			//记录保护城池
			CProjectCityPtr pProcCityPtr = new CProjectCity();
			pProcCityPtr->m_nCityID = idCity;
			pProcCityPtr->m_nProcTime = nowTime.getTotalSecs() + m_nProcTime;
			m_mapProCity.insert(make_pair(idCity, pProcCityPtr));
		}

		m_mapWarCityPtr.erase(idCity);
	}

	//循环删除保护城池
	std::map<uint8, CProjectCityPtr>::iterator iter_pro = m_mapProCity.begin();
	for (; iter_pro != m_mapProCity.end(); iter_pro++)
	{
		uint8 idCity = iter_pro->first;
		CProjectCityPtr pProcCityPtr = iter_pro->second;
		if (pProcCityPtr != NULL && pProcCityPtr->m_nProcTime <= nowTime.getTotalSecs())
		{
			m_mapProCity.erase(idCity);
			break;
		}
	}

	//启动玩家攻城战事件
	std::map<uint8, CWarCityPtr>::iterator iter_war  =  m_mapWarCityPtr.begin();
	for(; iter_war != m_mapWarCityPtr.end(); ++iter_war)
	{
		CWarCityPtr pWarCityPtr = iter_war->second;
		if (pWarCityPtr == NULL)
			continue;

		//判断是否是玩家攻城战类型
		if (pWarCityPtr->getAccType() == ACC_TYPE_NPC)
			continue;

		//判断类型
		if (pWarCityPtr->getStatus() != 0)
			continue;

		//判断事件是否开始
		if (pWarCityPtr->getBeginTime() <= nowTime.getTotalSecs())
		{
			pWarCityPtr->setStatus(1);

			//从大本营增加作战队伍
			while(pWarCityPtr->addNewTeamToAttFightQueue())
				;
			while (pWarCityPtr->addNewTeamToBlowFightQueue())
				;

			pWarCityPtr->setWheelFightTime(nowTime.getTotalSecs());

			//城战开始
			CGWWorldWarNoticeRequestPtr gwreq = new CGWWorldWarNoticeRequest();
			gwreq->m_nType = WORLD_WAR_FIGHT_BEGIN;
			gwreq->m_nAccID = pWarCityPtr->getAccID();
			gwreq->m_nCityID = pWarCityPtr->getCityID();
			//通知gateway
			m_vecGateWaySession.clear();
			g_pConsummer->getGateWaySession(m_vecGateWaySession);
			for( size_t i = 0; i < m_vecGateWaySession.size();i++ )
				g_DaServer->send(m_vecGateWaySession[i], GW_WORLD_WAR_NOTICE, gwreq.get());
		}
	}
	return ;
}

//校验事件是否到达结束时间
void CWorldWarMgr::checkAccTimeEnd(CWarCityPtr pWarCityPtr)
{
	if (pWarCityPtr->getStatus() != 1)
		return;

	if (pWarCityPtr->getAccType() == ACC_TYPE_NPC)
		return;

	CDateTime now;
	uint32 curTime = now.getTotalSecs();
	if (curTime <= pWarCityPtr->getEndTime())
		return;

	pWarCityPtr->setStatus(2); //防守成功

	return;
}

//减少阵营管辖的城池数量
uint8 CWorldWarMgr::decNaitonAdminCity(uint8 idNation)
{
	uint8 nNum = 0;
	std::map<uint8, uint8>::iterator iter = m_mapNationAdimNum.find(idNation);
	if (iter == m_mapNationAdimNum.end())
		return nNum;

	uint8 &nAdimNum = iter->second;
	if (nAdimNum > 0)
		nAdimNum--;

	nNum = nAdimNum;
	return nNum;
}

//增加阵营管辖的城池数量
uint8 CWorldWarMgr::addNaitonAdminCity(uint8 idNation)
{
	uint8 nNum = 0;
	std::map<uint8, uint8>::iterator iter = m_mapNationAdimNum.find(idNation);
	if (iter == m_mapNationAdimNum.end())
		return nNum;

	uint8 &nAdimNum = iter->second;
	nAdimNum++;

	nNum = nAdimNum;
	return nNum;
}

//获取阵营管辖的城池数量
uint8 CWorldWarMgr::getNaitonAdminCityNum(uint8 idNation)
{
	uint8 nNum = 0;
	std::map<uint8, uint8>::iterator iter = m_mapNationAdimNum.find(idNation);
	if (iter == m_mapNationAdimNum.end())
		return 0;

	return iter->second;
}


//登记城池被占领情况
void CWorldWarMgr::regCityOccup(CWarCityPtr pWarCityPtr)
{
	//获取事件信息
	if (pWarCityPtr == NULL)
		return;

	uint8 idCity = pWarCityPtr->getCityID();

	//攻击成功
	if (pWarCityPtr->getStatus() == 3)
	{
		//获取城池信息
		CWorldCityPtr pWorldCityPtr = g_pConsummer->getWorldCityPtr(idCity);
		if (pWorldCityPtr == NULL)
			return;

		uint8 idOldNation = pWarCityPtr->getNation();
		uint8 idAttNation = pWarCityPtr->getAttNation();
		this->addNaitonAdminCity(idAttNation);
		this->decNaitonAdminCity(idOldNation);
		if (idAttNation == pWorldCityPtr->m_nNation)
		{
			m_mapCityOccup.erase(idCity);
			CDBDelCityOccupRequestPtr dbreq = new CDBDelCityOccupRequest();
			dbreq->m_nCityID = idCity;
			g_DaServer->callServant(DB_DEL_CITY_OCCUP, dbreq.get(), NULL, 0, NULL, NULL);
		}
		else
		{
			m_mapCityOccup[idCity] = idAttNation;
			CDBRegCityOccupRequestPtr dbreq = new CDBRegCityOccupRequest();
			dbreq->m_nCityID = idCity;
			dbreq->m_nNatianID = idAttNation;
			g_DaServer->callServant(DB_REG_CITY_OCCUP, dbreq.get(), NULL, 0, NULL, NULL);
		}
        
		//城池信息通告
		this->warEndNotice(pWarCityPtr);

		//城池被攻占处理
		CSatrapCityPtr pCityInf = this->getSatrapCityInf(idCity);
		if (NULL != pCityInf) 
		{
			OBJID idPlayer = pCityInf->m_nSatrapPlayer;
			this->m_mapPlayerSatrapCity.erase(idPlayer);
			this->m_mapCitySatrapDbFormation.erase(idCity);
			this->m_mapSatrapCity.erase(idCity);
			
			//更新db
			CDBDelCitySatrapRequestPtr dbreq = new CDBDelCitySatrapRequest();
			dbreq->m_nCityID = idCity;
			g_DaServer->callServant(DB_DEL_CITY_SATRAP, dbreq.get(), NULL);

			//通知玩家太守被争夺
			this->playerWarNotice(idPlayer, WORLD_SATRAP_CITY_ABORT, idCity, "");

			std::string strCity = pWorldCityPtr->m_strName;
			std::string strNation = g_pConsummer->formatLangString(LANGTYPE_NATION_NAME, idAttNation);

			//发送邮件
			std::vector<CRewardInfo> vecAward;
			std::string strTitle = g_pConsummer->formatLangString(LANGTYPE_WORLD_CITY, LANGTYPE_CITY_OCC_TITLE);
			std::string sContent = g_pConsummer->formatLangString(LANGTYPE_WORLD_CITY, LANGTYPE_CITY_OCC_CONTENT, \
				strCity.c_str(), strNation.c_str(), strCity.c_str());
			g_pConsummer->sendMailToPlayer(idPlayer, strTitle, sContent, vecAward, REWARD_SOURCE_CITY_SATRAP);
		}
	}
	if (pWarCityPtr->getAccID() >= PRINCE_ACC_ID_BEGIN && pWarCityPtr->getAccID() <= PRINCE_ACC_ID_END)
	{
		for (size_t i = 0; i < m_vecPrinceAcc.size(); i++)
		{
			CPrinceCityAcc &cPrinceAcc = m_vecPrinceAcc[i];
			if (cPrinceAcc.m_nAccID == pWarCityPtr->getAccID())
			{
				cPrinceAcc.m_nStatus = 2;
				if (pWarCityPtr->getStatus() == 3)
					cPrinceAcc.m_nStatus = 1;
				break;
			}
		}
		CDBDelAccInfRequestPtr dbreq = new CDBDelAccInfRequest();
		dbreq->m_nAccID = pWarCityPtr->getAccID();
		g_DaServer->callServant(DB_DEL_ACC_INF, dbreq.get(), NULL, 0, NULL, NULL);
	}

	return;
}

//国战结束的通告
void CWorldWarMgr::warEndNotice(CWarCityPtr pWarCityPtr)
{
	if (pWarCityPtr == NULL)
		return;

	CGWWorldWarNoticeRequestPtr gwreq = new CGWWorldWarNoticeRequest();
	gwreq->m_nType = WORLD_CITY_CHANGE;
	gwreq->m_nAccID = pWarCityPtr->getAccID();
	gwreq->m_nCityID = pWarCityPtr->getCityID();
	char sBuf[128] = {0};
	sprintf(sBuf, "%d", pWarCityPtr->getAttNation());
	gwreq->m_strData = std::string(sBuf);
	m_vecGateWaySession.clear();
	g_pConsummer->getGateWaySession(m_vecGateWaySession);
	for( size_t i = 0; i < m_vecGateWaySession.size();i++ )
		g_DaServer->send(m_vecGateWaySession[i], GW_WORLD_WAR_NOTICE, gwreq.get());

	//攻占成功
	if (pWarCityPtr->getStatus() == 3)
	{
		if (pWarCityPtr->getAttNation() != NATIONAL_NONE)
		{
			uint8 idNation = pWarCityPtr->getAttNation();
		    uint8 nNum = this->getNaitonAdminCityNum(idNation);
			gwreq->m_nType = WORLD_ADMIN_CITY_NUM;
			gwreq->m_nAccID = nNum;
			gwreq->m_nCityID = idNation;
			for( size_t i = 0; i < m_vecGateWaySession.size();i++ )
				g_DaServer->send(m_vecGateWaySession[i], GW_WORLD_WAR_NOTICE, gwreq.get());
		}

		if (pWarCityPtr->getNation() != NATIONAL_NONE)
		{
			uint8 idNation = pWarCityPtr->getNation();
			uint8 nNum = this->getNaitonAdminCityNum(idNation);
			gwreq->m_nType = WORLD_ADMIN_CITY_NUM;
			gwreq->m_nAccID = nNum;
			gwreq->m_nCityID = idNation;
			for( size_t i = 0; i < m_vecGateWaySession.size();i++ )
				g_DaServer->send(m_vecGateWaySession[i], GW_WORLD_WAR_NOTICE, gwreq.get());
		}
	}

	return;
}

//获取城战最后结果
std::string CWorldWarMgr::getCityWarEndInf(CWarCityPtr pWarCityPtr)
{
	if (pWarCityPtr == NULL)
		return "";

	uint8 idCity = pWarCityPtr->getCityID();
	uint8 warresult = 0;  //防守成功
	if (pWarCityPtr->getStatus() == 3)
		warresult = 1;  //防守失败

	int16 blowLostNum = 0; //防守损失
	int16 blowLastNum = 0;  //防守剩余

	blowLostNum = pWarCityPtr->getTotBlowNpcTeam() + pWarCityPtr->getPlayerBlowTeamNum() - pWarCityPtr->getBlowTeamNum();
	blowLastNum = pWarCityPtr->getBlowTeamNum();

	int16 attLostNum = 0; //攻击损失
	int16 attLastNum = 0; //攻击剩余
	attLostNum = pWarCityPtr->getTotAttNpcTeam() + pWarCityPtr->getPlayerAttTeamNum() - pWarCityPtr->getAttTeamNum();
	attLastNum = pWarCityPtr->getAttTeamNum();

	char sBuf[256] ={0};
	sprintf(sBuf, "%d,%d,%d,%d,%d,%d",idCity, warresult, blowLostNum, blowLastNum, attLostNum, attLastNum);

	return std::string(sBuf);
}

void CWorldWarMgr::onServerStop()
{

}


//玩家国战信息通知
void CWorldWarMgr::playerWarNotice(OBJID idPlayer, uint8 nKey, uint32 nValues, std::string strData)
{
	if (idPlayer == 0)
		return;

	CGWWarPlayerNoticeRequestPtr gwreq = new CGWWarPlayerNoticeRequest();
	gwreq->m_nType = nKey;
	gwreq->m_nData = nValues;
	gwreq->m_strData = strData;
	gwreq->m_vecPlayer.push_back(idPlayer);

	uint8 idCity = (uint8)nValues;

	//删除队伍信息
	switch (nKey)
	{
	case WORLD_WAR_TEAM_END: //队伍阵亡
		{
			CWarJoinPlayerPtr pWarJoinPlayerPtr = this->getWarJoinPlayer(idPlayer);
			if (pWarJoinPlayerPtr != NULL)
				pWarJoinPlayerPtr->m_mapBuddy.erase(idCity);
		}
		break;
	default:
		break;
	}

	CUserSessionInfoPtr usersession = g_pConsummer->getUserSession(idPlayer);
	if( NULL != usersession )
		g_DaServer->send(usersession->m_nGWSessionID, GW_WAR_PLAYER_NOTICE, gwreq.get());
}

//玩家国战信息通知
void CWorldWarMgr::warPlayerNotice(std::vector<OBJID> &vecPlayer, uint8 nKey, uint32 nValues)
{
	if (vecPlayer.size() == 0)
		return;

	//CGWWarPlayerNoticeRequestPtr gwreq = new CGWWarPlayerNoticeRequest();
	//gwreq->m_nType = nKey;
	//gwreq->m_nData = nValues;
	//gwreq->m_vecPlayer.assign(vecPlayer.begin(), vecPlayer.end());

	////通知gateway
	//m_vecGateWaySession.clear();
	//g_pConsummer->getGateWaySession(m_vecGateWaySession);
	//for( size_t i = 0; i < m_vecGateWaySession.size();i++ )
	//	g_DaServer->send(m_vecGateWaySession[i], GW_WAR_PLAYER_NOTICE, gwreq.get());

}


//获取城池事件信息
CWarCityPtr CWorldWarMgr::getWarCityPtr(uint8 idCity)
{
	std::map<uint8, CWarCityPtr>::iterator iter = m_mapWarCityPtr.find(idCity);
	if (iter == m_mapWarCityPtr.end())
		return NULL;

	return iter->second;
}

//获取城池事件信息
CWarCityPtr CWorldWarMgr::getWarCityByAccID(uint8 idAcc)
{
	std::map<uint8, CWarCityPtr>::iterator iter = m_mapWarCityPtr.begin();
	for (; iter != m_mapWarCityPtr.end(); ++iter)
	{
		CWarCityPtr pWarCityPtr = iter->second;
		if (pWarCityPtr == NULL)
			continue;

		if (pWarCityPtr->getAccID() == idAcc)
			return iter->second;
	}

	return NULL;
}


//获取国战集团军
CWarGroupPtr CWorldWarMgr::getWarGroupPtr(uint32 idGroup , uint8 worldlevel)
{
	uint8 lowworldlevel = worldlevel;
	uint8 nearlevel = 0;
	if (worldlevel < 30)
		lowworldlevel = 30;

	OBJID idKey = (OBJID)idGroup;

	std::map<OBJID , CWarGroupPtr>::iterator iter1 = m_mapGroupPtr.begin();
	for (; iter1 != m_mapGroupPtr.end(); ++iter1)
	{
		CWarGroupPtr pWarGroupPtr = iter1->second;
		if (pWarGroupPtr == NULL)
			continue;

		if (pWarGroupPtr->m_nGroupID != idGroup)
			continue;

		if (pWarGroupPtr->m_nWorldLevel > lowworldlevel)
			continue;

		if (pWarGroupPtr->m_nWorldLevel == lowworldlevel)
			return iter1->second;

		if (pWarGroupPtr->m_nWorldLevel >= nearlevel)
			nearlevel = pWarGroupPtr->m_nWorldLevel;
	}

	idKey = idKey * 1000 + nearlevel;
	std::map<OBJID , CWarGroupPtr>::iterator iter = m_mapGroupPtr.find(idKey);
	if (iter != m_mapGroupPtr.end())
		return iter->second;

	return NULL;
}

//初始化城池NPC队伍
void CWorldWarMgr::initNpcTeam(CWarCityPtr pWarCityPtr, CWarGroupPtr pGroupPtr, uint8 type)
{
	//获取防守队伍怪物信息
	for (size_t i = 0; i < pGroupPtr->m_vecArms.size(); i++)
	{
		uint32 idNpc = pGroupPtr->m_vecArms[i];
		const Json::Value& pWarNpcConf = g_pConsummer->warNpcConf(idNpc);
		if (pWarNpcConf == Json::nullValue)
			continue;

		//判断是否已经有怪物信息
		if (pWarCityPtr->isHavaNPCTeam(idNpc))
			continue;

		CWarNPCTeamPtr pNpcTeamPtr = new CWarNPCTeam();
		pNpcTeamPtr->m_nID = idNpc;
		pNpcTeamPtr->m_nBatEff = pWarNpcConf["battle_effect"].asUInt();
		pNpcTeamPtr->m_nTeamLevel =  pWarNpcConf["team_level"].asUInt();
		pNpcTeamPtr->m_nTopHp = pWarNpcConf["tot_hp"].asUInt();
		pNpcTeamPtr->m_strName = pWarNpcConf["name"].asString();
		pNpcTeamPtr->m_strRateReward =  pWarNpcConf["rate_reward"].asString();
		pNpcTeamPtr->m_nFormationType = pWarNpcConf["formation_type"].asUInt();
		std::string strreward = pWarNpcConf["kill_reward"].asString();
		std::string strFormation = pWarNpcConf["formation"].asString();
		g_pConsummer->readRewardCfg(strreward, pNpcTeamPtr->m_vecKillReward, ";");
		this->convStr2Formation(strFormation, pNpcTeamPtr->m_vecGrid);
		pWarCityPtr->addInitNPCTeam(idNpc, pNpcTeamPtr);
	}

	//增加防守方队伍
	uint32 idNpcNum = pGroupPtr->m_vecArms.size();
	if (idNpcNum == 0)
		return;

	uint32 nTotNpcNum = 0;
	if (type == 0)
		nTotNpcNum = pWarCityPtr->getTotBlowNpcTeam();
	else
        nTotNpcNum = pWarCityPtr->getTotAttNpcTeam();
	uint32 iRounNum  = nTotNpcNum/idNpcNum;
	uint32 iLastNum = nTotNpcNum%idNpcNum;
	for (size_t i = 0; i < iRounNum; i++)
	{
		for (size_t j = 0; j < idNpcNum; j++)
		{
			CWarCityTeam cCityTeam;
			cCityTeam.m_nTeamID = pWarCityPtr->getTeamID();
			cCityTeam.m_nTeamType = 0; //0 NPC队伍 1玩家队伍
			cCityTeam.m_nNpcID =  pGroupPtr->m_vecArms[j];
			CWarNPCTeamPtr pNpcTeamPtr = pWarCityPtr->getNPCTeam(cCityTeam.m_nNpcID);
			if (pNpcTeamPtr == NULL)
				continue;

			cCityTeam.m_strName = pNpcTeamPtr->m_strName;
			cCityTeam.m_nNpcID = pNpcTeamPtr->m_nID;
			cCityTeam.m_nLevel = pNpcTeamPtr->m_nTeamLevel;
			cCityTeam.m_nBattEffect = pNpcTeamPtr->m_nBatEff;
			CTeamBatInf cBatInf;
			uint32 totHp = 0;
			for (size_t k = 0; k < pNpcTeamPtr->m_vecGrid.size(); k ++)
			{
				CWarNPCGrid &cNpcGrid = pNpcTeamPtr->m_vecGrid[k];
				cBatInf.m_nGrid = cNpcGrid.m_nGrid;
				CMonsterInitHPPtr pInitHpPtr = pWarCityPtr->getMonsterInitHp(cNpcGrid.m_nMonsterID);
				if (pInitHpPtr == NULL)
				{
					pInitHpPtr = new CMonsterInitHP();
					const Json::Value& monsterconf = g_pConsummer->monster(cNpcGrid.m_nMonsterID);
					if (monsterconf == Json::nullValue)
						continue;

					pInitHpPtr->m_nInitHP = monsterconf["hp"].asUInt();
					pInitHpPtr->m_nInitEN = monsterconf["en"].asUInt();
					pWarCityPtr->addMonsterInitHp(cNpcGrid.m_nMonsterID, pInitHpPtr);

				}

				totHp  += pInitHpPtr->m_nInitHP;
				cBatInf.m_nLastEn = pInitHpPtr->m_nInitEN;
				cBatInf.m_nLastHP = pInitHpPtr->m_nInitHP;
				cCityTeam.m_vecBatInf.push_back(cBatInf);
			}

			if (totHp != 0 && pNpcTeamPtr->m_nTopHp != totHp)
				pNpcTeamPtr->m_nTopHp = totHp;

			cCityTeam.m_nLastHP = pNpcTeamPtr->m_nTopHp;
			cCityTeam.m_nTotHP = pNpcTeamPtr->m_nTopHp;

			pWarCityPtr->addFightTeam(cCityTeam, type);
			if (pNpcTeamPtr->m_nFormationType == NPC_FORMATION_TYPE_CAPTAIN)
				pWarCityPtr->addCaptainTeam(cCityTeam.m_nTeamID);
			
			if (pNpcTeamPtr->m_nFormationType == NPC_FORMATION_TYPE_BLEW)
				pWarCityPtr->addBlewTeam(cCityTeam.m_nTeamID, type);

		}
	}

	for (size_t i = 0; i < iLastNum && i < pGroupPtr->m_vecArms.size(); i++)
	{
		CWarCityTeam cCityTeam;
		cCityTeam.m_nTeamID = pWarCityPtr->getTeamID();
		cCityTeam.m_nTeamType = TEAM_TYPE_NPC; //0 NPC队伍 1玩家队伍
		cCityTeam.m_nNpcID =  pGroupPtr->m_vecArms[i];
		CWarNPCTeamPtr pNpcTeamPtr = pWarCityPtr->getNPCTeam(cCityTeam.m_nNpcID);
		if (pNpcTeamPtr == NULL)
			continue;

		cCityTeam.m_strName = pNpcTeamPtr->m_strName;
		cCityTeam.m_nAvatarID = 0;
		cCityTeam.m_nLevel = pNpcTeamPtr->m_nTeamLevel;
		cCityTeam.m_nBattEffect = pNpcTeamPtr->m_nBatEff;
		uint32 tothp = 0;
		CTeamBatInf cBatInf;
		for (size_t k = 0; k < pNpcTeamPtr->m_vecGrid.size(); k ++)
		{
			CWarNPCGrid &cNpcGrid = pNpcTeamPtr->m_vecGrid[k];
			cBatInf.m_nGrid = cNpcGrid.m_nGrid;
			CMonsterInitHPPtr pInitHpPtr = pWarCityPtr->getMonsterInitHp(cNpcGrid.m_nMonsterID);
			if (pInitHpPtr == NULL)
			{
				pInitHpPtr = new CMonsterInitHP();
				const Json::Value& monsterconf = g_pConsummer->monster(cNpcGrid.m_nMonsterID);
				if (monsterconf == Json::nullValue)
					continue;

				pInitHpPtr->m_nInitHP = monsterconf["hp"].asUInt();
				pInitHpPtr->m_nInitEN = monsterconf["en"].asUInt();
				tothp += pInitHpPtr->m_nInitHP;
				pWarCityPtr->addMonsterInitHp(cNpcGrid.m_nMonsterID, pInitHpPtr);
			}

			cBatInf.m_nLastEn = pInitHpPtr->m_nInitEN;
			cBatInf.m_nLastHP = pInitHpPtr->m_nInitHP;
			cCityTeam.m_vecBatInf.push_back(cBatInf);
		}

		if (tothp != 0 && pNpcTeamPtr->m_nTopHp != tothp)
			pNpcTeamPtr->m_nTopHp = tothp;

		cCityTeam.m_nLastHP = pNpcTeamPtr->m_nTopHp;
		cCityTeam.m_nTotHP = pNpcTeamPtr->m_nTopHp;

		pWarCityPtr->addFightTeam(cCityTeam, type);
		if (pNpcTeamPtr->m_nFormationType == NPC_FORMATION_TYPE_CAPTAIN)
			pWarCityPtr->addCaptainTeam(cCityTeam.m_nTeamID);
		if (pNpcTeamPtr->m_nFormationType == NPC_FORMATION_TYPE_BLEW)
			pWarCityPtr->addBlewTeam(cCityTeam.m_nTeamID, type);
	}

	return;
}

//转换字符串 到阵型格子
void  CWorldWarMgr::convStr2Formation(std::string strFormation, std::vector<CWarNPCGrid> &vecGrid)
{
	CWarNPCGrid cGrid;
	std::vector<std::string> vecOption;
	splitString(strFormation, ";", vecOption, true);
	for (size_t i = 0; i < vecOption.size(); i++)
	{
		std::string strGrid = vecOption[i];
		uint32 ngrid = 0;
		uint32 nmonster = 0;
		if (sscanf(strGrid.c_str(), "%d:%d", &ngrid, &nmonster) != 2)
			continue;

		cGrid.m_nGrid =(uint8)ngrid - 1;
		cGrid.m_nMonsterID = nmonster;

		vecGrid.push_back(cGrid);
	}

	return;
}

//初始化城池NPC防守队伍
void CWorldWarMgr::initCityDefTeam(CWarCityPtr pWarCityPtr)
{
	uint8 idCity = pWarCityPtr->getCityID();
	uint8 idNation = pWarCityPtr->getNation();

	//获取城池信息
	CWorldCityPtr pWorldCityPtr = g_pConsummer->getWorldCityPtr(idCity);
	if (pWorldCityPtr == NULL)
		return;

	uint32 idBlowGroup = 0;
	for (size_t i = 0; i< pWorldCityPtr->m_vecNationGuard.size(); i++)
	{
		CNationGuard &cGuard = pWorldCityPtr->m_vecNationGuard[i];
		if (cGuard.m_nNation == idNation)
		{
			idBlowGroup = cGuard.m_nGuardID;
			break;
		}
	}

	//获取世界等级
	uint8 worldLevel = g_pConsummer->getWorldLevel();

	//获取防守方NPC信息
	CWarGroupPtr pBlowGroupPtr = this->getWarGroupPtr(idBlowGroup, worldLevel);
	if (pBlowGroupPtr == NULL)
		return;

	//设置队伍总数

	uint32 nTotArm = pBlowGroupPtr->m_nTotArms * pWorldCityPtr->m_nTeamRatio / 10000;
	pWarCityPtr->setTotBlowNpcTeam(nTotArm);

	//初始化防守方队伍
	this->initNpcTeam(pWarCityPtr, pBlowGroupPtr, 0);

	return;
}

//初始化城池NPC队伍
void CWorldWarMgr::initAccCityNpcTeam(CWarCityPtr pWarCityPtr)
{
	uint8 idAcc = pWarCityPtr->getAccID();

	//获取事件信息
	const Json::Value& pCityAccConf =  g_pConsummer->cityaccConf(idAcc);
	if (pCityAccConf == Json::nullValue)
		return;

	uint32 idAttackGroup = pCityAccConf["att_group_id"].asUInt();
	uint32 idBlowGroup = pCityAccConf["blow_group_id"].asUInt();

	uint8 worldLevel = g_pConsummer->getWorldLevel();

	//获取攻击方NPC信息
	CWarGroupPtr pAttGroupPtr = this->getWarGroupPtr(idAttackGroup, worldLevel);
	if (pAttGroupPtr == NULL)
		return;

	//设置队伍总数
	pWarCityPtr->setTotAttNpcTeam(pAttGroupPtr->m_nTotArms);

	//初始化攻击方队伍
	this->initNpcTeam(pWarCityPtr, pAttGroupPtr, 1);

	//获取防守方NPC信息
	CWarGroupPtr pBlowGroupPtr = this->getWarGroupPtr(idBlowGroup, worldLevel);
	if (pBlowGroupPtr == NULL)
		return;

	//设置队伍总数
	pWarCityPtr->setTotBlowNpcTeam(pBlowGroupPtr->m_nTotArms);

	//初始化防守方队伍
	this->initNpcTeam(pWarCityPtr, pBlowGroupPtr, 0);

	return;
}


//NPC主动单挑处理
void CWorldWarMgr::npcAutoPick(CWarCityPtr pWarCityPtr)
{
	//判断是否可以开战
	if (pWarCityPtr->getStatus() != 1)
		return;

	//判断CD时间
	CDateTime now;
	if (now.getTotalSecs() < pWarCityPtr->getAutoPickTime())
		return;

	pWarCityPtr->setAutoPickTime(now.getTotalSecs() + m_nAutoPickCd);

	//通知gateway
	m_vecGateWaySession.clear();
	g_pConsummer->getGateWaySession(m_vecGateWaySession);
	uint16 iGatewayNum = m_vecGateWaySession.size();
	if (iGatewayNum == 0)
		return;

	uint16 iGatewaySessiin = 0;
	bool bAutoPick = false;
	CWarCityTeam cNpcWarTeam;
	CWarCityTeam cPlayerWarTeam;

	for (size_t i = 0; i < 3; i++)
	{
		//获取单挑的队伍
		bAutoPick = pWarCityPtr->getAttPickCaptainTeam(cNpcWarTeam, cPlayerWarTeam);
		if (!bAutoPick)
			break;

		uint32 nGWSession = m_vecGateWaySession[iGatewaySessiin];
		iGatewaySessiin++;
		if (iGatewaySessiin >= iGatewayNum)
			iGatewaySessiin = 0;
        
		//单挑调用
		this->autoPickFight(pWarCityPtr, cNpcWarTeam, cPlayerWarTeam, nGWSession);
	}

	for (size_t i = 0; i < 3; i++)
	{
		//获取单挑的队伍
		bAutoPick = pWarCityPtr->getBlowPickCaptainTeam(cNpcWarTeam, cPlayerWarTeam);
		if (!bAutoPick)
			break;

		uint32 nGWSession = m_vecGateWaySession[iGatewaySessiin];
		iGatewaySessiin++;
		if (iGatewaySessiin >= iGatewayNum)
			iGatewaySessiin = 0;

		//单挑调用
		this->autoPickFight(pWarCityPtr, cNpcWarTeam,cPlayerWarTeam , nGWSession);
	}

	return;
}

//自动单挑处理
void CWorldWarMgr::autoPickFight(CWarCityPtr pWarCityPtr, CWarCityTeam &cAttWarTeam, CWarCityTeam &cBlowWarTeam, uint32 nGWSession)
{
	CGWWorldCityFightRequestPtr gwreq = new CGWWorldCityFightRequest();
	gwreq->m_nCityID = pWarCityPtr->getCityID();
	gwreq->m_nQueueNum = CITY_QUEUE_NUM_FIRST; //这个没有意义
	pWarCityPtr->convCityTeamBatTeam(cAttWarTeam,  gwreq->m_cAttBatTeam);
	pWarCityPtr->convCityTeamBatTeam(cBlowWarTeam, gwreq->m_cBlowBatTeam);

	IResponseHandlerPtr handler = new TResponseHandler<CWorldWarMgr>(this,
		&CWorldWarMgr::onResponseAutoPickTeamFight,
		&CWorldWarMgr::onTimeOut);
	g_DaServer->send(nGWSession, GW_WORLD_CITY_FIGHT, gwreq.get(), handler);
}

//城池车轮战
void CWorldWarMgr::cityWheelWar(CWarCityPtr pWarCityPtr)
{
	//判断是否可以开战
	if (pWarCityPtr->getStatus() != 1)
		return;

	//通知gateway
	m_vecGateWaySession.clear();
	g_pConsummer->getGateWaySession(m_vecGateWaySession);
	uint16 iGatewayNum = m_vecGateWaySession.size();
	if (iGatewayNum == 0)
		return;

	uint16 iGatewaySessiin = 0;

	CDateTime now;
	uint32 initNextTime = now.getTotalSecs() + m_nInitFightTime - 1;

	//循环每个堆栈，找出可以战斗的堆栈
	for (uint8 i = CITY_QUEUE_NUM_FIRST; i <= CITY_QUEUE_NUM_THIRTY; i++)
	{
	    //处理第一个对战堆栈
		if (pWarCityPtr->getQueueNextTime(i) < now.getTotalSecs())
	    {
		    this->wheelFightReq(pWarCityPtr, i, iGatewaySessiin, m_vecGateWaySession);
			pWarCityPtr->setQueueNextTime(i, initNextTime);
		}
	}

	return;
}
bool  CWorldWarMgr::isCanFightQueue(CWarCityPtr pWarCityPtr, uint8 nQueueNum)
{
	uint8 nQueueStatus = pWarCityPtr->getQueueStatus();
	bool isCanFight = false;
	if (nQueueNum == CITY_QUEUE_NUM_FIRST)
	{
		if (!(nQueueStatus & 1))
			isCanFight = true;
	}
	else if (nQueueNum == CITY_QUEUE_NUM_SECOND)
	{
		if (!(nQueueStatus & 2))
			isCanFight = true;
	}
	else
	{
		if (!(nQueueStatus & 4))
			isCanFight = true;
	}
	return isCanFight;
}
void CWorldWarMgr::wheelFightReq(CWarCityPtr pWarCityPtr, uint8 nQueueNum, uint16 &iGatewaySessiin, std::vector<uint32> &vecGateWaySession)
{
	//处理第三个对战堆栈
	bool isCanFight = this->isCanFightQueue(pWarCityPtr, nQueueNum);
	if (isCanFight)
	{
		this->fightTeamResult(pWarCityPtr, nQueueNum);
		isCanFight = this->isCanFightQueue(pWarCityPtr, nQueueNum);
		if (isCanFight)
		{
			CCitySTeam cCitySTeam;
			//获取前面两个队伍的信息,进行战斗攻击处理
			pWarCityPtr->getQueueSession(nQueueNum);
			pWarCityPtr->getSessionID();

			CGWWorldCityFightRequestPtr gwreq = new CGWWorldCityFightRequest();

			gwreq->m_nAccType = pWarCityPtr->getAccType();

			gwreq->m_nCityID = pWarCityPtr->getCityID();
			gwreq->m_nQueueNum = nQueueNum;
			pWarCityPtr->getBlowQueueFirstTeam(nQueueNum, cCitySTeam);
			pWarCityPtr->convQueueTeamBatTeam(cCitySTeam, gwreq->m_cBlowBatTeam);
			this->getPlayerBatInf(gwreq->m_cBlowBatTeam,  gwreq->m_nCityID, 1);

			pWarCityPtr->getAttQueueFirstTeam(nQueueNum, cCitySTeam);
			pWarCityPtr->convQueueTeamBatTeam(cCitySTeam, gwreq->m_cAttBatTeam);
			this->getPlayerBatInf(gwreq->m_cAttBatTeam, gwreq->m_nCityID);

			uint32 nGWSession = vecGateWaySession[iGatewaySessiin];
			iGatewaySessiin++;
			if (iGatewaySessiin >= vecGateWaySession.size())
				iGatewaySessiin = 0;

			IResponseHandlerPtr handler = new TResponseHandler<CWorldWarMgr>(this,
				&CWorldWarMgr::onResponseAutoCityTeamFight,
				&CWorldWarMgr::onTimeOut);
			g_DaServer->send(nGWSession, GW_WORLD_CITY_FIGHT, gwreq.get(), handler);
		}
	}
    
	//补丁处理
	if (pWarCityPtr->getQueueStatus() == 7)
	{
		pWarCityPtr->clearDirtyFightTeam();
	}

	return;
}

//战斗队伍结果处理
void CWorldWarMgr::fightTeamResult(CWarCityPtr pWarCityPtr, uint8 nQueueNum)
{
	if (pWarCityPtr == NULL)
		return;

	CCityFightTeam cFightTeam; //战斗队伍信息
	CTeamBatReport cbatReport;  //战报信息
	pWarCityPtr->getQueueFightTeam(nQueueNum, cFightTeam, cbatReport);
	if (cFightTeam.m_vecAttTeam.size() == 0 || cFightTeam.m_vecBlowTeam.size() == 0)
	{
		pWarCityPtr->checkQueueStatuts(nQueueNum);
		return;
	}

	uint8 faildir = cbatReport.m_nFailDir;

	CCitySTeam cAttTeam = cFightTeam.m_vecAttTeam[0];
	CCitySTeam cBlowTeam = cFightTeam.m_vecBlowTeam[0];
	CWarCityTeam cWarAttTeam;
	CWarCityTeam cWarBlowTeam;
	cWarAttTeam.m_nAvatarID = 0;
	cWarBlowTeam.m_nAvatarID = 0;

	pWarCityPtr->getInFightTeam(cAttTeam.m_nTeamID, cWarAttTeam);
	pWarCityPtr->getInFightTeam(cBlowTeam.m_nTeamID, cWarBlowTeam);
	uint32 lastHP = pWarCityPtr->getQueueLastHP(nQueueNum);
	//失败方 1攻击者 2防守者
	if (faildir == WAR_DIR_TYPE_ATT) 
	{
		//更新获胜队伍的血量
		pWarCityPtr->updSucTeamGrid(nQueueNum, cWarBlowTeam.m_nTeamID);
		if (cWarBlowTeam.m_nTeamType == TEAM_TYPE_PLAYER)
		{
			this->playerSuccFight(pWarCityPtr, cWarBlowTeam.m_nAvatarID, cWarBlowTeam.m_nTeamID);
			//获取战胜怪物奖励
			if (cWarAttTeam.m_nTeamType == TEAM_TYPE_NPC)
				this->rewardPlayerKillMonster(pWarCityPtr, cWarBlowTeam.m_nAvatarID, cWarAttTeam.m_nNpcID);
			else
				this->rewardPlayerKillPlayer(pWarCityPtr, cWarBlowTeam.m_nAvatarID);
			this->addPlayerKillNum(cWarBlowTeam.m_nAvatarID);
		}

		//通知玩家队伍被击杀
		if (cWarAttTeam.m_nTeamType == TEAM_TYPE_PLAYER)
		{
			this->playerWarNotice(cWarAttTeam.m_nAvatarID, WORLD_WAR_TEAM_END, pWarCityPtr->getCityID(), "");
		}

		pWarCityPtr->setFightQueueTeamLastHp(nQueueNum);
		pWarCityPtr->updSteamLastHP(nQueueNum, lastHP, faildir);
		//清理堆栈前失败的队伍
		pWarCityPtr->clearQueueFailTeam(nQueueNum);
	}
	else if (faildir == WAR_DIR_TYPE_BLOW)
	{
		//更新获胜队伍的血量
		pWarCityPtr->updSucTeamGrid(nQueueNum, cWarAttTeam.m_nTeamID);
		if (cWarAttTeam.m_nTeamType == TEAM_TYPE_PLAYER)
		{
			this->playerSuccFight(pWarCityPtr, cWarAttTeam.m_nAvatarID, cWarAttTeam.m_nTeamID);
			if (cWarBlowTeam.m_nTeamType == TEAM_TYPE_NPC)
				this->rewardPlayerKillMonster(pWarCityPtr, cWarAttTeam.m_nAvatarID, cWarBlowTeam.m_nNpcID);
			else
				this->rewardPlayerKillPlayer(pWarCityPtr, cWarAttTeam.m_nAvatarID);
			this->addPlayerKillNum(cWarAttTeam.m_nAvatarID);
		}

		//通知玩家队伍被击杀
		if (cWarBlowTeam.m_nTeamType == TEAM_TYPE_PLAYER)
		{
			this->playerWarNotice(cWarBlowTeam.m_nAvatarID, WORLD_WAR_TEAM_END, pWarCityPtr->getCityID(), "");
		}

		pWarCityPtr->setFightQueueTeamLastHp(nQueueNum);
		pWarCityPtr->updSteamLastHP(nQueueNum, lastHP, faildir);
		//清理堆栈前失败的队伍
		pWarCityPtr->clearQueueFailTeam(nQueueNum);
	}
	else  //双方都阵亡的情况
	{
		log_debug("fight reulst is not success  ");
		pWarCityPtr->clearQueueAllFailTeam(nQueueNum);
		if (cWarAttTeam.m_nTeamType == TEAM_TYPE_PLAYER)
		{
			this->playerDieSuccFight(pWarCityPtr, cWarAttTeam.m_nAvatarID, cWarAttTeam.m_nTeamID);
			if (cWarBlowTeam.m_nTeamType == TEAM_TYPE_NPC)
				this->rewardPlayerKillMonster(pWarCityPtr, cWarAttTeam.m_nAvatarID, cWarBlowTeam.m_nNpcID);
			else
				this->rewardPlayerKillPlayer(pWarCityPtr, cWarAttTeam.m_nAvatarID);

			this->addPlayerKillNum(cWarAttTeam.m_nAvatarID);
		}

		if (cWarBlowTeam.m_nTeamType == TEAM_TYPE_PLAYER)
		{
			this->playerDieSuccFight(pWarCityPtr, cWarBlowTeam.m_nAvatarID, cWarBlowTeam.m_nTeamID);
			//获取战胜怪物奖励
			if (cWarAttTeam.m_nTeamType == TEAM_TYPE_NPC)
				this->rewardPlayerKillMonster(pWarCityPtr, cWarBlowTeam.m_nAvatarID, cWarAttTeam.m_nNpcID);
			else
				this->rewardPlayerKillPlayer(pWarCityPtr, cWarBlowTeam.m_nAvatarID);

			this->addPlayerKillNum(cWarBlowTeam.m_nAvatarID);
		}
	}

	pWarCityPtr->getSessionID();

	//获取刚进入战斗的玩家队伍
	OBJID idPlayer = 0;
	uint32 idTeam = 0;
	pWarCityPtr->getInFightPlayer(idPlayer, idTeam);
	if (idPlayer != 0)
	{
		char sBuf[128]={0};
		sprintf(sBuf, "%d", idTeam);
		std::string strTeam = std::string(sBuf);
		//通知玩家上阵
		this->playerWarNotice(idPlayer, WORLD_WAR_TEAM_FIGHT, pWarCityPtr->getCityID(), strTeam);
	}
	return;
}
void CWorldWarMgr::addPlayerKillNum(OBJID idPlayer)
{
	if (idPlayer == 0)
		return;

	if (!g_pConsummer->isFuncAccDay(6))
		return;
    
	//判断是否需要进行时间处理
	if (m_nPrinceAccFlag == 0)
		return;

	uint32 nNum = 1;
	std::map<OBJID, uint32>::iterator iter = m_mapPlayerKillNum.find(idPlayer);
	if (iter == m_mapPlayerKillNum.end())
	{
		CWarJoinPlayerPtr pWarPlayer = this->getWarJoinPlayer(idPlayer);
		if (pWarPlayer == NULL)
			return;
		m_mapPlayerKillNum[idPlayer] = 1;
		CDBPrinceKillRank cKillRank;
		cKillRank.m_nKillNum = 1;
		cKillRank.m_nLevel = pWarPlayer->m_nLevel;
		cKillRank.m_nNation = pWarPlayer->m_nNation;
		cKillRank.m_nPlayerID = idPlayer;
		cKillRank.m_strName = pWarPlayer->m_strName;
		m_vecPrinceKillRank.push_back(cKillRank);
	}
	else
	{
		nNum = iter->second + 1;
		m_mapPlayerKillNum[idPlayer] = nNum;
		for (size_t i = 0; i < m_vecPrinceKillRank.size(); i++)
		{
			CDBPrinceKillRank &cKillRank = m_vecPrinceKillRank[i];
			if (cKillRank.m_nPlayerID == idPlayer)
			{
				cKillRank.m_nKillNum = nNum; 
				break;
			}
		}
        
		if (m_vecPrinceKillRank.size() > 2)
		    sort(m_vecPrinceKillRank.begin(), m_vecPrinceKillRank.end(), comparekillrank);
	}

	if (nNum > 60)
		log_info("kill nNum= %d player  "I64_FMT" ", nNum, idPlayer);

	//判断是否触发奖励，需要进行通知
	uint16 nRewardNum = (uint16)nNum;
	std::map<uint16, std::vector<CRewardInfo> >::iterator iter_reward = m_mapKillNumReward.find(nRewardNum);
	if (iter_reward != m_mapKillNumReward.end())
		this->playerWarNotice(idPlayer, WORLD_WAR_HAVE_PRINCE_REWARD, 0, "");

	return;
}

//自动战斗应答处理
void CWorldWarMgr::onResponseAutoCityTeamFight(IResponsePtr pResponse, CCurrentPtr Current, CRefSharePtr Param)
{
	CGWWorldCityFightResponsePtr presponse = new CGWWorldCityFightResponse(pResponse->getSerialStream());
	if (presponse->m_nRetCode != SUCCESS)
		return;

	CDateTime now;

	uint8 idCity = presponse->m_nCityID;
	//获取城战事件信息
	CWarCityPtr pWarCityPtr = this->getWarCityPtr(idCity);
	if (pWarCityPtr == NULL)
		return;

	//保存战报
	pWarCityPtr->setQueueBatReport(presponse->m_nQueueNum, presponse->m_strBatReport);
	//设置剩余血量信息
	pWarCityPtr->setFightQueueGrid(presponse->m_nQueueNum,presponse->m_vecGrid);

	//保存战斗简报
	CTeamBatReport cTeamBatReport;
	cTeamBatReport.m_nAttLoseHP = presponse->m_nAttackBlood;
	cTeamBatReport.m_nBlowLoseHP=presponse->m_nBlowBlood;
	if (presponse->m_nFightRet == 0)
	{
		cTeamBatReport.m_nFailDir = WAR_DIR_TYPE_BLOW; //失败方 1攻击者 2防守者
		pWarCityPtr->setTeamLastHP(presponse->m_nAttTeamID, presponse->m_nLastBlood);
		cTeamBatReport.m_nBlowLoseHP = pWarCityPtr->getFightTeamHp(presponse->m_nBlowTeamID);
	}
	else if (presponse->m_nFightRet == 1)
	{
		cTeamBatReport.m_nFailDir = WAR_DIR_TYPE_ATT; //失败方 1攻击者 2防守者
		pWarCityPtr->setTeamLastHP(presponse->m_nBlowTeamID, presponse->m_nLastBlood);
		cTeamBatReport.m_nAttLoseHP = pWarCityPtr->getFightTeamHp(presponse->m_nAttTeamID);
	}
	else
	{
		cTeamBatReport.m_nFailDir = WAR_DIR_TYPE_ALL; //失败方 1攻击者 2防守者 3 全体（自爆兵的情况）
		pWarCityPtr->setTeamLastHP(presponse->m_nBlowTeamID, presponse->m_nLastBlood);
		cTeamBatReport.m_nAttLoseHP = pWarCityPtr->getFightTeamHp(presponse->m_nAttTeamID);
	}

	//修改队伍的剩余的血量
	pWarCityPtr->setQueueLastHP(presponse->m_nQueueNum, presponse->m_nLastBlood);

	cTeamBatReport.m_nFightCount = presponse->m_nFightCount;
	cTeamBatReport.m_nQueueID = presponse->m_nQueueNum;
	uint32 nFightCount = cTeamBatReport.m_nFightCount;
	uint32 nexttime = (m_nCountFightTime * nFightCount)/1000;
	if (nexttime < m_nInitFightTime)
		nexttime = m_nInitFightTime;

	cTeamBatReport.m_nNextTime = now.getTotalSecs() + nexttime;
	pWarCityPtr->setQueueTeamBatReport(presponse->m_nQueueNum, cTeamBatReport);

	return;
}

//获取玩家战斗信息
void  CWorldWarMgr::getPlayerBatInf(CWorldBatTeam &cWorldBatTeam, uint8 idCity, uint8 nIsBlow/*=0*/)
{
	if (cWorldBatTeam.m_nType == TEAM_TYPE_PLAYER)
	{
		if (cWorldBatTeam.m_vecPlayerInf.size() == 0)
			return;

		CPlayerFightTeamInf &cTeamInf = cWorldBatTeam.m_vecPlayerInf[0];
		OBJID idPlayer = cTeamInf.m_nPlayerID;
		
		//获取玩家信息
		CWarJoinPlayerPtr pWarPlayer = this->getWarJoinPlayer(idPlayer);
		if (pWarPlayer != NULL)
		{
			cTeamInf.m_nSex = pWarPlayer->m_nSex;
			cTeamInf.m_strName = pWarPlayer->m_strName;
			cTeamInf.m_nLevel = pWarPlayer->m_nLevel;
			//获取城池的阵营BUFF加成
			this->getCityNationBuff(idCity, pWarPlayer->m_nNation, cTeamInf.m_vecBuff);

			//获取弱国保护的BUFF
			this->getWeakNationBuf(pWarPlayer->m_nNation, cTeamInf.m_vecBuff);

			//太守守城buff
			if (nIsBlow != 0)
			{
				std::map<OBJID, uint8>::iterator it = this->m_mapPlayerSatrapCity.find(idPlayer);
				if (it != this->m_mapPlayerSatrapCity.end())
				{
					if (it->second != 0) {
						CWarBuff buf;
						buf.m_nNum = 1;
						buf.m_nBuffID = m_nSatrapBlowFightBuff;
						cTeamInf.m_vecBuff.push_back(buf);
					}
				}
			}
		}
	}

	return;
}

//获取城池阵营的BUFF
void CWorldWarMgr::getCityNationBuff(uint8 idCity, uint8 idNation, std::vector<CWarBuff> &vecBuff)
{
	//根据城池获取城池BUFF
	CWorldCityPtr pWorldCityPtr = g_pConsummer->getWorldCityPtr(idCity);
	if (pWorldCityPtr != NULL)
	{
		for(size_t i = 0; i < pWorldCityPtr->m_vecNationBuff.size(); i++)
		{
			CNationBuff &cBuff = pWorldCityPtr->m_vecNationBuff[i];
			CWarBuff cWarBuff;
			if (cBuff.m_nNation == idNation)
			{
				for (size_t j = 0; j < cBuff.m_vecBuff.size(); j++)
				{
					cWarBuff.m_nBuffID = cBuff.m_vecBuff[i];
					cWarBuff.m_nNum = 1;
					vecBuff.push_back(cWarBuff);
				}
			}
		}
	}

	return;
}

//世界登录
IResponsePtr CWorldWarMgr::executeWorldLogin(IRequestPtr Request, CCurrentPtr Current)
{
	CGWMWorldLoginRequestPtr pGWMWLRequest = new CGWMWorldLoginRequest(Request->getSerialStream());
	CGWMWorldLoginResponsePtr pGWMWLResponse = new CGWMWorldLoginResponse();


	pGWMWLResponse->m_nRetCode = SUCCESS;
	pGWMWLResponse->m_nTotPraiseCnt = m_nTotPraiseCnt;
	return pGWMWLResponse.get();
}

//国战点赞
IResponsePtr CWorldWarMgr::executeWorldPraise(IRequestPtr Request, CCurrentPtr Current)
{
	CGWMWorldPraiseResponsePtr pGWMWPRequest = new CGWMWorldPraiseResponse();

	//更新国战点赞
	m_nTotPraiseCnt ++;

	//更新数据库
	CDBSetSystemVarRequestPtr pDBSSVRequest = new CDBSetSystemVarRequest();
	pDBSSVRequest->m_nVarID = SYSTEM_VARS_PRAISE;
	pDBSSVRequest->m_nVarData = (OBJID)m_nTotPraiseCnt;
	pDBSSVRequest->m_strMemo = "worldwar praise num";

	g_DaServer->callServant(DB_SET_SYSTEM_VAR, pDBSSVRequest.get(), NULL, 0, NULL, NULL);

	pGWMWPRequest->m_nRetCode = SUCCESS;
	return pGWMWPRequest.get();
}

//记录功勋宝箱通告
IResponsePtr CWorldWarMgr::executeRcdExploitLibNotice(IRequestPtr Request, CCurrentPtr Current)
{
	CGWMRcdExploitLibNoticeRequestPtr pGWMRELNRequest = new CGWMRcdExploitLibNoticeRequest(Request->getSerialStream());
	CGWMRcdExploitLibNoticeResponsePtr pGWMRELNResponse = new CGWMRcdExploitLibNoticeResponse();

	//密友面板通知
	CBytesBufferPtr buf = CBytesBufferPtr::createInstance();
	pushChatPlayerInfo( buf, pGWMRELNRequest->m_nPlayerId, pGWMRELNRequest->m_strNickName);
	pushChatConfItemInfo( buf, pGWMRELNRequest->m_nLibId);
	std::string strmsg = g_pConsummer->formatLangString(LANGTYPE_SHOWOFF,LANGTYPE_SHOWOFF_EXPLOIT_LIB);
	sendSystemNotice(CHANNEL_CHAT_EXPLOIT, strmsg, buf);

	//更新最新面板记录（5条）
	CExploitLibInfo cExploitLibInfo;
	cExploitLibInfo.m_nPlayerId = pGWMRELNRequest->m_nPlayerId;
	cExploitLibInfo.m_strPlayerNickName = pGWMRELNRequest->m_strNickName;
	cExploitLibInfo.m_nLibId = pGWMRELNRequest->m_nLibId;

	uint8 nationId = 0; //当前版本不分国家
	std::map<uint8, std::vector<CExploitLibInfo> > ::iterator itMELI = m_mapExploitLibRecord.find(nationId);
	if( itMELI == m_mapExploitLibRecord.end() )
	{
		std::vector<CExploitLibInfo> vecLibInfo;
		vecLibInfo.push_back(cExploitLibInfo);

		m_mapExploitLibRecord.insert(make_pair(nationId, vecLibInfo));
	}
	else
	{
		while( itMELI->second.size() >= 5 )
		{
			std::vector<CExploitLibInfo> ::iterator itVELI = itMELI->second.begin();
			itMELI->second.erase(itVELI);
		}

		itMELI->second.push_back(cExploitLibInfo);
	}

	pGWMRELNResponse->m_nRetCode = SUCCESS;
	return pGWMRELNResponse.get();
}

//清理玩家的事件队伍信息
void CWorldWarMgr::clearWarPlayerTeam(uint8 idCity)
{
	std::map<OBJID, CWarJoinPlayerPtr>::iterator iter = m_mapWarJoinPlayer.begin();
	for (; iter != m_mapWarJoinPlayer.end(); ++iter)
	{
		CWarJoinPlayerPtr pWarPlaytrPtr = iter->second;
		if (pWarPlaytrPtr == NULL)
			continue;

		pWarPlaytrPtr->m_mapBuddy.erase(idCity);
		std::vector<CPlayerCityWarInf>::iterator iter_team = pWarPlaytrPtr->m_vecCityWarInf.begin();
		for (; iter_team != pWarPlaytrPtr->m_vecCityWarInf.end(); ++iter_team)
		{
			if (iter_team->m_nCityID == idCity)
			{
				pWarPlaytrPtr->m_vecCityWarInf.erase(iter_team);
				break;
			}
		}
	}

	return;
}

//校验是否是相邻管辖的城池
bool CWorldWarMgr::checkConnectAdminCity(uint8 idNation, uint8 idCity)
{
	CWorldCityPtr cCityPtr = g_pConsummer->getWorldCityPtr(idCity);
	if (cCityPtr == NULL)
		return false;

	//判断城池是否属于管辖范围
	std::map<uint8, uint8>::iterator iter =  m_mapCityOccup.find(idCity);
	if (iter != m_mapCityOccup.end())
	{
		if (iter->second == idNation)
			return false;
	}
	else
	{
		if (cCityPtr->m_nNation == idNation)
			return false;
	}

	//判断相邻的城池是否有属于国家管辖的
	for (size_t i = 0; i < cCityPtr->m_vecCity.size(); i++)
	{
		uint8 idConnectCity = cCityPtr->m_vecCity[i];

		std::map<uint8, uint8>::iterator iter =  m_mapCityOccup.find(idConnectCity);
		if (iter != m_mapCityOccup.end())
		{
			if (iter->second == idNation)
				return true;
		}

		//获取相邻城池的信息
		CWorldCityPtr cConnectCityPtr = g_pConsummer->getWorldCityPtr(idConnectCity);
		if (cConnectCityPtr != NULL)
		{
			//判断是否是属于国家管辖
			if (cConnectCityPtr->m_nNation == idNation)
				return true;
		}
	}

	return false;
}


//获取宣战事件ID
uint8 CWorldWarMgr::getDeclareAccID()
{
	m_nDeclareAccID++;

	if (m_nDeclareAccID > 250)
		m_nDeclareAccID = 160;

	return m_nDeclareAccID;
}

//获取保护城池信息
CProjectCityPtr CWorldWarMgr::getProCityPtr(uint8 idCity)
{
	std::map<uint8, CProjectCityPtr>::iterator iter = m_mapProCity.find(idCity);
	if (iter == m_mapProCity.end())
		return NULL;

	return iter->second;
}

//删除队伍
void CWorldWarMgr::delEtapeTeam(std::vector<uint32> &vecTeamID)
{
	if (vecTeamID.size() == 0)
		return;

	CDBDelEtapeDeployRequestPtr dbreq = new CDBDelEtapeDeployRequest();
	dbreq->m_vecTeamID.assign(vecTeamID.begin(), vecTeamID.end());

	g_DaServer->callServant(DB_DEL_ETAPE_DEPLOY, dbreq.get(), NULL, 0, NULL, NULL);

	return;
}

//获取弱国保护的BUFF
void CWorldWarMgr::getWeakNationBuf(uint8 idNation, std::vector<CWarBuff> &vecBuff)
{
	//获取阵营管辖的城池数量
	uint8 nNum = this->getNaitonAdminCityNum(idNation);

	//获取城池数量获取的BUFF加成
	std::map<uint8, std::vector<uint32> >::iterator iter_buff = m_mapWeakNationBuff.begin();
	for (; iter_buff != m_mapWeakNationBuff.end(); ++iter_buff)
	{
		uint8 nAdminNum = iter_buff->first;
		if (nNum <= nAdminNum)
		{
			std::vector<uint32> &vecBuffID = iter_buff->second;
			CWarBuff cBuff;
			for (size_t i = 0; i < vecBuffID.size(); i++)
			{
				cBuff.m_nBuffID = vecBuffID[i];
				cBuff.m_nNum = 1;
				vecBuff.push_back(cBuff);
			}
		}
	}

	

	return;
}

//给太守发放俸禄
void CWorldWarMgr::sendAwardMailToCitySatrap() {

	std::string sContent;
	std::string strTitle = g_pConsummer->formatLangString(LANGTYPE_WORLD_CITY, LANGTYPE_WORLD_CITY_SARALY_TILE);

	std::map<uint8, CSatrapCityPtr>::iterator it = this->m_mapSatrapCity.begin();
	for (; it != this->m_mapSatrapCity.end(); ++ it) {

		CSatrapCityPtr pCitySatrap = it->second;
		if (NULL == pCitySatrap) {
			continue;
		}

		OBJID idPlayer = pCitySatrap->m_nSatrapPlayer;
		if (0 == idPlayer) {
			continue;
		}

		uint8 nCity = it->first;
		CWorldCityPtr pWarCity = g_pConsummer->getWorldCityPtr(nCity);
		if (NULL == pWarCity) {
			continue;
		}

		//发送邮件奖励
		sContent = g_pConsummer->formatLangString(LANGTYPE_WORLD_CITY, LANGTYPE_WORLD_CITY_SARALY_CONTENT, pWarCity->m_strName.c_str());
		g_pConsummer->sendMailToPlayer(idPlayer, strTitle, sContent, pWarCity->m_vecCitySatrapAward, REWARD_SOURCE_CITY_SATRAP);
	}
}


//每日清算
void CWorldWarMgr::dailyAcc()
{
	//更新国家国战清算
	this->updWarNation(NULL, 0);
	std::map<uint8, CWarNationPtr>::iterator iter_nation = m_mapWarNation.begin();
	for (; iter_nation != m_mapWarNation.end(); ++iter_nation)
	{
		CWarNationPtr pNationPtr = iter_nation->second;
		if (pNationPtr == NULL)
			continue;

		pNationPtr->setDenNum(0);
		pNationPtr->clearDomainReward();

	}

	//进行国家疆域产出奖励处理
	this->nationDomainReward(1);

	//给太守发放俸禄
	this->sendAwardMailToCitySatrap();
	this->rewardKillRank(); //排名奖励发送 
	this->rewardKillNum(); //个人杀敌奖励
	this->regKillRankDB();  //记录排行数据到数据库
	return;
}

//获取驻守城池信息
CSatrapCityPtr CWorldWarMgr::getSatrapCityInf(uint8 nCity) {
	std::map<uint8, CSatrapCityPtr>::iterator it = this->m_mapSatrapCity.find(nCity);
	if (it == this->m_mapSatrapCity.end()) {
		return NULL;
	}
	return it->second;
}


//查询城池驻守名将
IResponsePtr CWorldWarMgr::executeQuerySatrapBuddy(IRequestPtr Request,CCurrentPtr Current) 
{
	CGwmQueryCitySatrapBuddyRequestPtr pgwreq = new CGwmQueryCitySatrapBuddyRequest(Request->getSerialStream());
	CGwmQueryCitySatrapBuddyResponsePtr pgwrsp = new CGwmQueryCitySatrapBuddyResponse();

	//获取城池信息
	CWorldCityPtr cWorldCityPtr  = g_pConsummer->getWorldCityPtr(pgwreq->m_nCityId);
	if (cWorldCityPtr == NULL)
	{
		pgwrsp->m_nRetCode = INVALID_PARAM;
		return pgwrsp.get();
	}

	CSatrapCityPtr pSatrapCity = this->getSatrapCityInf(pgwreq->m_nCityId);
	if (NULL == pSatrapCity) 
	{
		pgwrsp->m_nRetCode = SUCCESS;
		pgwrsp->m_nCityId = pgwreq->m_nCityId;
		std::string strName = g_pConsummer->formatLangString(LANGTYPE_WORLD_CITY, LANGTYPE_WORLD_CITY_SATRAP);
		pgwrsp->m_strNick = cWorldCityPtr->m_strName + strName;
		return pgwrsp.get();
	}

	pgwrsp->m_nRetCode = SUCCESS;
	pgwrsp->m_nCityId = pgwreq->m_nCityId;
	pgwrsp->m_strNick = pSatrapCity->m_strNickName;
	return pgwrsp.get();
}

//查询城池太守信息
IResponsePtr CWorldWarMgr::executeQuerySatrapInf(IRequestPtr Request,CCurrentPtr Current) 
{
	CGwmQueryCitySatrapInfRequestPtr pgwreq = new CGwmQueryCitySatrapInfRequest(Request->getSerialStream());
	CGwmQueryCitySatrapInfResponsePtr pgwrsp = new CGwmQueryCitySatrapInfResponse();

	//获取城池信息
	CWorldCityPtr cWorldCityPtr  = g_pConsummer->getWorldCityPtr(pgwreq->m_nCityId);
	if (cWorldCityPtr == NULL)
	{
		pgwrsp->m_nRetCode = INVALID_PARAM;
		return pgwrsp.get();
	}

	CSatrapCityPtr pSatrapCity = this->getSatrapCityInf(pgwreq->m_nCityId);
	if (NULL != pSatrapCity) 
	{
		CCitySatrapInf cInf;
		cInf.m_nFightCD = pSatrapCity->m_nFightCd;
		cInf.m_nBattleEffect = pSatrapCity->m_nBattleEffect;
		cInf.m_strNickName = pSatrapCity->m_strNickName;
		//查询在阵名将sortid
		for (int i = 0; i < pSatrapCity->m_vecFormat.size(); ++ i) 
		{
			OBJID idBuddy = pSatrapCity->m_vecFormat[i].m_nBuddyID;
			for (int j = 0; j < pSatrapCity->m_vecBuddy.size(); ++ j) 
			{
				if (pSatrapCity->m_vecBuddy[j].m_nBuddyID != idBuddy) 
				{
					continue;
				}
				cInf.m_vecBuddySortId.push_back(pSatrapCity->m_vecBuddy[j].m_nBuddySort);
				break;
			}
		}

		pgwrsp->m_vecCitySatRapInf.push_back(cInf);
	}

	pgwrsp->m_nRetCode = SUCCESS;
	pgwrsp->m_nCityId = pgwreq->m_nCityId;
	return pgwrsp.get();
}

//太守调整阵形
IResponsePtr CWorldWarMgr::executeCitySatrapAdjustFormation(IRequestPtr Request,CCurrentPtr Current) {
	CGwmAdjustSatrapFormationRequestPtr pgwreq = new CGwmAdjustSatrapFormationRequest(Request->getSerialStream());
	CGwmAdjustSatrapFormationResponsePtr pgwrsp = new CGwmAdjustSatrapFormationResponse();

	std::map<OBJID, uint8>::iterator it = this->m_mapPlayerSatrapCity.find(pgwreq->cFightInf.m_nSatrapPlayer);
	if (it == this->m_mapPlayerSatrapCity.end())
	{
		pgwrsp->m_nRetCode = INVALID_PARAM;
		return pgwrsp.get();
	}
	pgwreq->cFightInf.m_nCityId = it->second;

	CSatrapCityPtr pSatrapCity = this->getSatrapCityInf(pgwreq->cFightInf.m_nCityId);
	if (NULL == pSatrapCity) 
	{
		pgwrsp->m_nRetCode = INVALID_PARAM;
		return pgwrsp.get();
	}

	OBJID idPlayer = pgwreq->cFightInf.m_nSatrapPlayer;
	if (pSatrapCity->m_nSatrapPlayer != idPlayer)
	{
		pgwrsp->m_nRetCode = SATRAP_CITY_NO_MASTER;
		return pgwrsp.get();
	}

	pSatrapCity->m_nLevel = pgwreq->cFightInf.m_nLevel;
	pSatrapCity->m_nBattleEffect = pgwreq->cFightInf.m_nBattleEffect;
	pSatrapCity->m_strProfessionLevel = pgwreq->cFightInf.m_strProfessionLevel;
	pSatrapCity->m_cDragon = pgwreq->cFightInf.m_cDragon;
	pSatrapCity->m_vecBuddy.assign(pgwreq->cFightInf.m_vecBuddy.begin(), pgwreq->cFightInf.m_vecBuddy.end());
	pSatrapCity->m_vecFormat.assign(pgwreq->cFightInf.m_vecFormat.begin(), pgwreq->cFightInf.m_vecFormat.end());
	
	pgwrsp->m_nRetCode = SUCCESS;
	return pgwrsp.get();
}

void CWorldWarMgr::converCitySatrap2Str(uint8 idCity, std::string& strDst, CSatrapCityPtr pSatrapCity/*=NULL*/) {
	strDst.clear();

	if (NULL == pSatrapCity) {
		pSatrapCity = this->getSatrapCityInf(idCity);
		if (NULL == pSatrapCity) {
			return;
		}
	}
	
	char strBuf[128];
	for (size_t i = 0; i < pSatrapCity->m_vecFormat.size(); ++ i) 
	{
		uint32 nGrid = pSatrapCity->m_vecFormat[i].m_nGrid;
		OBJID idBuddy = pSatrapCity->m_vecFormat[i].m_nBuddyID;
		sprintf(strBuf, "%d:"I64_FMT"", nGrid, idBuddy);
		if (0 != strDst.size()) {
			strDst += std::string(";");
		}
		strDst += std::string(strBuf);
	}
}

void CWorldWarMgr::updateDBCitySatrapInf(uint8 idCity) 
{
	CSatrapCityPtr pSatrapCity = this->getSatrapCityInf(idCity);
	if (NULL == pSatrapCity) 
		return;

	//更新db
	CDBUpdSatrapCityInfRequestPtr pdbreq = new CDBUpdSatrapCityInfRequest();
	pdbreq->m_nPlayerID = pSatrapCity->m_nSatrapPlayer;
	pdbreq->m_nCityId = idCity;
	pdbreq->m_nBattle = pSatrapCity->m_nBattleEffect;
	this->converCitySatrap2Str(idCity, pdbreq->m_strGridInf, pSatrapCity);
	g_DaServer->callServant(DB_UPD_CITY_SATRAP, pdbreq.get(), NULL);
}

//太守弃城
IResponsePtr CWorldWarMgr::executeCitySatrapAbort(IRequestPtr Request,CCurrentPtr Current) 
{
	CGwmAbandonSatrapCityRequestPtr pgwreq = new CGwmAbandonSatrapCityRequest(Request->getSerialStream());
	CGwmAbandonSatrapCityResponsePtr pgwrsp = new CGwmAbandonSatrapCityResponse();

	OBJID idPlayer = pgwreq->m_nPlayerId;

	//获取玩家的太守城池
    uint8 idCity = this->getPlayerSatrapCity(idPlayer);
	if (idCity == 0)
	{
		pgwrsp->m_nRetCode = SATRAP_CITY_NO_MASTER;
		return pgwrsp.get();
	}

	CSatrapCityPtr pSatrapCity = this->getSatrapCityInf(idCity);
	if (NULL == pSatrapCity) 
	{
		pgwrsp->m_nRetCode = INVALID_PARAM;
		return pgwrsp.get();
	}

	if (pSatrapCity->m_nSatrapPlayer != idPlayer) 
	{
		pgwrsp->m_nRetCode = SATRAP_CITY_NO_MASTER;
		return pgwrsp.get();
	}

	//更新玩家对应的城池太守信息
	this->m_mapPlayerSatrapCity.erase(idPlayer);

	//更新db
	CDBDelCitySatrapRequestPtr dbreq = new CDBDelCitySatrapRequest();
	dbreq->m_nCityID = idCity;
	g_DaServer->callServant(DB_DEL_CITY_SATRAP, dbreq.get(), NULL);

	m_mapSatrapCity.erase(idCity);

	//发送太守主动弃城邮件
	CWorldCityPtr pWarCityInf = g_pConsummer->getWorldCityPtr(idCity);
	if (NULL != pWarCityInf)
	{
		std::vector<CRewardInfo> vecAward;
		std::string strTitle = g_pConsummer->formatLangString(LANGTYPE_WORLD_CITY, LANGTYPE_WORLD_CITY_GIVE_UP_CITY_TILE);
		std::string sContent = g_pConsummer->formatLangString(LANGTYPE_WORLD_CITY, LANGTYPE_WORLD_CITY_GIVE_UP_CITY_CONTENT, pWarCityInf->m_strName.c_str());
		g_pConsummer->sendMailToPlayer(idPlayer, strTitle, sContent, vecAward, REWARD_SOURCE_CITY_SATRAP);
	}
	
	pgwrsp->m_nRetCode = SUCCESS;
	pgwrsp->m_nCityId = pgwreq->m_nCityId;
	pgwrsp->m_nType = pgwreq->m_nType;
	return pgwrsp.get();
}


//获取太守战斗数据
IResponsePtr CWorldWarMgr::executeGetCitySatrapFightData(IRequestPtr Request,CCurrentPtr Current) 
{
	CGwmQueryCitySatrapFightInfRequestPtr pgwreq = new CGwmQueryCitySatrapFightInfRequest(Request->getSerialStream());
	CGwmQueryCitySatrapFightInfResponsePtr pgwrsp = new CGwmQueryCitySatrapFightInfResponse();

	OBJID idAttacker = pgwreq->m_nPlayerId;
	uint8 nNation = pgwreq->m_nNation;
	uint8 idCity = pgwreq->m_nCityId;

	//玩家国籍是否和城池归属一致
	std::map<uint8, uint8>::iterator itx = this->m_mapCityOccup.find(idCity);
	if (itx == this->m_mapCityOccup.end()) {
		CWorldCityPtr pWarCtiy = g_pConsummer->getWorldCityPtr(idCity);
		if (NULL == pWarCtiy) {
			pgwrsp->m_nRetCode = INVALID_PARAM;
			return pgwrsp.get();
		}
		if (pWarCtiy->m_nNation != nNation) {
			pgwrsp->m_nRetCode = INVALID_PARAM;
			return pgwrsp.get();
		}
	}
	else if (itx->second != nNation) {
		pgwrsp->m_nRetCode = INVALID_PARAM;
		return pgwrsp.get();
	}
	
	uint8 nCity = pgwreq->m_nCityId;
	CSatrapCityPtr pSatrapCity = this->getSatrapCityInf(nCity);
	if (NULL == pSatrapCity) 
	{
		pgwrsp->m_nRetCode = SUCCESS;
		pgwrsp->m_cFightInf.m_nSatrapPlayer = 0;
		pgwrsp->m_cFightInf.m_nCityId = nCity;
		return pgwrsp.get();
	}

	//检查是否在保护时间内
	CDateTime now;
	if (now.getTotalSecs() < pSatrapCity->m_nFightCd) 
	{
		pgwrsp->m_nRetCode = SATRAP_CITY_IN_FIGHT_CD;
		return pgwrsp.get();
	}

	//挑战玩家是否已经弃城
	std::map<OBJID, uint8>::iterator it = this->m_mapPlayerSatrapCity.find(idAttacker);
	if (it != this->m_mapPlayerSatrapCity.end()) 
	{
		if (0 != it->second) 
		{
			pgwrsp->m_nRetCode = SATRAP_PLAYER_HAVE_CITY;
			return pgwrsp.get();
		}
	}

	pgwrsp->m_nRetCode = SUCCESS;
	CSatrapCity &cFightInf = pgwrsp->m_cFightInf;
	cFightInf.m_nCityId = nCity;
	if (0 != pSatrapCity->m_nSatrapPlayer) 
	{
		cFightInf.m_nLevel = pSatrapCity->m_nLevel;
		cFightInf.m_nSatrapPlayer = pSatrapCity->m_nSatrapPlayer;
		cFightInf.m_strNickName = pSatrapCity->m_strNickName;
		cFightInf.m_strProfessionLevel = pSatrapCity->m_strProfessionLevel;
		cFightInf.m_cDragon = pSatrapCity->m_cDragon;
		cFightInf.m_vecBuddy.assign(pSatrapCity->m_vecBuddy.begin(), pSatrapCity->m_vecBuddy.end());
		cFightInf.m_vecFormat.assign(pSatrapCity->m_vecFormat.begin(), pSatrapCity->m_vecFormat.end());
	}

	return pgwrsp.get();
}

//争夺太守结果通知
IResponsePtr CWorldWarMgr::executeCitySatrapFightResultNotice(IRequestPtr Request,CCurrentPtr Current) 
{
	CGwmCitySatrapFightResultRequestPtr pgwreq = new CGwmCitySatrapFightResultRequest(Request->getSerialStream());

	//太守易主处理
	uint8 nCity = pgwreq->m_nCityId;
	if(pgwreq->m_vecFightInf.size() == 0)
	{
		return NULL;
	}

	CSatrapCity& cityInf = pgwreq->m_vecFightInf[0];
	CSatrapCityPtr pSatrapCity = this->getSatrapCityInf(nCity);
	if (NULL == pSatrapCity) 
	{
		pSatrapCity = new CSatrapCity(cityInf);
		m_mapSatrapCity.insert(make_pair(nCity, pSatrapCity));

		//更新玩家对应的城池太守信息
		OBJID nCurPlayer = cityInf.m_nSatrapPlayer;
		this->m_mapPlayerSatrapCity[nCurPlayer] = cityInf.m_nCityId;

		//更新数据库
		this->updateDBCitySatrapInf(cityInf.m_nCityId);
		return NULL;
	}

	OBJID nOriPlayer = pSatrapCity->m_nSatrapPlayer;
	OBJID nCurPlayer = cityInf.m_nSatrapPlayer;

	//设置城池易主CD
	CDateTime now;
	cityInf.m_nFightCd = now.getTotalSecs() + this->m_nCityChangeCd;

	CSatrapCity& rCityInf = *(pSatrapCity.get());
	rCityInf = cityInf;

	//更新玩家对应的城池太守信息
	this->m_mapPlayerSatrapCity[nOriPlayer] = 0;
	this->m_mapPlayerSatrapCity[nCurPlayer] = pSatrapCity->m_nCityId;

	//太守被夺邮件
	CWorldCityPtr pWarCityInf = g_pConsummer->getWorldCityPtr(cityInf.m_nCityId);
	if (NULL != pWarCityInf)
	{
		std::vector<CRewardInfo> vecAward;
		std::string strTitle = g_pConsummer->formatLangString(LANGTYPE_WORLD_CITY, LANGTYPE_WORLD_CITY_BE_ROB_CITY_TILE);
		std::string sContent = g_pConsummer->formatLangString(LANGTYPE_WORLD_CITY, LANGTYPE_WORLD_CITY_BE_ROB_CITY_CONTENT, \
			pWarCityInf->m_strName.c_str(), pSatrapCity->m_strNickName.c_str());
		g_pConsummer->sendMailToPlayer(nOriPlayer, strTitle, sContent, vecAward, REWARD_SOURCE_CITY_SATRAP);
	}

	//通知玩家太守被争夺
	this->playerWarNotice(nOriPlayer, WORLD_SATRAP_CITY_ABORT, cityInf.m_nCityId, "");

	//更新数据库
	this->updateDBCitySatrapInf(pSatrapCity->m_nCityId);
	return NULL;
}

void  CWorldWarMgr::regKillRankDB()
{
	if (m_vecPrinceKillRank.size() == 0)
		return;
	CDBRegPrinceKillRankRequestPtr dbreq = new CDBRegPrinceKillRankRequest();
	dbreq->m_vecPrinceRank.assign(m_vecPrinceKillRank.begin(), m_vecPrinceKillRank.end());
	g_DaServer->callServant(DB_REG_PRINCE_KILL_RANK, dbreq.get(), NULL);
	m_vecPrinceKillRank.clear();
	m_mapPlayerKillNum.clear();
	return;
}
void CWorldWarMgr::rewardKillNum()
{
	if (m_vecPrinceKillRank.size() == 0)
		return;
	std::string strTitle = g_pConsummer->formatLangString(LANGTYPE_PRINCE_WAR, E_LANGTYPE_PRINCE_WAR_KILL_SELFT_TITLE);
	std::string sContent = g_pConsummer->formatLangString(LANGTYPE_PRINCE_WAR, E_LANGTYPE_PRINCE_WAR_KILL_SELFTCONTENT);
	for (size_t i = 0; i < m_vecPrinceKillRank.size(); i++)
	{
		std::vector<CRewardInfo> vecReward;
		CDBPrinceKillRank &cKillRank = m_vecPrinceKillRank[i];
		std::map<uint16, std::vector<CRewardInfo> >::iterator iter = m_mapKillNumReward.begin();
		for (; iter != m_mapKillNumReward.end(); ++iter)
		{
			uint16 nKillNum = iter->first;
			if (nKillNum > cKillRank.m_nKillNum)
				continue;
			if (this->isHaveGetReward(cKillRank.m_strKillReward, nKillNum))
				continue;
			char sBuf[127+1]={0};
			sprintf(sBuf, "%d", nKillNum);
			std::string strBuf = std::string(sBuf);
			cKillRank.m_strKillReward += ",";
			cKillRank.m_strKillReward += strBuf;
			std::vector<CRewardInfo> &vecKillReward = iter->second;
			for (size_t j = 0; j < vecKillReward.size(); j++)
				vecReward.push_back(vecKillReward[j]);
		}
		if (vecReward.size() == 0)
			continue;
		g_pConsummer->sendMailToPlayer(cKillRank.m_nPlayerID, strTitle, sContent, vecReward, REWARD_SOURCE_PRINCE_KILL_NUM);
	}
	return;
}
void  CWorldWarMgr::rewardKillRank()
{
	if (m_vecPrinceKillRank.size() == 0)
		return;

	m_nPrinceAccFlag = 0;
	std::string strTitle = g_pConsummer->formatLangString(LANGTYPE_PRINCE_WAR, E_LANGTYPE_PRINCE_WAR_KILL_RANK_TITLE);
	for (size_t i = 0; i < m_vecPrinceKillRank.size(); i++)
	{
		CDBPrinceKillRank &cKillRank = m_vecPrinceKillRank[i];
		uint16 nRank = i+1;
		std::vector<CRewardInfo> vecReward;
		this->getKillRankReward(2, nRank, vecReward);
		if (vecReward.size() == 0)
			continue;

		std::string sContent = g_pConsummer->formatLangString(LANGTYPE_PRINCE_WAR, E_LANGTYPE_PRINCE_WAR_KILL_RANK_CONTENT, nRank);
		g_pConsummer->sendMailToPlayer(cKillRank.m_nPlayerID, strTitle, sContent, vecReward, REWARD_SOURCE_PRINCE_KILL_RANK);
	}
	return;
}

uint8 CWorldWarMgr::getPrinceWarFlag()
{
     return m_nPrinceAccFlag;
}

//获取国战盟战信息
IResponsePtr CWorldWarMgr::executeGetAllyInf(IRequestPtr Request,CCurrentPtr Current)
{
	CGWGetAllyInfRequestPtr prequest = new CGWGetAllyInfRequest(Request->getSerialStream());
	CGWGetAllyInfResponsePtr presponse = new CGWGetAllyInfResponse();

	std::map<uint8, uint8>::iterator iter = m_mapAllyAcc.begin();
	for (; iter != m_mapAllyAcc.end(); ++iter)
	{
		uint8 idAcc = iter->first;
		uint8 idCity = iter->second;
		CAllyAccident cAllyAcc;

		cAllyAcc.m_nAccID = idAcc;
		cAllyAcc.m_nCityID = idCity;

		//获取事件信息
		CWarCityPtr pWarCityPtr = this->getWarCityByAccID(idAcc);
		if (pWarCityPtr == NULL)
		{
			cAllyAcc.m_nStatus = 2;
		}
		else
		{
			cAllyAcc.m_nStatus = pWarCityPtr->getStatus();
			cAllyAcc.m_nBlowNum = pWarCityPtr->getBlowTeamNum();
			cAllyAcc.m_nAttNum = pWarCityPtr->getAttTeamNum();
		}

		presponse->m_vecAccident.push_back(cAllyAcc);
	}

    presponse->m_nRetCode = SUCCESS; 
	return presponse.get();
}

/****************************************************** 文件结束 *********************************************/