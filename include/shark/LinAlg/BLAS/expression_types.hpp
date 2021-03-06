#ifndef SHARK_LINALG_BLAS_UBLAS_EXPRESSION_TYPE_HPP
#define SHARK_LINALG_BLAS_UBLAS_EXPRESSION_TYPE_HPP

namespace shark {
namespace blas {

/** \brief Base class for Vector Expression models
 *
 * it does not model the Vector Expression concept but all derived types should.
 * The class defines a common base type and some common interface for all
 * statically derived Vector Expression classes.
 * We implement the casts to the statically derived type.
 */
template<class E>
struct vector_expression {
	typedef E expression_type;

	const expression_type &operator()() const {
		return *static_cast<const expression_type *>(this);
	}

	expression_type &operator()() {
		return *static_cast<expression_type *>(this);
	}
};

/** \brief Base class for Vector container models
 *
 * it does not model the Vector concept but all derived types should.
 * The class defines a common base type and some common interface for all
 * statically derived Vector classes
 * We implement the casts to the statically derived type.
 */
template<class C>
struct vector_container:public vector_expression<C> {
	typedef C container_type;

	const container_type &operator()() const {
		return *static_cast<const container_type *>(this);
	}

	container_type &operator()() {
		return *static_cast<container_type *>(this);
	}
};


/** \brief Base class for Matrix Expression models
 *
 * it does not model the Matrix Expression concept but all derived types should.
 * The class defines a common base type and some common interface for all
 * statically derived Matrix Expression classes
 * We iboost::mplement the casts to the statically derived type.
 */
template<class E>
struct matrix_expression {
	typedef E expression_type;

	const expression_type &operator()() const {
		return *static_cast<const expression_type *>(this);
	}

	expression_type &operator()() {
		return *static_cast<expression_type *>(this);
	}
};

/** \brief Base class for Matrix container models
 *
 * it does not model the Matrix concept but all derived types should.
 * The class defines a common base type and some common interface for all
 * statically derived Matrix classes
 * We implement the casts to the statically derived type.
 */
template<class C>
struct matrix_container: public matrix_expression<C> {
	typedef C container_type;

	const container_type &operator()() const {
		return *static_cast<const container_type *>(this);
	}

	container_type &operator()() {
		return *static_cast<container_type *>(this);
	}
};


// Assignment proxy.
// Provides temporary free assigment when LHS has no alias on RHS
template<class C>
class noalias_proxy{
public:
	typedef typename C::closure_type closure_type;


	noalias_proxy(C &lval): m_lval(lval) {}

	noalias_proxy(const noalias_proxy &p):m_lval(p.m_lval) {}

	template <class E>
	closure_type &operator= (const E &e) {
		m_lval.assign(e);
		return m_lval;
	}

	template <class E>
	closure_type &operator+= (const E &e) {
		m_lval.plus_assign(e);
		return m_lval;
	}

	template <class E>
	closure_type &operator-= (const E &e) {
		m_lval.minus_assign(e);
		return m_lval;
	}
	
	template <class E>
	closure_type &operator*= (const E &e) {
		m_lval.multiply_assign(e);
		return m_lval;
	}

	template <class E>
	closure_type &operator/= (const E &e) {
		m_lval.divide_assign(e);
		return m_lval;
	}

private:
	closure_type m_lval;
};

// Improve syntax of efficient assignment where no aliases of LHS appear on the RHS
//  noalias(lhs) = rhs_expression
template <class C>
noalias_proxy<C> noalias(matrix_expression<C>& lvalue) {
	return noalias_proxy<C> (lvalue());
}
template <class C>
noalias_proxy<const C> noalias(matrix_expression<C> const& lvalue) {
	return noalias_proxy<const C> (lvalue());
}
template <class C>
noalias_proxy<C> noalias(vector_expression<C>& lvalue) {
	return noalias_proxy<C> (lvalue());
}
template <class C>
noalias_proxy<const C> noalias(vector_expression<C> const& lvalue) {
	return noalias_proxy<const C> (lvalue());
}

template<class P>
struct temporary_proxy:public P{
	temporary_proxy(P const& p):P(p){}
	
	template<class E>
	P& operator=(E const& e){
		return static_cast<P&>(*this) = e;
	}
	
	P& operator=(temporary_proxy<P> const& e){
		return static_cast<P&>(*this) = e;
	}
};


}
}

#endif
