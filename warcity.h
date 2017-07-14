/******************************************************
 *			Copyright 2015, by linglong.
 *				All right reserved.
 *���ܣ���ս�ǳ�ʵ��
 *���ڣ�2015-5-12 15:05
 *���ߣ�milo_woo
 ******************************************************/
#ifndef _GWM_WAR_CITY_H_
#define _GWM_WAR_CITY_H_

#include <framework/daserver.h>
#include <framework/endpoint.h>
#include <util/refshare.h>
#include <common/util/singleton.h>
#include <gatewaymgr/gwmworldwar.h>
#include <gateway/worldwar.h>

#define MAX_GAME_CY_ONE_ITEM 888888
#define MAX_CCY_ONE_ITEM     400
#define MAX_LIB_ONE_ITEM     10
#define MAX_EXPLOIT_ONE_ITEM 2000

enum _e_acc_type_enum
{
	ACC_TYPE_NPC            = 0, //NPC����ս
	ACC_TYPE_PLAYER         = 1, //��ҹ���ս
	ACC_TYPE_ALLY           = 2, //�˹�����ս
};

class CWarCity : public CRefShare
{
public:
	CWarCity();

	virtual ~CWarCity();

	friend class CRefObject<CWarCity>;

public:
	void playerOnline(OBJID idPlayer);
	void playerOffline(OBJID idPlayer);
	void onServerStop();

	/**
	 * @���ܣ���ʱ
	 */
	void onTime();
    
	/**
	* @���ܣ������¼�ID
	*/
	uint8 getAccID(){return m_nAccID;}
	void setAccID(uint8 idAcc){m_nAccID = idAcc;}
    
	uint8 getCityID(){return m_nCityID;}
	void setCityID(uint8 idCity){m_nCityID = idCity;}
	void setNation(uint8 idNation){m_nNation = idNation;}
	uint8 getNation(){return m_nNation;}
	void setAttNation(uint8 idNation){m_nAttNaion = idNation;}
	uint8 getAttNation(){return m_nAttNaion;}
    
	/**
	* @���ܣ��¼�״̬
	*/
	void setStatus(uint8 nstatus){m_nStatus = nstatus;}
	uint8 getStatus (){return m_nStatus;}

	//��ȡ��ս�����Ϣ
	CPlayerCityWarInfPtr getJoinPlayer(OBJID idPlayer);

	/**
	 * @���ܣ���ȡ��սʱ��
	 */
	uint32 getBeginTime() const { return m_nBeginTime; }
	void setBeginTime(uint32 beginTime){m_nBeginTime = beginTime;}
	
	uint32 getEndTime() const{return m_nEndTime;}
	void setEndTime(uint32 endTime){m_nEndTime = endTime;}
    
	/**
	* @���ܣ���ȡ����ID
	*/
	uint32 getTeamID();

	/**
	* @���ܣ���ȡ���к�ID
	*/
	uint32 getSessionID(){return ++m_nSessionID;};
	uint32 getCurSession(){return m_nSessionID;};

	/**
	* @���ܣ���ȡ���а����к�ID
	*/
	uint32 getRankSessionID(){return ++m_nRankSession;};
	uint32 getCurRankSession(){return m_nRankSession;};
    
	/**
	* @���ܣ���ȡս����ջ�����к�ID
	*/
	uint32 incQueueSessionID(uint8 queueNum);

	/**
	* @���ܣ���ȡ���к�ID
	*/
	uint32 getSession1(){return ++m_nQueueSession1;};
	uint32 getCurSession1(){return m_nQueueSession1;};

	/**
	* @���ܣ���ȡ���к�ID
	*/
	uint32 getSession2(){return ++m_nQueueSession2;};
	uint32 getCurSession2(){return m_nQueueSession2;};

	/**
	* @���ܣ���ȡ���к�ID
	*/
	uint32 getSession3(){return ++m_nQueueSession3;};
	uint32 getCurSession3(){return m_nQueueSession3;};

	/**
	* @���ܣ���ʼ���ǳ��¼�
	*/
	void init(uint8 idAcc, uint8 idCity, uint32 beginTime);


	/**
	* @���ܣ���ȡ�ǳ�����
	*/
	void setCityName(std::string strName){m_strName = strName;}
	std::string getCityName(){return m_strName;}

    
	/**
	* @���ܣ�������ҷ��ض���
	*/
	void addPlayerCityInf(OBJID idPlayer, CPlayerCityWarInfPtr pPlayerCityIntPtr);

	/**
	* @���ܣ�������ӷ��ض���
	*/
	void addPlayerBlowTeam(OBJID idPlayer, CPlayerCityWarInfPtr pPlayerCityIntPtr);

	/**
	* @���ܣ�ɾ����ҷ��ض���
	*/
	void delPlayerBlowTeam(CPlayerCityWarInfPtr pPlayerCityIntPtr);

	/**
	* @���ܣ��޸���ҷ��ض���
	*/

	/**
	* @���ܣ�ɾ������1��ǰ��һ�����ط���Ա
	*/
	void eraseBlowQueue1Team();
    void getBlowQueue1FirstTeam(CCitySTeam &cSTeam);

	/**
	* @���ܣ�ɾ������1��ǰ��һ����������Ա
	*/
	void eraseAttQueue1Team();
	void getAttQueue1FirstTeam(CCitySTeam &cSTeam);

	/**
	* @���ܣ�ɾ������2��ǰ��һ�����ط���Ա
	*/
	void eraseBlowQueue2Team();
	void getBlowQueue2FirstTeam(CCitySTeam &cSTeam);

	/**
	* @���ܣ�ɾ������2��ǰ��һ����������Ա
	*/
	void eraseAttQueue2Team();
	void getAttQueue2FirstTeam(CCitySTeam &cSTeam);


	/**
	* @���ܣ�ɾ������3��ǰ��һ�����ط���Ա
	*/
	void eraseBlowQueue3Team();
	void getBlowQueue3FirstTeam(CCitySTeam &cSTeam);

	void getBlowQueueFirstTeam(uint8 nQueueNum, CCitySTeam &cSTeam);

	/**
	* @���ܣ�ɾ������3��ǰ��һ����������Ա
	*/
	void eraseAttQueue3Team();
	void getAttQueue3FirstTeam(CCitySTeam &cSTeam);

	void getAttQueueFirstTeam(uint8 nQueueNum, CCitySTeam &cSTeam);

	/**
	* @���ܣ���ȡ������������
	*/
	uint32 getAttTeamNum();

	/**
	* @���ܣ���ȡ���ض�������
	*/
	uint32 getBlowTeamNum();

	/**
	* @���ܣ���ȡս����ջ��Ϣ
	*/
	uint8 getQueueStatus(){return m_nQueueStatus;}
	uint8 getQueueFail(){return m_nQueueFail;}
    
	/**
	* @���ܣ����ö�ջ״̬
	*/
	void setQueueStatus(uint8 queuestatus){m_nQueueStatus = queuestatus;}

	/**
	* @���ܣ����ó���ս��ʱ��
	*/
	void  setQueueNextTime(uint8 nQueueNum, uint32 nNextTime);

	/**
	* @���ܣ����ö�ջʱ��
	*/
	void setQueue1NextTime(uint32 queueTime){m_nQueueNextTime1 = queueTime;}
	uint32 getQueue1NextTime(){return m_nQueueNextTime1;}

	/**
	* @���ܣ����ö�ջʱ��
	*/
	void setQueue2NextTime(uint32 queueTime){m_nQueueNextTime2 = queueTime;}
	uint32 getQueue2NextTime(){return m_nQueueNextTime2;}

	/**
	* @���ܣ����ö�ջʱ��
	*/
	void setQueue3NextTime(uint32 queueTime){m_nQueueNextTime3 = queueTime;}
	uint32 getQueue3NextTime(){return m_nQueueNextTime3;}


	uint32 getQueueNextTime(uint8 nQueueNum);

	/**
	* @���ܣ���ȡ���а���Ϣ
	*/
	void getFightRank(std::vector<CKillRank> &vecKillRank);

	/**
	* @���ܣ��������а���Ϣ
	*/
	void updFightRank(OBJID idPlayer, uint8 nation, uint16 killNum, std::string strNickName);

	/**
	* @���ܣ����÷��ط���������
	*/
	void setTotBlowNpcTeam(uint16 teamnum){m_nTotBlowNpcTeam = teamnum;}
	uint16 getTotBlowNpcTeam(){return m_nTotBlowNpcTeam;}

	/**
	* @���ܣ����ù�������������
	*/
	void setTotAttNpcTeam(uint16 teamnum){m_nTotAttNpcTeam = teamnum;}
	uint16 getTotAttNpcTeam(){return m_nTotAttNpcTeam;}

	/**
	* @���ܣ��Ƿ��Ѿ�ӵ�й��������Ϣ
	*/
	bool isHavaNPCTeam(uint32 idNPC);

	/**
	* @���ܣ����ӳ�ʼ��NPC����
	*/
	void addInitNPCTeam(uint32 idNPC, CWarNPCTeamPtr pNpcTeamPtr){m_mapNpcTeam.insert(make_pair(idNPC, pNpcTeamPtr));};

	/**
	* @���ܣ����ӹ��������Ϣ
	*/
	void  addNPCTeam(CWarNPCTeamPtr pWarNpcTeamPtr);

	/**
	* @���ܣ���ȡ���������Ϣ
	*/
	CWarNPCTeamPtr getNPCTeam(uint32 idNPC);

	/**
	* @���ܣ���ȡ����ĳ�ʼѪ��
	*/
	CMonsterInitHPPtr getMonsterInitHp(uint32 idMonster);

	/**
	* @���ܣ����ӹ���ĳ�ʼѪ��
	*/
	void addMonsterInitHp(uint32 idMonster, CMonsterInitHPPtr pMonsterInitPtr);

	/**
	* @���ܣ�������ս����
	*/
	void addFightTeam(CWarCityTeam &cCityTeam, uint8 nType);
    
	/**
	* @���ܣ���ȡս���еĶ���
	*/
	void getFightTeam(std::vector<CCityFightTeam> &vecFightTeam);

	/**
	* @���ܣ���ȡս����
	*/
	void getFightSReport(std::vector<CTeamBatReport> &vecFightReport);

	/**
	* @���ܣ��Ƿ���Ԥ��ս����
	*/
	bool isInPreBatTeam(uint32 idTeam);

	/**
	* @���ܣ��Ƿ��ڵ���/������������
	*/
	bool isInPickBatTeam(uint32 idTeam);

	/**
	* @���ܣ�ɾ��������Ӫ�еĶ���
	*/
	bool  eraseAttTeam(uint32 idTeam);

	/**
	* @���ܣ�ɾ�����ش�Ӫ�еĶ���
	*/
	bool  eraseBlowTeam(uint32 idTeam);

	/**
	* @���ܣ����ӷ��ش�Ӫ����
	*/
	bool  addBlowTeam(CWarCityTeam &cCityTeam);

	/**
	* @���ܣ����ӹ�����Ӫ����
	*/
	bool  addAttTeam(CWarCityTeam &cCityTeam);

	/**
	* @���ܣ���ȡ��ս�����б�
	*/
	void  getTeamList(std::vector<CTeamList> &vecAttTeamList, std::vector<CTeamList> &vecBlowTeamList);

	/**
	* @���ܣ���ȡ������Ϣ
	*/
	uint16  getTeamInf(uint32 idTeam, uint8 &teamType, std::vector<CTeamDetail> &vecTeamDetail);

	/**
	* @���ܣ��Ӵ�Ӫ������ս����
	*/
	bool  addNewTeamToAttFightQueue();

	/**
	* @���ܣ��Ӵ�Ӫ������ս����
	*/
	bool  addNewTeamToBlowFightQueue();
    
	/**
	* @���ܣ�ת���Ķ���
	*/
	bool convQueueTeamBatTeam(CCitySTeam &cSTeam, CWorldBatTeam &cAttBatTeam);

	/**
	* @���ܣ�ת���Ķ���
	*/
	bool convCityTeamBatTeam(CWarCityTeam &cCityTeam, CWorldBatTeam &cBatTeam);

	/**
	* @���ܣ�ɾ����սǰ��ĵ�һ������
	*/
	void eraseQueueFistTeam(uint8 nQueueNum, uint32 idTeam, uint8 nType);

	/**
	* @���ܣ�����ս��
	*/
	void setQueueBatReport(uint8 nQueueNum, std::string strBatReport);

	/**
	* @���ܣ�����ս����
	*/
	void setQueueTeamBatReport(uint8 nQueueNum, CTeamBatReport &cTeamBatReport);
	bool getQueueTeamBatReport(uint8 nQueueNum, CTeamBatReport &cTeamBatReport);

	/**
	* @���ܣ��޸Ķ����ս��������Ϣ
	*/
	void updTeamBatGrid(uint32 idTeam, std::vector<CTeamBatInf> &vecGrid);

	/**
	* @���ܣ��޸Ķ����ս��������Ϣ
	*/
	void updSucTeamGrid(uint8 nQueueNum, uint32 idTeam);
    
	/**
	* @���ܣ������ջʧ�ܵĶ���
	*/
	void clearQueueFailTeam(uint8 nQueueNum);

	/**
	* @���ܣ������ջ˫��ʧ�ܵĶ���
	*/
	void clearQueueAllFailTeam(uint8 nQueueNum);
    
	/**
	* @���ܣ���ȡ��սɱ�����а�
	*/
	void getCityKillRank(std::vector<CKillRank> &vecCityRank);
   
	/**
	* @���ܣ����÷��سɹ�����
	*/
	void setDefSuccReward(std::vector<CRewardInfo> &vecRewad){m_vecDefSuccReward.assign(vecRewad.begin(), vecRewad.end());}
    
	/**
	* @���ܣ����÷���ʧ�ܽ���
	*/
	void setDefFailReward(std::vector<CRewardInfo> &vecRewad){m_vecDefFailRewad.assign(vecRewad.begin(), vecRewad.end());}
	void setAttSuccReward(std::vector<CRewardInfo> &vecRewad){m_vecAttSuccRewad.assign(vecRewad.begin(), vecRewad.end());}
	void setAttFailReward(std::vector<CRewardInfo> &vecRewad){m_vecAttFailReward.assign(vecRewad.begin(), vecRewad.end());}

	/**
	* @���ܣ���ս���˽���
	*/
	void  cityPlayerRewardAcc();

	/**
	* @���ܣ��ж��Ƿ��п���ս���Ķ���
	*/
	bool  isHavaCanFightTeam(bool battak);
    
	/**
	* @���ܣ���ȡ��ս�������Ķ���
	*/
	void getPickTeam(bool battType, CWorldBatTeam &cBlowBatTeam, std::string &strName);

	/**
	* @���ܣ���ȡ�������ս����Ϣ
	*/
	void getPlayerVirtualTeam(OBJID idPlayer, CPlayerFightTeamInf &cPlayerTeam);

	/**
	* @���ܣ�����ҵĶ���ŵ���ս������
	*/
	void putPlayerTeamToPickTeam(OBJID idPlayer, uint32 idTeam);

	/**
	* @���ܣ���������ش�Ӫ����
	*/
	void pickTeamReback(uint32 idTeam);

	/**
	* @���ܣ���������ɾ������
	*/
	void pickTeamErase(uint32 idTeam);

	/**
	* @���ܣ�����ʧ�ܴ���
	*/
	uint32 playerPickFail(uint32 idTeam, uint32 idBlowTeam);
    
	/**
	* @���ܣ�����ɱ�а�
	*/
	void updCityKillRank(OBJID idPlayer, CPlayerCityWarInfPtr pPlayerCityWarPtr);

	/**
	* @���ܣ���սɱ��������������
	*/
	void  getKillRank(std::vector<CWarCityRank> &vecKillRank){vecKillRank.clear(); vecKillRank.assign(m_vecCityRank.begin(), m_vecCityRank.end());};

	/**
	* @���ܣ�������ս���鵽������ջ
	*/
	void  addTeamToAttQueue(uint8 nQueueNum, CWarCityTeam &cCityTeam);

	/**
	* @���ܣ�������ս���鵽������ջ
	*/
	void  addTeamToAttQueueOrder(CWarCityTeam &cCityTeam);

	/**
	* @���ܣ���ȡ��ջ��ս��
	*/
	std::string  getQueueBattleRepor(uint8 nQueueNum);
    
	/**
	* @���ܣ���ȡǰһ�γ���ս��ʱ��
	*/
	uint32 getWheelFightTime(){return m_nLastWheelFighTime;}
	void setWheelFightTime(uint32 nTime){m_nLastWheelFighTime = nTime;}

	/**
	* @���ܣ���ȡǰһ���Զ�������ʱ��
	*/
	uint32 getAutoPickTime(){return m_nAutoPickTime;}
	void setAutoPickTime(uint32 nTime){m_nAutoPickTime = nTime;}

	/**
	* @���ܣ��������������Ķ���
	*/
	uint32 getClearPickTeamTime(){return m_nClearPickTeamTime;}
	void setClearPickTeamTime(uint32 nTime){m_nClearPickTeamTime = nTime;}
    
	/**
	* @���ܣ�����ʱ�ĵ����������Ķ���
	*/
	void clearPickTeam();
    
	/**
	* @���ܣ���ȡ���е����к�
	*/
	uint32 getQueueSession(uint8 nQueueNum);
    
	/**
	* @���ܣ���ȡս����սս������
	*/
	void getFightQueueTeam(uint8 nQueueNum, std::vector<CCityFightTeam> &vecFightTeam);

	/**
	* @���ܣ���ȡս����սս������
	*/
	void setConsummer(IGatewayMgrConsummerPtr pConsummer){m_pConsummer = pConsummer;};

	/**
	* @���ܣ����ö����ʣ��Ѫ�� 
	*/
	void setTeamLastHP(uint32 idTeam, uint32 lastHP);


	/**
	* @���ܣ�������Ҷ������� 
	*/
	void incPlayerBlowTeamNum(){m_nTotPlayerBlowTeam++;};

	/**
	* @���ܣ�������Ҷ������� 
	*/
	void decPlayerBlowTeamNum(){m_nTotPlayerBlowTeam--;};

	uint16 getPlayerBlowTeamNum(){return m_nTotPlayerBlowTeam;}

	/**
	* @���ܣ�������Ҷ������� 
	*/
	void incPlayerAttTeamNum(){m_nTotPlayerAttTeam++;};

	/**
	* @���ܣ�������Ҷ������� 
	*/
	void decPlayerAttTeamNum(){m_nTotPlayerAttTeam--;};

	uint16 getPlayerAttTeamNum(){return m_nTotPlayerAttTeam;}

	/**
	* @���ܣ���ȡ��������ID 
	*/
	OBJID getTeamPlayerID(uint32 idTeam);
    
	/**
	* @���ܣ�����ս������ʣ��Ѫ�� 
	*/
	void setFightQueueGrid(uint8 queueNum, std::vector<CTeamBatInf> &vecGrid);

	/**
	* @���ܣ�����ս������ʣ��Ѫ�� 
	*/
	void setFightQueueTeamLastHp(uint8 queueNum);
    
	/**
	* @���ܣ���ȡ���е�ս������ 
	*/
	void getQueueFightTeam(uint8 queueNum, CCityFightTeam &cFightTeam, CTeamBatReport &cbatReport);
   
	/**
	* @���ܣ���ȡս���еĶ���
	*/
	void getInFightTeam(uint32 idTeam, CWarCityTeam &cCityTeam);

	/**
	* @���ܣ���ȡ�ս���ս�������
	*/
	void  getInFightPlayer(OBJID &idPlayer, uint32 &idTeam);

	/**
	* @���ܣ���ȡ�ս���ս�������
	*/
	void  updSteamLastHP(uint8 nQueueNum, uint32 nLastHP, uint8 nFailDir);

	/**
	* @���ܣ���ȡս�������Ѫ��
	*/
	uint32  getFightTeamHp(uint32 idTeam);
    
	/**
	* @���ܣ���ȡս�������Ѫ��
	*/
	void setDepoyedItem(uint32 idItem){m_nDeployedItem = idItem;}
	void setDepoyedItemNum(uint16 itemNum){m_nDepoyedItemNum = itemNum;}
    
	/**
	* @���ܣ�����פ������
	*/
	void accEndRebackDeployItem();
    
	/**
	* @���ܣ����ö���ʣ��Ѫ��
	*/
	void setQueueLastHP(uint8 nQueueNum, uint32 lastHP);
	uint32 getQueueLastHP(uint8 nQueueNum);
   
	/**
	* @���ܣ����ö����б�������
	*/
	void setTeamListLmt(uint16 nTeamNum){m_nTeamListLmt = nTeamNum;}
	uint16 getTeamListLmt(){return m_nTeamListLmt;}
   
	/**
	* @���ܣ���ȡ��ս���������
	*/
	void getCityPlayer(std::vector<OBJID> &vecPlayer);

	void addJoinPlayer(OBJID idPlayer);
	void eraseJoinPlayer(OBJID idPlayer);
    
	/**
	* @���ܣ���ȡ��ս���������
	*/
	void getAllDefPlayer(uint8 idNation, uint8 nDir, std::vector<OBJID> &vecPlayer);

	/**
	* @���ܣ���ȡ������NPCͳ˧����
	*/
	bool getAttPickCaptainTeam(CWarCityTeam &cNpcWarTeam, CWarCityTeam &cPlayerWarTeam);
    bool getBlowPickCaptainTeam(CWarCityTeam &cNpcWarTeam, CWarCityTeam &cPlayerWarTeam);
   
	/**
	* @���ܣ�ͳ˧����Ĵ��� 
	*/
	void addCaptainTeam(uint32 idTeam){m_mapCaptainTeam[idTeam] = 0;}
	bool isCaptainTeam(uint32 idTeam);
    
	/**
	* @���ܣ��Ա�������Ĵ��� 
	*/
	void addBlewTeam(uint32 idTeam, uint8 nType);
	bool isBlewTeam(uint32 idTeam);


	void setAccType(uint8 nType){m_nAccType = nType;}
	uint8 getAccType(){return m_nAccType;}
	void setProcTime(uint32 nTime){m_nProtctTime = nTime;}
	uint32 getProcTime(){return m_nProtctTime;}
    
	//���õ��߽�������
	void setItemRewardExp(std::map<uint8, uint32> &mapItemRewardExp);
    
	//��ȡս��ID
	uint32 getBattleID();

    //����ս��
	void saveBattle(uint32 idBattle, std::string &strBattle);

	//��ѯս��
	std::string queryBattle(uint32 idBattle);

	void erasePlayerTeam(CPlayerCityWarInfPtr pPlayerCityWarPtr, uint32 idTeam);
	void checkQueueStatuts(uint8 nQueueNum);
    
	//�������
	void clearDirtyFightTeam();

	/**
	* @���ܣ���������
	*/
	void returnDeployedItem();

private:
	/**
	* @���ܣ���ȡ��ս�����б�
	*/
	void  getQueueTeamList(std::vector<CTeamList> &vecTeamList, std::vector<CCitySTeam> &vecQueueTeam);

	/**
	* @���ܣ���ȡ��ս�����б�
	*/
	void  getCampTeamList(std::vector<CTeamList> &vecTeamList, std::vector<CWarCityTeam> &vecCityTeam);

	/**
	* @���ܣ�������ս���鵽������ջ
	*/
	void  addTeamToBlowQueueOrder(CWarCityTeam &cCityTeam);

	/**
	* @���ܣ�������ս���鵽������ջ
	*/
	bool  addNPCTeamToBlowQueue();
	bool  addNPCTeamToAttQueue();

	/**
	* @���ܣ�������ս���鵽���ض�ջ
	*/
	void  addTeamToBlowQueue(uint8 nQueueNum, CWarCityTeam &cCityTeam);

	/**
	* @���ܣ��Ӵ�Ӫȡ������빥������
	*/
	bool  getCamTeamToAttQueue(uint8 nQueueNum);

	/**
	* @���ܣ��Ӵ�Ӫȡ���������ض���
	*/
	bool  getCamTeamToBlowQueue(uint8 nQueueNum);
    
	/**
	* @���ܣ��жϹ�����ջ�������Ӷ���
	*/
	bool isAttQueueFull(uint8 nQueueNum);

	/**
	* @���ܣ��жϷ��ض�ջ�������Ӷ���
	*/
	bool isBlowQueueFull(uint8 nQueueNum);
   
    
	/**
	* @���ܣ�������Ҷ���
	*/
	void clearPlayerTeam(uint32 idTeam);

	/**
	* @���ܣ�ʣ������ŵ���Ӫ�� 
	*/
	void teamRebackCamp(uint8 attType, std::vector<CCitySTeam> &vecQueueTeam);
   
	/**
	* @���ܣ���ȡ�Ա����ı��������� 
	*/
	bool getPickBlewTeam(bool battType, CWorldBatTeam &cBlowBatTeam, std::string &strName);
    
	//��ȡɱ�н���
	void getkillReward(CPlayerCityWarInfPtr pPlayerWarPtr, std::vector<CRewardInfo> &vecPlayerReward);
	


public:
	void incRef()
	{
		CRefShare::incRef();
	}

	bool decRef()
	{
		return CRefShare::decRef();
	}

private:
	uint8 m_nCityID;   //�ǳ�ID
	uint8 m_nAccType;   //�¼����� 0 NCP�¼� 1����¼�
	std::string m_strName; //�ǳ�����
	uint8 m_nNation;   //����
	uint8 m_nAttNaion;  //��������
	uint8 m_nAccID;    //�¼�ID
	uint32 m_nBeginTime; //��ʼʱ��
	uint32 m_nEndTime;    //����ʱ��
	uint32 m_nProtctTime; //����ʱ��
	uint16 m_nTotBlowNpcTeam;                     //����NPC��������
	uint16 m_nTotAttNpcTeam;                      //����NPC��������
	int16 m_nTotPlayerBlowTeam;                  //������Ҷ�������
	int16 m_nTotPlayerAttTeam;                   //������Ҷ�������
	uint32 m_nTeamID;                             //����ID
	uint32 m_nSessionID;                          //���к�
	uint8 m_nStatus;                              //�¼�״̬
	uint32 m_nLastWheelFighTime;                  //����һ�γ���սʱ��
	uint32 m_nClearPickTeamTime;                  //�����������ʱ��
	uint8 m_nQueueStatus;                          //������Ϣ 0���� 8���ֽڣ���3����λ��ʾ3������ 0��ʾ���� 1��ʾ�ݻ�
	uint8 m_nQueueFail;                            //���дݻ���Ϣ 0���� 8���ֽڣ���3����λ��ʾ3������ 0��ʾ�������ݻ� 1��ʾ���ط��ݻ�
	uint16 m_nDepoyedItemNum;                      //פ����������
	uint32 m_nDeployedItem;                        //פ������
	uint16 m_nTeamListLmt;                         //���ض����б�����
	uint32 m_nAutoPickTime;                         //�Զ�������ʱ��
	uint32 m_nBattleID;                             //ս��ID

	//�Ա�����Ϣ
	bool bHaveAttBlewTeam;                         //�Ƿ񻹴��ڹ������Ա���
	bool bHaveDefBlewTeam;                         //�Ƿ񻹴��ڷ��ص��Ա���
	std::map<uint32, uint8> m_mapBlewTeam;                //�Ա�������

	std::vector<CWarCityTeam> m_vecBlowNPCTeam;     //����NPC��������
	std::vector<CWarCityTeam> m_vecAttNPCTeam;      //����NPC��������

	std::map<uint32, CWarNPCTeamPtr> m_mapNpcTeam;       //NPC������Ϣ
	std::map<uint32, CMonsterInitHPPtr> m_mapMonsterHP;  //����Ѫ��

	std::vector<CWarCityTeam> m_vecBlowTeam;          //���ض�����Ϣ  (��Ӫ�в�����ҵĶ���)
	std::vector<CWarCityTeam> m_vecAttTeam;          //����������Ϣ  (��Ӫ�в�����ҵĶ���)

	std::map<OBJID, CPlayerCityWarInfPtr> m_mapPlayerTeam;     //��Ҷ�����Ϣ

	std::map<uint32, CCityPickTeam> m_mapPickTeam;      //���ڵ����򱻵����Ķ���

	std::map<uint32, CWarCityTeam> m_mapFightTeam;  //ս���еĶ��� 
	std::map<uint32, uint8>   m_mapCaptainTeam;   //ͳ˧�Ķ���

	std::vector<CWarCityTeam> m_vecInFightPlayer;         //�ս���ս������ң���֪ͨ��ɾ��


	std::vector<CCitySTeam> m_vecAttQueue1Team;    //ս�Ӷ���1������
	std::vector<CCitySTeam> m_vecBlowQueue1Team;   //ս�Ӷ���1���ط�  
	CTeamBatReport cbatReport1;                    //ս����1
	std::string m_strBattleReport1;                 //ս��1
	uint32 m_nQueueNextTime1;                      //����1��Ҫ�����ʱ��
	uint32 m_nQueueSession1;                       //����1session
	std::vector<CTeamBatInf> m_vecGrid1;            //ս��ʣ��Ѫ��
	uint32 m_nTeamLastHP1;                          //����ʣ��Ѫ��
	

	std::vector<CCitySTeam> m_vecAttQueue2Team;    //ս�Ӷ���2������
	std::vector<CCitySTeam> m_vecBlowQueue2Team;    //ս�Ӷ���2���ط�
	CTeamBatReport cbatReport2;                    //ս����2
	std::string m_strBattleReport2;                 //ս��2
	uint32 m_nQueueNextTime2;                      //����2��Ҫ�����ʱ��
	uint32 m_nQueueSession2;                       //����2session
	std::vector<CTeamBatInf> m_vecGrid2;            //ս��ʣ��Ѫ��
	uint32 m_nTeamLastHP2;                          //����ʣ��Ѫ��

	std::vector<CCitySTeam> m_vecAttQueue3Team;    //ս�Ӷ���3������
	std::vector<CCitySTeam> m_vecBlowQueue3Team;    //ս�Ӷ���3���ط�
	CTeamBatReport cbatReport3;                    //ս����3
	std::string m_strBattleReport3;                 //ս��3
	uint32 m_nQueueNextTime3;                      //����3��Ҫ�����ʱ��
	uint32 m_nQueueSession3;                       //����3session
	std::vector<CTeamBatInf> m_vecGrid3;            //ս��ʣ��Ѫ��
	uint32 m_nTeamLastHP3;                          //����ʣ��Ѫ��
	

	std::vector<CWarCityRank>  m_vecCityRank;      //ɱ�а���Ϣ
	uint32 m_nRankSession;

	std::map<OBJID, uint8> m_mapEnterPlayer;       //�����ս�����
	std::map<OBJID, uint8> m_mapFightPlayer;       //�μӳ�սս�������
	std::vector<CRewardInfo> m_vecDefSuccReward;   //���سɹ�����
	std::vector<CRewardInfo> m_vecDefFailRewad;    //����ʧ�ܽ���
	std::vector<CRewardInfo> m_vecAttFailReward;   //����ʧ�ܽ���
	std::vector<CRewardInfo> m_vecAttSuccRewad;    //�����ɹ�����
	IGatewayMgrConsummerPtr m_pConsummer;
	std::map<uint8, uint32> m_mapItemRewardExp;            //���߽�������
	std::map<uint32, std::string> m_mapBattle;        //ս����Ϣ
};
typedef CRefObject<CWarCity> CWarCityPtr;

#endif
