/* Copyright 1998 by the Massachusetts Institute of Technology.
 *
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright
 * notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting
 * documentation, and that the name of M.I.T. not be used in
 * advertising or publicity pertaining to distribution of the
 * software without specific, written prior permission.
 * M.I.T. makes no representations about the suitability of
 * this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 */

/* This file is part of liblocker. It deals with zephyr subscriptions
 * pertaining to lockers.
 */

static const char rcsid[] = "$Id: zephyr.c,v 1.7 2003-11-04 19:21:12 ghudson Exp $";

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <com_err.h>

#include "locker.h"
#include "locker_private.h"

/* Magic number taken from zctl sources. */
#define ZEPHYR_MAXONEPACKET 7

int locker_do_zsubs(locker_context context, int op)
{
  locker__free_zsubs(context);
  return LOCKER_SUCCESS;
}

int locker__add_zsubs(locker_context context, char **subs, int nsubs)
{
  int newnzsubs = context->nzsubs + nsubs, i;
  char **newzsubs;

  newzsubs = realloc(context->zsubs, newnzsubs * sizeof(char *));
  if (!newzsubs)
    {
      locker__error(context, "Out of memory recording Zephyr subscriptions.\n");
      return LOCKER_ENOMEM;
    }

  context->zsubs = newzsubs;
  for (i = 0; i < nsubs; i++)
    {
      context->zsubs[context->nzsubs + i] = strdup(subs[i]);
      if (!context->zsubs[context->nzsubs + i])
	{
	  locker__error(context, "Out of memory recording Zephyr "
			"subscriptions.\n");
	  while (i--)
	    free(context->zsubs[context->nzsubs + i]);
	  return LOCKER_ENOMEM;
	}
    }
  context->nzsubs += nsubs;

  return LOCKER_SUCCESS;
}

void locker__free_zsubs(locker_context context)
{
  int i;

  for (i = 0; i < context->nzsubs; i++)
    free(context->zsubs[i]);
  free(context->zsubs);

  context->nzsubs = 0;
  context->zsubs = NULL;
}
