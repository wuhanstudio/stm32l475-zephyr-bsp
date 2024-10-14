/*
 * Copyright (c) 2024 Nordic Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#if defined(CONFIG_NET_CONNECTION_MANAGER)
void wait_for_wifi(void);
#else
static inline void wait_for_wifi(void) { }
#endif /* CONFIG_NET_CONNECTION_MANAGER */