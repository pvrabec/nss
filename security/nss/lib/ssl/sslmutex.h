/*
 * The contents of this file are subject to the Mozilla Public
 * License Version 1.1 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy of
 * the License at http://www.mozilla.org/MPL/
 * 
 * Software distributed under the License is distributed on an "AS
 * IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * rights and limitations under the License.
 * 
 * The Original Code is the Netscape security libraries.
 * 
 * The Initial Developer of the Original Code is Netscape
 * Communications Corporation.  Portions created by Netscape are 
 * Copyright (C) 2001 Netscape Communications Corporation.  All
 * Rights Reserved.
 * 
 * Contributor(s): 
 * 
 * Alternatively, the contents of this file may be used under the
 * terms of the GNU General Public License Version 2 or later (the
 * "GPL"), in which case the provisions of the GPL are applicable 
 * instead of those above.  If you wish to allow use of your 
 * version of this file only under the terms of the GPL and not to
 * allow others to use your version of this file under the MPL,
 * indicate your decision by deleting the provisions above and
 * replace them with the notice and other provisions required by
 * the GPL.  If you do not delete the provisions above, a recipient
 * may use your version of this file under either the MPL or the
 * GPL.
 *
 * $Id$
 */
#ifndef __SSLMUTEX_H_
#define __SSLMUTEX_H_ 1

/* What SSL really wants is portable process-shared unnamed mutexes in 
 * shared memory, that have the property that if the process that holds
 * them dies, they are released automatically, and that (unlike fcntl 
 * record locking) lock to the thread, not to the process.  
 * NSPR doesn't provide that.  
 * Windows has mutexes that meet that description, but they're not portable.  
 * POSIX mutexes are not automatically released when the holder dies, 
 * and other processes/threads cannot release the mutex on behalf of the 
 * dead holder.  
 * POSIX semaphores can be used to accomplish this on systems that implement 
 * process-shared unnamed POSIX semaphores, because a watchdog thread can 
 * discover and release semaphores that were held by a dead process.  
 * On systems that do not support process-shared POSIX unnamed semaphores, 
 * they can be emulated using pipes.  
 * The performance cost of doing that is not yet measured.
 *
 * So, this API looks a lot like POSIX pthread mutexes.
 */

#if defined(WIN32)

#include <wtypes.h>
typedef HANDLE sslMutex;
typedef int    sslPID;

#elif defined(LINUX) || defined(AIX)

#include <sys/types.h>

typedef struct { int ps[3]; } sslMutex;
typedef pid_t sslPID;

#elif defined(XP_UNIX) /* other types of Unix */

#include <sys/types.h>	/* for pid_t */
#include <semaphore.h>  /* for sem_t, and sem_* functions */

typedef sem_t sslMutex;
typedef pid_t sslPID;

#else

/* what platform is this ?? */

typedef int sslMutex;
typedef int sslPID;

#endif

#include "seccomon.h"

SEC_BEGIN_PROTOS

extern SECStatus sslMutex_Init(sslMutex *sem, int shared);

extern SECStatus sslMutex_Destroy(sslMutex *sem);

extern SECStatus sslMutex_Unlock(sslMutex *sem);

extern SECStatus sslMutex_Lock(sslMutex *sem);

SEC_END_PROTOS

#endif
