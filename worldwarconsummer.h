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

	//�������
	virtual uint32 playerOnline(OBJID idPlayer, IResponsePtr pdbresponse);

	//�������
	virtual uint32 playerOffline(OBJID idPlayer);

	//���ӶϿ�
	virtual void onSessionClose(CSessionPtr pSession) { return; }

	//�����ļ�����
	virtual void onProfileUpdate(CDAServer& Server) { return; }

	//�������ر�
	virtual void onServerStop();

	//��ʱ������
	virtual int onTimer(uint32 nInterval);


	virtual void onStaticDataUpdate(bool bUpdate);

	/**
	* @���ܣ�����dbmgr����
	*/
	virtual void loadFromDB();

	//��������
	virtual void dailyAcc();

	//��ȡȺ����¹��ı�־
	virtual uint8 getPrinceWarFlag();

	void incRef() { CRefShare::incRef(); }
	bool decRef() { return CRefShare::decRef(); }

private:

};
typedef CRefObject<CWorldWarConsummer> CWorldWarConsummerPtr;

#endif