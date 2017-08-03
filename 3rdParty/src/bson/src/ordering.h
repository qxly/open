#ifndef BSON_ORDERING_H
#define BSON_ORDERING_H


namespace bson {

	/** A precomputation of a BSON key pattern.
	The constructor is private to make conversion more explicit so we notice
	where we call make(). Over time we should push this up higher and
	higher.
	*/
	class Ordering
	{
		const unsigned bits;
		const unsigned nkeys;
		Ordering(unsigned b, unsigned n) : bits(b), nkeys(n) { }
	public:
		/** so, for key pattern { a : 1, b : -1 }
		get(0) == 1
		get(1) == -1
		*/
		int get(int i) const
		{
			return ((1 << i) & bits) ? -1 : 1;
		}

		// for woCompare...
		unsigned descending(unsigned mask) const { return bits & mask; }

		operator string() const
		{
			StringBuilder buf(32);
			for (unsigned i = 0; i<nkeys; i++)
				buf.append(get(i) > 0 ? "+" : "-");
			return buf.str();
		}

		static Ordering make(const BSONObj& obj)
		{
			unsigned b = 0;
			BSONObjIterator k(obj);
			unsigned n = 0;
			while (1)
			{
				BSONElement e = k.next();
				if (e.eoo())
					break;
				uassert(13103, "too many compound keys", n <= 31);
				if (e.number() < 0)
					b |= (1 << n);
				n++;
			}
			return Ordering(b, n);
		}
	};

}

#endif