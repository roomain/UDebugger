#pragma once
#include <deque>
#include <memory>
#include <cstdint>
#include <functional>
#include "debugger_global.h"

namespace Debugger
{
	class IDebugObject;
	class ISerializer;
	class IDeserializer;
	class IDatabaseSerializer;

	/*@brief reactor for database modifications*/
	struct DebugDatabaseReactor
	{
		std::function<void(const uint64_t&)> onObjectAdded;
		std::function<void(const uint64_t&, const uint64_t&)> onObjectRemoved;
		std::function<void(const uint64_t&, const uint64_t&)> onObjectOwner;
	};

	/*@brief Debug database contains all debug instances*/
	class DEBUGGER_INTERFACE_EXPORT DebugDatabase
	{
	private:
		std::deque<IDebugObject*> m_qDatabase;	//!< database
		bool m_bEnableReactor = true;
		DebugDatabaseReactor m_reactors;
		
	public:
		DebugDatabase() = default;
		virtual ~DebugDatabase() = default;

		using Object_constIter = std::deque<IDebugObject*>::const_iterator;
		Object_constIter cbegin()const { return m_qDatabase.cbegin(); }
		Object_constIter cend()const { return m_qDatabase.cend(); }

		void setEnableReactors(const bool a_bEnable) { m_bEnableReactor = a_bEnable; }
		void setReactor(const DebugDatabaseReactor& a_reactors) { m_reactors = a_reactors; }
		void serializeTree(IDatabaseSerializer& a_ISerializer)const;
		bool serializeObject(const uint64_t& a_objUID, ISerializer& a_ISerializer)const;
		bool deserializeObject(const uint64_t& a_objUID, const unsigned int a_varIndex, IDeserializer& a_IDeserializer);
		void append(IDebugObject* const a_pObject);
		void remove(IDebugObject* const a_pObject);
		void ownerChange(const IDebugObject* a_pObject, const IDebugObject* a_pOwnerObject)const;
	};
	using DebugDatabasePtr = std::shared_ptr<DebugDatabase>;
}