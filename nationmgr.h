/*********************************************************************
*  Copyright 2014, by linglong.
*  All right reserved.
*
*  ���ܣ�����ϵͳ2.0
*
*  Edit History:
*
*    2015/08/20 - RaymondCheung ����
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
	//���ӹ��ҽ����
	IResponsePtr executeAddNationExp(IRequestPtr Request,CCurrentPtr Current);

	//gm���ù�����Ϣ
	IResponsePtr executeGmSetNationInf(IRequestPtr Request,CCurrentPtr Current);

	//ÿ������
	void dailyAccount();

public:
	void onTimeOut(CRefSharePtr Param){ return;};

	/*
	 * @���ܣ�������� 
	 */
	void playerOnline(OBJID idPlayer);
	/*
	 * @���ܣ��������
	 */
	void playerOffline(OBJID idPlayer);
	/*
	 * @���ܣ�������ֹͣ����
	 */
	void onServerStop();
	/*
	 * @���ܣ���ʱ�鿴 
	 */
	int onTimer();
    /**
	 * @���ܣ�����json���ݣ�ת���ɼ����������ݸ�ʽ
	 */
	virtual void relolutionTable(bool bUpdate);

	void incRef() { CRefShare::incRef(); }
	bool decRef() {	return CRefShare::decRef(); }

private:
	//����ʵ�����ӵĽ����
	uint32 getRealAddExp(uint8 nNation, uint32 nAddExp, bool isAddDailyExp);

	//�·���������
	void issuNationInfo(uint8 nNation, const std::vector<uint32>& vecKey);

	//֪ͨDB���¹�����Ϣ
	void updNationInfoDB(uint8 nNation, const std::vector<uint32>& vecKey);

	//������������
	void sendLvUpAnnouce(uint8 nNation, uint8 nLevel);

private:
	std::vector<uint32> m_vecGateWaySession;

};
typedef CRefObject<CNationMgr> CNationMgrPtr;
#endif

