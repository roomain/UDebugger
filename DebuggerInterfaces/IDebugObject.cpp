#include "IDebugObject.h"
#include "DebugDatabase.h"

namespace Debugger
{
	int64_t IDebugObject::s_Nextidentifier = 0;
	DebugDatabasePtr IDebugObject::s_pDatabase = std::make_shared<DebugDatabase>();

	IDebugObject::IDebugObject() : m_uid{ ++s_Nextidentifier }
	{
		s_pDatabase->append(this);
	}

	IDebugObject::~IDebugObject()
	{
		s_pDatabase->remove(this);
	}

	void IDebugObject::setOwner(IDebugObject* const a_pOwner)noexcept
	{
		s_pDatabase->ownerChange(this, a_pOwner);
		m_ownerObject = a_pOwner;
	}
}