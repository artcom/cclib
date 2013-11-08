#ifndef __CCLIB_VEC3_INCLUDED__
#define __CCLIB_VEC3_INCLUDED__

#include <math.h>
#include <math/math.h>
#include <iostream>
#include <tr1/memory>
#include "simplexnoise.h"

namespace cclib {

/**
 * A Vector3D is a point in a 2D system.
 * @author sebastianh 
 *
 */
template<typename T>
class Vector3 {
    
    public:
        typedef std::tr1::shared_ptr<Vector3<T> > Ptr;
        
        /**
         * Initializes a new vector by setting x and y to 0
         */
        Vector3(): _x((T)0), _y((T)0), _z((T)0) {};
        
        /**
         * Initializes a new Vector by two coordinates
         * @param theX
         * @param theY
         */
        Vector3(T x, T y, T z): _x(x), _y(y), _z(z) 
        {};
        
        Vector3(const Vector3<T> & vec): _x(vec.x()), _y(vec.y()), _z(vec.z()) 
        {};

        ~Vector3() {};

        /**
         * Sets the vector to the given vector
         * @param theVector3f
         */
	    void set(const Vector3<T> & vec) {
            _x = vec.x();
            _y = vec.y();
            _z = vec.z();
        }
        
        /**
         * Sets the coords of this vector to the given coords
         * @param theX double, new x coord of the vector
         * @param theY double, new y coord of the vector
         */
	    void set(const T &x, const T &y, const T &z) {
            _x = x;
            _y = y;
            _z = z;
        };
        
        /**
         * Sets x and y to zero
         */
        void setZero() {
            _x = static_cast<T>(0);
            _y = static_cast<T>(0);
            _z = static_cast<T>(0);
        }
        
        /**
         * Compares the Vector and the given one
         * @param theVector Vector3f, vector to compare
         * @return boolean, true if both vectors are equal 
         */
        bool operator==(const Vector3<T> & vec) {
            return _x == vec.x() && _y == vec.y() && _z == vec.z();
        }
        
        /**
         * Returns true if both x and y are zero
         * @return boolean, true if the vector is zero
         */
        bool equalsZero() {
            return _x == static_cast<T>(0) && _y == static_cast<T>(0) && _z == static_cast<T>(0);
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
            return _x * _x + _y * _y + _z * _z;
        }

        /**
         * Returns the dot product of two vectors. The dot
         * product is the cosinus of the angle between two
         * vectors
         * @param theVector, the other vector
         * @return double, dot product of two vectors 
         */
	    double dot(const Vector3<T> & vec) {
		    return static_cast<double>( _x * vec.x() + _y * vec.y() + _z * vec.z());
        }
        
        /**
         * Returns the cross product of a vector. The
         * cross product returns a vector standing vertical
         * on the given vector.
         * @param theVector the vector
         * @return the cross product
         */
        // XXX
        // Vector3<T>::Ptr cross() {
        //     Vector3<T>::Ptr result = Vector3<T>::Ptr(new Vector3<T>(_y, -_x));
        //     return result;
        // }
        
        /**
         * Sets a position randomly distributed inside a sphere of unit radius
         * centered at the origin.  Orientation will be random and length will range
         * between 0 and 1
         */
        void randomize(double theMax=1.0) {
            _x = cclib::random<T>(theMax); 
            _y = cclib::random<T>(theMax); 
            _z = cclib::random<T>(theMax); 
        }
        
        /**
         * Returns the distance between this vector and th one passed as a parameter
         * @param theVector Vector3f, the vector to get the distance to
         * @return double, the distance between the vector and the given one
         */
        double distance(const Vector3<T> & vec) {
            double dx = _x - vec.x();
            double dy = _y - vec.y();
            double dz = _z - vec.z();
            return sqrt(dx * dx + dy * dy + dz * dz);
        }
	
        /**
         * Returns the distance between this vector and th one passed as a parameter
         * @param theX the x coord of the vector to get the distance to
         * @param theY the x coord of the vector to get the distance to
         * @return double, the distance between the vector and the given one
         */
        double distance(const T & x, const T & y, const T & z) {
            double dx = static_cast<double>(_x - x);
            double dy = static_cast<double>(_y - y);
            double dz = static_cast<double>(_z - z);
            return sqrt(dx * dx + dy * dy + dz * dz);
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
         * @param theVector Vector3f, vector to be added
         */
        Vector3<T> operator+(const Vector3<T> & vec) {
            return Vector3<T>(_x + vec.x(), _y + vec.y(), _z + vec.z());
        }
	
        /**
         * Subtracts the given vector from this vector, by subtracting the
         * x and y coordinates.
         * @param theVector Vector3f, vector to subtract
         */
        Vector3<T> operator-(const Vector3<T> & vec) {
            return  Vector3<T>(_x - vec.x(), _y - vec.y(), _z - vec.z());
        }
        
        /**
         * Scales this vector with the given factor.
         * @param theScale double, factor to scale the vector
         */
        
        Vector3<T> scale(double scale) {
            _x *= scale;
            _y *= scale;
            _z *= scale;
            return Vector3<T>(_x, _y, _z);
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
         * @param theVector Vector3f, other vector for interpolation
         */
        void interpolate(double blend, const Vector3<T> & vec) {
            _x = vec.x() + blend * (_x - vec.x());
    		_y = vec.y() + blend * (_y - vec.y());
    		_z = vec.z() + blend * (_z - vec.z());
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
        // Vector3<T> normal();
	
        // /**
        //  * Returns positive 1 if theVector is clockwise of this vector,
        //  * minus -1 if anti clockwise (Y axis pointing down, X axis to right)
        //  * @param theVector
        //  * @return
        //  */
        // int sign(Vector3<T> vec);   
        
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
        
        const T z() const {
            return _z;
        };
        
        void x(T x) { 
            _x = x;
        };

        void y(T y) {
            _y = y;
        };
        
        void z(T z) {
            _z = z;
        };

        // some generators
        static Vector3<T> 
        simplexNoise(float x, float y, float z, float ocatve, float persistence, float scale, float loBound=-1.0f, float hiBound=1.0f) {
            float px = scaled_octave_noise_3d(ocatve, persistence, scale, loBound, hiBound, x, 1+y, 3+z);
            float py = scaled_octave_noise_3d(ocatve, persistence, scale, loBound, hiBound, y, 2+z, 2+x);
            float pz = scaled_octave_noise_3d(ocatve, persistence, scale, loBound, hiBound, z, 3+x, 1+y);
            return Vector3<T>(px, py, pz);
        };


    private:
        T _x;
        T _y;
        T _z;
};

typedef Vector3<float> Vector3f;
typedef Vector3<double> Vector3d;
typedef Vector3<int> Vector3i;

typedef std::tr1::shared_ptr< Vector3f > Vector3fPtr;
typedef std::tr1::shared_ptr< Vector3d > Vector3dPtr;
typedef std::tr1::shared_ptr< Vector3i > Vector3iPtr;

} // namespace cclib

#endif
