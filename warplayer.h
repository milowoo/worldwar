/******************************************************
 *			Copyright 2015, by linglong.
 *				All right reserved.
 *���ܣ���ս���ʵ��
 *���ڣ�2015-6-1 15:05
 *���ߣ�milo_woo
 ******************************************************/
#ifndef _GWM_WAR_PLAYER_H_
#define _GWM_WAR_PLAYER_H_

#include <framework/daserver.h>
#include <framework/endpoint.h>
#include <util/refshare.h>
#include <common/util/singleton.h>
#include <gatewaymgr/gwmworldwar.h>
#include <gateway/worldwar.h>

class CWarJoinPlayer : public CRefShare
{
public:
	CWarJoinPlayer();

	virtual ~CWarJoinPlayer();

	friend class CRefObject<CWarJoinPlayer>;

public:
	void playerOnline(OBJID idPlayer);
	void playerOffline(OBJID idPlayer);
	void onServerStop();

	/**
	 * @���ܣ���ʱ
	 */
	void onTime();

	/**
	* @���ܣ��ж��佫�Ƿ�����
	*/
	bool isHaveDeployedBuddy(std::vector<CFormatUint> &vecUint);

	/**
	* @���ܣ����ӳǳ������佫����
	*/
	void addDeployedBuddy(uint8 idCity, std::vector<CFormatUint> &vecUint);

	/**
	* @���ܣ���ȡ���������佫��Ϣ(����ĳ���ǳ���)
	*/
	void getNotInDeployedBuddy(uint8 idCity, std::vector<uint32> &vecBuddy);

	/**
	* @���ܣ���ȡ�ǳ����������佫��Ϣ
	*/
	void getCityDeployedBuddy(uint8 idCity, std::vector<uint32> &vecBuddy);



public:
	void incRef()
	{
		CRefShare::incRef();
	}

	bool decRef()
	{
		return CRefShare::decRef();
	}

public:
	OBJID m_nPlayerID;     //���ID
	std::string m_strName; //����
	uint8 m_nSex;   //�Ա�
	uint8 m_nLevel; //�ȼ�
	uint32 m_nBattEffect; //ս����
	uint8 m_nNation;  //��Ӫ
	uint8  m_nGoverPosts; //���ҹ�ְ
	uint8  m_nCityID;    //��ǰ�ǳ�
	std::vector<CPlayerCityWarInf> m_vecCityWarInf; //��ҳ�ս��Ϣ
	std::map<uint8, std::map<uint32, uint8> > m_mapBuddy; //��ҳǳ�פ����Ϣ
};
typedef CRefObject<CWarJoinPlayer> CWarJoinPlayerPtr;

#endif
