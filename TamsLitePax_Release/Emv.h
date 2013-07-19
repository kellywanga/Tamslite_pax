    // !!!!!!!!!!
    // You can either (1) copy this file to your working folder, or (2) set a path dependency in Visual Studio project settings.
    // Since EMVLIB may upgrade sometime, we suggest you use (2)
#include "emvlib.h"


/**************************************DATA TAGS**************************************************************/

#define tagACQUIRER_ID               0x9F01  /*     n            6  */
#define tagADDIT_TERML_CAPABILITIES  0x9F40  /*     b            5  */
#define tagAUTH_AMOUNT_BIN           0x0081  /*     b            4  */
#define tagAUTH_AMOUNT_NUM           0x9F02  /*   n12            6  */
#define tagOTHER_AMOUNT_BIN          0x9F04  /*     b            4  */
#define tagOTHER_AMOUNT_NUM          0x9F03  /*   n12            6  */
#define tagAMOUNT_IN_REF_CURRENCY    0x9F3A  /*     b            4  */
#define tagAPPL_CRYPTOGRAM           0x9F26  /*     b            8  */
#define tagAPPL_CURRENCY_CODE        0x9F42  /*     n 3          2  */
#define tagAPPL_CURRENCY_EXPONENT    0x9F44  /*     n 1          1  */
#define tagAPPL_DISCRETIONARY_DATA   0x9F05  /*     b         1-32  */
#define tagAPPL_EFFECTIVE_DATE       0x5F25  /*     n 6          3  */
#define tagAPPL_EXPIRATION_DATE      0x5F24  /*     n 6          3  */
#define tagAFL                       0x0094  /*    var        -252  */
#define tagAID_CARD                  0x004F  /*     b         5-16  */
#define tagAID_TERMINAL              0x9F06  /*     b         5-16  */
#define tagAIP                       0x0082  /*     b            2  */
#define tagAPPL_LABEL                0x0050  /*    ans**         1-16  */
#define tagAPPL_PREF_NAME            0x9F12  /*    ans** 1-16    1-16  */
#define tagPAN                       0x005A  /*    cn -19      -10  */
#define tagPAN_SEQUENCE_NUMBER       0x5F34  /*     n 2          1  */
#define tagAPPL_PRIORITY_IND         0x0087  /*     b            1  */
#define tagAPPL_REF_CURR             0x9F3B  /*     n 3        2-8  */
#define tagAPPL_REF_CURR_EXPONENT    0x9F43  /*     n 1        1-4  */
#define tagAPPL_TEMPLATE             0x0061  /*     b         -252  */
#define tagATC                       0x9F36  /*     b            2  */
#define tagAPPL_USAGE_CONTROL        0x9F07  /*     b            2  */
#define tagCARD_APPL_VERSION         0x9F08  /*     b            2  */
#define tagTERM_APPL_VERSION         0x9F09  /*     b            2  */
#define tagAUTH_CODE                 0x0089  /*    var           6  */
#define tagAUTH_RESPONSE_CODE        0x008A  /*    an            2  */
#define tagCDOL1                     0x008C  /*     b         -252  */
#define tagCDOL2                     0x008D  /*     b         -252  */
#define tagCARDHOLDER_NAME           0x5F20  /*    ans 2-26   2-26  */
#define tagCRDHLDR_NAME_EXT          0x9F0B  /*    ans 27-45 27-45  */
#define tagCVM_LIST                  0x008E  /*     b         -252  */
#define tagCVM_RESULT                0x9F34  /*     b            3  */
#define tagCERT_AUTH_PKI             0x008F  /*     b            1  */
#define tagCERT_AUTH_PKI_TERMINAL    0x9F22  /*     b            1  */
#define tagCOMMAND_TEMPLATE          0x0083  /*     b          var  */
#define tagCRYPTOGRAM_INFO           0x9F27  /*     b            1  */
#define tagDATA_AUTH_CODE            0x9F45  /*     b            2  */
#define tagDF_NAME                   0x0084  /*     b         5-16  */
#define tagDDF_NAME                  0x009D  /*     b         5-16  */
#define tagDIR_DISCR_TEMPLATE        0x0073  /*    var        -252  */
#define tagDDOL                      0x9F49  /*     b         -252  */
#define tagFCI_ISSUER_DISCR          0xBF0C  /*    var        -222  */
#define tagFCI_PROPRIETARY_TEMPLATE  0x00A5  /*    var         var  */
#define tagFCI_TEMPLATE              0x006F  /*    var        -252  */
#define tagICC_DYNAMIC_NUMBER        0x9F4C  /*     b          2-8  */
#define tagPIN_PK_CERTIFICATE        0x9F2D  /*     b           Ni  */
#define tagPIN_PK_EXPONENT           0x9F2E  /*     b          1,3  */
#define tagPIN_PK_REMAINDER          0x9F2F  /*     b    Npe-Ni+42  */
#define tagICC_PK_CERTIFICATE        0x9F46  /*     b           Ni  */
#define tagICC_PK_EXPONENT           0x9F47  /*     b          1-3  */
#define tagICC_PK_REMAINDER          0x9F48  /*     b    Nic-Ni+42  */
#define tagIFD_SERIAL_NUM            0x9F1E  /*    an            8  */
#define tagISSUER_ACTION_DEFAULT     0x9F0D  /*     b            5  */
#define tagISSUER_ACTION_DENIAL      0x9F0E  /*     b            5  */
#define tagISSUER_ACTION_ONLINE      0x9F0F  /*     b            5  */
#define tagISSUER_APPL_DATA          0x9F10  /*     b          -32  */
#define tagISSUER_AUTH_DATA          0x0091  /*     b         8-16  */
#define tagISSUER_CODE_TABLE_INDEX   0x9F11  /*     n 2          1  */
#define tagISSUER_COUNTRY_CODE       0x5F28  /*     n 3          2  */
#define tagISSUER_PK_CERTIFICATE     0x0090  /*     b          Nca  */
#define tagISSUER_PK_EXPONENT        0x9F32  /*     b          1,3  */
#define tagISSUER_PK_REMAINDER       0x0092  /*     b    Ni-Nca+36  */
#define tagISSUER_SCRIPT_COMMAND     0x0086  /*     b         -261  */
#define tagISSUER_SCRIPT_ID          0x9F18  /*     b            4  */
#define tagISSUER_SCRIPT_TEMPLATE_1  0x0071  /*     b          var  */
#define tagISSUER_SCRIPT_TEMPLATE_2  0x0072  /*     b          var  */
#define tagISSUER_URL                0x5F50  /*     ans         var  */
#define tagLANGUAGE_PREFERENCE       0x5F2D  /*    an 2        2-8  */
#define tagLAST_ONLINE_ATC_REGISTER  0x9F13  /*     b            2  */
#define tagLOG_ENTRY                 0x9F4D  /*     b            2  */
#define tagLOG_FORMAT                0x9F4F  /*     b          var  */
#define tagMIN_CONS_OFFLINE_LIMIT    0x9F14  /*     b            1  */
#define tagMERCH_CATEGORY_CODE       0x9F15  /*     n            2  */
#define tagMERCH_ID                  0x9F16  /*   ans           15  */
#define tagMERCH_NAME_LOCATION       0x9F4E  /*   ans          var  */
#define tagPIN_TRY_COUNTER           0x9F17  /*     b            1  */
#define tagPOS_ENTRY_MODE            0x9F39  /*     n2           1  */
#define tagPDOL                      0x9F38  /*     b          var  */
#define tagRESPONSE_MSG_TEMPLATE1    0x0080  /*    var         var  */
#define tagRESPONSE_MSG_TEMPLATE2    0x0077  /*    var         var  */
#define tagSERVICE_CODE              0x5F30  /*     n 3          2  */
#define tagSFI                       0x0088  /*     b            1  */
#define tagSIGNED_DYNAMIC_APPL_DATA  0x9F4B  /*     b          Nic  */
#define tagSIGNED_STATIC_APPL_DATA   0x0093  /*     b           Ni  */
#define tagSTATIC_DATA_AUTH_TAG_LIST 0x9F4A  /*    --          var  */
#define tagTERML_CAPABILITIES        0x9F33  /*     b            3  */
#define tagTERML_COUNTRY_CODE        0x9F1A  /*     n            2  */
#define tagTERML_FLOOR_LIMIT         0x9F1B  /*     b            4  */
#define tagTERML_ID                  0x9F1C  /*    an            8  */
#define tagTERML_RISK_MANAG_DATA     0x9F1D  /*     b          1-8  */
#define tagTERML_TYPE                0x9F35  /*     n            1  */
#define tagTVR                       0x0095  /*     b            5  */
#define tagTRACK1_DISC_DATA          0x9F1F  /*    ans         var  */
#define tagTRACK2_DISC_DATA          0x9F20  /*     cn         var  */
#define tagTRACK2_EQUIVALENT_DATA    0x0057  /*     b          -19  */
#define tagTDOL                      0x0097  /*     b         -252  */
#define tagTC_HASH_VALUE             0x0098  /*     b           20  */
#define tagTRAN_CURR_CODE            0x5F2A  /*     n 3          2  */
#define tagTRAN_CURR_EXPONENT        0x5F36  /*     b            1  */
#define tagTRAN_DATE                 0x009A  /*     n 6          3  */
#define tagTRAN_PIN_DATA             0x0099  /*     b          var  */
#define tagTRAN_REF_CURR_CODE        0x9F3C  /*     n 3          2  */
#define tagTRAN_REF_CURR_EXPONENT    0x9F3D  /*     n 1          1  */
#define tagTRAN_SEQUENCE_COUNTER     0x9F41  /*     n 4-8      2-4  */
#define tagTSI                       0x009B  /*     b            2  */
#define tagTRAN_TIME                 0x9F21  /*     n 6          3  */
#define tagTRAN_TYPE                 0x009C  /*     n 2          1  */
#define tagUNPREDICTABLE_NUMBER      0x9F37  /*     b            4  */
#define tagMAX_CONS_OFFLINE_LIMIT    0x9F23  /*     b            1  */
#define tagAEF_DATA_TEMPLATE         0x0070  /* var              */


/* Spec. Update Bulletin #39 */
#define tagACCOUNT_TYPE              0x5F57  /*     n            2  */

/* Mastercard specific!!  */
#define tagTRN_CATEGORY_CODE         0x9F53   /*   an            1  */

/* Proprietary tag for FORCE APPROVAL option  */
#define tagPRV_FORCE_APPROVAL       0xDF99    /*   b            1  */

/* Proprietary tag for 4B Authorization Mode  */
#define tagPRV_4B_AUTH_MODE         0xDF98    /*   b            1  */

/* Proprietary tags for China PBOC ID checks  */
#define tagPRV_PBOC_CRDHLDR_ID_NO    0x9F61    /*   an           40  */
#define tagPRV_PBOC_CRDHLDR_ID_TYPE  0x9F62    /*   cn           1   */

/*
Yoel 7.10.08
New tags for Electronic Cash Payment System (EC Payment)
*/
#define tagEC_BALANCE                    0x9F79  /*     n 12         6   */
#define tagEC_BALANCE_LIMIT              0x9F77  /*     n 12         6   */
#define tagEC_ISSUER_AUTHORIZATION_CODE  0x9F74  /*     a	         6   */
#define tagEC_SINGLE_TRANSACTION_LIMIT   0x9F78  /*     n 12         6   */
#define tagEC_RESET_THRESHOLD            0x9F6D  /*     n 12         6   */
#define tagEC_TERMINAL_SUPPORT_INDICATOR 0x9F7A  /*     b	         1   */
#define tagEC_TERMINAL_TRANSACTION_LIMIT 0x9F7B  /*     n 12         6   */
/*End new tags for EC*/

/*
Yoel 11.11.08 
Additional EMV Tags
*/
#define tagISSUER_IDENT_NUMBER	         0x0042  /*      n6			3	   */
#define tagIBAN					         0x5F53  /*      var	   1-34    */
#define tagBIC						     0x5F54  /*		 var       8 or 11 */
#define tagISSUER_COUNTRY_CODE_F2        0x5F55  /*		 a          2      */
#define tagISSUER_COUNTRY_CODE_F3        0x5F56  /*		 a          3      */
/*End Additional EMV Tags*/

/*Terminal Verification Results: (Byte 1) */
#define tvr_1_NO_OFFLINE_AUTH                0x80
#define tvr_1_FAIL_OFFLINE_STATIC_AUTH       0x40
#define tvr_1_ICC_DATA_MISSING               0x20
#define tvr_1_CARD_IN_STOPLIST               0x10
#define tvr_1_FAIL_OFFLINE_DYNAMIC_AUTH      0x08
#define tvr_1_FAIL_COMBINED_DDA              0x04

/*TVR (Byte 2)*/
#define tvr_2_DIFFERENT_APPL_VER             0x80
#define tvr_2_EXPIRED_APPL                   0x40
#define tvr_2_NOT_YET_EFFECTIVE              0x20
#define tvr_2_SERVICE_NOT_ALLOWED            0x10
#define tvr_2_NEW_CARD                       0x08

/*TVR (Byte 3)*/
#define tvr_3_CHV_UNSUCCESSFUL               0x80
#define tvr_3_UNRECOGNISED_CVM               0x40
#define tvr_3_PIN_TRY_LIMIT_EXCEEDED         0x20
#define tvr_3_PINPAD_NOT_FOUND               0x10
#define tvr_3_PIN_NOT_ENTERED                0x08
#define tvr_3_ONLINE_PIN_ENTERED             0x04

/*TVR (Byte 4)*/
#define tvr_4_OVER_FLOOR_LIMIT               0x80
#define tvr_4_OVER_LOWER_OFFLINE_LIM         0x40
#define tvr_4_OVER_UPPER_OFFLINE_LIM         0x20
#define tvr_4_RANDOMLY_SELECTED              0x10
#define tvr_4_MERCH_FORCED_ONLINE            0x08

/*TVR (Byte 5)*/
#define tvr_5_DEFAULT_TDOL_USED              0x80
#define tvr_5_FAIL_ISSUER_AUTH               0x40
#define tvr_5_FAIL_SCRIPT_BEFORE_LAST_AC     0x20
#define tvr_5_FAIL_SCRIPT_AFTER_LAST_AC      0x10


/*Transaction Status Information: (Byte 1)*/
#define tsi_1_OFFLINE_DATA_AUTHENTICATION    0x80
#define tsi_1_CARDHOLDER_VERFIFICATION       0x40
#define tsi_1_CARD_RISK_MANAGEMENT           0x20
#define tsi_1_ISSUER_AUTHENTICATION          0x10
#define tsi_1_TERML_RISK_MANAGEMENT          0x08
#define tsi_1_SCRIPT_PROCESSED               0x04

/*TSI (Byte 2) completely reserved for future use*/

/* First two digits of ISO 8583:1987 Processing Codes */
/* Transaction type data shall be set to one of these values */
/* DEBITS */
#define ttdGOODSNSERVICES	                   0x00 
#define ttdWITHDRWL_CASHADV                  0x01 
#define ttdADJUSTMENT                        0x02 
#define ttdCHEQUE_GUARANTEE                  0x03 
#define ttdCHEQUE_VERIFICATION               0x04
#define ttdEUROCHEQUE                        0x05
#define ttdTRAVELLER_CHEQUE                  0x06
#define ttdLETTER_OF_CREDIT                  0x07
#define ttdGIRO                              0x08
#define ttdGOODSNSER_WITH_CASH               0x09
/* CREDITS */
#define ttcRETURNS                           0x20
#define ttcDEPOSITS                          0x21
#define ttcADJUSTMENT                        0x22
#define ttcCHEQUE_DPST_GUARANTEE             0x23
#define ttcCHEQUE_DEPOSIT                    0x24
/* INQUIRY SERVICES */
#define ttiAVAILABLE_FUNDS_INQ               0x30
#define ttiBALANCE_INQ                       0x31
/* TRANSFER SERVICES */
#define tttCRDHOLDER_ACCOUNT_TRN             0x40

/* Application Selection constants */
#define asEXACT_MATCH                        0x00
#define asPARTIAL_MATCH                      0x01

/* Lipman Kernel Options: (Byte 1) */
#define opt_1_PSE_SELECTION                  0x80
#define opt_1_CARDHOLDER_SELECT              0x40
#define opt_1_CARDHODER_CONFIRM              0x20
#define opt_1_TERM_PREF_ORDER                0x10
#define opt_1_PARTIAL_APP_SELECTION          0x08
#define opt_1_DDF_2_ALGORITHM                0x04
#define opt_1_ACCOUNT_TYPE_SELECT            0x02
/* China PBOC proprietary Cardholder ID verification method 
Not in use!!! */
/*#define opt_1_PBOC_ID_VERIFICATION           0x01  */


/* Lipman Kernel Options: (Byte 2) */
#define opt_2_MULTI_LANGUAGE                 0x80
#define opt_2_DEFAULT_TDOL                   0x40
#define opt_2_DEFAULT_DDOL                   0x20
#define opt_2_FORCE_ONLINE                   0x10
#define opt_2_FORCE_APPROVAL                 0x08
#define opt_2_AMOUNT_CONFIRMATION            0x04

/* Lipman Kernel Options: (Byte 3) */
#define opt_3_EXTERNAL_PINPAD                0x80
#define opt_3_PIN_BYPASS                     0x40
#define opt_3_GDPTC                          0x20
#define opt_3_EXCEPTION_LIST                 0x10
#define opt_3_REVOCATION_LIST                0x08
/*Piter 4.1.c */
#define opt_3_CDA_NOT_FAIL_PRIOR_TAA         0x04
#define opt_3_CDA_NEVER_IN_GENAC1            0x02 /*0-cda always requested in Genac1,ARQC , 1 - cda never requested in Genac1 , ARQC*/ 
#define opt_3_CDA_NEVER_IN_GENAC2            0x01 /*0-cda always requested in Genac2,TC , 1 - cda never requested in Genac2,TC*/ 

/* Lipman Kernel Options: (Byte 4) */
#define opt_4_RISK_MANAGEMENT                0x80
#define opt_4_FLOOR_LIMIT                    0x40
#define opt_4_VELOCITY_CHECK                 0x20
#define opt_4_RANDOM_SELECTION               0x10
#define opt_4_TRANSACTION_LOG                0x08
#define opt_4_FAIL_CVM                       0x04
#define opt_4_TRM_BASED_ON_AIP               0x02

/* Lipman Kernel Options: (Byte 5) */
#define opt_5_CA_248                         0x80
#define opt_5_CA_224                         0x40
#define opt_5_CA_192                         0x20
#define opt_5_CA_160                         0x10
#define opt_5_REFER_REVERSAL_AAC             0x08
#define opt_5_PBOC                           0x04
#define opt_5_PBANCOMAT                      0x02
#define opt_5_4B                             0x01

/* Lipman Kernel Options: (Byte 6) */
#define opt_6_ONLINE_DATA_CAPTURE            0x80
#define opt_6_BATCH_DATA_CAPTURE             0x40
#define opt_6_ADVICE                         0x20
#define opt_6_REFER_REVERSAL_TC              0x08
#define opt_6_ISSUER_REFERRAL                0x04
#define opt_6_DISABLE_OFFLINE_TRN            0x02
#define opt_6_PRNT_TRN_EXCE_READ             0x01

/* Lipman Kernel Options: (Byte 7) */
/*
Yoel 7.10.08
New option for Electronic Cash Payment System
*/
#define    opt_7_EC_PAYMENT_SYSTEM           0x80
/*
Yoel 25.12.08
New option for ETEC to allow fallback when applist is
empty because the only application on the ICC is blocked
*/
#define    opt_7_EMPTY_APPLIST_APP_BLOCKED   0x40 
/* #define opt_7_CARD_REFERRAL                  0x80 */
/* #define opt_7_CARD_REFERRAL_ARQC             0x40 */
/* #define opt_7_CARD_REFERRAL_TC               0x20 */
/* #define opt_7_CARD_REFERRAL_AAC              0x10 */
/* #define opt_7_CARD_REFERRAL_MA               0x08 */
#define opt_7_FALLBACK_NO_CH_CONF            0x04
#define opt_7_FALLBACK_TERMINATE             0x02
#define opt_7_FALLBACK_TRN_NOT_COMP          0x01   /* No cryptogram generated for GENERATE AC */

/* Lipman Kernel Options: (Byte 8) */
#define opt_8_ETEC                           0x80
#define opt_8_VSDC                           0x40
#define opt_8_ASHRAIT                        0x20
#define opt_8_VISANET                        0x10
#define opt_8_JCB                            0x08
#define opt_8_SERMEPA                        0x04
#define opt_8_AMEX                           0x02
#define opt_8_PRINT_TRANSPORT                0x01

/*Terminal Capabilities: Card Data Input Capability(Byte 1) */
#define capMANUAL_KEY_ENTRY               0x80
#define capMAGNETIC_STRIPE                0x40
#define capICC_WITH_CONTACTS              0x20

/*Terminal Capabilities: CVM Capability (Byte 2)*/
#define capPLAIN_PIN                      0x80
#define capENCIPHERED_PIN_ONLINE          0x40
#define capSIGNATURE                      0x20
#define capENCIPHERED_PIN_OFFLINE         0x10
#define capNO_CVM                         0x08  
/* China PBOC proprietary Cardholder ID verification method */
#define capPBOC_ID_VERIFICATION           0x01  

/*Terminal Capabilities: Security Capability (Byte 3)*/
#define capSTATIC_AUTH                    0x80
#define capDYNAMIC_AUTH                   0x40
#define capCARD_CAPTURE                   0x20
#define capCOMBINED_DDA                   0x08

/*Additional Terminal Capabilities: Transaction Type Capability (Byte 1): */
#define acpCASH                           0x80
#define acpGOODS                          0x40
#define acpSERVICES                       0x20
#define acpCASHBACK                       0x10
#define acpINQUIRY                        0x08 
#define acpTRANSFER                       0x04 
#define acpPAYMENT                        0x02 
#define acpADMINISTRATIVE                 0x01

/*Additional Terminal Capabilities: Transaction Type Capability continued(Byte 2): */
#define acpCASH_DEPOSIT                   0x80

/*Additional Terminal Capabilities: Terminal Data Input acpability (Byte 3)*/
#define acpNUMERIC_KEYS                   0x80
#define acpALPHA_AND_SPECIAL_KEYS         0x40
#define acpCOMMAND_KEYS                   0x20
#define acpFUNCTION_KEYS                  0x10

/*Additional Terminal Capabilities: Terminal Data Output Capability (Byte 4) */
#define acpPRINT_ATTENDANT                0x80
#define acpPRINT_CARDHOLDER               0x40
#define acpDISPLAY_ATTENDANT              0x20
#define acpDISPLAY_CARDHOLDER             0x10

/*Additional Terminal Capabilities: Terminal Data Output Capability continued(Byte 5) */
#define acpNORDIC                         0x02 /*ISO8859-10Latin6 (Nordic)*/
#define acpTURKISH                        0x01 /*ISO8859-9 Latin5 (Turkish)*/
#define acpHEBREW                         0x80 /*ISO8859-8 Hebrew*/
#define acpGREEK                          0x40 /*ISO8859-7 Greek*/
#define acpARABIC                         0x20 /*ISO8859-6 Arabic*/
#define acpCYRILLIC                       0x10 /*ISO8859-5 Cyrillic*/
#define acpLATIN4                         0x08 /*ISO8859-4 North Europe*/
#define acpLATIN3                         0x04 /*ISO8859-3 South Europe*/
#define acpLATIN2                         0x02 /*ISO8859-2 East Europe */
#define acpLATIN1                         0x01 /*ISO8859-1 West Europe */

/*  Account Type values  - Specification Update Bulletin #39 */
#define accDEFAULT                        0x00 
#define accSAVINGS                        0x10
#define accCHEQUE_DEBIT                   0x20 
#define accCREDIT                         0x30 

/* Number of languages supported */ 
#define MAX_LANGUAGES                        0x05  /* 1 local + 4 foreign */   

/* Saving modes in the internal structure */
#define DB_UPDATEABLE				                 0x01
#define DB_UNIQUE						                 0x02

/* PIN options - PIN verification options */
typedef enum
{
  pin_ONLINE              = 0x80,
  pin_BYPASS_ALLOWED      = 0x40,
  pin_AMOUNT_CONFIRMATION = 0x20,
  pin_LAST_TRY            = 0x10,
  pin_OFFLINE_ENC         = 0x08
} PIN_OPTIONS;

/* Authorization Response Codes recognized by EMV */
#define ARC_APPROVED                         0x00
#define ARC_DECLINED                         0x51
#define ARC_REFERRAL                         0x01  /* Defined in the scope of library, actual code may differ */
#define ARC_PICK_UP                          0x04
#define ARC_NO_CONNECTION                    0xFF  /* Indicates that terminal is unable to go online */

/* Host messages that are required to send in return */
#define hms_ADVICE                           0x80  
#define hms_REVERSAL                         0x40
#define hms_NOT_ACCEPTED                     0x08

/*VERIFY Command - Qualifier of Reference Data*/
#define VERIFY_OFFLINE_PIN          0x80
#define VERIFY_OFFLINE_ENC_PIN      0x88


void EraseExpireCAPK(void);
void InitTestKeys(void);
void InitTestApps(void);
void PromptRemoveICC(void);
void ShowTVR_TAC_IAC(void);

