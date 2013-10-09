/* Copyright (C) 1996 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Thorsten Kukuk <kukuk@vt.uni-paderborn.de>, 1996.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the GNU C Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <libc-lock.h>
#include <rpcsvc/yp.h>
#include <rpcsvc/ypclnt.h>
#include <rpcsvc/ypupd.h>

struct dom_binding
  {
    struct dom_binding *dom_pnext;
    char dom_domain[YPMAXDOMAIN + 1];
    struct sockaddr_in dom_server_addr;
    int dom_socket;
    CLIENT *dom_client;
    long int dom_vers;
  };
typedef struct dom_binding dom_binding;

static struct timeval TIMEOUT = {25, 0};
static int const MAXTRIES = 5;
static char __ypdomainname[MAXHOSTNAMELEN + 1] = "\0";
__libc_lock_define_initialized (static, ypbindlist_lock)
static dom_binding *__ypbindlist = NULL;


static int
__yp_bind (const char *domain, dom_binding ** ypdb)
{
  struct sockaddr_in clnt_saddr;
  struct ypbind_resp ypbr;
  dom_binding *ysd;
  int clnt_sock;
  CLIENT *client;
  int is_new = 0;
  int try;

  if (ypdb != NULL)
    *ypdb = NULL;

  if ((domain == NULL) || (strlen (domain) == 0))
    return YPERR_BADARGS;

  ysd = __ypbindlist;
  while (ysd != NULL)
    {
      if (strcmp (domain, ysd->dom_domain) == 0)
        break;
      ysd = ysd->dom_pnext;
    }

  if (ysd == NULL)
    {
      is_new = 1;
      ysd = (dom_binding *) malloc (sizeof *ysd);
      memset (ysd, '\0', sizeof *ysd);
      ysd->dom_socket = -1;
      ysd->dom_vers = -1;
    }

  try = 0;

  do
    {
      try++;
      if (try > MAXTRIES)
        {
          if (is_new)
            free (ysd);
          return YPERR_YPBIND;
        }

      if (ysd->dom_vers == -1)
        {
          if(ysd->dom_client)
            {
              clnt_destroy(ysd->dom_client);
              ysd->dom_client = NULL;
              ysd->dom_socket = -1;
            }
          memset (&clnt_saddr, '\0', sizeof clnt_saddr);
          clnt_saddr.sin_family = AF_INET;
          clnt_saddr.sin_addr.s_addr = htonl (INADDR_LOOPBACK);
          clnt_sock = RPC_ANYSOCK;
          client = clnttcp_create (&clnt_saddr, YPBINDPROG, YPBINDVERS,
                                   &clnt_sock, 0, 0);
          if (client == NULL)
            {
              if (is_new)
                free (ysd);
              return YPERR_YPBIND;
            }
          /*
          ** Check the port number -- should be < IPPORT_RESERVED.
          ** If not, it's possible someone has registered a bogus
          ** ypbind with the portmapper and is trying to trick us.
          */
          if (ntohs(clnt_saddr.sin_port) >= IPPORT_RESERVED)
            {
              clnt_destroy(client);
              if (is_new)
                free(ysd);
              return(YPERR_YPBIND);
            }

          if (clnt_call (client, YPBINDPROC_DOMAIN,
                         (xdrproc_t) xdr_domainname, &domain,
                         (xdrproc_t) xdr_ypbind_resp,
                         &ypbr, TIMEOUT) != RPC_SUCCESS)
            {
              clnt_destroy (client);
              if (is_new)
                free (ysd);
              return YPERR_YPBIND;
            }

          clnt_destroy (client);
          if (ypbr.ypbind_status != YPBIND_SUCC_VAL)
            {
              switch (ypbr.ypbind_resp_u.ypbind_error)
                {
                case YPBIND_ERR_ERR:
                  fputs (_("YPBINDPROC_DOMAIN: Internal error\n"), stderr);
                  break;
                case YPBIND_ERR_NOSERV:
                  fprintf (stderr,
                           _("YPBINDPROC_DOMAIN: No server for domain %s\n"),
                           domain);
                  break;
                case YPBIND_ERR_RESC:
                  fputs (_("YPBINDPROC_DOMAIN: Resource allocation failure\n"),
                         stderr);
                  break;
                default:
                  fputs (_("YPBINDPROC_DOMAIN: Unknown error\n"), stderr);
                  break;
                }
              if (is_new)
                free (ysd);
              return YPERR_DOMAIN;
            }
          memset (&ysd->dom_server_addr, '\0', sizeof ysd->dom_server_addr);
          ysd->dom_server_addr.sin_family = AF_INET;
          memcpy (&ysd->dom_server_addr.sin_port,
                  ypbr.ypbind_resp_u.ypbind_bindinfo.ypbind_binding_port,
                  sizeof (ysd->dom_server_addr.sin_port));
          memcpy (&ysd->dom_server_addr.sin_addr.s_addr,
                  ypbr.ypbind_resp_u.ypbind_bindinfo.ypbind_binding_addr,
                  sizeof (ysd->dom_server_addr.sin_addr.s_addr));
          ysd->dom_vers = YPVERS;
          strcpy (ysd->dom_domain, domain);
        }

      if (ysd->dom_client)
        clnt_destroy (ysd->dom_client);
      ysd->dom_socket = RPC_ANYSOCK;
      ysd->dom_client = clntudp_create (&ysd->dom_server_addr, YPPROG, YPVERS,
                                        TIMEOUT, &ysd->dom_socket);
      if (ysd->dom_client == NULL)
        ysd->dom_vers = -1;

    }
  while (ysd->dom_client == NULL);

  /* If the program exists, close the socket */
  if (fcntl (ysd->dom_socket, F_SETFD, 1) == -1)
    perror (_("fcntl: F_SETFD"));

  if (is_new)
    {
      ysd->dom_pnext = __ypbindlist;
      __ypbindlist = ysd;
    }

  if (NULL != ypdb)
    *ypdb = ysd;

  return YPERR_SUCCESS;
}

static void
__yp_unbind (dom_binding *ydb)
{
  clnt_destroy (ydb->dom_client);
  ydb->dom_client = NULL;
  ydb->dom_socket = -1;
}

static int
do_ypcall (const char *domain, u_long prog, xdrproc_t xargs,
	   caddr_t req, xdrproc_t xres, caddr_t resp)
{
  dom_binding *ydb = NULL;
  int try, result;

  try = 0;
  result = YPERR_YPERR;

  while (try < MAXTRIES && result != RPC_SUCCESS)
    {
      __libc_lock_lock (ypbindlist_lock);

      if (__yp_bind (domain, &ydb) != 0)
	{
	  __libc_lock_unlock (ypbindlist_lock);
	  return YPERR_DOMAIN;
	}

      result = clnt_call (ydb->dom_client, prog,
			  xargs, req, xres, resp, TIMEOUT);

      if (result != RPC_SUCCESS)
	{
	  clnt_perror (ydb->dom_client, "do_ypcall: clnt_call");
	  ydb->dom_vers = -1;
	  __yp_unbind (ydb);
	  result = YPERR_RPC;
	}

      __libc_lock_unlock (ypbindlist_lock);

      try++;
    }

  return result;
}

int
yp_bind (const char *indomain)
{
  int status;

  __libc_lock_lock (ypbindlist_lock);

  status = __yp_bind (indomain, NULL);

  __libc_lock_unlock (ypbindlist_lock);

  return status;
}

void
yp_unbind (const char *indomain)
{
  dom_binding *ydbptr, *ydbptr2;

  __libc_lock_lock (ypbindlist_lock);

  ydbptr2 = NULL;
  ydbptr = __ypbindlist;
  while (ydbptr != NULL)
    {
      if (strcmp (ydbptr->dom_domain, indomain) == 0)
	{
	  dom_binding *work;

	  work = ydbptr;
	  if (ydbptr2 == NULL)
	    __ypbindlist = __ypbindlist->dom_pnext;
	  else
	    ydbptr2 = ydbptr->dom_pnext;
	  __yp_unbind (work);
	  free (work);
	  break;
	}
      ydbptr2 = ydbptr;
      ydbptr = ydbptr->dom_pnext;
    }

  __libc_lock_unlock (ypbindlist_lock);

  return;
}

__libc_lock_define_initialized (static, domainname_lock)

int
yp_get_default_domain (char **outdomain)
{
  int result = YPERR_SUCCESS;;
  *outdomain = NULL;

  __libc_lock_lock (domainname_lock);

  if (__ypdomainname[0] == '\0')
    {
      if (getdomainname (__ypdomainname, MAXHOSTNAMELEN))
	result = YPERR_NODOM;
      else
	*outdomain = __ypdomainname;
    }
  else
    *outdomain = __ypdomainname;

  __libc_lock_unlock (domainname_lock);

  return result;
}

int
__yp_check (char **domain)
{
  char *unused;

  if (__ypdomainname[0] == '\0')
    if (yp_get_default_domain (&unused))
      return 0;
    else if (strcmp (__ypdomainname, "(none)") == 0)
      return 0;

  if (domain)
    *domain = __ypdomainname;

  if (yp_bind (__ypdomainname) == 0)
    return 1;
  return 0;
}

int
yp_match (const char *indomain, const char *inmap, const char *inkey,
	  const int inkeylen, char **outval, int *outvallen)
{
  ypreq_key req;
  ypresp_val resp;
  int result;

  if (indomain == NULL || indomain[0] == '\0' ||
      inmap == NULL || inmap[0] == '\0' ||
      inkey == NULL || inkey[0] == '\0' || inkeylen <= 0)
    return YPERR_BADARGS;

  req.domain = (char *) indomain;
  req.map = (char *) inmap;
  req.key.keydat_val = (char *) inkey;
  req.key.keydat_len = inkeylen;

  *outval = NULL;
  *outvallen = 0;
  memset (&resp, '\0', sizeof (resp));

  result = do_ypcall (indomain, YPPROC_MATCH, (xdrproc_t) xdr_ypreq_key,
		      (caddr_t) & req, (xdrproc_t) xdr_ypresp_val,
		      (caddr_t) & resp);

  if (result != RPC_SUCCESS)
    return result;
  if (resp.stat != YP_TRUE)
    return ypprot_err (resp.stat);

  *outvallen = resp.val.valdat_len;
  *outval = malloc (*outvallen + 1);
  memcpy (*outval, resp.val.valdat_val, *outvallen);
  (*outval)[*outvallen] = '\0';

  xdr_free ((xdrproc_t) xdr_ypresp_val, (char *) &resp);

  return YPERR_SUCCESS;
}

int
yp_first (const char *indomain, const char *inmap, char **outkey,
	  int *outkeylen, char **outval, int *outvallen)
{
  ypreq_nokey req;
  ypresp_key_val resp;
  int result;

  if (indomain == NULL || indomain[0] == '\0' ||
      inmap == NULL || inmap[0] == '\0')
    return YPERR_BADARGS;

  req.domain = (char *) indomain;
  req.map = (char *) inmap;

  *outkey = *outval = NULL;
  *outkeylen = *outvallen = 0;
  memset (&resp, '\0', sizeof (resp));

  result = do_ypcall (indomain, YPPROC_FIRST, (xdrproc_t) xdr_ypreq_nokey,
		      (caddr_t) & req, (xdrproc_t) xdr_ypresp_key_val,
		      (caddr_t) & resp);

  if (result != RPC_SUCCESS)
    return result;
  if (resp.stat != YP_TRUE)
    return ypprot_err (resp.stat);

  *outkeylen = resp.key.keydat_len;
  *outkey = malloc (*outkeylen + 1);
  memcpy (*outkey, resp.key.keydat_val, *outkeylen);
  (*outkey)[*outkeylen] = '\0';
  *outvallen = resp.val.valdat_len;
  *outval = malloc (*outvallen + 1);
  memcpy (*outval, resp.val.valdat_val, *outvallen);
  (*outval)[*outvallen] = '\0';

  xdr_free ((xdrproc_t) xdr_ypresp_key_val, (char *) &resp);

  return YPERR_SUCCESS;
}

int
yp_next (const char *indomain, const char *inmap, const char *inkey,
	 const int inkeylen, char **outkey, int *outkeylen, char **outval,
	 int *outvallen)
{
  ypreq_key req;
  ypresp_key_val resp;
  int result;

  if (indomain == NULL || indomain[0] == '\0' ||
      inmap == NULL || inmap[0] == '\0' ||
      inkeylen <= 0 || inkey == NULL || inkey[0] == '\0')
    return YPERR_BADARGS;

  req.domain = (char *) indomain;
  req.map = (char *) inmap;
  req.key.keydat_val = (char *) inkey;
  req.key.keydat_len = inkeylen;

  *outkey = *outval = NULL;
  *outkeylen = *outvallen = 0;
  memset (&resp, '\0', sizeof (resp));

  result = do_ypcall (indomain, YPPROC_NEXT, (xdrproc_t) xdr_ypreq_key,
		      (caddr_t) & req, (xdrproc_t) xdr_ypresp_key_val,
		      (caddr_t) & resp);

  if (result != RPC_SUCCESS)
    return result;
  if (resp.stat != YP_TRUE)
    return ypprot_err (resp.stat);

  *outkeylen = resp.key.keydat_len;
  *outkey = malloc (*outkeylen + 1);
  memcpy (*outkey, resp.key.keydat_val, *outkeylen);
  (*outkey)[*outkeylen] = '\0';
  *outvallen = resp.val.valdat_len;
  *outval = malloc (*outvallen + 1);
  memcpy (*outval, resp.val.valdat_val, *outvallen);
  (*outval)[*outvallen] = '\0';

  xdr_free ((xdrproc_t) xdr_ypresp_key_val, (char *) &resp);

  return YPERR_SUCCESS;
}

int
yp_master (const char *indomain, const char *inmap, char **outname)
{
  ypreq_nokey req;
  ypresp_master resp;
  int result;

  if (indomain == NULL || indomain[0] == '\0' ||
      inmap == NULL || inmap[0] == '\0')
    return YPERR_BADARGS;

  req.domain = (char *) indomain;
  req.map = (char *) inmap;

  memset (&resp, '\0', sizeof (ypresp_master));

  result = do_ypcall (indomain, YPPROC_MASTER, (xdrproc_t) xdr_ypreq_nokey,
	  (caddr_t) & req, (xdrproc_t) xdr_ypresp_master, (caddr_t) & resp);

  if (result != RPC_SUCCESS)
    return result;
  if (resp.stat != YP_TRUE)
    return ypprot_err (resp.stat);

  *outname = strdup (resp.peer);
  xdr_free ((xdrproc_t) xdr_ypresp_master, (char *) &resp);

  return YPERR_SUCCESS;
}

int
yp_order (const char *indomain, const char *inmap, unsigned int *outorder)
{
  struct ypreq_nokey req;
  struct ypresp_order resp;
  int result;

  if (indomain == NULL || indomain[0] == '\0' ||
      inmap == NULL || inmap == '\0')
    return YPERR_BADARGS;

  req.domain = (char *) indomain;
  req.map = (char *) inmap;

  memset (&resp, '\0', sizeof (resp));

  result = do_ypcall (indomain, YPPROC_ORDER, (xdrproc_t) xdr_ypreq_nokey,
	   (caddr_t) & req, (xdrproc_t) xdr_ypresp_order, (caddr_t) & resp);

  if (result != RPC_SUCCESS)
    return result;
  if (resp.stat != YP_TRUE)
    return ypprot_err (resp.stat);

  *outorder = resp.ordernum;
  xdr_free ((xdrproc_t) xdr_ypresp_order, (char *) &resp);

  return YPERR_SUCCESS;
}

static void *ypall_data;
static int (*ypall_foreach) ();

static bool_t
__xdr_ypresp_all (XDR * xdrs, u_long * objp)
{
  while (1)
    {
      struct ypresp_all resp;

      memset (&resp, '\0', sizeof (struct ypresp_all));
      if (!xdr_ypresp_all (xdrs, &resp))
	{
	  xdr_free ((xdrproc_t) xdr_ypresp_all, (char *) &resp);
	  *objp = YP_YPERR;
	  return (FALSE);
	}
      if (resp.more == 0)
	{
	  xdr_free ((xdrproc_t) xdr_ypresp_all, (char *) &resp);
	  *objp = YP_NOMORE;
	  return (FALSE);
	}

      switch (resp.ypresp_all_u.val.stat)
	{
	case YP_TRUE:
	  {
	    char key[resp.ypresp_all_u.val.key.keydat_len + 1];
	    char val[resp.ypresp_all_u.val.val.valdat_len + 1];
	    int keylen = resp.ypresp_all_u.val.key.keydat_len;
	    int vallen = resp.ypresp_all_u.val.val.valdat_len;

	    *objp = YP_TRUE;
	    memcpy (key, resp.ypresp_all_u.val.key.keydat_val, keylen);
	    key[keylen] = '\0';
	    memcpy (val, resp.ypresp_all_u.val.val.valdat_val, vallen);
	    val[vallen] = '\0';
	    xdr_free ((xdrproc_t) xdr_ypresp_all, (char *) &resp);
	    if ((*ypall_foreach) (*objp, key, keylen,
				  val, vallen, ypall_data))
	      return TRUE;
	  }
	  break;
	case YP_NOMORE:
	  *objp = YP_NOMORE;
	  xdr_free ((xdrproc_t) xdr_ypresp_all, (char *) &resp);
	  return TRUE;
	  break;
	default:
	  *objp = resp.ypresp_all_u.val.stat;
	  xdr_free ((xdrproc_t) xdr_ypresp_all, (char *) &resp);
	  return TRUE;
	}
    }
}

int
yp_all (const char *indomain, const char *inmap,
	const struct ypall_callback *incallback)
{
  struct ypreq_nokey req;
  dom_binding *ydb;
  int try, result;
  struct sockaddr_in clnt_sin;
  CLIENT *clnt;
  unsigned long status;
  int clnt_sock;

  if (indomain == NULL || indomain[0] == '\0' ||
      inmap == NULL || inmap == '\0')
    return YPERR_BADARGS;

  try = 0;
  result = YPERR_YPERR;

  while (try < MAXTRIES && result != RPC_SUCCESS)
    {
      __libc_lock_lock (ypbindlist_lock);

      if (__yp_bind (indomain, &ydb) != 0)
	{
	  __libc_lock_unlock (ypbindlist_lock);
	  return YPERR_DOMAIN;
	}

      /* YPPROC_ALL get its own TCP channel to ypserv */
      clnt_sock = RPC_ANYSOCK;
      clnt_sin = ydb->dom_server_addr;
      clnt_sin.sin_port = 0;
      clnt = clnttcp_create (&clnt_sin, YPPROG, YPVERS, &clnt_sock, 0, 0);
      if (clnt == NULL)
	{
	  puts ("yp_all: clnttcp_create failed");
	  __libc_lock_unlock (ypbindlist_lock);
	  return YPERR_PMAP;
	}
      req.domain = (char *) indomain;
      req.map = (char *) inmap;

      ypall_foreach = incallback->foreach;
      ypall_data = (void *) incallback->data;

      result = clnt_call (clnt, YPPROC_ALL, (xdrproc_t) xdr_ypreq_nokey, &req,
			  (xdrproc_t) __xdr_ypresp_all, &status, TIMEOUT);

      if (result != RPC_SUCCESS)
	{
	  clnt_perror (ydb->dom_client, "yp_all: clnt_call");
	  clnt_destroy (clnt);
	  __yp_unbind (ydb);
	  result = YPERR_RPC;
	}
      else
	{
	  clnt_destroy (clnt);
	  result = YPERR_SUCCESS;
	}

      __libc_lock_unlock (ypbindlist_lock);

      if (status != YP_NOMORE)
	return ypprot_err (status);
      try++;
    }

  return result;
}

int
yp_maplist (const char *indomain, struct ypmaplist **outmaplist)
{
  struct ypresp_maplist resp;
  int result;

  if (indomain == NULL || indomain[0] == '\0')
    return YPERR_BADARGS;

  memset (&resp, '\0', sizeof (resp));

  result = do_ypcall (indomain, YPPROC_MAPLIST, (xdrproc_t) xdr_domainname,
    (caddr_t) & indomain, (xdrproc_t) xdr_ypresp_maplist, (caddr_t) & resp);

  if (result != RPC_SUCCESS)
    return result;
  if (resp.stat != YP_TRUE)
    return ypprot_err (resp.stat);

  *outmaplist = resp.maps;
  /* We give the list not free, this will be done by ypserv
     xdr_free((xdrproc_t)xdr_ypresp_maplist, (char *)&resp); */

  return YPERR_SUCCESS;
}

const char *
yperr_string (const int error)
{
  switch (error)
    {
    case YPERR_SUCCESS:
      return _("Success");
    case YPERR_BADARGS:
      return _("Request arguments bad");
    case YPERR_RPC:
      return _("RPC failure on NIS operation");
    case YPERR_DOMAIN:
      return _("Can't bind to server which serves this domain");
    case YPERR_MAP:
      return _("No such map in server's domain");
    case YPERR_KEY:
      return _("No such key in map");
    case YPERR_YPERR:
      return _("Internal NIS error");
    case YPERR_RESRC:
      return _("Local resource allocation failure");
    case YPERR_NOMORE:
      return _("No more records in map database");
    case YPERR_PMAP:
      return _("Can't communicate with portmapper");
    case YPERR_YPBIND:
      return _("Can't communicate with ypbind");
    case YPERR_YPSERV:
      return _("Can't communicate with ypserv");
    case YPERR_NODOM:
      return _("Local domain name not set");
    case YPERR_BADDB:
      return _("NIS map data base is bad");
    case YPERR_VERS:
      return _("NIS client/server version mismatch - can't supply service");
    case YPERR_ACCESS:
      return _("Permission denied");
    case YPERR_BUSY:
      return _("Database is busy");
    }
  return _("Unknown NIS error code");
}

int
ypprot_err (const int code)
{
  switch (code)
    {
    case YP_TRUE:
      return YPERR_SUCCESS;
    case YP_NOMORE:
      return YPERR_NOMORE;
    case YP_FALSE:
      return YPERR_YPERR;
    case YP_NOMAP:
      return YPERR_MAP;
    case YP_NODOM:
      return YPERR_DOMAIN;
    case YP_NOKEY:
      return YPERR_KEY;
    case YP_BADOP:
      return YPERR_YPERR;
    case YP_BADDB:
      return YPERR_BADDB;
    case YP_YPERR:
      return YPERR_YPERR;
    case YP_BADARGS:
      return YPERR_BADARGS;
    case YP_VERS:
      return YPERR_VERS;
    }
  return YPERR_YPERR;
}

const char *
ypbinderr_string (const int error)
{
  switch (error)
    {
    case 0:
      return _("Success");
    case YPBIND_ERR_ERR:
      return _("Internal ypbind error");
    case YPBIND_ERR_NOSERV:
      return _("Domain not bound");
    case YPBIND_ERR_RESC:
      return _("System resource allocation failure");
    default:
      return _("Unknown ypbind error");
    }
}


#define WINDOW 60

int
yp_update (char *domain, char *map, unsigned ypop,
	   char *key, int keylen, char *data, int datalen)
{
#if 0
  union
    {
      ypupdate_args update_args;
      ypdelete_args delete_args;
    }
  args;
  xdrproc_t xdr_argument;
  unsigned res = 0;
  CLIENT *clnt;
  char *master;
  struct sockaddr saddr;
  char servername[MAXNETNAMELEN + 1];
  int r;

  if (!domain || !map || !key || (ypop != YPOP_DELETE && !data))
    return YPERR_BADARGS;

  args.update_args.mapname = map;
  args.update_args.key.yp_buf_len = keylen;
  args.update_args.key.yp_buf_val = key;
  args.update_args.datum.yp_buf_len = datalen;
  args.update_args.datum.yp_buf_val = data;

  if ((r = yp_master (domain, map, &master)) != 0)
    return r;

  if (!host2netname (servername, master, domain))
    {
      fputs (_("yp_update: cannot convert host to netname\n"), stderr);
      return YPERR_YPERR;
    }

  if ((clnt = clnt_create (master, YPU_PROG, YPU_VERS, "tcp")) == NULL)
    {
      clnt_pcreateerror ("yp_update: clnt_create");
      return YPERR_RPC;
    }

  if (!clnt_control (clnt, CLGET_SERVER_ADDR, (char *) &saddr))
    {
      fputs (_("yp_update: cannot get server address\n"), stderr);
      return YPERR_RPC;
    }

  switch (ypop)
    {
    case YPOP_CHANGE:
    case YPOP_INSERT:
    case YPOP_STORE:
      xdr_argument = (xdrproc_t) xdr_ypupdate_args;
      break;
    case YPOP_DELETE:
      xdr_argument = (xdrproc_t) xdr_ypdelete_args;
      break;
    default:
      return YPERR_BADARGS;
      break;
    }

  clnt->cl_auth = authdes_create (servername, WINDOW, &saddr, NULL);

  if (clnt->cl_auth == NULL)
    clnt->cl_auth = authunix_create_default ();

again:
  r = clnt_call (clnt, ypop, xdr_argument, &args,
		 (xdrproc_t) xdr_u_int, &res, TIMEOUT);

  if (r == RPC_AUTHERROR)
    {
      if (clnt->cl_auth->ah_cred.oa_flavor == AUTH_DES)
	{
	  clnt->cl_auth = authunix_create_default ();
	  goto again;
	}
      else
	return YPERR_ACCESS;
    }
  if (r != RPC_SUCCESS)
    {
      clnt_perror (clnt, "yp_update: clnt_call");
      return YPERR_RPC;
    }
  return res;
#else
  return YPERR_YPERR;
#endif
}
