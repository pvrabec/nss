/* -*- Mode: C; tab-width: 8 -*-*/
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


/* Header file with all of the structures and types that will be exported 
 * by the security library for implementation of CRMF.
 */

#ifndef _CRMFT_H_
#define _CRMFT_H_

/* Use these enumerated values for adding fields to the certificate request */
typedef enum {
    crmfVersion,
    crmfSerialNumber,
    crmfSigningAlg,
    crmfIssuer,
    crmfValidity,
    crmfSubject,
    crmfPublicKey,
    crmfIssuerUID,
    crmfSubjectUID,
    crmfExtension
} CRMFCertTemplateField;

/*
 * An enumeration for the different types of controls.
 */
typedef enum {
    crmfNoControl,
    crmfRegTokenControl,
    crmfAuthenticatorControl,
    crmfPKIPublicationInfoControl,
    crmfPKIArchiveOptionsControl,
    crmfOldCertIDControl,
    crmfProtocolEncrKeyControl
} CRMFControlType;

/*
 * The possible values that are passed into CRMF_CreatePKIPublicationInfo
 */
typedef enum{
    crmfDontPublish,
    crmfPleasePublish
} CRMFPublicationAction;

/*
 * An enumeration for the possible for pubMethod which is a part of 
 * the SinglePubInfo ASN1 type.
 */
typedef enum {
    crmfDontCare,
    crmfX500,
    crmfWeb,
    crmfLdap
} CRMFPublicationMethod;

/*
 * An enumeration for the different options for PKIArchiveOptions type.
 */
typedef enum {
    crmfNoArchiveOptions,
    crmfEncryptedPrivateKey,
    crmfKeyGenParameters,
    crmfArchiveRemGenPrivKey
} CRMFPKIArchiveOptionsType;

/*
 * An enumeration for the different options for ProofOfPossession
 */
typedef enum {
    crmfNoPOPChoice,
    crmfRAVerified,
    crmfSignature,
    crmfKeyEncipherment,
    crmfKeyAgreement
} CRMFPOPChoice;

/*
 * An enumertion type for options for the authInfo field of the 
 * CRMFPOPOSigningKeyInput structure.
 */
typedef enum {
    crmfSender,
    crmfPublicKeyMAC
} CRMFPOPOSkiInputAuthChoice;

/*
 * An enumeration for the SubsequentMessage Options.
 */
typedef enum {
    crmfNoSubseqMess,
    crmfEncrCert,
    crmfChallengeResp
} CRMFSubseqMessOptions;

/*
 * An enumeration for the choice used by POPOPrivKey.
 */
typedef enum {
    crmfNoMessage,
    crmfThisMessage,
    crmfSubsequentMessage,
    crmfDHMAC
} CRMFPOPOPrivKeyChoice;

/*
 * An enumeration for the choices for the EncryptedKey type.
 */
typedef enum {
    crmfNoEncryptedKeyChoice,
    crmfEncryptedValueChoice,
    crmfEnvelopedDataChoice
} CRMFEncryptedKeyChoice;

/*
 * TYPE: CRMFEncoderOutputCallback
 *     This function type defines a prototype for a function that the CRMF
 *     library expects when encoding is performed.
 *
 * ARGUMENTS:
 *     arg
 *         This will be a pointer the user passed into an encoding function.
 *         The user of the library is free to use this pointer in any way.
 *         The most common use is to keep around a buffer for writing out
 *         the DER encoded bytes.
 *     buf
 *         The DER encoded bytes that should be written out.
 *     len
 *         The number of DER encoded bytes to write out.
 *
 */
typedef void (*CRMFEncoderOutputCallback) (void *arg,
					   const char *buf,
					   unsigned long len);

/*
 * Type for the function that gets a password.  Just in case we ever
 * need to support publicKeyMAC for POPOSigningKeyInput
 */
typedef SECItem* (*CRMFMACPasswordCallback) (void *arg);

typedef struct CRMFOptionalValidityStr      CRMFOptionalValidity;
typedef struct CRMFValidityCreationInfoStr  CRMFGetValidity;
typedef struct CRMFCertTemplateStr          CRMFCertTemplate;
typedef struct CRMFCertRequestStr           CRMFCertRequest;
typedef struct CRMFCertReqMsgStr            CRMFCertReqMsg;
typedef struct CRMFCertReqMessagesStr       CRMFCertReqMessages;
typedef struct CRMFProofOfPossessionStr     CRMFProofOfPossession;
typedef struct CRMFPOPOSigningKeyStr        CRMFPOPOSigningKey;
typedef struct CRMFPOPOSigningKeyInputStr   CRMFPOPOSigningKeyInput;
typedef struct CRMFPOPOPrivKeyStr           CRMFPOPOPrivKey;
typedef struct CRMFPKIPublicationInfoStr    CRMFPKIPublicationInfo;
typedef struct CRMFSinglePubInfoStr         CRMFSinglePubInfo;
typedef struct CRMFPKIArchiveOptionsStr     CRMFPKIArchiveOptions;
typedef struct CRMFEncryptedKeyStr          CRMFEncryptedKey;
typedef struct CRMFEncryptedValueStr        CRMFEncryptedValue;
typedef struct CRMFCertIDStr                CRMFCertID;
typedef struct CRMFCertIDStr                CRMFOldCertID;
typedef CERTSubjectPublicKeyInfo            CRMFProtocolEncrKey;
typedef struct CRMFValidityCreationInfoStr  CRMFValidityCreationInfo;
typedef struct CRMFCertExtCreationInfoStr   CRMFCertExtCreationInfo;
typedef struct CRMFPKMACValueStr            CRMFPKMACValue;
typedef struct CRMFAttributeStr             CRMFAttribute;
typedef struct CRMFControlStr               CRMFControl;
typedef CERTGeneralName                     CRMFGeneralName;
typedef struct CRMFCertExtensionStr         CRMFCertExtension;

struct CRMFValidityCreationInfoStr {
    PRTime *notBefore;
    PRTime *notAfter;
};

struct CRMFCertExtCreationInfoStr {
    CRMFCertExtension **extensions;
    int numExtensions;
};

/*
 * Some ASN1 Templates that may be needed.
 */
extern const SEC_ASN1Template CRMFCertReqMessagesTemplate[];
extern const SEC_ASN1Template CRMFCertRequestTemplate[];


#endif /*_CRMFT_H_*/
