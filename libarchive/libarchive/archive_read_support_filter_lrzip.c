/*-
 * Copyright (c) 2003-2007 Tim Kientzle
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
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

#include "archive_platform.h"

#ifdef HAVE_ERRNO_H
#include <errno.h>
#endif
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "archive.h"
#include "archive_private.h"
#include "archive_read_private.h"
#ifdef __APPLE__
#include "archive_check_entitlement.h"
#endif

#define LRZIP_HEADER_MAGIC "LRZI"
#define LRZIP_HEADER_MAGIC_LEN 4

static int	lrzip_bidder_bid(struct archive_read_filter_bidder *,
		    struct archive_read_filter *);
static int	lrzip_bidder_init(struct archive_read_filter *);


static const struct archive_read_filter_bidder_vtable
lrzip_bidder_vtable = {
	.bid = lrzip_bidder_bid,
	.init = lrzip_bidder_init,
};

int
archive_read_support_filter_lrzip(struct archive *_a)
{
	struct archive_read *a = (struct archive_read *)_a;

#ifdef __APPLE__
	if (!archive_allow_entitlement_filter("lrzip")) {
		archive_set_error(_a, ARCHIVE_ERRNO_MISC, "Format not allow-listed in entitlements");
		return ARCHIVE_FATAL;
	}
#endif

	if (__archive_read_register_bidder(a, NULL, "lrzip",
				&lrzip_bidder_vtable) != ARCHIVE_OK)
		return (ARCHIVE_FATAL);

	/* This filter always uses an external program. */
	archive_set_error(_a, ARCHIVE_ERRNO_MISC,
	    "Using external lrzip program for lrzip decompression");
	return (ARCHIVE_WARN);
}

/*
 * Bidder just verifies the header and returns the number of verified bits.
 */
static int
lrzip_bidder_bid(struct archive_read_filter_bidder *self,
    struct archive_read_filter *filter)
{
	const unsigned char *p;
	ssize_t avail, len;
	int i;

	(void)self; /* UNUSED */
	/* Start by looking at the first six bytes of the header, which
	 * is all fixed layout. */
	len = 6;
	p = __archive_read_filter_ahead(filter, len, &avail);
	if (p == NULL || avail == 0)
		return (0);

	if (memcmp(p, LRZIP_HEADER_MAGIC, LRZIP_HEADER_MAGIC_LEN))
		return (0);

	/* current major version is always 0, verify this */
	if (p[LRZIP_HEADER_MAGIC_LEN])
		return 0;
	/* support only v0.6+ lrzip for sanity */
	i = p[LRZIP_HEADER_MAGIC_LEN + 1];
	if ((i < 6) || (i > 10))
		return 0;

	return (int)len;
}

static int
lrzip_bidder_init(struct archive_read_filter *self)
{
	int r;

	r = __archive_read_program(self, "lrzip -d -q");
	/* Note: We set the format here even if __archive_read_program()
	 * above fails.  We do, after all, know what the format is
	 * even if we weren't able to read it. */
	self->code = ARCHIVE_FILTER_LRZIP;
	self->name = "lrzip";
	return (r);
}
