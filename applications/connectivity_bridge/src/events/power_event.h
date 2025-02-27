/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef _POWER_EVENT_H_
#define _POWER_EVENT_H_

/**
 * @brief Power Event
 * @defgroup power_event Power Event
 * @{
 */

#include <event_manager.h>
#include <event_manager_profiler.h>

#ifdef __cplusplus
extern "C" {
#endif

struct power_down_event {
	struct event_header header;
	bool error;
};

EVENT_TYPE_DECLARE(power_down_event);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* _POWER_EVENT_H_ */
