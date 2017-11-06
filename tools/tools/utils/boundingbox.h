/***********************************************************************
* Software License Agreement (BSD License)
*
* Copyright 2017	Wolfgang Brandenburger
*					(w.brandenburger@unibw.de).
*					All rights reserved.
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

#ifndef UTILS_BOUNDINGBOX_H_
#define UTILS_BOUNDINGBOX_H_

namespace utils
{
	template<typename ElementType> struct BoundingBox 
	{
	public:

		/**
			Constructor

			@param[in] dataset_ Dataset
			@param[in] number_of_elements_ Number of elements
			@param[in] dim_ Specific dimension
		*/
		BoundingBox() : min(nullptr), max(nullptr), dim(NULL) {}

		/**
			Constructor

			@param[in] dataset_ Dataset
			@param[in] number_of_elements_ Number of elements
			@param[in] dim_ Specific dimension
		*/
		BoundingBox(ElementType* dataset_, size_t number_of_elements, size_t dim_) :
			min(nullptr), max(nullptr), dim(dim_)
		{
			min = new ElementType[dim];
			max = new ElementType[dim];

			std::memcpy(min, dataset_, sizeof(ElementType)*dim);
			std::memcpy(max, dataset_, sizeof(ElementType)*dim);

			ElementType* dataset_ptr = dataset_;
			for (size_t i = 0; i < number_of_elements * dim; i++)
			{
				setCompareValue(*dataset_ptr, i % dim);
				dataset_ptr++;
			}
		}

		/**
			Destructor
		*/
		~BoundingBox()
		{
			clear();
		}

		/**
			Copy Constructor

			@param[in] bounding_box_ Bounding Box
		*/
		BoundingBox(const BoundingBox<ElementType>& bounding_box_) : min(nullptr), max(nullptr), dim(NULL)
		{
			dim = bounding_box_.getDim();

			min = new ElementType[dim];
			max = new ElementType[dim];

			std::memcpy(min, bounding_box_.getMin(), sizeof(ElementType)*dim);
			std::memcpy(max, bounding_box_.getMax(), sizeof(ElementType)*dim);
		}
		
		/**
			Copy Constructor

			@param[in] bounding_box_ Bounding Box
		*/
		BoundingBox(const BoundingBox<ElementType>&& bounding_box_) : min(nullptr), max(nullptr), dim(NULL)
		{
			dim = bounding_box_.getDim();

			min = new ElementType[dim];
			max = new ElementType[dim];

			std::memcpy(min, bounding_box_.getMin(), sizeof(ElementType)*dim);
			std::memcpy(max, bounding_box_.getMax(), sizeof(ElementType)*dim);
		}

		/**
			Operator =

			@param[in] bounding_box_ Bounding Box
		*/
		BoundingBox& operator=(const BoundingBox<ElementType>& bounding_box_)
		{
			clear();

			dim = bounding_box_.getDim();

			min = new ElementType[dim];
			max = new ElementType[dim];

			std::memcpy(min, bounding_box_.getMin(), sizeof(ElementType)*dim);
			std::memcpy(max, bounding_box_.getMax(), sizeof(ElementType)*dim);

			return (*this);
		}

		/**
			Operator =

			@param[in] bounding_box_ Bounding Box
		*/
		BoundingBox& operator=(const BoundingBox<ElementType>&& bounding_box_)
		{
			clear();

			dim = bounding_box_.getDim();

			min = new ElementType[dim];
			max = new ElementType[dim];

			std::memcpy(min, bounding_box_.getMin(), sizeof(ElementType)*dim);
			std::memcpy(max, bounding_box_.getMax(), sizeof(ElementType)*dim);

			return (*this);
		}

		/**
			Clear
		*/
		void clear()
		{
			if (min) {
				delete[] min;
				min = nullptr;
			}

			if (max) {
				delete[] max;
				max = nullptr;
			}
			dim = NULL;
		}

		/**
			Set the bounding box

			@param[in] dataset_ Dataset
			@param[in] number_of_elements_ Number of elements
			@param[in] dim_ Specific dimension
		*/
		void setBoundingBox(ElementType* dataset_, size_t number_of_elements, size_t dim_)
		{
			if (dim && dim != dim_) {
				std::cout << "Exit in " << __FILE__ << " in line " << __LINE__ << std::endl;
				std::exit(EXIT_FAILURE);
			}
			
			dim = dim_;
			
			if (!min) {
				min = new ElementType[dim];
				std::memcpy(min, dataset_, sizeof(ElementType)*dim);
			}
			if (!max) {
				max = new ElementType[dim];
				std::memcpy(max, dataset_, sizeof(ElementType)*dim);
			}

			for (size_t i = 0; i < number_of_elements * dim; i++ )
			{
				setCompareValue(dataset_[i], i % dim);
			}
		}

		/**
			Get minimum value for specific dimension

			@param[in] dim_ Specific dimension
		*/
		ElementType getMinDim(size_t dim_) const
		{
			return min[dim_];
		}

		/**
			Get maximum value for specific dimension

			@param[in] dim_ Specific dimension
		*/
		ElementType getMaxDim(size_t dim_) const
		{
			return max[dim_];
		}

		/**
			Get dimension
		*/
		size_t getDim() const
		{
			return dim;
		}

		/**
			Get minimum values
		*/
		ElementType* getMin() const
		{
			return min;
		}

		/**
			Get maximum values
		*/
		ElementType* getMax() const
		{
			return max;
		}	

		/**
			Get difference for a specific dimension
		*/
		ElementType getDifference(size_t dim_) const
		{
			return max[dim_] - min[dim_];
		}

		/**
			Get difference for a specific dimension
		*/
		ElementType getMiddle(size_t dim_) const
		{
			return (max[dim_] + min[dim_]) / (ElementType)2.0;
		}

		/**
			Set value for specific dimension

			@param[in] value_ Value
			@param[in] dim_ Specific dimension
		*/
		void setValue(ElementType value_, size_t dim_)
		{
			min[dim_] = value_;
			max[dim_] = value_;
		}

		/**
			Compare and set value for specific dimension

			@param[in] value_ Value
			@param[in] dim_ Specific dimension
		*/
		void setCompareValue(ElementType value_, size_t dim_)
		{
			if (value_ < min[dim_]) {
				min[dim_] = value_;
			}

			if (value_ > max[dim_]) {
				max[dim_] = value_;
			}
		}

	private:

		/**
			Minimum values
		*/
		ElementType* min;

		/**
			Maximum values
		*/
		ElementType* max;

		/**
			Dimension
		*/
		size_t dim;
	};

	/**
		Operator << Prints the values of the bounding box

		@param[in,out] out_ Outstream in which the bounding box will be printed
		@param[in] bounding_box_ Bounding box which shall be printed
	*/
	template<typename ElementType>
	std::ostream& operator<<(std::ostream& out_, const utils::BoundingBox<ElementType>& bounding_box_)
	{
		for (size_t i = 0; i <  bounding_box_.getDim(); i++) {
			out_ << bounding_box_.getMinDim(i) << " " 
				<< bounding_box_.getMaxDim(i) << " " 
				<< bounding_box_.getDifference(i) << " " 
				<< bounding_box_.getMiddle(i) << std::endl;
		}
		return out_;
	}
}

#endif /* UTILS_BOUNDINGBOX_H_ */