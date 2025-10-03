#ifndef COMPAT_VECTOR_H_
#define COMPAT_VECTOR_H_

#include <stddef.h> // size_t
#include <new>      // placement new, operator new/delete

#if defined(__AVR__)
// Minimal std::vector replacement for AVR targets without <vector> support.
// Supports the subset used by OPEnSLab SSLClient:
// - constructors: default, copy, from pointer range (T* first,last)
// - destructor, copy assignment
// - reserve, clear, size, data
// - push_back, emplace_back(const T&)
// - begin, end, operator[]
// - erase(iterator) for a single element
// This implementation assumes T is copy-constructible and assignable.
namespace std {
	template <typename T>
	class vector {
	public:
		using value_type = T;
		using iterator = T*;
		using const_iterator = const T*;

		vector() : _data(nullptr), _size(0), _capacity(0) {}

		vector(const T* first, const T* last) : _data(nullptr), _size(0), _capacity(0) {
			size_t n = (last > first) ? static_cast<size_t>(last - first) : 0;
			if (n) {
				reserve(n);
				for (size_t i = 0; i < n; i++) new (_data + i) T(first[i]);
				_size = n;
			}
		}

		vector(const vector& other) : _data(nullptr), _size(0), _capacity(0) {
			if (other._size) {
				reserve(other._size);
				for (size_t i = 0; i < other._size; i++) new (_data + i) T(other._data[i]);
				_size = other._size;
			}
		}

		vector& operator=(const vector& other) {
			if (this == &other) return *this;
			clear();
			reserve(other._size);
			for (size_t i = 0; i < other._size; i++) new (_data + i) T(other._data[i]);
			_size = other._size;
			return *this;
		}

		~vector() {
			clear();
			if (_data) ::operator delete((void*)_data);
			_data = nullptr; _capacity = 0;
		}

		void reserve(size_t newcap) {
			if (newcap <= _capacity) return;
			T* newdata = (T*)::operator new(sizeof(T) * newcap);
			// move/copy construct into new storage
			for (size_t i = 0; i < _size; i++) {
				new (newdata + i) T(_data[i]);
				_data[i].~T();
			}
			if (_data) ::operator delete((void*)_data);
			_data = newdata;
			_capacity = newcap;
		}

		void clear() {
			for (size_t i = 0; i < _size; i++) _data[i].~T();
			_size = 0;
		}

		size_t size() const { return _size; }
		T* data() { return _data; }
		const T* data() const { return _data; }

		T& operator[](size_t idx) { return _data[idx]; }
		const T& operator[](size_t idx) const { return _data[idx]; }

		iterator begin() { return _data; }
		const_iterator begin() const { return _data; }
		iterator end() { return _data + _size; }
		const_iterator end() const { return _data + _size; }

		void push_back(const T& value) {
			if (_size == _capacity) reserve(_capacity ? (_capacity * 2) : 4);
			new (_data + _size) T(value);
			_size++;
		}
		void emplace_back(const T& value) { push_back(value); }

		void erase(iterator it) {
			size_t idx = static_cast<size_t>(it - _data);
			if (idx >= _size) return;
			// shift left by assignment
			for (size_t i = idx; i + 1 < _size; i++) {
				_data[i] = _data[i + 1];
			}
			// destroy last now duplicated element
			_data[_size - 1].~T();
			_size--;
		}

	private:
		T* _data;
		size_t _size;
		size_t _capacity;
	};
} // namespace std

#else
#include <vector>
#endif

#endif // COMPAT_VECTOR_H_

