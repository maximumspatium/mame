// license:GPL-2.0+
// copyright-holders:Couriersud
/*
 * nl_string.c
 *
 */

#include "pfmtlog.h"
#include "palloc.h"

#include <algorithm>
#include <cstdarg>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <locale>

namespace plib {

pfmt &pfmt::format_element(const char *l, const unsigned cfmt_spec,  ...)
{
	va_list ap;
	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
	va_start(ap, cfmt_spec);
	pstring fmt("%");
	char buf[2048]; // FIXME
	std::size_t sl;

	m_arg++;

	pstring search("{");
	search += plib::to_string(m_arg);
	sl = search.size();

	auto p = m_str.find(search + ":");
	sl++; // ":"
	if (p == pstring::npos) // no further specifiers
	{
		p = m_str.find(search + "}");
		if (p == pstring::npos) // not found try default
		{
			sl = 2;
			p = m_str.find("{}");
		}
		if (p == pstring::npos)
		{
			sl=1;
			p = m_str.find("{");
			if (p != pstring:: npos)
			{
				auto p1 = m_str.find("}", p);
				if (p1 != pstring::npos)
				{
					sl = p1 - p + 1;
					fmt += m_str.substr(p+1, p1 - p - 1);
				}
			}
		}
	}
	else
	{
		auto p1 = m_str.find("}", p);
		if (p1 != pstring::npos)
		{
			sl = p1 - p + 1;
			fmt += ((m_arg>=10) ? m_str.substr(p+4, p1 - p - 4) : m_str.substr(p+3, p1 - p - 3));
		}
	}
	pstring::value_type pend = fmt.at(fmt.size() - 1);
	if (pstring("duxo").find(cfmt_spec) != pstring::npos)
	{
		if (pstring("duxo").find(pend) == pstring::npos)
			fmt += (pstring(l) + static_cast<pstring::value_type>(cfmt_spec));
		else
			fmt = plib::left(fmt, fmt.size() - 1) + pstring(l) + plib::right(fmt, 1);
	}
	else if (pstring("fge").find(cfmt_spec) != pstring::npos)
	{
		if (pstring("fge").find(pend) == pstring::npos)
			fmt += cfmt_spec;
	}
	else
		fmt += cfmt_spec;
	vsprintf(buf, fmt.c_str(), ap);
	if (p != pstring::npos)
		m_str = m_str.substr(0, p) + pstring(buf) + m_str.substr(p + sl);
	va_end(ap);
	return *this;
}

} // namespace plib
