#ifndef __KPLAYERRELATION_DATADEF_H__
#define __KPLAYERRELATION_DATADEF_H__

// 师徒关系枚举
enum KE_RELATION_TRAINING
{
	emKRELATION_TRAINING_GRADUATE_LEVEL = 60,		// 出师级别
};

struct KNULL
{
	BOOL IsModify() { return FALSE; }
	LPVOID GetData() { return this; }
};

template <typename T>
struct KSIZE_T
{
	enum {size = sizeof(T)};
};

template <>
struct KSIZE_T<KNULL>
{
	enum {size = 0};
};

// 师徒错误代码
enum KE_RELATION_TRAINING_ERROR
{
	emKRELATION_TRAINING_ERR_NONE,
	emKRELATION_TRAINING_ERR_OFFLINE,	// 不在线
	emKRELATION_TRAINING_ERR1,			// 师父已经有了3个弟子，不能再接受弟子（发送给师父方）
	emKRELATION_TRAINING_ERR2,			// 师父已经有了3个弟子，不能再接受弟子（发送给弟子方）
	emKRELATION_TRAINING_ERR3,			// 弟子等级不够20级（发送给弟子方）
	emKRELATION_TRAINING_ERR4,			// 师父等级不够69级（发送给弟子方）
	emKRELATION_TRAINING_ERR5,			// 弟子等级必须低于69级（发送给师父方）
	emKRELATION_TRAINING_ERR6,			// 弟子等级必须低于69级（发送给弟子方）
	emKRELATION_TRAINING_ERR7,			// 师父不能是别人的当前弟子（发送给师父方）
	emKRELATION_TRAINING_ERR8,			// 师父不能是别人的当前弟子（发送给弟子方）
	emKRELATION_TRAINING_ERR9,			// 对方没有选中"允许拜师"选项
	emKRELATION_TRAINING_ERR10,			// 对方没有选中"允许收徒"选项
	emKRELATION_TRAINING_ERR11,			// 本月收徒人数已满（发送给弟子方）
	emKRELATION_TRAINING_ERR12,			// 本月收徒人数已满（发送给师父方）
	emKRELATION_TRAINING_ERR13,			// 对方已经有师父了
	emKRELATION_TRAINING_ERR14,			// 对方已经出师了
	emKERLATION_TRAINING_ERR15,			// 由于师傅钱不够，在申请方出现的提示
	emKERLATION_TRAINING_ERR16,			// 师傅钱不够， 
	emKERLATION_TRAINING_ERR17,			// 师徒，密友数量达到上限（发送给师父方）
	emKERLATION_TRAINING_ERR18,			// 师徒，密友数量达到上限（发送给师父方）
	emKRELATION_TRAINING_ERR19,			// 师傅，你等级不足69级，暂时不能接受弟子
	emKRELATION_TRAINING_ERR20,			// 弟子，你的等级还不满20级，不能拜师
	emKRELATION_TRAINING_ERR21,			// 弟子，你的等级超过了50级，不能拜师
	emKRELATION_TRAINING_ERR_COUNT
};
#endif //__KPLAYERRELATION_DATADEF_H__
