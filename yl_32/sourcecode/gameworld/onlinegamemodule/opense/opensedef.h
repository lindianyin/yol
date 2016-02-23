#pragma once

struct OpenseDayCfg
{
	INT nId;
	INT nAwardId;
	INT nFinishCount;
	INT nAutoGive;
};

struct OpenseCfg
{
	INT nId;
	INT nAwardId;
	INT nAutoGive;
	std::map<INT, OpenseDayCfg*> mapOpenseDay;
};