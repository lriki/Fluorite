
#include "Internal.h"
#include <Fluorite/Diagnostics.h>

namespace fl {

//==============================================================================
// DiagnosticsItemSet
//==============================================================================
//------------------------------------------------------------------------------
void DiagnosticsItemSet::Report(DiagnosticsCode code)
{
	DiagnosticsItem item;
	item.m_code = code;
	item.m_lineNumber = m_currentLineNumber;
	item.m_columnNumber = m_currentColumnNumber;
	m_items.Add(item);
}

//------------------------------------------------------------------------------
void DiagnosticsItemSet::Report(DiagnosticsCode code, flString option1)
{
	DiagnosticsItem item;
	item.m_code = code;
	item.m_lineNumber = m_currentLineNumber;
	item.m_columnNumber = m_currentColumnNumber;
	item.m_options.Add(option1);
	m_items.Add(item);
}

} // namespace fl


