#ifndef ALEPH_PERSISTENCE_DIAGRAM_HH__
#define ALEPH_PERSISTENCE_DIAGRAM_HH__

#include <algorithm>
#include <iosfwd>
#include <limits>
#include <utility>
#include <set>
#include <vector>

namespace aleph
{

template <class T> class PersistenceDiagram
{
public:

  // Exporting the data type of the underlying persistence diagrams
  // makes it easier for client code to specify the desired type in
  // advanced. Else, I would require `decltype` or related concepts
  // that make the code harder to read.
  using DataType = T;

  class Point
  {
  public:

    explicit Point( DataType x )
      : _x( x )
      , _y( std::numeric_limits<DataType>::max() )
    {
      if( std::numeric_limits<DataType>::has_infinity )
        _y = std::numeric_limits<DataType>::infinity();
    }

    Point( DataType x, DataType y )
      : _x( x )
      , _y( y )
    {
    }

    DataType x() const noexcept { return _x; }
    DataType y() const noexcept { return _y; }

    DataType persistence() const noexcept
    {
      return _y - _x;
    }

    bool operator==( const Point& other ) const noexcept
    {
      return _x == other._x && _y == other._y;
    }

    bool operator!=( const Point& other ) const noexcept
    {
      return !this->operator==( other );
    }

    bool operator<( const Point& other ) const noexcept
    {
      if( _x == other._x )
        return _y < other._y;
      else
        return _x < other._x;
    }

    bool isUnpaired() const noexcept
    {
      return    (  std::numeric_limits<DataType>::has_infinity && _y == std::numeric_limits<DataType>::infinity() )
             || ( !std::numeric_limits<DataType>::has_infinity && _y == std::numeric_limits<DataType>::max() );
    }

  private:
    DataType _x;
    DataType _y;
  };

  // Typedefs & aliases ------------------------------------------------

  using ValueType     = Point;
  using ContainerType = std::vector<ValueType>;

  using ConstIterator = typename ContainerType::const_iterator;
  using Iterator      = typename ContainerType::iterator;

  using value_type    = ValueType;

  // Iterators ---------------------------------------------------------

  ConstIterator begin() const { return _points.begin(); }
  Iterator      begin()       { return _points.begin(); }

  ConstIterator end() const   { return _points.end(); }
  Iterator      end()         { return _points.end(); }

  // Modification ------------------------------------------------------

  void add( DataType x )
  {
    _points.push_back( Point( x ) );
  }

  void add( DataType x, DataType y )
  {
    _points.push_back( Point( x, y ) );
  }

  /**
    Inserts a sequence of points at the specified position of the
    persistence diagram.

    @param position Position at which to insert the points
    @param first    Iterator to beginning of range
    @param end      Iterator to end of range
  */

  template <class InputIterator> void insert( Iterator position,
                                              InputIterator first,
                                              InputIterator last )
  {
    _points.insert( position, first, last );
  }

  Iterator erase( Iterator position )
  {
    return _points.erase( position );
  }

  Iterator erase( Iterator begin, Iterator end )
  {
    return _points.erase( begin, end );
  }

  /** Removes all points that appear on the diagonal of a persistence diagram */
  void removeDiagonal() noexcept
  {
    _points.erase(
      std::remove_if( _points.begin(), _points.end(),
                      [] ( const Point& p )
                      {
                        return p.x() == p.y();
                      } ),
      _points.end()
    );
  }

  /** Removes all unpaired points, i.e. points with infinite persistence */
  void removeUnpaired() noexcept
  {
    _points.erase(
      std::remove_if( _points.begin(), _points.end(),
                      [] ( const Point& p )
                      {
                        return p.isUnpaired();
                      } ),
      _points.end()
    );
  }

  /**
    Removes all duplicate points, i.e. enforces that the multiplicity of
    each point is exactly one. This function does not preserve the local
    order of points in the persistence diagram (which should not matter,
    in any case).
  */

  void removeDuplicates() noexcept
  {
    std::set<Point> points( _points.begin(), _points.end() );

    _points.assign( points.begin(),
                    points.end() );
  }

  /**
    Merges two persistence diagrams. The points stored in the other
    persistence diagram will simply be added to the current diagram
    without accounting for duplicates.

    @param other Persistence diagram to merge into the current one
  */

  void merge( const PersistenceDiagram<DataType>& other )
  {
    _points.insert( _points.end(),
                    other._points.begin(), other._points.end() );
  }

  // Attributes --------------------------------------------------------

  void setDimension( std::size_t dimension )
  {
    _dimension = dimension;
  }

  std::size_t dimension() const
  {
    return _dimension;
  }

  // Comparison operators ----------------------------------------------

  bool operator==( const PersistenceDiagram<DataType>& other ) const
  {
    return _points == other._points;
  }

  bool operator!=( const PersistenceDiagram<DataType>& other ) const
  {
    return !( this->operator==( other ) );
  }

  // Queries -----------------------------------------------------------

  /** @returns Betti number of the persistence diagram, i.e. the number of unpaired points */
  std::size_t betti() const
  {
    auto numUnpairedPoints
      = std::count_if( _points.begin(), _points.end(),
                       [] ( const Point& p )
                       {
                         return p.isUnpaired();
                       } );

    return static_cast<std::size_t>( numUnpairedPoints );
  }

  std::size_t size() const
  {
    return _points.size();
  }

  bool empty() const
  {
    return _points.empty();
  }

private:

  /** Dimension of the persistence pairs stored in the diagram */
  std::size_t _dimension = 0;

  /** Container of persistence pairs */
  ContainerType _points;
};

template <class DataType> std::ostream& operator<<( std::ostream& o, const PersistenceDiagram<DataType>& D )
{
  for( auto&& p : D )
    o << p.x() << "\t" << p.y() << "\n";

  return o;
}

}

#endif
