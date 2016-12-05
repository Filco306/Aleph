#ifndef ALEPH_BOUNDARY_MATRIX_HH__
#define ALEPH_BOUNDARY_MATRIX_HH__

#include <ostream>
#include <vector>

namespace aleph
{

template <class Representation> class BoundaryMatrix
{
public:
  using Index = typename Representation::Index;

  void setNumColumns( Index numColumns )
  {
    _representation.setNumColumns( numColumns );
  }

  Index getNumColumns() const
  {
    return _representation.getNumColumns();
  }

  std::pair<Index, bool> getMaximumIndex( Index column ) const
  {
    return _representation.getMaximumIndex( column );
  };

  void addColumns( Index source, Index target )
  {
    _representation.addColumns( source, target );
  };

  template <class InputIterator> void setColumn( Index column,
                                                 InputIterator begin, InputIterator end )

  {
    _representation.setColumn( column, begin, end );
  }

  std::vector<Index> getColumn( Index column ) const
  {
    return _representation.getColumn( column );
  }

  void clearColumn( Index column )
  {
    _representation.clearColumn( column );
  }

  Index getDimension( Index column ) const
  {
    return _representation.getDimension( column );
  }

  Index getDimension() const
  {
    return _representation.getDimension();
  }

  void setDualized( bool value = true )
  {
    _isDualized = value;
  }

  bool isDualized() const
  {
    return _isDualized;
  }

private:
  Representation _representation;

  /**
    Flag indicating whether the matrix is dualized or not. By default
    no matrix is dualized. This flag is used by some of the reduction
    algorithms to determine how to calculate indices.
  */

  bool _isDualized = false;

};

}

// ---------------------------------------------------------------------

template <class Representation> std::ostream& operator<< ( std::ostream& o, const aleph::BoundaryMatrix<Representation>& M )
{
  using Index = typename Representation::Index;

  auto numColumns = M.getNumColumns();

  for( Index j = Index(0); j < numColumns; ++j )
  {
    auto column = M.getColumn( j );

    if( !column.empty() )
    {
      for( auto&& c : column )
        o << c << " ";
    }
    else
      o << "-";

    o << "\n";
  }

  return o;
}

// ---------------------------------------------------------------------

#endif