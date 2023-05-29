#pragma once
#include <type_traits>

#include "IDebugObject.h"
#include "IClassDescriptor.h"

namespace Debugger
{
	/*@brief Template used as base class for debugging object, Descriptor is the class debug descriptor*/
	template<typename Descriptor> requires (std::is_base_of_v<IClassDescriptor, Descriptor>)
	class TDebugObject : public IDebugObject 
	{
	private:
		static const Descriptor m_descriptor;

	public:
		virtual const IClassDescriptor& descriptor()const noexcept override { return m_descriptor; }
	};

	template<typename Descriptor> requires (std::is_base_of_v<IClassDescriptor, Descriptor>)
	const Descriptor TDebugObject<Descriptor>::m_descriptor;

	
}