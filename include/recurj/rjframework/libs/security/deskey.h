#pragma once
#include <recurj/rjframework/libs/security/security.h>
namespace rjSecurity {
	constexpr size_t cUSDESKeyLong=16;
	constexpr size_t cUSDESKeyShort=8;
	class RJSECURITY_API CRJDESKey {
	public:
		CRJDESKey();
		CRJDESKey(const uint8_t*, size_t);
		CRJDESKey(const uint8_t*, size_t,bool);
		virtual~CRJDESKey();
		uint8_t* GetKey(size_t&);
		bool SetKey(const uint8_t*, size_t);
		bool Decrypt(uint8_t*, size_t) const;
		bool Encrypt(uint8_t*, size_t) const;
		bool GetMixMode() const;
		void SetMixMode(bool);
		inline const uint8_t* GetKeyData() const {
			return m_bKey;
		}
	protected:
		uint8_t m_bKey[cSizeSecurityKey];
		size_t m_lKeyLen;
		bool m_bMix;
	};
}

