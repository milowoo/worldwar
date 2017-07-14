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

#define  EXPLOID_LIB_RECORD_SAVE_CD	5 * 60  //功勋宝箱获取记录保存数据库CD时间 5分钟


class CWorldWarMgr : public CSingleton<CWorldWarMgr>, public IStaticData,  public CRefShare
{
public:
	CWorldWarMgr();
	virtual ~CWorldWarMgr();
	friend class CRefObject<CWorldWarMgr>;

public:
	//国战通知
	IResponsePtr executeGWMWorldWarNotice(IRequestPtr Request, CCurrentPtr Current);
	//国战移动
	IResponsePtr executeGWMWorldWarMove(IRequestPtr Request, CCurrentPtr Current);
	//进入国战
	IResponsePtr executeGWMEnterWorldWar(IRequestPtr Request, CCurrentPtr Current);
	IResponsePtr executeGWMDeclareWar(IRequestPtr Request, CCurrentPtr Current);

	//进入城战城池
	IResponsePtr executeGWMEnterFightCity(IRequestPtr Request, CCurrentPtr Current);

	//获取城战队伍数量
	IResponsePtr executeGWMGetCityTeamNum(IRequestPtr Request, CCurrentPtr Current);

	//选择作战阵营
	IResponsePtr executeSelectFightDirect(IRequestPtr Request, CCurrentPtr Current);

	IResponsePtr executeEtapeDeployed(IRequestPtr Request,CCurrentPtr Current);
	IResponsePtr executeGetEtapeTeamList(IRequestPtr Request,CCurrentPtr Current);
	//城战驻兵
	IResponsePtr executeCityFightDeployed(IRequestPtr Request,CCurrentPtr Current);

	//获取城池战斗队列
	IResponsePtr executeGetCityFightTeam(IRequestPtr Request,CCurrentPtr Current);

	//获取战斗队列信息
	IResponsePtr executeGetFightQueue(IRequestPtr Request,CCurrentPtr Current);

	IResponsePtr executeDeployForce(IRequestPtr Request,CCurrentPtr Current);
	//撤兵
	IResponsePtr executeGWMCityDepart(IRequestPtr Request, CCurrentPtr Current);
	IResponsePtr executeGWMSetVision(IRequestPtr Request, CCurrentPtr Current);
	IResponsePtr executeGetDeployTeamList(IRequestPtr Request,CCurrentPtr Current);
	IResponsePtr executeBeatRetreat(IRequestPtr Request, CCurrentPtr Current);

	//单挑
	IResponsePtr executeGWMCityPick(IRequestPtr Request, CCurrentPtr Current);

	//单挑结果通知
	IResponsePtr executeGWMCityPickNotice(IRequestPtr Request, CCurrentPtr Current);

	//@获取城战队伍列表
	IResponsePtr executeGetCityTeamList(IRequestPtr Request,CCurrentPtr Current);
    
	//查询队伍阵型
	IResponsePtr  executeGetCityFightTeamInf(IRequestPtr Request,CCurrentPtr Current);

	//获取城战杀敌榜
	IResponsePtr executeQueryCityFightRank(IRequestPtr Request,CCurrentPtr Current);
	IResponsePtr executeQueryDomainRewardInf(IRequestPtr Request, CCurrentPtr Current);
	IResponsePtr executeExitFightCity(IRequestPtr Request, CCurrentPtr Current);
	IResponsePtr executeExitWorldWar(IRequestPtr Request, CCurrentPtr Current);

	//世界登录
	IResponsePtr executeWorldLogin(IRequestPtr Request, CCurrentPtr Current);

	//国战点赞
	IResponsePtr executeWorldPraise(IRequestPtr Request, CCurrentPtr Current);

	//记录功勋宝箱通告
	IResponsePtr executeRcdExploitLibNotice(IRequestPtr Request, CCurrentPtr Current);

	//查询战报
	IResponsePtr executeQueryCityBattleReport(IRequestPtr Request,CCurrentPtr Current);

	//查询国战战报
	IResponsePtr executeQueryWorldWarBattle(IRequestPtr Request,CCurrentPtr Current);

	IResponsePtr executeGMUpdCityOccup(IRequestPtr Request, CCurrentPtr Current);

	IResponsePtr executeGWMWorldWarOnline(IRequestPtr Request, CCurrentPtr Current);

	/************** 驻守城池相关协议  ********************/
	//查询城池驻守名将
	IResponsePtr executeQuerySatrapBuddy(IRequestPtr Request,CCurrentPtr Current);

	//查询城池太守信息
	IResponsePtr executeQuerySatrapInf(IRequestPtr Request,CCurrentPtr Current);

	//太守调整阵形
	IResponsePtr executeCitySatrapAdjustFormation(IRequestPtr Request,CCurrentPtr Current);

	//太守弃城
	IResponsePtr executeCitySatrapAbort(IRequestPtr Request,CCurrentPtr Current);

	//获取太守战斗数据
	IResponsePtr executeGetCitySatrapFightData(IRequestPtr Request,CCurrentPtr Current);

	//争夺太守结果通知
	IResponsePtr executeCitySatrapFightResultNotice(IRequestPtr Request,CCurrentPtr Current);

	//给太守发放俸禄
	void sendAwardMailToCitySatrap();

	void onResponseGetCitySatrapInfFromDB(IResponsePtr pResponse, CCurrentPtr Current, CRefSharePtr Param);

	/************** 驻守城池相关协议  ********************/


    /************** 群雄逐鹿 begin  ********************/

	//获取杀敌排行榜
	IResponsePtr executeGetKillRank(IRequestPtr Request,CCurrentPtr Current);

	//获取杀敌奖励
	IResponsePtr executeGetKillNumAward(IRequestPtr Request,CCurrentPtr Current);

	//获取群雄逐鹿事件
	IResponsePtr executeGetPrinceAcc(IRequestPtr Request,CCurrentPtr Current);


	//群雄逐鹿事件处理
	void princeAccProc(uint8 nType);
	//获取阵营城池信息(边境城池)
	void getNationPrinceAccCity(uint8 idNation, uint8 nCityNum, std::vector<uint8> &vecEliteCity, std::vector<uint8> &vecNorCity);
	//获取阵营城池信息(边境城池)
	void getSpeNationPrinceAccCity(uint8 idNation, std::vector<uint8> &vecEliteCity, std::vector<uint8> &vecNorCity);

	/************** 群雄逐鹿 end  **********************/


	/************** 国战盟战 begin  ********************/
	//获取国战盟战信息
	IResponsePtr executeGetAllyInf(IRequestPtr Request,CCurrentPtr Current);

	/************** 国战盟战 end  **********************/





	/**
	* @功能：处理json数据，转换成计算所需数据格式
	*/
	virtual void relolutionTable(bool bUpdate);
    
	//自动战斗应答处理
	void onResponseAutoCityTeamFight(IResponsePtr pResponse, CCurrentPtr Current, CRefSharePtr Param);

	//自动单挑应答处理
	void onResponseAutoPickTeamFight(IResponsePtr pResponse, CCurrentPtr Current, CRefSharePtr Param);
    
	//玩家获胜处理
	void playerSuccFight(CWarCityPtr pWarCityPtr, OBJID idPlayer, uint32 idTeam);

	//玩家获胜处理(同时阵亡)
	void playerDieSuccFight(CWarCityPtr pWarCityPtr, OBJID idPlayer, uint32 idTeam);
   
	//玩家战斗击杀玩家奖励
	void rewardPlayerKillPlayer(CWarCityPtr pWarCityPtr, OBJID idPlayer);

	//玩家战斗胜利怪物奖励
	void rewardPlayerKillMonster(CWarCityPtr pWarCityPtr, OBJID idPlayer, uint32 idNpc);

	//清理玩家的事件队伍信息
	void clearWarPlayerTeam(uint8 idCity);
    
	//NPC主动单挑处理
	void npcAutoPick(CWarCityPtr pWarCityPtr);
	//攻击NPC主动单挑处理
	void attNpcAutoPick(CWarCityPtr pWarCityPtr);
	//自动单挑处理
	void autoPickFight(CWarCityPtr pWarCityPtr, CWarCityTeam &cAttWarTeam, CWarCityTeam &cBlowWarTeam, uint32 nGWSession);

	void dailyAcc();
	void wheelFightReq(CWarCityPtr pWarCityPtr, uint8 nQueueNum, uint16 &iGatewaySessiin, std::vector<uint32> &vecGateWaySession);
	bool isCanFightQueue(CWarCityPtr pWarCityPtr, uint8 nQueueNum);
	uint8 checkWarStatus(CWarCityPtr pWarCityPtr);
	void erasePlayerTeam(CPlayerCityWarInfPtr pPlayerCityWarPtr, uint32 idTeam);
	void getNationAdminCity(std::vector<CCityOccup> &vecCityOccup, std::map<uint8, std::vector<uint8> > &mapNationCity);
    
	//宣战信息通知
	void decWarNotice(CWarCityPtr pWarCityPtr);

	uint8 getPrinceWarFlag();
     
	//兵站清理
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


	//玩家国战信息通知
	void warPlayerNotice(std::vector<OBJID> &vecPlayer, uint8 nKey, uint32 nValues);

	//玩家国战信息通知
	void playerWarNotice(OBJID idPlayer, uint8 nKey, uint32 nValues, std::string strData);
    
	void onResponseGetWorldWar(IResponsePtr pResponse, CCurrentPtr Current, CRefSharePtr Param);
	void onResponseGetPlayerTeam(IResponsePtr pResponse, CCurrentPtr Current, CRefSharePtr Param);
	void onResponseGetExploitLog(IResponsePtr pResponse, CCurrentPtr Current, CRefSharePtr Param);

    //获取参战人员信息
    CWarJoinPlayerPtr getWarJoinPlayer(OBJID idPlayer)
	{
		std::map<OBJID, CWarJoinPlayerPtr>::iterator iter = m_mapWarJoinPlayer.find(idPlayer);
		if (iter == m_mapWarJoinPlayer.end())
			return NULL;

		return iter->second;
	}
    
	//转化BUDDY信息
	void convDBBuddy(CDBBatBuddy &cDbBuddy, CBattleBuddy &cBatBuddy);

	//转化灵龙信息
	void convStrDragon(std::string strDragon, std::vector<CLSkillData> &Skill);
   
    
	//获取城池事件信息
	CWarCityPtr getWarCityPtr(uint8 idCity);
	CWarCityPtr getWarCityByAccID(uint8 idAcc);


	//初始化城池NPC队伍
	void initNpcTeam(CWarCityPtr pWarCityPtr, CWarGroupPtr pGroupPtr, uint8 type);
    
	//初始化城池NPC队伍
	void initAccCityNpcTeam(CWarCityPtr pWarCityPtr);

	void initCityDefTeam(CWarCityPtr pWarCityPtr);
	//获取国战集团军
	CWarGroupPtr getWarGroupPtr(uint32 idGroup , uint8 worldlevel);

    //城池车轮战
	void cityWheelWar(CWarCityPtr pWarCityPtr);
    
	//获取玩家战斗信息
	void getPlayerBatInf(CWorldBatTeam &cWorldBatTeam, uint8 idCity, uint8 nIsBlow = 0);
    
	//导入集团军的配置
	void reloadWorldGroup(bool bUpdate);
   
	//导入排名奖励的配置
	void reloadKillRankReward(bool bUpdate);
    
	//导入道具奖励经验表
	void loadItemRewardExp(bool bUpdate);

	//导入城战结束奖励的配置
	void reloadCityEndReward(bool bUpdate);
	void reloadSpeDomainReward(bool bUpdate);
	void loadNationAdminInf();
	void loadWeakNationBuff();
    
	//城战结束杀敌排名奖励
	void cityKillRankReward(CWarCityPtr pWarCityPtr);

	//根据排名获取奖励信息
	void getKillRankReward(uint8 accType, uint16 nRank, std::vector<CRewardInfo> &vecReward);
    
	//给玩家增加杀怪的奖励
	void addPlayerKillMonsterReward(CWarCityPtr pWarCityPtr, CPlayerCityWarInfPtr pPlayerCityWarPtr, OBJID idPlayer, uint32 idNpc);

	//获取杀怪奖励
	void getKillMonsterReward(CWarCityPtr pWarCityPtr, CPlayerCityWarInfPtr pPlayerCityWarPtr, OBJID idPlayer, uint32 idNpc, std::vector<CRewardInfo> &vecReward);
    
	//更新城池杀敌榜
	void updCityKillRank(CPlayerCityWarInfPtr pPlayerCityWarPtr);

	//转化阵型信息
	std::string conFormationUnit2Str(std::vector<CFormatUint> &vecUint);

	//转化阵型信息
	void conStr2FormationUnit(std::string strFormation,std::vector<CFormatUint> &vecUint);

	//获取BUDDYid
	void getBuddyID(std::vector<CFormatUint> &vecUint, std::vector<CBattleBuddy> &vecBuddy);
    
	//随机产生一个事件城池
	uint8 getRandAccCity(uint8 idNation);

	//随机产生一个事件城池
	uint8 getRandPrinceAccCity(std::vector<uint8> &vecBorCity, std::vector<uint8> &vecNorCity);
    
	uint8 getRandSpeAccCity();
	//转换字符串 到阵型格子
	void convStr2Formation(std::string strFormation, std::vector<CWarNPCGrid> &vecGrid);
    
	//获取城战最后结果
	std::string getCityWarEndInf(CWarCityPtr pWarCityPtr);

	//战斗队伍结果处理
	void fightTeamResult(CWarCityPtr pWarCityPtr, uint8 nQueueNum);
    
	//发送连斩通告
	void sendContinueKillNotice(CWarCityPtr pWarCityPtr, CPlayerCityWarInfPtr pPlayerCityWarPtr, uint16 nContinueKillNum, uint8 nVision);
   
	//给击杀名将NPC的阵营发送奖励
	void rewardNationKillTeamReward(CWarCityPtr pWarCityPtr, CPlayerCityWarInfPtr pPlayerCityWarPtr, OBJID idPlayer, uint8 nTeamType, std::string &strNpcName);
    
	//玩家增加新的奖励
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
    
	//校验是否有定时的奖励
	void checkFixTimeReward(CWarJoinPlayerPtr pWarPlayerPtr, uint16 nFixTime);
   
	//登记国家管辖城池
	void regNationDomainCity(std::map<uint8, std::vector<uint8> > &mapNaitonDomain, uint16 nHour);
   
	//转换城池
	void convStrCity(std::string strCity, std::vector<uint8> &vecCity);
    
	//获取被占领的城池信息
	std::string getOccupCity();

	//获取被占领的城池信息
	void  convOccupCity(std::string strCity, std::vector<CCityOccup> &vecOccupCity);
    
	//国家管辖城池产出
	void nationCityReward(uint8 idNation, std::vector<uint8> &vecCity, std::vector<CCityOccup> &vecCityOccup, uint16 nHour);

    //获取武将的总血量
	uint32 getTotBuddyHP(std::vector<CFormatUint> &vecUint);

	//获取保护城池信息
	CProjectCityPtr getProCityPtr(uint8 idCity);

	//删除队伍
	void delEtapeTeam(std::vector<uint32> &vecTeamID);

	//国家疆域产出处理
	void nationDomainReward(uint8 nAccFlag);
   
	//获取城池被占领的国家
	void getCityOccupNation(uint8 idCity, uint8 &idNation);
   
	//获取阵营管辖的城池
	void getNationOccupCity(uint8 idNation, std::vector<uint8> &vecCity);

	//登记城池被占领情况 
	void regCityOccup(CWarCityPtr pWarCityPtr);

	//减少阵营管辖的城池数量
	uint8 decNaitonAdminCity(uint8 idNation);
    
	//增加阵营管辖的城池数量
	uint8 addNaitonAdminCity(uint8 idNation);

	//获取阵营管辖的城池数量
	uint8 getNaitonAdminCityNum(uint8 idNation);

	//获取弱国保护的BUFF
	void getWeakNationBuf(uint8 idNation, std::vector<CWarBuff> &vecBuff);

	//获取城池阵营的BUFF
	void getCityNationBuff(uint8 idCity, uint8 idNation, std::vector<CWarBuff> &vecBuff);

	//获取玩家的国战BUFF
	void getPlayerWarBuff(uint8 idCity, CWorldBatTeam &cBatTeam);

	//更新国家国战信息
	void updWarNation(CWarNationPtr pWarNationPtr, uint8 nKey);

	//获取参战的人员
	void getJoinPlayer(std::vector<OBJID> &vecPlayer);

	//获取驻守城池信息
	CSatrapCityPtr getSatrapCityInf(uint8 nCity);

	void updateDBCitySatrapInf(uint8 idCity);

	void converCitySatrap2Str(uint8 idCity, std::string& strDst, CSatrapCityPtr pSatrapCity = NULL);
   
	//获取玩家的太守城池ID
	uint8 getPlayerSatrapCity(OBJID idPlayer);

	//校验是否有定时的奖励
	void checkReturnSatrapCity(OBJID idPlayer);

	//国战结束的通告
	void warEndNotice(CWarCityPtr pWarCityPtr);
    
	//获取玩家驻兵的事件
	void getPlayerDeployAcc(CWarJoinPlayerPtr pWarJoinPlayerPtr, std::vector<uint8> &vecDeployedAcc);
    
	//返还玩家道具
	void returnItem();

    //校验事件是否到达结束时间
	void checkAccTimeEnd(CWarCityPtr pWarCityPtr);

	/************** 群雄逐鹿 begin  ********************/

	//初始化群雄逐鹿的事件
	void initPrinceAcc(uint8 idNation, uint8 accNum, bool isElite); 
	//重置群雄逐鹿的事件
	void resetPrinceAcc(std::vector<CPrinceCityAcc> &vecAccInf);
	//导入杀敌排行数据 
	void loadKillRank(std::vector<CDBPrinceKillRank> &vecKillRank);
	//增加玩家杀敌数量
	void addPlayerKillNum(OBJID idPlayer);
	//记录杀敌榜数据到数据库
	void regKillRankDB();
	//发放杀敌榜排名奖励
	void rewardKillRank();
	//校验奖励是否领取
	bool isHaveGetReward(std::string &strRewardInf, uint16 nKillNum);
    //发放个人杀敌数奖励
	void rewardKillNum();
	/************** 群雄逐鹿 end  ********************/


    /************** 盟战 begin  ********************/

	//校验是否是盟战日期
	bool isAllyDay();
	//结盟处理
	void allyProc();
	//导入盟国的数据
	void reloadAllyData(bool bUpdate);
	//弱国增加禁卫军
	void weakNationAddGuard(uint8 idNation);


	/************** 盟战 end  ********************/


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

	//进程启动处理
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

	uint32 m_nTotPraiseCnt; //全服点赞数

	uint32 m_nPickCDTime;                      //单挑的CD时间
	uint32 m_nInitFightTime;                   //每次战斗的最少时间计算
	uint32 m_nCountFightTime;                  //每回合需要冷却的CD时间 按万分比的配置 比如1000 为1秒
	uint32 m_nFightTime;

	std::map<OBJID, CWarJoinPlayerPtr> m_mapWarJoinPlayer; //参战人员信息
	std::map<uint8, CWarCityPtr> m_mapWarCityPtr;          //城池事件信息
	std::map<OBJID, CWarGroupPtr> m_mapGroupPtr;           //集团军信息 key = groupid * 1000 + worldlevel
	std::vector<CCityRankReward> m_vecRankReward;          //城战排名奖励
	std::vector<CCityRankReward> m_vecRankReward1;          //玩家攻城排名奖励
	std::map<uint8, CWarNationPtr> m_mapWarNation;         //城战国家信息
	std::map<uint8, CProjectCityPtr> m_mapProCity;         //保护城池信息
	std::map<uint8, uint8>  m_mapCityOccup;                //城池的变化关系
	std::vector<CNationNpcRate> m_vecSpeCity;              //特殊的NPC攻击城池 
	uint32 m_nMaxSpeNpcRate ;                              //最大的NPC攻城概率
	std::map<uint8, uint8> m_mapNationAdimNum;             //国家管辖城池数量
	std::map<uint8, std::vector<uint32> > m_mapWeakNationBuff; //弱国保护BUFF
	std::map<OBJID, uint8> m_mapEnterPlayer;               //在国战场景的玩家
	std::map<uint8, uint32> m_mapItemRewardExp1;            //道具奖励经验 --虎符
	std::map<uint8, uint32> m_mapItemRewardExp2;            //道具奖励经验 --道具

	std::vector<CRewardInfo> m_vecDefSuccReward;   //防守成功奖励
	std::vector<CRewardInfo> m_vecDefFailRewad;    //防守失败奖励
	std::vector<CRewardInfo> m_vecAttFailReward;   //进攻失败奖励
	std::vector<CRewardInfo> m_vecAttSuccRewad;    //进攻成功奖励
	std::map<uint16, std::vector<CRewardInfo> > m_mapSpeTimeReward; //特殊定点奖励
	std::vector<CRewardInfo> m_vecKillTeam6Reward;  //击杀部队类型6的奖励 
	std::vector<CRewardInfo> m_vecKillTeam5Reward;  //击杀部队类型6的奖励
	std::vector<CRewardInfo> m_vecKillTeam4Reward;  //击杀部队类型4的奖励
	std::vector<CRewardInfo> m_vecKillPlayerReward; //击杀玩家的奖励

	std::map<uint8, std::vector<CExploitLibInfo> > m_mapExploitLibRecord; //功勋宝箱获取记录
	uint32 m_nLastLibRcdSaveTime; //上次保存功勋宝箱获取记录时间

	uint16 m_nDepoyedItemNum;                      //驻兵道具数量
	uint32 m_nDeployedItem;                        //驻兵道具
	uint32 m_nDecDeployItem;                       //宣战需要的驻兵道具
	uint16 m_nTotTeamLmt;                          //总部队限制
	uint16 m_nTeamListLmt;                         //返回队伍列表限制
	uint32 m_nKillTeam6Rate;                       //击杀部队类型6的奖励概率
	uint32 m_nKillTeam4Rate;                       //击杀部队类型4的奖励概率
	uint32 m_nAutoPickCd;                          //自动单挑的CD时间
	uint8  m_nDeclareAccID;                        //宣战事件ID
	uint32 m_nDeclareTime;                         //宣战后开战的准备时间
	uint32 m_nProcTime;                            //城战结束后的保护时间
	uint16 m_nDomainBeginTime;                     //疆域产出的起始时间
    uint16 m_nDomainEndTime;                       //疆域产出的结束时间
	uint8  m_nDeclareNumLmt;                       //国家宣战次数限制
	uint32 m_nKillPlayerRewardRate;                //击杀玩家的概率奖励     

	//城池驻守相关信息
	std::map<uint8, CSatrapCityPtr> m_mapSatrapCity;			//各个城池驻守信息
	std::map<OBJID, uint8> m_mapPlayerSatrapCity;				//玩家太守城池信息
	std::map<uint8, std::string> m_mapCitySatrapDbFormation;	//记录城池上一次守城玩家的阵形信息
	
	uint32 m_nSatrapBlowFightBuff;	//城战太守守城buff
	uint32 m_nCityChangeCd;			//城池易主CD
    
	//群雄逐鹿事件信息
	std::vector<CPrinceCityAcc> m_vecPrinceAcc;           //群雄逐鹿事件信息
	std::vector<CDBPrinceKillRank> m_vecPrinceKillRank;    //群雄逐鹿杀敌榜
	std::map<OBJID, uint32> m_mapPlayerKillNum;          //玩家杀敌数
	std::vector<CCityRankReward> m_vecPrinceRankReward;  //群雄逐鹿杀敌排名奖励
	std::map<uint16, std::vector<CRewardInfo> > m_mapKillNumReward;  //杀敌数奖励
	uint8 m_nPrinceAccFlag;

    //盟战信息
	uint32 m_nAllyDay;                         //结盟首次日期
	uint32 m_nAllyCycle;                       //结盟周期
	uint8 m_nAllyNation;                       //非结盟国家
	uint8 m_nSignNaitonNum;                    //单个国家的城池数量
	uint8 m_nTotNationNum;                     //所有国家的城池数量总和
	uint8 m_nLifeGuardNum;                     //每日奖励禁卫军数量
	uint8 m_nLifeGuardLmt;                     //禁卫军限制
	uint8 m_nWeakNationCityNum;                //弱国管辖城池的数量
	std::map<uint16, uint8> m_mapViceNum;      //小号限制
	std::map<uint8, uint8> m_mapAllyAcc;       //盟战事件

};

typedef CRefObject<CWorldWarMgr> CWorldWarMgrPtr;

#endif
