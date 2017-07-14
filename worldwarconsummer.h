#ifndef _WORLD_WAR_CONSUMMER_H_
#define _WORLD_WAR_CONSUMMER_H_
#include <public.h>
#include <business/consummer/iworldwarconsummer.h>

class CWorldWarConsummer : public IWorldWarConsummer, public CRefShare
{
protected:
	friend class CRefObject<CWorldWarConsummer> ;

public:
	CWorldWarConsummer();
	virtual ~CWorldWarConsummer();

	//玩家上线
	virtual uint32 playerOnline(OBJID idPlayer, IResponsePtr pdbresponse);

	//玩家下线
	virtual uint32 playerOffline(OBJID idPlayer);

	//连接断开
	virtual void onSessionClose(CSessionPtr pSession) { return; }

	//配置文件更新
	virtual void onProfileUpdate(CDAServer& Server) { return; }

	//服务器关闭
	virtual void onServerStop();

	//定时器触发
	virtual int onTimer(uint32 nInterval);


	virtual void onStaticDataUpdate(bool bUpdate);

	/**
	* @功能：加载dbmgr数据
	*/
	virtual void loadFromDB();

	//日切清算
	virtual void dailyAcc();

	//获取群雄逐鹿活动的标志
	virtual uint8 getPrinceWarFlag();

	void incRef() { CRefShare::incRef(); }
	bool decRef() { return CRefShare::decRef(); }

private:

};
typedef CRefObject<CWorldWarConsummer> CWorldWarConsummerPtr;

#endif