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

#ifndef _NATION_MGR_
#define _NATION_MGR_

#include <framework/daserver.h>
#include <framework/endpoint.h>
#include <util/refshare.h>
#include <common/util/singleton.h>
#include <common/util/tinytimer.h>

#include "business/istaticdata.h"
#include <util/singleton.h>


class CNationMgr : public CSingleton<CNationMgr>, public IStaticData,  public CRefShare
{
public:
	CNationMgr();
	virtual ~CNationMgr();
	
	friend class CRefObject<CNationMgr>;

public:
	//增加国家建设度
	IResponsePtr executeAddNationExp(IRequestPtr Request,CCurrentPtr Current);

	//gm设置国家信息
	IResponsePtr executeGmSetNationInf(IRequestPtr Request,CCurrentPtr Current);

	//每日清算
	void dailyAccount();

public:
	void onTimeOut(CRefSharePtr Param){ return;};

	/*
	 * @功能：玩家上线 
	 */
	void playerOnline(OBJID idPlayer);
	/*
	 * @功能：玩家下线
	 */
	void playerOffline(OBJID idPlayer);
	/*
	 * @功能：服务器停止运行
	 */
	void onServerStop();
	/*
	 * @功能：定时查看 
	 */
	int onTimer();
    /**
	 * @功能：处理json数据，转换成计算所需数据格式
	 */
	virtual void relolutionTable(bool bUpdate);

	void incRef() { CRefShare::incRef(); }
	bool decRef() {	return CRefShare::decRef(); }

private:
	//计算实际增加的建设度
	uint32 getRealAddExp(uint8 nNation, uint32 nAddExp, bool isAddDailyExp);

	//下发国家数据
	void issuNationInfo(uint8 nNation, const std::vector<uint32>& vecKey);

	//通知DB更新国家信息
	void updNationInfoDB(uint8 nNation, const std::vector<uint32>& vecKey);

	//发送升级公告
	void sendLvUpAnnouce(uint8 nNation, uint8 nLevel);

private:
	std::vector<uint32> m_vecGateWaySession;

};
typedef CRefObject<CNationMgr> CNationMgrPtr;
#endif

