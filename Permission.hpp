#pragma once
#include "cqsdk/cqsdk.h"
#include "BotBase.h"
#include <string>
import std;
namespace BambooBot {
	// 身份类型
	enum class RoleType : int {
		// 群主
		Owner = 4,
		// 管理员
		Administrator = 3,
		// 临时权限拥有者
		TemporaryOperator = 2,
		// 普通群员
		NormalMember = 1,
		// 被禁言的群员
		MutedMember = 0
	};

	// 将枚举类型转换为 int 类型
	// WARNING 不建议这么做，因为这是提供给机器人内部数据处理时使用的。
	int convert_role_type(RoleType type) {
		switch (type) {
		case RoleType::Owner:
			return 4;
		case RoleType::Administrator:
			return 3;
		case RoleType::TemporaryOperator:
			return 2;
		case RoleType::NormalMember:
			return 1;
		case RoleType::MutedMember:
			return 0;
		default:
			return -233;
		}
	}

	// 权限身份
	class GroupRole {
	public:
		GroupRole() = default;
		GroupRole(RoleType type) {
			this->_type = type;
		}

		GroupRole(cq::GroupRole role) {
			switch (role) {
			case cq::GroupRole::OWNER:
				this->_type = BambooBot::RoleType::Owner;
			case cq::GroupRole::ADMIN:
				this->_type = BambooBot::RoleType::Administrator;
			case cq::GroupRole::MEMBER:
				this->_type = BambooBot::RoleType::NormalMember;
			}
		}

		bool operator==(const GroupRole &right) { return this->_type == right._type; }
		bool operator!=(const GroupRole &right) { return !(*this == right); }
		bool operator>(const GroupRole &right) { return this->_type > right._type; }
		bool operator<(const GroupRole &right) { return !(*this < right); }
		bool operator>=(const GroupRole &right) { return *this > right || *this == right; }
		bool operator<=(const GroupRole &right) { return *this < right || *this == right; }
		GroupRole & operator++() {
			int _t = convert_role_type(this->_type);
			if (_t + 1 <= 4) {
				this->_type = RoleType(_t++);
			}
			return *this;
		}
		GroupRole & operator--() {
			int _t = convert_role_type(_type);
			if (_t - 1 >= 0) {
				this->_type = RoleType(_t--);
			}
			return *this;
		}
		GroupRole & operator-(const GroupRole &right) = delete;
		GroupRole & operator+(const GroupRole &right) = delete;
		GroupRole & operator*(const GroupRole &right) = delete;
		GroupRole & operator/(const GroupRole &right) = delete;
		GroupRole & operator=(const GroupRole &right) {
			this->_type = right._type;
			return *this;
		}

	private:
		BambooBot::RoleType _type;
	};

	// 机器人权限节点
	class BotPermission : public BotObject {
	public:
		BotPermission() = default;
		BotPermission(const string &name) {
			_permission_name = name;
		}
		// 权限节点名
		virtual string name() { return _permission_name; }
		// 权限描述
		virtual string descripton() = 0;
		// 默认的权限拥有者
		virtual GroupRole& defaultOwnedBy() = 0;

	private:
		string _permission_name;
	};
}