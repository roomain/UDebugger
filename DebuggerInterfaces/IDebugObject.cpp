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
		if (m_ownerObject)
			m_ownerObject->removeOwned(this);
		m_ownedObjects.clear();
	}

	void IDebugObject::setOwner(IDebugObject* const a_pOwner)noexcept
	{
		s_pDatabase->ownerChange(this, a_pOwner);
		if (m_ownerObject != a_pOwner)
		{
			if (a_pOwner)
			{
				a_pOwner->appendOwned(this);
			}
			else
			{
				a_pOwner->removeOwned(this);
			}
		}
		m_ownerObject = a_pOwner;
	}

	void IDebugObject::removeOwned(IDebugObject* const a_owned)
	{
		auto iter = std::ranges::find(m_ownedObjects.begin(), m_ownedObjects.end(), a_owned);
		if (iter != m_ownedObjects.end())
			m_ownedObjects.erase(iter);
	}

	void IDebugObject::appendOwned(IDebugObject* const a_owned)
	{
		m_ownedObjects.push_back(a_owned);
	}
}
