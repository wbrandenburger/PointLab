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

#ifndef UTILS_ANY_H_
#define UTILS_ANY_H_

#include <stdexcept>
#include <ostream>
#include <typeinfo>

namespace utils
{

	namespace anyimpl
	{

		struct BadAnyCast : public std::runtime_error
		{
			BadAnyCast() : std::runtime_error("Cannot convert 'any' value") { }
		};

		struct EmptyAny {};

		inline std::ostream& operator <<(std::ostream& out, const EmptyAny&)
		{
			out << "[empty_any]";
			return out;
		}

		struct BaseAnyPolicy
		{
			virtual void staticDelete(void** x) = 0;
			virtual void copyFromValue(void const* src, void** dest) = 0;
			virtual void clone(void* const* src, void** dest) = 0;
			virtual void move(void* const* src, void** dest) = 0;
			virtual void* getValue(void** src) = 0;
			virtual const void* getValue(void* const * src) = 0;
			virtual ::size_t getSize() = 0;
			virtual const std::type_info& type() = 0;
			virtual void print(std::ostream& out, void* const* src) = 0;
		};

		template<typename T> struct TypedBaseAnyPolicy : BaseAnyPolicy
		{
			virtual ::size_t getSize() { return sizeof(T); }
			virtual const std::type_info& type() { return typeid(T); }

		};

		template<typename T> struct SmallAnyPolicy : TypedBaseAnyPolicy<T>
		{
			virtual void staticDelete(void**) { }
			virtual void copyFromValue(void const* src, void** dest)
			{
				new (dest) T(* reinterpret_cast<T const*>(src));
			}
			virtual void clone(void* const* src, void** dest) { *dest = *src; }
			virtual void move(void* const* src, void** dest) { *dest = *src; }
			virtual void* getValue(void** src) { return reinterpret_cast<void*>(src); }
			virtual const void* getValue(void* const * src) { return reinterpret_cast<const void*>(src); }
			virtual void print(std::ostream& out, void* const* src) { out << *reinterpret_cast<T const*>(src); }
		};

		template<typename T> struct BigAnyPolicy : TypedBaseAnyPolicy<T>
		{
			virtual void staticDelete(void** x)
			{
				if (* x) delete (* reinterpret_cast<T**>(x)); *x = NULL;
			}
			virtual void copyFromValue(void const* src, void** dest)
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
			virtual void* getValue(void** src) { return *src; }
			virtual const void* getValue(void* const * src) { return *src; }
			virtual void print(std::ostream& out, void* const* src) { out << *reinterpret_cast<T const*>(*src); }
		};

		template<typename T>
		struct ChoosePolicy
		{
			typedef BigAnyPolicy<T> Type;
		};

		template<typename T>
		struct ChoosePolicy<T*>
		{
			typedef SmallAnyPolicy<T*> Type;
		};

		struct any;

		/// Choosing the policy for an any type is illegal, but should never happen.
		/// This is designed to throw a compiler error.
		template<> struct ChoosePolicy<any>
		{
			typedef void Type;
		};

		/// Specializations for small types.
		#define SMALL_ANY_POLICY(TYPE) \
			template<> \
			struct ChoosePolicy<TYPE> { typedef SmallAnyPolicy<TYPE> Type; \
			}

		SMALL_ANY_POLICY(signed char);
		SMALL_ANY_POLICY(unsigned char);
		SMALL_ANY_POLICY(signed short);
		SMALL_ANY_POLICY(unsigned short);
		SMALL_ANY_POLICY(signed int);
		SMALL_ANY_POLICY(unsigned int);
		SMALL_ANY_POLICY(signed long);
		SMALL_ANY_POLICY(unsigned long);
		SMALL_ANY_POLICY(float);
		SMALL_ANY_POLICY(bool);

		/// This function will return a different policy for each type.
		template<typename T> BaseAnyPolicy* getPolicy()
		{
		    static typename ChoosePolicy<T>::Type policy;
			return &policy;
		}
	} 

	class Any
	{

	public:
		/**
			Constructor

			@param[in] x_ Pointer or instance of any abitrary type
		*/
		template <typename T> Any(const T& x_) : policy(anyimpl::getPolicy<anyimpl::EmptyAny>()), object(NULL)
		{
			assign(x_);
		}

		/**
			Constructor
		*/
		Any() : policy(anyimpl::getPolicy<anyimpl::EmptyAny>()), object(NULL) {}
		
		/**
			Special initializing constructor for string literals

			@param[in] x_ Pointer to a string literal
		*/
		Any(const char* x_) : policy(anyimpl::getPolicy<anyimpl::EmptyAny>()), object(NULL)
		{
			assign(x_);
		}

		/**
			Copy Constructor

			@param[in]	x_ Instance of class any
		*/
		Any(const Any& x_) : policy(anyimpl::getPolicy<anyimpl::EmptyAny>()), object(NULL)
		{
			assign(x_);
		}

		/**
			Destructor
		*/
		~Any()
		{
			policy->staticDelete(&object);
		}
		
		/**
			Assignment function from another instance of class any

			@param[in]	x_ Instance of class any
			@return Dereference of this
		*/
		Any& assign(const Any& x_)
		{
			reset();
			policy = x_.policy;
			policy->clone(&x_.object, &object);
			return *this;
	    }

		/**
			Assignment function

			@param[in]	x_ Pointer or instance of any abitrary type
			@return Dereference of this
		*/
	    template <typename T> Any& assign(const T& x_)
	    {
			reset();
			policy = anyimpl::getPolicy<T>();
			policy->copyFromValue(&x_, &object);
			return *this;
		}
	
		/**
			Operator = assignment from  a pointer or instance of any abitrary type

			@param[in]	x_ Pointer or instance of any abitrary type
			@return Dereference of this
		*/
	    template<typename T> Any& operator=(const T& x_)
		{
			return assign(x_);
		}

		/**
			Operator = specialized for literal strings

			@param[in] x_ Pointer to a string literal
			@return Dereference of this
		*/
		Any& operator=(const char* x_)
		{
			return assign(x_);
		}

		/**
			Swap function

			@param[in]	x_ Instance of class any
			@return Dereference of this
		*/
		Any& swap(Any& x_)
		{
			std::swap(policy, x_.policy);
			std::swap(object, x_.object);
			return *this;
		}

		/**
			Cast operator

			@return Pointer or instance which is stored in any
		*/
		template<typename T> T& cast()
		{
			if (policy->type() != typeid(T)) throw anyimpl::BadAnyCast();
			T* r = reinterpret_cast<T*>(policy->getValue(&object));
			return *r;
		}

		/** 
			Cast operator

			@return Pointer or instance which is stored in any
		*/
		template<typename T> const T& cast() const
		{
			if (policy->type() != typeid(T)) throw anyimpl::BadAnyCast();
			const T* r = reinterpret_cast<const T*>(policy->getValue(&object));
			return *r;
		}

		/**
			Returns true if the any contains no value

			@return True if the any contains no value
		*/
		bool empty() const
		{
			return policy->type() == typeid(anyimpl::EmptyAny);
		}

		/**
			Frees any allocated memory, and sets the value to NULL.
		*/
		void reset()
		{
			policy->staticDelete(&object);
			policy = anyimpl::getPolicy<anyimpl::EmptyAny>();
		}

		/**
			Returns true if the two types are the same
		
			@param[in]	x_ Instance of class any
			@return True if the two types are the same
		*/
		bool compatible(const Any& x_) const
		{
			return policy->type() == x_.policy->type();
		}

		/**
			Returns if the type is compatible with the policy

			@return True if the type is compatible with the policy
		*/
		template<typename T>
		bool hasType()
		{
			return policy->type() == typeid(T);
		}
		
		/**
			Returns the type of the pointer or instance stored in object

			@return  Type of the pointer or instance stored in object
		*/
		const std::type_info& type() const
		{
			return policy->type();
		}

		friend std::ostream& operator <<(std::ostream& out, const Any& any_val);


	private:

		/**
			Differentiation whether object contains a pointer or an instance
		*/
		anyimpl::BaseAnyPolicy* policy;
		
		/**
			Pointer which is targeted on a pointer or an instance
		*/
		void* object;
	};

	/**
		Operator << Prints the instance which is stored in any_val

		@param[in,out] out_ Outstream in which the instance will be printed
		@param[in] any_val_ Instance of any which object shall be printed
	*/
	inline std::ostream& operator <<(std::ostream& out_, const Any& any_val_)
	{
		any_val_.policy->print(out_,&any_val_.object);
		return out_;
	}
}

#endif /* UTILS_ANY_H_ */
