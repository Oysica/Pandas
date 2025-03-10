﻿// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef NPC_HPP
#define NPC_HPP

#include "../common/timer.hpp"

#include "map.hpp" // struct block_list
#include "status.hpp" // struct status_change
#include "unit.hpp" // struct unit_data

struct block_list;
struct npc_data;
struct view_data;

struct npc_timerevent_list {
	int timer,pos;
};

struct npc_label_list {
	char name[NAME_LENGTH+1];
	int pos;
};

/// Item list for NPC sell/buy list
struct npc_item_list {
	t_itemid nameid;
	unsigned int value;
#if PACKETVER >= 20131223
	unsigned short qty; ///< Stock counter (Market shop)
	uint8 flag; ///< 1: Item added by npcshopitem/npcshopadditem, force load! (Market shop)
#endif
};

#if !defined(sun) && (!defined(__NETBSD__) || __NetBSD_Version__ >= 600000000) // NetBSD 5 and Solaris don't like pragma pack but accept the packed attribute
#pragma pack(push, 1)
#endif // not NetBSD < 6 / Solaris

/// List of bought/sold item for NPC shops
struct s_npc_buy_list {
	unsigned short qty;		///< Amount of item will be bought
#if PACKETVER_MAIN_NUM >= 20181121 || PACKETVER_RE_NUM >= 20180704 || PACKETVER_ZERO_NUM >= 20181114
	uint32 nameid;	///< ID of item will be bought
#else
	uint16 nameid;	///< ID of item will be bought
#endif
} __attribute__((packed));

#if !defined(sun) && (!defined(__NETBSD__) || __NetBSD_Version__ >= 600000000) // NetBSD 5 and Solaris don't like pragma pack but accept the packed attribute
#pragma pack(pop)
#endif // not NetBSD < 6 / Solaris

struct s_questinfo {
	e_questinfo_types icon;
	e_questinfo_markcolor color;
	struct script_code* condition;

	~s_questinfo(){
		if( this->condition != nullptr ){
			script_free_code( this->condition );
		}
	}
};

#ifdef Pandas_Redeclaration_Struct_Event_Data
struct event_data {
	struct npc_data* nd;
	int pos;
};
#endif // Pandas_Redeclaration_Struct_Event_Data

struct npc_data {
	struct block_list bl;
	struct unit_data ud; //Because they need to be able to move....
	struct view_data vd;
	struct status_change sc; //They can't have status changes, but.. they want the visual opt values.
	struct npc_data *master_nd;
	short class_,speed;
	char name[NPC_NAME_LENGTH+1];// display name
	char exname[NPC_NAME_LENGTH+1];// unique npc name
	int chat_id,touching_id;
	unsigned int next_walktime;
	int instance_id;

	unsigned size : 2;

	struct status_data status;
	unsigned int level,stat_point;
	struct s_npc_params {
		pec_ushort str, agi, vit, int_, dex, luk;
	} params;

	void* chatdb; // pointer to a npc_parse struct (see npc_chat.cpp)
	char* path;/* path dir */
	enum npc_subtype subtype;
	bool trigger_on_hidden;
	int src_id;
	union {
		struct {
			struct script_code *script;
			short xs,ys; // OnTouch area radius
			int guild_id;
			t_tick timer;
			int timerid,timeramount,rid;
			t_tick timertick;
			struct npc_timerevent_list *timer_event;
			int label_list_num;
			struct npc_label_list *label_list;
		} scr;
		struct {
			struct npc_item_list *shop_item;
			uint16 count;
			t_itemid itemshop_nameid; // Item Shop cost item ID
			char pointshop_str[32]; // Point Shop cost variable name
#ifdef Pandas_Support_Pointshop_Variable_DisplayName
			// --------------------------------------------------------------------------
			// 提醒: 针对 npc_data.u.shop 中添加的内容, 需要在复制 npc 的时候也同时进行复制
			// 涉及到的函数有 npc.cpp 的 npc_parse_duplicate 和 copynpc 脚本指令 [Sola丶小克]
			// --------------------------------------------------------------------------
			char pointshop_str_nick[64]; // 用于保存变量的昵称, 以便呈现给玩家
#endif // Pandas_Support_Pointshop_Variable_DisplayName
			bool discount;
		} shop;
		struct {
			short xs,ys; // OnTouch area radius
			short x,y; // destination coords
			unsigned short mapindex; // destination map
		} warp;
		struct {
			struct mob_data *md;
			time_t kill_time;
			char killer_name[NAME_LENGTH];
			int spawn_timer;
		} tomb;
	} u;

	struct sc_display_entry **sc_display;
	unsigned char sc_display_count;

	std::vector<std::shared_ptr<s_questinfo>> qi_data;

	struct {
		t_tick timeout;
		unsigned long color;
	} progressbar;

#ifdef Pandas_ScriptCommand_ShowVend
	struct {
		char message[NAME_LENGTH + 1];
		bool show;
	} vendingboard;
#endif // Pandas_ScriptCommand_ShowVend

#ifdef Pandas_Struct_Npc_Data_Pandas
	struct {
#ifdef Pandas_Struct_Npc_Data_DestructionStrategy
		int destruction_strategy = 0;	// 记录当前 NPC 的自毁策略 ( 0 - 不自毁; 1 - 最后一个对话结束时自毁)
		int destruction_timer = INVALID_TIMER; // 记录当前 NPC 的立刻自毁计时器
#endif // Pandas_Struct_Npc_Data_DestructionStrategy
	} pandas;
#endif // Pandas_Struct_Npc_Data_Pandas

#ifdef Pandas_Struct_Unit_CommonData
	struct s_unit_common_data ucd;
#endif // Pandas_Struct_Unit_CommonData
};

struct eri;
extern struct eri *npc_sc_display_ers;

#define START_NPC_NUM 110000000

enum e_job_types
{
	NPC_RANGE1_START = 44,
	JT_WARPNPC,
	JT_1_ETC_01,
	JT_1_M_01,
	JT_1_M_02,
	JT_1_M_03,
	JT_1_M_04,
	JT_1_M_BARD,
	JT_1_M_HOF,
	JT_1_M_INNKEEPER,
	JT_1_M_JOBGUIDER,
	JT_1_M_JOBTESTER,
	JT_1_M_KNIGHTMASTER,
	JT_1_M_LIBRARYMASTER,
	JT_1_M_MERCHANT,
	JT_1_M_ORIENT01,
	JT_1_M_PASTOR,
	JT_1_M_PUBMASTER,
	JT_1_M_SIZ,
	JT_1_M_SMITH,
	JT_1_M_WIZARD,
	JT_1_M_YOUNGKNIGHT,
	JT_1_F_01,
	JT_1_F_02,
	JT_1_F_03,
	JT_1_F_04,
	JT_1_F_GYPSY,
	JT_1_F_LIBRARYGIRL,
	JT_1_F_MARIA,
	JT_1_F_MERCHANT_01,
	JT_1_F_MERCHANT_02,
	JT_1_F_ORIENT_01,
	JT_1_F_ORIENT_02,
	JT_1_F_ORIENT_03,
	JT_1_F_ORIENT_04,
	JT_1_F_PRIEST,
	JT_1_F_PUBGIRL,
	JT_4_DOG01,
	JT_4_KID01,
	JT_4_M_01,
	JT_4_M_02,
	JT_4_M_03,
	JT_4_M_04,
	JT_4_M_BARBER,
	JT_4_M_ORIENT01,
	JT_4_M_ORIENT02,
	JT_4_F_01,
	JT_4_F_02,
	JT_4_F_03,
	JT_4_F_04,
	JT_4_F_MAID,
	JT_4_F_SISTER,
	JT_4W_KID,
	JT_4W_M_01,
	JT_4W_M_02,
	JT_4W_M_03,
	JT_4W_SAILOR,
	JT_4W_F_01,
	JT_8_F,
	JT_8_F_GRANDMOTHER,
	JT_EFFECTLAUNCHER,
	JT_8W_SOLDIER,
	JT_1_M_MOC_LORD,
	JT_1_M_PAY_ELDER,
	JT_1_M_PRON_KING,
	JT_4_M_MANAGER,
	JT_4_M_MINISTER,
	JT_HIDDEN_NPC,
	JT_4_F_KAFRA6,
	JT_4_F_KAFRA5,
	JT_4_F_KAFRA4,
	JT_4_F_KAFRA3,
	JT_4_F_KAFRA2,
	JT_4_F_KAFRA1,
	JT_2_M_THIEFMASTER,
	JT_2_M_SWORDMASTER,
	JT_2_M_PHARMACIST,
	JT_2_M_MOLGENSTEIN,
	JT_2_M_DYEINGER,
	JT_2_F_MAGICMASTER,
	JT_4_F_TELEPORTER,
	JT_4_M_TELEPORTER,
	NPC_RANGE1_END,

	JT_HIDDEN_WARP_NPC = 139,

	NPC_RANGE2_START = 400,
	JT_4_M_MUT2,
	JT_4_M_SCIENCE,
	JT_4_F_VALKYRIE2,
	JT_4_M_UNCLEKNIGHT,
	JT_4_M_YOUNGKNIGHT,
	JT_2_MONEMUS,
	JT_4_M_ATEIL,
	JT_4_F_ANNIVERSARY,
	JT_4_M_GREATPO,
	JT_4_M_NOVELIST,
	JT_4_M_CHAMPSOUL,
	JT_4_M_OLDFRIAR,
	JT_4_M_CRU_SOLD,
	JT_4_M_CRU_KNT,
	JT_4_M_CRU_HEAD,
	JT_4_M_CRU_CRUA,
	JT_4_M_KY_SOLD,
	JT_4_M_KY_KNT,
	JT_4_M_KY_HEAD,
	JT_4_M_KY_KIYOM,
	JT_4_M_BOSSCAT,
	JT_4_M_BABYCAT,
	JT_4W_F_KAFRA2,
	JT_4_F_MUNAK,
	JT_4_M_BONGUN,
	JT_4_BEAR,
	JT_4_BLUEWOLF,
	JT_4_PECOPECO,
	JT_4_M_JP_MID,
	JT_4_M_JP_RUN,
	JT_4_ORCLADY,
	JT_4_ORCLADY2,
	JT_4_ORCWARRIOR,
	JT_4_ORCWARRIOR2,
	JT_4_F_FAIRY,
	JT_4_F_FAIRYKID,
	JT_4_F_FAIRYKID2,
	JT_4_F_FAIRYKID3,
	JT_4_F_FAIRYKID4,
	JT_4_F_FAIRYKID5,
	JT_4_F_FAIRYKID6,
	JT_4_M_FAIRYKID,
	JT_4_M_FAIRYKID2,
	JT_4_M_FAIRYKID3,
	JT_4_M_FAIRYKID4,
	JT_4_M_FAIRYKID5,
	JT_4_M_FAIRYSOLDIER,
	JT_4_M_TUFFOLD,
	JT_4_MAN_BENKUNI,
	JT_4_MAN_GALTUN,
	JT_4_MAN_JERUTOO,
	JT_4_MAN_LAVAIL,
	JT_4_MAN_NITT,
	JT_4_MAN_PIOM,
	JT_4_MAN_PIOM2,
	JT_4_M_DSTMAN,
	JT_4_M_DSTMANDEAD,
	JT_4_BABYLEOPARD,
	JT_4_M_REDSWORD,
	JT_4_MAN_PIOM3,
	JT_4_M_FAIRYSOLDIER2,
	JT_4_F_FAIRYSOLDIER,
	JT_4_DRAGON_EGG,
	JT_4_MIMIC,
	JT_4_F_FAIRY1,
	JT_4_F_GUILLOTINE,
	JT_4_M_GUILLOTINE,
	JT_4_M_KNIGHT_BLACK,
	JT_4_M_KNIGHT_GOLD,
	JT_4_M_KNIGHT_SILVER,
	JT_4_SKULL_MUD,
	JT_4_M_BRZ_INDIAN,
	JT_4_F_BRZ_INDIAN,
	JT_4_F_BRZ_INDOLD,
	JT_4_M_BRZ_JACI,
	JT_4_M_BRZ_MAN1,
	JT_4_M_BRZ_MAN2,
	JT_4_F_BRZ_WOMAN,
	JT_4_M_MINSTREL,
	JT_4_M_MINSTREL1,
	JT_4_M_SHADOWCHASER,
	JT_4_F_SHADOWCHASER,
	JT_4_M_SURA,
	JT_4_F_SURA,
	JT_4_F_WANDERER,
	JT_4_M_BARD,
	JT_1_FLAG_NOFEAR,
	JT_4_M_NOFEARGUY,
	JT_4_MAN_PIOM6,
	JT_4_MAN_PIOM4,
	JT_4_MAN_PIOM5,
	JT_4_MAN_GALTUN1,
	JT_4_HUMAN_GERUTOO,
	JT_4_M_ROKI,
	JT_4_M_MERCAT1,
	JT_4_M_MERCAT2,
	JT_4_M_CATMAN1,
	JT_4_M_CATMAN2,
	JT_4_F_BRZ_WOMAN2,
	JT_4_M_JP_DISH,
	JT_4_F_JP_NOAH,
	JT_4_F_JP_OZ,
	JT_4_F_JP_CHROME,
	JT_4_F_JP_RINNE,
	JT_4_WHITETIGER,
	JT_4_VENDING_MACHINE,
	JT_4_MISTY,
	JT_4_NECORING,
	JT_4_ELEPHANT,
	JT_4_F_NYDHOG,
	JT_4_F_NYDHOG2,
	JT_4_M_ROKI2,
	JT_4_M_DOGTRAVELER,
	JT_4_M_DOGTRAVELER2,
	JT_4_F_DOGTRAVELER,
	JT_4_M_RAFLE_GR,
	JT_4_M_RAFLE_OLD,
	JT_4_F_RAFLE_PK,
	JT_4_M_LYINGDOG,
	JT_4_F_MORAFINE1,
	JT_4_F_MORAFINE2,
	JT_4_M_RAFLE_OR,
	JT_4_F_RAFLE_YE,
	JT_4_M_RAFLE_VI,
	JT_4_F_RAFLE_VI,
	JT_4_M_ARDHA,
	JT_4_CREEPER,
	JT_JP_RUFAKU,
	JT_JP_SUPIKA,
	JT_JP_SABIKU,
	JT_JP_ARUGORU,
	JT_JP_ARUNA,
	JT_JP_AIRI,
	JT_4_M_DEWOLDMAN,
	JT_4_M_DEWOLDWOMAN,
	JT_4_M_DEWMAN,
	JT_4_M_DEWWOMAN,
	JT_4_M_DEWBOY,
	JT_4_M_DEWGIRL,
	JT_4_M_DEWZATICHIEF,
	JT_4_M_DEWZATIMAN,
	JT_4_M_ALCHE_E,
	JT_4_MASK_SMOKEY,
	JT_4_CAT_SAILOR1,
	JT_4_CAT_SAILOR2,
	JT_4_CAT_SAILOR3,
	JT_4_CAT_SAILOR4,
	JT_4_CAT_CHEF,
	JT_4_CAT_MERMASTER,
	JT_4_CRACK,
	JT_4_ASTER,
	JT_4_F_STARFISHGIRL,
	JT_4_CAT_DOWN,
	JT_4_CAT_REST,
	JT_4_CAT_3COLOR,
	JT_4_CAT_ADMIRAL,
	JT_4_SOIL,
	JT_4_F_ALCHE_A,
	JT_4_CAT_ADV1,
	JT_4_CAT_ADV2,
	JT_4_CAT_SAILOR5,
	JT_2_DROP_MACHINE,
	JT_2_SLOT_MACHINE,
	JT_2_VENDING_MACHINE1,
	JT_MOB_TOMB,
	JT_4_MYSTCASE,
	JT_4_M_SIT_NOVICE,
	JT_4_OCTOPUS_LEG,
	JT_4_F_NURSE,
	JT_4_MAL_SOLDIER,
	JT_4_MAL_CAPTAIN,
	JT_4_MAL_BUDIDAI,
	JT_4_M_MAYOR,
	JT_4_M_BARYO_OLD,
	JT_4_F_BARYO_OLD,
	JT_4_F_BARYO_GIRL,
	JT_4_M_BARYO_BOY,
	JT_4_M_BARYO_MAN,
	JT_4_F_BARYO_WOMAN,
	JT_4_BARYO_CHIEF,
	JT_4_MAL_KAFRA,
	JT_4_M_MALAYA,
	JT_4_F_MALAYA,
	JT_4_F_PATIENT,
	JT_4_M_PATIENT,
	JT_4_F_KR_TIGER,
	JT_4_M_KR_BOY,
	JT_4_M_KAGE_OLD,
	JT_4_WHIKEBAIN,
	JT_4_EREND,
	JT_4_RAWREL,
	JT_4_ARMAIA,
	JT_4_KAVAC,
	JT_4_YGNIZEM,
	JT_4_EREMES,
	JT_4_MAGALETA,
	JT_4_KATRINN,
	JT_4_SHECIL,
	JT_4_SEYREN,
	JT_4_HARWORD,
	JT_4_F_JP_CYNTHIA,
	JT_4_M_JP_GUSTON,
	JT_4_M_JP_BERKUT,
	JT_4_F_JP_DARK_ADELAIDE,
	JT_4_M_JP_DARK_DARIUS,
	JT_4_M_JP_JESTER,
	JT_XMAS_SMOKEY_B,
	JT_XMAS_SMOKEY_R,
	JT_XMAS_SMOKEY_Y,
	JT_4_F_CLOCKDOLL,
	JT_4_F_FAIRY2,
	JT_4_F_PINKWOMAN,
	JT_4_FAIRYDEADLEAF,
	JT_4_FROG,
	JT_4_M_BLACKMAN,
	JT_4_M_BLUEMAN,
	JT_4_M_FAIRYANG,
	JT_4_M_FAIRYAVANT,
	JT_4_M_FAIRYFREAK,
	JT_4_M_FAIRYKID6,
	JT_4_M_FAIRYSCHOLAR,
	JT_4_M_FAIRYSCHOLAR_DIRTY,
	JT_4_M_FARIY_HISIE,
	JT_4_M_FARIYKING,
	JT_4_M_NEWOZ,
	JT_4_M_OLIVER,
	JT_4_M_PROFESSORWORM,
	JT_4_M_REDMAN,
	JT_4_F_GELKA,
	JT_4_M_ROTERT,
	JT_4_BLACKDRAGON,
	JT_4_M_GUNSLINGER,
	JT_4_F_GUNSLINGER,
	JT_4_M_ARCHER,
	JT_4_M_SWORDMAN,
	JT_4_M_NINJA_RED,
	JT_4_M_NINJA_BLUE,
	JT_4_M_THIEF_RUMIN,
	JT_4_M_NOV_RUMIN,
	JT_4_F_MAYSEL,
	JT_4_F_ACOLYTE,
	JT_4_M_NOV_HUNT,
	JT_4_F_GENETIC,
	JT_4_F_TAEKWON,
	JT_4_F_SWORDMAN,
	JT_4_F_IU,
	JT_4_M_RAGI,
	JT_4_M_MELODY,
	JT_4_TRACE,
	JT_4_F_HIMEL,
	JT_4_LEVITATEMAN,
	JT_4_M_HEINRICH,
	JT_4_M_ROYALGUARD,
	JT_4_M_BARMUND,
	JT_4_F_KHALITZBURG,
	JT_4_F_HIMEL2,
	JT_4_WHITEKNIGHT,
	JT_4_COCO,
	JT_4_M_ALADDIN,
	JT_4_M_GENIE,
	JT_4_F_GENIE,
	JT_4_JP_MID_SWIM,
	JT_4_JP_RUNE_SWIM,
	JT_4_F_FENRIR,
	JT_4_F_GEFFEN_FAY,
	JT_4_F_IRIS,
	JT_4_F_LUCILE,
	JT_4_F_SARAH_BABY,
	JT_4_GEFFEN_01,
	JT_4_GEFFEN_02,
	JT_4_GEFFEN_03,
	JT_4_GEFFEN_04,
	JT_4_GEFFEN_05,
	JT_4_GEFFEN_06,
	JT_4_GEFFEN_07,
	JT_4_GEFFEN_08,
	JT_4_GEFFEN_09,
	JT_4_GEFFEN_10,
	JT_4_GEFFEN_11,
	JT_4_GEFFEN_12,
	JT_4_GEFFEN_13,
	JT_4_GEFFEN_14,
	JT_4_M_CHAOS,
	JT_4_M_CHIEF_IRIN,
	JT_4_M_SAKRAY,
	JT_4_M_SAKRAYROYAL,
	JT_4_TOWER_01,
	JT_4_TOWER_02,
	JT_4_TOWER_03,
	JT_4_TOWER_04,
	JT_4_TOWER_05,
	JT_4_TOWER_06,
	JT_4_TOWER_07,
	JT_4_TOWER_08,
	JT_4_TOWER_09,
	JT_4_TOWER_10,
	JT_4_TOWER_11,
	JT_4_TOWER_12,
	JT_4_TOWER_13,
	JT_8_F_GIRL,
	JT_4_F_GODEMOM,
	JT_4_F_GON,
	JT_4_F_KID2,
	JT_4_M_BIBI,
	JT_4_M_GEF_SOLDIER,
	JT_4_M_KID1,
	JT_4_M_MOC_SOLDIER,
	JT_4_M_PAY_SOLDIER,
	JT_4_M_SEAMAN,
	JT_4_M_SNOWMAN,
	JT_4_F_05,
	JT_4_M_05,
	JT_4_M_06,
	JT_4_F_06,
	JT_4_M_PIERROT,
	JT_4_M_KID2,
	JT_4_F_KID3,
	JT_4_M_SANTA,
	JT_4_F_NACORURI,
	JT_4_F_SHAMAN,
	JT_4_F_KAFRA7,
	JT_GUILD_FLAG,
	JT_1_SHADOW_NPC,
	JT_4_F_07,
	JT_4_F_JOB_ASSASSIN,
	JT_4_F_JOB_BLACKSMITH,
	JT_4_F_JOB_HUNTER,
	JT_4_F_JOB_KNIGHT,
	JT_4_F_NOVICE,
	JT_4_M_JOB_ASSASSIN,
	JT_4_M_JOB_BLACKSMITH,
	JT_4_M_JOB_HUNTER,
	JT_4_M_JOB_KNIGHT1,
	JT_4_M_JOB_KNIGHT2,
	JT_4_M_JOB_WIZARD,
	JT_4_BAPHOMET,
	JT_4_DARKLORD,
	JT_4_DEVIRUCHI,
	JT_8_DOPPEL,
	JT_2_M_ALCHE,
	JT_2_M_BARD_ORIENT,
	JT_2_M_SAGE_B,
	JT_2_M_SAGE_OLD,
	JT_4_F_ALCHE,
	JT_4_F_CRU,
	JT_4_F_MONK,
	JT_4_F_ROGUE,
	JT_4_M_ALCHE_A,
	JT_4_M_ALCHE_B,
	JT_4_M_ALCHE_C,
	JT_4_M_CRU,
	JT_4_M_CRU_OLD,
	JT_4_M_MONK,
	JT_4_M_SAGE_A,
	JT_4_M_SAGE_C,
	JT_4_F_SON,
	JT_4_F_JPN2,
	JT_4_F_JPN,
	JT_4_F_JPNCHIBI,
	JT_4_F_JPNOBA2,
	JT_4_F_JPNOBA,
	JT_4_M_JPN2,
	JT_4_M_JPN,
	JT_4_M_JPNCHIBI,
	JT_4_M_JPNOJI2,
	JT_4_M_JPNOJI,
	JT_8_M_JPNSOLDIER,
	JT_8_M_JPNMASTER,
	JT_4_F_JPNMU,
	JT_4_F_TWGIRL,
	JT_4_F_TWGRANDMOM,
	JT_4_F_TWMASKGIRL,
	JT_4_F_TWMIDWOMAN,
	JT_4_M_TWBOY,
	JT_4_M_TWMASKMAN,
	JT_4_M_TWMIDMAN,
	JT_4_M_TWOLDMAN,
	JT_4_M_TWTEAMAN,
	JT_4_M_YOYOROGUE,
	JT_8_M_TWSOLDIER,
	JT_4_F_UMGIRL,
	JT_4_F_UMOLDWOMAN,
	JT_4_F_UMWOMAN,
	JT_4_M_UMCHIEF,
	JT_4_M_UMDANCEKID2,
	JT_4_M_UMDANCEKID,
	JT_4_M_UMKID,
	JT_4_M_UMOLDMAN,
	JT_4_M_UMSOLDIER,
	JT_4_M_SALVATION,
	JT_4_F_NFDEADKAFRA,
	JT_4_F_NFDEADMGCIAN,
	JT_4_F_NFLOSTGIRL,
	JT_4_M_NFDEADMAN2,
	JT_4_M_NFDEADMAN,
	JT_4_M_NFDEADSWDMAN,
	JT_4_M_NFLOSTMAN,
	JT_4_M_NFMAN,
	JT_4_NFBAT,
	JT_4_NFCOCK,
	JT_4_NFCOFFIN,
	JT_4_NFWISP,
	JT_1_F_SIGNZISK,
	JT_1_M_SIGN1,
	JT_1_M_SIGNALCHE,
	JT_1_M_SIGNART,
	JT_1_M_SIGNMCNT,
	JT_1_M_SIGNMONK2,
	JT_1_M_SIGNMONK,
	JT_1_M_SIGNROGUE,
	JT_4_F_VALKYRIE,
	JT_TW_TOWER,
	JT_2_M_OLDBLSMITH,
	JT_4_F_CHNDOCTOR,
	JT_4_F_CHNDRESS1,
	JT_4_F_CHNDRESS2,
	JT_4_F_CHNDRESS3,
	JT_4_F_CHNWOMAN,
	JT_4_M_CHN8GUEK,
	JT_4_M_CHNCOOK,
	JT_4_M_CHNGENERL,
	JT_4_M_CHNMAN,
	JT_4_M_CHNMONK,
	JT_4_M_CHNOLD,
	JT_4_M_CHNSOLDIER,
	JT_4_M_DWARF,
	JT_4_M_GRANDMONK,
	JT_4_M_ROGUE,
	JT_4_M_DOMINO,
	JT_4_F_DOMINO,
	JT_4_F_ZONDAGIRL,
	JT_4_M_REIDIN_KURS,
	JT_4_M_ZONDAOYAJI,
	JT_4_M_BUDDHIST,
	JT_2_BOARD1,
	JT_2_BOARD2,
	JT_2_BULLETIN_BOARD,
	JT_4_F_THAIAYO,
	JT_4_F_THAIGIRL,
	JT_4_F_THAISHAMAN,
	JT_4_M_THAIAYO,
	JT_4_M_THAIOLD,
	JT_4_M_THAIONGBAK,
	JT_CLEAR_NPC,
	JT_4_F_RACING,
	JT_4_F_EINOLD,
	JT_4_M_EINOLD,
	JT_4_M_EINMINER,
	JT_4_M_DIEMAN,
	JT_4_F_EINWOMAN,
	JT_4_M_REPAIR,
	JT_4_M_EIN_SOLDIER,
	JT_4_M_YURI,
	JT_4_M_EINMAN2,
	JT_4_M_EINMAN,
	JT_2_F_SIGN1,
	JT_4_BOARD3,
	JT_4_BULLETIN_BOARD2,
	JT_4_F_AGENTKAFRA,
	JT_4_F_KAFRA8,
	JT_4_F_KAFRA9,
	JT_4_F_LGTGIRL,
	JT_4_F_LGTGRAND,
	JT_4_F_OPERATION,
	JT_4_LGTSCIENCE,
	JT_4_M_LGTGRAND,
	JT_4_M_LGTGUARD2,
	JT_4_M_LGTGUARD,
	JT_4_M_LGTMAN,
	JT_4_M_LGTPOOR,
	JT_4_M_OPERATION,
	JT_4_M_PRESIDENT,
	JT_4_M_REINDEER,
	JT_4_M_ZONDAMAN,
	JT_4_M_PECOKNIGHT,
	JT_4_CAT,
	JT_4_F_YUNYANG,
	JT_4_M_OILMAN,
	JT_4_F_CAPEGIRL,
	JT_4_M_MASKMAN,
	JT_4_M_SITDOWN,
	JT_4_F_SITDOWN,
	JT_4_M_ALCHE_D,
	JT_4_M_ACROSS,
	JT_4_F_ACROSS,
	JT_4_COOK,
	JT_4_M_LIEMAN,
	JT_2_POSTBOX,
	JT_4_BULL,
	JT_4_LAM,
	JT_4_F_HUGIRL,
	JT_4_F_HUGRANMA,
	JT_4_F_HUWOMAN,
	JT_4_F_KHELLISIA,
	JT_4_F_KHELLY,
	JT_4_M_HUBOY,
	JT_4_M_HUGRANFA,
	JT_4_M_HUMAN_01,
	JT_4_M_HUMAN_02,
	JT_4_M_HUMERCHANT,
	JT_4_M_HUOLDARMY,
	JT_4_M_KHKIEL,
	JT_4_M_KHKYEL,
	JT_4_M_KHMAN,
	JT_4_F_KHWOMAN,
	JT_4_F_KHGIRL,
	JT_4_M_KHBOY,
	JT_4_M_PHILMAN,
	JT_4_PORING,
	JT_2_COLAVEND,
	JT_4_F_SOCCER,
	JT_4_M_SOCCER7,
	JT_4_M_SOCCER9,
	JT_4_F_CHILD,
	JT_4_F_MADAME,
	JT_4_F_MASK1,
	JT_4_F_MASK,
	JT_4_F_RACHOLD,
	JT_4_F_SHABBY,
	JT_4_F_TRAINEE,
	JT_4_M_CHILD1,
	JT_4_M_CHILD,
	JT_4_M_DOCTOR,
	JT_4_M_FROZEN1,
	JT_4_M_FROZEN,
	JT_4_M_MASK1,
	JT_4_M_MASK,
	JT_4_M_MIDDLE1,
	JT_4_M_MIDDLE,
	JT_4_M_RACHMAN2,
	JT_4_M_RACHMAN1,
	JT_4_M_RACHOLD1,
	JT_4_M_RACHOLD,
	JT_4_M_RASWORD,
	JT_4_M_TRAINEE,
	JT_4_F_ARUNA_POP,
	JT_4_M_ARUNA_NFM1,
	JT_4_DST_CAMEL,
	JT_4_DST_SOLDIER,
	JT_4_F_DESERT,
	JT_4_F_DST_CHILD,
	JT_4_F_DST_GRAND,
	JT_4_M_DESERT,
	JT_4_M_DST_CHILD,
	JT_4_M_DST_GRAND,
	JT_4_M_DST_MASTER,
	JT_4_M_DST_TOUGH,
	JT_4_ANGELING,
	JT_4_ARCHANGELING,
	JT_4_GHOSTRING,
	JT_4_F_EDEN_MASTER,
	JT_4_F_EDEN_OFFICER,
	JT_4_M_EDEN_GUARDER,
	JT_4_M_PATRICK,
	JT_4_DONKEY,
	JT_4_M_TRISTAN,
	JT_4_WHITE_COW,
	JT_4_F_RUSCHILD,
	JT_4_F_RUSWOMAN1,
	JT_4_F_RUSWOMAN2,
	JT_4_F_RUSWOMAN3,
	JT_4_M_RUSCHILD,
	JT_4_M_GUSLIMAN,
	JT_4_M_RUSBALD,
	JT_4_M_RUSKING,
	JT_4_M_RUSKNIGHT,
	JT_4_M_RUSMAN1,
	JT_4_M_RUSMAN2,
	JT_4_M_DRAKE,
	JT_4_F_BABAYAGA,
	JT_4_F_RUSGREEN,
	JT_4_RUS_DWOLF,
	JT_1_FLAG_LION,
	JT_1_FLAG_EAGLE,
	JT_4_M_MIKID,
	JT_4_BLUE_FLOWER,
	JT_4_RED_FLOWER,
	JT_4_YELL_FLOWER,
	JT_4_F_CAVE1,
	JT_4_F_MUT1,
	JT_4_F_MUT2,
	JT_4_F_SCIENCE,
	JT_4_M_1STPRIN1,
	JT_4_M_1STPRIN2,
	JT_4_M_2NDPRIN1,
	JT_4_M_2NDPRIN2,
	JT_4_M_3RDPRIN1,
	JT_4_M_3RDPRIN2,
	JT_4_M_4THPRIN1,
	JT_4_M_4THPRIN2,
	JT_4_M_5THPRIN1,
	JT_4_M_5THPRIN2,
	JT_4_M_6THPRIN1,
	JT_4_M_6THPRIN2,
	JT_4_M_CASMAN1,
	JT_4_M_CAVE1,
	JT_4_M_MOCASS1,
	JT_4_M_MOCASS2,
	JT_4_M_MUT1,
	NPC_RANGE2_END, // Official JT_MON_BEGIN

	NPC_RANGE3_START = 10000, // Official JT_NEW_NPC_3RD_BEGIN
	JT_4_TOWER_14,
	JT_4_TOWER_15,
	JT_4_TOWER_16,
	JT_4_TOWER_17,
	JT_4_TREASURE_BOX,
	JT_ACADEMY_MASTER,
	JT_PORTAL,
	JT_THANATOS_BATTLE,
	JT_THANATOS_KEEP,
	JT_4_F_LYDIA,
	JT_4_LUDE,
	JT_4_ALIZA,
	JT_4_ALICE,
	JT_4_ARCHER_SKEL,
	JT_4_JACK,
	JT_4_SOLDIER_SKEL,
	JT_4_LOLI_RURI,
	JT_4_M_SAKRAY_TIED,
	JT_4_M_ANTONIO,
	JT_4_M_COOKIE,
	JT_4_M_BELIEVER01,
	JT_4_F_BELIEVER01,
	JT_4_M_BELIEVER02,
	JT_4_ROPEPILE,
	JT_4_BRICKPILE,
	JT_4_WOODPILE,
	JT_4_M_TAMARIN,
	JT_4_M_DEATH,
	JT_4_GHOST_STAND,
	JT_4_GHOST_COLLAPSE,
	JT_4_COOKIEHOUSE,
	JT_4_F_SKULL06GIRL,
	JT_4_NONMYSTCASE,
	JT_4_F_KIMI,
	JT_4_M_FROZEN_GC,
	JT_4_M_FROZEN_KN,
	JT_4_SNAKE_LORD,
	JT_4_F_MOCBOY,
	JT_4_F_RUNAIN,
	JT_4_M_ROEL,
	JT_4_F_SHALOSH,
	JT_4_ENERGY_RED,
	JT_4_ENERGY_BLUE,
	JT_4_ENERGY_YELLOW,
	JT_4_ENERGY_BLACK,
	JT_4_ENERGY_WHITE,
	JT_4_F_PERE01,
	JT_4_JITTERBUG,
	JT_4_SEA_OTTER,
	JT_4_GALAPAGO,
	JT_4_DESERTWOLF_B,
	JT_4_BB_PORING,
	JT_4_F_CHARLESTON01,
	JT_4_F_CHARLESTON02,
	JT_4_F_CHARLESTON03,
	JT_4_M_IAN,
	JT_4_M_OLDSCHOLAR,
	JT_4_F_LAPERM,
	JT_4_M_DEBON,
	JT_4_M_BIRMAN,
	JT_4_F_SHAM,
	JT_4_M_REBELLION,
	JT_4_F_REBELLION,
	JT_4_CHN_SHAOTH,
	JT_4_SHOAL,
	JT_4_F_SARAH,
	JT_4_GIGANTES_BIG,
	JT_4_GIGANTES,
	JT_4_GIGANTES_SMALL,
	JT_4_GARGOYLE_STATUE,
	JT_4_AIRA,
	JT_4_EZELLA,
	JT_4_KULUNA,
	JT_4_LUNE,
	JT_4_MALLINA,
	JT_4_MORIN,
	JT_4_NASARIN,
	JT_4_F_BERRYTEA,
	JT_4_F_FRUIT,
	JT_4_SCR_MT_ROBOTS,
	JT_4_MACHINE_DEVICE,
	JT_4_GC109,
	JT_4_SYS_MSG,
	JT_4_M_TATIO,
	JT_4_M_REKENBER,
	JT_4_XMAS_CAT1,
	JT_4_XMAS_CAT2,
	JT_4_XMAS_CAT3,
	JT_4_XMAS_CAT4,
	JT_4_XMAS_CAT5,
	JT_4_XMAS_CAT6,
	JT_4_M_DEATH2,
	JT_4_S_KADOMATSU,
	JT_4_B_KADOMATSU,
	JT_4_F_08,
	JT_4_F_08_STATUE,
	JT_4_M_DARKPRIEST,
	JT_4_JP_GARM_H,
	JT_4_JP_MEDUSA_H,
	JT_4_CHN_GVG_01,
	JT_4_SPRING_RABBIT,
	JT_4_PD_TYRA,
	JT_4_PD_TYRANOS,
	JT_4_PD_PLESI,
	JT_4_PD_PLESIO,
	JT_4_PD_BRACHI,
	JT_4_PD_BRACHIOS,
	JT_4_PD_GOLDDRAGON,
	JT_4_PD_ZAEROG,
	JT_4_PD_TAMADORA,
	JT_4_JP_EDGA_H,
	JT_4_JP_BRAGOLEM_H,
	JT_4_EL_AQUA,
	JT_4_EP16_NIHIL,
	JT_4_EP16_SPICA,
	JT_4_EP16_SKIA,
	JT_4_EP16_PETER,
	JT_4_EP16_CRUX,
	JT_4_EP16_GRANZ,
	JT_4_EP16_STOLZ,
	JT_4_EP16_EGEO,
	JT_4_EP16_COOK,
	JT_4_EP16_MARK,
	JT_4_EP16_TAMARIN,
	JT_4_EP16_POE,
	JT_4_EP16_ISAAC,
	JT_4_EP16_HELMUT,
	JT_4_EP16_WOLF,
	JT_4_EP16_MEYER,
	JT_4_EP16_AGNES,
	JT_4_EP16_FOOD,
	JT_4_EP16_LOUVIERE,
	JT_4_EP16_MAX,
	JT_4_EP16_SPIEGEL,
	JT_4_MOONLIGHT,
	JT_4_MISTRESS,
	JT_4_DRACULA,
	JT_4_STORMKNIGHT,
	JT_4_TATTER,
	JT_4_AS_RAGGED_GOLEM,
	JT_4_AS_BLOODY_KNIGHT,
	JT_4_AS_WIND_GHOST,
	JT_4_F_BIJOU,
	JT_4_EP16_COOK2,
	JT_4_SERVICE_30_M_01,
	JT_4_SERVICE_30_F_01,
	JT_4_SCR_AT_ROBOTS,
	JT_4_F_RANGER,
	JT_4_WAG,
	JT_4_NPC_TRAP,
	JT_4_RAGGLER,
	JT_4_DR_PEPE,
	JT_4_DR_GAMBERI,
	JT_4_DR_AGLIO,
	JT_4_DR_OLIO,
	JT_4_DR_STELO,
	JT_4_DR_TORTEL,
	JT_4_BASIL_SLAVE,
	JT_4_DOU_JINDO,
	JT_4_DOU_SIBA,
	JT_4_TARUTUPI,
	JT_4_DR_SOLDIER,
	JT_4_DR_M_01,
	JT_4_DR_M_02,
	JT_4_DR_F_01,
	JT_4_DR_F_02,
	JT_4_DR_KID_01,
	JT_4_JP_2015EVT,
	JT_4_ALLIGATOR,
	JT_4_ANOLIAN,
	JT_4_TACNU,
	JT_4_CENERE,
	JT_4_F_ARUNA_POP2,
	JT_4_JACK_HEAD,
	JT_4_INJUSTICE,
	JT_4_BLOODYMAN,
	JT_4_GIBBET,
	JT_4_DULLAHAN,
	JT_4_M_LAZY,
	JT_4_M_GONY,
	JT_4_M_ROOKIE,
	JT_4_M_PHILOFONTES,
	JT_4_F_ESTLOVELOY,
	JT_4_F_LEEDSH,
	JT_4_F_DIENE,
	JT_4_F_COATNEIS,
	JT_4_M_RUPERT,
	JT_4_M_FALLENGONY,
	JT_4_M_EISEN,
	JT_4_F_DEADEVIL,
	JT_4_F_HUNTER_EVIL,
	JT_4_F_ELENA,
	JT_4_F_ANYA,
	JT_4_M_SEIREN_UC,
	JT_4_M_GUNSLINGER2,
	JT_4_M_GUNSLINGER3,
	JT_4_M_REBELLION2,
	JT_4_M_REBELLION3,
	JT_4_F_GUNSLINGER2,
	JT_4_F_GUNSLINGER3,
	JT_4_F_REBELLION2,
	JT_4_F_REBELLION3,
	JT_4_M_ILYA,

	JT_4_ELDER = 10205,
	JT_4_LUNATIC,
	JT_4_F_NOVICE2,
	JT_4_WICKEDNYMPH,
	JT_4_F_PREMI,
	JT_4_M_COSTELL,
	JT_4_M_YATTWARP,
	JT_4_M_EVOKASCUDI,
	JT_4_M_JOHNNYJAMES,
	JT_4_M_ALBERTFORD,
	JT_4_M_SEANMCCURDY,
	JT_4_M_KARAMPUCCI,
	JT_4_M_CACTUSMAN1,
	JT_4_M_CACTUSMAN2,
	JT_4_M_CACTUSMAN3,
	JT_4_M_CACTUSLADY,
	JT_4_M_GAST,
	JT_4_M_CACTUSCHILD,
	JT_4_KING,
	JT_4_F_BOMI,
	JT_4_M_CACTUSCHIEF,
	JT_4_F_CACTUSCHILD2,
	JT_4_F_CACTUSLADY2,
	JT_4_F_CACTUSLADY3,
	JT_4_M_CACTUS,
	JT_4_M_COWRAIDERS1,
	JT_4_M_COWRAIDERS2,
	JT_4_M_COWRAIDERS3,
	JT_4_F_JP14THEVT,
	JT_4_M_POORSCHOLAR,
	JT_4_M_PEPPERROTI,
	JT_JP_NPC01,
	JT_4_PURPLE_WARP,
	JT_4_F_NARIN,
	JT_4_M_URGENT_MAN,
	JT_4_M_KEEN_SOLDIER,
	JT_4_F_SLOPPY_WOMAN,
	JT_4_F_DRKAFRA01,
	JT_4_M_DRZONDA01,
	JT_4_M_SWD_RENO,
	JT_4_M_KNG_RENO,
	JT_4_M_AC_RUMIN,
	JT_4_M_HIGH_WIZARD,
	JT_4_SYSTEM_BOX,
	JT_4_STEELBOX,
	JT_4_WOODBOX,
	JT_4_M_POPFESTA,
	JT_4_BONFIRE,
	JT_4_PCCOUNT,
	JT_4_LAVAGOLEM,
	JT_4_F_IA,
	JT_4_LOCKE,
	JT_4_LIL_LIF,
	JT_4_LIL_LIF2,
	JT_4_F_DRAGONHAT,
	JT_4_M_DRAGONHAT,
	JT_JP_NPC02,
	JT_JP_NPC03,
	JT_JP_NPC04,
	JT_JP_NPC05,
	JT_JP_NPC06,
	JT_JP_NPC07,
	JT_JP_NPC08,
	JT_JP_NPC09,
	JT_JP_NPC10,
	JT_4_FIRERING,
	JT_4_SCATLETON,
	JT_4_JP_AB_NPC_001,
	JT_4_JP_AB_NPC_002,
	JT_4_JP_AB_NPC_003,
	JT_4_JP_AB_NPC_004,
	JT_4_JP_AB_NPC_005,
	JT_4_JP_AB_NPC_006,
	JT_4_JP_AB_NPC_007,
	JT_4_JP_AB_NPC_008,
	JT_4_JJAK,
	JT_4_M_SNOWMAN_B,
	JT_4_M_SNOWMAN_R,
	JT_4_M_SNOWMAN_G,
	JT_WARPEFFECTNPC,
	JT_4_HEN,
	JT_4_F_DANGDANG,
	JT_4_M_DANGDANG,
	JT_4_F_DANGDANG1,
	JT_4_LEAFCAT,
	JT_4_NASARIAN,
	JT_4_NASARIAN_EM,
	JT_4_TEDDY_BEAR_W,
	JT_4_TEDDY_BEAR_B,
	JT_4_TEDDY_BEAR_B_L,
	JT_4_M_SOULREAPER,
	JT_4_F_SE_SUN,
	JT_4_M_SE_MOON,
	JT_4_M_SE_STAR,
	JT_4_EP17_KAYA,
	JT_4_EP17_AS,
	JT_4_EP17_ELYUMINA,
	JT_4_EP17_MORNING,
	JT_4_EP17_MIGUEL,
	JT_4_EP17_NIHIL_K,
	JT_4_EP17_MIGUEL_D,
	JT_4_ED_SCHMIDT,
	JT_4_ED_OSCAR,
	JT_4_ED_ORB,
	JT_4_ED_FENCE,
	JT_4_M_ANDREA,
	JT_4_M_ANDREA_D,
	JT_4_F_ANES,
	JT_4_F_ANES_D,
	JT_4_M_SILVANO,
	JT_4_M_SILVANO_D,
	JT_4_F_CECILIA,
	JT_4_F_CECILIA_D,
	JT_4_M_MD_SEYREN,
	JT_4_M_MD_EREMES,
	JT_4_M_MD_HARWORD,
	JT_4_F_MD_MAGALETA,
	JT_4_F_MD_SHECIL,
	JT_4_F_MD_KATRINN,
	JT_4_M_MD_SEYREN_H,
	JT_4_M_MD_EREMES_H,
	JT_4_M_MD_HARWORD_H,
	JT_4_F_MD_MAGALETA_H,
	JT_4_F_MD_SHECIL_H,
	JT_4_F_MD_KATRINN_H,
	JT_4_M_MD_SEYREN_D,
	JT_4_M_MD_EREMES_D,
	JT_4_M_MD_HARWORD_D,
	JT_4_F_MD_MAGALETA_D,
	JT_4_F_MD_SHECIL_D,
	JT_4_F_MD_KATRINN_D,
	JT_4_F_MD_YGNIZEM,
	JT_4_F_ERENE,
	JT_4_M_EINCPTMINER,
	JT_4_F_EINRESERCHER,
	JT_4_F_REINDEER,
	JT_4_PIGOCTO,
	JT_4_ORK_HERO,
	JT_4_JP_16TH,
	JT_4_EP17_MASTER_A,
	JT_4_EP17_BASIC_B,
	JT_4_EP17_GUARD_B,
	JT_4_EP17_BASIC_B_NG,
	JT_4_EP17_GUARD_B_NG,
	JT_4_EP17_SWEETY,
	JT_4_EP17_BOY_A,
	JT_4_EP17_BOY_B,
	JT_4_EP17_TAMARIN,
	JT_4_EP17_SCISSORE,
	JT_4_EP17_TABLET,
	JT_4_EP17_BUCKETS,
	JT_4_EP17_CLEANER,
	JT_4_EP17_BASKET,
	JT_4_EP17_BROKENBETA,
	JT_4_EP17_CLEANER_W,
	JT_4_EP17_MERMAID,
	JT_4_JP_AB_NPC_009,
	JT_4_JP_AB_NPC_010,

	JT_4_4JOB_SILLA = 10364,
	JT_4_4JOB_MAGGI,
	JT_4_4JOB_ROBIN,
	JT_4_4JOB_ROBIN_DRUNK,
	JT_4_4JOB_LETICIA,
	JT_4_4JOB_SERANG,
	JT_4_4JOB_EINHAR,
	JT_4_4JOB_SEALSTONE,
	JT_4_4JOB_PHANTOMBOOK1,
	JT_4_4JOB_PHANTOMBOOK2,
	JT_4_4JOB_PHANTOMBOOK3,
	JT_4_VENDING_MACHINE2,
	JT_4_EP18_MARAM,
	JT_4_EP18_MIRIAM,
	JT_4_EP18_SUAD,
	JT_4_EP18_IMRIL,
	JT_4_EP18_MERCHANT,
	JT_4_EP18_TAMARIN,
	JT_4_EP18_DEW,
	JT_4_EP18_MARK,
	JT_4_EP18_ALF,
	JT_4_EP18_SHULANG,
	JT_4_EP18_BAGOT,
	JT_4_EP18_DEMIFREYA,
	JT_4_EP18_KAMIL,
	JT_4_EP18_HAZAR,
	JT_4_EP18_WAGON,
	JT_4_EP18_PAPERS,
	JT_4_EP18_HALFFLOWER,
	JT_4_EP18_GW_OLD01,
	JT_4_EP18_GW_OLD02,
	JT_4_EP18_GW_MIDDLE01,
	JT_4_EP18_GW_MIDDLE02,
	JT_4_EP18_GW_MAN01,
	JT_4_EP18_GW_MAN02,
	JT_4_EP18_GW_WOMAN01,
	JT_4_EP18_GW_WOMAN02,
	JT_4_EP18_GW_CHILD01,
	JT_4_EP18_GW_CHILD02,
	JT_4_STAR_BOX_SCORE,
	JT_4_STAR_BOX_POW1,
	JT_4_STAR_BOX_POW2,
	JT_4_STAR_BOX_STA1,
	JT_4_STAR_BOX_STA2,
	JT_4_STAR_BOX_SPL1,
	JT_4_STAR_BOX_SPL2,
	JT_4_STAR_BOX_CON1,
	JT_4_STAR_BOX_CON2,
	JT_4_STAR_BOX_WIS1,
	JT_4_STAR_BOX_WIS2,
	JT_4_STAR_BOX_CRT1,
	JT_4_STAR_BOX_CRT2,
	JT_4_4JOB_MAURA,
	JT_4_STAR_BOX_N,
	JT_4_STAR_BOX_H,
	JT_4_STAR_BOX_HP1,
	JT_4_STAR_BOX_HP2,
	JT_4_STAR_BOX_ATK1,
	JT_4_STAR_BOX_ATK2,
	JT_4_STAR_BOX_BARRIER1,
	JT_4_STAR_BOX_BARRIER2,
	JT_4_STAR_BOX_TRAP1,
	JT_4_STAR_BOX_TRAP2,
	JT_4_STAR_BOX_MASTER,
	JT_4_POINT_RED,
	JT_4_POINT_BLUE,
	JT_4_POINT_YELLOW,
	JT_4_POINT_BLACK,
	JT_4_POINT_WHITE,
	JT_4_EXJOB_GERHOLD,
	JT_4_EXJOB_NINJA,
	JT_4_EXJOB_MASTER_J,
	JT_4_EXJOB_MR_SEO,
	JT_4_EXJOB_HAPPY_CLOUD,
	JT_4_EXJOB_STAR,
	JT_4_EXJOB_CHUL_HO,
	JT_4_EXJOB_KI_SUL,
	JT_4_EXJOB_HYUN_ROK,
	JT_1_JOURNEY_STONE_D,
	JT_1_JOURNEY_STONE_F,
	JT_ROZ_MQ_SIGRUN,
	JT_ROZ_MQ_SIGRUN_S,
	JT_ROZ_MQ_HARACE01,
	JT_ROZ_MQ_HARACE02,
	JT_ROZ_MQ_SAHARIO,
	JT_ROZ_MQ_SUPIGEL,
	JT_ROZ_MQ_DEADSOLDIER,

	JT_1_RAGFES_01 = 10476,
	JT_1_RAGFES_01_M,
	JT_4_RAGFES_02,
	JT_4_RAGFES_02_M,
	JT_4_RAGFES_03,
	JT_4_RAGFES_03_M,
	JT_4_RAGFES_04,
	JT_4_RAGFES_04_M,
	JT_4_RAGFES_05,
	JT_4_RAGFES_05_M,
	JT_4_RAGFES_06,
	JT_4_RAGFES_06_M,
	JT_4_RAGFES_07,
	JT_4_RAGFES_07_M,
	JT_4_RAGFES_08,
	JT_4_RAGFES_08_M,
	JT_4_RAGFES_09,
	JT_4_RAGFES_09_M,
	JT_4_RAGFES_10,
	JT_4_RAGFES_10_M,
	JT_4_RAGFES_11,
	JT_4_RAGFES_11_M,
	JT_4_RAGFES_12,
	JT_4_RAGFES_12_M,
	JT_4_RAGFES_13,
	JT_4_RAGFES_13_M,
	JT_4_RAGFES_14,
	JT_4_RAGFES_14_M,
	JT_4_RAGFES_15,
	JT_4_RAGFES_15_M,
	JT_4_RAGFES_16,
	JT_4_RAGFES_16_M,
	JT_4_EXJOB_NINJA2,

	JT_ROZ_MQ_LUCIAN = 10510,
	JT_ROZ_MQ_BRITIA,
	JT_ROZ_MQ_ASSASIN01,
	JT_STRANGE_B_SMITH1,
	JT_STRONGER_B_SMTIH,

	JT_NEW_NPC_3RD_END = 19999,
	NPC_RANGE3_END, // Official: JT_NEW_NPC_3RD_END=19999

	// Unofficial
	JT_INVISIBLE = 32767,
	JT_FAKENPC = -1
};

// Old NPC range
#define MAX_NPC_CLASS NPC_RANGE2_END
// New NPC range
#define MAX_NPC_CLASS2_START NPC_RANGE3_START
#define MAX_NPC_CLASS2_END NPC_RANGE3_END

//Checks if a given id is a valid npc id. [Skotlex]
//Since new npcs are added all the time, the max valid value is the one before the first mob (Scorpion = 1001)
#define npcdb_checkid(id) ( ( (id) > NPC_RANGE1_START && (id) < NPC_RANGE1_END ) || (id) == JT_HIDDEN_WARP_NPC || ( (id) > NPC_RANGE2_START && (id) < NPC_RANGE2_END ) || (id) == JT_INVISIBLE || ( (id) > NPC_RANGE3_START && (id) < NPC_RANGE3_END ) )

#ifdef PCRE_SUPPORT
void npc_chat_finalize(struct npc_data* nd);
#endif

//Script NPC events.
enum npce_event : uint8 {
	NPCE_LOGIN,
	NPCE_LOGOUT,
	NPCE_LOADMAP,
	NPCE_BASELVUP,
	NPCE_JOBLVUP,
	NPCE_DIE,
	NPCE_KILLPC,
	NPCE_KILLNPC,

	/************************************************************************/
	/* Filter 类型的过滤事件，这些事件可以被 processhalt 中断                    */
	/************************************************************************/

#ifdef Pandas_NpcFilter_IDENTIFY
	NPCF_IDENTIFY,	// identify_filter_name	// OnPCIdentifyFilter		// 当玩家在装备鉴定列表中选择好装备, 并点击“确定”按钮时触发过滤器
#endif // Pandas_NpcFilter_IDENTIFY

#ifdef Pandas_NpcFilter_ENTERCHAT
	NPCF_ENTERCHAT,	// enterchat_filter_name	// OnPCInChatroomFilter		// 当玩家进入 NPC 开启的聊天室时触发过滤器
#endif // Pandas_NpcFilter_ENTERCHAT

#ifdef Pandas_NpcFilter_INSERT_CARD
	NPCF_INSERT_CARD,	// insert_card_filter_name	// OnPCInsertCardFilter		// 当玩家准备插入卡片时触发过滤器
#endif // Pandas_NpcFilter_INSERT_CARD

#ifdef Pandas_NpcFilter_USE_ITEM
	NPCF_USE_ITEM,	// use_item_filter_name	// OnPCUseItemFilter		// 当玩家准备使用非装备类道具时触发过滤器
#endif // Pandas_NpcFilter_USE_ITEM

#ifdef Pandas_NpcFilter_USE_SKILL
	NPCF_USE_SKILL,	// use_skill_filter_name	// OnPCUseSkillFilter		// 当玩家准备使用技能时触发过滤器
#endif // Pandas_NpcFilter_USE_SKILL

#ifdef Pandas_NpcFilter_ROULETTE_OPEN
	NPCF_ROULETTE_OPEN,	// roulette_open_filter_name	// OnPCOpenRouletteFilter		// 当玩家准备打开乐透大转盘的时候触发过滤器
#endif // Pandas_NpcFilter_ROULETTE_OPEN

#ifdef Pandas_NpcFilter_VIEW_EQUIP
	NPCF_VIEW_EQUIP,	// view_equip_filter_name	// OnPCViewEquipFilter		// 当玩家准备查看某个角色的装备时触发过滤器
#endif // Pandas_NpcFilter_VIEW_EQUIP

#ifdef Pandas_NpcFilter_EQUIP
	NPCF_EQUIP,	// equip_filter_name	// OnPCEquipFilter		// 当玩家准备穿戴装备时触发过滤器
#endif // Pandas_NpcFilter_EQUIP

#ifdef Pandas_NpcFilter_UNEQUIP
	NPCF_UNEQUIP,	// unequip_filter_name	// OnPCUnequipFilter		// 当玩家准备脱下装备时触发过滤器
#endif // Pandas_NpcFilter_UNEQUIP

#ifdef Pandas_NpcFilter_CHANGETITLE
	NPCF_CHANGETITLE,	// changetitle_filter_name	// OnPCChangeTitleFilter		// 当玩家试图变更称号时将触发过滤器
#endif // Pandas_NpcFilter_CHANGETITLE

#ifdef Pandas_NpcFilter_SC_START
	NPCF_SC_START,	// sc_start_filter_name	// OnPCBuffStartFilter		// 当玩家准备获得一个状态(Buff)时触发过滤器
#endif // Pandas_NpcFilter_SC_START

#ifdef Pandas_NpcFilter_USE_REVIVE_TOKEN
	NPCF_USE_REVIVE_TOKEN,	// use_revive_token_filter_name	// OnPCUseReviveTokenFilter		// 当玩家使用菜单中的原地复活之证时触发过滤器
#endif // Pandas_NpcFilter_USE_REVIVE_TOKEN

#ifdef Pandas_NpcFilter_ONECLICK_IDENTIFY
	NPCF_ONECLICK_IDENTIFY,	// oneclick_identify_filter_name	// OnPCUseOCIdentifyFilter		// 当玩家使用一键鉴定道具时触发过滤器
#endif // Pandas_NpcFilter_ONECLICK_IDENTIFY

#ifdef Pandas_NpcFilter_GUILDCREATE
	NPCF_GUILDCREATE,	// guildcreate_filter_name	// OnPCGuildCreateFilter		// 当玩家准备创建公会时触发过滤器 [聽風]
#endif // Pandas_NpcFilter_GUILDCREATE

#ifdef Pandas_NpcFilter_GUILDJOIN
	NPCF_GUILDJOIN,	// guildjoin_filter_name	// OnPCGuildJoinFilter		// 当玩家即将加入公会时触发过滤器 [聽風]
#endif // Pandas_NpcFilter_GUILDJOIN

#ifdef Pandas_NpcFilter_GUILDLEAVE
	NPCF_GUILDLEAVE,	// guildleave_filter_name	// OnPCGuildLeaveFilter		// 当玩家准备离开公会时触发过滤器 [聽風]
#endif // Pandas_NpcFilter_GUILDLEAVE

#ifdef Pandas_NpcFilter_PARTYCREATE
	NPCF_PARTYCREATE,	// partycreate_filter_name	// OnPCPartyCreateFilter		// 当玩家准备创建队伍时触发过滤器 [聽風]
#endif // Pandas_NpcFilter_PARTYCREATE

#ifdef Pandas_NpcFilter_PARTYJOIN
	NPCF_PARTYJOIN,	// partyjoin_filter_name	// OnPCPartyJoinFilter		// 当玩家即将加入队伍时触发过滤器 [聽風]
#endif // Pandas_NpcFilter_PARTYJOIN

#ifdef Pandas_NpcFilter_PARTYLEAVE
	NPCF_PARTYLEAVE,	// partyleave_filter_name	// OnPCPartyLeaveFilter		// 当玩家准备离开队伍时触发过滤器 [聽風]
#endif // Pandas_NpcFilter_PARTYLEAVE

#ifdef Pandas_NpcFilter_DROPITEM
	NPCF_DROPITEM,	// dropitem_filter_name	// OnPCDropItemFilter		// 当玩家准备丢弃或掉落道具时触发过滤器
#endif // Pandas_NpcFilter_DROPITEM
	// PYHELP - NPCEVENT - INSERT POINT - <Section 2>

	/************************************************************************/
	/* Event  类型的标准事件，这些事件不能被 processhalt 打断                    */
	/************************************************************************/

#ifdef Pandas_NpcEvent_KILLMVP
	NPCE_KILLMVP,	// killmvp_event_name	// OnPCKillMvpEvent		// 当玩家杀死 MVP 魔物后触发事件
#endif // Pandas_NpcEvent_KILLMVP

#ifdef Pandas_NpcEvent_IDENTIFY
	NPCE_IDENTIFY,	// identify_event_name	// OnPCIdentifyEvent		// 当玩家成功鉴定了装备时触发事件
#endif // Pandas_NpcEvent_IDENTIFY

#ifdef Pandas_NpcEvent_INSERT_CARD
	NPCE_INSERT_CARD,	// insert_card_event_name	// OnPCInsertCardEvent		// 当玩家成功插入卡片后触发事件
#endif // Pandas_NpcEvent_INSERT_CARD

#ifdef Pandas_NpcEvent_USE_ITEM
	NPCE_USE_ITEM,	// use_item_event_name	// OnPCUseItemEvent		// 当玩家成功使用非装备类道具后触发事件
#endif // Pandas_NpcEvent_USE_ITEM

#ifdef Pandas_NpcEvent_USE_SKILL
	NPCE_USE_SKILL,	// use_skill_event_name	// OnPCUseSkillEvent		// 当玩家成功使用技能后触发事件
#endif // Pandas_NpcEvent_USE_SKILL

#ifdef Pandas_NpcEvent_EQUIP
	NPCE_EQUIP,	// equip_event_name	// OnPCEquipEvent		// 当玩家成功穿戴一件装备时触发事件
#endif // Pandas_NpcEvent_EQUIP

#ifdef Pandas_NpcEvent_UNEQUIP
	NPCE_UNEQUIP,	// unequip_event_name	// OnPCUnequipEvent		// 当玩家成功脱下一件装备时触发事件
#endif // Pandas_NpcEvent_UNEQUIP
	// PYHELP - NPCEVENT - INSERT POINT - <Section 8>

	/************************************************************************/
	/* Express 类型的快速事件，这些事件将会被立刻执行, 不进事件队列                */
	/************************************************************************/

#ifdef Pandas_NpcExpress_STATCALC
	NPCE_STATCALC,	// statcalc_express_name	// OnPCStatCalcEvent		// 当角色能力被重新计算时触发事件
#endif // Pandas_NpcExpress_STATCALC

#ifdef Pandas_NpcExpress_SC_END
	NPCX_SC_END,	// sc_end_express_name	// OnPCBuffEndExpress		// 当玩家成功解除一个状态(Buff)后触发实时事件
#endif // Pandas_NpcExpress_SC_END

#ifdef Pandas_NpcExpress_SC_START
	NPCX_SC_START,	// sc_start_express_name	// OnPCBuffStartExpress		// 当玩家成功获得一个状态(Buff)后触发实时事件
#endif // Pandas_NpcExpress_SC_START

#ifdef Pandas_NpcExpress_ENTERMAP
	NPCX_ENTERMAP,	// entermap_express_name	// OnPCEnterMapExpress		// 当玩家进入或者改变地图时触发实时事件
#endif // Pandas_NpcExpress_ENTERMAP

#ifdef Pandas_NpcExpress_PROGRESSABORT
	NPCX_PROGRESSABORT,	// progressabort_express_name	// OnPCProgressAbortExpress		// 当 progressbar 进度条被打断时触发实时事件
#endif // Pandas_NpcExpress_PROGRESSABORT

#ifdef Pandas_NpcExpress_UNIT_KILL
	NPCX_UNIT_KILL,	// unit_kill_express_name	// OnUnitKillExpress		// 当某个单位被击杀时触发实时事件
#endif // Pandas_NpcExpress_UNIT_KILL

#ifdef Pandas_NpcExpress_MOBDROPITEM
	NPCX_MOBDROPITEM,	// mobdropitem_express_name	// OnMobDropItemExpress		// 当魔物即将掉落道具时触发实时事件
#endif // Pandas_NpcExpress_MOBDROPITEM

#ifdef Pandas_NpcExpress_PCATTACK
	NPCX_PCATTACK,	// pcattack_express_name	// OnPCAttackExpress		// 当玩家发起攻击并即将进行结算时触发实时事件 [聽風]
#endif // Pandas_NpcExpress_PCATTACK

#ifdef Pandas_NpcExpress_MER_CALL
	NPCX_MER_CALL,	// mer_call_express_name	// OnPCMerCallExpress		// 当玩家成功召唤出佣兵时触发实时事件
#endif // Pandas_NpcExpress_MER_CALL

#ifdef Pandas_NpcExpress_MER_LEAVE
	NPCX_MER_LEAVE,	// mer_leave_express_name	// OnPCMerLeaveExpress		// 当佣兵离开玩家时触发实时事件
#endif // Pandas_NpcExpress_MER_LEAVE
	// PYHELP - NPCEVENT - INSERT POINT - <Section 14>

	NPCE_MAX
};

#ifdef Pandas_NpcEvent_KILLMVP
void npc_event_aide_killmvp(struct map_session_data* sd, struct map_session_data* mvp_sd, struct mob_data* md);
#endif // Pandas_NpcEvent_KILLMVP
#ifdef Pandas_NpcExpress_UNIT_KILL
void npc_event_aide_unitkill(struct block_list* src, struct block_list* target, uint16 skillid);
#endif // Pandas_NpcExpress_UNIT_KILL
#ifdef Pandas_NpcExpress_MOBDROPITEM
bool npc_express_aide_mobdropitem(struct mob_data* md,
	struct block_list* src, int belond_rid, t_itemid nameid, int drop_rate, int drop_type);
bool npc_express_aide_mobdropitem(struct mob_data* md,
	struct block_list* src, struct item_drop_list* dlist, t_itemid nameid, int drop_rate, int drop_type);
#endif // Pandas_NpcExpress_MOBDROPITEM

#ifdef Pandas_Helper_Common_Function
struct event_data* npc_event_data(const char* eventname);
bool npc_event_exists(const char* eventname);
bool npc_event_exists(struct npc_data *nd, const char* eventname);
#endif // Pandas_Helper_Common_Function

// Status of NPC view.
enum e_npcv_status : uint8 {
	NPCVIEW_DISABLE  = 0x01,
	NPCVIEW_ENABLE   = 0x02,
	NPCVIEW_HIDEOFF  = 0x04,
	NPCVIEW_HIDEON   = 0x08,
	NPCVIEW_CLOAKOFF = 0x10,
	NPCVIEW_CLOAKON  = 0x20,

	NPCVIEW_VISIBLE   = 0x16,
	NPCVIEW_INVISIBLE = 0x29,
	NPCVIEW_CLOAK     = 0x30,
};
struct view_data* npc_get_viewdata(int class_);
int npc_chat_sub(struct block_list* bl, va_list ap);
int npc_event_dequeue(struct map_session_data* sd,bool free_script_stack=true);
int npc_event(struct map_session_data* sd, const char* eventname, int ontouch);
int npc_touch_areanpc(struct map_session_data* sd, int16 m, int16 x, int16 y, struct npc_data* nd);
int npc_touch_area_allnpc(struct map_session_data* sd, int16 m, int16 x, int16 y);
int npc_touch_areanpc2(struct mob_data *md); // [Skotlex]
int npc_check_areanpc(int flag, int16 m, int16 x, int16 y, int16 range);
int npc_touchnext_areanpc(struct map_session_data* sd,bool leavemap);
int npc_click(struct map_session_data* sd, struct npc_data* nd);
bool npc_scriptcont(struct map_session_data* sd, int id, bool closing);
struct npc_data* npc_checknear(struct map_session_data* sd, struct block_list* bl);
int npc_buysellsel(struct map_session_data* sd, int id, int type);
uint8 npc_buylist(struct map_session_data* sd, uint16 n, struct s_npc_buy_list *item_list);
static int npc_buylist_sub(struct map_session_data* sd, uint16 n, struct s_npc_buy_list *item_list, struct npc_data* nd);
uint8 npc_selllist(struct map_session_data* sd, int n, unsigned short *item_list);
void npc_parse_mob2(struct spawn_data* mob);
struct npc_data* npc_add_warp(char* name, short from_mapid, short from_x, short from_y, short xs, short ys, unsigned short to_mapindex, short to_x, short to_y);
int npc_globalmessage(const char* name,const char* mes);
const char *npc_get_script_event_name(int npce_index);

void npc_setcells(struct npc_data* nd);
void npc_unsetcells(struct npc_data* nd);
bool npc_movenpc(struct npc_data* nd, int16 x, int16 y);
bool npc_is_cloaked(struct npc_data* nd, struct map_session_data* sd);
bool npc_enable_target(npc_data& nd, uint32 char_id, e_npcv_status flag);
#define npc_enable(nd, flag) npc_enable_target(nd, 0, flag)
void npc_setdisplayname(struct npc_data* nd, const char* newname);
void npc_setclass(struct npc_data* nd, short class_);
struct npc_data* npc_name2id(const char* name);
int npc_isnear_sub(struct block_list* bl, va_list args);
bool npc_isnear(struct block_list * bl);

int npc_get_new_npc_id(void);

int npc_addsrcfile(const char* name, bool loadscript);
void npc_delsrcfile(const char* name);
int npc_parsesrcfile(const char* filepath);
void do_clear_npc(void);
void do_final_npc(void);
void do_init_npc(void);
void npc_event_do_oninit(void);

int npc_event_do(const char* name);
int npc_event_do_id(const char* name, int rid);
int npc_event_doall(const char* name);
void npc_event_runall( const char* eventname );
int npc_event_doall_id(const char* name, int rid);
int npc_event_doall_path(const char* event_name, const char* path);

int npc_timerevent_start(struct npc_data* nd, int rid);
int npc_timerevent_stop(struct npc_data* nd);
void npc_timerevent_quit(struct map_session_data* sd);
t_tick npc_gettimerevent_tick(struct npc_data* nd);
int npc_settimerevent_tick(struct npc_data* nd, int newtimer);
int npc_remove_map(struct npc_data* nd);
void npc_unload_duplicates (struct npc_data* nd);
int npc_unload(struct npc_data* nd, bool single);
int npc_reload(void);
void npc_read_event_script(void);
int npc_script_event(struct map_session_data* sd, enum npce_event type);

int npc_duplicate4instance(struct npc_data *snd, int16 m);
int npc_instanceinit(struct npc_data* nd);
int npc_instancedestroy(struct npc_data* nd);
int npc_cashshop_buy(struct map_session_data *sd, t_itemid nameid, int amount, int points);

void npc_shop_currency_type(struct map_session_data *sd, struct npc_data *nd, int cost[2], bool display);

extern struct npc_data* fake_nd;

int npc_cashshop_buylist(struct map_session_data *sd, int points, int count, struct PACKET_CZ_PC_BUY_CASH_POINT_ITEM_sub* item_list);
bool npc_shop_discount(struct npc_data* nd);

#if PACKETVER >= 20131223
void npc_market_tosql(const char *exname, struct npc_item_list *list);
void npc_market_delfromsql_(const char *exname, t_itemid nameid, bool clear);
#endif

#ifdef SECURE_NPCTIMEOUT
	TIMER_FUNC(npc_secure_timeout_timer);
#endif

// @commands (script-based)
int npc_do_atcmd_event(struct map_session_data* sd, const char* command, const char* message, const char* eventname);

bool npc_unloadfile( const char* path );

#ifdef Pandas_Struct_Map_Session_Data_EventHalt
bool setProcessHalt(struct map_session_data *sd, enum npce_event event, bool halt = true);
bool getProcessHalt(struct map_session_data *sd, enum npce_event event, bool autoreset = true);
bool npc_script_filter(struct map_session_data* sd, enum npce_event type);
bool npc_script_filter(struct map_session_data* sd, const char* eventname);
#endif // Pandas_Struct_Map_Session_Data_EventHalt

#ifdef Pandas_Struct_Map_Session_Data_WorkInEvent
enum npce_event npc_get_script_event_type(const char* eventname);
#endif // Pandas_Struct_Map_Session_Data_WorkInEvent

#ifdef Pandas_Struct_Map_Session_Data_EventTrigger
enum npce_trigger : uint16 {
	EVENT_TRIGGER_NONE = 0x00,
	EVENT_TRIGGER_DISABLED = 0x01,
	EVENT_TRIGGER_ONCE = 0x02,
	EVENT_TRIGGER_EVER = 0x03,
	EVENT_TRIGGER_MAX
};

bool setEventTrigger(struct map_session_data *sd, enum npce_event event, enum npce_trigger trigger_flag);
npce_trigger getEventTrigger(struct map_session_data *sd, enum npce_event event);
bool isAllowTriggerEvent(struct map_session_data* sd, enum npce_event event);
#endif // Pandas_Struct_Map_Session_Data_EventTrigger

#ifdef Pandas_ScriptEngine_Express
bool npc_event_is_express(enum npce_event eventtype);
bool npc_event_is_filter(enum npce_event eventtype);
bool npc_event_is_realtime(enum npce_event eventtype);
#endif // Pandas_ScriptEngine_Express

#ifdef Pandas_ScriptCommand_Copynpc
// 以下几个函数主要是为了将 npc.cpp 中定义的几个全局变量导出
DBMap* get_npcname_db_ptr();
int* get_npc_script_ptr();
int* get_npc_shop_ptr();
int* get_npc_warp_ptr();

// 以下几个函数是 rAthena 默认存在的, 这里导出给其他 cpp 使用
struct npc_data *npc_create_npc(int16 m, int16 x, int16 y);
int npc_event_export(struct npc_data *nd, int i);
int npc_timerevent_export(struct npc_data *nd, int i);
void npc_parsename(struct npc_data* nd, const char* name, const char* start, const char* buffer, const char* filepath);
int npc_parseview(const char* w4, const char* start, const char* buffer, const char* filepath);
#endif // Pandas_ScriptCommand_Copynpc

#ifdef Pandas_Character_Title_Controller
bool npc_change_title_event(struct map_session_data* sd, uint32 title_id, int mode);
#endif // Pandas_Character_Title_Controller

#endif /* NPC_HPP */
