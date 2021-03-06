/****
DIAMOND protein aligner
Copyright (C) 2013-2019 Benjamin Buchfink <buchfink@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
****/

#ifndef SCORE_VECTOR_INT8_H_
#define SCORE_VECTOR_INT8_H_

#include "score_vector.h"

#ifdef __SSE4_1__

template<>
struct score_vector<int8_t>
{

	score_vector():
		data_(_mm_set1_epi8(std::numeric_limits<char>::min()))
	{}

	score_vector(__m128i data):
		data_(data)
	{}

	score_vector(int8_t x):
		data_(_mm_set1_epi8(x))
	{}

	score_vector(const int8_t* s) :
		data_(_mm_loadu_si128(reinterpret_cast<const __m128i*>(s)))
	{ }

	score_vector(unsigned a, const __m128i &seq)
	{
#ifdef __SSSE3__
		set_ssse3(a, seq);
#else
		set_generic(a, seq);
#endif
	}

	void set_ssse3(unsigned a, const __m128i &seq)
	{
#ifdef __SSSE3__
		const __m128i *row = reinterpret_cast<const __m128i*>(&score_matrix.matrix8()[a << 5]);

		__m128i high_mask = _mm_slli_epi16(_mm_and_si128(seq, _mm_set1_epi8('\x10')), 3);
		__m128i seq_low = _mm_or_si128(seq, high_mask);
		__m128i seq_high = _mm_or_si128(seq, _mm_xor_si128(high_mask, _mm_set1_epi8('\x80')));

		__m128i r1 = _mm_load_si128(row);
		__m128i r2 = _mm_load_si128(row + 1);
		__m128i s1 = _mm_shuffle_epi8(r1, seq_low);
		__m128i s2 = _mm_shuffle_epi8(r2, seq_high);
		data_ = _mm_or_si128(s1, s2);
#endif
	}

	void set_generic(unsigned a, const __m128i &seq)
	{
		const int8_t* row(&score_matrix.matrix8()[a << 5]);
		const int8_t* seq_ptr(reinterpret_cast<const int8_t*>(&seq));
		int8_t* dest(reinterpret_cast<int8_t*>(&data_));
		for (unsigned i = 0; i < 16; i++)
			*(dest++) = row[*(seq_ptr++)];
	}

	score_vector operator+(const score_vector &rhs) const
	{
		return score_vector(_mm_adds_epi8(data_, rhs.data_));
	}

	score_vector operator-(const score_vector &rhs) const
	{
		return score_vector(_mm_subs_epi8(data_, rhs.data_));
	}

	score_vector& operator-=(const score_vector &rhs)
	{
		data_ = _mm_subs_epi8(data_, rhs.data_);
		return *this;
	}

	score_vector& operator++()
	{
		data_ = _mm_adds_epi8(data_, _mm_set(1));
		return *this;
	}

	__m128i operator==(const score_vector &rhs) const
	{
		return _mm_cmpeq_epi8(data_, rhs.data_);
	}

	int operator [](unsigned i) const
	{
		return *(((uint8_t*)&data_) + i);
	}

	void set(unsigned i, uint8_t v)
	{
		*(((uint8_t*)&data_) + i) = v;
	}

	score_vector& max(const score_vector &rhs)
	{
		data_ = _mm_max_epi8(data_, rhs.data_);
		return *this;
	}

	score_vector& min(const score_vector &rhs)
	{
		data_ = _mm_min_epi8(data_, rhs.data_);
		return *this;
	}

	friend score_vector max(const score_vector& lhs, const score_vector &rhs)
	{
		return score_vector(_mm_max_epi8(lhs.data_, rhs.data_));
	}

	friend score_vector min(const score_vector& lhs, const score_vector &rhs)
	{
		return score_vector(_mm_min_epi8(lhs.data_, rhs.data_));
	}

	void store(int8_t *ptr) const
	{
		_mm_storeu_si128((__m128i*)ptr, data_);
	}

	friend std::ostream& operator<<(std::ostream &s, score_vector v)
	{
		int8_t x[16];
		v.store(x);
		for (unsigned i = 0; i < 16; ++i)
			printf("%3i ", (int)x[i]);
		return s;
	}

	__m128i data_;

};

template<>
struct ScoreTraits<score_vector<int8_t>>
{
	enum { CHANNELS = 16 };
	typedef int8_t Score;
	static score_vector<int8_t> zero() {
		return score_vector<int8_t>();
	}
	static constexpr int8_t max_score() {
		return std::numeric_limits<int8_t>::max();
	}
	static int int_score(int8_t s)
	{
		return (int)s - SCHAR_MIN;
	}
	static constexpr int8_t zero_score() {
		return SCHAR_MIN;
	}
	static void saturate(score_vector<int8_t> &v) {}
};

#endif

#endif