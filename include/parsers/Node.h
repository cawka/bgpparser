
#ifndef _NODE_H_
#define _NODE_H_

class Node
{
	virtual void accept( Visitor &v )							=0;
	virtual void accept( GJVoidVisitor &v, boost::any param )	=0;
	virtual boost::any accept( GJNoArguVisitor &v )				=0;
	virtual boost::any accept( GJVisitor &v, boost::any param )	=0;
};

#endif
