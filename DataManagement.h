#ifndef KEYWORD_HEADER
#define KEYWORD_HEADER

#include "Keyword.h"
#include "cqsdk/cqsdk.h"
#include <string>
#include <unordered_map>

class BotKeyword;

#define botDataMgr BambooBot::BotDataManagement::self()

namespace BambooBot
{
	using namespace std;
	// 机器人数据管理类
	class BotDataManagement
	{
	public:
		BotDataManagement();
		~BotDataManagement();
		// 注册关键词
		void registerKeyword(BotKeyword *k);
		// 删除关键词
		void deleteKeyword(BotKeyword *k);
		// 根据关键词名进行搜索
		BotKeyword* find(const string &keyword);
		// 返回所有关键词，使用完毕以后请 delete 返回的向量指针
		vector<BotKeyword*>* keywords() noexcept;
		// 返回自己的指针
		static BotDataManagement* self() { return self_ptr; }

		// 消息接收槽函数
		void slotReceiveMessage(const cq::event::GroupMessageEvent &e);
		void slotReceiveMessage(const cq::event::PrivateMessageEvent &e);

		bool operator ==(const BotDataManagement &right) = delete;
		bool operator >=(const BotDataManagement &right) = delete;
		bool operator <=(const BotDataManagement &right) = delete;
		bool operator >(const BotDataManagement &right) = delete;
		bool operator !=(const BotDataManagement &right) = delete;
		BotDataManagement& operator =(const BotDataManagement &right) = delete;
		BotDataManagement& operator <<(BotKeyword *right);
	private:
		unordered_map<string, BotKeyword*> _keyword_map;
		static BotDataManagement *self_ptr;
		vector<string>* keys() noexcept;
		size_t matchCounts(const string &input, const string &patten);
	};
}
#endif