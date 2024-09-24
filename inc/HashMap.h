/*
 * HashMap.h
 *
 *  Created on: 15 Jun 2015
 *      Author: GBY18020
 */

#ifndef BASE_HASHMAP_H_
#define BASE_HASHMAP_H_

#include <unordered_map>
#include "bstrwrap.h"

#define XXH_PRIME1   2654435761U
#define XXH_PRIME2   2246822519U
#define XXH_PRIME3   3266489917U
#define XXH_PRIME4    668265263U
#define XXH_PRIME5   0x165667b1

#define _rotl(x, n) (((x) << (n)) | ((x) >> (32-(n))))
#define _rotr(x, n) (((x) << (n)) | ((x) >> (32-(n))))

// This version is for very small inputs (< 16  bytes)
inline unsigned int XXH_small(CBString key, int len, unsigned int seed)
{
	const unsigned char* p = (unsigned char*)key.data;
	const unsigned char* const bEnd = p + len;
	unsigned int idx = seed + XXH_PRIME1;
	unsigned int crc = XXH_PRIME5;
	const unsigned char* const limit = bEnd - 4;

	while (p<limit)
	{
		crc += ((*(unsigned int*)p) + idx++);
		crc += _rotl(crc, 17) * XXH_PRIME4;
		crc *= XXH_PRIME1;
		p+=4;
	}

	while (p<bEnd)
	{
		crc += ((*p) + idx++);
		crc *= XXH_PRIME1;
		p++;
	}

	crc += len;

	crc ^= crc >> 15;
	crc *= XXH_PRIME2;
	crc ^= crc >> 13;
	crc *= XXH_PRIME3;
	crc ^= crc >> 16;

	return crc;
}

struct StringHash
{
  std::size_t operator()(CBString k) const
  {
		unsigned int seed = 1688943522; // a random seed
		size_t len = k.length();
		// Special case, for small inputs
		if (len < 16) return XXH_small(k, len, seed);

		const unsigned char* p = (const unsigned char*)k.data;
		const unsigned char* const bEnd = p + len;
		unsigned int v1 = seed + XXH_PRIME1;
		unsigned int v2 = v1 * XXH_PRIME2 + len;
		unsigned int v3 = v2 * XXH_PRIME3;
		unsigned int v4 = v3 * XXH_PRIME4;
		const unsigned char* const limit = bEnd - 16;
		unsigned int crc;

		while (p<limit)
		{
			v1 = _rotl(v1, 13) + (*(unsigned int*)p); p+=4;
			v2 = _rotl(v2, 11) + (*(unsigned int*)p); p+=4;
			v3 = _rotl(v3, 17) + (*(unsigned int*)p); p+=4;
			v4 = _rotl(v4, 19) + (*(unsigned int*)p); p+=4;
		}

		p = bEnd - 16;
		v1 += _rotl(v1, 17); v2 += _rotl(v2, 19); v3 += _rotl(v3, 13); v4 += _rotl(v4, 11);
		v1 *= XXH_PRIME1; v2 *= XXH_PRIME1; v3 *= XXH_PRIME1; v4 *= XXH_PRIME1;
		v1 += *(unsigned int*)p; p+=4; v2 += *(unsigned int*)p; p+=4; v3 += *(unsigned int*)p; p+=4; v4 += *(unsigned int*)p;   // p+=4;
		v1 *= XXH_PRIME2; v2 *= XXH_PRIME2; v3 *= XXH_PRIME2; v4 *= XXH_PRIME2;
		v1 += _rotl(v1, 11); v2 += _rotl(v2, 17); v3 += _rotl(v3, 19); v4 += _rotl(v4, 13);
		v1 *= XXH_PRIME3; v2 *= XXH_PRIME3; v3 *= XXH_PRIME3; v4 *= XXH_PRIME3;

		crc = v1 + _rotl(v2, 3) + _rotl(v3, 6) + _rotl(v4, 9);
		crc ^= crc >> 11;
		crc += (XXH_PRIME4+len) * XXH_PRIME1;
		crc ^= crc >> 15;
		crc *= XXH_PRIME2;
		crc ^= crc >> 13;

		return crc;
  }
};

// This version is for very small inputs (< 16  bytes)
inline unsigned int XXH_smallc(char * key, int len, unsigned int seed)
{
printf("Called for str<%s>", key);
	const unsigned char* p = (unsigned char*)key;
	const unsigned char* const bEnd = p + len;
	unsigned int idx = seed + XXH_PRIME1;
	unsigned int crc = XXH_PRIME5;
	const unsigned char* const limit = bEnd - 4;

	while (p<limit)
	{
		crc += ((*(unsigned int*)p) + idx++);
		crc += _rotl(crc, 17) * XXH_PRIME4;
		crc *= XXH_PRIME1;
		p+=4;
	}

	while (p<bEnd)
	{
		crc += ((*p) + idx++);
		crc *= XXH_PRIME1;
		p++;
	}

	crc += len;

	crc ^= crc >> 15;
	crc *= XXH_PRIME2;
	crc ^= crc >> 13;
	crc *= XXH_PRIME3;
	crc ^= crc >> 16;
printf("Hash <%d>\n", crc);
	return crc;
}

struct CharHash
{
  std::size_t operator()(char *k) const
  {
printf("Called for str<%s>", k);
		unsigned int seed = 1688943522; // a random seed
		size_t len = strlen(k);
		// Special case, for small inputs
		if (len < 16) return XXH_smallc(k, len, seed);

		const unsigned char* p = (const unsigned char*)k;
		const unsigned char* const bEnd = p + len;
		unsigned int v1 = seed + XXH_PRIME1;
		unsigned int v2 = v1 * XXH_PRIME2 + len;
		unsigned int v3 = v2 * XXH_PRIME3;
		unsigned int v4 = v3 * XXH_PRIME4;
		const unsigned char* const limit = bEnd - 16;
		unsigned int crc;

		while (p<limit)
		{
			v1 = _rotl(v1, 13) + (*(unsigned int*)p); p+=4;
			v2 = _rotl(v2, 11) + (*(unsigned int*)p); p+=4;
			v3 = _rotl(v3, 17) + (*(unsigned int*)p); p+=4;
			v4 = _rotl(v4, 19) + (*(unsigned int*)p); p+=4;
		}

		p = bEnd - 16;
		v1 += _rotl(v1, 17); v2 += _rotl(v2, 19); v3 += _rotl(v3, 13); v4 += _rotl(v4, 11);
		v1 *= XXH_PRIME1; v2 *= XXH_PRIME1; v3 *= XXH_PRIME1; v4 *= XXH_PRIME1;
		v1 += *(unsigned int*)p; p+=4; v2 += *(unsigned int*)p; p+=4; v3 += *(unsigned int*)p; p+=4; v4 += *(unsigned int*)p;   // p+=4;
		v1 *= XXH_PRIME2; v2 *= XXH_PRIME2; v3 *= XXH_PRIME2; v4 *= XXH_PRIME2;
		v1 += _rotl(v1, 11); v2 += _rotl(v2, 17); v3 += _rotl(v3, 19); v4 += _rotl(v4, 13);
		v1 *= XXH_PRIME3; v2 *= XXH_PRIME3; v3 *= XXH_PRIME3; v4 *= XXH_PRIME3;

		crc = v1 + _rotl(v2, 3) + _rotl(v3, 6) + _rotl(v4, 9);
		crc ^= crc >> 11;
		crc += (XXH_PRIME4+len) * XXH_PRIME1;
		crc ^= crc >> 15;
		crc *= XXH_PRIME2;
		crc ^= crc >> 13;
printf("Hash <%d>\n", crc);
		return crc;
  }
};


typedef std::tr1::unordered_map<CBString, int, StringHash> StringIntMap; // string as key, int as value
typedef std::tr1::unordered_map<CBString, int>::iterator StringIntMapIterator;

#endif /* BASE_HASHMAP_H_ */

