#pragma once
#include <string>
#include <vector>
#include <functional>
#include <typeinfo>
	
#include "IClassDescriptor.h"
#include "IDebugVariable.h"

namespace Debugger
{
	/*@brief describe class of type DebugType*/
	template<typename DebugType>
	class TClassDescriptor : public IClassDescriptor
	{
	protected:
		std::vector<std::unique_ptr<IDebugVariable>> m_vVariableDescriptors;/*!< debug variables*/

	public:
		virtual std::string className()const noexcept override { return typeid(DebugType).name(); }
		virtual size_t classSize()const noexcept { return sizeof(DebugType); }
		virtual std::string parentClassName() const noexcept { return ""; }

		void serialize(IDebugObject* const a_pObj, ISerializer& a_ISerializer)const final
		{
			if (a_pObj)
			{
				for (auto&& descriptor : m_vVariableDescriptors)
					descriptor->serialize(a_pObj, a_ISerializer);
			}
		}

		void deserialize(const unsigned int a_varIndex, IDebugObject* const a_pObj, IDeserializer& a_Deserializer)const final
		{
			if (a_pObj && a_varIndex < static_cast<unsigned int>(m_vVariableDescriptors.size()))
				m_vVariableDescriptors[a_varIndex]->deserialize(a_pObj, a_Deserializer);
		}
	};

	template<typename Base, typename DebugType> requires (std::is_base_of_v<IClassDescriptor, Base>)
	class TClassDerivedDescriptor : public Base
	{
	public:
		virtual std::string className()const noexcept override { return typeid(DebugType).name(); }
		virtual size_t classSize()const noexcept { return sizeof(DebugType); }
		virtual std::string parentClassName() const noexcept { return Base::className() + ">" + Base::parentClassName(); }
	};
}