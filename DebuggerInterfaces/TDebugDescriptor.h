#pragma once
#include <string>
#include <vector>
#include <functional>
#include <typeinfo>
	
#include "IClassDescriptor.h"
#include "IDebugVariable.h"

namespace Debugger
{
	template<typename Type>
	std::string type_to_string()
	{
		std::string strType = typeid(Type).name();
		auto iter = std::find(strType.begin(), strType.end(), ' ');
		if (iter != strType.end())
		{
			int index = iter - strType.begin();
			strType = strType.substr(index + 1, strType.size() - index - 1);
		}
		return strType;
	}

	/*@brief describe class of type DebugType*/
	template<typename DebugType>
	class TClassDescriptor : public IClassDescriptor
	{
	protected:
		std::vector<std::unique_ptr<IDebugVariable>> m_vVariableDescriptors;/*!< debug variables*/

	public:
		virtual std::string className()const noexcept override { return type_to_string<DebugType>(); }
		virtual size_t classSize()const noexcept { return sizeof(DebugType); }
		virtual std::string parentClassName() const noexcept { return ""; }
		void edit(IDebugObject* const a_pObj, IInterfacePropertyEditor& a_editor) final
		{
			if (a_pObj)
			{
				for (auto&& descriptor : m_vVariableDescriptors)
					descriptor->edit(a_pObj, a_editor);
			}
		}

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
		virtual std::string className()const noexcept override { return type_to_string<DebugType>(); }
		virtual size_t classSize()const noexcept { return sizeof(DebugType); }
		virtual std::string parentClassName() const noexcept { return Base::className() + ">" + Base::parentClassName(); }
	};
}

#define ADD_MEMBER(classname, type, memberName, getter, setter) \
m_vVariableDescriptors.emplace_back(std::make_unique<TDebugVariable<classname, type>>(#memberName, \
	std::bind_front(&classname::setter), std::bind_front(&classname::getter)));

#define ADD_READ_ONLY_MEMBER(classname, type, memberName, getter) \
m_vVariableDescriptors.emplace_back(std::make_unique<TDebugVariable<classname, type>>(#memberName, \
	nullptr, std::bind_front(&classname::getter)));

#define GEN_DESCRIPTORNAME(classname) Debug##classname

#define BEGIN_DESCRIPTOR(classname) \
class GEN_DESCRIPTORNAME(classname) : public TClassDescriptor<classname>\
{\
public:\
	~Debug##classname() = default; \
	Debug##classname()\
{


#define BEGIN_INHERIT_DESCRIPTOR(classname, parent) \
class GEN_DESCRIPTORNAME(classname) : public TClassDerivedDescriptor<Debug##parent>\
{\
public:\
	~Debug##classname() = default; \
	Debug##classname()\
{


#define END_DESCRIPTOR \
}\
};
