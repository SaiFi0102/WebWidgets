#ifndef _PAIR_HASH_H
#define _PAIR_HASH_H

#include <functional>
#include <boost/functional/hash.hpp>

namespace std
{
	template<typename A, typename B>
	struct hash<std::pair<A, B>>
	{
		size_t operator()(const std::pair<A, B> &v) const
		{
			std::size_t seed = 0;
			boost::hash_combine(seed, v.first);
			boost::hash_combine(seed, v.second);
			return seed;
		}
	};
}

#endif