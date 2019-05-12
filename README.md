# Bamboo Bot Framework
**A simple, light, cross-platform QQ bot framework**
**一个简易、轻量的跨平台 QQ 机器人框架**

#### Requirement 需求
1. a C++ compiler support C++ 17 Standard 支持 C++ 17 标准的编译器
2. [Optional] libcurl 7.47.0 or higher [可选] libcurl 7.47.0 或更高版本

#### How to start? 如何开始
```c++
#include <string>
#include <vector>
#include <BotBase.h> // The Bamboo Bot Framework Definitions 框架的宏定义以及基础模板类
#include <Keyword.h> // Keyword pure-virtual class
#include <DataManagement.h>
import std;
import BambooBot;

// Inherit the BotKeyword class
// 继承 BotKeyword 类，并实现纯虚类定义的函数
class ExampleKeyword : public BotKeyword {
public:
    ExampleKeyword() = default;
    ~ExampleKeyword() = default;
	virtual string keyword() { return u8"你好"; } // 关键词名，只要在聊天内容中包含此关键字就会使用 response() 中所定的返回值进行回复
	virtual string response() { return u8"你好呀"; }
	virtual string description() { return u8"一个简单的应答"; } // 关键词描述，可以留空
	virtual exNullableContent<vector<string>> aliases() { createVectors(u8"您好", u8"在吗"); } // 关键词别名，若不需要别名，请返回 null。createVectors 是一个宏定义，在新版本中已被移除，请手动返回一个 vector<string> 对象
	virtual BotResponseType responseType() { return BotResponseType::All; } // 响应的模式，机器人分群聊和私聊响应，若要限定响应的范围，请返回 BotResponseType::PrivateMessage 或 BotResponseType::GroupMessage
};

// 完成类的编写以后，请在您程序的入口函数处调用 botDataMgr->registerKeyword(new ExampleKeyword()); 来注册关键词
```

#### Donate us 赞助我们
请单击链接: https://www.bambooisland.top/donate/
您的支持是我们前进的不竭动力！
