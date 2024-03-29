#include <algorithm>
#include "DebugDatabase.h"
#include "IDeserializer.h"
#include "ISerializer.h"
#include "IDebugObject.h"
#include "IClassDescriptor.h"

namespace Debugger
{

	void DebugDatabase::readAllObject(ReadObjectCallback a_readCallback)const
	{
		std::lock_guard<std::mutex> lock(m_accessMutex);
		for (const IDebugObject* pObject : m_qDatabase)
			a_readCallback(pObject);
	}

	void DebugDatabase::serializeTree(IDatabaseSerializer& a_ISerializer)const
	{
		a_ISerializer.beginDatabase();
		for (const IDebugObject* pObject : m_qDatabase)
		{
			a_ISerializer.serializeObject(pObject->objectName(), m_pCurrentCreated == pObject ? "none" : pObject->descriptor().className(), pObject->uiid(),
				pObject->owner() ? pObject->owner()->uiid() : 0);
		}
		a_ISerializer.endDatabase();
		m_pCurrentCreated = nullptr;
	}

	bool DebugDatabase::serializeObject(const int64_t& a_objUID, ISerializer& a_ISerializer)const
	{
		auto iter = std::ranges::find_if(m_qDatabase.cbegin(), m_qDatabase.cend(), [&a_objUID](auto pObject)
			{
				return a_objUID == pObject->uiid();
			});
		if (iter != m_qDatabase.cend())
		{
			ClassInfo info{ (*iter)->descriptor().className(),  (*iter)->descriptor().parentClassName(), (*iter)->descriptor().classSize() };

			a_ISerializer.beginObject((*iter)->objectName(), info, a_objUID,
				(*iter)->owner() ? (*iter)->owner()->uiid() : 0);
			(*iter)->descriptor().serialize((*iter), a_ISerializer);
			a_ISerializer.endObject();
			return true;
		}
		return false;
	}
	
	bool DebugDatabase::deserializeObject(const int64_t& a_objUID, const unsigned int a_varIndex, IDeserializer& a_IDeserializer)
	{
		auto iter = std::ranges::find_if(m_qDatabase.begin(), m_qDatabase.end(), [&a_objUID](auto pObject)
			{
				return a_objUID == pObject->uiid();
			});

		if (iter != m_qDatabase.cend())
		{
			(*iter)->descriptor().deserialize(a_varIndex, (*iter), a_IDeserializer);
			return true;
		}
		return false;
	}

	void DebugDatabase::append(IDebugObject* const a_pObject)
	{
		std::lock_guard<std::mutex> lock(m_accessMutex);
		m_pCurrentCreated = a_pObject;
		m_qDatabase.push_back(a_pObject);
		if (m_bEnableReactor && m_reactors.onObjectAdded && a_pObject)
			m_reactors.onObjectAdded(a_pObject->uiid());
	}

	void DebugDatabase::remove(IDebugObject* const a_pObject)
	{
		std::lock_guard<std::mutex> lock(m_accessMutex);
		auto iter = std::ranges::find(m_qDatabase.begin(), m_qDatabase.end(), a_pObject);
		if(iter != m_qDatabase.end())
			m_qDatabase.erase(iter);

		if (m_bEnableReactor && m_reactors.onObjectRemoved && a_pObject)
			m_reactors.onObjectRemoved(a_pObject->uiid(), a_pObject->owner() ? a_pObject->owner()->uiid() : 0);
	}

	void DebugDatabase::ownerChange(const IDebugObject* a_pObject, const IDebugObject* a_pOwnerObject)const
	{
		if (m_bEnableReactor && m_reactors.onObjectOwner && a_pObject)
			m_reactors.onObjectOwner(a_pObject->uiid(), a_pOwnerObject ? a_pOwnerObject->uiid() : 0);
	}
}