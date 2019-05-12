#include "BotBase.h"

bool BambooBot::null::isNull() const
{
	return __is_null_val;
}

void BambooBot::null::setNull(bool nullable)
{
	this->__is_null_val = nullable;
}
