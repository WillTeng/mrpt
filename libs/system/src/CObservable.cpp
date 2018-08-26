/* +------------------------------------------------------------------------+
   |                     Mobile Robot Programming Toolkit (MRPT)            |
   |                          http://www.mrpt.org/                          |
   |                                                                        |
   | Copyright (c) 2005-2018, Individual contributors, see AUTHORS file     |
   | See: http://www.mrpt.org/Authors - All rights reserved.                |
   | Released under BSD License. See details in http://www.mrpt.org/License |
   +------------------------------------------------------------------------+ */

#include "system-precomp.h"  // Precompiled headers

#include <mrpt/system/CObserver.h>
#include <mrpt/system/CObservable.h>
#include <mrpt/core/exceptions.h>

using namespace mrpt::system;
using namespace std;

CObservable::CObservable() {}
CObservable::~CObservable()
{
	// Notify my destruction:
	this->publishEvent(mrptEventOnDestroy(this));

	// Tell observers to unsubscribe:
	while (!m_subscribers.empty()) (*m_subscribers.begin())->observeEnd(*this);
}

void CObservable::internal_observer_begin(CObserver* o)
{
	m_subscribers.insert(o);
}

void CObservable::internal_observer_end(CObserver* o)
{
	MRPT_START
	auto it = m_subscribers.find(o);
	ASSERTMSG_(
		it != m_subscribers.end(),
		"Ending subscription from an observer not subscribed to this object!");
	m_subscribers.erase(it);
	MRPT_END
}

/** Called when you want this object to emit an event to all the observers
 * currently subscribed to this object.
 */
void CObservable::publishEvent(const mrptEvent& e) const
{
	MRPT_START
	for (auto& s : m_subscribers)
		if (s) s->internal_on_event(e);
	MRPT_END
}