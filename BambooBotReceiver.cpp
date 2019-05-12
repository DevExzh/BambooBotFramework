#include "cqsdk/cqsdk.h"
#include <string>
#include <algorithm>
#include <filesystem>
#include <sstream>
#include <fstream>
#include <ostream>
#include <vector>
#include <tuple>
#include <thread>
#include <chrono>
#include <unordered_map>
#include "DataManagement.h"

namespace app = cq::app; // 插件本身的生命周期事件和管理
namespace event = cq::event; // 用于注册 QQ 相关的事件处理函数
namespace api = cq::api; // 用于调用酷 Q 提供的接口
namespace logging = cq::logging; // 用于日志
namespace message = cq::message; // 提供封装了的 Message 等类

import std;
import BambooBot;

namespace Kit {
	// 判断字符串是否以特定的字符开头
	bool startsWith(const char* _string, const char* _patten)
	{
		bool result = false;
		if (strlen(_string) < strlen(_patten)) return result;
		for (int i = 0;i < static_cast<int>(string(_patten).length());++i)
		{
			if (_string[i] == _patten[i]) result = true;
			else result = false;
		}
		return result;
	}
	// 判断字符串是否以特定的字符结尾
	bool endsWith(const char* _string, const char* _patten)
	{
		bool result = false;
		if (strlen(_string) < strlen(_patten)) return result;
		for (int i = 0;i < static_cast<int>(string(_patten).length());++i)
		{
			if (_string[strlen(_string) - i] == _patten[strlen(_patten) - i]) result = true;
			else result = false;
		}
		return result;
	}
	// 判断字符串是否包含指定的子串
	bool contains(const string &_string, const string &_patten)
	{
		if (_string == _patten) return true;
		if (_string.size() < _string.size()) return false;
		if (_string.find(_patten) == string::npos) return false;
		else return true;
	}
	// 根据索引值取得字符串的子串，从 indexStart 开始到 indexEnd 结束
	string stringFromIndexes(string input, int indexStart, int indexEnd)
	{
		return input.substr(indexStart,indexEnd-indexStart);
	}
	// 在字符串中根据键值取对应值
	// input : 输入的字符串
	// key : 键值
	// 格式：[Key]=[Value];
	string value(const string &input, const string &key)
	{
		int index = -404;
		for (int i = 0;i < static_cast<int>(input.length());++i)
		{
			if (input[i] == key[i]) {
				bool result = false;
				for (int j = 0;j < static_cast<int>(key.length());++j)
				{
					if (input[i + j] == key[j]) result = true;
					else result = false;
				}
				if (!result) continue; else
				{
					index = i + key.length();
					break;
				}
			}
		}
		for (int k = 0;k < static_cast<int>(input.length() - key.length());++k)
		{
			if (input[index + k] == ';')
			{
				return stringFromIndexes(input, index, k - 1);
			}
		}
		return u8"unknown";
	}
}

namespace BotChannel {
	using namespace Kit;
	// 删除特定的字符
	string removeChar(const string &input, const char* character)
	{
		string _tmp = input;
		for (int i = 0;i < static_cast<int>(input.length());++i)
		{
			if (_tmp[i] == *character) _tmp[i] = *u8"";
		}
		return _tmp;
	}
	// 将 std::string 转换成 bool
	bool toBoolean(const string &boolean)
	{
		auto && _bl(removeChar(removeChar(boolean, "\n"), "\0"));
		if (_bl == u8"true") return true; else return false;
	}
	// 检查命令是否合法
	bool isAvaliable(const string &str)
	{
		if (str.find_first_of("!") == 0) return true;
		else return false;
	}

	std::tuple<vector<int>,int> findAll(const string &input, const string &patten)
	{
		vector<int> *_g = new vector<int>();
		auto _t = input.find(patten, 0);
		int num = 0;
		while (_t != string::npos)
		{
			_g->push_back(_t);
			++num;
			_t = input.find(patten, _t + 1);
		}
		return { *_g, num };
	}

	std::tuple<string, std::vector<string>> readCommandFromString(const string &input)
	{
		vector<string> *_vec = new vector<string>();
		string _cmd = input.substr(input.find_first_of(u8"!") + 1, input.find_first_of(u8" ") - input.find_first_of(u8"!") - 1);
		string _in = input + u8" ";
		auto _r = findAll(_in, u8" ");
		for (int p = 0; p < get<1>(_r) - 1; ++p)
		{
			_vec->push_back(_in.substr(get<0>(_r)[p] + 1, get<0>(_r)[p + 1] - get<0>(_r)[p]));
		}
		return { _cmd, *_vec };
	}
}

using namespace Kit;

// 初始化 App Id
CQ_INITIALIZE("xyz.bambooisland.bot");

// 插件入口，在静态成员初始化之后，app::on_initialize 事件发生之前被执行，用于配置 SDK 和注册事件回调
CQ_MAIN {
    cq::config.convert_unicode_emoji = true; // 配置 SDK 自动转换 Emoji 到 Unicode（默认就是 true）

    app::on_enable = [] {
		BambooBot::BotDataManagement *mgr = new BambooBot::BotDataManagement();
    };

	app::on_disable = [] {
		delete botDataMgr;
	};

	std::unordered_map<string, string> *_key_map = nullptr;

	event::on_group_member_decrease = [](const cq::event::GroupMemberDecreaseEvent &e) {
		auto sexString = [=](cq::Sex &sex)->const string{
			switch (sex)
			{
			case cq::Sex::FEMALE:
				return u8"女";
			case cq::Sex::MALE:
				return u8"男";
			case cq::Sex::UNKNOWN:
			default:
				return u8"未知";
			}
		};
		auto sender = cq::api::get_stranger_info(e.user_id, false);
		message::send(e.target, u8"群成员离开了本群，尝试将其移出白名单系统");
		message::send(e.target, u8"将被操作的群成员信息如下：\n----------------------------\n昵称: " + sender.nickname + u8"\n年龄: " + std::to_string(sender.age) + u8"\n性别: " + sexString(sender.sex) + u8"\nQQ 号: " + std::to_string(sender.user_id) + u8"\n----------------------------");
	};

	event::on_private_msg = [](const cq::PrivateMessageEvent &e) {
		botDataMgr->slotReceiveMessage(e);
	};

    event::on_group_msg = [](const cq::event::GroupMessageEvent &e) {
		auto func_at = [=]()->string {return u8"@" + api::get_group_member_info(e.group_id, e.user_id, false).card;};
		auto isAdmin = [=]()->bool {return (api::get_group_member_info(e.group_id, e.user_id, false).role == cq::GroupRole::ADMIN || api::get_group_member_info(e.group_id, e.user_id, false).role == cq::GroupRole::OWNER);};
		botDataMgr->slotReceiveMessage(e);
        if (BotChannel::isAvaliable(e.message.extract_plain_text()) && get<0>(BotChannel::readCommandFromString(e.message.extract_plain_text())) == u8"version")
		{
			message::send(e.target, func_at() + u8" BambooBot v2.2.8\n开发者: Exzh_PMGI\n类型: [Release]\nGithub: https://github.com/DevExzh\n基于 CoolQ C++ SDK 的多功能机器人");
			return;
		}
    };

    event::on_group_request = [](const cq::event::GroupRequestEvent &e)
    {
		auto lambda_lower = [=]()->string {
			string _xx = e.comment;
			transform(_xx.begin(), _xx.end(), _xx.begin(), ::tolower);
			return _xx;
		};
		if (Kit::contains(lambda_lower(), string(u8"请在此输入您的加群关键词")))
        {
            cq::api::set_group_add_request(e.flag,cq::request::SubType::GROUP_ADD,cq::request::Operation::APPROVE);
		}
		else cq::api::set_group_add_request(e.flag,cq::request::SubType::GROUP_ADD,cq::request::Operation::REJECT,u8"请正确回答问题！");
	};

	event::on_group_member_increase = [](const cq::event::GroupMemberIncreaseEvent &e)
	{
		message::send(e.target, u8"@" + api::get_group_member_info(e.group_id, e.user_id, false).nickname + u8"  欢迎来到竹洲 Minecraft 服务器！\n入群须知\nI.进入服务器请先阅读群公告并完成白名单申请表的填写！\nII.请将您的群名片更改为您的正版账号ID，有多个正版账号的请填写您经常使用的一个ID\nIII.有关服务器规定的请参见群公告中的《竹洲服务器管理条例》\nIV.申请表填写完毕以后您将会收到来自服务器发送的确认邮件，请查收\n\n发送\"关键词\"以了解更多");
	};
}
