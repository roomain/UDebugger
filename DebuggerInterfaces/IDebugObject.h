#pragma once
#include <memory>
#include <string_view>
#include <cstdint>
#include <vector>
#include "DebugDatabase.h"
#include "debugger_global.h"

/*
 * @brief raw pointer are used because
 * we don't suppose user is using smart pointer
*/

namespace Debugger
{
	class IClassDescriptor;

	/*@brief interface for debuging object*/
	class DEBUGGER_INTERFACE_EXPORT IDebugObject
	{
	private:
		static int64_t s_Nextidentifier;
		static DebugDatabasePtr s_pDatabase;
		
		IDebugObject* m_ownerObject = nullptr;		/*!< container of this*/
		std::vector<IDebugObject*> m_ownedObjects;	/*!< owned objects*/

		std::string m_objectName = "no name";	/*!< user name*/
		int64_t m_uid;							/*!< unique identifier*/

	protected:
		void removeOwned(IDebugObject* const a_owned);
		void appendOwned(IDebugObject* const a_owned);

	public:

		using Object_constIter = std::vector<IDebugObject*>::const_iterator;
		Object_constIter cbegin()const { return m_ownedObjects.cbegin(); }
		Object_constIter cend()const { return m_ownedObjects.cend(); }

		IDebugObject();
		virtual ~IDebugObject();
		constexpr int64_t uiid()const { return m_uid; }
		void setOwner(IDebugObject* const a_pOwner)noexcept;
		constexpr const IDebugObject* owner()const noexcept { return m_ownerObject; }
		void setObjectName(const std::string_view& a_name) { m_objectName = a_name; }
		constexpr std::string_view objectName()const noexcept { return m_objectName; }
		virtual const IClassDescriptor& descriptor()const noexcept = 0;
		static [[nodiscard]] DebugDatabasePtr database() { return s_pDatabase; }
	};
}


