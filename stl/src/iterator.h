#ifndef _ITERATOR_H_
#define _ITERATOR_H_

namespace mystl
{
	struct input_iterator_tag {};
	struct out_iterator_tag {};
	struct forward_iterator_tag : public input_iterator_tag {};
	struct bidirectional_iterator_tag : public forward_iterator_tag {};
	struct random_access_iterator_tag : public bidirectional_iterator_tag {};



}

#endif