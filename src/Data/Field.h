/*
 * FieldScalar.h
 *
 *  Created on: 03.02.2016
 *      Author: dschmidt
 */

#ifndef FIELDSCALAR_H_
#define FIELDSCALAR_H_

#include <iostream>
#include <fstream>
#include <Eigen/Dense>
#include <random>

#include "Constants.h"
#include "Lattice.h"

namespace FermiOwn {

enum InitType {
	zeroInit,
	oneInit,
	randomInit,
	gaussianInit
};

/**
 * Every Field class is associated with a lattice that is passed to it at construction.
 */
template<class ScalarType> class Field {
public:
	//TODO: remove dependence on random number generator, since we don't need it for boolean fields
	Field( const size_t latticeVolume, const size_t DoF_per_Point, std::ranlux48 * rndGen, InitType init );
	virtual ~Field();

	void setGaussian();
	/**
	 * get the value of the field at point x, can be modified!
	 * @param x is the position to get the value from
	 * @return reference to field value at x
	 */
	ScalarType& operator()( const size_t x );

	/**
	 * get a copy of value of the field at point x, cannot be modified!
	 * @param x is the position to get the value from
	 * @return field value at x
	 */
	ScalarType operator()( const size_t x ) const;

	/**
	 * add a field to this field
	 * @param rhs is the field to add to the current one.
	 * @return the sum of the current vector and rhs
	 */
	Field<ScalarType>& operator+=( const Field<ScalarType>& rhs );

	/**
	 * @brief subtract a field from this field
	 * @param rhs is the field to subtract from the current one.
	 * @return the current vector minus rhs
	 */
	Field<ScalarType>& operator-=( const Field<ScalarType>& rhs );

	/**
	 * @brief Coefficient-wise multiply
	 * @param rhs is the field to multiply with
	 * @return the current vector with each element divided by the corresponding one in rhs
	 */
	Field<ScalarType>& operator*=( const Field<ScalarType>& rhs );

	/**
	 * @brief Coefficient-wise divide
	 * @param rhs is the field to divide by
	 * @return the current vector with each element divided by the corresponding one in rhs
	 */
	Field<ScalarType>& operator/=( const Field<ScalarType>& rhs );

	/**
	 * @brief Scalar addition
	 * @param rhs scalar to add to each vector component
	 * @return the current field shifted by rhs
	 */
	Field<ScalarType>& operator+=( const ScalarType& rhs );

	/**
	 * @brief Scalar subtraction
	 * @param rhs scalar to subtract from each vector component
	 * @return the current field shifted by -rhs
	 */
	Field<ScalarType>& operator-=( const ScalarType& rhs );

	/**
	 * @brief Scalar multiplication
	 * @param rhs scalar to multiply with
	 * @return the current field multiplied by rhs
	 */
	Field<ScalarType>& operator*=( const ScalarType& rhs );

	/**
	 * @brief Scalar division
	 * @param rhs scalar to divide by
	 * @return the current field divided by rhs
	 */
	Field<ScalarType>& operator/=( const ScalarType& rhs );

	/**
	 * @brief Calculate scalar product
	 * @param rhs other vector to perform scalar product with
	 * @return scalar product of this field with rhs
	 */
	ScalarType cwiseMultAndSum( const Field<ScalarType>& rhs ) const;

	/**
	 * @brief Returns the lattice associated with this field.
	 * @return reference to the lattice used at creation of this field.
	 */

	size_t getSize() const;

	void Print() const; // TODO: replace Print() method by overloaded << operator.

	void writeToFile( const std::string& filename ) const;

protected:
	size_t latVol;		//TODO: make this (and similar things in derived class) const and implement sensible copy constructor/assignment
	Eigen::Matrix< ScalarType, Eigen::Dynamic, Eigen::Dynamic > data;
	std::ranlux48 * randomGenerator;
	std::normal_distribution<Real> normalDistribution01;	///< Gaussian (normal) distribution with mean 0 and standard deviation 1
};

/* ---------------------------------------------------------------------------------------------------
 * Implementation of member functions
 * ---------------------------------------------------------------------------------------------------*/

template<class ScalarType> Field<ScalarType>::Field(const size_t latticeVolume, const size_t DoF_per_Point, std::ranlux48 * rndGen, InitType init) :
				latVol(latticeVolume),
				randomGenerator(rndGen)
				{
	data = Eigen::Matrix< ScalarType, Eigen::Dynamic, Eigen::Dynamic >(latticeVolume, DoF_per_Point);
	//TODO: move this switch to a general setter function.
	switch( init ) {
	case zeroInit:
		data.setZero();
		return;
	case oneInit:
		data.setOnes();
		return;
	case randomInit:
		data.setRandom();
		return;
	case gaussianInit:
		setGaussian();
		return;
	}

				}

template<class ScalarType> Field<ScalarType>::~Field() {}

template<class ScalarType> ScalarType& Field<ScalarType>::operator()( const size_t x ) {
	return data(x);
}

template<class ScalarType> ScalarType Field<ScalarType>::operator()( const size_t x ) const {
	return data(x);
}

template<class ScalarType> Field<ScalarType>& Field<ScalarType>::operator+=( const Field<ScalarType>& rhs ) {
	data += rhs.data;
	return *this;
}

template<class ScalarType> Field<ScalarType>& Field<ScalarType>::operator-=( const Field<ScalarType>& rhs ) {
	data -= rhs.data;
	return *this;
}

template<class ScalarType> Field<ScalarType>& Field<ScalarType>::operator*=( const Field<ScalarType>& rhs ) {
	data = data.cwiseProduct(rhs.data);
	return *this;
}

template<class ScalarType> Field<ScalarType>& Field<ScalarType>::operator/=( const Field<ScalarType>& rhs ) {
	data = data.cwiseQuotient(rhs.data);
	return *this;
}

template<class ScalarType> Field<ScalarType>& Field<ScalarType>::operator+=( const ScalarType& rhs ) {
	data -= Eigen::Matrix< ScalarType, Eigen::Dynamic, 1>::Constant(data.size(), rhs);
	return *this;
}

template<class ScalarType> Field<ScalarType>& Field<ScalarType>::operator-=( const ScalarType& rhs ) {
	data -= Eigen::Matrix< ScalarType, Eigen::Dynamic, 1>::Constant(data.size(), rhs);
	return *this;
}

template<class ScalarType> Field<ScalarType>& Field<ScalarType>::operator*=( const ScalarType& rhs ) {
	data *= rhs;
	return *this;
}

template<class ScalarType> Field<ScalarType>& Field<ScalarType>::operator/=( const ScalarType& rhs ) {
	data /= rhs;
	return *this;
}

template<class ScalarType> ScalarType Field<ScalarType>::cwiseMultAndSum( const Field<ScalarType>& rhs ) const {
	return data.cwiseProduct(rhs.data).sum();
}

template<class ScalarType> size_t Field<ScalarType>::getSize() const {
	return latVol;
}

template<class ScalarType> void Field<ScalarType>::Print() const {
	std::cout << data << std::endl;
}

template<class ScalarType> void Field<ScalarType>::writeToFile( const std::string& filename ) const {
	std::ofstream file(filename);
	for( int i = 0; i < data.size(); i++ ) {
		file << std::real(data(i)) << "\t" << std::imag(data(i)) << std::endl;
	}
	file.close();
}

/* ---------------------------------------------------------------------------------------------------
 * Implementation of non-member functions
 * ---------------------------------------------------------------------------------------------------*/

template<class ScalarType> inline Field<ScalarType> operator+( Field<ScalarType> lhs, const Field<ScalarType>& rhs ) {
	lhs += rhs;
	return lhs;
}

template<class ScalarType> inline Field<ScalarType> operator-( Field<ScalarType> lhs, const Field<ScalarType>& rhs ) {
	lhs -= rhs;
	return lhs;
}

template<class ScalarType> inline Field<ScalarType> operator*( Field<ScalarType> lhs, const Field<ScalarType>& rhs ) {
	lhs *= rhs;
	return lhs;
}

template<class ScalarType> inline Field<ScalarType> operator/( Field<ScalarType> lhs, const Field<ScalarType>& rhs ) {
	lhs /= rhs;
	return lhs;
}

template<class ScalarType> inline Field<ScalarType> operator+( Field<ScalarType> lhs, const ScalarType& rhs ) {
	lhs += rhs;
	return lhs;
}

template<class ScalarType> inline Field<ScalarType> operator-( Field<ScalarType> lhs, const ScalarType& rhs ) {
	lhs -= rhs;
	return lhs;
}

template<class ScalarType> inline Field<ScalarType> operator*( Field<ScalarType> lhs, const ScalarType& rhs ) {
	lhs *= rhs;
	return lhs;
}

template<class ScalarType> inline Field<ScalarType> operator/( Field<ScalarType> lhs, const ScalarType& rhs ) {
	lhs /= rhs;
	return lhs;
}

template<class ScalarType> inline Field<ScalarType> operator+( const ScalarType& lhs, Field<ScalarType> rhs ) {
	rhs += lhs;
	return rhs;
}

template<class ScalarType> inline Field<ScalarType> operator-( const ScalarType& lhs, Field<ScalarType> rhs ) {
	rhs -= lhs;
	return rhs;
}

template<class ScalarType> inline Field<ScalarType> operator*( const ScalarType& lhs, Field<ScalarType> rhs ) {
	rhs *= lhs;
	return rhs;
}

template<class ScalarType> inline Field<ScalarType> operator/( const ScalarType& lhs, Field<ScalarType> rhs ) {
	rhs /= lhs;
	return rhs;
}

} // namespace FermiOwn

#endif /* FIELDSCALAR_H_ */
