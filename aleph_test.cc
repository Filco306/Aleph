#include "BoundaryMatrix.hh"
#include "Vector.hh"
#include "IO.hh"
#include "Standard.hh"
#include "Twist.hh"
#include "PersistencePairs.hh"

#include "Conversions.hh"
#include "Dualization.hh"
#include "PersistenceDiagram.hh"
#include "PersistenceDiagramConversion.hh"
#include "Simplex.hh"
#include "SimplicialComplex.hh"

#include <iostream>

using namespace aleph;
using namespace representations;

using I  = unsigned;
using V  = Vector<I>;
using BM = BoundaryMatrix<V>;
using SR = StandardReduction;
using TR = TwistReduction;

using S  = Simplex<float, unsigned>;
using SC = SimplicialComplex<S>;

int main()
{
  auto M = load<BM>( "Triangle.txt" );

  std::cout << "* Boundary matrix\n" << M << "\n"
            << "* Maximum dimension: " << M.getDimension() << "\n";

  computePersistencePairs<SR>( M );
  computePersistencePairs<TR>( M );

  computePersistencePairs<SR>( dualizeTrivial( M ), true );
  computePersistencePairs<TR>( dualizeTrivial( M ), true );

  std::cout << "* Boundary matrix [doubly-dualized]\n"
            << dualizeTrivial( dualizeTrivial( M ) ) << "\n";

  {
    S simplex( {0,1,2} );
    SC K( { {0}, {1}, {2}, {0,1}, {0,2}, {1,2}, {0,1,2} } );

    std::cout << K;

    auto M = makeBoundaryMatrix<BM>( K );

    auto&& pairing1 = computePersistencePairs<SR>( M );
    auto&& pairing2 = computePersistencePairs<TR>( M );

    auto&& diagrams1 = makePersistenceDiagrams( pairing1, K );
    auto&& diagrams2 = makePersistenceDiagrams( pairing2, K );

    for( auto&& D : diagrams1 )
      std::cout << D << "\n";

    for( auto&& D : diagrams2 )
      std::cout << D << "\n";
  }
}
