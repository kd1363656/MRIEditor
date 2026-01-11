#include "MRIComponentBase.h"

const MRI::TypeInfo& MRI::Component::ComponentBase::GetTypeInfo() const
{
	return MRI::GetTypeInfo<MRI::Component::ComponentBase>();
}

void MRI::Component::ComponentBase::Enable()
{
	if (m_isEnabled) { return; }
	m_isEnabled = true;
}
void MRI::Component::ComponentBase::Disable()
{
	if (!m_isEnabled) { return; }
	m_isEnabled = false;
}