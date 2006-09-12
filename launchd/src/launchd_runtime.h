/*
 * Copyright (c) 2005 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_APACHE_LICENSE_HEADER_START@
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 * @APPLE_APACHE_LICENSE_HEADER_END@
 */
#ifndef __LAUNCHD_RUNTIME_H__
#define __LAUNCHD_RUNTIME_H__

#include <mach/mach.h>

/*
 * Use launchd_assumes() when we can recover, even if it means we leak or limp along.
 *
 * Use launchd_assert() for core initialization routines.
 */
#define launchd_assumes(e)	\
	(__builtin_expect(!(e), 0) ? _log_launchd_bug(__rcs_file_version__, __FILE__, __LINE__, #e), false : true)

#define launchd_blame(e, b)	\
	(__builtin_expect(!(e), 0) ? syslog(LOG_DEBUG, "Encountered bug: %d", b), false : true)

#define launchd_assert(e)	launchd_assumes(e) ? true : abort();

void _log_launchd_bug(const char *rcs_rev, const char *path, unsigned int line, const char *test);

typedef void (*kq_callback)(void *, struct kevent *);
typedef boolean_t (*mig_callback)(mach_msg_header_t *, mach_msg_header_t *);
typedef void (*timeout_callback)(void);

boolean_t launchd_internal_demux(mach_msg_header_t *Request, mach_msg_header_t *Reply);

void launchd_runtime_init(void);
void launchd_runtime(void) __attribute__((noreturn));

void runtime_force_on_demand(bool);
void runtime_set_timeout(timeout_callback to_cb, mach_msg_timeout_t to);
kern_return_t runtime_add_mport(mach_port_t name, mig_callback demux, mach_msg_size_t msg_size);
kern_return_t runtime_remove_mport(mach_port_t name);

int kevent_mod(uintptr_t ident, short filter, u_short flags, u_int fflags, intptr_t data, void *udata);

kern_return_t launchd_set_bport(mach_port_t name);
kern_return_t launchd_get_bport(mach_port_t *name);
kern_return_t launchd_mport_notify_req(mach_port_t name, mach_msg_id_t which);
kern_return_t launchd_mport_notify_cancel(mach_port_t name, mach_msg_id_t which);
kern_return_t launchd_mport_create_recv(mach_port_t *name);
kern_return_t launchd_mport_deallocate(mach_port_t name);
kern_return_t launchd_mport_make_send(mach_port_t name);
kern_return_t launchd_mport_close_recv(mach_port_t name);

#endif
