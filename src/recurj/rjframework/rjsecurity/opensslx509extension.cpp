#include "stdafx.h"
#include <cdata/security/septadaopenssl.h>
#include <cdata/security/septadaopensslx509extension.h>
#define THIS_X509_EXT ((X509_EXTENSION *)m_hSSLHandle)
CSeptadaOpenSSLX509Extension::CSeptadaOpenSSLX509Extension() {}
CSeptadaOpenSSLX509Extension::~CSeptadaOpenSSLX509Extension() {}
bool CSeptadaOpenSSLX509Extension::ValidateFQDN(CRJStringByte& clHost) {
	CSeptadaStringDefault str(OBJ_nid2sn(OBJ_obj2nid(X509_EXTENSION_get_object(THIS_X509_EXT))));
	if (str.IsThatString(CRJSecurityHandle::s_clSSLHandleWordSubjectAltName)) {
		const X509V3_EXT_METHOD* meth=X509V3_EXT_get(THIS_X509_EXT);
		if (meth) {
			int32_t j;
			CONF_VALUE* nval;
			const unsigned char* data=THIS_X509_EXT->value->data;
			STACK_OF(CONF_VALUE)* val=meth->i2v(meth, meth->d2i(NULL,& data, THIS_X509_EXT->value->length), NULL);
			for (j=0; j<sk_CONF_VALUE_num(val); j++) {
				nval=sk_CONF_VALUE_value(val, j);
				if ((nval)&&
					(CRJSecurityHandle::s_clSSLHandleWordDNS.IsThatString(nval->name))&&
					(clHost.IsThatString(nval->value))) return true;
			}
		}
	}
	return false;
}

