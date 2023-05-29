#pragma once
#include <type_traits>

#include "IDebugObject.h"
#include "IClassDescriptor.h"

namespace Debugger
{
	template<typename Base, typename Descriptor>
		requires (std::is_base_of_v<IClassDescriptor, Descriptor>&& std::is_base_of_v<IDebugObject, Base>)
	class TDebugDerivedObject : public Base
	{
	private:
		static const Descriptor m_descriptor;

	public:
		virtual const IClassDescriptor& descriptor()const noexcept override { return m_descriptor; }
	};

	template<typename Base, typename Descriptor>
		requires (std::is_base_of_v<IClassDescriptor, Descriptor>&& std::is_base_of_v<IDebugObject, Base>)
	const Descriptor TDebugDerivedObject<Base, Descriptor>::m_descriptor;

}