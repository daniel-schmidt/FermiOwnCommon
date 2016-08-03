/*
 * FieldScalar.cpp
 *
 *  Created on: 02.03.2016
 *      Author: dschmidt
 */

#include "Field.h"

namespace FermiOwn {

// template specialization for real numbers
template<> void Field<Real>::setGaussian() {
	auto gaussian = [&] (Real) {return normalDistribution01(*randomGenerator); };
	data = Eigen::Matrix< Real, Eigen::Dynamic, 1>::NullaryExpr( data.size(), gaussian );
}
// template specialization for complex numbers: draw two reals
// TODO: simpler implementation without seperate assignment of real and imag may be possible
template<> void Field<Complex>::setGaussian() {
	auto gaussian = [&] (Real) {return normalDistribution01(*randomGenerator); };
	data.real() = Eigen::Matrix< Real, Eigen::Dynamic, 1>::NullaryExpr( data.size(), gaussian );
	data.imag() = Eigen::Matrix< Real, Eigen::Dynamic, 1>::NullaryExpr( data.size(), gaussian );
}

template<> void Field<bool>::setGaussian() {
	data.setRandom(); //TODO: remove setGaussian from general field since its not very sensible for bool field
}

} // namespace FermiOwn
