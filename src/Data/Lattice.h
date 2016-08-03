/*
 * Lattice.h
 *
 *  Created on: 02.02.2016
 *      Author: dschmidt
 */

#ifndef LATTICE_H_
#define LATTICE_H_


#include <cmath>
#include <cstddef>
#include <vector>

namespace FermiOwn {

/**
 * @brief Representation of the lattice, holding informations about the geometry
 *
 * Information stored in this object are purely about the lattice layout, like time- and space-sizes.
 * Additional information about neighbouring points etc. are included, mapping the linear lattice index to the actual geometry.
 */
class Lattice {
public:
	/**
	 * @brief Constructs a new lattice with given dimensions.
	 * @param lenTime is the number of lattice points in time-direction
	 * @param lenSpace is the number of lattice points in space-direction
	 * @param numDim is the number of dimensions, must be at least 2 for the moment...
	 */
  Lattice(const size_t lenTime, const size_t lenSpace, const size_t numDim);
  ~Lattice();

  enum Dir {fwd, bwd};

  inline const size_t getDim() const;
  inline const size_t getTimeSize() const;
  inline const size_t getSpaceSize() const;
  inline const size_t getVol() const;

  inline const std::vector< size_t > getNeighbours( const size_t x ) const;
  inline const std::vector< size_t > getNeighbours( const size_t x, Dir dir) const;
  inline const std::vector< std::vector<size_t> > & getLineIndex() const;


private:
  /**
   * @brief Creates a list of nearest neighbours.
   * The neighbours are sorted by (time-bwd, time-fwd, space1-bwd, space1-fwd,...)
   */
  void makeNeighbourIndex();
  // creates a list of every point belonging to a line in time direction for every spatial point in the lattice
  void makeLineIndex();

  const size_t Nt;    ///< lattice size in time direction
  const size_t Ns;    ///< lattice size in space direction
  const size_t dim;   ///< number of dimensions
  const size_t V;     ///< total amount of lattice points
  std::vector< std::vector<size_t> > nnIndex;  ///< contains a list of nearest neighbours for every lattice point sorted by (time-bwd, time-fwd, space1-bwd, space1-fwd,...)
  std::vector< std::vector<size_t> > lineIndex;
};

/* =============================================================================
 * implementation of inline functions
 * ============================================================================= */

const size_t Lattice::getDim() const {
  return dim;
}

const size_t Lattice::getSpaceSize() const {
	return Ns;
}

const size_t Lattice::getTimeSize() const {
	return Nt;
}

const size_t Lattice::getVol() const {
  return V;
}

const std::vector< size_t > Lattice::getNeighbours( const size_t x ) const {
	return nnIndex[x];
}

const std::vector< size_t > Lattice::getNeighbours( const size_t x, Dir dir ) const {
	// return only the first (bwd) or second point (bwd) for each direction, according to index layout
	std::vector< size_t > result;
	if( dir == fwd ) {
		for( size_t i = 1; i < nnIndex[x].size(); i+=2 ) {
			result.push_back(nnIndex[x][i]);
		}
	} else {
		for( size_t i = 0; i < nnIndex[x].size(); i+=2 ) {
			result.push_back(nnIndex[x][i]);
		}
	}

	return result;
}

const std::vector< std::vector<size_t> > & Lattice::getLineIndex() const {
  return lineIndex;
}

} // namespace FermiOwn

#endif /* LATTICE_H_ */
