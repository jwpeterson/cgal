// Copyright (c) 1999,2004   INRIA Sophia-Antipolis (France).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org); you may redistribute it under
// the terms of the Q Public License version 1.0.
// See the file LICENSE.QPL distributed with CGAL.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL$
// $Id$
// 
//
// Author(s)     : Sylvain Pion <Sylvain.Pion@sophia.inria.fr>
//                 Monique Teillaud <Monique.Teillaud@sophia.inria.fr>
//                 Mariette Yvinec <Mariette.Yvinec@sophia.inria.fr>

#ifndef CGAL_REGULAR_TRIANGULATION_EUCLIDEAN_TRAITS_3_H
#define CGAL_REGULAR_TRIANGULATION_EUCLIDEAN_TRAITS_3_H

#include <CGAL/basic.h>
#include <CGAL/Triangulation_short_names_3.h>
#include <CGAL/triangulation_assertions.h>
#include <CGAL/Weighted_point.h>
#include <CGAL/representation_tags.h>

#include <CGAL/predicates/Regular_triangulation_ftC3.h>
#include <CGAL/predicates/Regular_triangulation_rtH3.h>
#include <CGAL/predicates/predicates_on_weighted_points_cartesian_3.h>
#include <CGAL/constructions/constructions_on_weighted_points_cartesian_3.h>

CGAL_BEGIN_NAMESPACE 

// returns minus the sign of the determinant of the lifted points
// associated with p,q,r,s,t  [P,Q,R,S,T]
// where the P colum is [ p, p^2-wp,1]
template < typename K >
class Power_test_3
{
public:
  typedef typename K::Weighted_point_3                  Weighted_point_3;

  typedef Arity_tag< 5 >   Arity;
  typedef Oriented_side    result_type;

  Oriented_side operator() ( const Weighted_point_3 & p,
			     const Weighted_point_3 & q,
			     const Weighted_point_3 & r,
			     const Weighted_point_3 & s,
			     const Weighted_point_3 & t) const
    {
      return power_test_3(p,q,r,s,t);
    }

  Oriented_side operator() ( const Weighted_point_3 & p,
			     const Weighted_point_3 & q,
			     const Weighted_point_3 & r,
			     const Weighted_point_3 & s) const
    {
      return power_test_3(p,q,r,s);
    }

  Oriented_side operator() ( const Weighted_point_3 & p,
			     const Weighted_point_3 & q,
			     const Weighted_point_3 & r) const
    {
      return power_test_3(p,q,r);
    }

  Oriented_side operator() ( const Weighted_point_3 & p,
			     const Weighted_point_3 & q) const
    {
      return power_test_3(p,q);
    }
};


template < typename K >
class Compare_power_distance_3
{
public:
  typedef typename K::Weighted_point_3                  Weighted_point_3;
  typedef typename K::Bare_point                        Point_3;

  typedef Arity_tag< 3 >     Arity;
  typedef Comparison_result  result_type;

  Comparison_result operator() ( const Point_3 & p,
				 const Weighted_point_3 & q,
				 const Weighted_point_3 & r) const
    {
      return compare_power_distance_3(p,q,r);
    }
};


// operator ()
// return the sign of the power test of  last weighted point
// with respect to the smallest sphere orthogonal to the others
template< typename K >
class In_smallest_orthogonal_sphere_3
{
public:
  typedef typename K::Weighted_point_3               Weighted_point_3;

  typedef Arity_tag< 5 >   Arity;
  typedef Sign             result_type;

  Sign operator() ( const Weighted_point_3 & p,
		    const Weighted_point_3 & q,
		    const Weighted_point_3 & r,
		    const Weighted_point_3 & s,
		    const Weighted_point_3 & t) const
  {
    K traits;
    typename K::Orientation_3  orientation = traits.orientation_3_object();
    Orientation o = orientation(p,q,r,s);
    Oriented_side os = power_test_3(p,q,r,s,t);
    CGAL_triangulation_assertion( o != COPLANAR);
    // the minus sign below is due to the fact that power_test_3
    // return in fact minus the 5x5 determinant of lifted (p,q,r,s.t)
    return - o * os;
  }
  
  Sign operator() ( const Weighted_point_3 & p,
		    const Weighted_point_3 & q,
		    const Weighted_point_3 & r,
		    const Weighted_point_3 & s) const
  {
    return in_smallest_orthogonal_sphereC3(
                              p.x(), p.y(), p.z(), p.weight(),
			      q.x(), q.y(), q.z(), q.weight(),
			      r.x(), r.y(), r.z(), r.weight(),
			      s.x(), s.y(), s.z(), s.weight());
  }

  Sign operator() ( const Weighted_point_3 & p,
		    const Weighted_point_3 & q,
		    const Weighted_point_3 & r) const
  {
    return in_smallest_orthogonal_sphereC3(
                              p.x(), p.y(), p.z(), p.weight(),
			      q.x(), q.y(), q.z(), q.weight(),
			      r.x(), r.y(), r.z(), r.weight());
  }

  Sign operator() ( const Weighted_point_3 & p,
		    const Weighted_point_3 & q) const
  {
    return CGAL_NTS sign( CGAL_NTS square(p.x()-q.x()) +
			  CGAL_NTS square(p.y()-q.y()) +
			  CGAL_NTS square(p.z()-q.z()) +
			  p.weight() - q.weight());
  }

};

template < typename K >
class Side_of_bounded_orthogonal_sphere_3
{
public :
  typedef typename K::Weighted_point_3                 Weighted_point_3;
  typedef typename K::In_smallest_orthogonal_sphere_3  In_sphere;

  typedef Arity_tag< 5 >   Arity;
  typedef Bounded_side     result_type;

  Bounded_side operator() ( const Weighted_point_3 & p,
			    const Weighted_point_3 & q,
			    const Weighted_point_3 & r,
			    const Weighted_point_3 & s,
			    const Weighted_point_3 & t) const
  {
    return Bounded_side( (-1) * In_sphere()(p,q,r,s,t));
  }
  
  Bounded_side operator() ( const Weighted_point_3 & p,
			    const Weighted_point_3 & q,
			    const Weighted_point_3 & r,
			    const Weighted_point_3 & s) const
  {
    return Bounded_side ( (-1) * In_sphere()(p,q,r,s) );
  }

  Bounded_side operator() ( const Weighted_point_3 & p,
			    const Weighted_point_3 & q,
			    const Weighted_point_3 & r) const
  {
    return Bounded_side ( (-1) * In_sphere()(p,q,r) );
  }

};


// operator() returns true if the affine hull of the dual
// to the given weighted points 
// intersect  the simplex formed by the bare points 
template < typename K >
class Does_simplex_intersect_weighted_dual_support_3
{
public:
  typedef typename K::Weighted_point_3               Weighted_point_3;

  typedef Arity_tag< 4 >   Arity;
  typedef Bounded_side     result_type;

  Bounded_side operator()(const Weighted_point_3 & p,
		          const Weighted_point_3 & q,
		          const Weighted_point_3 & r,
		          const Weighted_point_3 & s) const
  {
    return does_simplex_intersect_weighted_dual_supportC3(
                                        p.x(), p.y(), p.z(), p.weight(),
					q.x(), q.y(), q.z(), q.weight(),
					r.x(), r.y(), r.z(), r.weight(),
					s.x(), s.y(), s.z(), s.weight());
  }

  Bounded_side operator()(const Weighted_point_3 & p,
		          const Weighted_point_3 & q,
		          const Weighted_point_3 & r) const
  {
    return does_simplex_intersect_weighted_dual_supportC3(
                                        p.x(), p.y(), p.z(), p.weight(),
					q.x(), q.y(), q.z(), q.weight(),
					r.x(), r.y(), r.z(), r.weight()); 
  }

  Bounded_side operator()(const Weighted_point_3 & p,
		          const Weighted_point_3 & q) const
  {
    return does_simplex_intersect_weighted_dual_supportC3(
                                        p.x(), p.y(), p.z(), p.weight(),
					q.x(), q.y(), q.z(), q.weight());
  }
};

template < typename K >
class Construct_weighted_circumcenter_3
{
public:
  typedef typename K::Weighted_point_3               Weighted_point_3;
  typedef typename K::Bare_point                     Bare_point;
  typedef typename K::FT                             FT;

  typedef Arity_tag< 4 >   Arity;
  typedef Bare_point       result_type;

  Bare_point operator() ( const Weighted_point_3 & p,
			  const Weighted_point_3 & q,
			  const Weighted_point_3 & r,
			  const Weighted_point_3 & s) const
    {
      FT x, y, z;
      weighted_circumcenterC3(p.x(), p.y(), p.z(), p.weight(),
			      q.x(), q.y(), q.z(), q.weight(),
			      r.x(), r.y(), r.z(), r.weight(),
			      s.x(), s.y(), s.z(), s.weight(),
			      x,y,z);
      return Bare_point(x,y,z);
    }

  Bare_point operator() ( const Weighted_point_3 & p,
			  const Weighted_point_3 & q,
			  const Weighted_point_3 & r) const
    {
      FT x, y, z;
      weighted_circumcenterC3(p.x(), p.y(), p.z(), p.weight(),
			      q.x(), q.y(), q.z(), q.weight(),
			      r.x(), r.y(), r.z(), r.weight(),
			      x,y,z);
      return Bare_point(x,y,z);
    }

  Bare_point operator() ( const Weighted_point_3 & p,
			  const Weighted_point_3 & q) const
    {
      FT x, y, z;
      weighted_circumcenterC3(p.x(), p.y(), p.z(), p.weight(),
			      q.x(), q.y(), q.z(), q.weight(),
			      x,y,z);
      return Bare_point(x,y,z);
    }
};


template < typename K >
class Compute_power_product_3
{
public:
  typedef typename K::Weighted_point_3               Weighted_point_3;
  typedef typename K::FT                             FT;

  typedef Arity_tag< 2 >   Arity;
  typedef FT               result_type;

  FT operator() (const Weighted_point_3 & p,
		 const Weighted_point_3 & q) const
  {
    return power_productC3(p.x(), p.y(), p.z(), p.weight(),
			   q.x(), q.y(), q.z(), q.weight());
  }
};


template < typename K >
class Compute_squared_radius_smallest_orthogonal_sphere_3
{
public:
  typedef typename K::Weighted_point_3               Weighted_point_3;
  typedef typename K::FT                             FT;

  typedef Arity_tag< 4 >   Arity;
  typedef FT               result_type;

  FT operator() ( const Weighted_point_3 & p,
		  const Weighted_point_3 & q,
		  const Weighted_point_3 & r,
		  const Weighted_point_3 & s) const
    {
        return squared_radius_orthogonal_sphereC3(
                              p.x(), p.y(), p.z(), p.weight(),
			      q.x(), q.y(), q.z(), q.weight(),
			      r.x(), r.y(), r.z(), r.weight(),
			      s.x(), s.y(), s.z(), s.weight());
    }

  FT operator() ( const Weighted_point_3 & p,
		  const Weighted_point_3 & q,
		  const Weighted_point_3 & r) const
    {
      return squared_radius_smallest_orthogonal_sphereC3(
                              p.x(), p.y(), p.z(), p.weight(),
			      q.x(), q.y(), q.z(), q.weight(),
			      r.x(), r.y(), r.z(), r.weight());
    }

  FT operator() (const Weighted_point_3 & p,
		 const Weighted_point_3 & q) const
  {
    return squared_radius_smallest_orthogonal_sphereC3(
			   p.x(), p.y(), p.z(), p.weight(),
			   q.x(), q.y(), q.z(), q.weight());
  }
};


// Compute the square radius of the circle centered in t
// and orthogonal to  the circle orthogonal a p,q,r,s
template< typename K>
class Compute_critical_squared_radius_3
{
 public:
  typedef typename K::Weighted_point_3                  Weighted_point_3; 
  typedef typename K::FT                                FT;
 
  typedef Arity_tag< 5 >   Arity;
  typedef FT               result_type;

  result_type operator() (const Weighted_point_3 & p,
			  const Weighted_point_3 & q,
			  const Weighted_point_3 & r,
			  const Weighted_point_3 & s,
			  const Weighted_point_3 & t) const
  {
    return critical_squared_radiusC3 (p.x(),p.y(),p.z(),FT(p.weight()),
				      q.x(),q.y(),q.z(),FT(q.weight()),
				      r.x(),r.y(),r.z(),FT(r.weight()),
				      s.x(),s.y(),s.z(),FT(s.weight()),
				      t.x(),t.y(),t.z(),FT(t.weight()));
  }
};








template < class K, class Weight = typename K::RT >
class Regular_triangulation_euclidean_traits_base_3
  : public K
{
public:
  typedef K                                          Kernel;
  typedef typename K::FT                             FT;
  typedef typename K::Point_3                        Bare_point;
  typedef CGAL::Weighted_point<Bare_point, Weight>   Weighted_point;
  typedef Weighted_point                             Weighted_point_3;
  typedef Weighted_point                             Point_3;
  //typedef typename K::Point_3                        Point_3;

  typedef Regular_triangulation_euclidean_traits_base_3<K, Weight> Self;

  // The next typedef is there for backward compatibility
  // Some users take their point type from the traits class.
  // Before this type was Point
  typedef Point_3 Point;

  typedef CGAL::Power_test_3<Self>                 Power_test_3;
  typedef CGAL::Compare_power_distance_3<Self>     Compare_power_distance_3;
  typedef CGAL::In_smallest_orthogonal_sphere_3<Self>
                                In_smallest_orthogonal_sphere_3;
  typedef CGAL::Side_of_bounded_orthogonal_sphere_3<Self>
                                Side_of_bounded_orthogonal_sphere_3;
  typedef CGAL::Does_simplex_intersect_weighted_dual_support_3<Self>
                                Does_simplex_intersect_dual_support_3; 
  typedef CGAL::Construct_weighted_circumcenter_3<Self>
                                 Construct_weighted_circumcenter_3;
  typedef CGAL::Compute_squared_radius_smallest_orthogonal_sphere_3<Self>
                Compute_squared_radius_smallest_orthogonal_sphere_3;
  typedef CGAL::Compute_power_product_3<Self>    Compute_power_product_3;
  typedef CGAL::Compute_critical_squared_radius_3<Self>
                                       Compute_critical_squared_radius_3;
  
  Power_test_3   power_test_3_object() const
  { return Power_test_3(); }

  Compare_power_distance_3 compare_power_distance_3_object() const
  { return Compare_power_distance_3(); }

  In_smallest_orthogonal_sphere_3 
  in_smallest_orthogonal_sphere_3_object() const
  { return In_smallest_orthogonal_sphere_3(); }

  Side_of_bounded_orthogonal_sphere_3
  side_of_bounded_orthogonal_sphere_3_object() const
  { return Side_of_bounded_orthogonal_sphere_3(); }

  Does_simplex_intersect_dual_support_3 
  does_simplex_intersect_dual_support_3_object() const
  { return Does_simplex_intersect_dual_support_3(); }

  Construct_weighted_circumcenter_3 
  construct_weighted_circumcenter_3_object() const
  { return Construct_weighted_circumcenter_3(); }

  Compute_power_product_3
  compute_power_product_3_object() const
  { return Compute_power_product_3(); }

  Compute_squared_radius_smallest_orthogonal_sphere_3
  compute_squared_radius_smallest_orthogonal_sphere_3_object() const
  { return Compute_squared_radius_smallest_orthogonal_sphere_3(); }

  Compute_critical_squared_radius_3
  compute_critical_squared_radius_3_object() const
  {return  Compute_critical_squared_radius_3(); }
};

// We need to introduce a "traits_base_3" class in order to get the
// specialization for Exact_predicates_inexact_constructions_kernel to work,
// otherwise there is a cycle in the derivation.
template < class K, class Weight = typename K::RT >
class Regular_triangulation_euclidean_traits_3
  : public Regular_triangulation_euclidean_traits_base_3<K, Weight>
{};

// Cartesian versions.
template < class pt, class Weight >
inline
Oriented_side
power_test_3(const Weighted_point<pt, Weight> &p,
           const Weighted_point<pt, Weight> &q,
           const Weighted_point<pt, Weight> &r,
           const Weighted_point<pt, Weight> &s,
           const Weighted_point<pt, Weight> &t,
	   Cartesian_tag)
{
    typedef typename pt::R::FT FT;
    return power_testC3(p.x(), p.y(), p.z(), FT(p.weight()),
                        q.x(), q.y(), q.z(), FT(q.weight()),
                        r.x(), r.y(), r.z(), FT(r.weight()),
                        s.x(), s.y(), s.z(), FT(s.weight()),
                        t.x(), t.y(), t.z(), FT(t.weight()));
}

template < class pt, class Weight >
inline
Oriented_side
power_test_3(const Weighted_point<pt, Weight> &p,
           const Weighted_point<pt, Weight> &q,
           const Weighted_point<pt, Weight> &r,
           const Weighted_point<pt, Weight> &t,
	   Cartesian_tag)
{
    typedef typename pt::R::FT FT;
    return power_testC3(p.x(), p.y(), p.z(), FT(p.weight()),
                        q.x(), q.y(), q.z(), FT(q.weight()),
                        r.x(), r.y(), r.z(), FT(r.weight()),
                        t.x(), t.y(), t.z(), FT(t.weight()));
}

template < class pt, class Weight >
inline
Oriented_side
power_test_3(const Weighted_point<pt, Weight> &p,
           const Weighted_point<pt, Weight> &q,
           const Weighted_point<pt, Weight> &t,
	   Cartesian_tag)
{
    typedef typename pt::R::FT FT;
    return power_testC3(p.x(), p.y(), p.z(), FT(p.weight()),
                        q.x(), q.y(), q.z(), FT(q.weight()),
                        t.x(), t.y(), t.z(), FT(t.weight()));
}

template < class pt, class Weight >
inline
Oriented_side
power_test_3(const Weighted_point<pt, Weight> &p,
           const Weighted_point<pt, Weight> &q,
	   Cartesian_tag)
{
    typedef typename pt::R::FT FT;
    return power_testC3(FT(p.weight()),
                        FT(q.weight()));
}


template < class pt, class Weight >
inline
Comparison_result
compare_power_distance_3 (const pt &p,
			  const Weighted_point<pt, Weight> &q,
			  const Weighted_point<pt, Weight> &r,
			  Cartesian_tag)
{
   typedef typename pt::R::FT FT;
   return compare_power_distanceC3(p.x(), p.y(), p.z(),
				   q.x(), q.y(), q.z(), FT(q.weight()),
				   r.x(), r.y(), r.z(), FT(r.weight()));
}     



// Homogeneous versions.
template < class pt, class Weight >
inline
Oriented_side
power_test_3(const Weighted_point<pt, Weight> &p,
           const Weighted_point<pt, Weight> &q,
           const Weighted_point<pt, Weight> &r,
           const Weighted_point<pt, Weight> &s,
           const Weighted_point<pt, Weight> &t,
	   Homogeneous_tag)
{
    typedef typename pt::R::RT RT;
    return power_testH3(p.hx(), p.hy(), p.hz(), p.hw(), RT(p.weight()),
                        q.hx(), q.hy(), q.hz(), q.hw(), RT(q.weight()),
                        r.hx(), r.hy(), r.hz(), r.hw(), RT(r.weight()),
                        s.hx(), s.hy(), s.hz(), s.hw(), RT(s.weight()),
                        t.hx(), t.hy(), t.hz(), t.hw(), RT(t.weight()));
}

// The followings call the cartesian version over FT, because an homogeneous
// special version would be boring to write.

template < class pt, class Weight >
inline
Oriented_side
power_test_3(const Weighted_point<pt, Weight> &p,
           const Weighted_point<pt, Weight> &q,
           const Weighted_point<pt, Weight> &r,
           const Weighted_point<pt, Weight> &t,
	   Homogeneous_tag)
{
    typedef typename pt::R::FT FT;
    return power_testC3(p.x(), p.y(), p.z(), FT(p.weight()),
                        q.x(), q.y(), q.z(), FT(q.weight()),
                        r.x(), r.y(), r.z(), FT(r.weight()),
                        t.x(), t.y(), t.z(), FT(t.weight()));
}

template < class pt, class Weight >
inline
Oriented_side
power_test_3(const Weighted_point<pt, Weight> &p,
           const Weighted_point<pt, Weight> &q,
           const Weighted_point<pt, Weight> &t,
	   Homogeneous_tag)
{
    typedef typename pt::R::FT FT;
    return power_testC3(p.x(), p.y(), p.z(), FT(p.weight()),
                        q.x(), q.y(), q.z(), FT(q.weight()),
                        t.x(), t.y(), t.z(), FT(t.weight()));
}

template < class pt, class Weight >
inline
Oriented_side
power_test_3(const Weighted_point<pt, Weight> &p,
           const Weighted_point<pt, Weight> &q,
	   Homogeneous_tag)
{
    typedef typename pt::R::FT FT;
    return power_testC3(FT(p.weight()),
                        FT(q.weight()));
}

template < class Point, class Weight >
inline
Comparison_result
compare_power_distance_3 (const Point &p,
			  const Weighted_point<Point, Weight> &q,
			  const Weighted_point<Point, Weight> &t,
			  Homogeneous_tag)
{
  typedef typename Point::R::FT FT;
  return compare_power_distanceC3(p.x(), p.y(), p.z(), FT(p.weight()),
				  q.x(), q.y(), q.z(), FT(q.weight()),
				  t.x(), t.y(), t.z(), FT(t.weight()));
}     

// Kludges for M$.

template < class pt, class Weight >
inline
Oriented_side
power_test_3(const Weighted_point<pt,Weight> &p,
	   const Weighted_point<pt,Weight> &q,
	   const Weighted_point<pt,Weight> &r,
	   const Weighted_point<pt,Weight> &s,
	   const Weighted_point<pt,Weight> &t)
{
  typedef typename pt::R::Rep_tag Tag;
  return power_test_3(p,q,r,s,t, Tag());
}

template < class pt, class Weight >
inline
Oriented_side
power_test_3(const Weighted_point<pt,Weight> &p,
	   const Weighted_point<pt,Weight> &q,
	   const Weighted_point<pt,Weight> &r,
	   const Weighted_point<pt,Weight> &t)
{
  typedef typename pt::R::Rep_tag Tag;
  return power_test_3(p,q,r,t, Tag());
}

template < class pt, class Weight >
inline
Oriented_side
power_test_3(const Weighted_point<pt,Weight> &p,
	   const Weighted_point<pt,Weight> &q,
	   const Weighted_point<pt,Weight> &t)
{
  typedef typename pt::R::Rep_tag Tag;
  return power_test_3(p,q,t, Tag());
}

template < class pt, class Weight >
inline
Oriented_side
power_test_3(const Weighted_point<pt,Weight> &p,
	   const Weighted_point<pt,Weight> &q)
{
  typedef typename pt::R::Rep_tag Tag;
  return power_test_3(p,q, Tag());
}

template < class Point, class Weight >
inline
Comparison_result
compare_power_distance_3 (const Point &p,
			  const Weighted_point<Point, Weight> &q,
			  const Weighted_point<Point, Weight> &r)
{
  typedef typename Point::R::Rep_tag Tag;
  return compare_power_distance_3(p,q,r, Tag());
}

CGAL_END_NAMESPACE

// Partial specialization for Filtered_kernel<CK>.

#include <CGAL/Regular_triangulation_filtered_traits_3.h>
#include <CGAL/Filtered_kernel.h>

CGAL_BEGIN_NAMESPACE

// This declaration is needed to break the cyclic dependency.
template < typename K >
class Regular_triangulation_filtered_traits_3;


template < typename CK, typename T >
class Regular_triangulation_euclidean_traits_3 < Filtered_kernel<CK>, T>
  : public Regular_triangulation_filtered_traits_3 < Filtered_kernel<CK> >
{
public:
  typedef Filtered_kernel<CK>  Kernel;
};

CGAL_END_NAMESPACE

#endif // CGAL_REGULAR_TRIANGULATION_EUCLIDEAN_TRAITS_3_H
