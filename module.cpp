/*********************************************************************
 *  Copyright 2014, by linglong.
 *  All right reserved.
 *
 *  功能：国战模块协议管理
 *
 *  Edit History:
 *
 *    2014/07/22 - milowoo 创建
 *************************************************************************/
 
#include <net/netdefines.h>
#include "module.h"
#include "worldwarmgr.h"
#include "nationmgr.h"

CDAServerPtr g_DaServer = NULL;
IGatewayMgrConsummerPtr g_pConsummer = NULL;
CWorldWarConsummerPtr g_pProvider = NULL;

//注册servant
void initDllServant(CDAServerPtr pServer, CLogPtr pLog, CSystemLogPtr pSystemLog, IConsummerPtr pConsumer, std::map<uint32, IServantPtr>& mapServant)
{
	ASSERT(pServer!= NULL);
	ASSERT(pSystemLog != NULL);
	ASSERT(pConsumer != NULL);
	ASSERT(pLog != NULL);

	CCException::initStackTrace();

	g_DaServer = pServer;
	g_pConsummer = static_cast<IGatewayMgrConsummer*>(pConsumer.get());
	g_Log = pLog;
	g_pSystemLog = pSystemLog;
	g_nWorldID = (g_DaServer->getSettingUint32("machine settings", "platform", "0")<<16) | g_DaServer->getSettingUint32("machine settings", "serverid", "0");

	g_pProvider->onStaticDataUpdate(false);
   

	mapServant.clear();

	mapServant[GWM_WORLD_WAR_MOVE] = new TServant<CWorldWarMgr>(CWorldWarMgr::getInstance(), &CWorldWarMgr::executeGWMWorldWarMove);
	mapServant[GWM_WORLD_WAR_NOTICE] = new TServant<CWorldWarMgr>(CWorldWarMgr::getInstance(), &CWorldWarMgr::executeGWMWorldWarNotice);

	mapServant[GWM_ENTER_WORLD_WAR] = new TServant<CWorldWarMgr>(CWorldWarMgr::getInstance(), &CWorldWarMgr::executeGWMEnterWorldWar);
	mapServant[GWM_ENTER_FIGHT_CITY] = new TServant<CWorldWarMgr>(CWorldWarMgr::getInstance(), &CWorldWarMgr::executeGWMEnterFightCity);
	mapServant[GWM_SELECT_FIGHT_DIRECT] = new TServant<CWorldWarMgr>(CWorldWarMgr::getInstance(), &CWorldWarMgr::executeSelectFightDirect);
	mapServant[GWM_GET_CITY_TEAM_NUM] = new TServant<CWorldWarMgr>(CWorldWarMgr::getInstance(), &CWorldWarMgr::executeGWMGetCityTeamNum);
	mapServant[GWM_CITY_DEPART] = new TServant<CWorldWarMgr>(CWorldWarMgr::getInstance(), &CWorldWarMgr::executeGWMCityDepart);
	mapServant[GWM_GET_CITY_TEAM_LIST] = new TServant<CWorldWarMgr>(CWorldWarMgr::getInstance(), &CWorldWarMgr::executeGetCityTeamList);
	mapServant[GWM_GET_FIGHT_TEAM_INF] = new TServant<CWorldWarMgr>(CWorldWarMgr::getInstance(), &CWorldWarMgr::executeGetCityFightTeamInf);
	mapServant[GWM_QUERY_CITY_BATTLE] = new TServant<CWorldWarMgr>(CWorldWarMgr::getInstance(), &CWorldWarMgr::executeQueryCityBattleReport);
	mapServant[GWM_CITY_PICK] = new TServant<CWorldWarMgr>(CWorldWarMgr::getInstance(), &CWorldWarMgr::executeGWMCityPick);
	mapServant[GWM_QUERY_CITY_FIGHT_RANK] = new TServant<CWorldWarMgr>(CWorldWarMgr::getInstance(), &CWorldWarMgr::executeQueryCityFightRank);
	mapServant[GWM_CITY_FIGHT_DEPLOYED] = new TServant<CWorldWarMgr>(CWorldWarMgr::getInstance(), &CWorldWarMgr::executeCityFightDeployed);
	mapServant[GWM_GET_FIGHT_QUEUE] = new TServant<CWorldWarMgr>(CWorldWarMgr::getInstance(), &CWorldWarMgr::executeGetFightQueue);
	mapServant[GWM_CITY_PICK_NOTICE] = new TServant<CWorldWarMgr>(CWorldWarMgr::getInstance(), &CWorldWarMgr::executeGWMCityPickNotice);
	mapServant[GWM_GET_CITY_FIGHT_TEAM] = new TServant<CWorldWarMgr>(CWorldWarMgr::getInstance(), &CWorldWarMgr::executeGetCityFightTeam);

	mapServant[GWM_KING_DECLARE] = new TServant<CWorldWarMgr>(CWorldWarMgr::getInstance(), &CWorldWarMgr::executeGWMDeclareWar);
	mapServant[GWM_ETAPE_DEPLOYED] = new TServant<CWorldWarMgr>(CWorldWarMgr::getInstance(), &CWorldWarMgr::executeEtapeDeployed);
	mapServant[GWM_GET_ETAPE_LIST] = new TServant<CWorldWarMgr>(CWorldWarMgr::getInstance(), &CWorldWarMgr::executeGetEtapeTeamList);
	mapServant[GWM_DEPLOY_FORCE] = new TServant<CWorldWarMgr>(CWorldWarMgr::getInstance(), &CWorldWarMgr::executeDeployForce);
	mapServant[GWM_GET_DEPLOY_TEAM_LIST] = new TServant<CWorldWarMgr>(CWorldWarMgr::getInstance(), &CWorldWarMgr::executeGetDeployTeamList);
	mapServant[GWM_BEAT_RETREAT] = new TServant<CWorldWarMgr>(CWorldWarMgr::getInstance(), &CWorldWarMgr::executeBeatRetreat);
	mapServant[GWM_QUERY_DOMAIN_REWARD_INF] = new TServant<CWorldWarMgr>(CWorldWarMgr::getInstance(), &CWorldWarMgr::executeQueryDomainRewardInf);
	mapServant[GWM_EXIT_FIGHT_CITY] = new TServant<CWorldWarMgr>(CWorldWarMgr::getInstance(), &CWorldWarMgr::executeExitFightCity);
	mapServant[GWM_SET_VISION] = new TServant<CWorldWarMgr>(CWorldWarMgr::getInstance(), &CWorldWarMgr::executeGWMSetVision);
	mapServant[GWM_WORLD_LOGOUT] = new TServant<CWorldWarMgr>(CWorldWarMgr::getInstance(), &CWorldWarMgr::executeExitWorldWar);
	mapServant[GWM_GET_WORLD_WAR_BATTLE] = new TServant<CWorldWarMgr>(CWorldWarMgr::getInstance(), &CWorldWarMgr::executeQueryWorldWarBattle);

	mapServant[GWM_GM_UPD_CITY_OCCUP] = new TServant<CWorldWarMgr>(CWorldWarMgr::getInstance(), &CWorldWarMgr::executeGMUpdCityOccup);
	mapServant[GWM_WORLD_WAR_ONLINE] = new TServant<CWorldWarMgr>(CWorldWarMgr::getInstance(), &CWorldWarMgr::executeGWMWorldWarOnline);

	mapServant[GWM_RCD_EXPLOIT_LIB_NOTICE] = new TServant<CWorldWarMgr>(CWorldWarMgr::getInstance(), &CWorldWarMgr::executeRcdExploitLibNotice);
	mapServant[GWM_WORLD_PRAISE] = new TServant<CWorldWarMgr>(CWorldWarMgr::getInstance(), &CWorldWarMgr::executeWorldPraise);
	mapServant[GWM_WORLD_LOGIN] = new TServant<CWorldWarMgr>(CWorldWarMgr::getInstance(), &CWorldWarMgr::executeWorldLogin);

	//城池系统
	mapServant[GWM_CITY_SATRAP_ABORT] = new TServant<CWorldWarMgr>(CWorldWarMgr::getInstance(), &CWorldWarMgr::executeCitySatrapAbort);
	mapServant[GWM_CITY_SATRAP_ADJUST_FORM] = new TServant<CWorldWarMgr>(CWorldWarMgr::getInstance(), &CWorldWarMgr::executeCitySatrapAdjustFormation);
	mapServant[GWM_CITY_SATRAP_FIGHT_RST] = new TServant<CWorldWarMgr>(CWorldWarMgr::getInstance(), &CWorldWarMgr::executeCitySatrapFightResultNotice);
	mapServant[GWM_CITY_SATRAP_GET_FIGHT_DAT] = new TServant<CWorldWarMgr>(CWorldWarMgr::getInstance(), &CWorldWarMgr::executeGetCitySatrapFightData);
	mapServant[GWM_CITY_SATRAP_GET_BUDDY_INF] = new TServant<CWorldWarMgr>(CWorldWarMgr::getInstance(), &CWorldWarMgr::executeQuerySatrapBuddy);
	mapServant[GWM_CITY_SATRAP_GET_INF] = new TServant<CWorldWarMgr>(CWorldWarMgr::getInstance(), &CWorldWarMgr::executeQuerySatrapInf);

	//国家系统
	mapServant[GWM_ADD_NATION_EXP] = new TServant<CNationMgr>(CNationMgr::getInstance(), &CNationMgr::executeAddNationExp);
	mapServant[GWM_GM_SET_NATION_INF] = new TServant<CNationMgr>(CNationMgr::getInstance(), &CNationMgr::executeGmSetNationInf);

	//群雄逐鹿
    mapServant[GWM_GET_KILL_RANK] = new TServant<CWorldWarMgr>(CWorldWarMgr::getInstance(), &CWorldWarMgr::executeGetKillRank);
	mapServant[GWM_GET_KILL_NUM_AWARD] = new TServant<CWorldWarMgr>(CWorldWarMgr::getInstance(), &CWorldWarMgr::executeGetKillNumAward);
	mapServant[GWM_GET_PRINCE_ACC] = new TServant<CWorldWarMgr>(CWorldWarMgr::getInstance(), &CWorldWarMgr::executeGetPrinceAcc);
}

void getConsummer(IConsummerPtr& pConsumer)
{
	g_pProvider = new CWorldWarConsummer();
	pConsumer = g_pProvider.get();
}

#ifdef WIN32
BOOL APIENTRY DllMain( HMODULE hModule,
					  DWORD  ul_reason_for_call,
					  LPVOID lpReserved
					  )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
#endif // WIN32


/******************************************** 文件结束 ****************************************************/

