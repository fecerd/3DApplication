module;
#include <thread>
module Time;
using namespace System;
using namespace std::chrono;

void Timer::Start() noexcept {
	m_start = steady_clock::now();
	m_scaledCurrent = m_start;
	m_unscaledCurrent = m_start;
	m_unscaledDelta = nanoseconds(0);
	m_frameCount = 0;
	m_fixedDeltaMod = nanoseconds(0);
}
void Timer::Update(bool callInvoke) noexcept {
	steady_clock::time_point current = steady_clock::now();
	m_unscaledDelta = duration_cast<nanoseconds>(current - m_unscaledCurrent);
	m_unscaledCurrent = current;
	m_scaledDelta = nanoseconds(static_cast<uint64_t>(m_unscaledDelta.count() * m_scale));
	m_scaledCurrent = m_scaledCurrent + m_scaledDelta;
	++m_frameCount;
	nanoseconds fixedDelta = m_fixedDeltaMod + m_scaledDelta;
	m_fixedUpdateCount = (fixedDelta.count() / m_fixedTimeStep.count());
	m_fixedDeltaMod = nanoseconds(fixedDelta.count() - (m_fixedTimeStep.count() * m_fixedUpdateCount));
	if (callInvoke) {
		for (uint64_t i = 0; i < m_fixedUpdateCount; ++i) FixedInvoke();
		Invoke();
	}
}

void Timer::Sleep(milliseconds ms) noexcept {
	std::this_thread::sleep_for(ms);
}
void Timer::Sleep(nanoseconds ns) noexcept {
	std::this_thread::sleep_for(ns);
}
void Timer::Sleep(seconds s) noexcept {
	std::this_thread::sleep_for(s);
}

//void Timer::Update() noexcept {
//	m_unscaledPrev = m_unscaledCurrent;
//	m_unscaledCurrent = steady_clock::now();
//	m_unscaledDelta = duration_cast<nanoseconds>(m_unscaledCurrent - m_unscaledPrev);
//	m_prev = m_current;
//	m_delta = nanoseconds(static_cast<uint64_t>(m_unscaledDelta.count() * m_timeScale));
//	m_current += m_delta;
//	++m_frameCount;
//	Invoke();
//}
//
//void Timer::UpdateAndSleep() noexcept {
//	Update();
//	//if (m_unscaledDelta < nanoseconds(17000000)) std::this_thread::sleep_for(nanoseconds(17000000) - m_unscaledDelta);
//}
//
//void Timer::FixedUpdate() noexcept {
//	m_fixedPrev = m_fixedCurrent;
//	std::chrono::time_point current = steady_clock::now();
//	if (FixedInvokeStep == nanoseconds(0)) {
//		m_fixedCurrent = current;
//		return;
//	}
//	std::chrono::time_point tmp = m_fixedPrev + FixedInvokeStep;
//	if (tmp > current) return;
//	while (tmp < current)
//	{
//		FixedInvoke();
//		tmp += FixedInvokeStep;
//	}
//	m_fixedCurrent = tmp - FixedInvokeStep;
//}
