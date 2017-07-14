/******************************************************
 *			Copyright 2015, by linglong.
 *				All right reserved.
 *���ܣ���ս����ʵ��
 *���ڣ�2015-7-16 15:05
 *���ߣ�milo_woo
 ******************************************************/
#ifndef _GWM_WAR_NATION_H_
#define _GWM_WAR_NATION_H_

#include <framework/daserver.h>
#include <framework/endpoint.h>
#include <util/refshare.h>
#include <common/util/singleton.h>
#include <gatewaymgr/gwmworldwar.h>
#include <gateway/worldwar.h>

class CWarNation : public CRefShare
{
public:
	CWarNation();

	virtual ~CWarNation();

	friend class CRefObject<CWarNation>;

public:
	void playerOnline(OBJID idPlayer);
	void playerOffline(OBJID idPlayer);
	void onServerStop();

	/**
	 * @���ܣ���ʱ
	 */
	void onTime();
   
	//���ù���
	void setNation(uint8 idNation){m_nNationID = idNation;}
	uint8 getNation(){return m_nNationID;}

	/**
	* @���ܣ���ȡ�����Ͻ����
	*/
	void getAllDomainReward(std::vector<CDomainReward> &vecDomainReward);

	/**
	* @���ܣ���ȡĳСʱ�����Ͻ����
	*/
	void getHourDomainReward(uint16 nHour, std::vector<CRewardInfo> &vecReward);

	void clearDomainReward();

	//��ȡ��ս�����Ϣ
	CPlayerCityWarInfPtr getJoinPlayer(OBJID idPlayer);

	/**
	* @���ܣ�������ҷ��ض���
	*/
	void addPlayerCityInf(OBJID idPlayer, CPlayerCityWarInfPtr pPlayerCityIntPtr);

	/**
	* @���ܣ���ȡ����ID
	*/
	uint32 getTeamID();
	void setTeamID(uint32 idTeam){m_nTeamID = idTeam;};

	/**
	* @���ܣ�������פ������
	*/
	void addNewEtapeTeam(CEtapeTeam &cEtapeTeam);
    
	//���Ӷ���ID
	void addNewTeamID(uint32 idTeam);

	/**
	* @���ܣ�ɾ��פ������
	*/
	void delEtapeTeam(uint32 idTeam);

	/**
	* @���ܣ���ȡפ�������б�
	*/
	void getEtapeTeamList(std::vector<CEtapeTeam> &vecEtapeTeam);

	/**
	* @���ܣ���ȡפ����������
	*/
	uint16 getEtapeTeamNum();

	/**
	* @���ܣ���ȡ���������б�
	*/
	void getEtapeNpcTeamList(std::vector<CEtapeTeam> &vecEtapeTeam);
   
	/**
	* @���ܣ���ȡ������������
	*/
	uint16 getNpcTeamNum();


	/**
	* @���ܣ���ն�����Ϣ
	*/
	void clearEtapeTeam();

	/**
	* @���ܣ���ȡsession
	*/
	void incSessionID(){m_nSessionID++;};
	uint32 getSessionID(){return m_nSessionID;}
    
	/**
	* @���ܣ���ս����
	*/
	uint8 getDecNum(){return m_nDecNum;}
	void setDenNum(uint8 nNum){m_nDecNum = nNum;}
    
	/**
	* @���ܣ�У�齱��
	*/
	bool checkDomainReward(uint16 nHour);
	void erasePlayerTeam(CPlayerCityWarInfPtr pPlayerCityWarPtr, uint32 idTeam);
	OBJID eraseEtapeTeam(uint32 idTeam);




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
	uint8 m_nNationID;   //����ID
	uint32 m_nTeamID;    //����ID
	uint32 m_nSessionID; //��վsession
	uint8 m_nDecNum;     //��ս����
	std::string m_strName; //��������
	std::vector<CDomainReward> m_vecDomainReward;
	std::vector<CEtapeTeam>  m_vecEtapeTeam;  //��վ��Ϣ
	std::vector<CEtapeTeam> m_vecEtapeNpcTeam; //��վ��������
	std::map<OBJID, CPlayerCityWarInfPtr > m_mapPlayerInf; //���פ����Ϣ

};
typedef CRefObject<CWarNation> CWarNationPtr;

#endif
