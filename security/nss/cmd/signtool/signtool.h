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
 * Copyright (C) 1994-2000 Netscape Communications Corporation.  All
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
 */

#ifndef SIGNTOOL_H
#define SIGNTOOL_H
 
#define DJN_TEST

#include <stdio.h>
#include <string.h>

#include "errno.h"
#include "prprf.h"
#include "prio.h"
#include "secutil.h"
#include "jar.h"
#include "jarfile.h"
#include "secpkcs7.h"
#include "pk11func.h"
#include "secmod.h"
#include "plhash.h"

#ifdef _UNIX
#include <unistd.h> 
#endif

/**********************************************************************
 * General Defines
 */
#define JAR_BASE_END JAR_BASE + 100
#define ERRX (-1)	/* the exit code used on failure */
#define FNSIZE 256	/* the maximum length for filenames */
#define MAX_RSA_KEY_SIZE 4096
#define DEFAULT_RSA_KEY_SIZE 1024
#define MANIFEST "manifest.mf"
#define DEFAULT_X509_BASENAME "x509"
#define DEFAULT_COMMON_NAME "Signtool 1.3 Testing Certificate"
#define CREATOR  "Signtool (signtool 1.3)"
#define BREAKAGE "PLEASE DO NOT EDIT THIS FILE. YOU WILL BREAK IT."
#define MIN_COMPRESSION_LEVEL (-1)
#define MAX_COMPRESSION_LEVEL 9
#define DEFAULT_COMPRESSION_LEVEL (-1) /* zlib understands this to be default*/
#define STDIN_BUF_SIZE 160
#define PROGRAM_NAME	"signtool"
#define LONG_PROGRAM_NAME "Signing Tool"
#define DEFAULT_BASE_NAME "zigbert"
#define VERSION		"1.3"
#define TMP_OUTPUT "signtool.tmp"


/***************************************************************
 * Main Task Functions
 */
int GenerateCert(char *nickname, int keysize, char *token);
int ListCerts(char *key, int list_certs);
int VerifyJar(char *filename);
int SignArchive(char *tree, char *keyName, char *zip_file, int javascript,
	char *meta_file, char *install_script, int _optimize, PRBool recurse);
int SignAllArc(char *jartree, char *keyName, int javascript, char *metafile,
	char *install_script, int optimize, PRBool recurse);
int InlineJavaScript(char *dir, PRBool recurse);
int JarWho(char *filename);
void JarListModules(void);

/**************************************************************
 * Utility Functions
 */
CERTCertDBHandle *OpenCertDB (PRBool readOnly);

int RemoveAllArc(char *tree);
void VerifyCertDir(char *dir, char *keyName);
int InitCrypto(char *cert_dir, PRBool readOnly);
int foreach (char *dirname, char *prefix,
	int (*fn)(char *filename, char *dirname, char *basedir,char *base,void*arg),
	PRBool recurse, PRBool includeDirs, void *arg);
void print_error (int i);
void give_help (int status);
const char* secErrorString(long code);
void displayVerifyLog(CERTVerifyLog *log);
void usage (void);
char* chop(char*);
void out_of_memory(void);
void FatalError(char *msg);
char* get_default_cert_dir(void);
SECItem *password_hardcode(void *arg, SECKEYKeyDBHandle *handle);
char* pk11_password_hardcode(PK11SlotInfo *slot, PRBool retry, void *arg);
int rm_dash_r(char *path);
char* pr_fgets(char *buf, int size, PRFileDesc *file);


/*****************************************************************
 * Global Variables (*gag*)
 */
extern char *password;	/* the password passed in on the command line */
extern PLHashTable *excludeDirs;  /* directory entry to skip while recursing */
extern int no_time;
extern char *base;		/* basename of ".rsa" and ".sf" files */
extern long *mozilla_event_queue;
extern char *progName; /* argv[0] */
extern PLHashTable *extensions;/* only sign files with this extension */
extern PRBool extensionsGiven;
extern char *scriptdir;
extern int compression_level;
extern PRFileDesc *outputFD, *errorFD;
extern int verbosity;
extern int errorCount;
extern int warningCount;

#endif /* SIGNTOOL_H */
