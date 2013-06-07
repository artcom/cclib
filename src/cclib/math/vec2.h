#ifndef __CCLIB_VEC2_INCLUDED__
#define __CCLIB_VEC2_INCLUDED__

#include <math.h>
#include <math/math.h>
#include <iostream>
#include <tr1/memory>

namespace cclib {

/**
 * A Vector2D is a point in a 2D system.
 * @author sebastianh 
 *
 */
template<typename T>
class Vector2 {
    
    public:
        typedef std::tr1::shared_ptr<Vector2<T> > Ptr;
        /**
         * Initializes a new vector by setting x and y to 0
         */
        Vector2(): _x((T)0), _y((T)0) {};
        
        /**
         * Initializes a new Vector by two coordinates
         * @param theX
         * @param theY
         */
        Vector2(T x, T y): _x(x), _y(y) 
        {};
        
        Vector2(const Vector2<T> & vec): _x(vec.x()), _y(vec.y()) 
        {};

        ~Vector2() {};

        /**
         * Sets the vector to the given vector
         * @param theVector3f
         */
	    void set(const Vector2<T> & vec) {
            _x = vec.x();
            _y = vec.y();
        }
        
        /**
         * Sets the coords of this vector to the given coords
         * @param theX double, new x coord of the vector
         * @param theY double, new y coord of the vector
         */
	    void set(const T &x, const T &y) {
            _x = x;
            _y = y;
        };
        
        /**
         * Sets the vector using values in the polar format.
         * @param theLength
         * @param theDirection
         */
        void setPolar(const double & length, const double & direction) {
		    _x = (length*cos(direction));
		    _y = (length*sin(direction));
        }
        
        /**
         * Sets x and y to zero
         */
        void setZero() {
            _x = static_cast<T>(0);
            _y = static_cast<T>(0);
        }
        
        /**
         * Compares the Vector and the given one
         * @param theVector Vector2f, vector to compare
         * @return boolean, true if both vectors are equal 
         */
        bool operator==(const Vector2<T> & vec) {
            return _x == vec.x() && _y == vec.y();
        }
        
        /**
         * Returns true if both x and y are zero
         * @return boolean, true if the vector is zero
         */
        bool equalsZero() {
            return _x == static_cast<T>(0) && _y == static_cast<T>(0);
        }
        
        /*/usr/local/lib/libboost_unit_test_framework-mt.dylib*
         * Calculates the length of the vector, the length is given by
         * the magnitude.
         * @return
         */
        double length() {
            return sqrt(lengthSquared());
        }
	
        /**
         * Returns the length of the vector as square. This often
         * prevents calculating the squareroot.
         * @return double, the length of the vector squared
         */
        T lengthSquared() {
            return _x * _x + _y * _y;
        }

        /**
         * Returns the dot product of two vectors. The dot
         * product is the cosinus of the angle between two
         * vectors
         * @param theVector, the other vector
         * @return double, dot product of two vectors 
         */
	    double dot(const Vector2<T> & vec) {
		    return static_cast<double>( _x * vec.x() + _y * vec.y() );
        }
        
        /**
         * Returns the cross product of a vector. The
         * cross product returns a vector standing vertical
         * on the given vector.
         * @param theVector the vector
         * @return the cross product
         */
        Vector2<T>::Ptr cross() {
            Vector2<T>::Ptr result = Vector2<T>::Ptr(new Vector2<T>(_y, -_x));
            return result;
        }
        
        /**
         * Sets a position randomly distributed inside a sphere of unit radius
         * centered at the origin.  Orientation will be random and length will range
         * between 0 and 1
         */
        void randomize() {
            _x = cclib::random<T>();
            _y = cclib::random<T>();
        }
        
        /**
         * Returns the distance between this vector and th one passed as a parameter
         * @param theVector Vector2f, the vector to get the distance to
         * @return double, the distance between the vector and the given one
         */
        double distance(const Vector2<T> & vec) {
            double dx = _x - vec.x();
            double dy = _y - vec.y();
            return sqrt(dx * dx + dy * dy);
        }
	
        /**
         * Returns the distance between this vector and th one passed as a parameter
         * @param theX the x coord of the vector to get the distance to
         * @param theY the x coord of the vector to get the distance to
         * @return double, the distance between the vector and the given one
         */
        double distance(const T & x, const T & y) {
            double dx = static_cast<double>(_x - x);
            double dy = static_cast<double>(_y - y);
            return sqrt(dx * dx + dy * dy);
        }	
        
        /**
         * unary minus operation for all coords of the vector
         */
        void negate() {
		    scale(-1);
        }

        /**
         * Adds the given vector to this vector, by adding the
         * x and y coordinates.
         * @param theVector Vector2f, vector to be added
         */
        Vector2<T>::Ptr operator+(const Vector2<T> & vec) {
            return Vector2<T>::Ptr( new Vector2<T>(_x + vec.x(), _y + vec.y()));
        }
	
        /**
         * Subtracts the given vector from this vector, by subtracting the
         * x and y coordinates.
         * @param theVector Vector2f, vector to subtract
         */
        Vector2<T>::Ptr operator-(const Vector2<T> & vec) {
            return Vector2<T>::Ptr( new Vector2<T>(_x - vec.x(), _y - vec.y()));
        }
        
        /**
         * Scales this vector with the given factor.
         * @param theScale double, factor to scale the vector
         */
        
        Vector2<T>::Ptr scale(double scale) {
            return Vector2<T>::Ptr( new Vector2<T>(_x * scale, _y * scale));
        }
	
        /**
         * Norms the vector to the length of 1
         *
         */
        void normalize() {
            double m = length();
            if (m != 0.0)
                scale(1.0 / m);
        }
	
        /**
         * Interpolates between this vector and the given vector
         * by a given blend value. The blend value has to be between 0
         * and 1. A blend value 0 would change nothing, a blend value 1
         * would set this vector to the given one.
         * @param blend double, blend value for interpolation
         * @param theVector Vector2f, other vector for interpolation
         */
        void interpolate(double blend, const Vector2<T> & vec) {
            _x = vec.x() + blend * (_x - vec.x());
    		_y = vec.y() + blend * (_y - vec.y());
        }

        // /**
        //  * clamps the length of a given vector to maxLength.  If the vector is 
        //  * shorter its value is returned unaltered, if the vector is longer
        //  * the value returned has length of maxLength and is parallel to the
        //  * original input.
        //  * @param theTreshhold double, maximum length to vector is set to.
        //  */
        // void truncate(double threshold);
           
        // /**
        //  * given a vector, return a vector perpendicular to it.
        //  */
        // Vector2<T> normal();
	
        // /**
        //  * Returns positive 1 if theVector is clockwise of this vector,
        //  * minus -1 if anti clockwise (Y axis pointing down, X axis to right)
        //  * @param theVector
        //  * @return
        //  */
        // int sign(Vector2<T> vec);   
        
        // /**
        //  * Given a normalized vector this method reflects the vector it
        //  * is operating upon. (like the path of a ball bouncing off a wall)
        //  * @param theVector
        //  */
        // void reflect(const Vector<T> & vec);
        // void rotate(double r);
        // double direction();
 
        const T x() const { 
            return _x;
        };

        const T y() const {
            return _y;
        };

    private:
        T _x;
        T _y;
};

typedef Vector2<float> Vector2f;
typedef Vector2<double> Vector2d;
typedef Vector2<int> Vector2i;
        
typedef std::tr1::shared_ptr<Vector2f> Vector2fPtr;
typedef std::tr1::shared_ptr<Vector2i> Vector2iPtr;
typedef std::tr1::shared_ptr<Vector2d> Vector2dPtr;
        

} // namespace cclib

#endif
