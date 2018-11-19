﻿#ifndef _RATHENA_CN_CONFIG_HPP_
#define _RATHENA_CN_CONFIG_HPP_

#include "renewal.hpp"
#include "packets.hpp"

#define rAthenaCN

#ifdef rAthenaCN
	#define rAthenaCN_Basic
	#define rAthenaCN_Creative_Work
#endif // rAthenaCN

#ifdef rAthenaCN_Basic
	// 定义 rAthenaCN 的版本号
	#define rAthenaCN_Version "v1.8.0"

	// 在启动时显示 rAthenaCN 的 LOGO
	#define rAthenaCN_Show_Logo

	// 在启动时显示免责声明
	#define rAthenaCN_Disclaimer

	// 在启动时显示 rAthenaCN 的版本号
	#define rAthenaCN_Show_Version
#endif // rAthenaCN_Basic

#ifdef rAthenaCN_Creative_Work
	// 是否启用崩溃转储文件生成机制
	#ifdef _WIN32
		#define rAthenaCN_Crash_Report
	#endif // _WIN32
#endif // rAthenaCN_Creative_Work

#endif // _RATHENA_CN_CONFIG_HPP_
