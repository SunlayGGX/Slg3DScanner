#pragma once

namespace std
{
#ifdef NEED_FORWARD_STRING_DECLARATION
    template<class _Elem, class _Traits, class _Alloc> class basic_string;
    template<class T> struct char_traits;
    template<class _Ty> class allocator;
    typedef basic_string<char, char_traits<char>, allocator<char>> string;
#endif

#ifdef NEED_FORWARD_OSTREAM_DECLARATION
    template<class _Elem, class _Traits> class basic_ostream;
    template<class T> struct char_traits;
    typedef basic_ostream<char, char_traits<char>> ostream;
#endif


#ifdef NEED_FORWARD_MUTEX_DECLARATION
	class mutex;
#endif
}