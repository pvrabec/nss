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
#ifndef SSLT_H
#define SSLT_H


extern int Error(int);
#ifndef DEBUG_stevep
#define dbmsg(x) if (debug) PR_fprintf x ;
#else
#define dbmsg(x) ;
#endif
extern PRInt32 debug;

#ifdef INSERT_TABLES
int testId = 0;

int ClientCert[] = {
	NO_CERT,
	CLIENT_CERT_HARDCOREII_1024
};

int ServerCert[] = {
	SERVER_CERT_HARDCOREII_512
};

int Policy[] = {
	POLICY_EXPORT,
	POLICY_DOMESTIC
};

int SSLVersion2[] = {
	PR_TRUE,
	PR_FALSE
};

int SSLVersion3[] = {
	PR_TRUE,
	PR_FALSE
};

int ClientIOSessionLength[] = {
	0,
	1024
};

int ServerIOSessionLength[] = {
	0,
	1024
};

int ServerDoClientAuth[] = {
	SSLT_CLIENTAUTH_OFF
};

PRBool ClientForceHandshake[] = {
	PR_FALSE
};

PRBool ServerForceHandshake[] = {
	PR_TRUE
};

PRBool ClientRedoHandshake[] = {
	PR_FALSE
};

PRBool ServerRedoHandshake[] = {
	PR_FALSE
};

PRBool Cipher_EN_RC4_128_WITH_MD5[] = {
	PR_TRUE,
	PR_FALSE
};

PRBool Cipher_EN_RC4_128_EXPORT40_WITH_MD5[] = {
	PR_TRUE,
	PR_FALSE
};

PRBool Cipher_EN_RC2_128_CBC_WITH_MD5[] = {
	PR_TRUE,
	PR_FALSE
};

PRBool Cipher_EN_DES_192_EDE3_CBC_WITH_MD5[] = {
	PR_TRUE,
	PR_FALSE
};

PRBool Cipher_RSA_EXPORT_WITH_RC4_40_MD5[] = {
	PR_TRUE,
	PR_FALSE
};

PRBool Cipher_RSA_WITH_RC4_128_MD5[] = {
	PR_TRUE,
	PR_FALSE
};

PRBool Cipher_RSA_EXPORT_WITH_RC2_CBC_40_MD5[] = {
	PR_TRUE,
	PR_FALSE
};

PRBool Cipher_RSA_WITH_DES_CBC_SHA[] = {
	PR_TRUE,
	PR_FALSE
};

PRBool Cipher_RSA_WITH_3DES_EDE_CBC_SHA[] = {
	PR_TRUE,
	PR_FALSE
};


#else
extern int ClientCert[];
extern int ServerCert[];
extern int Policy[];
extern int SSLVersion2[];
extern int SSLVersion3[];
extern int ClientIOSessionLength[];
extern int ServerIOSessionLength[];
extern int ServerDoClientAuth[];
extern PRBool ClientForceHandshake[];
extern PRBool ServerForceHandshake[];
extern PRBool ClientRedoHandshake[];
extern PRBool ServerRedoHandshake[];
extern PRBool Cipher_EN_RC4_128_WITH_MD5[];
extern PRBool Cipher_EN_RC4_128_EXPORT40_WITH_MD5[];
extern PRBool Cipher_EN_RC2_128_CBC_WITH_MD5[];
extern PRBool Cipher_EN_DES_192_EDE3_CBC_WITH_MD5[];
extern PRBool Cipher_RSA_EXPORT_WITH_RC4_40_MD5[];
extern PRBool Cipher_RSA_WITH_RC4_128_MD5[];
extern PRBool Cipher_RSA_EXPORT_WITH_RC2_CBC_40_MD5[];
extern PRBool Cipher_RSA_WITH_DES_CBC_SHA[];
extern PRBool Cipher_RSA_WITH_3DES_EDE_CBC_SHA[];
extern int testId;
#endif


#define REP_ClientCert            ClientCert[testId%2]
#define REP_ServerCert            ServerCert[testId/2%1]
#define REP_Policy                Policy[testId/2/1%2]
#define REP_SSLVersion2           SSLVersion2[testId/2/1/2%2]
#define REP_SSLVersion3           SSLVersion3[testId/2/1/2/2%2]
#define REP_ClientIOSessionLength ClientIOSessionLength[testId/2/1/2/2/2%2]
#define REP_ServerIOSessionLength ServerIOSessionLength[testId/2/1/2/2/2/2%2]
#define REP_ServerDoClientAuth    ServerDoClientAuth[testId/2/1/2/2/2/2/2%1]
#define REP_ClientForceHandshake  ClientForceHandshake[testId/2/1/2/2/2/2/2/1%1]
#define REP_ServerForceHandshake  ServerForceHandshake[testId/2/1/2/2/2/2/2/1/1%1]
#define REP_ClientRedoHandshake   ClientRedoHandshake[testId/2/1/2/2/2/2/2/1/1/1%1]
#define REP_ServerRedoHandshake   ServerRedoHandshake[testId/2/1/2/2/2/2/2/1/1/1/1%1]


#define REP_Cipher_EN_RC4_128_WITH_MD5                    Cipher_EN_RC4_128_WITH_MD5[testId/2/1/2/2/2/2/2/1/1/1/1/1%2]
#define REP_Cipher_EN_RC4_128_EXPORT40_WITH_MD5           Cipher_EN_RC4_128_EXPORT40_WITH_MD5[testId/2/1/2/2/2/2/2/1/1/1/1/1/2%2]
#define REP_Cipher_EN_RC2_128_CBC_WITH_MD5                Cipher_EN_RC2_128_CBC_WITH_MD5[testId/2/1/2/2/2/2/2/1/1/1/1/1/2/2%2]
#ifdef undef
#define REP_Cipher_EN_RC2_128_CBC_EXPORT40_WITH_MD5       $[Cipher_EN_RC2_128_CBC_EXPORT40_WITH_MD5]
#define REP_Cipher_EN_IDEA_128_CBC_WITH_MD5               $[Cipher_EN_IDEA_128_CBC_WITH_MD5]
#define REP_Cipher_EN_DES_64_CBC_WITH_MD5                 $[Cipher_EN_DES_64_CBC_WITH_MD5
#define REP_Cipher_RSA_WITH_NULL_SHA                      $[Cipher_RSA_WITH_NULL_SHA]
#define REP_Cipher_RSA_WITH_RC4_128_SHA                   $[Cipher_RSA_WITH_RC4_128_SHA]
#define REP_Cipher_RSA_WITH_IDEA_CBC_SHA                  $[Cipher_RSA_WITH_IDEA_CBC_SHA]
#define REP_Cipher_NULL_WITH_NULL_NULL                    $[Cipher_NULL_WITH_NULL_NULL]
#define REP_Cipher_RSA_WITH_NULL_MD5                      $[Cipher_RSA_WITH_NULL_MD5]
#endif
#define REP_Cipher_EN_DES_192_EDE3_CBC_WITH_MD5           Cipher_EN_DES_192_EDE3_CBC_WITH_MD5[testId/2/1/2/2/2/2/2/1/1/1/1/1/2/2/2%2]
#define REP_Cipher_RSA_EXPORT_WITH_RC4_40_MD5             Cipher_RSA_EXPORT_WITH_RC4_40_MD5[testId/2/1/2/2/2/2/2/1/1/1/1/1/2/2/2/2%2]
#define REP_Cipher_RSA_WITH_RC4_128_MD5                   Cipher_RSA_WITH_RC4_128_MD5[testId/2/1/2/2/2/2/2/1/1/1/1/1/2/2/2/2/2%2]
#define REP_Cipher_RSA_EXPORT_WITH_RC2_CBC_40_MD5         Cipher_RSA_EXPORT_WITH_RC2_CBC_40_MD5[testId/2/1/2/2/2/2/2/1/1/1/1/1/2/2/2/2/2/2%2]
#define REP_Cipher_RSA_WITH_DES_CBC_SHA                   Cipher_RSA_WITH_DES_CBC_SHA[testId/2/1/2/2/2/2/2/1/1/1/1/1/2/2/2/2/2/2/2%2]
#define REP_Cipher_RSA_WITH_3DES_EDE_CBC_SHA              Cipher_RSA_WITH_3DES_EDE_CBC_SHA[testId/2/1/2/2/2/2/2/1/1/1/1/1/2/2/2/2/2/2/2/2%2]

#ifdef undef
#define REP_Cipher_RSA_EXPORT_WITH_DES40_CBC_SHA          $[Cipher_RSA_EXPORT_WITH_DES40_CBC_SHA]

#define REP_Cipher_DH_DSS_EXPORT_WITH_DES40_CBC_SHA       [Cipher_DH_DSS_EXPORT_WITH_DES40_CBC_SHA]
#define REP_Cipher_DH_DSS_WITH_DES_CBC_SHA                [Cipher_DH_DSS_WITH_DES_CBC_SHA]
#define REP_Cipher_DH_DSS_WITH_3DES_EDE_CBC_SHA           [Cipher_DH_DSS_WITH_3DES_EDE_CBC_SHA]
#define REP_Cipher_DH_RSA_EXPORT_WITH_DES40_CBC_SHA       [Cipher_DH_RSA_EXPORT_WITH_DES40_CBC_SHA]
#define REP_Cipher_DH_RSA_WITH_DES_CBC_SHA                [Cipher_DH_RSA_WITH_DES_CBC_SHA]
#define REP_Cipher_DH_RSA_WITH_3DES_EDE_CBC_SHA           [Cipher_DH_RSA_WITH_3DES_EDE_CBC_SHA]
#define REP_Cipher_DHE_DSS_EXPORT_WITH_DES40_CBC_SHA      [Cipher_DHE_DSS_EXPORT_WITH_DES40_CBC_SHA]
#define REP_Cipher_DHE_DSS_WITH_DES_CBC_SHA               [Cipher_DHE_DSS_WITH_DES_CBC_SHA]
#define REP_Cipher_DHE_DSS_WITH_3DES_EDE_CBC_SHA          [Cipher_DHE_DSS_WITH_3DES_EDE_CBC_SHA]
#define REP_Cipher_DHE_RSA_EXPORT_WITH_DES40_CBC_SHA      [Cipher_DHE_RSA_EXPORT_WITH_DES40_CBC_SHA]
#define REP_Cipher_DHE_RSA_WITH_DES_CBC_SHA               [Cipher_DHE_RSA_WITH_DES_CBC_SHA]
#define REP_Cipher_DHE_RSA_WITH_3DES_EDE_CBC_SHA          [Cipher_DHE_RSA_WITH_3DES_EDE_CBC_SHA]
#define REP_Cipher_DH_ANON_EXPORT_WITH_RC4_40_MD5         [Cipher_DH_ANON_EXPORT_WITH_RC4_40_MD5]
#define REP_Cipher_DH_ANON_WITH_RC4_128_MD5               [Cipher_DH_ANON_WITH_RC4_128_MD5]
#define REP_Cipher_DH_ANON_WITH_DES_CBC_SHA               [Cipher_DH_ANON_WITH_DES_CBC_SHA]
#define REP_Cipher_DH_ANON_WITH_3DES_EDE_CBC_SHA          [Cipher_DH_ANON_WITH_3DES_EDE_CBC_SHA]
#define REP_Cipher_FORTEZZA_DMS_WITH_FORTEZZA_CBC_SHA     $[Cipher_FORTEZZA_DMS_WITH_FORTEZZA_CBC_SHA]
#define REP_Cipher_FORTEZZA_DMS_WITH_RC4_128_SHA          $[Cipher_FORTEZZA_DMS_WITH_RC4_128_SHA]
#endif
#define REP_Cipher_RSA_FIPS_WITH_3DES_EDE_CBC_SHA         $[Cipher_RSA_FIPS_WITH_3DES_EDE_CBC_SHA]
#define REP_Cipher_RSA_FIPS_WITH_DES_CBC_SHA              $[Cipher_RSA_FIPS_WITH_DES_CBC_SHA]


#endif




