/*
 * IsingHamiltonian.h
 *
 *  Created on: 29.08.2016
 *      Author: dschmidt
 */

#ifndef SRC_ISINGHAMILTONIAN_H_
#define SRC_ISINGHAMILTONIAN_H_

#include "Lattice.h"
#include "Field.h"

namespace FermiOwn {

class IsingHamiltonian {
public:
	IsingHamiltonian( const Lattice& lattice, Field<int>& spinField, const double coupling );
	virtual ~IsingHamiltonian();

	/**
	 * @brief Returns the value of the Hamiltonian
	 */
	double calculateEnergy() const;

	/**
	 * @brief Returns the change in the Hamiltonian H_old - H_new, if a spin at a given position is flipped
	 *
	 * The return value is positive, if the energy is decreasing!
	 *
	 * @param x is the position where a spin is flipped
	 * @return the change in energy resulting from the single-spin flip
	 */
	double changeAt( size_t x ) const;

private:
	const Lattice& lat;
	Field<int> & spin;
	const double J;
};

} /* namespace FermiOwn */

#endif /* SRC_ISINGHAMILTONIAN_H_ */
