#ifndef BSON_NONCE_H
#define BSON_NONCE_H

#include <stdlib.h>
#include <iostream>

namespace Nonce
{

	typedef unsigned long long nonce;

	struct Security
	{
		Security();

		nonce getNonce();
		/** safe during global var initialization */
		nonce getNonceInitSafe()
		{
			init();
			return getNonce();
		}

	private:
		std::ifstream *_devrandom;
		static bool _initialized;
		void init();

	};

	extern Security security;

} // namespace mongo

#endif