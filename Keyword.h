#pragma once
#include <string>
#include <vector>
#include "cqsdk/cqsdk.h"
#include "BotBase.h"
import std;
namespace BambooBot
{
	// 机器人响应模式
	enum class BotResponseType
	{
		// 仅私聊
		PrivateMessage,
		// 仅群聊
		GroupMessage,
		// 所有途径
		All = PrivateMessage | GroupMessage
	};

	// 所有机器人关键词的基类
	class BotKeyword : public BotObject
	{
	public:
		// 机器人响应模式
		virtual BotResponseType responseType() = 0;
		// 关键词
		virtual string keyword() = 0;
		// 返回的文本
		virtual string response() = 0;
		// 关键词描述
		virtual string description() = 0;
		// 关键词别名
		virtual exNullableContent<vector<string>> aliases() = 0;
	};
}