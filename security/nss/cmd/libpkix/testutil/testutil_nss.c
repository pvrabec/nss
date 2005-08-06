/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is the Netscape security libraries.
 *
 * The Initial Developer of the Original Code is
 * Netscape Communications Corporation.
 * Portions created by the Initial Developer are Copyright (C) 1994-2000
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *   Sun Microsystems
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */
/*
 * testutil_nss.c
 *
 * NSS-specific utility functions for handling test errors
 *
 */

#include <stdio.h>
#include <string.h>
#include <stddef.h>

#include "pkix_pl_generalname.h"
#include "pkix_pl_cert.h"
#include "pkix.h"
#include "testutil.h"
#include "prlong.h"
#include "plstr.h"
#include "prthread.h"
#include "secutil.h"
#include "nspr.h"
#include "prtypes.h"
#include "prtime.h"
#include "pk11func.h"
#include "secasn1.h"
#include "cert.h"
#include "cryptohi.h"
#include "secoid.h"
#include "certdb.h"
#include "secitem.h"
#include "keythi.h"
#include "nss.h"

PKIX_PL_Cert *
createCert(
        char *certFileName,
        void *plContext)
{
        PKIX_PL_ByteArray *byteArray = NULL;
        void *buf = NULL;
        PRFileDesc *certFile = NULL;
        PKIX_UInt32 len;
        SECItem certDER;
        SECStatus rv;
        /* default: NULL cert (failure case) */
        PKIX_PL_Cert *cert = NULL;

        PKIX_TEST_STD_VARS();

        certDER.data = NULL;

        certFile = PR_Open(certFileName, PR_RDONLY, 0);

        if (!certFile){
                pkixTestErrorMsg = "Unable to open cert file";
                goto cleanup;
        } else {
                rv = SECU_ReadDERFromFile(&certDER, certFile, PR_FALSE);
                if (!rv){
                        buf = (void *)certDER.data;
                        len = certDER.len;

                        PKIX_TEST_EXPECT_NO_ERROR
                                (PKIX_PL_ByteArray_Create
                                (buf, len, &byteArray, plContext));

                        PKIX_TEST_EXPECT_NO_ERROR(PKIX_PL_Cert_Create
                                                (byteArray, &cert, plContext));

                        SECITEM_FreeItem(&certDER, PR_FALSE);
                } else {
                        pkixTestErrorMsg = "Unable to read DER from cert file";
                        goto cleanup;
                }
        }

cleanup:

        if (certFile){
                PR_Close(certFile);
        }

        if (PKIX_TEST_ERROR_RECEIVED){
                SECITEM_FreeItem(&certDER, PR_FALSE);
        }

        PKIX_TEST_DECREF_AC(byteArray);

        PKIX_TEST_RETURN();

        return (cert);
}

PKIX_PL_CRL *
createCRL(
        char *crlFileName,
        void *plContext)
{
        PKIX_PL_ByteArray *byteArray = NULL;
        PKIX_PL_CRL *crl = NULL;
        PKIX_Error *error = NULL;
        PRFileDesc *inFile = NULL;
        SECItem crlDER;
        void *buf = NULL;
        PKIX_UInt32 len;
        SECStatus rv;

        PKIX_TEST_STD_VARS();

        crlDER.data = NULL;

        inFile = PR_Open(crlFileName, PR_RDONLY, 0);

        if (!inFile){
                pkixTestErrorMsg = "Unable to open crl file";
                goto cleanup;
        } else {
                rv = SECU_ReadDERFromFile(&crlDER, inFile, PR_FALSE);
                if (!rv){
                        buf = (void *)crlDER.data;
                        len = crlDER.len;

                        error = PKIX_PL_ByteArray_Create
                                (buf, len, &byteArray, plContext);

                        if (error){
                                pkixTestErrorMsg =
                                        "PKIX_PL_ByteArray_Create failed";
                                goto cleanup;
                        }

                        error = PKIX_PL_CRL_Create(byteArray, &crl, plContext);
                        if (error){
                                pkixTestErrorMsg = "PKIX_PL_Crl_Create failed";
                                goto cleanup;
                        }

                        SECITEM_FreeItem(&crlDER, PR_FALSE);
                } else {
                        pkixTestErrorMsg = "Unable to read DER from crl file";
                        goto cleanup;
                }
        }

cleanup:

        if (inFile){
                PR_Close(inFile);
        }

        if (error){
                SECITEM_FreeItem(&crlDER, PR_FALSE);
        }

        PKIX_TEST_DECREF_AC(byteArray);

        PKIX_TEST_RETURN();

        return (crl);

}

PKIX_TrustAnchor *
createTrustAnchor(
        char *certFileName,
        PKIX_Boolean useCert,
        void *plContext)
{
        PKIX_TrustAnchor *anchor = NULL;
        PKIX_PL_Cert *cert = NULL;
        PKIX_PL_X500Name *name = NULL;
        PKIX_PL_PublicKey *pubKey = NULL;
        PKIX_PL_CertNameConstraints *nameConstraints = NULL;

        PKIX_TEST_STD_VARS();

        cert = createCert(certFileName, plContext);

        if (useCert){
                PKIX_TEST_EXPECT_NO_ERROR(PKIX_TrustAnchor_CreateWithCert
                                        (cert, &anchor, plContext));
        } else {
                PKIX_TEST_EXPECT_NO_ERROR(PKIX_PL_Cert_GetSubject
                                        (cert, &name, plContext));

                if (name == NULL){
                        goto cleanup;
                }

                PKIX_TEST_EXPECT_NO_ERROR(PKIX_PL_Cert_GetSubjectPublicKey
                                        (cert, &pubKey, plContext));

                PKIX_TEST_EXPECT_NO_ERROR(PKIX_PL_Cert_GetNameConstraints
                                        (cert, &nameConstraints, NULL));

                PKIX_TEST_EXPECT_NO_ERROR
                        (PKIX_TrustAnchor_CreateWithNameKeyPair
                        (name, pubKey, nameConstraints, &anchor, plContext));
        }

cleanup:

        if (PKIX_TEST_ERROR_RECEIVED){
                PKIX_TEST_DECREF_AC(anchor);
        }

        PKIX_TEST_DECREF_AC(cert);
        PKIX_TEST_DECREF_AC(name);
        PKIX_TEST_DECREF_AC(pubKey);
        PKIX_TEST_DECREF_AC(nameConstraints);

        PKIX_TEST_RETURN();

        return (anchor);
}

PKIX_CertChain *
createCertChain(
        char *firstCertFileName,
        char *secondCertFileName,
        void *plContext)
{
        PKIX_PL_Cert *firstCert = NULL;
        PKIX_PL_Cert *secondCert = NULL;
        PKIX_List *certList = NULL;
        PKIX_CertChain *certChain = NULL;

        PKIX_TEST_STD_VARS();

        PKIX_TEST_EXPECT_NO_ERROR(PKIX_List_Create(&certList, plContext));

        firstCert = createCert(firstCertFileName, plContext);

        PKIX_TEST_EXPECT_NO_ERROR(PKIX_List_AppendItem
                (certList, (PKIX_PL_Object *)firstCert, plContext));

        if (secondCertFileName){
                secondCert = createCert(secondCertFileName, plContext);

                PKIX_TEST_EXPECT_NO_ERROR(PKIX_List_AppendItem
                        (certList, (PKIX_PL_Object *)secondCert, plContext));
        }

        PKIX_TEST_EXPECT_NO_ERROR(PKIX_CertChain_Create
                                    (certList, &certChain, plContext));

cleanup:

        if (PKIX_TEST_ERROR_RECEIVED){
                PKIX_TEST_DECREF_AC(certChain);
        }

        PKIX_TEST_DECREF_AC(firstCert);
        PKIX_TEST_DECREF_AC(secondCert);
        PKIX_TEST_DECREF_AC(certList);

        PKIX_TEST_RETURN();

        return (certChain);
}

PKIX_CertChain *
createCertChainPlus(
        char *certNames[],
        PKIX_PL_Cert *certs[],
        PKIX_UInt32 numCerts,
        void *plContext)
{
        PKIX_List *certList = NULL;
        PKIX_CertChain *certChain = NULL;
        PKIX_UInt32 i;

        PKIX_TEST_STD_VARS();

        PKIX_TEST_EXPECT_NO_ERROR(PKIX_List_Create(&certList, plContext));

        for (i = 0; i < numCerts; i++) {

                certs[i] = createCert(certNames[i], plContext);

                /* Create Cert may fail */
                if (certs[i] == NULL) {
                        PKIX_TEST_DECREF_BC(certList);
                        goto cleanup;
                }

                PKIX_TEST_EXPECT_NO_ERROR(PKIX_List_AppendItem
                                            (certList,
                                            (PKIX_PL_Object *)certs[i],
                                            plContext));
        }

        PKIX_TEST_EXPECT_NO_ERROR(PKIX_CertChain_Create
                                    (certList, &certChain, plContext));


cleanup:

        if (PKIX_TEST_ERROR_RECEIVED){
                PKIX_TEST_DECREF_AC(certChain);
        }

        for (i = 0; i < numCerts; i++) {
                PKIX_TEST_DECREF_AC(certs[i]);
        }

        PKIX_TEST_DECREF_AC(certList);

        PKIX_TEST_RETURN();

        return (certChain);


}

PKIX_PL_Date *
createDate(
        char *asciiDate,
        void *plContext)
{
        PKIX_PL_Date *date = NULL;
        PKIX_PL_String *plString = NULL;

        PKIX_TEST_STD_VARS();

        PKIX_TEST_EXPECT_NO_ERROR(PKIX_PL_String_Create
            (PKIX_ESCASCII, asciiDate, 0, &plString, plContext));

        PKIX_TEST_EXPECT_NO_ERROR(PKIX_PL_Date_Create_UTCTime
                                    (plString, &date, plContext));

cleanup:

        PKIX_TEST_DECREF_AC(plString);

        PKIX_TEST_RETURN();

        return (date);
}

PKIX_ProcessingParams *
createProcessingParams(
        char *firstAnchorFileName,
        char *secondAnchorFileName,
        char *dateAscii,
        PKIX_Boolean isCrlEnabled,
        void *plContext)
{

        PKIX_TrustAnchor *firstAnchor = NULL;
        PKIX_TrustAnchor *secondAnchor = NULL;
        PKIX_List *anchorsList = NULL;
        PKIX_ProcessingParams *procParams = NULL;
        PKIX_PL_String *dateString = NULL;
        PKIX_PL_Date *testDate = NULL;

        PKIX_TEST_STD_VARS();

        PKIX_TEST_EXPECT_NO_ERROR(PKIX_List_Create(&anchorsList, plContext));

        firstAnchor = createTrustAnchor
                (firstAnchorFileName, PKIX_FALSE, plContext);

        PKIX_TEST_EXPECT_NO_ERROR(PKIX_List_AppendItem
                                    (anchorsList,
                                    (PKIX_PL_Object *)firstAnchor,
                                    plContext));

        if (secondAnchorFileName){
                secondAnchor =
                        createTrustAnchor
                        (secondAnchorFileName, PKIX_FALSE, plContext);

                PKIX_TEST_EXPECT_NO_ERROR(PKIX_List_AppendItem
                                            (anchorsList,
                                            (PKIX_PL_Object *)secondAnchor,
                                            plContext));
        }

        PKIX_TEST_EXPECT_NO_ERROR(PKIX_ProcessingParams_Create
                                    (anchorsList, &procParams, plContext));

        if (dateAscii){
                PKIX_TEST_EXPECT_NO_ERROR
                        (PKIX_PL_String_Create
                        (PKIX_ESCASCII,
                        dateAscii,
                        0,
                        &dateString,
                        plContext));

                PKIX_TEST_EXPECT_NO_ERROR
                        (PKIX_PL_Date_Create_UTCTime
                        (dateString, &testDate, plContext));

                PKIX_TEST_EXPECT_NO_ERROR
                        (PKIX_ProcessingParams_SetDate
                        (procParams, testDate, plContext));
        }

        PKIX_TEST_EXPECT_NO_ERROR(PKIX_ProcessingParams_SetRevocationEnabled
                                    (procParams, isCrlEnabled, plContext));

cleanup:

        if (PKIX_TEST_ERROR_RECEIVED){
                PKIX_TEST_DECREF_AC(procParams);
        }

        PKIX_TEST_DECREF_AC(dateString);
        PKIX_TEST_DECREF_AC(testDate);
        PKIX_TEST_DECREF_AC(anchorsList);
        PKIX_TEST_DECREF_AC(firstAnchor);
        PKIX_TEST_DECREF_AC(secondAnchor);

        PKIX_TEST_RETURN();

        return (procParams);
}

PKIX_ValidateParams *
createValidateParams(
        char *firstAnchorFileName,
        char *secondAnchorFileName,
        char *dateAscii,
        PKIX_List *initialPolicies, /* List of PKIX_PL_OID */
        PKIX_Boolean initialPolicyMappingInhibit,
        PKIX_Boolean initialAnyPolicyInhibit,
        PKIX_Boolean initialExplicitPolicy,
        PKIX_Boolean isCrlEnabled,
        PKIX_CertChain *chain,
        void *plContext)
{

        PKIX_ProcessingParams *procParams = NULL;
        PKIX_ValidateParams *valParams = NULL;

        PKIX_TEST_STD_VARS();

        procParams =
                createProcessingParams
                    (firstAnchorFileName,
                    secondAnchorFileName,
                    dateAscii,
                    isCrlEnabled,
                    plContext);

        PKIX_TEST_EXPECT_NO_ERROR(PKIX_ProcessingParams_SetInitialPolicies
                (procParams, initialPolicies, plContext));

        PKIX_TEST_EXPECT_NO_ERROR
                (PKIX_ProcessingParams_SetPolicyMappingInhibited
                (procParams, initialPolicyMappingInhibit, NULL));

        PKIX_TEST_EXPECT_NO_ERROR(PKIX_ProcessingParams_SetAnyPolicyInhibited
                (procParams, initialAnyPolicyInhibit, NULL));

        PKIX_TEST_EXPECT_NO_ERROR
                (PKIX_ProcessingParams_SetExplicitPolicyRequired
                (procParams, initialExplicitPolicy, NULL));

        PKIX_TEST_EXPECT_NO_ERROR(PKIX_ValidateParams_Create
                                    (procParams, chain, &valParams, plContext));

cleanup:

        if (PKIX_TEST_ERROR_RECEIVED){
                PKIX_TEST_DECREF_AC(valParams);
        }

        PKIX_TEST_DECREF_AC(procParams);

        PKIX_TEST_RETURN();

        return (valParams);
}

PKIX_ValidateResult *
createValidateResult(
        char *anchorFileName,
        char *pubKeyCertFileName,
        void *plContext)
{

        PKIX_TrustAnchor *anchor = NULL;
        PKIX_ValidateResult *valResult = NULL;
        PKIX_PL_Cert *cert = NULL;
        PKIX_PL_PublicKey *pubKey = NULL;

        PKIX_TEST_STD_VARS();

        anchor = createTrustAnchor(anchorFileName, PKIX_FALSE, plContext);
        cert = createCert(pubKeyCertFileName, plContext);

        PKIX_TEST_EXPECT_NO_ERROR(PKIX_PL_Cert_GetSubjectPublicKey
                                    (cert, &pubKey, plContext));

        PKIX_TEST_EXPECT_NO_ERROR
                (pkix_ValidateResult_Create
                (pubKey, anchor, NULL, &valResult, plContext));

cleanup:

        if (PKIX_TEST_ERROR_RECEIVED){
                PKIX_TEST_DECREF_AC(valResult);
        }

        PKIX_TEST_DECREF_AC(anchor);
        PKIX_TEST_DECREF_AC(cert);
        PKIX_TEST_DECREF_AC(pubKey);

        PKIX_TEST_RETURN();

        return (valResult);
}

PKIX_PL_GeneralName *
createGeneralName(
        PKIX_UInt32 nameType,
        char *asciiName,
        void *plContext)
{

        PKIX_PL_GeneralName *generalName = NULL;
        PKIX_PL_String *plString = NULL;

        PKIX_TEST_STD_VARS();

        PKIX_TEST_EXPECT_NO_ERROR(PKIX_PL_String_Create
            (PKIX_ESCASCII, asciiName, 0, &plString, plContext));

        PKIX_TEST_EXPECT_NO_ERROR(PKIX_PL_GeneralName_Create
            (nameType, plString, &generalName, plContext));

cleanup:

        PKIX_TEST_DECREF_AC(plString);

        PKIX_TEST_RETURN();

        return (generalName);
}

PKIX_BuildParams *
createBuildParams(
        char *firstAnchorFileName,
        char *secondAnchorFileName,
        char *dateAscii,
        PKIX_List *initialPolicies, /* List of PKIX_PL_OID */
        PKIX_Boolean isCrlEnabled,
        void *plContext)
{
        PKIX_ProcessingParams *procParams = NULL;
        PKIX_BuildParams *buildParams = NULL;

        PKIX_TEST_STD_VARS();

        procParams =
            createProcessingParams
                (firstAnchorFileName,
                secondAnchorFileName,
                dateAscii,
                isCrlEnabled,
                plContext);

        PKIX_TEST_EXPECT_NO_ERROR(PKIX_ProcessingParams_SetInitialPolicies
                (procParams, initialPolicies, plContext));

        PKIX_TEST_EXPECT_NO_ERROR(PKIX_BuildParams_Create
                (procParams, &buildParams, plContext));

cleanup:

        if (PKIX_TEST_ERROR_RECEIVED){
                PKIX_TEST_DECREF_AC(buildParams);
        }

        PKIX_TEST_DECREF_AC(procParams);

        PKIX_TEST_RETURN();

        return (buildParams);
}

PKIX_BuildResult *
createBuildResult(
        char *anchorFileName,
        char *pubKeyCertFileName,
        char *firstChainCertFileName,
        char *secondChainCertFileName,
        void *plContext)
{
        PKIX_BuildResult *buildResult = NULL;
        PKIX_ValidateResult *valResult = NULL;
        PKIX_CertChain *certChain = NULL;

        PKIX_TEST_STD_VARS();

        valResult = createValidateResult
                (anchorFileName, pubKeyCertFileName, plContext);
        certChain = createCertChain
                (firstChainCertFileName, secondChainCertFileName, plContext);

        PKIX_TEST_EXPECT_NO_ERROR
                (pkix_BuildResult_Create
                (valResult, certChain, &buildResult, plContext));

cleanup:

        if (PKIX_TEST_ERROR_RECEIVED){
                PKIX_TEST_DECREF_AC(buildResult);
        }

        PKIX_TEST_DECREF_AC(valResult);
        PKIX_TEST_DECREF_AC(certChain);

        PKIX_TEST_RETURN();

        return (buildResult);
}
