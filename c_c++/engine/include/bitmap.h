#ifndef BITMAP_H
#define BITMAP_H

#include<vector>
#include<cstdint>

class bitmap : public std::vector<uint64_t>
{
	public:
		bitmap();
		virtual ~bitmap();

	protected:

	private:
};

#endif // BITMAP_H
