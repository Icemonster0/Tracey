/* Header file for the Xoshiro256Plus class. */

#ifndef XOSHIRO_HPP
#define XOSHIRO_HPP

#include <cstdint>

namespace trc {

class Xoshiro256Plus {
public:
	Xoshiro256Plus(uint64_t seed_a, uint64_t seed_b = 0, uint64_t seed_c = 0, uint64_t seed_d = 0);

	uint64_t operator()();
	uint64_t min();
	uint64_t max();

	void seed(uint64_t seed_a, uint64_t seed_b = 0, uint64_t seed_c = 0, uint64_t seed_d = 0);
	void jump(void);
	void long_jump(void);

private:
	uint64_t next(void);
	inline uint64_t rotl(const uint64_t x, int k);
	uint64_t s[4];
	int a_cool_number = 0;
};

} /* trc */

#endif /* end of include guard: XOSHIRO_HPP */
