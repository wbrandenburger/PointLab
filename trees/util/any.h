/***********************************************************************
* Software License Agreement (BSD License)
*
* Copyright 2005-2011 Christopher Diggins
* Copyright 2005 Pablo Aguilar
* Copyright 2001 Kevlin Henney
*
* THE BSD LICENSE
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
*
* THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
* NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
* THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
* THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*************************************************************************/

#ifndef TREES_ANY_H_
#define TREES_ANY_H_

#include <stdexcept>
#include <ostream>
#include <typeinfo>

namespace trees
{

	namespace anyimpl
	{

		struct bad_any_cast : public std::runtime_error
		{
			bad_any_cast() : std::runtime_error("Cannot convert 'any' value") { }
		};

		struct empty_any {};

		inline std::ostream& operator <<(std::ostream& out, const empty_any&)
		{
			out << "[empty_any]";
			return out;
		}

		struct base_any_policy
		{
			virtual void static_delete(void** x) = 0;
			virtual void copy_from_value(void const* src, void** dest) = 0;
			virtual void clone(void* const* src, void** dest) = 0;
			virtual void move(void* const* src, void** dest) = 0;
			virtual void* get_value(void** src) = 0;
			virtual const void* get_value(void* const * src) = 0;
			virtual ::size_t get_size() = 0;
			virtual const std::type_info& type() = 0;
			virtual void print(std::ostream& out, void* const* src) = 0;
		};

		template<typename T> struct typed_base_any_policy : base_any_policy
		{
			virtual ::size_t get_size() { return sizeof(T); }
			virtual const std::type_info& type() { return typeid(T); }

		};

		template<typename T> struct small_any_policy : typed_base_any_policy<T>
		{
			virtual void static_delete(void**) { }
			virtual void copy_from_value(void const* src, void** dest)
			{
				new (dest) T(* reinterpret_cast<T const*>(src));
			}
			virtual void clone(void* const* src, void** dest) { *dest = *src; }
			virtual void move(void* const* src, void** dest) { *dest = *src; }
			virtual void* get_value(void** src) { return reinterpret_cast<void*>(src); }
			virtual const void* get_value(void* const * src) { return reinterpret_cast<const void*>(src); }
			virtual void print(std::ostream& out, void* const* src) { out << *reinterpret_cast<T const*>(src); }
		};

		template<typename T> struct big_any_policy : typed_base_any_policy<T>
		{
			virtual void static_delete(void** x)
			{
				if (* x) delete (* reinterpret_cast<T**>(x)); *x = NULL;
			}
			virtual void copy_from_value(void const* src, void** dest)
			{
				*dest = new T(*reinterpret_cast<T const*>(src));
			}
			virtual void clone(void* const* src, void** dest)
			{
				*dest = new T(**reinterpret_cast<T* const*>(src));
			}
			virtual void move(void* const* src, void** dest)
			{
				(*reinterpret_cast<T**>(dest))->~T();
				**reinterpret_cast<T**>(dest) = **reinterpret_cast<T* const*>(src);
			}
			virtual void* get_value(void** src) { return *src; }
			virtual const void* get_value(void* const * src) { return *src; }
			virtual void print(std::ostream& out, void* const* src) { /*out << *reinterpret_cast<T const*>(*src);*/ }
		};

		template<typename T>
		struct choose_policy
		{
			typedef big_any_policy<T> type;
		};

		template<typename T>
		struct choose_policy<T*>
		{
			typedef small_any_policy<T*> type;
		};

		struct any;

		/// Choosing the policy for an any type is illegal, but should never happen.
		/// This is designed to throw a compiler error.
		template<> struct choose_policy<any>
		{
			typedef void type;
		};

		/// Specializations for small types.
		#define SMALL_POLICY(TYPE) \
			template<> \
			struct choose_policy<TYPE> { typedef small_any_policy<TYPE> type; \
			}

		SMALL_POLICY(signed char);
		SMALL_POLICY(unsigned char);
		SMALL_POLICY(signed short);
		SMALL_POLICY(unsigned short);
		SMALL_POLICY(signed int);
		SMALL_POLICY(unsigned int);
		SMALL_POLICY(signed long);
		SMALL_POLICY(unsigned long);
		SMALL_POLICY(float);
		SMALL_POLICY(bool);

		/// This function will return a different policy for each type.
		template<typename T> base_any_policy* get_policy()
		{
		    static typename choose_policy<T>::type policy;
			return &policy;
		}
	} 

	class any
	{

	public:
		/// Initializing constructor.
		template <typename T>
		any(const T& x) : policy(anyimpl::get_policy<anyimpl::empty_any>()), object(NULL)
		{
			assign(x);
		}

		/// Empty constructor.
		any() : policy(anyimpl::get_policy<anyimpl::empty_any>()), object(NULL) {}

		/// Special initializing constructor for string literals.
		any(const char* x) : policy(anyimpl::get_policy<anyimpl::empty_any>()), object(NULL)
		{
			assign(x);
		}

		/// Copy constructor.
		any(const any& x) : policy(anyimpl::get_policy<anyimpl::empty_any>()), object(NULL)
		{
			assign(x);
		}

		/// Destructor.
		~any()
		{
			policy->static_delete(&object);
		}

		/// Assignment function from another any.
		any& assign(const any& x)
		{
			reset();
			policy = x.policy;
			policy->clone(&x.object, &object);
			return *this;
	    }

	    /// Assignment function.
	    template <typename T> any& assign(const T& x)
	    {
			reset();
			policy = anyimpl::get_policy<T>();
			policy->copy_from_value(&x, &object);
			return *this;
		}
	
	    /// Assignment operator.
	    template<typename T> any& operator=(const T& x)
		{
			return assign(x);
		}

		/// Assignment operator, specialed for literal strings.
		/// They have types like const char [6] which don't work as expected.
		any& operator=(const char* x)
		{
			return assign(x);
		}

    /// Utility functions
		any& swap(any& x)
		{
			std::swap(policy, x.policy);
			std::swap(object, x.object);
			return *this;
		}

		/// Cast operator. You can only cast to the original type.
		template<typename T> T& cast()
		{
			if (policy->type() != typeid(T)) throw anyimpl::bad_any_cast();
			T* r = reinterpret_cast<T*>(policy->get_value(&object));
			return *r;
		}

		/// Cast operator. You can only cast to the original type.
		template<typename T> const T& cast() const
		{
			if (policy->type() != typeid(T)) throw anyimpl::bad_any_cast();
			const T* r = reinterpret_cast<const T*>(policy->get_value(&object));
			return *r;
		}

		/// Returns true if the any contains no value.
		bool empty() const
		{
			return policy->type() == typeid(anyimpl::empty_any);
		}

		/// Frees any allocated memory, and sets the value to NULL.
		void reset()
		{
			policy->static_delete(&object);
			policy = anyimpl::get_policy<anyimpl::empty_any>();
		}

		/// Returns true if the two types are the same.
		bool compatible(const any& x) const
		{
			return policy->type() == x.policy->type();
		}

		/// Returns if the type is compatible with the policy
		template<typename T>
		bool has_type()
		{
			return policy->type() == typeid(T);
		}

		const std::type_info& type() const
		{
			return policy->type();
		}

		friend std::ostream& operator <<(std::ostream& out, const any& any_val);


	private:

		anyimpl::base_any_policy* policy;
		
		void* object;
	};

	inline std::ostream& operator <<(std::ostream& out, const any& any_val)
	{
		any_val.policy->print(out,&any_val.object);
		return out;
	}
}

#endif /* TREES_ANY_H_ */
