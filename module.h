#ifndef _MODULE_H_
#define _MODULE_H_

#include <public.h>
#include <framework/daserver.h>
#include <framework/servant.h>
#include <business/bussinesscommon.h>
#include <business/consummer/igatewaymgrconsummer.h>
#include "worldwarconsummer.h"

extern CDAServerPtr g_DaServer;
extern IGatewayMgrConsummerPtr g_pConsummer;
extern CWorldWarConsummerPtr g_pProvider;

#ifdef WIN32
extern "C" __declspec(dllexport) void initDllServant(CDAServerPtr pServer, CLogPtr pLog, CSystemLogPtr pSystemLog, IConsummerPtr pConsumer, std::map<uint32, IServantPtr>& mapServant);
extern "C" __declspec(dllexport) void getConsummer(IConsummerPtr& pConsumer);
#else
extern "C" void initDllServant(CDAServerPtr pServer, CLogPtr pLog, CSystemLogPtr pSystemLog, IConsummerPtr pConsumer, std::map<uint32, IServantPtr>& mapServant);
extern "C" void getConsummer(IConsummerPtr& pConsumer);
#endif

#endif