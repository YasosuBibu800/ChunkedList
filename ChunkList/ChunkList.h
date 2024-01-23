﻿#pragma once
#include <iterator>
#include <memory>
#include <stdexcept>

namespace fefu_laboratory_two
{
	template <typename T>
	class Allocator {
	public:
		using value_type = T;
		using size_type = std::size_t;
		using pointer = value_type*;

		Allocator() noexcept = default;

		Allocator(const Allocator& other) noexcept {};

		template <class U>
		Allocator(const Allocator<U>& other) noexcept {};

		~Allocator() = default;

		pointer allocate(size_type n) {
			pointer ptr = static_cast<pointer>(malloc(sizeof(value_type) * n));
			if (ptr == nullptr) throw std::bad_alloc();
			return ptr;
		};

		void deallocate(pointer p) noexcept { free(p); };
	};

	template <typename ValType>
	class ChunkListInterface {
	public:
		virtual ~ChunkListInterface() = default;
		virtual size_t size() const noexcept = 0;
		virtual ValType& at(size_t pos) = 0;
		virtual ValType& operator[](size_t n) = 0;
	};

	template <typename ValType>
	class ChunkList_Iterator {
	protected:
		ChunkListInterface<ValType>* _container = nullptr;
		ValType* _value = nullptr;
		int _ind = 0;
	public:
		using iterator_category = std::random_access_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using pointer = ValType*;
		using reference = ValType&;

		virtual int get_index()
		{ 
			return _ind;
		};

		constexpr ChunkList_Iterator() noexcept = default;

		ChunkList_Iterator(ChunkListInterface<ValType>* chunk, int _index, ValType* _value) :
			_container(chunk),
			_ind(_index),
			_value(_value)
		{};

		ChunkList_Iterator(const ChunkList_Iterator& other) noexcept = default;

		ChunkList_Iterator& operator=(const ChunkList_Iterator&) = default;

		virtual ~ChunkList_Iterator() = default;

		friend void swap(ChunkList_Iterator<ValType>& a, ChunkList_Iterator<ValType>& b) 
		{
			std::swap(a._container, b._container);
			std::swap(a._value, b._value);
			std::swap(a._ind, b._ind);
		};

		friend bool operator==(const ChunkList_Iterator<ValType>& lhs,
			const ChunkList_Iterator<ValType>& rhs) 
		{
			return lhs._value == rhs._value;
		};
		friend bool operator!=(const ChunkList_Iterator<ValType>& lhs,
			const ChunkList_Iterator<ValType>& rhs) 
		{
			return !(lhs._value == rhs._value);
		};

		reference operator*() const { return *_value; };
		pointer operator->() const { return _value; };

		ChunkList_Iterator operator++(int) 
		{
			if (_ind + 1 == _container->size())
			{
				return ChunkList_Iterator();
			}

			_ind++;
			_value = &_container->at(_ind);
			return *this;
		};

		ChunkList_Iterator operator--(int) 
		{
			_ind--;
			if (_ind > -1)
			{
				_value = &_container->at(_ind);
			}

			return *this;
		};

		ChunkList_Iterator& operator++() 
		
		{
			if (_ind + 1 == _container->size()) {
				this->_value = nullptr;
				this->_container = nullptr;
				this->_ind = 0;
				return *this;
			}
			_ind++;
			_value = &_container->at(_ind);
			return *this;
		};

		ChunkList_Iterator& operator--() 
		{
			_ind--;
			if (_ind <= -1)
			{
				return *this;
			}

			_value = &_container->at(_ind);
			return *this;
		};

		ChunkList_Iterator operator+(const difference_type& n) const 
		{
			return ChunkList_Iterator(_container, _ind + n, &_container->at(_ind + n));
		};


		ChunkList_Iterator operator-(const difference_type& n) const 
		{
			return ChunkList_Iterator(_container, _ind - n, &_container->at(_ind - n));
		};

		ChunkList_Iterator& operator+=(const difference_type& n) 
		{
			_ind += n;
			_value = &_container->at(_ind);
			return *this;
		};

		ChunkList_Iterator& operator-=(const difference_type& n) 
		{
			_ind -= n;
			_value = &_container->at(_ind);
			return *this;
		};

		reference operator[](const difference_type& n) 
		{
			return _container[n];
		};

		friend bool operator<(const ChunkList_Iterator<ValType>& lhs,
			const ChunkList_Iterator<ValType>& rhs) 
		{
			return rhs._ind - lhs._ind > 0;
		};
		friend bool operator<=(const ChunkList_Iterator<ValType>& lhs,
			const ChunkList_Iterator<ValType>& rhs) 
		{
			return !(rhs._ind > lhs._ind);
		};
		friend bool operator>(const ChunkList_Iterator<ValType>& lhs,
			const ChunkList_Iterator<ValType>& rhs) 
		{
			return rhs._ind < lhs._ind;
		};
		friend bool operator>=(const ChunkList_Iterator<ValType>& lhs,
			const ChunkList_Iterator<ValType>& rhs) 
		{
			return !(lhs._ind < rhs._ind);
		};
	};

	template <typename value_type>
	class ChunkList_ConstIterator : public ChunkList_Iterator<value_type> {
	public:
		using iterator_category = std::random_access_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using const_pointer = const value_type*;
		using const_reference = const value_type&;
		constexpr ChunkList_ConstIterator() : ChunkList_Iterator<value_type>() {};
		ChunkList_ConstIterator(ChunkListInterface<value_type>* chunk, int _index, value_type* _value) :
			ChunkList_Iterator<value_type>(chunk, _index, _value) {};

		ChunkList_ConstIterator(const ChunkListInterface<value_type>* chunk, int _index, const value_type* _value) :
			ChunkList_Iterator<value_type>(const_cast<ChunkListInterface<value_type>*>(chunk), _index, const_cast<value_type*>(_value)) {};

		ChunkList_ConstIterator(const ChunkList_ConstIterator& other) noexcept = default;

		ChunkList_ConstIterator& operator=(const ChunkList_ConstIterator&) = default;

		~ChunkList_ConstIterator() override = default;

		friend void swap(ChunkList_ConstIterator<value_type>& a, ChunkList_ConstIterator<value_type>& b) 
		{
			std::swap(a._list, b._list);
			std::swap(a._value, b._value);
			std::swap(a._index, b._index);
		};

		friend bool operator==(const ChunkList_ConstIterator<value_type>& lhs,
			const ChunkList_ConstIterator<value_type>& rhs) 
		{
			return lhs._value == rhs._value;
		};
		friend bool operator!=(const ChunkList_ConstIterator<value_type>& lhs,
			const ChunkList_ConstIterator<value_type>& rhs) 
		{
			return !(lhs._value == rhs._value);
		};

		const_reference operator*() const 
		{ 
			return *this->_value;
		};

		const_pointer operator->() const { return this->_value; };
		const_reference operator[](const difference_type& n)
		{
			return this->_container[n];
		};

		ChunkList_ConstIterator operator++(int) 
		{
			if (this->_index + 1 == this->_container->size())
			{
				return ChunkList_ConstIterator();
			}

			this->_index++;
			this->_value = &this->_container->at(this->_index);
			return *this;
		};

		ChunkList_ConstIterator operator--(int) 
		{
			return ChunkList_Iterator<value_type>::operator--(0);
		};

		ChunkList_ConstIterator& operator++() 
		{
			if (this->_index + 1 == this->_container->size()) 
			{
				this->_value = nullptr;
				this->_container = nullptr;
				this->_index = 0;
				return *this;
			}
			this->_index++;
			this->_value = &this->_container->at(this->_index);
			return *this;
		};

		ChunkList_ConstIterator& operator--() 
		{
			if (!(this->_index))
			{
				throw std::exception();
			}
			this->_index--;
			this->_value = &this->_container->at(this->_index);
			return *this;
		};

		ChunkList_ConstIterator operator+(const difference_type& n) const 
		{
			return ChunkList_ConstIterator(this->_container, this->_ind + n, &this->_container->at(this->_ind + n));
		};


		ChunkList_ConstIterator operator-(const difference_type& n) const 
		{
			return ChunkList_ConstIterator(this->_container, this->_ind - n, &this->_container->at(this->_index - n));
		};

		ChunkList_ConstIterator& operator+=(const difference_type& n) 
		{
			this->_ind += n;
			this->_value = &this->_container->at(this->_index);
			return *this;
		};

		ChunkList_ConstIterator& operator-=(const difference_type& n) 
		{
			this->_index -= n;
			this->_value = &this->_container->at(this->_index);
			return *this;
		};

		friend bool operator<(const ChunkList_ConstIterator<value_type>& lhs,
			const ChunkList_ConstIterator<value_type>& rhs) 
		{
			return rhs._index - lhs._index > 0;
		};

		friend bool operator<=(const ChunkList_ConstIterator<value_type>& lhs,
			const ChunkList_ConstIterator<value_type>& rhs) 
		{
			return !(rhs._index > lhs._index);
		};

		friend bool operator>(const ChunkList_ConstIterator<value_type>& lhs,
			const ChunkList_ConstIterator<value_type>& rhs) 
		{
			return rhs._index < lhs._index;
		};

		friend bool operator>=(const ChunkList_ConstIterator<value_type>& lhs,
			const ChunkList_ConstIterator<value_type>& rhs) 
		{
			return !(lhs._index < rhs._index);
		};
	};

	template <typename value_type, int N, typename Allocator = Allocator<value_type>>
	class ChunkList : public ChunkListInterface<value_type> {
		class Chunk {
		public:
			value_type* list = nullptr;
			Chunk* prev = nullptr;
			Chunk* next = nullptr;
			int node_size = 0;
			Allocator allocator;
			Chunk(const Allocator& alloc = Allocator()) : allocator(alloc) 
			{
				list = allocator.allocate(N);
			}

			Chunk(Chunk* other)
			{
				list = allocator.allocate(N);
				for (size_type i = 0; i < N; i++)
				{
					list[i] = other->list[i];
				}

				node_size = other->node_size;
			}
		};
	protected:
		Chunk* _first_chunk = nullptr;
		int _size = 0;
	public:
		using allocator_type = Allocator;
		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = value_type*;
		using const_pointer = const pointer;
		using iterator = ChunkList_Iterator<value_type>;
		using const_iterator = ChunkList_ConstIterator<value_type>;

		/// Default constructor. Constructs an empty _container with a
		/// default-constructed allocator.
		ChunkList() : _first_chunk(new Chunk()) {};

		/// Constructs the _container with count copies of elements with _value
		/// and with the given allocator
		ChunkList(size_type count, const value_type& _value = value_type(), const Allocator& alloc = Allocator())
			: _first_chunk(new Chunk())
		{
			for (size_type i = 0; i < count; i++)
			{
				push_back(_value);
			}
		};

		/// Constructs the _container with count default-inserted instances of
		/// value_type. No copies are made.
		explicit ChunkList(size_type count, const Allocator& alloc = Allocator())
			: _first_chunk(new Chunk())
		{
			for (size_type i = 0; i < count; i++)
			{
				push_back(value_type());
			}
		};

		/// Constructs the _container with the contents of the range [_first_chunk,
		/// last).
		template <class InputIt>
		ChunkList(InputIt first, InputIt last, const Allocator& alloc = Allocator()) 
		{
			if (first == last) return;
			this->_first_chunk = new Chunk(alloc);

			auto it = first;

			for (; it != last; ++it)
			{
				push_back(*it);
			}
		};

		/// Copy constructor. Constructs the _container with the copy of the
		/// contents of other.
		ChunkList(const ChunkList& other)
		{

			Chunk* otherNode = other._first_chunk;
			if (otherNode == nullptr) return;
			_first_chunk = new Chunk(otherNode);
			Chunk* node = _first_chunk;

			while (otherNode->next != nullptr)
			{
				node->next = new Chunk(otherNode->next);
				node = node->next;
				otherNode = otherNode->next;
			}

			_size = other._size;
		};

		/// Constructs the _container with the copy of the contents of other,
		/// using alloc as the allocator.
		ChunkList(const ChunkList& other, const Allocator& alloc) 
		{
			this = ChunkList(other);
			Chunk* tmp = this->_first_chunk;
			while (tmp != nullptr) {
				tmp->allocator = alloc;
				tmp = tmp->next;
			}
		};

		// Move constructor.
		// Constructs the _container with the contents of other using move semantics.
		// Allocator is obtained by move-construction from the allocator belonging to
		// other.
		ChunkList(ChunkList&& other) 
		{
			_first_chunk = std::move(other._first_chunk);
			_size = std::move(other._size);
			other.clear();
		};

		// Allocator-extended move constructor.
		// Using alloc as the allocator for the new _container, moving the contents
		// from other; if alloc != other.get_allocator(), this results in an
		// element-wise move.
		ChunkList(ChunkList&& other, const Allocator& alloc) 
		{
			this = ChunkList(other);
			Chunk* tmp = _first_chunk;
			while (tmp != nullptr) {
				tmp->allocator = alloc;
				tmp = tmp->next;
			}
		};

		/// Constructs the _container with the contents of the initializer list
		/// init.
		ChunkList(std::initializer_list<value_type> init, const Allocator& alloc = Allocator()) 
		{
			if (init.size() == 0) return;
			_first_chunk = new Chunk(alloc);

			auto it = init.begin();

			for (; it != init.end(); ++it)
			{
				push_back(*it);
			}
		};

		/// Destructs the ChunkList.
		~ChunkList() override 
		{
			clear();
		};

		/// Copy assignment operator. Replaces the contents with a copy of the
		/// contents of other.
		ChunkList& operator=(const ChunkList& other) 
		{
			(this) = new ChunkList(other);
			return (*this);
		};

		// Move assignment operator.
		// Replaces the contents with those of other using move semantics
		// (i.e. the data in other is moved from other into this _container).
		// other is in a valid but unspecified state afterwards.
		ChunkList& operator=(ChunkList&& other) 
		{
			if (this == &other)
				return this;
			_first_chunk = std::move(other._first_chunk);
			_size = std::move(other._size);
			other.clear();

			return *this;
		};

		/// Replaces the contents with those identified by initializer list
		/// ilist.
		ChunkList& operator=(std::initializer_list<value_type> ilist) 
		{
			auto it = ilist.begin();

			for (; it != ilist.end(); ++it)
			{
				push_back(*it);
			}

			return *this;
		};

		/// Replaces the contents with count copies of _value
		void assign(size_type count, const value_type& _value) {
			clear();
			for (size_type i = 0; i < count; i++) push_back(_value);
			_size = count;
		};

		/// Replaces the contents with copies of those in the range [_first_chunk,
		/// last).
		template <class InputIt>
		void assignIt(InputIt _first_chunk, InputIt last) 
		{
			if (_first_chunk == last) return;
			clear();

			auto it = _first_chunk;

			for (; it != last; ++it)
			{
				push_back(*it);
			}
		};

		/// Replaces the contents with the elements from the initializer list
		void assign(std::initializer_list<value_type> ilist) {
			if (ilist.size() == 0) return;
			clear();

			auto it = ilist.begin();

			for (; it != ilist.end(); ++it)
			{
				push_back(*it);
			}
		};

		/// Returns the allocator associated with the _container.
		allocator_type get_allocator() const noexcept 
		{
			return _first_chunk->allocator;
		};

		Chunk* last_chunk() 
		{
			Chunk* tmp = _first_chunk;
			while (tmp->next != nullptr)
			{
				tmp = tmp->next;
			}
			return tmp;
		}

		/// ELEMENvalue_type ACCESS

		/// Returns a reference to the element at specified location pos, with
		/// bounds checking. If pos is not within the range of the _container, an
		/// exception of type std::out_of_range is thrown.
		reference at(size_type pos) override 
		{
			int chunk_index = pos / N;
			int elemnt_index = pos % N;
			if (pos >= max_size() || pos < 0)
			{
				throw std::out_of_range("Out of range");
			}

			Chunk* tmp = _first_chunk;
			while (chunk_index > 0) {
				tmp = tmp->next;
				chunk_index--;
			}
			return tmp->list[elemnt_index];
		};

		/// Returns a const reference to the element at specified location pos,
		/// with bounds checking. If pos is not within the range of the _container, an
		/// exception of type std::out_of_range is thrown.
		const_reference at(size_type pos) const {
			int chunk_index = pos / N;
			int elemnt_index = pos % N;
			if (pos >= max_size() || pos < 0)
			{
				throw std::out_of_range("Out of range");
			}

			Chunk* tmp = _first_chunk;
			while (chunk_index > 0) {
				tmp = tmp->next;
				chunk_index--;
			}
			return tmp->list[elemnt_index];
		};

		/// Returns a reference to the element at specified location pos. No
		/// bounds checking is performed.
		reference operator[](size_type pos) override 
		{
			return at(pos);
		};

		/// Returns a const reference to the element at specified location pos.
		/// No bounds checking is performed.
		const_reference operator[](size_type pos) const 
		{
			return at(pos);
		};

		/// Returns a reference to the _first_chunk element in the _container.
		/// Calling front on an empty _container is undefined.
		reference front() 
		{
			if (!_size)
			{
				throw std::logic_error("Empty container");
			}

			return _first_chunk->list[0];
		};

		/// Returns a const reference to the _first_chunk element in the _container.
		/// Calling front on an empty _container is undefined.
		const_reference front() const 
		{
			if (!_size)
			{
				throw std::logic_error("Empty container");
			}
			return _first_chunk->_container[0];
		};

		/// Returns a reference to the last element in the _container.
		/// Calling back on an empty _container causes undefined behavior.
		reference back() 
		{
			if (!_size)
			{
				throw std::logic_error("Empty container");
			}

			Chunk* tmp = last_chunk();
			while (tmp->next != nullptr)
			{
				tmp = tmp->next;
			}

			return tmp->list[tmp->node_size - 1];
		};

		/// Returns a const reference to the last element in the _container.
		/// Calling back on an empty _container causes undefined behavior.
		const_reference back() const 
		{
			if (!_size) throw std::logic_error("Empty");

			Chunk* tmp = last_chunk();
			return tmp->_container[tmp->node_size - 1];
		};

		/// Ivalue_typeERAvalue_typeORS

		/// Returns an iterator to the _first_chunk element of the ChunkList.
		/// If the ChunkList is empty, the returned iterator will be equal to end().
		iterator begin() noexcept 
		{ 
			return ChunkList_Iterator<value_type>(this, 0, &at(0)); 
		};

		/// Returns an iterator to the _first_chunk element of the ChunkList.
		/// If the ChunkList is empty, the returned iterator will be equal to end().
		const_iterator begin() const noexcept
		{ 
			return ChunkList_ConstIterator<value_type>(this, 0, &at(0));
		};

		/// Same to begin()
		const_iterator cbegin() const noexcept
		{ 
			return begin(); 
		};

		/// Returns an iterator to the element following the last element of
		/// the ChunkList. value_typehis element acts as a placeholder; attempting to access it
		/// results in undefined behavior.
		iterator end() noexcept
		{ 
			return ChunkList_Iterator<value_type>(); 
		};

		/// Returns an constant iterator to the element following the last
		/// element of the ChunkList. value_typehis element acts as a placeholder; attempting to
		/// access it results in undefined behavior.
		const_iterator end() const noexcept 
		{ 
			return ChunkList_ConstIterator<value_type>(); 
		};

		/// Same to end()
		const_iterator cend() const noexcept 
		{ 
			return end(); 
		};

		/// CAPACITY

		/// Checks if the _container has no elements
		bool empty() const noexcept 
		{ 
			return !_size; 
		};

		/// Returns the number of elements in the _container
		size_type size() const noexcept override 
		{ 
			return _size; 
		};

		/// Returns the maximum number of elements the _container is able to
		/// hold due to system or library implementation limitations
		size_type max_size() const noexcept 
		{
			int r = _size % N;
			return (r == 0 ? _size : _size + N - r);
		};

		/// Requests the removal of unused capacity.
		/// It is a non-b_inding request to reduce the memory usage without changing
		/// the size of the sequence. All iterators and references are invalidated.
		/// Past-the-end iterator is also invalidated.
		void shrink_to_fit() 
		{
			Chunk* tmp = last_chunk();
			while (tmp->node_size == 0)
			{
				tmp = tmp->prev;
				tmp->next = nullptr;
			}
		};

		/// MODIFIERS

		/// Erases all elements from the _container.
		/// nvalidates any references, pointers, or iterators referring to contained
		/// elements. Any past-the-end iterators are also invalidated.
		void clear() noexcept 
		{
			Chunk* cur = _first_chunk;
			while (cur != nullptr) {
				Chunk* tmp = cur;
				cur = cur->next;
				delete tmp;
			}
			_size = 0;
			_first_chunk = nullptr;
		};


		/// Inserts _value before pos.
		iterator insert(const_iterator pos, const value_type& _value) 
		{
			push_back(value_type());
			iterator it = iterator(this, _size - 1, &at(_size - 1));

			while (true)
			{
				*it = *(it - 1);
				it--;
				if (it == pos) break;
			}

			iterator it2 = iterator(this, pos.get_index(), &at(pos.get_index()));
			(*it2) = _value;
			return it2;
		};

		/// Inserts _value before pos.
		iterator insert(const_iterator pos, value_type&& _value) 
		{
			push_back(value_type());
			iterator it = iterator(this, _size - 1, &at(_size - 1));

			while (true)
			{
				*it = *(it - 1);
				it--;
				if (it == pos) break;
			}

			iterator it2 = iterator(this, pos.get_index(), &at(pos.get_index()));
			(*it2) = std::move(_value);
			return it2;
		};

		/// Inserts count copies of the _value before pos.
		iterator insert(const_iterator pos, size_type count, const value_type& _value)
		{
			if (count == 0) return pos;

			int _index = pos.get_index();

			for (int i = 0; i < count; i++) insert(pos, _value);

			return ChunkList_Iterator<value_type>(this, _index, at(_index));
		};

		/// Inserts elements from range [_first_chunk, last) before pos.
		template <class InputIt>
		iterator insert(const_iterator pos, InputIt _first_chunk, InputIt last) 
		{
			if (_first_chunk == last) return pos;

			int _index = pos.get_index();
			auto it = last - 1;
			while (true)
			{
				insert(pos, *it);
				if (it == _first_chunk) break;
				it--;
			}

			return ChunkList_Iterator<value_type>(this, _index, &at(_index));
		};

		/// Inserts elements from initializer list before pos.
		iterator insert(const_iterator pos, std::initializer_list<value_type> ilist)
		{
			if (ilist.size() == 0) return pos;
			int _index = pos.get_index();
			auto it = ilist.end() - 1;

			while (true)
			{
				insert(pos, *it);
				if (it == ilist.begin()) break;
				it--;
			}

			return ChunkList_Iterator<value_type>(this, _index, &at(_index));
		};


		/// Inserts a new element into the _container directly before pos.
		template <class... Args>
		iterator emplace(const_iterator pos, Args&&... args) 
		{
			auto data = { std::forward<Args>(args)... };
			return insert(pos, data);
		};

		/// Removes the element at pos.
		iterator erase(const_iterator pos) 
		{
			size_type _index = pos.get_index();

			iterator it = iterator(this, pos.get_index(), &at(pos.get_index()));
			while (true)
			{
				*it = *(it + 1);
				it++;
				if (it.get_index() == _size - 1) break;
			}
			_size--;

			return ChunkList_Iterator<value_type>(this, _index, &at(_index));
		};

		/// Removes the elements in the range [_first_chunk, last).
		iterator erase(const_iterator _first_chunk, const_iterator last) 
		{
			size_type diff = last.get_index() - _first_chunk.get_index();

			for (size_type i = 0; i < diff; i++)
			{
				erase(_first_chunk + 1);
			}

			return ChunkList_Iterator<value_type>(this, _first_chunk.get_index(), &at(_first_chunk.get_index()));
		};

		/// Appends the given element _value to the end of the _container.
		/// value_typehe new element is initialized as a copy of _value.
		void push_back(const value_type& _value) 
		{
			if (_first_chunk == nullptr)
			{
				_first_chunk = new Chunk();
			}

			Chunk* tmp = last_chunk();
			if (tmp->node_size == N)
			{
				tmp->next = new Chunk();
				auto tmpPrev = tmp;
				tmp = tmp->next;
				tmp->prev = tmpPrev;
			}
			tmp->list[tmp->node_size] = _value;
			tmp->node_size++;
			_size++;
		};

		/// Appends the given element _value to the end of the _container.
		/// Value is moved into the new element.
		void push_back(value_type&& _value) 
		{
			if (_first_chunk == nullptr)
			{
				_first_chunk = new Chunk();
			}

			Chunk* tmp = last_chunk();
			if (tmp->node_size == N)
			{
				tmp->next = new Chunk();
				tmp = tmp->next;
			}

			tmp->list[tmp->node_size] = std::move(_value);
			tmp->node_size++;
			_size++;
		};

		/// Appends a new element to the end of the _container.
		template <class... Args>
		reference emplace_back(Args&&... args) 
		{
			auto data = { std::forward<Args>(args)... };
			auto it = data.begin();
			for (; it != data.end(); it++)
			{
				push_back(*it);
			}
			return back();
		};

		/// @brief Removes the last element of the _container.
		void pop_back() 
		{
			this->_size--;
			Chunk* tmp = last_chunk();
			tmp->node_size--;
		};

		/// Prepends the given element _value to the beginning of the _container.
		/// _value the _value of the element to prepend
		void push_front(const value_type& _value) 
		{
			insert(cbegin(), _value);
		};

		/// Prepends the given element _value to the beginning of the _container.
		void push_front(value_type&& _value) 
		{
			insert(cbegin(), std::move(_value));
		};

		/// Inserts a new element to the beginning of the _container.
		template <class... Args>
		reference emplace_front(Args&&... args) 
		{
			auto data = { std::forward<Args>(args)... };
			auto it = data.end() - 1;
			while (true)
			{
				insert(cbegin(), *it);
				if (it == data.begin()) break;
				it--;
			}
			return back();
		};

		/// Removes the _first_chunk element of the _container.
		void pop_front() 
		{
			erase(cbegin());
		};

		/// Resizes the _container to contain count elements.
		/// If the current size is greater than count, the _container is reduced to its
		/// _first_chunk count elements. If the current size is less than count, additional
		/// default-inserted elements are appended
		void resize(size_type count) 
		{
			if (count == _size) return;
			if (count < _size)
			{
				while (_size != count)
				{
					pop_back();
				}
			}
			else
			{
				while (_size != count)
				{
					push_back(value_type());
				}
			}
		};

		/// Resizes the _container to contain count elements.
		/// If the current size is greater than count, the _container is reduced to its
		/// _first_chunk count elements. If the current size is less than count, additional
		/// copies of _value are appended.
		void resize(size_type count, const value_type& _value) {
			if (count == _size) return;
			if (count < _size)
			{
				while (_size != count)
				{
					pop_back();
				}
			}
			else
			{
				while (_size != count)
				{
					push_back(_value);
				}
			}
		};

		void swap(ChunkList& other) 
		{
			Chunk* _first_chunk_tmp = other._first_chunk;
			int size_tmp = other._size;

			other._first_chunk = _first_chunk;
			other._size = _size;

			_first_chunk = _first_chunk_tmp;
			_size = size_tmp;
		};

		friend bool operator==(const ChunkList& lhs,
			const ChunkList& rhs) 
		{
			if (lhs._size != rhs._size)
				return false;

			for (int i = 0; i < lhs._size; i++)
				if (lhs.at(i) != rhs.at(i))
					return false;

			return true;
		};

		template <class U, int N, class Alloc>
		friend bool operator!=(const ChunkList& lhs,
			const ChunkList& rhs) 
		{
			return !operator==(lhs, rhs);
		};

		friend bool operator>(const ChunkList& lhs, const ChunkList& rhs) 
		{
			if (lhs._size < rhs._size) return false;

			if (lhs._size > rhs._size) return true;

			for (int i = 0; i < lhs._size; i++)
				if (rhs.at(i) < lhs.at(i))
					return true;

			return false;
		};

		friend bool operator<(const ChunkList& lhs, const ChunkList& rhs) 
		{
			if ((lhs > rhs) || (lhs == rhs)) return false;
			return true;
		};

		friend bool operator>=(const ChunkList& lhs,
			const ChunkList& rhs) 
		{
			return !operator<(lhs, rhs);
		};

		friend bool operator<=(const ChunkList& lhs,
			const ChunkList& rhs) 
		{
			return !operator>(lhs, rhs);
		};
	};

	template <class T, int N, class Alloc>
	void swap(ChunkList<T, N, Alloc>& lhs, ChunkList<T, N, Alloc>& rhs);


	template <class T, int N, class Alloc, class U>
	typename ChunkList<T, N, Alloc>::size_type erase(ChunkList<T, N, Alloc>& c, const U& _value);


	template <class T, int N, class Alloc, class Pred>
	typename ChunkList<T, N, Alloc>::size_type erase_if(ChunkList<T, N, Alloc>& c, Pred pred);
}