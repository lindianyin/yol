#ifndef KMagicAttribH
#define KMagicAttribH

#ifndef KD_MAGIC_INVALID2
#define	KD_MAGIC_INVALID2	magic_begin
#elif   KD_MAGIC_INVALID2 != magic_begin
#error	KD_MAGIC_INVALID2 definition not identical, please check your version of codes
#endif


enum MAGIC_ATTRIB1
{
	magic_begin						= 0x0000,

	modifydmg_p,
	modifydmg_v,
	addmaxlife_v,
	addmaxlife_p,
	addmaxmana_v,
	addmaxmana_p,
	addmaxpower_v,
	addmaxpower_p,
	addhitrate_v,
	addcriticalrate_v,
	addresist_v,
	addcriticaldmg_p,
	addrunspeed_v,
	adddef_v,
	adddodge_v,
	rebound_dmg_p,
	rebound_dmg_v,
	reducedmg,
	transdmgtolife,
	transdmgtomana,
	addtreatlife_v,
	addtreatlife_p,
	addtreatmana_v,
	addtreatmana_p,
	add_buffer,
	addweapondmg_v,
	addcurlife_v,
	addcurmana_v,
	addatkdmg,
	addrunspeed_p,

	magic_end,

};

#endif
