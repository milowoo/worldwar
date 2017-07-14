/*********************************************************************
 *  Copyright 2014, by linglong.
 *  All right reserved.
 *
 *  功能：国战模块玩家公共管理
 *
 *  Edit History:
 *
 *    2014/07/22 - milowoo 创建
 *************************************************************************/

#include <net/netdefines.h>
#include "worldwarconsummer.h"
#include "module.h"
#include "worldwarmgr.h"
#include "nationmgr.h"

CWorldWarConsummer::CWorldWarConsummer()
{

}

CWorldWarConsummer::~CWorldWarConsummer()
{

}

void CWorldWarConsummer::onStaticDataUpdate(bool bUpdate)
{
     CWorldWarMgr::getInstance()->relolutionTable(bUpdate);
	 CNationMgr::getInstance()->relolutionTable(bUpdate);
}

int CWorldWarConsummer::onTimer(uint32 nInterval)
{
	CWorldWarMgr::getInstance()->onTimer(nInterval);
	//CNationMgr::getInstance()->onTimer();
	return 0; 
}

uint32 CWorldWarConsummer::playerOnline(OBJID idPlayer, IResponsePtr iResponsePtr)
{
	CWorldWarMgr::getInstance()->playerOnline(idPlayer);
	CNationMgr::getInstance()->playerOnline(idPlayer);
	return SUCCESS;
}

uint32 CWorldWarConsummer::playerOffline(OBJID idPlayer)
{
	CWorldWarMgr::getInstance()->playerOffline(idPlayer);
	CNationMgr::getInstance()->playerOffline(idPlayer);
	return SUCCESS;
}

void CWorldWarConsummer::onServerStop()
{
	CWorldWarMgr::getInstance()->onServerStop();
	CNationMgr::getInstance()->onServerStop();
}

/**
* @功能：加载dbmgr数据
*/
void CWorldWarConsummer::loadFromDB()
{
	CWorldWarMgr::getInstance()->onDbmgrOpen();
}

void CWorldWarConsummer::dailyAcc()
{
	CWorldWarMgr::getInstance()->dailyAcc();
	CNationMgr::getInstance()->dailyAccount();
}

//获取群雄逐鹿活动的标志
uint8 CWorldWarConsummer::getPrinceWarFlag()
{

	return CWorldWarMgr::getInstance()->getPrinceWarFlag();
}

/**************************************************** 文件结束 *************************************************/

