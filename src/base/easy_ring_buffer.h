/********************************************************************
created:	2011/11/21
created:	21:11:2011   9:49
file base:	easy_ring_buffer
file ext:	h
author:		Lee

purpose:	a ring buffer 
*********************************************************************/
#ifndef easy_ring_buffer_h__
#define easy_ring_buffer_h__
#include <string>

#if 0
#ifndef easy_base_type_h__
#include "easy_base_type.h"
#endif //easy_base_type_h__
#else
#include "easy_assist.h"
#endif

#define EXTRA_BUFFER_SIZE		64

namespace easy
{
	template<class _Type,class _Alloc >
	class EasyRingbuffer 
	{
	public:
		typedef _Alloc allocator_type;

		explicit  EasyRingbuffer(size_t size):
		size_(size),
			wpos_(0),
			rpos_(0)
		{
			buffer_ = _allocate(size_);
		}

		~EasyRingbuffer() { _deallocate(buffer_,size_); }

		template<typename T> void append(T val) 
		{ 
			append((easy_uint8*)&val,sizeof(val));
#ifdef _DEBUG
			std::cout << "append---buffer size = " << size_ << " rpos_ = " << rpos_ << " wpos_ = " << wpos_ << std::endl;
#endif // _DEBUG
		}

		void append(const easy_uint8* src, size_t cnt)
		{
			if (!cnt)
			{
				return;
			}

			//	case 1: rpos_ <= wpos_
			if (rpos_ <= wpos_)
			{
				if (size_ - wpos_ >= cnt)
				{
					memmove(buffer_ + wpos_,src,cnt);
					wpos_ += cnt;
					return;
				}
				else
				{
					if (size_ - wpos_ + rpos_ > cnt)	// >= is ok>
					{
						memmove(buffer_ + wpos_, src, size_ - wpos_);
						memmove(buffer_, src + size_ - wpos_, cnt - (size_ - wpos_));
						wpos_ = cnt - (size_ - wpos_);
						return;
					}
					else
					{
						_Type* new_buffer = _allocate(size_ + cnt - (size_ - wpos_));
						memmove(new_buffer,buffer_,wpos_);
						memmove(new_buffer + wpos_, src, cnt);
						_deallocate(buffer_,size_);
						size_ = size_ + cnt - (size_ - wpos_);
						wpos_ += cnt;
						buffer_ = new_buffer;
						return;
					}
				}
			}
			//	case 2: rpos_ > wpos_ 
			else if(rpos_ > wpos_)
			{
				if (rpos_ - wpos_ > cnt)	// >= is ok ?
				{
					if (rpos_ - wpos_ > cnt)
					{
						memmove(buffer_ + wpos_,src,cnt);
						wpos_ += cnt;
						return;
					}
					else
					{
						_Type* new_buffer = _allocate(size_ + cnt - (rpos_ - wpos_) + EXTRA_BUFFER_SIZE);
						memmove(new_buffer,buffer_,wpos_);
						memmove(new_buffer + wpos_,src,cnt);
						memmove(new_buffer + wpos_ + cnt - (rpos_ - wpos_) + EXTRA_BUFFER_SIZE,buffer_ + rpos_,size_ - rpos_);
						_deallocate(buffer_,size_);
						rpos_ += cnt - (rpos_ - wpos_) + EXTRA_BUFFER_SIZE;
						wpos_ += cnt;
						size_ = size_ + cnt - (rpos_ - wpos_) + EXTRA_BUFFER_SIZE;
						buffer_ = new_buffer;
						return;
					}
				}
			}
		}

		EasyRingbuffer& operator << (easy_bool val)
		{
			append<easy_bool>(val);
			return *this;
		}

		EasyRingbuffer& operator << (easy_uint8 val)
		{
			append<easy_uint8>(val);
			return *this;
		}

		EasyRingbuffer& operator << (easy_uint16 val)
		{
			append<easy_uint16>(val);
			return *this;
		}

		EasyRingbuffer& operator << (easy_uint32 val)
		{
			append<easy_uint32>(val);
			return *this;
		}

		EasyRingbuffer& operator << (easy_uint64 val)
		{
			append<easy_uint64>(val);
			return *this;
		}

		EasyRingbuffer& operator << (easy_int8 val)
		{
			append<easy_int8>(val);
			return *this;
		}

		EasyRingbuffer& operator << (easy_int16 val)
		{
			append<easy_int16>(val);
			return *this;
		}

		EasyRingbuffer& operator << (easy_int32 val)
		{
			append<easy_int32>(val);
			return *this;
		}

		EasyRingbuffer& operator << (easy_int64 val)
		{
			append<easy_int64>(val);
			return *this;
		}

		EasyRingbuffer& operator << (easy_float val)
		{
			append<easy_float>(val);
			return *this;
		}

		EasyRingbuffer& operator << (easy_double val)
		{
			append<easy_double>(val);
			return *this;
		}

		EasyRingbuffer& operator << (const std::string& val)
		{
			append((easy_uint8 const*)val.c_str(),val.length());
			return *this;
		}

		EasyRingbuffer& operator << (const char* val)
		{
			append((easy_uint8 const *)val, val ? strlen(val) : 0);
			return *this;
		}

		template<typename T> T read() 
		{
			T r;
			read((easy_uint8*)&r,sizeof(T));
#ifdef _DEBUG
			std::cout << "read---buffer size = " << size_ << " rpos_ = " << rpos_ << " wpos_ = " << wpos_ << std::endl;
#endif // _DEBUG
			return r;
		}

		void read(easy_uint8* des,size_t len)
		{
			if (_read_finish())
			{
				return;
			}
			if (rpos_ < wpos_)
			{
				if (wpos_ - rpos_ >= len)
				{
					memmove(des,buffer_ + rpos_,len);
					rpos_ += len;
				}
				//	else just skip
			}
			else if (rpos_ > wpos_)
			{
				if (size_ - rpos_ >= len)
				{
					memmove(des,buffer_ + rpos_,len);
					rpos_ += len;
				}
				else
				{
					memmove(des,buffer_ + rpos_, size_ - rpos_);
					memmove(des + size_ - rpos_, buffer_, len - (size_ - rpos_));
					rpos_ = len - (size_ - rpos_);
				}
			}
		}

		EasyRingbuffer& operator >> (easy_bool& val)
		{
			val = read<easy_bool>();
			return *this;
		}

		EasyRingbuffer& operator >> (easy_uint8& val)
		{
			val = read<easy_uint8>();
			return *this;
		}

		EasyRingbuffer& operator >> (easy_uint16& val)
		{
			val = read<easy_uint16>();
			return *this;
		}

		EasyRingbuffer& operator >> (easy_uint32& val)
		{
			val = read<easy_uint32>();
			return *this;
		}

		EasyRingbuffer& operator >> (easy_uint64& val)
		{
			val = read<easy_uint64>();
			return *this;
		}

		EasyRingbuffer& operator >> (easy_int8& val)
		{
			val = read<easy_int8>();
			return *this;
		}

		EasyRingbuffer& operator >> (easy_int16& val)
		{
			val = read<easy_int16>();
			return *this;
		}

		EasyRingbuffer& operator >> (easy_int32& val)
		{
			val = read<easy_int32>();
			return *this;
		}

		EasyRingbuffer& operator >> (easy_int64& val)
		{
			val = read<easy_int64>();
			return *this;
		}

		EasyRingbuffer& operator >> (easy_float& val)
		{
			val = read<easy_float>();
			return *this;
		}

		EasyRingbuffer& operator >> (easy_double& val)
		{
			val = read<easy_double>();
			return *this;
		}

		size_t size() const { return size_; }

		size_t rpos() const { return rpos_; }

		size_t wpos() const { return wpos_; }

	private:
		_Type* _allocate(size_t size) 
		{ 
			_Type* res = 0;
			res = static_cast<_Type*>(alloc_type_.allocate(size)); 
			return res;
		}

		void _deallocate(void* p,size_t size) 
		{ 
			alloc_type_.deallocate(p,size); 
		}

		void _reallocate(void* p,size_t old_size,size_t new_size) { alloc_type_.reallocate(p,old_size,new_size); }

		easy_bool _read_finish() { return wpos_ == rpos_; }

	private:
		EasyRingbuffer ( const EasyRingbuffer& );
		EasyRingbuffer& operator = ( const EasyRingbuffer& );
	private:
		size_t			size_;

		_Type*			buffer_;

		size_t			wpos_;

		size_t			rpos_;

		allocator_type	alloc_type_;
	};
}

#endif // easy_ring_buffer_h__