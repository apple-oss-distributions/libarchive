/*-
 * Copyright (c) 2003-2007 Tim Kientzle
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer
 *    in this position and unchanged.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR(S) ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR(S) BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef ARCHIVE_WRITE_DISK_PRIVATE_H_INCLUDED
#define ARCHIVE_WRITE_DISK_PRIVATE_H_INCLUDED

#ifndef __LIBARCHIVE_BUILD
#error This header is only to be used internally to libarchive.
#endif

#include "archive_platform_acl.h"
#include "archive_acl_private.h"
#include "archive_entry.h"

struct archive_write_disk;

int archive_write_disk_set_acls(struct archive *, int, const char *,
    struct archive_acl *, __LA_MODE_T);

#ifdef __APPLE__
#include "archive_mac.h"
#endif // __APPLE__

#ifdef HAVE_MAC_QUARANTINE

void archive_write_disk_set_quarantine(struct archive *, qtn_file_t qf);
#endif // HAVE_MAC_QUARANTINE

#endif
