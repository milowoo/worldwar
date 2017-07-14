/******************************************************
 *			Copyright 2015, by linglong.
 *				All right reserved.
 *功能：国战城池实例
 *日期：2015-5-12 15:05
 *作者：milo_woo
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
	ACC_TYPE_NPC            = 0, //NPC攻城战
	ACC_TYPE_PLAYER         = 1, //玩家攻城战
	ACC_TYPE_ALLY           = 2, //盟国攻城战
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
	 * @功能：定时
	 */
	void onTime();
    
	/**
	* @功能：返回事件ID
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
	* @功能：事件状态
	*/
	void setStatus(uint8 nstatus){m_nStatus = nstatus;}
	uint8 getStatus (){return m_nStatus;}

	//获取城战玩家信息
	CPlayerCityWarInfPtr getJoinPlayer(OBJID idPlayer);

	/**
	 * @功能：获取开战时间
	 */
	uint32 getBeginTime() const { return m_nBeginTime; }
	void setBeginTime(uint32 beginTime){m_nBeginTime = beginTime;}
	
	uint32 getEndTime() const{return m_nEndTime;}
	void setEndTime(uint32 endTime){m_nEndTime = endTime;}
    
	/**
	* @功能：获取队伍ID
	*/
	uint32 getTeamID();

	/**
	* @功能：获取序列号ID
	*/
	uint32 getSessionID(){return ++m_nSessionID;};
	uint32 getCurSession(){return m_nSessionID;};

	/**
	* @功能：获取排行榜序列号ID
	*/
	uint32 getRankSessionID(){return ++m_nRankSession;};
	uint32 getCurRankSession(){return m_nRankSession;};
    
	/**
	* @功能：获取战斗堆栈的序列号ID
	*/
	uint32 incQueueSessionID(uint8 queueNum);

	/**
	* @功能：获取序列号ID
	*/
	uint32 getSession1(){return ++m_nQueueSession1;};
	uint32 getCurSession1(){return m_nQueueSession1;};

	/**
	* @功能：获取序列号ID
	*/
	uint32 getSession2(){return ++m_nQueueSession2;};
	uint32 getCurSession2(){return m_nQueueSession2;};

	/**
	* @功能：获取序列号ID
	*/
	uint32 getSession3(){return ++m_nQueueSession3;};
	uint32 getCurSession3(){return m_nQueueSession3;};

	/**
	* @功能：初始化城池事件
	*/
	void init(uint8 idAcc, uint8 idCity, uint32 beginTime);


	/**
	* @功能：获取城池名称
	*/
	void setCityName(std::string strName){m_strName = strName;}
	std::string getCityName(){return m_strName;}

    
	/**
	* @功能：增加玩家防守队伍
	*/
	void addPlayerCityInf(OBJID idPlayer, CPlayerCityWarInfPtr pPlayerCityIntPtr);

	/**
	* @功能：玩家增加防守队伍
	*/
	void addPlayerBlowTeam(OBJID idPlayer, CPlayerCityWarInfPtr pPlayerCityIntPtr);

	/**
	* @功能：删除玩家防守队伍
	*/
	void delPlayerBlowTeam(CPlayerCityWarInfPtr pPlayerCityIntPtr);

	/**
	* @功能：修改玩家防守队伍
	*/

	/**
	* @功能：删除队列1的前面一个防守方人员
	*/
	void eraseBlowQueue1Team();
    void getBlowQueue1FirstTeam(CCitySTeam &cSTeam);

	/**
	* @功能：删除队列1的前面一个攻击方人员
	*/
	void eraseAttQueue1Team();
	void getAttQueue1FirstTeam(CCitySTeam &cSTeam);

	/**
	* @功能：删除队列2的前面一个防守方人员
	*/
	void eraseBlowQueue2Team();
	void getBlowQueue2FirstTeam(CCitySTeam &cSTeam);

	/**
	* @功能：删除队列2的前面一个攻击方人员
	*/
	void eraseAttQueue2Team();
	void getAttQueue2FirstTeam(CCitySTeam &cSTeam);


	/**
	* @功能：删除队列3的前面一个防守方人员
	*/
	void eraseBlowQueue3Team();
	void getBlowQueue3FirstTeam(CCitySTeam &cSTeam);

	void getBlowQueueFirstTeam(uint8 nQueueNum, CCitySTeam &cSTeam);

	/**
	* @功能：删除队列3的前面一个攻击方人员
	*/
	void eraseAttQueue3Team();
	void getAttQueue3FirstTeam(CCitySTeam &cSTeam);

	void getAttQueueFirstTeam(uint8 nQueueNum, CCitySTeam &cSTeam);

	/**
	* @功能：获取攻击队伍数量
	*/
	uint32 getAttTeamNum();

	/**
	* @功能：获取防守队伍数量
	*/
	uint32 getBlowTeamNum();

	/**
	* @功能：获取战斗堆栈信息
	*/
	uint8 getQueueStatus(){return m_nQueueStatus;}
	uint8 getQueueFail(){return m_nQueueFail;}
    
	/**
	* @功能：设置堆栈状态
	*/
	void setQueueStatus(uint8 queuestatus){m_nQueueStatus = queuestatus;}

	/**
	* @功能：设置车轮战的时间
	*/
	void  setQueueNextTime(uint8 nQueueNum, uint32 nNextTime);

	/**
	* @功能：设置堆栈时间
	*/
	void setQueue1NextTime(uint32 queueTime){m_nQueueNextTime1 = queueTime;}
	uint32 getQueue1NextTime(){return m_nQueueNextTime1;}

	/**
	* @功能：设置堆栈时间
	*/
	void setQueue2NextTime(uint32 queueTime){m_nQueueNextTime2 = queueTime;}
	uint32 getQueue2NextTime(){return m_nQueueNextTime2;}

	/**
	* @功能：设置堆栈时间
	*/
	void setQueue3NextTime(uint32 queueTime){m_nQueueNextTime3 = queueTime;}
	uint32 getQueue3NextTime(){return m_nQueueNextTime3;}


	uint32 getQueueNextTime(uint8 nQueueNum);

	/**
	* @功能：获取排行榜信息
	*/
	void getFightRank(std::vector<CKillRank> &vecKillRank);

	/**
	* @功能：更新排行榜信息
	*/
	void updFightRank(OBJID idPlayer, uint8 nation, uint16 killNum, std::string strNickName);

	/**
	* @功能：设置防守方队伍总数
	*/
	void setTotBlowNpcTeam(uint16 teamnum){m_nTotBlowNpcTeam = teamnum;}
	uint16 getTotBlowNpcTeam(){return m_nTotBlowNpcTeam;}

	/**
	* @功能：设置攻击方队伍总数
	*/
	void setTotAttNpcTeam(uint16 teamnum){m_nTotAttNpcTeam = teamnum;}
	uint16 getTotAttNpcTeam(){return m_nTotAttNpcTeam;}

	/**
	* @功能：是否已经拥有怪物队伍信息
	*/
	bool isHavaNPCTeam(uint32 idNPC);

	/**
	* @功能：增加初始的NPC队伍
	*/
	void addInitNPCTeam(uint32 idNPC, CWarNPCTeamPtr pNpcTeamPtr){m_mapNpcTeam.insert(make_pair(idNPC, pNpcTeamPtr));};

	/**
	* @功能：增加怪物队伍信息
	*/
	void  addNPCTeam(CWarNPCTeamPtr pWarNpcTeamPtr);

	/**
	* @功能：获取怪物队伍信息
	*/
	CWarNPCTeamPtr getNPCTeam(uint32 idNPC);

	/**
	* @功能：获取怪物的初始血量
	*/
	CMonsterInitHPPtr getMonsterInitHp(uint32 idMonster);

	/**
	* @功能：增加怪物的初始血量
	*/
	void addMonsterInitHp(uint32 idMonster, CMonsterInitHPPtr pMonsterInitPtr);

	/**
	* @功能：增加作战队伍
	*/
	void addFightTeam(CWarCityTeam &cCityTeam, uint8 nType);
    
	/**
	* @功能：获取战斗中的队伍
	*/
	void getFightTeam(std::vector<CCityFightTeam> &vecFightTeam);

	/**
	* @功能：获取战斗简报
	*/
	void getFightSReport(std::vector<CTeamBatReport> &vecFightReport);

	/**
	* @功能：是否在预备战斗中
	*/
	bool isInPreBatTeam(uint32 idTeam);

	/**
	* @功能：是否在单挑/被单挑队伍中
	*/
	bool isInPickBatTeam(uint32 idTeam);

	/**
	* @功能：删除攻击大本营中的队伍
	*/
	bool  eraseAttTeam(uint32 idTeam);

	/**
	* @功能：删除防守大本营中的队伍
	*/
	bool  eraseBlowTeam(uint32 idTeam);

	/**
	* @功能：增加防守大本营队伍
	*/
	bool  addBlowTeam(CWarCityTeam &cCityTeam);

	/**
	* @功能：增加攻击大本营队伍
	*/
	bool  addAttTeam(CWarCityTeam &cCityTeam);

	/**
	* @功能：获取城战队伍列表
	*/
	void  getTeamList(std::vector<CTeamList> &vecAttTeamList, std::vector<CTeamList> &vecBlowTeamList);

	/**
	* @功能：获取队伍信息
	*/
	uint16  getTeamInf(uint32 idTeam, uint8 &teamType, std::vector<CTeamDetail> &vecTeamDetail);

	/**
	* @功能：从大本营增加作战队伍
	*/
	bool  addNewTeamToAttFightQueue();

	/**
	* @功能：从大本营增加作战队伍
	*/
	bool  addNewTeamToBlowFightQueue();
    
	/**
	* @功能：转化的队伍
	*/
	bool convQueueTeamBatTeam(CCitySTeam &cSTeam, CWorldBatTeam &cAttBatTeam);

	/**
	* @功能：转化的队伍
	*/
	bool convCityTeamBatTeam(CWarCityTeam &cCityTeam, CWorldBatTeam &cBatTeam);

	/**
	* @功能：删除作战前面的第一个队伍
	*/
	void eraseQueueFistTeam(uint8 nQueueNum, uint32 idTeam, uint8 nType);

	/**
	* @功能：保存战报
	*/
	void setQueueBatReport(uint8 nQueueNum, std::string strBatReport);

	/**
	* @功能：保存战斗简报
	*/
	void setQueueTeamBatReport(uint8 nQueueNum, CTeamBatReport &cTeamBatReport);
	bool getQueueTeamBatReport(uint8 nQueueNum, CTeamBatReport &cTeamBatReport);

	/**
	* @功能：修改队伍的战斗阵型信息
	*/
	void updTeamBatGrid(uint32 idTeam, std::vector<CTeamBatInf> &vecGrid);

	/**
	* @功能：修改队伍的战斗阵型信息
	*/
	void updSucTeamGrid(uint8 nQueueNum, uint32 idTeam);
    
	/**
	* @功能：清理堆栈失败的队伍
	*/
	void clearQueueFailTeam(uint8 nQueueNum);

	/**
	* @功能：清理堆栈双方失败的队伍
	*/
	void clearQueueAllFailTeam(uint8 nQueueNum);
    
	/**
	* @功能：获取城战杀敌排行榜
	*/
	void getCityKillRank(std::vector<CKillRank> &vecCityRank);
   
	/**
	* @功能：设置防守成功奖励
	*/
	void setDefSuccReward(std::vector<CRewardInfo> &vecRewad){m_vecDefSuccReward.assign(vecRewad.begin(), vecRewad.end());}
    
	/**
	* @功能：设置防守失败奖励
	*/
	void setDefFailReward(std::vector<CRewardInfo> &vecRewad){m_vecDefFailRewad.assign(vecRewad.begin(), vecRewad.end());}
	void setAttSuccReward(std::vector<CRewardInfo> &vecRewad){m_vecAttSuccRewad.assign(vecRewad.begin(), vecRewad.end());}
	void setAttFailReward(std::vector<CRewardInfo> &vecRewad){m_vecAttFailReward.assign(vecRewad.begin(), vecRewad.end());}

	/**
	* @功能：城战个人奖励
	*/
	void  cityPlayerRewardAcc();

	/**
	* @功能：判断是否还有可以战斗的队伍
	*/
	bool  isHavaCanFightTeam(bool battak);
    
	/**
	* @功能：获取挑战被单挑的队伍
	*/
	void getPickTeam(bool battType, CWorldBatTeam &cBlowBatTeam, std::string &strName);

	/**
	* @功能：获取玩家虚拟战斗信息
	*/
	void getPlayerVirtualTeam(OBJID idPlayer, CPlayerFightTeamInf &cPlayerTeam);

	/**
	* @功能：把玩家的队伍放到挑战队列中
	*/
	void putPlayerTeamToPickTeam(OBJID idPlayer, uint32 idTeam);

	/**
	* @功能：单挑队伍回大本营处理
	*/
	void pickTeamReback(uint32 idTeam);

	/**
	* @功能：单挑队伍删除处理
	*/
	void pickTeamErase(uint32 idTeam);

	/**
	* @功能：单挑失败处理
	*/
	uint32 playerPickFail(uint32 idTeam, uint32 idBlowTeam);
    
	/**
	* @功能：更新杀敌榜
	*/
	void updCityKillRank(OBJID idPlayer, CPlayerCityWarInfPtr pPlayerCityWarPtr);

	/**
	* @功能：城战杀敌排名奖励清算
	*/
	void  getKillRank(std::vector<CWarCityRank> &vecKillRank){vecKillRank.clear(); vecKillRank.assign(m_vecCityRank.begin(), m_vecCityRank.end());};

	/**
	* @功能：增加作战队伍到攻击堆栈
	*/
	void  addTeamToAttQueue(uint8 nQueueNum, CWarCityTeam &cCityTeam);

	/**
	* @功能：增加作战队伍到攻击堆栈
	*/
	void  addTeamToAttQueueOrder(CWarCityTeam &cCityTeam);

	/**
	* @功能：获取队栈的战报
	*/
	std::string  getQueueBattleRepor(uint8 nQueueNum);
    
	/**
	* @功能：获取前一次车轮战的时间
	*/
	uint32 getWheelFightTime(){return m_nLastWheelFighTime;}
	void setWheelFightTime(uint32 nTime){m_nLastWheelFighTime = nTime;}

	/**
	* @功能：获取前一次自动单挑的时间
	*/
	uint32 getAutoPickTime(){return m_nAutoPickTime;}
	void setAutoPickTime(uint32 nTime){m_nAutoPickTime = nTime;}

	/**
	* @功能：清理单挑被单挑的队伍
	*/
	uint32 getClearPickTeamTime(){return m_nClearPickTeamTime;}
	void setClearPickTeamTime(uint32 nTime){m_nClearPickTeamTime = nTime;}
    
	/**
	* @功能：清理超时的单挑被单挑的队伍
	*/
	void clearPickTeam();
    
	/**
	* @功能：获取队列的序列号
	*/
	uint32 getQueueSession(uint8 nQueueNum);
    
	/**
	* @功能：获取战斗对战战斗队伍
	*/
	void getFightQueueTeam(uint8 nQueueNum, std::vector<CCityFightTeam> &vecFightTeam);

	/**
	* @功能：获取战斗对战战斗队伍
	*/
	void setConsummer(IGatewayMgrConsummerPtr pConsummer){m_pConsummer = pConsummer;};

	/**
	* @功能：设置队伍的剩余血量 
	*/
	void setTeamLastHP(uint32 idTeam, uint32 lastHP);


	/**
	* @功能：增加玩家队伍数量 
	*/
	void incPlayerBlowTeamNum(){m_nTotPlayerBlowTeam++;};

	/**
	* @功能：减少玩家队伍数量 
	*/
	void decPlayerBlowTeamNum(){m_nTotPlayerBlowTeam--;};

	uint16 getPlayerBlowTeamNum(){return m_nTotPlayerBlowTeam;}

	/**
	* @功能：增加玩家队伍数量 
	*/
	void incPlayerAttTeamNum(){m_nTotPlayerAttTeam++;};

	/**
	* @功能：减少玩家队伍数量 
	*/
	void decPlayerAttTeamNum(){m_nTotPlayerAttTeam--;};

	uint16 getPlayerAttTeamNum(){return m_nTotPlayerAttTeam;}

	/**
	* @功能：获取队伍的玩家ID 
	*/
	OBJID getTeamPlayerID(uint32 idTeam);
    
	/**
	* @功能：设置战斗队伍剩余血量 
	*/
	void setFightQueueGrid(uint8 queueNum, std::vector<CTeamBatInf> &vecGrid);

	/**
	* @功能：设置战斗队伍剩余血量 
	*/
	void setFightQueueTeamLastHp(uint8 queueNum);
    
	/**
	* @功能：获取队列的战斗队伍 
	*/
	void getQueueFightTeam(uint8 queueNum, CCityFightTeam &cFightTeam, CTeamBatReport &cbatReport);
   
	/**
	* @功能：获取战斗中的队伍
	*/
	void getInFightTeam(uint32 idTeam, CWarCityTeam &cCityTeam);

	/**
	* @功能：获取刚进入战斗的玩家
	*/
	void  getInFightPlayer(OBJID &idPlayer, uint32 &idTeam);

	/**
	* @功能：获取刚进入战斗的玩家
	*/
	void  updSteamLastHP(uint8 nQueueNum, uint32 nLastHP, uint8 nFailDir);

	/**
	* @功能：获取战斗队伍的血量
	*/
	uint32  getFightTeamHp(uint32 idTeam);
    
	/**
	* @功能：获取战斗队伍的血量
	*/
	void setDepoyedItem(uint32 idItem){m_nDeployedItem = idItem;}
	void setDepoyedItemNum(uint16 itemNum){m_nDepoyedItemNum = itemNum;}
    
	/**
	* @功能：返还驻兵道具
	*/
	void accEndRebackDeployItem();
    
	/**
	* @功能：设置队列剩余血量
	*/
	void setQueueLastHP(uint8 nQueueNum, uint32 lastHP);
	uint32 getQueueLastHP(uint8 nQueueNum);
   
	/**
	* @功能：设置队列列表返回数量
	*/
	void setTeamListLmt(uint16 nTeamNum){m_nTeamListLmt = nTeamNum;}
	uint16 getTeamListLmt(){return m_nTeamListLmt;}
   
	/**
	* @功能：获取城战的所有玩家
	*/
	void getCityPlayer(std::vector<OBJID> &vecPlayer);

	void addJoinPlayer(OBJID idPlayer);
	void eraseJoinPlayer(OBJID idPlayer);
    
	/**
	* @功能：获取城战的所有玩家
	*/
	void getAllDefPlayer(uint8 idNation, uint8 nDir, std::vector<OBJID> &vecPlayer);

	/**
	* @功能：获取单挑的NPC统帅队伍
	*/
	bool getAttPickCaptainTeam(CWarCityTeam &cNpcWarTeam, CWarCityTeam &cPlayerWarTeam);
    bool getBlowPickCaptainTeam(CWarCityTeam &cNpcWarTeam, CWarCityTeam &cPlayerWarTeam);
   
	/**
	* @功能：统帅队伍的处理 
	*/
	void addCaptainTeam(uint32 idTeam){m_mapCaptainTeam[idTeam] = 0;}
	bool isCaptainTeam(uint32 idTeam);
    
	/**
	* @功能：自爆兵队伍的处理 
	*/
	void addBlewTeam(uint32 idTeam, uint8 nType);
	bool isBlewTeam(uint32 idTeam);


	void setAccType(uint8 nType){m_nAccType = nType;}
	uint8 getAccType(){return m_nAccType;}
	void setProcTime(uint32 nTime){m_nProtctTime = nTime;}
	uint32 getProcTime(){return m_nProtctTime;}
    
	//设置道具奖励经验
	void setItemRewardExp(std::map<uint8, uint32> &mapItemRewardExp);
    
	//获取战报ID
	uint32 getBattleID();

    //保存战报
	void saveBattle(uint32 idBattle, std::string &strBattle);

	//查询战报
	std::string queryBattle(uint32 idBattle);

	void erasePlayerTeam(CPlayerCityWarInfPtr pPlayerCityWarPtr, uint32 idTeam);
	void checkQueueStatuts(uint8 nQueueNum);
    
	//清理队伍
	void clearDirtyFightTeam();

	/**
	* @功能：返还道具
	*/
	void returnDeployedItem();

private:
	/**
	* @功能：获取城战队伍列表
	*/
	void  getQueueTeamList(std::vector<CTeamList> &vecTeamList, std::vector<CCitySTeam> &vecQueueTeam);

	/**
	* @功能：获取城战队伍列表
	*/
	void  getCampTeamList(std::vector<CTeamList> &vecTeamList, std::vector<CWarCityTeam> &vecCityTeam);

	/**
	* @功能：增加作战队伍到攻击堆栈
	*/
	void  addTeamToBlowQueueOrder(CWarCityTeam &cCityTeam);

	/**
	* @功能：增加作战队伍到攻击堆栈
	*/
	bool  addNPCTeamToBlowQueue();
	bool  addNPCTeamToAttQueue();

	/**
	* @功能：增加作战队伍到防守堆栈
	*/
	void  addTeamToBlowQueue(uint8 nQueueNum, CWarCityTeam &cCityTeam);

	/**
	* @功能：从大本营取队伍存入攻击队伍
	*/
	bool  getCamTeamToAttQueue(uint8 nQueueNum);

	/**
	* @功能：从大本营取队伍存入防守队伍
	*/
	bool  getCamTeamToBlowQueue(uint8 nQueueNum);
    
	/**
	* @功能：判断攻击堆栈可以增加队伍
	*/
	bool isAttQueueFull(uint8 nQueueNum);

	/**
	* @功能：判断防守堆栈可以增加队伍
	*/
	bool isBlowQueueFull(uint8 nQueueNum);
   
    
	/**
	* @功能：清理玩家队伍
	*/
	void clearPlayerTeam(uint32 idTeam);

	/**
	* @功能：剩余队伍存放到大本营中 
	*/
	void teamRebackCamp(uint8 attType, std::vector<CCitySTeam> &vecQueueTeam);
   
	/**
	* @功能：获取自爆兵的被单挑队伍 
	*/
	bool getPickBlewTeam(bool battType, CWorldBatTeam &cBlowBatTeam, std::string &strName);
    
	//获取杀敌奖励
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
	uint8 m_nCityID;   //城池ID
	uint8 m_nAccType;   //事件类型 0 NCP事件 1玩家事件
	std::string m_strName; //城池名称
	uint8 m_nNation;   //国家
	uint8 m_nAttNaion;  //攻击国家
	uint8 m_nAccID;    //事件ID
	uint32 m_nBeginTime; //开始时间
	uint32 m_nEndTime;    //结束时间
	uint32 m_nProtctTime; //保护时间
	uint16 m_nTotBlowNpcTeam;                     //防守NPC队伍总数
	uint16 m_nTotAttNpcTeam;                      //攻击NPC队伍总数
	int16 m_nTotPlayerBlowTeam;                  //防守玩家队伍数量
	int16 m_nTotPlayerAttTeam;                   //攻击玩家队伍数量
	uint32 m_nTeamID;                             //队伍ID
	uint32 m_nSessionID;                          //序列号
	uint8 m_nStatus;                              //事件状态
	uint32 m_nLastWheelFighTime;                  //最新一次车轮战时间
	uint32 m_nClearPickTeamTime;                  //清理单挑队伍的时间
	uint8 m_nQueueStatus;                          //队列信息 0正常 8个字节，后3个低位表示3个队列 0表示正常 1表示摧毁
	uint8 m_nQueueFail;                            //队列摧毁信息 0正常 8个字节，后3个低位表示3个队列 0表示攻击方摧毁 1表示防守方摧毁
	uint16 m_nDepoyedItemNum;                      //驻兵道具数量
	uint32 m_nDeployedItem;                        //驻兵道具
	uint16 m_nTeamListLmt;                         //返回队伍列表限制
	uint32 m_nAutoPickTime;                         //自动单挑的时间
	uint32 m_nBattleID;                             //战报ID

	//自爆兵信息
	bool bHaveAttBlewTeam;                         //是否还存在攻击的自爆兵
	bool bHaveDefBlewTeam;                         //是否还存在防守的自爆兵
	std::map<uint32, uint8> m_mapBlewTeam;                //自爆兵队伍

	std::vector<CWarCityTeam> m_vecBlowNPCTeam;     //防守NPC队伍排序
	std::vector<CWarCityTeam> m_vecAttNPCTeam;      //进攻NPC队伍排序

	std::map<uint32, CWarNPCTeamPtr> m_mapNpcTeam;       //NPC队伍信息
	std::map<uint32, CMonsterInitHPPtr> m_mapMonsterHP;  //怪物血量

	std::vector<CWarCityTeam> m_vecBlowTeam;          //防守队伍信息  (大本营中参与玩家的队列)
	std::vector<CWarCityTeam> m_vecAttTeam;          //攻击队伍信息  (大本营中参与玩家的队列)

	std::map<OBJID, CPlayerCityWarInfPtr> m_mapPlayerTeam;     //玩家队伍信息

	std::map<uint32, CCityPickTeam> m_mapPickTeam;      //属于单挑或被单挑的队伍

	std::map<uint32, CWarCityTeam> m_mapFightTeam;  //战斗中的队伍 
	std::map<uint32, uint8>   m_mapCaptainTeam;   //统帅的队伍

	std::vector<CWarCityTeam> m_vecInFightPlayer;         //刚进入战斗的玩家，发通知后删除


	std::vector<CCitySTeam> m_vecAttQueue1Team;    //战队队列1攻击方
	std::vector<CCitySTeam> m_vecBlowQueue1Team;   //战队队列1防守方  
	CTeamBatReport cbatReport1;                    //战斗简报1
	std::string m_strBattleReport1;                 //战报1
	uint32 m_nQueueNextTime1;                      //队列1需要处理的时间
	uint32 m_nQueueSession1;                       //队列1session
	std::vector<CTeamBatInf> m_vecGrid1;            //战斗剩余血量
	uint32 m_nTeamLastHP1;                          //队伍剩余血量
	

	std::vector<CCitySTeam> m_vecAttQueue2Team;    //战队队列2攻击方
	std::vector<CCitySTeam> m_vecBlowQueue2Team;    //战队队列2防守方
	CTeamBatReport cbatReport2;                    //战斗简报2
	std::string m_strBattleReport2;                 //战报2
	uint32 m_nQueueNextTime2;                      //队列2需要处理的时间
	uint32 m_nQueueSession2;                       //队列2session
	std::vector<CTeamBatInf> m_vecGrid2;            //战斗剩余血量
	uint32 m_nTeamLastHP2;                          //队伍剩余血量

	std::vector<CCitySTeam> m_vecAttQueue3Team;    //战队队列3攻击方
	std::vector<CCitySTeam> m_vecBlowQueue3Team;    //战队队列3防守方
	CTeamBatReport cbatReport3;                    //战斗简报3
	std::string m_strBattleReport3;                 //战报3
	uint32 m_nQueueNextTime3;                      //队列3需要处理的时间
	uint32 m_nQueueSession3;                       //队列3session
	std::vector<CTeamBatInf> m_vecGrid3;            //战斗剩余血量
	uint32 m_nTeamLastHP3;                          //队伍剩余血量
	

	std::vector<CWarCityRank>  m_vecCityRank;      //杀敌榜信息
	uint32 m_nRankSession;

	std::map<OBJID, uint8> m_mapEnterPlayer;       //参与城战的玩家
	std::map<OBJID, uint8> m_mapFightPlayer;       //参加城战战斗的玩家
	std::vector<CRewardInfo> m_vecDefSuccReward;   //防守成功奖励
	std::vector<CRewardInfo> m_vecDefFailRewad;    //防守失败奖励
	std::vector<CRewardInfo> m_vecAttFailReward;   //进攻失败奖励
	std::vector<CRewardInfo> m_vecAttSuccRewad;    //进攻成功奖励
	IGatewayMgrConsummerPtr m_pConsummer;
	std::map<uint8, uint32> m_mapItemRewardExp;            //道具奖励经验
	std::map<uint32, std::string> m_mapBattle;        //战报信息
};
typedef CRefObject<CWarCity> CWarCityPtr;

#endif
