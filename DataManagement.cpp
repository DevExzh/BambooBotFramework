#include "DataManagement.h"
#include "Keyword.h"
#include <vector>
using namespace std;

BambooBot::BotDataManagement* BambooBot::BotDataManagement::self_ptr = nullptr;

BambooBot::BotDataManagement::BotDataManagement()
{
	if (self_ptr != nullptr) return; else self_ptr = this;
}

BambooBot::BotDataManagement::~BotDataManagement()
{
	auto _k = this->keys();
	for (int i = 0;i < static_cast<int>(_k->size());++i)
	{
		auto _p = _keyword_map.at(_k->at(i));
		delete _p;
		_p = nullptr;
	}
	_keyword_map.clear();
}

void BambooBot::BotDataManagement::registerKeyword(BotKeyword *k)
{
	_keyword_map[k->keyword()] = k;
}

void BambooBot::BotDataManagement::deleteKeyword(BotKeyword *k)
{
	_keyword_map.erase(k->keyword());
}

BambooBot::BotKeyword* BambooBot::BotDataManagement::find(const string & keyword)
{
	return _keyword_map[keyword];
}

vector<BambooBot::BotKeyword*>* BambooBot::BotDataManagement::keywords() noexcept
{
	vector<BambooBot::BotKeyword*> *_result = new vector<BambooBot::BotKeyword*>();
	auto _keys = this->keys();
	for (int i = 0;i < static_cast<int>(_keys->size());++i)
	{
		_result->push_back(_keyword_map.at(_keys->at(i)));
	}
	delete _keys;
	return _result;
}

void BambooBot::BotDataManagement::slotReceiveMessage(const cq::event::PrivateMessageEvent &e)
{
	vector<BambooBot::BotKeyword*> *_keys = this->keywords();
	size_t num = 0;
	for (size_t k = 0;k < _keys->size();++k)
	{
		num += matchCounts(e.message.extract_plain_text(), _keys->at(k)->keyword());
	}
	auto contain_alias = [=](BambooBot::BotKeyword *key)->bool {
		for (size_t j = 0; j < key->aliases().value().size(); ++j)
		{
			if (e.message.extract_plain_text().find(key->aliases().value()[j]) != string::npos)
				return true;
		}
		return false;
	};
	for (size_t i = 0;i < _keys->size();++i)
	{
		if (((e.message.extract_plain_text().find(_keys->at(i)->keyword()) != string::npos) || contain_alias(_keys->at(i))) && (_keys->at(i)->responseType() == BotResponseType::All || _keys->at(i)->responseType() == BotResponseType::PrivateMessage))
		{
			cq::message::send(e.target,_keys->at(i)->response());
		}
	}
	delete _keys;
}

void BambooBot::BotDataManagement::slotReceiveMessage(const cq::event::GroupMessageEvent & e)
{
	auto func_at = [=]()->string {return u8"@" + cq::api::get_group_member_info(e.group_id, e.user_id, false).nickname + u8" ";};
	vector<BambooBot::BotKeyword*> *_keys = this->keywords();
	size_t num = 0;
	for (size_t k = 0;k < _keys->size();++k)
	{
		num += matchCounts(e.message.extract_plain_text(), _keys->at(k)->keyword());
	}
	if (num > 1)
	{
		cq::message::send(e.target, func_at() + u8"非法的多命令请求");
		return;
	}
	auto isAdmin = [=]()->bool {return (cq::api::get_group_member_info(e.group_id, e.user_id, false).role == cq::GroupRole::ADMIN || cq::api::get_group_member_info(e.group_id, e.user_id, false).role == cq::GroupRole::OWNER);};
	auto contain_alias = [=](BambooBot::BotKeyword *key)->bool {
		for (size_t j = 0; j < key->aliases().value().size(); ++j)
		{
			if (e.message.extract_plain_text().find(key->aliases().value()[j]) != string::npos)
				return true;
		}
		return false;
	};
	for (size_t i = 0;i < _keys->size();++i)
	{
		if (((e.message.extract_plain_text().find(_keys->at(i)->keyword()) != string::npos) || contain_alias(_keys->at(i))) && (_keys->at(i)->responseType() == BotResponseType::All || _keys->at(i)->responseType() == BotResponseType::GroupMessage))
		{
			cq::message::send(e.target, func_at() + _keys->at(i)->response());
		}
	}
	delete _keys;
}

BambooBot::BotDataManagement & BambooBot::BotDataManagement::operator<<(BotKeyword *right)
{
	this->registerKeyword(right);
	return *this;
}

vector<string>* BambooBot::BotDataManagement::keys() noexcept
{
	vector<string> *_keys = new vector<string>();
	_keys->reserve(_keyword_map.size());
	for (unordered_map<string, BambooBot::BotKeyword*>::iterator iter = _keyword_map.begin();iter != _keyword_map.end();++iter)
	{
		_keys->push_back(iter->first);
	}
	return _keys;
}

size_t BambooBot::BotDataManagement::matchCounts(const string & input, const string & patten)
{
	auto _t = input.find(patten, 0);
	size_t num = 0;
	while (_t != string::npos)
	{
		++num;
		_t = input.find(patten, _t + 1);
	}
	return num;
}
