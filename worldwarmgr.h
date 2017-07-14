#ifndef _WORLD_WAR_MGR_H_
#define _WORLD_WAR_MGR_H_

#include <framework/daserver.h>
#include <framework/endpoint.h>
#include <util/refshare.h>
#include <common/io/bytesbuffer.h>

#include <framework/responsehandler.h>
#include <gateway/emaildata.h>
#include <business/staticdatadef.h>
#include "../gatewaymgr/gatewaymgrglobal.h"
#include <gatewaymgr/gwmworldwar.h>
#include <dbmgr/dbgetworldwar.h>
#include <gateway/worldwar.h>
#include "warcity.h"
#include "warplayer.h"
#include "warnation.h"

#define  EXPLOID_LIB_RECORD_SAVE_CD	5 * 60  //��ѫ�����ȡ��¼�������ݿ�CDʱ�� 5����


class CWorldWarMgr : public CSingleton<CWorldWarMgr>, public IStaticData,  public CRefShare
{
public:
	CWorldWarMgr();
	virtual ~CWorldWarMgr();
	friend class CRefObject<CWorldWarMgr>;

public:
	//��ս֪ͨ
	IResponsePtr executeGWMWorldWarNotice(IRequestPtr Request, CCurrentPtr Current);
	//��ս�ƶ�
	IResponsePtr executeGWMWorldWarMove(IRequestPtr Request, CCurrentPtr Current);
	//�����ս
	IResponsePtr executeGWMEnterWorldWar(IRequestPtr Request, CCurrentPtr Current);
	IResponsePtr executeGWMDeclareWar(IRequestPtr Request, CCurrentPtr Current);

	//�����ս�ǳ�
	IResponsePtr executeGWMEnterFightCity(IRequestPtr Request, CCurrentPtr Current);

	//��ȡ��ս��������
	IResponsePtr executeGWMGetCityTeamNum(IRequestPtr Request, CCurrentPtr Current);

	//ѡ����ս��Ӫ
	IResponsePtr executeSelectFightDirect(IRequestPtr Request, CCurrentPtr Current);

	IResponsePtr executeEtapeDeployed(IRequestPtr Request,CCurrentPtr Current);
	IResponsePtr executeGetEtapeTeamList(IRequestPtr Request,CCurrentPtr Current);
	//��սפ��
	IResponsePtr executeCityFightDeployed(IRequestPtr Request,CCurrentPtr Current);

	//��ȡ�ǳ�ս������
	IResponsePtr executeGetCityFightTeam(IRequestPtr Request,CCurrentPtr Current);

	//��ȡս��������Ϣ
	IResponsePtr executeGetFightQueue(IRequestPtr Request,CCurrentPtr Current);

	IResponsePtr executeDeployForce(IRequestPtr Request,CCurrentPtr Current);
	//����
	IResponsePtr executeGWMCityDepart(IRequestPtr Request, CCurrentPtr Current);
	IResponsePtr executeGWMSetVision(IRequestPtr Request, CCurrentPtr Current);
	IResponsePtr executeGetDeployTeamList(IRequestPtr Request,CCurrentPtr Current);
	IResponsePtr executeBeatRetreat(IRequestPtr Request, CCurrentPtr Current);

	//����
	IResponsePtr executeGWMCityPick(IRequestPtr Request, CCurrentPtr Current);

	//�������֪ͨ
	IResponsePtr executeGWMCityPickNotice(IRequestPtr Request, CCurrentPtr Current);

	//@��ȡ��ս�����б�
	IResponsePtr executeGetCityTeamList(IRequestPtr Request,CCurrentPtr Current);
    
	//��ѯ��������
	IResponsePtr  executeGetCityFightTeamInf(IRequestPtr Request,CCurrentPtr Current);

	//��ȡ��սɱ�а�
	IResponsePtr executeQueryCityFightRank(IRequestPtr Request,CCurrentPtr Current);
	IResponsePtr executeQueryDomainRewardInf(IRequestPtr Request, CCurrentPtr Current);
	IResponsePtr executeExitFightCity(IRequestPtr Request, CCurrentPtr Current);
	IResponsePtr executeExitWorldWar(IRequestPtr Request, CCurrentPtr Current);

	//�����¼
	IResponsePtr executeWorldLogin(IRequestPtr Request, CCurrentPtr Current);

	//��ս����
	IResponsePtr executeWorldPraise(IRequestPtr Request, CCurrentPtr Current);

	//��¼��ѫ����ͨ��
	IResponsePtr executeRcdExploitLibNotice(IRequestPtr Request, CCurrentPtr Current);

	//��ѯս��
	IResponsePtr executeQueryCityBattleReport(IRequestPtr Request,CCurrentPtr Current);

	//��ѯ��սս��
	IResponsePtr executeQueryWorldWarBattle(IRequestPtr Request,CCurrentPtr Current);

	IResponsePtr executeGMUpdCityOccup(IRequestPtr Request, CCurrentPtr Current);

	IResponsePtr executeGWMWorldWarOnline(IRequestPtr Request, CCurrentPtr Current);

	/************** פ�سǳ����Э��  ********************/
	//��ѯ�ǳ�פ������
	IResponsePtr executeQuerySatrapBuddy(IRequestPtr Request,CCurrentPtr Current);

	//��ѯ�ǳ�̫����Ϣ
	IResponsePtr executeQuerySatrapInf(IRequestPtr Request,CCurrentPtr Current);

	//̫�ص�������
	IResponsePtr executeCitySatrapAdjustFormation(IRequestPtr Request,CCurrentPtr Current);

	//̫������
	IResponsePtr executeCitySatrapAbort(IRequestPtr Request,CCurrentPtr Current);

	//��ȡ̫��ս������
	IResponsePtr executeGetCitySatrapFightData(IRequestPtr Request,CCurrentPtr Current);

	//����̫�ؽ��֪ͨ
	IResponsePtr executeCitySatrapFightResultNotice(IRequestPtr Request,CCurrentPtr Current);

	//��̫�ط���ٺ»
	void sendAwardMailToCitySatrap();

	void onResponseGetCitySatrapInfFromDB(IResponsePtr pResponse, CCurrentPtr Current, CRefSharePtr Param);

	/************** פ�سǳ����Э��  ********************/


    /************** Ⱥ����¹ begin  ********************/

	//��ȡɱ�����а�
	IResponsePtr executeGetKillRank(IRequestPtr Request,CCurrentPtr Current);

	//��ȡɱ�н���
	IResponsePtr executeGetKillNumAward(IRequestPtr Request,CCurrentPtr Current);

	//��ȡȺ����¹�¼�
	IResponsePtr executeGetPrinceAcc(IRequestPtr Request,CCurrentPtr Current);


	//Ⱥ����¹�¼�����
	void princeAccProc(uint8 nType);
	//��ȡ��Ӫ�ǳ���Ϣ(�߾��ǳ�)
	void getNationPrinceAccCity(uint8 idNation, uint8 nCityNum, std::vector<uint8> &vecEliteCity, std::vector<uint8> &vecNorCity);
	//��ȡ��Ӫ�ǳ���Ϣ(�߾��ǳ�)
	void getSpeNationPrinceAccCity(uint8 idNation, std::vector<uint8> &vecEliteCity, std::vector<uint8> &vecNorCity);

	/************** Ⱥ����¹ end  **********************/


	/************** ��ս��ս begin  ********************/
	//��ȡ��ս��ս��Ϣ
	IResponsePtr executeGetAllyInf(IRequestPtr Request,CCurrentPtr Current);

	/************** ��ս��ս end  **********************/





	/**
	* @���ܣ�����json���ݣ�ת���ɼ����������ݸ�ʽ
	*/
	virtual void relolutionTable(bool bUpdate);
    
	//�Զ�ս��Ӧ����
	void onResponseAutoCityTeamFight(IResponsePtr pResponse, CCurrentPtr Current, CRefSharePtr Param);

	//�Զ�����Ӧ����
	void onResponseAutoPickTeamFight(IResponsePtr pResponse, CCurrentPtr Current, CRefSharePtr Param);
    
	//��һ�ʤ����
	void playerSuccFight(CWarCityPtr pWarCityPtr, OBJID idPlayer, uint32 idTeam);

	//��һ�ʤ����(ͬʱ����)
	void playerDieSuccFight(CWarCityPtr pWarCityPtr, OBJID idPlayer, uint32 idTeam);
   
	//���ս����ɱ��ҽ���
	void rewardPlayerKillPlayer(CWarCityPtr pWarCityPtr, OBJID idPlayer);

	//���ս��ʤ�����ｱ��
	void rewardPlayerKillMonster(CWarCityPtr pWarCityPtr, OBJID idPlayer, uint32 idNpc);

	//������ҵ��¼�������Ϣ
	void clearWarPlayerTeam(uint8 idCity);
    
	//NPC������������
	void npcAutoPick(CWarCityPtr pWarCityPtr);
	//����NPC������������
	void attNpcAutoPick(CWarCityPtr pWarCityPtr);
	//�Զ���������
	void autoPickFight(CWarCityPtr pWarCityPtr, CWarCityTeam &cAttWarTeam, CWarCityTeam &cBlowWarTeam, uint32 nGWSession);

	void dailyAcc();
	void wheelFightReq(CWarCityPtr pWarCityPtr, uint8 nQueueNum, uint16 &iGatewaySessiin, std::vector<uint32> &vecGateWaySession);
	bool isCanFightQueue(CWarCityPtr pWarCityPtr, uint8 nQueueNum);
	uint8 checkWarStatus(CWarCityPtr pWarCityPtr);
	void erasePlayerTeam(CPlayerCityWarInfPtr pPlayerCityWarPtr, uint32 idTeam);
	void getNationAdminCity(std::vector<CCityOccup> &vecCityOccup, std::map<uint8, std::vector<uint8> > &mapNationCity);
    
	//��ս��Ϣ֪ͨ
	void decWarNotice(CWarCityPtr pWarCityPtr);

	uint8 getPrinceWarFlag();
     
	//��վ����
	void etapeClear(uint8 idNation);

private:
    
	void onResponseNULL(IResponsePtr pResponse, CCurrentPtr Current, CRefSharePtr Param) {return;}
	void onTimeOut(CRefSharePtr Param){ return;}

	uint32 getSessionID(){return m_nSessionID;}
	void  sesssionInc()
	{
		m_nSessionID++; 
		if (m_nSessionID >= 99999999)
			m_nSessionID = 1;
	}


	//��ҹ�ս��Ϣ֪ͨ
	void warPlayerNotice(std::vector<OBJID> &vecPlayer, uint8 nKey, uint32 nValues);

	//��ҹ�ս��Ϣ֪ͨ
	void playerWarNotice(OBJID idPlayer, uint8 nKey, uint32 nValues, std::string strData);
    
	void onResponseGetWorldWar(IResponsePtr pResponse, CCurrentPtr Current, CRefSharePtr Param);
	void onResponseGetPlayerTeam(IResponsePtr pResponse, CCurrentPtr Current, CRefSharePtr Param);
	void onResponseGetExploitLog(IResponsePtr pResponse, CCurrentPtr Current, CRefSharePtr Param);

    //��ȡ��ս��Ա��Ϣ
    CWarJoinPlayerPtr getWarJoinPlayer(OBJID idPlayer)
	{
		std::map<OBJID, CWarJoinPlayerPtr>::iterator iter = m_mapWarJoinPlayer.find(idPlayer);
		if (iter == m_mapWarJoinPlayer.end())
			return NULL;

		return iter->second;
	}
    
	//ת��BUDDY��Ϣ
	void convDBBuddy(CDBBatBuddy &cDbBuddy, CBattleBuddy &cBatBuddy);

	//ת��������Ϣ
	void convStrDragon(std::string strDragon, std::vector<CLSkillData> &Skill);
   
    
	//��ȡ�ǳ��¼���Ϣ
	CWarCityPtr getWarCityPtr(uint8 idCity);
	CWarCityPtr getWarCityByAccID(uint8 idAcc);


	//��ʼ���ǳ�NPC����
	void initNpcTeam(CWarCityPtr pWarCityPtr, CWarGroupPtr pGroupPtr, uint8 type);
    
	//��ʼ���ǳ�NPC����
	void initAccCityNpcTeam(CWarCityPtr pWarCityPtr);

	void initCityDefTeam(CWarCityPtr pWarCityPtr);
	//��ȡ��ս���ž�
	CWarGroupPtr getWarGroupPtr(uint32 idGroup , uint8 worldlevel);

    //�ǳس���ս
	void cityWheelWar(CWarCityPtr pWarCityPtr);
    
	//��ȡ���ս����Ϣ
	void getPlayerBatInf(CWorldBatTeam &cWorldBatTeam, uint8 idCity, uint8 nIsBlow = 0);
    
	//���뼯�ž�������
	void reloadWorldGroup(bool bUpdate);
   
	//������������������
	void reloadKillRankReward(bool bUpdate);
    
	//������߽��������
	void loadItemRewardExp(bool bUpdate);

	//�����ս��������������
	void reloadCityEndReward(bool bUpdate);
	void reloadSpeDomainReward(bool bUpdate);
	void loadNationAdminInf();
	void loadWeakNationBuff();
    
	//��ս����ɱ����������
	void cityKillRankReward(CWarCityPtr pWarCityPtr);

	//����������ȡ������Ϣ
	void getKillRankReward(uint8 accType, uint16 nRank, std::vector<CRewardInfo> &vecReward);
    
	//���������ɱ�ֵĽ���
	void addPlayerKillMonsterReward(CWarCityPtr pWarCityPtr, CPlayerCityWarInfPtr pPlayerCityWarPtr, OBJID idPlayer, uint32 idNpc);

	//��ȡɱ�ֽ���
	void getKillMonsterReward(CWarCityPtr pWarCityPtr, CPlayerCityWarInfPtr pPlayerCityWarPtr, OBJID idPlayer, uint32 idNpc, std::vector<CRewardInfo> &vecReward);
    
	//���³ǳ�ɱ�а�
	void updCityKillRank(CPlayerCityWarInfPtr pPlayerCityWarPtr);

	//ת��������Ϣ
	std::string conFormationUnit2Str(std::vector<CFormatUint> &vecUint);

	//ת��������Ϣ
	void conStr2FormationUnit(std::string strFormation,std::vector<CFormatUint> &vecUint);

	//��ȡBUDDYid
	void getBuddyID(std::vector<CFormatUint> &vecUint, std::vector<CBattleBuddy> &vecBuddy);
    
	//�������һ���¼��ǳ�
	uint8 getRandAccCity(uint8 idNation);

	//�������һ���¼��ǳ�
	uint8 getRandPrinceAccCity(std::vector<uint8> &vecBorCity, std::vector<uint8> &vecNorCity);
    
	uint8 getRandSpeAccCity();
	//ת���ַ��� �����͸���
	void convStr2Formation(std::string strFormation, std::vector<CWarNPCGrid> &vecGrid);
    
	//��ȡ��ս�����
	std::string getCityWarEndInf(CWarCityPtr pWarCityPtr);

	//ս������������
	void fightTeamResult(CWarCityPtr pWarCityPtr, uint8 nQueueNum);
    
	//������նͨ��
	void sendContinueKillNotice(CWarCityPtr pWarCityPtr, CPlayerCityWarInfPtr pPlayerCityWarPtr, uint16 nContinueKillNum, uint8 nVision);
   
	//����ɱ����NPC����Ӫ���ͽ���
	void rewardNationKillTeamReward(CWarCityPtr pWarCityPtr, CPlayerCityWarInfPtr pPlayerCityWarPtr, OBJID idPlayer, uint8 nTeamType, std::string &strNpcName);
    
	//��������µĽ���
	void addPlayerNewReward(CPlayerCityWarInfPtr pPlayerCityWarPtr, std::vector<CRewardInfo> &vecReward);
	bool checkConnectAdminCity(uint8 idNation, uint8 idCity);
	uint8 getDeclareAccID();
	CWarNationPtr  getWarNationPtr(uint8 idNation)
	{
		std::map<uint8, CWarNationPtr>::iterator iter = m_mapWarNation.find(idNation);
		if (iter == m_mapWarNation.end())
			return NULL;
		return iter->second;
	}
    
	//У���Ƿ��ж�ʱ�Ľ���
	void checkFixTimeReward(CWarJoinPlayerPtr pWarPlayerPtr, uint16 nFixTime);
   
	//�Ǽǹ��ҹ�Ͻ�ǳ�
	void regNationDomainCity(std::map<uint8, std::vector<uint8> > &mapNaitonDomain, uint16 nHour);
   
	//ת���ǳ�
	void convStrCity(std::string strCity, std::vector<uint8> &vecCity);
    
	//��ȡ��ռ��ĳǳ���Ϣ
	std::string getOccupCity();

	//��ȡ��ռ��ĳǳ���Ϣ
	void  convOccupCity(std::string strCity, std::vector<CCityOccup> &vecOccupCity);
    
	//���ҹ�Ͻ�ǳز���
	void nationCityReward(uint8 idNation, std::vector<uint8> &vecCity, std::vector<CCityOccup> &vecCityOccup, uint16 nHour);

    //��ȡ�佫����Ѫ��
	uint32 getTotBuddyHP(std::vector<CFormatUint> &vecUint);

	//��ȡ�����ǳ���Ϣ
	CProjectCityPtr getProCityPtr(uint8 idCity);

	//ɾ������
	void delEtapeTeam(std::vector<uint32> &vecTeamID);

	//���ҽ����������
	void nationDomainReward(uint8 nAccFlag);
   
	//��ȡ�ǳر�ռ��Ĺ���
	void getCityOccupNation(uint8 idCity, uint8 &idNation);
   
	//��ȡ��Ӫ��Ͻ�ĳǳ�
	void getNationOccupCity(uint8 idNation, std::vector<uint8> &vecCity);

	//�Ǽǳǳر�ռ����� 
	void regCityOccup(CWarCityPtr pWarCityPtr);

	//������Ӫ��Ͻ�ĳǳ�����
	uint8 decNaitonAdminCity(uint8 idNation);
    
	//������Ӫ��Ͻ�ĳǳ�����
	uint8 addNaitonAdminCity(uint8 idNation);

	//��ȡ��Ӫ��Ͻ�ĳǳ�����
	uint8 getNaitonAdminCityNum(uint8 idNation);

	//��ȡ����������BUFF
	void getWeakNationBuf(uint8 idNation, std::vector<CWarBuff> &vecBuff);

	//��ȡ�ǳ���Ӫ��BUFF
	void getCityNationBuff(uint8 idCity, uint8 idNation, std::vector<CWarBuff> &vecBuff);

	//��ȡ��ҵĹ�սBUFF
	void getPlayerWarBuff(uint8 idCity, CWorldBatTeam &cBatTeam);

	//���¹��ҹ�ս��Ϣ
	void updWarNation(CWarNationPtr pWarNationPtr, uint8 nKey);

	//��ȡ��ս����Ա
	void getJoinPlayer(std::vector<OBJID> &vecPlayer);

	//��ȡפ�سǳ���Ϣ
	CSatrapCityPtr getSatrapCityInf(uint8 nCity);

	void updateDBCitySatrapInf(uint8 idCity);

	void converCitySatrap2Str(uint8 idCity, std::string& strDst, CSatrapCityPtr pSatrapCity = NULL);
   
	//��ȡ��ҵ�̫�سǳ�ID
	uint8 getPlayerSatrapCity(OBJID idPlayer);

	//У���Ƿ��ж�ʱ�Ľ���
	void checkReturnSatrapCity(OBJID idPlayer);

	//��ս������ͨ��
	void warEndNotice(CWarCityPtr pWarCityPtr);
    
	//��ȡ���פ�����¼�
	void getPlayerDeployAcc(CWarJoinPlayerPtr pWarJoinPlayerPtr, std::vector<uint8> &vecDeployedAcc);
    
	//������ҵ���
	void returnItem();

    //У���¼��Ƿ񵽴����ʱ��
	void checkAccTimeEnd(CWarCityPtr pWarCityPtr);

	/************** Ⱥ����¹ begin  ********************/

	//��ʼ��Ⱥ����¹���¼�
	void initPrinceAcc(uint8 idNation, uint8 accNum, bool isElite); 
	//����Ⱥ����¹���¼�
	void resetPrinceAcc(std::vector<CPrinceCityAcc> &vecAccInf);
	//����ɱ���������� 
	void loadKillRank(std::vector<CDBPrinceKillRank> &vecKillRank);
	//�������ɱ������
	void addPlayerKillNum(OBJID idPlayer);
	//��¼ɱ�а����ݵ����ݿ�
	void regKillRankDB();
	//����ɱ�а���������
	void rewardKillRank();
	//У�齱���Ƿ���ȡ
	bool isHaveGetReward(std::string &strRewardInf, uint16 nKillNum);
    //���Ÿ���ɱ��������
	void rewardKillNum();
	/************** Ⱥ����¹ end  ********************/


    /************** ��ս begin  ********************/

	//У���Ƿ�����ս����
	bool isAllyDay();
	//���˴���
	void allyProc();
	//�����˹�������
	void reloadAllyData(bool bUpdate);
	//�������ӽ�����
	void weakNationAddGuard(uint8 idNation);


	/************** ��ս end  ********************/


private:
	std::vector<uint32> m_vecGateWaySession;

public:
	void incRef()
	{
		CRefShare::incRef();
	}

	bool decRef()
	{
		return CRefShare::decRef();
	}

	//������������
	void onDbmgrOpen();
	void onServerStop();

	void playerOnline(OBJID idPlayer);
	void playerOffline(OBJID idPlayer);
	void onTimer(uint32 nInterval);

private:
	uint8 m_ndbopenflag;
	uint8 m_ndbgetlogflag;
	uint8 m_ndbsatrapflag;
	uint32 m_nSessionID;

	uint32 m_nTotPraiseCnt; //ȫ��������

	uint32 m_nPickCDTime;                      //������CDʱ��
	uint32 m_nInitFightTime;                   //ÿ��ս��������ʱ�����
	uint32 m_nCountFightTime;                  //ÿ�غ���Ҫ��ȴ��CDʱ�� ����ֱȵ����� ����1000 Ϊ1��
	uint32 m_nFightTime;

	std::map<OBJID, CWarJoinPlayerPtr> m_mapWarJoinPlayer; //��ս��Ա��Ϣ
	std::map<uint8, CWarCityPtr> m_mapWarCityPtr;          //�ǳ��¼���Ϣ
	std::map<OBJID, CWarGroupPtr> m_mapGroupPtr;           //���ž���Ϣ key = groupid * 1000 + worldlevel
	std::vector<CCityRankReward> m_vecRankReward;          //��ս��������
	std::vector<CCityRankReward> m_vecRankReward1;          //��ҹ�����������
	std::map<uint8, CWarNationPtr> m_mapWarNation;         //��ս������Ϣ
	std::map<uint8, CProjectCityPtr> m_mapProCity;         //�����ǳ���Ϣ
	std::map<uint8, uint8>  m_mapCityOccup;                //�ǳصı仯��ϵ
	std::vector<CNationNpcRate> m_vecSpeCity;              //�����NPC�����ǳ� 
	uint32 m_nMaxSpeNpcRate ;                              //����NPC���Ǹ���
	std::map<uint8, uint8> m_mapNationAdimNum;             //���ҹ�Ͻ�ǳ�����
	std::map<uint8, std::vector<uint32> > m_mapWeakNationBuff; //��������BUFF
	std::map<OBJID, uint8> m_mapEnterPlayer;               //�ڹ�ս���������
	std::map<uint8, uint32> m_mapItemRewardExp1;            //���߽������� --����
	std::map<uint8, uint32> m_mapItemRewardExp2;            //���߽������� --����

	std::vector<CRewardInfo> m_vecDefSuccReward;   //���سɹ�����
	std::vector<CRewardInfo> m_vecDefFailRewad;    //����ʧ�ܽ���
	std::vector<CRewardInfo> m_vecAttFailReward;   //����ʧ�ܽ���
	std::vector<CRewardInfo> m_vecAttSuccRewad;    //�����ɹ�����
	std::map<uint16, std::vector<CRewardInfo> > m_mapSpeTimeReward; //���ⶨ�㽱��
	std::vector<CRewardInfo> m_vecKillTeam6Reward;  //��ɱ��������6�Ľ��� 
	std::vector<CRewardInfo> m_vecKillTeam5Reward;  //��ɱ��������6�Ľ���
	std::vector<CRewardInfo> m_vecKillTeam4Reward;  //��ɱ��������4�Ľ���
	std::vector<CRewardInfo> m_vecKillPlayerReward; //��ɱ��ҵĽ���

	std::map<uint8, std::vector<CExploitLibInfo> > m_mapExploitLibRecord; //��ѫ�����ȡ��¼
	uint32 m_nLastLibRcdSaveTime; //�ϴα��湦ѫ�����ȡ��¼ʱ��

	uint16 m_nDepoyedItemNum;                      //פ����������
	uint32 m_nDeployedItem;                        //פ������
	uint32 m_nDecDeployItem;                       //��ս��Ҫ��פ������
	uint16 m_nTotTeamLmt;                          //�ܲ�������
	uint16 m_nTeamListLmt;                         //���ض����б�����
	uint32 m_nKillTeam6Rate;                       //��ɱ��������6�Ľ�������
	uint32 m_nKillTeam4Rate;                       //��ɱ��������4�Ľ�������
	uint32 m_nAutoPickCd;                          //�Զ�������CDʱ��
	uint8  m_nDeclareAccID;                        //��ս�¼�ID
	uint32 m_nDeclareTime;                         //��ս��ս��׼��ʱ��
	uint32 m_nProcTime;                            //��ս������ı���ʱ��
	uint16 m_nDomainBeginTime;                     //�����������ʼʱ��
    uint16 m_nDomainEndTime;                       //��������Ľ���ʱ��
	uint8  m_nDeclareNumLmt;                       //������ս��������
	uint32 m_nKillPlayerRewardRate;                //��ɱ��ҵĸ��ʽ���     

	//�ǳ�פ�������Ϣ
	std::map<uint8, CSatrapCityPtr> m_mapSatrapCity;			//�����ǳ�פ����Ϣ
	std::map<OBJID, uint8> m_mapPlayerSatrapCity;				//���̫�سǳ���Ϣ
	std::map<uint8, std::string> m_mapCitySatrapDbFormation;	//��¼�ǳ���һ���س���ҵ�������Ϣ
	
	uint32 m_nSatrapBlowFightBuff;	//��ս̫���س�buff
	uint32 m_nCityChangeCd;			//�ǳ�����CD
    
	//Ⱥ����¹�¼���Ϣ
	std::vector<CPrinceCityAcc> m_vecPrinceAcc;           //Ⱥ����¹�¼���Ϣ
	std::vector<CDBPrinceKillRank> m_vecPrinceKillRank;    //Ⱥ����¹ɱ�а�
	std::map<OBJID, uint32> m_mapPlayerKillNum;          //���ɱ����
	std::vector<CCityRankReward> m_vecPrinceRankReward;  //Ⱥ����¹ɱ����������
	std::map<uint16, std::vector<CRewardInfo> > m_mapKillNumReward;  //ɱ��������
	uint8 m_nPrinceAccFlag;

    //��ս��Ϣ
	uint32 m_nAllyDay;                         //�����״�����
	uint32 m_nAllyCycle;                       //��������
	uint8 m_nAllyNation;                       //�ǽ��˹���
	uint8 m_nSignNaitonNum;                    //�������ҵĳǳ�����
	uint8 m_nTotNationNum;                     //���й��ҵĳǳ������ܺ�
	uint8 m_nLifeGuardNum;                     //ÿ�ս�������������
	uint8 m_nLifeGuardLmt;                     //����������
	uint8 m_nWeakNationCityNum;                //������Ͻ�ǳص�����
	std::map<uint16, uint8> m_mapViceNum;      //С������
	std::map<uint8, uint8> m_mapAllyAcc;       //��ս�¼�

};

typedef CRefObject<CWorldWarMgr> CWorldWarMgrPtr;

#endif
