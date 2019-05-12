#pragma once
#include "BotBase.h"
#include "Permission.h"
#include <string>
#include <vector>
import std;
namespace BambooBot {
	// 机器人命令类，所有的命令必须继承此基类
	class BotCommand : public BotObject
	{
	public:
		// 是否需要指定的权限
		virtual bool needPermission(BotPermission *permission) = 0;
		// 命令名称 (必须是全英文的)
		virtual string command() = 0;
		// 命令描述
		virtual string decription() = 0;
		// 命令用法示例
		virtual string commandUsage() = 0;
		// 当用户无权使用命令时返回的消息
		virtual string permissionMessage() = 0;
		// 命令的别名
		virtual exNullableContent<vector<string>> aliases() = 0;
	};
}